/*
 * emulator_main.cpp
 *
 *  Created on: 9 Apr 2021
 *      Author: mathieu
 */

#include <unistd.h>
#include <x86intrin.h>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/ue_engine.hpp"

//Has to be declared in the main for some reason
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "PolyLoader.hpp"

#define WIDTH 1280
#define HEIGHT 720
#define N_SPAN 12
#define SCALE 1

//Config for thread
#define THREADED true
#define THREAD_COUNT N_SPAN

//Force N_SPAN to be 1 if in full frame render mode
#if UE_CONFIG_FULL_FRAME_RENDER == true
#undef N_SPAN
#undef THREADED
#undef THREAD_COUNT
#define N_SPAN 1
#define THREADED false
#define THREAD_COUNT 1
#endif


bool antiAliasing = false;
float antiAliasingFactor = 1;

SDL_Event event;
SDL_Surface* surface;
SDL_Window* window;

ue::Renderer3D renderer;

ue::CubeMap skybox;

//Prototypes
void drawToScreen(const ue::Colour*);
void drawSpanToScreen(int, ue::Renderer3D&);
inline void set_pixel(SDL_Surface *surface, int x, int y, const Uint32 pixel);
ue::Matrix3 computeRotationMatrix(ue::Vector3 angles);
void drawText(SDL_Surface* screen, const char* string, int x, int y, TTF_Font* font);

//Custom function to sort the particles
bool compareParticleDistance(ue::Particle p1, ue::Particle p2);

#if THREADED == true

ue::Renderer3D renderers[THREAD_COUNT];
std::thread renderThreads[THREAD_COUNT];

//Thread
volatile bool th_startRender = false;
volatile bool th_stop = false;
volatile bool th_renderDone[THREAD_COUNT];

void renderThreaded(int threadNumber, const ue::Poly& obj)
{
	renderers[threadNumber].renderSpanIndex = threadNumber;
	while(!th_stop)
	{
		//Clear the done flag
		th_renderDone[threadNumber] = false;

		//Wait for the ready flag
		while(!th_startRender && !th_stop) usleep(10);
		if(th_stop)
			break;

		//**********Render the span**********

		//Clear the buffers
		renderers[threadNumber].clearDepthBuffer();
		//renderers[threadNumber].clearFrameBuffer((ue::Colour){0xFFCFCFCF});
		renderers[threadNumber].clearFrameBufferSkybox(skybox);

		//Render the object
		renderers[threadNumber].renderObject_old(obj);

		//**********Rendering done***********

		//Set the done flag
		th_renderDone[threadNumber] = true;

		//Wait for the ready flag to be toggled again
		while(th_startRender && !th_stop) usleep(10);
		if(th_stop)
			break;
	}
}

#endif

