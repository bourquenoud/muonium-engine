/*
 *
 */

#ifdef UNDEF

using namespace std;

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdbool>
#include <cstdint>
#include <cmath>
#include <ctime>
#include <limits.h>
#include <sys/time.h>

//Has to be declared in the main for some reason
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "engine/ue_engine.hpp"

#define lengthof(x) sizeof(x)/sizeof((x)[0])

//#define SIGN(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))

#define SCALE 1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS_LIMIT 500
#define FRAME_TIME_LIMIT  (1000000/FPS_LIMIT)

#define MAX_RENDER_DISTANCE 500
#define NEAR 1

#define MOVE_SPEED 30
#define ROT_SPEED 0.001

typedef struct
{
	uint32_t pixelDrawn;
	uint32_t triangleDrawn;
}stats_t;

SDL_Event event;
SDL_Surface* surface;
SDL_Window* window;

colour_t imageBuffer [WINDOW_WIDTH][WINDOW_HEIGHT];
float depthBuffer [WINDOW_WIDTH][WINDOW_HEIGHT];

vector3_t lightVector = {1,1,1};
float ambientLight = 0.3;
float lightIntensity = 0.5;


//Prototypes
void moveCamera(object_t* objList, uint32_t nObject, vector3_t dir);
vector3_t toCameraPerspective(vector3_t vert, vector3_t objCenter, smatrix3_t worldRotation);
float edgeFunction(vector3_t v0, vector3_t v1, vector3_t p);
float isInsideTriangle(triangle_t projTriangle, vector2_t point);
stats_t drawTriangle(triangle_t triangle, texture_t texture, vector3_t objCenter);
colour_t getTextureAt(vector2_t pos, texture_t texture);
float computeLight(triangle_t triangle);
vector3_t move(vector3_t point, vector3_t direction);
vector3_t moveInv(vector3_t point, vector3_t direction);
vector3_t rotateAroundAxis(vector3_t point, uint8_t axis, float angle);
stats_t renderAll(object_t* objList, uint32_t nObject);
stats_t render(object_t obj);
smatrix3_t computeRotationMatrix(vector3_t angles);
smatrix3_t computeInverseRotationMatrix(vector3_t angles);
void init(void);
void clearBuffers();
void drawToScreen();
void delay_ms(int t);
void delay_us(uint64_t t);
uint64_t timeSinceLast_us(uint64_t* ref);
void analysePerformance(uint64_t* ref, int32_t analysePerf, const char* txt);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
float fclamp(float a, float b, float val);

void drawSkybox();
colour_t sampleSkybox(vector2_t screenPos, smatrix3_t worldRotation);

void drawText(SDL_Surface* screen, const char* string, uint8_t size, int x, int y);
void drawStats(stats_t statsVal, uint64_t* timeRef);
void fog(float limitDistance, colour_t fogColour);

uint8_t captureState = 0;
uint8_t showFog = 0;
uint8_t drawBuffer = 0;
int8_t rotX = 0;
int8_t rotY = 0;
int8_t rotZ = 0;

vector3_t cameraRotation = {0,0,0};

uint64_t ref1, ref2, ref3;
int32_t cyc1, cyc2, cyc3;

object_t objects[50];

