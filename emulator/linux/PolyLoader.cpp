/*
 * PolyLoader.cpp
 *
 *  Created on: 9 Apr 2021
 *      Author: mathieu
 */

#include "PolyLoader.hpp"

#define VERTEX "v "
#define TEXTURE "vt "
#define NORMAL "vn "
#define FACE "f "
#define OBJECT "o "
#define GROUP "g "
#define MATERIAL "usemtl "
#define COMMENT "#"

char * strtok_single_r (char *, char const *, char**);
ue::Texture loadTexture(const char* path);
__attribute((unused)) void printObject(ue::Poly obj);

ue::Poly PolyLoader::loadFromObj(const char* objectPath, const char* texturePath, ue::Vector3 size)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char** lines;
  uint32_t lineCount = 0;
  uint32_t vertexCount = 0;
  uint32_t normalCount = 0;
  uint32_t textureCount = 0;
  uint32_t faceCount = 0;

  ue::Vector3* vertices;
  ue::Vector3* normals;
  ue::Vector2* textures;
  ue::Triangle* faces;

  ue::Poly poly;

  puts("count lines\n");

  //Count the lines
  std::ifstream inFile(objectPath);
  uint32_t nLine = std::count(std::istreambuf_iterator<char>(inFile),
      std::istreambuf_iterator<char>(), '\n');

  lines = new char*[nLine];

  puts("reading\n");

  fp = fopen(objectPath, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
      lines[lineCount] = line;
      line = (char*)NULL;
      lineCount++;
  }

  //fclose(fp);

  puts("counting vertices\n");

  for(uint32_t i = 0; i < lineCount; i++)
    {
      //Count vertices
      if(strstr(lines[i], VERTEX))
        vertexCount++;

      //Count normals
      if(strstr(lines[i], NORMAL))
        normalCount++;

      //Count texture
      if(strstr(lines[i], TEXTURE))
        textureCount++;

      //Count faces
      if(strstr(lines[i], FACE))
        faceCount++;
    }

  //avoid using new with microcontrollers
  vertices = new ue::Vector3[vertexCount];
  normals = new ue::Vector3[normalCount];
  textures = new ue::Vector2[textureCount];
  faces = new ue::Triangle[faceCount];

  if(vertices == NULL || textures == NULL || faces == NULL)
    puts("Malloc error\n");

  printf("Vertices : %i\n Faces : %i\n", vertexCount, faceCount);

  vertexCount = 0;
  normalCount = 0;
  textureCount = 0;
  faceCount = 0;

  ue::Vector3 max = {UE_REAL_MIN, UE_REAL_MIN, UE_REAL_MIN};
  ue::Vector3 min = {UE_REAL_MAX, UE_REAL_MAX, UE_REAL_MAX};

  for(uint32_t i = 0; i < lineCount; i++)
    {
      if(strstr(lines[i], COMMENT)) //Skip if the line contains a #
        continue;

      //Get vertices
      if(strstr(lines[i], VERTEX))
        {
          ue::Vector3 vertex;

          // Extract the first token
          char * token = strtok(lines[i], " "); //First token is discarded
          token = strtok(NULL, " ");
          vertex.x = (ue::Real)atof(token);
          if(vertex.x > max.x)
            max.x = vertex.x;
          if(vertex.x < min.x)
            min.x = vertex.x;

          token = strtok(NULL, " ");
          vertex.y = (ue::Real)atof(token);
          if(vertex.y > max.y)
            max.y = vertex.y;
          if(vertex.y < min.y)
            min.y = vertex.y;

          token = strtok(NULL, " ");
          vertex.z = (ue::Real)atof(token);
          if(vertex.z > max.z)
            max.z = vertex.z;
          if(vertex.z < min.z)
            min.z = vertex.z;

          vertices[vertexCount++] = vertex;
        }

      //Get normals
      if(strstr(lines[i], NORMAL))
        {
          ue::Vector3 normal;

          // Extract the first token
          char * token = strtok(lines[i], " "); //First token is discarded
          token = strtok(NULL, " ");
          normal.x = (ue::Real)atof(token);
          if(normal.x > max.x)
            max.x = normal.x;
          if(normal.x < min.x)
            min.x = normal.x;

          token = strtok(NULL, " ");
          normal.y = (ue::Real)atof(token);
          if(normal.y > max.y)
            max.y = normal.y;
          if(normal.y < min.y)
            min.y = normal.y;

          token = strtok(NULL, " ");
          normal.z = (ue::Real)atof(token);
          if(normal.z > max.z)
            max.z = normal.z;
          if(normal.z < min.z)
            min.z = normal.z;

          normals[normalCount++] = normal;
        }

      //Get vertices
      if(strstr(lines[i], TEXTURE))
        {
          ue::Vector2 texture;

          // Extract the first token
          char * token = strtok(lines[i], " "); //First token is discarded
          token = strtok(NULL, " ");
          texture.x = (ue::Real)atof(token);

          token = strtok(NULL, " ");
          texture.y = (ue::Real)atof(token);

          textures[textureCount++] = texture;
        }

      //Count faces
      if(strstr(lines[i], FACE))
        {
          char* state1;
          char* state2;
          ue::Triangle triangle;

          char* block = strtok_r(lines[i], " ", &state1); //First token is discarded
          block = strtok_r(NULL, " ", &state1); // Second block is the vertex 1

          char* token = strtok_single_r(block, "/", &state2);
          triangle.va = &(vertices[atoi(token)-1]); //Vertex
          token = strtok_single_r(NULL, "/", &state2);
          //triangle.vta = &(textures[atoi(token)-1]); //Texture vertex
          token = strtok_single_r(NULL, "/", &state2);
          triangle.vna = &(normals[atoi(token)-1]); //Normal

          block = strtok_single_r(NULL, " ", &state1); //
          token = strtok_single_r(block, "/", &state2);
          triangle.vb = &(vertices[atoi(token)-1]);
          token = strtok_single_r(NULL, "/", &state2);
          //triangle.vtb = &(textures[atoi(token)-1]); //Texture vertex
          token = strtok_single_r(NULL, "/", &state2);
          triangle.vnb = &(normals[atoi(token)-1]); //Normal

          block = strtok_single_r(NULL, " ", &state1); //
          token = strtok_single_r(block, "/", &state2);
          triangle.vc = &(vertices[atoi(token)-1]);
          token = strtok_single_r(NULL, "/", &state2);
          //triangle.vtc = &(textures[atoi(token)-1]); //Texture vertex
          token = strtok_single_r(NULL, "/", &state2);
          triangle.vnc = &(normals[atoi(token)-1]); //Normal

          faces[faceCount++] = triangle;
        }
    }

  //Calculate the scale
  ue::Vector3 objDim =
      {
          ue::Real::abs(max.x - min.x),
          ue::Real::abs(max.y - min.y),
          ue::Real::abs(max.z - min.z)
      };

  ue::Vector3 scales =
      {
          size.x/objDim.x,
          size.y/objDim.y,
          size.z/objDim.z,
      };

  ue::Real scale = ue::Real::min(scales.x,scales.y,scales.z);

  //Scale the object
  for(uint32_t i = 0; i < vertexCount; i++)
    {
      vertices[i].x *= scale;
      vertices[i].y *= scale;
      vertices[i].z *= scale;
    }


  /*
  //Load the texture
  if(texturePath)
    {
      poly.texture = loadTexture(texturePath);
    }
  else
    {
      poly.texture = (ue::Texture){0,0,NULL}; //No texture
    }
    */


  poly.faceCount = faceCount;
  poly.vertexCount = vertexCount;
  //poly.textureCount = textureCount;
  poly.normalCount = normalCount;
  poly.normals = normals;
  poly.faces = faces;
  //poly.textureVerts = textures;
  poly.vertices = vertices;
  poly.position = ue::Vector3(R(0),R(0),R(0));

  for(uint32_t i = 0; i < lineCount; i++)
    {
      free(lines[i]);
    }
  delete[] lines;

  printf("Vertices : %i\n Faces : %i\n", poly.vertexCount, poly.faceCount);

  return poly;
}