int emulator_main(void)
{

	//Declare the time struct
	timeval tv;
	int64_t elapsedTime = 0;
	int64_t totalTime = 0;
	int32_t frames = 0;
	uint32_t totalFrames = 0;
	double fps = 0;
	double averageFps = 0;
	double averageCpf = 0; //Cycle per frame

	uint64_t cycleCounter = 0;

	//Initialise the pseudo random generator
	srand(0); //Chosen by hitting my head on the numpad

	//********Load the objects********

	//Create the poly loader
	PolyLoader polyLoader;

	//Create the object list
	ue::Poly* objectList = new ue::Poly[1];

	//Load a skybox
	skybox = polyLoader.loadCubeMap("emulator/resource/Epic_BlueSunset_*.png");

	//Load an obj file with texture
	ue::Vector3 cube = ue::Vector3(R(40.0), R(40.0), R(40.0));
	objectList[0] = polyLoader.loadFromObj("emulator/resource/moon.obj", "emulator/resource/moon_texture.png", cube);

	//Move the poly to the front of the camera
	objectList[0].position = objectList[0].position
			+ ue::Vector3(R(0.0), R(0.0), R(0.0));

	objectList[0].rotation = ue::Quaternion(R(0.0), R(3.1415)/R(4.0), R(0.0));

	//Rotate the object
	/*ue::Matrix3 rotMat = computeRotationMatrix(ue::Vector3(R(0),R(3.1415),R(0)));
  rotMat = computeRotationMatrix(ue::Vector3(R(0),R(0),R(5*3.1415/4)))*rotMat;
  for(uint32_t i = 0; i < objectList[0].vertexCount; i++)
    {
      objectList[0].vertices[i] = rotMat * objectList[0].vertices[i];
    }*/

	//Load a smoke particle
	ue::Texture smokeTex = polyLoader.loadTexture("emulator/resource/smoke.png");
	smokeTex.premultiplyAlpha(R(0.5)); //Compute for premultiplied alpha,

	//********configure the renderer********

	//Create the camera from the pinhole model
	ue::Camera camera = ue::Camera(
			24.892,
			18.669,
			WIDTH,
			HEIGHT,
			ue::FitResolutionGate::fill,
			1, //Near-clipping plane
			128 //Far-clipping plane
	);

	camera.position = ue::Vector3(R(0.0), R(0.0), R(-40.0));
	camera.rotation = ue::Quaternion(R(0.0), R(0.0), R(0.0));

	//Create the light
	ue::Vector3 lightVector = ue::Vector3(R(4.0),R(0.1),R(1.0));
	lightVector.normalise();
	ue::LightSun sun = ue::LightSun(lightVector, R(0.4));

#if THREADED == true

	//Allocate memory for the buffers
	ue::Colour pixelArray[WIDTH*HEIGHT];
	ue::Real depthArray[WIDTH*HEIGHT];

	//Create the buffers
	ue::FrameBuffer frameBuffer[THREAD_COUNT];
	ue::DepthBuffer depthBuffer[THREAD_COUNT];

	//Build the renderers
	for(int i = 0; i < THREAD_COUNT; i++)
	{
		//Configure the buffers
		depthBuffer[i] = ue::DepthBuffer();
		depthBuffer[i].buffer = depthArray + (WIDTH*HEIGHT/THREAD_COUNT) * i; //Offset the starting point, use contiguous memory
		depthBuffer[i].width = WIDTH;
		depthBuffer[i].height = HEIGHT/THREAD_COUNT;

		frameBuffer[i] = ue::FrameBuffer();
		frameBuffer[i].buffer = pixelArray + (WIDTH*HEIGHT/THREAD_COUNT) * i; //Offset the starting point, use contiguous memory
		frameBuffer[i].width = WIDTH;
		frameBuffer[i].height = HEIGHT/THREAD_COUNT;

		//Build the renderers TODO: make a constructor
		renderers[i] = ue::Renderer3D();
		renderers[i].camera = &camera;
		renderers[i].sun = &sun;
		renderers[i].ambientLight = R(0.5);
		renderers[i].depthBuffer = depthBuffer[i];
		renderers[i].frameBuffer = frameBuffer[i];

		//Build and start the threads
		renderThreads[i] = std::thread(renderThreaded, i, objectList[0]);
	}


#else

	//Allocate memory for the buffers
	ue::Colour pixelArray[WIDTH*HEIGHT/N_SPAN];
	ue::Real depthArray[WIDTH*HEIGHT/N_SPAN];

	//Create the buffers
	ue::FrameBuffer frameBuffer = ue::FrameBuffer(WIDTH, HEIGHT/N_SPAN, pixelArray);
	ue::DepthBuffer depthBuffer = ue::DepthBuffer(WIDTH, HEIGHT/N_SPAN, depthArray);

	//Build the renderer TODO: make a constructor
	renderer = ue::Renderer3D();
	renderer.camera = &camera;
	renderer.sun = &sun;
	renderer.ambientLight = R(0.5);
	renderer.depthBuffer = depthBuffer;
	renderer.frameBuffer = frameBuffer;
#endif

	//********Configure the window********
	//Declare state vars
	bool quit = false;
	ue::Vector2 mouseDelta = ue::Vector2(R(0.0),R(0.0));
	SDL_Event event;
	bool captureState = false;

	//Open the window and initialise
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
			"3D Rasteriser",
			0,
			0,
			WIDTH * SCALE,
			HEIGHT * SCALE,
			0);
	surface = SDL_GetWindowSurface(window);
	SDL_UpdateWindowSurface(window);

	//Open the font
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("emulator/resource/arial.ttf", 13);

	int64_t lastTime;
	gettimeofday(&tv, NULL);
	lastTime = tv.tv_usec + tv.tv_sec * 1000000;

	//********main loop********
	while(!quit)
	{

		ue::Vector3 moveVector = ue::Vector3::ZERO;

		//SDL event
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				std::cout << "Total frames : " << totalFrames << std::endl;
				TTF_CloseFont(font);
				TTF_Quit();
				font = NULL;

#if THREADED == true
				th_stop = true;
				usleep(10000);
				for(int i = 0; i < THREAD_COUNT; i++) renderThreads[i].join(); //Wait for all threads
#endif

				puts("Exit\n");
				quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouseDelta.x += event.motion.xrel;
				mouseDelta.y += event.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_CaptureMouse((SDL_bool)true);
				SDL_SetRelativeMouseMode((SDL_bool)true);
				captureState = true;
				puts("Capture");
				break;
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym )
				{
				case SDLK_ESCAPE:
					SDL_CaptureMouse((SDL_bool)false);
					SDL_SetRelativeMouseMode((SDL_bool)false);
					captureState = false;
					puts("Uncapture");
					break;
				default:
					break;
				}
				break;

			}
		}

		if(quit)
			break;

		//Read the keyboard
		SDL_PumpEvents();
		const uint8_t* keyboard = SDL_GetKeyboardState(NULL);

		if(keyboard[SDL_SCANCODE_W])
			moveVector = moveVector + ue::Vector3::Z;
		if(keyboard[SDL_SCANCODE_A])
			moveVector = moveVector + ue::Vector3::X;
		if(keyboard[SDL_SCANCODE_S])
			moveVector = moveVector - ue::Vector3::Z;
		if(keyboard[SDL_SCANCODE_D])
			moveVector = moveVector - ue::Vector3::X;
		if(keyboard[SDL_SCANCODE_SPACE])
			moveVector = moveVector + ue::Vector3::Y;
		if(keyboard[SDL_SCANCODE_LSHIFT])
			moveVector = moveVector - ue::Vector3::Y;


		//Get the time before rendering
		gettimeofday(&tv, NULL);

		//Compute the delta time
		int64_t deltaTime = (tv.tv_usec + tv.tv_sec * 1000000) - lastTime;

		lastTime = tv.tv_usec + tv.tv_sec * 1000000;

		//Get the cycle count
		uint64_t lastCycleCount = __rdtsc();