//TODO: quaternion and rotation stuff
//Main
int main(void)
{
	srand((unsigned) time(0));

	struct timeval tv;

	SDL_Event event;

	vector3_t buildingSize = {10,10,10};
	vector3_t monkeySize = {10,10,10};
	vector3_t planeSize = {10,10,10};

	objects[0] = object_t("/home/mathieu/Desktop/building_block.obj", "/home/mathieu/Desktop/building_texture.png", buildingSize);
	objects[1] = object_t("/home/mathieu/Desktop/monkey.obj", NULL, monkeySize);
	objects[2] = object_t("/home/mathieu/Desktop/phantom.obj", "/home/mathieu/Desktop/plane_texture.png", planeSize);

	objects[0].center = objects[0].center + (vector3_t){50,0,-50};
	objects[1].center = objects[1].center + (vector3_t){-50,0,-50};
	objects[2].center = objects[2].center + (vector3_t){0,0,-50};

	for(uint32_t i = 0; i < lengthof(objects)-3; i++)
	{
		objects[i+3] = object_t(objects[2]);
		objects[i+3].center = objects[i+3].center + (vector3_t){(float)(rand()%200 - 100),(float)(rand()%200 - 100),(float)(rand()%200 - 100)};
	}

	init();
	bool quit = false;

	timeSinceLast_us(&ref1);
	timeSinceLast_us(&ref3);

	gettimeofday(&tv, NULL);
	uint64_t lastTime = tv.tv_usec + tv.tv_sec * 1000000;

	//Normalise the light vector
	lightVector = lightVector.normalise();
	while (!quit)
	{
		float deltaTime = (float)timeSinceLast_us(&ref1) / 1000000.0f;
		stats_t stats = {0, 0};
		vector2_t mouseDelta = {0,0};

		vector3_t moveVector = {0,0,0};

		const Uint8 *keyState = SDL_GetKeyboardState(NULL);

		if(keyState[SDL_SCANCODE_W])
			moveVector.z = -MOVE_SPEED;
		if(keyState[SDL_SCANCODE_S])
			moveVector.z = MOVE_SPEED;
		if(keyState[SDL_SCANCODE_A])
			moveVector.x = -MOVE_SPEED;
		if(keyState[SDL_SCANCODE_D])
			moveVector.x = MOVE_SPEED;
		if(keyState[SDL_SCANCODE_SPACE])
			moveVector.y = MOVE_SPEED;
		if(keyState[SDL_SCANCODE_LSHIFT])
			moveVector.y = -MOVE_SPEED;


		//SDL event
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
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
				case SDLK_f:
					showFog = ~showFog;
				default:
					break;
				}
				break;

			}
		}

		objects[2].center = objects[2].center + (vector3_t){0,0,5*deltaTime};

		if(captureState)
		{
			cameraRotation.x += mouseDelta.y * ROT_SPEED;
			cameraRotation.y += mouseDelta.x * ROT_SPEED;
		}

		smatrix3_t invWorldRotation = computeInverseRotationMatrix(cameraRotation);
		moveVector = invWorldRotation * moveVector * deltaTime;
		moveCamera(objects, lengthof(objects), moveVector);

		clearBuffers();
		stats = renderAll(objects, lengthof(objects));
		if(showFog)
			fog(MAX_RENDER_DISTANCE, (colour_t){0xFF7F7F7FU});
		else
			drawSkybox(); //Skybox is hidden when putting fog

		drawToScreen();
		drawStats(stats, &ref3);
		SDL_UpdateWindowSurface(window);

		gettimeofday(&tv, NULL);
		int64_t elapsedTime = (tv.tv_usec + tv.tv_sec * 1000000) - lastTime;
		lastTime = tv.tv_usec + tv.tv_sec * 1000000;
		if(elapsedTime < FRAME_TIME_LIMIT)
			delay_us(FRAME_TIME_LIMIT - elapsedTime);
	}

	return 0;
}

stats_t renderAll(object_t* objList, uint32_t nObject)
{
	stats_t result = {0,0};
	for(uint32_t i = 0; i < nObject; i++)
	{
		stats_t val = {0,0};
		val = render(objList[i]);
		result.pixelDrawn += val.pixelDrawn;
		result.triangleDrawn += val.triangleDrawn;
	}

	return result;
}

stats_t render(object_t obj)
{
	stats_t result = {0,0};
	for(uint32_t i = 0; i < obj.faceCount; i++)
	{
		stats_t val = {0,0};
		val = drawTriangle(obj.faces[i], obj.texture, obj.center);
		result.pixelDrawn += val.pixelDrawn;
		result.triangleDrawn += val.triangleDrawn;
	}

	return result;
}

//TODO: Find a way to not process the same vertex multiple time (yes it's easy)
vector3_t toCameraPerspective(vector3_t vert, vector3_t objCenter, smatrix3_t worldRotation)
{
	if (vert.z == 0)
		return (vector3_t){NAN,NAN,NAN};

	//Move to the position
	vert = vert + objCenter;

	//Rotate
	vert = worldRotation * vert;

	//Project to screen
	vert = (vector3_t){	(1 + NEAR * vert.x / -vert.z) * 0.5f * WINDOW_WIDTH, (1 + NEAR * vert.y / vert.z) * 0.5f * WINDOW_WIDTH, -vert.z };


	return vert;
}

//P is the screen position in pixels, z the world z coordinate
vector3_t toWorldCoordinate(vector2_t p, float z, smatrix3_t worldRotation)
{
	vector3_t result;

	result = (vector3_t){-(2*p.x/WINDOW_WIDTH-1)*z/NEAR, (2*p.y/WINDOW_WIDTH-1)*z/NEAR, z};

	result = worldRotation * result;

	return result;
}

