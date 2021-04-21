/*
 * emulator_main.cpp
 *
 *  Created on: 9 Apr 2021
 *      Author: mathieu
 */

#include <x86intrin.h>
#include <iostream>
#include <ctime>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/ue_engine.hpp"

//Has to be declared in the main for some reason
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "PolyLoader.hpp"

#define WIDTH 800
#define HEIGHT 600
#define SCALE 1

bool antiAliasing = false;
float antiAliasingFactor = 1;

SDL_Event event;
SDL_Surface* surface;
SDL_Window* window;

ue::Renderer3D renderer;

//Prototypes
void drawToScreen();
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
ue::Matrix3 computeRotationMatrix(ue::Vector3 angles);
void drawText(SDL_Surface* screen, const char* string, uint8_t size, int x, int y);

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

  //********Load the objects********

  //Create the poly loader
  PolyLoader polyLoader;

  //Create the object list
  ue::Poly* objectList = new ue::Poly[1];

  //Load an obj file without texture
  ue::Vector3 cube = ue::Vector3(R(40.0), R(40.0), R(40.0));
  objectList[0] = polyLoader.loadFromObj("emulator/resource/phantom.obj", "emulator/resource/phantom_texture.png", cube);

  //Move the poly to the front of the camera
  objectList[0].position = objectList[0].position
      + ue::Vector3(R(0.0), R(-10.0), R(40.0));

  //Create a 32x32 red to green gradient texture for the particle
  ue::Texture gradTex;
  gradTex.height = 512;
  gradTex.width = 512;
  ue::Colour colArray[gradTex.height * gradTex.width];
  gradTex.pixel = colArray;

  //Generate the texture
  for(int x = 0; x < (int)gradTex.width; x++)
    {
      for(int y = 0; y < (int)gradTex.height; y++)
        {
          //Calculate the colour
          ue::Colour col;
          col.colour.b = (255*(x+y) / (gradTex.width+gradTex.height));
          col.colour.g = 255 - col.colour.b;
          col.colour.r = 0;

          //Calculate the alpha (make a circle)
          int ax = (x - gradTex.width/2);
          int ay = (y - gradTex.height/2);
          if(ax*ax + ay*ay < gradTex.width * gradTex.width / 6)
            col.colour.a = 200;
          else
            col.colour.a = 0;


          gradTex.pixel[x + y*(int)gradTex.width] = col;
        }
    }

  //Create the particle
  ue::Particle particle;
  particle.texture = gradTex;
  particle.size = ue::Vector2(R(20.0), R(20.0));
  particle.position = ue::Vector3(R(0.0), R(-10.0), R(40.0));


  //********configure the renderer********

  //Allocate memory for the buffers
  ue::Colour pixelArray[WIDTH*HEIGHT];
  ue::Real depthArray[WIDTH*HEIGHT];

  //Create the buffers
  ue::FrameBuffer frameBuffer = ue::FrameBuffer(WIDTH, HEIGHT, pixelArray);
  ue::DepthBuffer depthBuffer = ue::DepthBuffer(WIDTH, HEIGHT, depthArray);

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

  //Create the light
  ue::Vector3 lightVector = ue::Vector3(R(4.0),R(0.1),R(1.0));
  lightVector.normalise();
  ue::LightSun sun = ue::LightSun(lightVector, R(0.6));

  //Build the renderer TODO: make a constructor
  renderer = ue::Renderer3D();
  renderer.camera = camera;
  renderer.sun = sun;
  renderer.ambientLight = R(0.5);
  renderer.depthBuffer = depthBuffer;
  renderer.frameBuffer = frameBuffer;

  //********Configure the window********
  //Declare state vars
  bool quit = false;
  ue::Vector2 mouseDelta = ue::Vector2(R(0.0),R(0.0));
  SDL_Event event;

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

  //********main loop********
  while(!quit)
    {
      //SDL event
      while(SDL_PollEvent(&event))
        {
          switch (event.type)
          {
          case SDL_QUIT:
            std::cout << "Total frames : " << totalFrames << std::endl;
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
            //captureState = true;
            puts("Capture");
            break;
          case SDL_KEYDOWN:
            switch( event.key.keysym.sym )
            {
            case SDLK_ESCAPE:
              SDL_CaptureMouse((SDL_bool)false);
              SDL_SetRelativeMouseMode((SDL_bool)false);
              //captureState = false;
              puts("Uncapture");
              break;
            case SDLK_1:
              antiAliasing = !antiAliasing;
              break;
            case SDLK_UP:
              antiAliasingFactor *= 1/0.9f;
              break;
            case SDLK_DOWN:
              antiAliasingFactor *= 0.9f;
              break;
            default:
              break;
            }
            break;

          }
        }

      //Get the time before rendering
      gettimeofday(&tv, NULL);
      int64_t lastTime = tv.tv_usec + tv.tv_sec * 1000000;

      //Get the cycle count
      uint64_t lastCycleCount = __rdtsc();

      //Render
      //Clear the depth buffer and the frame buffer
      renderer.clearDepthBuffer();
      renderer.clearFrameBufferGrid((ue::Colour){0xFFFFFFFF},(ue::Colour){0xFFCFCFCF}, 12);

      //Works object by object
      renderer.renderObject(objectList[0]);
      renderer.renderParticle(particle);

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

      if(totalFrames == 1000)
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

      //Display the image
      drawToScreen();

      //Display the fps
      TTF_Init();
      char charBuffer[64];
      snprintf(charBuffer, sizeof(charBuffer), "Time : %.2fs", (double)totalTime / 1000000.0);
      drawText(surface, charBuffer, 12, 10, 10);
      snprintf(charBuffer, sizeof(charBuffer), "FPS : %.2f", fps);
      drawText(surface, charBuffer, 12, 10, 24);
      snprintf(charBuffer, sizeof(charBuffer), "Average FPS : %.2f", averageFps);
      drawText(surface, charBuffer, 12, 10, 38);
      snprintf(charBuffer, sizeof(charBuffer), "Average CPF : %.5e", averageCpf);
      drawText(surface, charBuffer, 12, 10, 52);
      TTF_Quit();

      SDL_UpdateWindowSurface(window);

      //Rotate the object
      ue::Matrix3 rotMat = computeRotationMatrix(ue::Vector3(R(0.0),R(0.01),R(0.0)));
      for(uint32_t i = 0; i < objectList[0].vertexCount; i++)
        {
          objectList[0].vertices[i] = rotMat * objectList[0].vertices[i];
        }

#if UE_CONFIG_ENABLE_NORMAL == true
      for(uint32_t i = 0; i < objectList[0].normalCount; i++)
        {
          objectList[0].normals[i] = rotMat * objectList[0].normals[i];
        }
#endif

    }


  return 0;
}

void drawToScreen()
{
  for(int i = 0; i < WIDTH; i++)
    {
      for(int j = 0; j < HEIGHT; j++)
        {
          for(int x = i*SCALE; x < (i+1)*SCALE; x++)
            {
              for(int y = j*SCALE; y < (j+1)*SCALE; y++)
                {
                  set_pixel(surface, x, y, renderer.frameBuffer[i+j*WIDTH].raw);
                }
            }
        }
    }
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
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

void drawText(SDL_Surface* screen, const char* string, uint8_t size, int x, int y)
{
  TTF_Font* font = TTF_OpenFont("emulator/resource/arial.ttf", size);

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