ue::Texture loadTexture(const char* path)
{
  uint32_t imgPtr = 0;
  int width, height, channels;
  unsigned char *img = stbi_load(path, &width, &height, &channels, 4);
  if(img == NULL)
    {
      printf("Error in loading the image\n");
    }

  ue::Texture tex;
  tex.pixel = new ue::Colour[width * height];

  if(tex.pixel == NULL)
    {
      puts("Malloc error\n");
    }

  tex.height = height;
  tex.width = width;

  for(int32_t i = 0; i < width*height; i++)
    {
      tex.pixel[i].raw = 0x00000000;

      //Why the fuck is it in BGRA order ? is it ARGB ?
      if(channels > 0)
        tex.pixel[i].colour.b = img[imgPtr++];
      if(channels > 1)
        tex.pixel[i].colour.g = img[imgPtr++];
      if(channels > 2)
        tex.pixel[i].colour.r = img[imgPtr++];
      if(channels > 3)
        tex.pixel[i].colour.a = img[imgPtr++];
    }

  printf("Texture : (%i x %i x %i)\n", width, height, channels);

  return tex;
}

/*
void PolyLoader::printObject(ue::Poly obj)
{
  printf("********Vertices (%i)********\n", obj.vertexCount);
  for(uint32_t i = 0; i < obj.vertexCount; i++)
    {
      printf("\tv (%f;%f;%f)\n",
          (float)(obj.vertices[i].x),
          (float)(obj.vertices[i].y),
          (float)(obj.vertices[i].z));
    }

#if UE_CONFIG_ENABLE_TEXTURE == true
  printf("********Texture vertices (%i)********\n", obj.textureCount);
  for(uint32_t i = 0; i < obj.textureCount; i++)
    {
      printf("\tvt (%f;%f)\n",
          (float)(obj.textureVerts[i].x),
          (float)(obj.textureVerts[i].y));
    }
#endif

  //TODO this
#if UE_CONFIG_ENABLE_NORMAL == true
  printf("********Normals (%i)********\n", obj.textureCount);
  for(uint32_t i = 0; i < obj.textureCount; i++)
    {
      printf("\tvn (%f;%f)\n",
          (float)obj.textureVerts[i].x,
          (float)obj.textureVerts[i].y);
    }
#endif
}*/

char * strtok_single_r (char * str, char const * delims, char** src)
{
  //static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    *src = str;

  if (*src == NULL)
    return NULL;

  if ((p = strpbrk (*src, delims)) != NULL) {
    *p  = 0;
    ret = *src;
    *src = ++p;

  } else if (**src) {
    ret = *src;
    *src = NULL;
  }

  return ret;
}