void drawSkybox()
{
	smatrix3_t worldRotation = computeInverseRotationMatrix(cameraRotation);
	vector2_t screenPos;
	for(screenPos.x = 0; screenPos.x < WINDOW_WIDTH; screenPos.x++)
	{
		for(screenPos.y = 0; screenPos.y < WINDOW_HEIGHT; screenPos.y++)
		{
			if(depthBuffer[(int)screenPos.x][(int)screenPos.y] > MAX_RENDER_DISTANCE)
				imageBuffer[(int)screenPos.x][(int)screenPos.y] = sampleSkybox(screenPos,worldRotation);
		}
	}
}

colour_t sampleSkybox(vector2_t screenPos, smatrix3_t worldRotation)
{

	const float spacing = 0.02f;
	colour_t result;

	vector3_t worldPos = toWorldCoordinate(screenPos,1,worldRotation);

	if(worldPos.y < -spacing)
		result.raw = 0xFF007FFF;
	else if(worldPos.y > spacing)
		result.raw = 0xFF755b1d;
	else
	{
		float val = (worldPos.y + spacing)/(2*spacing);
		result.colour.b = (1-val) * 0x00 + (val) * 0x75;
		result.colour.g = (1-val) * 0x7F + (val) * 0x5b;
		result.colour.r = (1-val) * 0xFF + (val) * 0x1b;
		result.colour.a = 0xFF;
	}

	return result;
}

boundingBox2_t computeBoundingBox(triangle_t face)
{
	boundingBox2_t box;

	box.topLeft.x = fmax(fmax(face.a->x,face.b->x),face.c->x);
	box.topLeft.y = fmax(fmax(face.a->y,face.b->y),face.c->y);
	box.bottomRight.x = fmin(fmin(face.a->x,face.b->x),face.c->x);
	box.bottomRight.y = fmin(fmin(face.a->y,face.b->y),face.c->y);

	return box;
}

//DEPRECATED
//Return the depth of the point, -1 if outside
float isInsideTriangle(triangle_t projTriangle, vector2_t point)
{
	vector3_t p = {point.x, point.y, 0};
	float w0, w1, w2, area;

	w0 = edgeFunction(*(projTriangle.a), *(projTriangle.b), p);
	w1 = edgeFunction(*(projTriangle.b), *(projTriangle.c), p);
	w2 = edgeFunction(*(projTriangle.c), *(projTriangle.a), p);

	if ((w0 >= 0 && w1 >= 0 && w2>= 0))
	{
		area = edgeFunction(*(projTriangle.a), *(projTriangle.b), *(projTriangle.c));
		w0 /= area; w1 /= area; w2 /= area;
		return fabs(w0 * projTriangle.c->z + w1 * projTriangle.a->z + w2 * projTriangle.b->z);
	}
	else
		return -1.0f;
}

