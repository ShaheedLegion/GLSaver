#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

/*This file is similar to the trigger class which manages it's own IDs*/
enum {TYPE_NEB = 0, TYPE_STAR, TYPE_PLANET};

void LoadTextures();
int GetTextureCount();
int GetTextureID(int idx);
int GetTextureType(int idx);

int GetTextureIDFromType(int type);
#endif // TEXTURE_H_INCLUDED
