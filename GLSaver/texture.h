#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <gl/gl.h>
#include <gl/glu.h>

/*This file is similar to the trigger class which manages it's own IDs*/
enum {TYPE_RING = 1, TYPE_STAR, TYPE_PLANET};

void LoadTextures();

int GetTextureCount();
int GetPlanetTextureCount();
int GetRingTextureCount();

int GetTextureID(int idx);
int GetPlanetTextureID(int idx);
int GetRingTextureID(int idx);

int GetTextureType(int idx);
int GetPlanetTextureType(int idx);
int GetRingTextureType(int idx);

int GetTextureIDFromType(int type);
GLUquadric * GetQuadricPointer(int idx);
#endif // TEXTURE_H_INCLUDED
