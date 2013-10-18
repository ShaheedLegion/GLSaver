#include "texture.h"
#include <gl/gl.h>
#include <stdio.h>
#define numtex 9
//totally arbitrary number.

GLuint _textures[numtex];
int _curr_tex = 0;

typedef struct tagtex
{
    int _w;
    int _h;
    int bpp;
    unsigned char * _data;
} _texture, *_lp_texture;

_lp_texture LoadTextureFile(char * path, int w, int h)
{
    FILE * _f = fopen(path, "r");
    if (_f == 0)
        return 0;

    _lp_texture _tex = (_lp_texture)malloc(1 * sizeof(_texture));

    //for now we will assume that the images are all the same size.
    //This is wasteful, but we will write a converter later that processes raw files to add
    //metadata to the files so that we can load the image size from the file itself.
    _tex->_w = w; //simply generate textures to match these params
    _tex->_h = h;
    _tex->bpp = 24;

    int datasize = ((_tex->_w * _tex->_h) * (_tex->bpp / 8)) * sizeof(unsigned char);
    _tex->_data = (unsigned char *)malloc(datasize);
    fread(_tex->_data, datasize, 1, _f);

    fclose(_f);
    return _tex;
}

int LoadTexture(char * tex, int w, int h)
{
    if (_curr_tex < numtex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h);    //load the texture here...
        if (_text == 0)
            return -1;

        // Generate and Bind The Texture
        glGenTextures(1, &_textures[_curr_tex]);
        glBindTexture(GL_TEXTURE_2D, _textures[_curr_tex]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
                     _text->_w, _text->_h,
                     0, GL_RGB, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_tex;
        _curr_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

LoadTextures()
{
    LoadTexture("nebula.raw", 256, 256);    //takes care of the texture binding
    LoadTexture("star-4point.raw", 16, 16);    //takes care of the texture binding
    LoadTexture("star-round.raw", 32, 32);    //takes care of the texture binding
    LoadTexture("sun.raw", 256, 256);    //takes care of the texture binding

    //the following textures require spherical mapping.
    LoadTexture("earth.raw", 720, 360);    //takes care of the texture binding
    LoadTexture("jupiter.raw", 720, 360);    //takes care of the texture binding
    LoadTexture("mars.raw", 720, 360);    //takes care of the texture binding
    LoadTexture("saturn.raw", 720, 360);    //takes care of the texture binding
    LoadTexture("neptune.raw", 720, 360);    //takes care of the texture binding
}

