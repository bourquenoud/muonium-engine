/*
 * emulator_main.cpp
 *
 *  Created on: 9 Apr 2021
 *      Author: mathieu
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "engine/ue_engine.hpp"

//Has to be declared in the main for some reason
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "PolyLoader.hpp"

#define WIDTH 640
#define HEIGHT 480
#define SCALE 1

SDL_Event event;
SDL_Surface* surface;
SDL_Window* window;

ue::Renderer3D renderer;

bool drawBuffer = false;

//Prototypes
void drawToScreen();
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int emulator_main(void)
{
  //********configure the renderer********

  //Allocate memory for the buffers
  ue::Colour pixelArray[WIDTH*HEIGHT];
  ue::Real depthArray[WIDTH*HEIGHT];

  //Create the buffers
  ue::FrameBuffer frameBuffer = ue::FrameBuffer(WIDTH, HEIGHT, pixelArray);
  ue::DepthBuffer depthBuffer = ue::DepthBuffer(WIDTH, HEIGHT, depthArray);

  //Create the camera from the pinhole model
  ue::Camera camera = ue::Camera(
      6.223, //Super-8 camera format
      4.2164,
      WIDTH,
      HEIGHT,
      ue::FitResolutionGate::fill,
      1, //Near-clipping plane
      500 //Far-clipping plane
  );

  //Create the light
  ue::LightSun sun = ue::LightSun(ue::Vector3(R(1.0),R(1.0),R(0.0)), R(0.5));

  //Create the poly loader
  PolyLoader polyLoader;

  //Create the object list
  ue::Poly* objectList = new ue::Poly[1];

  //Load an obj file without texture
  ue::Vector3 monkeySize = ue::Vector3(R(20.0), R(20.0), R(20.0));
  objectList[0] = polyLoader.loadFromObj("/home/mathieu/Desktop/monkey.obj", NULL, monkeySize);

  //Move the poly to the front of the camera
  objectList[0].position = objectList[0].position + ue::Vector3(R(0.0), R(0.0), R(500.0));

  //Build the renderer TODO: make a constructor
  renderer = ue::Renderer3D();
  renderer.camera = camera;
  renderer.sun = sun;
  renderer.depthBuffer = depthBuffer;
  renderer.frameBuffer = frameBuffer;
  renderer.objectList = objectList;
  renderer.objectNumber = 1;

  //********Configure the window********
  //Declare state vars
  bool quit = false;
  //bool captureState = false;
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
            case SDLK_b:
              drawBuffer = !drawBuffer;
            default:
              break;
            }
            break;

          }
        }

      renderer.RenderFullFrame();
      drawToScreen();
      SDL_UpdateWindowSurface(window);
    }


  return 0;
}

ue::Real maxDepth = R(0.0);
ue::Real minDepth = R(1e20);
void drawToScreen()
{
  if(drawBuffer)
    {
      for(int i = 0; i < WIDTH; i++)
        {
          for(int j = 0; j < HEIGHT; j++)
            {
              if(renderer.depthBuffer[i+j*WIDTH] > maxDepth && renderer.depthBuffer[i+j*WIDTH] < renderer.camera.far)
                maxDepth = renderer.depthBuffer[i+j*WIDTH];
              if(renderer.depthBuffer[i+j*WIDTH] < minDepth)
                minDepth = renderer.depthBuffer[i+j*WIDTH];
            }
        }
    }
  for(int i = 0; i < WIDTH; i++)
    {
      for(int j = 0; j < HEIGHT; j++)
        {
          if(drawBuffer)
            {
              ue::Colour col;
              float depthCol = ((renderer.depthBuffer[i+j*WIDTH] - minDepth) / (maxDepth - minDepth)) * R(255.0);

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
                      set_pixel(surface, x, y, renderer.frameBuffer[i+j*WIDTH].raw);
                    }
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