float edgeFunction(vector3_t v0, vector3_t v1, vector3_t p)
{
	float val = (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
	return val;
}

//TODO: Make triangle partially outside the screen still visible
stats_t drawTriangle(triangle_t triangle, texture_t texture, vector3_t objCenter)
{
	stats_t stats = {0,0};

	//Generate the rotation matrix
	smatrix3_t worldRotation = computeRotationMatrix(cameraRotation);

	//Convert to camera perspective and compute the texture vertices correction
	vector3_t projVertexA, projVertexB, projVertexC;
	vector2_t projTextureA, projTextureB, projTextureC;
	triangle_t projTriangle = {&projVertexA, &projVertexB, &projVertexC, &projTextureA, &projTextureB, &projTextureC};

	//Project
	projVertexA = toCameraPerspective(*(triangle.a), objCenter, worldRotation);
	projVertexB = toCameraPerspective(*(triangle.b), objCenter, worldRotation);
	projVertexC = toCameraPerspective(*(triangle.c), objCenter, worldRotation);

	//Don't process triangles behind the camera clipping plane (z is reversed)
	if(projTriangle.a->z < 0 && projTriangle.b->z < 0 && projTriangle.c->z < 0)
		return (stats_t){0,0};

	//Don't process triangles that are too far
	if(projTriangle.a->z > MAX_RENDER_DISTANCE && projTriangle.b->z > MAX_RENDER_DISTANCE && projTriangle.c->z > MAX_RENDER_DISTANCE)
		return (stats_t){0,0};

	//Copy the texture vectors
	projTextureA = *(triangle.ta);
	projTextureB = *(triangle.tb);
	projTextureC = *(triangle.tc);

	//Compute the light
	float light = fclamp(0,1,computeLight(triangle));

	//Get the bounding box
	boundingBox2_t box = computeBoundingBox(projTriangle);

	//At least one point of the triangle is out of screen
	if (box.topLeft.x > WINDOW_WIDTH - 1 || box.bottomRight.x < 0 || box.topLeft.y > WINDOW_HEIGHT - 1 || box.bottomRight.y < 0)
		return (stats_t){0,0};

	box.bottomRight.x = fmax(0, box.bottomRight.x);
	box.bottomRight.y = fmax(0, box.bottomRight.y);
	box.topLeft.x = fmin(WINDOW_WIDTH, box.topLeft.x);
	box.topLeft.y = fmin(WINDOW_HEIGHT, box.topLeft.y);

	stats.triangleDrawn++;

	//Draw the triangle
	for(int x = box.bottomRight.x; x < box.topLeft.x; x++)
	{
		for(int y = box.bottomRight.y; y < box.topLeft.y; y++)
		{
			vector3_t p = {(float)x, (float)y, 0.0f};
			float w0, w1, w2, area, z;

			w0 = edgeFunction(*(projTriangle.a), *(projTriangle.b), p);
			w1 = edgeFunction(*(projTriangle.b), *(projTriangle.c), p);
			w2 = edgeFunction(*(projTriangle.c), *(projTriangle.a), p);

			//If the point is inside the triangle
			if ((w0 >= 0 && w1 >= 0 && w2>= 0))
			{
				//Calculate the area of the triangle
				area = edgeFunction(*(projTriangle.a), *(projTriangle.b), *(projTriangle.c));
				w0 /= area; w1 /= area; w2 /= area;

				//Get the Z position
				z = w0 * projTriangle.c->z + w1 * projTriangle.a->z + w2 * projTriangle.b->z;

				//If the depth is smaller, draw the triangle
				if(z > NEAR && z < depthBuffer[x][y])
				{

					colour_t colour;

					//Calculate the texture coordinate
					vector2_t texPos;
					texPos.x = (w0 * projTriangle.tc->x + w1 * projTriangle.ta->x + w2 * projTriangle.tb->x);
					texPos.y = (w0 * projTriangle.tc->y + w1 * projTriangle.ta->y + w2 * projTriangle.tb->y);

					colour = getTextureAt(texPos, texture);

					colour.colour.r *= light;
					colour.colour.g *= light;
					colour.colour.b *= light;
					stats.pixelDrawn++;

					depthBuffer[x][y] = z;
					imageBuffer[x][y] = colour;
				}
			}
		}
	}

	//return true if a pixel has been drawn
	return stats;
}

/*
 * Get the colour of a point from a texture (value from 0 to 1)
 */
colour_t getTextureAt(vector2_t pos, texture_t texture)
{

	//If no texture
	if(!texture.pixel || pos.x < 0 || pos.x > 1 || pos.y < 0 || pos.x > 1)
	{
		colour_t colour;
		//Purple missing texture (alternating purple and black grid)
		colour.raw = ((
				((int32_t)((pos.x)*32) & 1) ^
				((int32_t)((pos.y)*32) & 1))
				?0xFFdc00ffU:0xFF101010U);

		return colour;
	}

	uint32_t x = (uint32_t)((pos.x) * texture.width);
	uint32_t y = (uint32_t)((1 - pos.y) * texture.height);

	return texture.pixel[x + (texture.width * y)];
}

float computeLight(triangle_t triangle)
{
	float light;
	vector3_t a,b,s1;

	//Get the direction vector
	a = *triangle.b - *triangle.a;
	b = *triangle.c - *triangle.a;

	//Calculate the normal
	s1 = a.cross(b);

	//Normalise the vectors
	s1 = s1.normalise();

	//Calculate the dot product
	light = fmax(0,lightVector*s1); //Prevent negative light

	light *= lightIntensity;
	light += ambientLight;

	return light;
}

//***********************PHYSICS*********************************

vector3_t rotateAroundAxis(vector3_t point, uint8_t axis, float angle)
{
	float cosAlpha, sinAlpha;
	cosAlpha = cosf(angle);
	sinAlpha = sinf(angle);
	smatrix3_t rotMatrix ={{  {1, 0, 0},{0, 1, 0},{0, 0, 1}  }};

	if(axis == 1)
		rotMatrix =	(smatrix3_t){{  {1, 0 , 0 },{0, cosAlpha, -sinAlpha},{0, sinAlpha, cosAlpha }}};
	else if(axis == 2)
		rotMatrix =	(smatrix3_t){{  {cosAlpha , 0, sinAlpha},{0 ,1, 0},{-sinAlpha, 0, cosAlpha}}};
	else if(axis == 4)
		rotMatrix =	(smatrix3_t){{{cosAlpha, -sinAlpha, 0},{sinAlpha, cosAlpha , 0},{0, 0, 1}}};

	return rotMatrix * point;
}


smatrix3_t computeRotationMatrix(vector3_t angles)
{
	float cosAlpha = cosf(angles.x);
	float sinAlpha = sinf(angles.x);
	float cosBeta = cosf(angles.y);
	float sinBeta = sinf(angles.y);
	float cosGamma = cosf(angles.z);
	float sinGamma = sinf(angles.z);

	smatrix3_t A = {{{1, 0 , 0 },{0, cosAlpha, -sinAlpha},{0, sinAlpha, cosAlpha }}}; //X
	smatrix3_t B = {{{cosBeta , 0, sinBeta},{0 ,1, 0},{-sinBeta, 0, cosBeta}}}; //Y
	smatrix3_t C = {{{cosGamma, -sinGamma, 0},{sinGamma, cosGamma , 0},{0, 0, 1}}}; //Z

	return A*B*C;
}

smatrix3_t computeInverseRotationMatrix(vector3_t angles)
{
	angles = -angles;

	float cosAlpha = cosf(angles.x);
	float sinAlpha = sinf(angles.x);
	float cosBeta = cosf(angles.y);
	float sinBeta = sinf(angles.y);
	float cosGamma = cosf(angles.z);
	float sinGamma = sinf(angles.z);

	smatrix3_t A = {{{1, 0 , 0 },{0, cosAlpha, -sinAlpha},{0, sinAlpha, cosAlpha }}}; //X
	smatrix3_t B = {{{cosBeta , 0, sinBeta},{0 ,1, 0},{-sinBeta, 0, cosBeta}}}; //Y
	smatrix3_t C = {{{cosGamma, -sinGamma, 0},{sinGamma, cosGamma , 0},{0, 0, 1}}}; //Z

	return C*B*A;
}

void moveCamera(object_t* objList, uint32_t nObject, vector3_t dir)
{
	for(uint32_t i = 0; i < nObject; i++)
	{
		objList[i].center = objList[i].center - dir;
	}
}

//************************POSTPROCESS*************************************

//Desaturate depending of the distance
void fog(float limitDistance, colour_t fogColour)
{
	for(int i = 0; i < WINDOW_WIDTH; i++)
	{
		for(int j = 0; j < WINDOW_HEIGHT; j++)
		{
			colour_t pixel = imageBuffer[i][j];
			float d = fmin(depthBuffer[i][j], limitDistance)/limitDistance; //Remap from 0 to 1

			pixel.colour.r = d * fogColour.colour.r + (1-d) * pixel.colour.r;
			pixel.colour.g = d * fogColour.colour.g + (1-d) * pixel.colour.g;
			pixel.colour.b = d * fogColour.colour.b + (1-d) * pixel.colour.b;

			//DESATURATION
			/*colour_t pixel = imageBuffer[i][j];
			colour_t newPixel;
			float f = fmin(depthBuffer[i][j], limitDistance)/limitDistance; //Remap from 0 to 1
			float L = 0.3*pixel.colour.r + 0.6*pixel.colour.g + 0.1*pixel.colour.b;
			newPixel.colour.r = pixel.colour.r + f * (L - pixel.colour.r);
			newPixel.colour.g = pixel.colour.g + f * (L - pixel.colour.g);
			newPixel.colour.b = pixel.colour.b + f * (L - pixel.colour.b);*/

			imageBuffer[i][j] = pixel;
		}
	}
}

//************************EMULATION***************************************
//Initialise the window
void init(void)
{
	//Open the window and initialise
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("3D Rasteriser",0,0,WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE, 0);
	surface = SDL_GetWindowSurface(window);

	SDL_UpdateWindowSurface(window);
}

void clearBuffers()
{
	for(int i = 0; i < WINDOW_WIDTH; i++)
	{
		for(int j = 0; j < WINDOW_HEIGHT; j++)
		{
			imageBuffer[i][j].raw = 0xFFFFFFFF;
			depthBuffer[i][j] = 1e38; //Max float
		}
	}
}

float maxDepth = 0;
float minDepth = 1e20;
void drawToScreen()
{
	if(drawBuffer)
	{
		for(int i = 0; i < WINDOW_WIDTH; i++)
		{
			for(int j = 0; j < WINDOW_HEIGHT; j++)
			{
				if(depthBuffer[i][j] > maxDepth && depthBuffer[i][j] < MAX_RENDER_DISTANCE)
					maxDepth = depthBuffer[i][j];
				if(depthBuffer[i][j] < minDepth)
					minDepth = depthBuffer[i][j];
			}
		}
	}
	for(int i = 0; i < WINDOW_WIDTH; i++)
	{
		for(int j = 0; j < WINDOW_HEIGHT; j++)
		{
			if(drawBuffer)
			{
				colour_t col;
				float depthCol = ((depthBuffer[i][j] - minDepth) / (maxDepth - minDepth)) * 255;

				col.colour.r = 255 - depthCol;
				col.colour.g = 0;
				col.colour.b = depthCol;
				col.colour.a = 0xFF;

				for(int x = i*SCALE; x < (i+1)*SCALE; x++)
				{
					for(int y = j*SCALE; y < (j+1)*SCALE; y++)
					{
						set_pixel(surface, x, y, col.raw);
					}
				}
			}
			else
			{
				for(int x = i*SCALE; x < (i+1)*SCALE; x++)
				{
					for(int y = j*SCALE; y < (j+1)*SCALE; y++)
					{
						set_pixel(surface, x, y, imageBuffer[i][j].raw);
					}
				}
			}
		}
	}
}

void drawStats(stats_t statsVal, uint64_t* timeRef)
{
	static uint64_t totalTime = 0;
	static uint32_t frames = 0;
	static float fps = 0;

	char buffer[32];
	totalTime += timeSinceLast_us(timeRef);
	frames++;

	if (totalTime > 1000000)
	{
		fps = 1e6f * (float)frames / (float)totalTime;
		frames = 0;
		totalTime -= 1000000;
	}

	TTF_Init();
	snprintf(buffer, sizeof(buffer), "%.2f FPS", fps);
	drawText(surface, buffer, 12, 10, 10);
	snprintf(buffer, sizeof(buffer), "%i px", statsVal.pixelDrawn);
	drawText(surface, buffer, 12, 10, 22);
	snprintf(buffer, sizeof(buffer), "%i tri",statsVal.triangleDrawn);
	drawText(surface, buffer, 12, 10, 34);
	TTF_Quit();
}

uint64_t timeSinceLast_us(uint64_t* ref)
{
	struct timeval tv;
	uint64_t result;

	gettimeofday(&tv, NULL);

	result = (tv.tv_usec + tv.tv_sec * 1000000) - *ref;

	*ref = (tv.tv_usec + tv.tv_sec * 1000000);

	return result;
}

void analysePerformance(uint64_t* ref, int32_t analysePerf, const char* txt)
{
	if(analysePerf <= 0)
		return;
	uint64_t time = timeSinceLast_us(ref);
	printf("%s %.3f ms\n", txt, time/1000.0f);
	timeSinceLast_us(ref);
}

void delay_ms(int t)
{
	//Sleep for t ms
	struct timespec ts;
	ts.tv_sec = t / 1000;
	ts.tv_nsec = (t % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void delay_us(uint64_t t)
{
	//Sleep for t ms
	struct timespec ts;
	ts.tv_sec = t / 1000000;
	ts.tv_nsec = (t % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
			+ y * surface->pitch
			+ x * surface->format->BytesPerPixel);
	*target_pixel = pixel;
}

float fclamp(float a, float b, float val)
{
	return fmax(a, fmin(b, val));
}

void drawText(SDL_Surface* screen, const char* string, uint8_t size, int x, int y)
{
	TTF_Font* font = TTF_OpenFont("/usr/share/fonts/manual_install/arial.ttf", size);

	SDL_Color foregroundColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_Color backgroundColor =  { 0,0,0,0 };

	SDL_Surface* textSurface
	= TTF_RenderText_Shaded
	(font, string, foregroundColor, backgroundColor);

	SDL_Rect textLocation = { x, y, 0, 0 };

	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);

	SDL_FreeSurface(textSurface);

	TTF_CloseFont(font);
}
//*/

#endif //UNDEF
