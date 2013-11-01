#include "texture.h"
#include <stdio.h>
#include "logger.h"
#include "noise.h"

#define numstartex 4
#define numplanettex 3
#define numringtex 1
#define numnebtex 1
//totally arbitrary number.

GLuint _star_textures[numstartex];
GLuint _planet_textures[numplanettex];
GLuint _ring_textures[numringtex];
GLuint _neb_textures[numnebtex];
GLUquadric *_planetquads[numplanettex];

int _types[numstartex];
int _planet_types[numplanettex];
int _ring_types[numringtex];
int _neb_types[numnebtex];
int _curr_tex = 0;
int _curr_planet_tex = 0;
int _curr_ring_tex = 0;
int _curr_neb_tex = 0;

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
    fread(_tex->_data, 1, dataSize, _f);
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
    if (_curr_tex < numstartex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 32);    //load the texture here...
        if (_text == 0)
        {
            LogI("Could not load star texture ", 0);
            LogI(tex, 1);
            return -1;
        }

        // set the texture type
        _types[_curr_tex] = TYPE_STAR;
        // Generate and Bind The Texture
        glBindTexture(GL_TEXTURE_2D, _star_textures[_curr_tex]);
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

int LoadNebulaTexture(char * tex, int w, int h)
{
    if (_curr_neb_tex < numnebtex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 32);    //load the texture here...
        if (_text == 0)
        {
            LogI("Could not load nebula texture ", 0);
            LogI(tex, 1);
            return -1;
        }

        // set the texture type
        _neb_types[_curr_neb_tex] = TYPE_NEBULA;
        // Generate and Bind The Texture
        glBindTexture(GL_TEXTURE_2D, _neb_textures[_curr_neb_tex]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     _text->_w, _text->_h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_neb_tex;
        //SaveTexture(_text, _retval);
        _curr_neb_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        _text->_data = 0;
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

int GenerateNebulaTexture(int w, int h, int age)
{
    if (_curr_neb_tex < numnebtex)
    {
        _lp_texture _text = (_lp_texture)malloc(1 * sizeof(_texture));

        _text->_w = w; //simply generate textures to match these params
        _text->_h = h;
        _text->bpp = 32;

        int dataSize = ((_text->_w * _text->_h) * (_text->bpp / 8));// * sizeof(unsigned char);
        _text->_data = (unsigned char *)malloc(dataSize);//LoadTextureFile(tex, w, h, 32);    //load the texture here...

        /// <param name="cover">Nebula cover from 0 (no nebula) to 255 (all nebula). Default is 150</param>
        /// <param name="sharpness">Nebula sharpness (color gradient) from 0 to 1; Default 0.985</param>
        /// <param name="time">Nebula age, default is 9; Use different values for different nebulas</param>
        CreateNebula(_text->_data, w, h, 150, 0.985, age);

        // set the texture type
        _neb_types[_curr_neb_tex] = TYPE_NEBULA;
        // Generate and Bind The Texture
        glBindTexture(GL_TEXTURE_2D, _neb_textures[_curr_neb_tex]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     _text->_w, _text->_h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_neb_tex;
        //SaveTexture(_text, _retval);
        _curr_neb_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        _text->_data = 0;
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

int LoadPlanetTexture(char * tex, int w, int h)
{
    if (_curr_planet_tex < numplanettex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 24);    //load the texture here...
        if (_text == 0)
        {
            LogI("Could not load planet texture ", 0);
            LogI(tex, 1);
            return -1;
        }

        // set the texture type
        _planet_types[_curr_planet_tex] = TYPE_PLANET;
        _planetquads[_curr_planet_tex] = gluNewQuadric();

        gluQuadricNormals(_planetquads[_curr_planet_tex], GLU_SMOOTH);

        // Generate and Bind The Texture
        glBindTexture(GL_TEXTURE_2D, _planet_textures[_curr_planet_tex]);
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

int LoadRingTexture(char * tex, int w, int h)
{
    if (_curr_ring_tex < numringtex)
    {
        _lp_texture _text = LoadTextureFile(tex, w, h, 32);    //load the texture here...
        if (_text == 0)
        {
            LogI("Could not load ring texture ", 0);
            LogI(tex, 1);
            return -1;
        }

        // set the texture type
        _ring_types[_curr_ring_tex] = TYPE_RING;
        // Generate and Bind The Texture
        glBindTexture(GL_TEXTURE_2D, _ring_textures[_curr_ring_tex]);
        //LogI("Binding Ring Tex idx:", _curr_ring_tex);
        //LogI(tex, _ring_textures[_curr_ring_tex]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     _text->_w, _text->_h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, _text->_data);
        int _retval = _curr_ring_tex;
        //SaveTexture(_text, _retval, 32);
        _curr_ring_tex++;

        free(_text->_data); //clean up any ram we used for the texture data
        _text->_data = 0;
        free(_text);    //clean up any ram we used for the texture

        return _retval;
    }
    return -1;
}

void ExtractPath(char * output, char * input, int size)
{
    int x;
    int foundIdx = 0;
    //LogI(input, foundIdx);
    for (x = 0; x < size; x++)
    {
        if (input[x] == '\\')
        {
            foundIdx = x;
            //LogI(input, foundIdx);
        }
    }
    //at end of loop, foundIdx will point to last occurence of search string.
    if (foundIdx != 0)
        strncpy(output, input, foundIdx);
    //LogI(output, foundIdx);

    for (x =0; x < size; x++)
        if (output[x] == '\\')
            output[x] = '/';
    //LogI(output, foundIdx);
}

void LoadTextures()
{
    memset(_star_textures, 0, numstartex * sizeof(GLuint));
    memset(_planet_textures, 0, numplanettex * sizeof(GLuint));
    memset(_ring_textures, 0, numringtex * sizeof(GLuint));

    memset(_types, 0, numstartex * sizeof(int));
    memset(_planet_types, 0, numplanettex * sizeof(int));
    memset(_ring_types, 0, numringtex * sizeof(int));

    memset(_planetquads, 0, numstartex * sizeof(GLUquadric*));

    //generate texture names beforehand.
    glGenTextures(numringtex, _ring_textures);
    glGenTextures(numplanettex, _planet_textures);
    glGenTextures(numstartex, _star_textures);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    //Get path to file here to make sure we run correctly when triggered by Windows.
    TCHAR lpFileName[512];
    memset(lpFileName, 0 , 512 * sizeof(TCHAR));
    char fullPath[1024];
    memset(fullPath, 0 , 1024 * sizeof(char));
    DWORD nStrLen = GetModuleFileName(NULL,lpFileName,512);
    //LogI(lpFileName, 0);


    char SpareBuffer[512];
    memset(SpareBuffer, 0 , 512 * sizeof(char));
    ExtractPath(SpareBuffer, lpFileName, nStrLen);
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/star-blue.raw");
    LoadStarTexture(fullPath, 256, 256);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/star-green.raw");
    LoadStarTexture(fullPath, 256, 256);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/star-yellow.raw");
    LoadStarTexture(fullPath, 256, 256);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());

    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/ring-blue.raw");
    LoadRingTexture(fullPath, 512, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());

    //sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/ring-red.raw");
    //LoadRingTexture(fullPath, 512, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    //sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/ring-green.raw");
    //LoadRingTexture(fullPath, 512, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());

    //sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/nebula-dark.raw");
    //LoadNebulaTexture(fullPath, 512, 512);    //takes care of the texture binding
    GenerateNebulaTexture(512, 512, 7);
    //LogI(fullPath, GetLastError());
}

int GetTextureID(int idx)
{
    if (idx >= 0 && idx < numstartex)
    {
        return _star_textures[idx];
    }
    return -1;
}

int GetPlanetTextureID(int idx)
{
    if (idx >= 0 && idx < numplanettex)
    {
        return _planet_textures[idx];
    }
    return -1;
}

int GetRingTextureID(int idx)
{
    if (idx >= 0 && idx < numringtex)
    {
        return _ring_textures[idx];
    }
    return -1;
}

int GetNebulaTextureID(int idx)
{
    if (idx >= 0 && idx < numnebtex)
    {
        return _neb_textures[idx];
    }
    return -1;
}

int GetTextureType(int idx)
{
    if (idx >= 0 && idx < numstartex)
    {
        return _types[idx];
    }
    return -1;
}

int GetPlanetTextureType(int idx)
{
    if (idx >= 0 && idx < numplanettex)
    {
        return _planet_types[idx];
    }
    return -1;
}

int GetRingTextureType(int idx)
{
    if (idx >= 0 && idx < numringtex)
    {
        return _ring_types[idx];
    }
    return -1;
}

int GetNebulaTextureType(int idx)
{
    if (idx >= 0 && idx < numnebtex)
    {
        return _neb_types[idx];
    }
    return -1;
}

GLUquadric * GetQuadricPointer(int idx)
{
    if (idx >= 0 && idx < numstartex)
    {
        return _planetquads[idx];
    }
    return (GLUquadric *)-1;
}

int GetTextureCount()
{
    return numstartex;
}

int GetPlanetTextureCount()
{
    return numplanettex;
}

int GetRingTextureCount()
{
    return numringtex;
}

int GetNebulaTextureCount()
{
    return numnebtex;
}
