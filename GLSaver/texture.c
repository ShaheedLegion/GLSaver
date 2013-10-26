#include "texture.h"
#include <stdio.h>
#include "logger.h"
#define numstartex 4
#define numplanettex 3
#define numringtex 3
//totally arbitrary number.

GLuint _star_textures[numstartex];
GLuint _planet_textures[numplanettex];
GLuint _ring_textures[numringtex];
GLUquadric *_planetquads[numplanettex];

int _types[numstartex];
int _planet_types[numplanettex];
int _ring_types[numringtex];
int _curr_tex = 0;
int _curr_planet_tex = 0;
int _curr_ring_tex = 0;

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
    BOOL bFound = FALSE;
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
/*
    int i;
    for (i = 0; i < numringtex; i++)
        LogI("Ring Texture ID:", _ring_textures[i]);
    for (i = 0; i < numplanettex; i++)
        LogI("Planet Texture ID:", _planet_textures[i]);
    for (i = 0; i < numstartex; i++)
        LogI("Star Texture ID:", _star_textures[i]);
*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    // Specify a global ambient
    GLfloat globalAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );

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

    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/ring-red.raw");
    LoadRingTexture(fullPath, 512, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/ring-green.raw");
    LoadRingTexture(fullPath, 512, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
/*
    //the following textures require spherical mapping.
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/earth.raw");
    LoadPlanetTexture(fullPath, 1024, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/jupiter.raw");
    LoadPlanetTexture(fullPath, 1024, 512);    //takes care of the texture binding
    //LogI(fullPath, GetLastError());
    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/mars.raw");
    LoadPlanetTexture(fullPath, 1024, 512);    //takes care of the texture binding

    sprintf(fullPath, "%s/%s", SpareBuffer, "Textures/earth.raw");
    LoadPlanetTexture(fullPath, 1024, 512);    //takes care of the texture binding
*/
    //LogI(fullPath, GetLastError());
    //LoadPlanetTexture("Textures/saturn.raw", 720, 360, TYPE_PLANET);    //takes care of the texture binding
    //LoadPlanetTexture("Textures/neptune.raw", 720, 360, TYPE_PLANET);    //takes care of the texture binding
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

GLUquadric * GetQuadricPointer(int idx)
{
    if (idx >= 0 && idx < numstartex)
    {
        return _planetquads[idx];
    }
    return -1;
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

/*
Texture map in solid sphere using GLUT(OpenGL)

 This source code is written in C used to draw a solid sphere using OpenGL and you can map texture in it.
Create a project for OpenGL  and copy and paste this code and use your desire imagefile for texture mapping.You can download source code here.

//main.c

#include <GL/gl.h>
#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>

GLuint texture;
double angle = 0;
typedef struct
{
    int X;
    int Y;
    int Z;
    double U;
    double V;
}VERTICES;

const double PI = 3.1415926535897;
const int space = 10;
const int VertexCount = (90 / space) * (360 / space) * 4;
VERTICES VERTEX[VertexCount];
GLuint LoadTextureRAW( const char * filename );



void DisplaySphere (double R, GLuint texture)
{

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int b;
    glScalef (0.0125 * R, 0.0125 * R, 0.0125 * R);
    //glRotatef (90, 1, 0, 0);
   glBindTexture (GL_TEXTURE_2D, texture);
    glBegin (GL_TRIANGLE_STRIP);
    for ( b = 0; b <VertexCount; b++)
    {
        glTexCoord2f (VERTEX[b].U, VERTEX[b].V);
        glVertex3f (VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
    }



for ( b = 0; b <VertexCount; b++)
{

    glTexCoord2f (VERTEX[b].U, -VERTEX[b].V);

    glVertex3f (VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);

}

    glEnd();
}
void CreateSphere (double R, double H, double K, double Z) {
    int n;
    double a;
    double b;
    n = 0;
for( b = 0; b <= 90 - space; b+=space){

    for( a = 0; a <= 360 - space; a+=space)
    {
        VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
        VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
        VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
        VERTEX[n].V = (2 * b) / 360;
        VERTEX[n].U = (a) / 360;

        n++;
        VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
        VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
        VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
        VERTEX[n].V = (2 * (b + space)) / 360;
        VERTEX[n].U = (a) / 360;
        n++;
        VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
        VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
        VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
        VERTEX[n].V = (2 * b) / 360;
        VERTEX[n].U = (a + space) / 360;
        n++;
        VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) /180 * PI) - H;
        VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) /180 * PI) + K;
        VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
        VERTEX[n].V = (2 * (b + space)) / 360;
        VERTEX[n].U = (a + space) / 360;
        n++;
    }

}
}
void display (void) {

    glClearDepth(1);
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-10);
    glRotatef(angle,0,1,0);
    DisplaySphere(5, texture);
    glutSwapBuffers();
    angle ++;
}
void init (void) {
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );

    glDepthFunc(GL_LEQUAL);

    texture= LoadTextureRAW( "your_image_name.bmp" );

    CreateSphere(30,0,0,0);
}
void reshape (int w, int h) {

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode (GL_PROJECTION);

    glLoadIdentity ();

    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

    glMatrixMode (GL_MODELVIEW);
}
int main (int argc, char **argv) {

    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);

    glutInitWindowPosition (100, 100);

    glutCreateWindow ("A basic OpenGL Window");

    init();

    glutDisplayFunc (display);

    glutIdleFunc (display);

    glutReshapeFunc (reshape);

    glutMainLoop ();

    return 0;
}
GLuint LoadTextureRAW( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;



  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;



  width = 1024;

  height = 512;

  data = (unsigned char *)malloc( width * height * 3 );


    //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );

  fclose( file );

for(int i = 0; i < width * height ; ++i)
{
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];
    //B = data[index];
    data[index] = R;
    data[index+2] = B;

}


    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );

return texture;
}

*/