#if UE_CONFIG_FULL_FRAME_RENDER == true
		//Render
		//Clear the depth buffer and the frame buffer
		renderer.clearDepthBuffer();
		//renderer.clearFrameBufferGrid((ue::Colour){0xFFFFFFFF},(ue::Colour){0xFFCFCFCF}, 20);
		renderer.clearFrameBufferSkybox(skybox);
		//renderer.clearFrameBuffer((ue::Colour){0xFFCFCFCF});

		//Render the object
		renderer.renderObject_old(objectList[0]);

#elif THREADED == true
		//Start to render
		th_startRender = true;
		usleep(100); //Wait a little

		//Wait until all thread have rendered
		bool allDone = false;
		while(!allDone)
		{
			allDone = true;
			for(int i = 0; i < THREAD_COUNT; i++)
			{
				if(!th_renderDone[i]) //At least one isn't done
				{
					allDone = false;
					break;
				}
			}
			usleep(100);
		}

		//Reset the start flag
		th_startRender = false;

		//Draw to screen
		drawToScreen(pixelArray);

#else

		//Start the threads
		for(int i = 0; i < N_SPAN; i++)
		{
			renderer.renderSpanIndex = i;
			renderer.clearDepthBuffer();
			renderer.clearFrameBuffer((ue::Colour){0xFFCFCFCF});
			renderer.renderObject_old(objectList[0]);
			drawSpanToScreen(i, renderer);

		}
