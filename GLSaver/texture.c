#include "texture.h"
#include <stdio.h>
#include "logger.h"
#define numtex 3
//totally arbitrary number.

GLuint _textures[numtex];
GLuint _planets[numtex];
GLUquadric *_planetquads[numtex];

int _types[numtex];
int _planet_types[numtex];
int _curr_tex = 0;
int _curr_planet_tex = 0;

typedef struct tagtex
{
    int _w;
    int _h;
    int bpp;
    unsigned char * _data;
} _texture, *_lp_texture;

_lp_texture LoadTextureFile(char * path, int w, int h, int bpp)
{
    FILE * _f = fopen(path, "rb");
    if (_f == 0)
        return 0;

    _lp_texture _tex = (_lp_texture)malloc(1 * sizeof(_texture));

    //for now we will assume that the images are all the same size.
    //This is wasteful, but we will write a converter later that processes raw files to add
    //metadata to the files so that we can load the image size from the file itself.
    _tex->_w = w; //simply generate textures to match these params
    _tex->_h = h;
    _tex->bpp = bpp;

    int dataSize = ((_tex->_w * _tex->_h) * (bpp / 8));// * sizeof(unsigned char);
    //LogI("Texture w:", _tex->_w);
    //LogI("Texture h:", _tex->_h);
    //LogI("Data Size:", dataSize);
    _tex->_data = (unsigned char *)malloc(dataSize);
    size_t read = fread(_tex->_data, 1, dataSize, _f);
    //LogI("Data Read:", read);
    fclose(_f);
    return _tex;
}


void SaveTexture(_lp_texture tex, int idx, int bpp)
{
    char _buf[256];
    sprintf(_buf, "texture%d.raw", idx);
    FILE * _f = fopen(_buf, "wb");

    if (_f != 0)
    {
        int dataSize = ((tex->_w * tex->_h) * (bpp / 8));// * sizeof(unsigned char);
        fwrite(tex->_data, 1, dataSize, _f);
        fflush(_f);
        fclose(_f);
    }
}


int LoadStarTexture(char * tex, int w, int h)
{
    if (_curr_tex < numtex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 32);    //load the texture here...
        if (_text == 0)
            return -1;

        // set the texture type
        _types[_curr_tex] = TYPE_STAR;
        // Generate and Bind The Texture
        glGenTextures(1, &_textures[_curr_tex]);
        glBindTexture(GL_TEXTURE_2D, _textures[_curr_tex]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     _text->_w, _text->_h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_tex;
        //SaveTexture(_text, _retval);
        _curr_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        _text->_data = 0;
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

int LoadPlanetTexture(char * tex, int w, int h)
{
    if (_curr_planet_tex < numtex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 24);    //load the texture here...
        if (_text == 0)
            return -1;

        // set the texture type
        _planet_types[_curr_planet_tex] = TYPE_PLANET;
        _planetquads[_curr_planet_tex] = gluNewQuadric();

        gluQuadricNormals(_planetquads[_curr_planet_tex], GLU_SMOOTH);

        // Generate and Bind The Texture
        glGenTextures(1, &_planets[_curr_planet_tex]);
        glBindTexture(GL_TEXTURE_2D, _planets[_curr_planet_tex]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                     _text->_w, _text->_h,
                     0, GL_RGB, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_planet_tex;
        //SaveTexture(_text, _retval, 24);
        _curr_planet_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        _text->_data = 0;
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

void LoadTextures()
{
    memset(_textures, 0, numtex * sizeof(GLuint));
    memset(_planets, 0, numtex * sizeof(GLuint));
    memset(_types, 0, numtex * sizeof(int));
    memset(_planet_types, 0, numtex * sizeof(int));
    memset(_planetquads, 0, numtex * sizeof(GLUquadric*));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    // Specify a global ambient
    GLfloat globalAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );

    LoadStarTexture("Textures/star-blue.raw", 256, 256);    //takes care of the texture binding
    LoadStarTexture("Textures/star-green.raw", 256, 256);    //takes care of the texture binding
    LoadStarTexture("Textures/star-yellow.raw", 256, 256);    //takes care of the texture binding

    //the following textures require spherical mapping.
    LoadPlanetTexture("Textures/earth.raw", 1024, 512);    //takes care of the texture binding
    LoadPlanetTexture("Textures/jupiter.raw", 1024, 512);    //takes care of the texture binding
    LoadPlanetTexture("Textures/mars.raw", 1024, 512);    //takes care of the texture binding
    //LoadPlanetTexture("Textures/saturn.raw", 720, 360, TYPE_PLANET);    //takes care of the texture binding
    //LoadPlanetTexture("Textures/neptune.raw", 720, 360, TYPE_PLANET);    //takes care of the texture binding
}

int GetTextureID(int idx)
{
    if (idx >= 0 && idx < numtex)
    {
        return _textures[idx];
    }
    return -1;
}

int GetPlanetTextureID(int idx)
{
    if (idx >= 0 && idx < numtex)
    {
        return _planets[idx];
    }
    return -1;
}

int GetTextureType(int idx)
{
    if (idx >= 0 && idx < numtex)
    {
        return _types[idx];
    }
    return -1;
}

int GetPlanetTextureType(int idx)
{
    if (idx >= 0 && idx < numtex)
    {
        return _planet_types[idx];
    }
    return -1;
}

GLUquadric * GetQuadricPointer(int idx)
{
    if (idx >= 0 && idx < numtex)
    {
        return _planetquads[idx];
    }
    return -1;
}

int GetTextureCount()
{
    return numtex;
}

int GetPlanetTextureCount()
{
    return numtex;
}