#endif

		//Compute the cycles spent
		cycleCounter += __rdtsc() - lastCycleCount;

		//Compute the FPS
		frames++;
		totalFrames++;
		gettimeofday(&tv, NULL);
		elapsedTime += (tv.tv_usec + tv.tv_sec * 1000000) - lastTime;
		totalTime += (tv.tv_usec + tv.tv_sec * 1000000) - lastTime;
		if(elapsedTime >= 1000000)
		{
			fps = 1000000.0 * frames / elapsedTime;
			averageFps = 1000000.0 * totalFrames / totalTime;
			frames = 0;
			elapsedTime = 0;

			averageCpf = (double)cycleCounter / totalFrames;
		}

		if(totalFrames == 100)
		{
			printf(
					"*************************\n"
					"Stats :\n"
					"\tFrames -> 1'00\n"
					"\tFPS -> %.2f\n"
					"\tCPF -> %.5e\n"
					"\tTime -> %.2f\n"
					"*************************\n",
					1000000.0 * totalFrames / totalTime,
					(double)cycleCounter / totalFrames,
					(double)totalTime / 1000000.0);
		}
		else if(totalFrames == 1000)
		{
			printf(
					"*************************\n"
					"Stats :\n"
					"\tFrames -> 1'000\n"
					"\tFPS -> %.2f\n"
					"\tCPF -> %.5e\n"
					"\tTime -> %.2f\n"
					"*************************\n",
					1000000.0 * totalFrames / totalTime,
					(double)cycleCounter / totalFrames,
					(double)totalTime / 1000000.0);
		}
		else if(totalFrames == 10000)
		{
			printf(
					"*************************\n"
					"Stats :\n"
					"\tFrames -> 10'000\n"
					"\tFPS -> %.2f\n"
					"\tCPF -> %.5e\n"
					"\tTime -> %.2f\n"
					"*************************\n",
					1000000.0 * totalFrames / totalTime,
					(double)cycleCounter / totalFrames,
					(double)totalTime / 1000000.0);
		}

		//Span screen drawing is done in the render loop
#if UE_CONFIG_FULL_FRAME_RENDER == true
		//Display the image
		drawToScreen(renderer.frameBuffer.buffer);
#endif

		//Display the fps
		char charBuffer[128];
		snprintf(charBuffer, sizeof(charBuffer), "Time : %.2fs", (double)totalTime / 1000000.0);
		drawText(surface, charBuffer, 10, 10, font);
		snprintf(charBuffer, sizeof(charBuffer), "FPS : %.2f", fps);
		drawText(surface, charBuffer, 10, 24, font);
		snprintf(charBuffer, sizeof(charBuffer), "Average FPS : %.2f", averageFps);
		drawText(surface, charBuffer, 10, 38, font);
		snprintf(charBuffer, sizeof(charBuffer), "Average CPF : %.5e", averageCpf);
		drawText(surface, charBuffer, 10, 52, font);

		SDL_UpdateWindowSurface(window);

		//Move the object according to mouse motion

		if(captureState)
		{
			//Compute the rotation from the mouse
			ue::Quaternion rotQuat(R(1.0),R(0.0),R(0.0),R(0.0));
			rotQuat = rotQuat * ue::Quaternion(ue::Vector3::Y, mouseDelta.x * R(0.001));

			ue::Vector3 rotVec = rotQuat.rotationMatrix3() * ue::Vector3::Z;

			rotQuat = rotQuat * ue::Quaternion(rotVec, mouseDelta.y * R(0.001));

			//Compute the forward vector
			moveVector = camera.rotation.rotationMatrix3() * moveVector;

			//rotQuat = rotQuat * ((float)deltaTime / 1000000.0f);
			/*ue::Vector3 moveVector = ue::Vector3::Y;
      renderer.camera.position = renderer.camera.position + moveVector * (ue::Real)(deltaTime/1000000.0f * 10.0f);*/

			//objectList[0].rotation = objectList[0].rotation * rotQuat;
			camera.rotation = camera.rotation * rotQuat;
			camera.position = camera.position + moveVector * (ue::Real)(deltaTime/1000000.0f * 100.0f);
		}

		/*
      //Rotate the object
      ue::Matrix3 rotMat = computeRotationMatrix(ue::Vector3(R(0.0),R(0.01),R(0.0)));
      for(uint32_t i = 0; i < objectList[0].vertexCount; i++)
        {
          objectList[0].vertices[i] = rotMat * objectList[0].vertices[i];
        }

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
      for(uint32_t i = 0; i < objectList[0].normalCount; i++)
        {
          objectList[0].normals[i] = rotMat * objectList[0].normals[i];
        }
#endif*/

		mouseDelta = ue::Vector2::ZERO;

	}


	return 0;
}

void drawToScreen(const ue::Colour* buff)
{
	for(int j = 0; j < HEIGHT; j++)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			for(int x = i*SCALE; x < (i+1)*SCALE; x++)
			{
				for(int y = j*SCALE; y < (j+1)*SCALE; y++)
				{
					set_pixel(surface, x, y, buff[i+j*WIDTH].raw);
				}
			}
		}
	}
}

void drawSpanToScreen(int spanIndex, ue::Renderer3D& r)
{
	for(int j = 0; j < HEIGHT/N_SPAN; j++)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			for(int x = i*SCALE; x < (i+1)*SCALE; x++)
			{
				for(int y = j*SCALE; y < (j+1)*SCALE; y++)
				{
					set_pixel(surface, x, y + HEIGHT/N_SPAN * spanIndex, r.frameBuffer[i+j*WIDTH].raw);
				}
			}
		}
	}
}

inline void set_pixel(SDL_Surface *surface, int x, int y, const Uint32 pixel)
{
	Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
			+ y * surface->pitch
			+ x * surface->format->BytesPerPixel);
	*target_pixel = pixel;
}

ue::Matrix3 computeRotationMatrix(ue::Vector3 angles)
{
	ue::Real cosAlpha = cosf((float)angles.x);
	ue::Real sinAlpha = sinf((float)angles.x);
	ue::Real cosBeta = cosf((float)angles.y);
	ue::Real sinBeta = sinf((float)angles.y);
	ue::Real cosGamma = cosf((float)angles.z);
	ue::Real sinGamma = sinf((float)angles.z);

	ue::Matrix3 A = {{{R(1.0), R(0.0) , R(0.0) },{R(0.0), cosAlpha, -sinAlpha},{R(0.0), sinAlpha, cosAlpha }}}; //X
	ue::Matrix3 B = {{{cosBeta , R(0.0), sinBeta},{R(0.0) ,R(1.0), R(0.0)},{-sinBeta, R(0.0), cosBeta}}}; //Y
	ue::Matrix3 C = {{{cosGamma, -sinGamma, R(0.0)},{sinGamma, cosGamma , R(0.0)},{R(0.0), R(0.0), R(1.0)}}}; //Z

	return A*B*C;
}

void drawText(SDL_Surface* screen, const char* string, int x, int y, TTF_Font* font)
{

	SDL_Color foregroundColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_Color backgroundColor =  { 0,0,0,0 };

	SDL_Surface* textSurface
	= TTF_RenderText_Shaded
	(font, string, foregroundColor, backgroundColor);

	SDL_Rect textLocation = { x, y, 0, 0 };

	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);

	SDL_FreeSurface(textSurface);
}



//Custom function to sort the particles
bool compareParticleDistance(ue::Particle p1, ue::Particle p2)
{
	return (p1.position.z > p2.position.z);
}
