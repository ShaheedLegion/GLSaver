#include "animation.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "logger.h"
#include "trigger.h"
#include "texture.h"

//This file contains "private" declarations used internally by this file.
int _num_stars = 5000;   //how many stars to display
int _num_rings = 8;//25;//100;   //how many rings to display
int _display_rings = 0;
int nearest = 400;
int farthest = 1;
int Width = 0;
int Height = 0;

int _spin_triggers[3];
int *_srings;
int *_sringidx;

/*
We start with the 'simple case' of a traditional star which
only contains location data.
*/
typedef struct _tag_vertex
{
    int x;
    int y;
    int z;
    int tex_type;
    int tex_idx;
    int id;
} _vertex, * _lp_vertex;

_lp_vertex _stars = NULL;
_lp_vertex _rings = NULL;

void CheckStar(_lp_vertex star, int idx);  //forward declaration
void CheckRing(_lp_vertex ring, int idx, int initial);  //forward declaration
void SortRings(int print);

void SetupAnimation(int w, int h)
{
    if (h < 1080)
    {
        _num_stars = 2500;
        _display_rings = 1;
    }
    Width = w / 10;
    Height = h / 6;

    GLdouble aspect = ((double)w / (double)h) * 10.0;

    glShadeModel(GL_FLAT);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(aspect, (GLdouble)w/(GLdouble)h,(GLdouble)nearest, (GLdouble)farthest);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, (GLdouble)(nearest - 20), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glFogi(GL_FOG_MODE, GL_LINEAR);        // Fog Mode
    GLfloat fogColor[4]= {0.9f, 0.9f, 0.9f, 0.01f};      // Fog Color
    glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
    glFogf(GL_FOG_DENSITY, 0.0091f);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
    glFogf(GL_FOG_END, nearest);             // Fog Start Depth
    glFogf(GL_FOG_START, farthest);               // Fog End Depth
    glEnable(GL_FOG);                   // Enables GL_FOG

    //camera xyz, the xyz to look at, and the up vector (+y is up)
    _stars = (_lp_vertex)malloc(_num_stars * sizeof(_vertex));
    _rings = (_lp_vertex)malloc(_num_rings * sizeof(_vertex));
    _srings = (int)malloc(_num_rings * sizeof(int));
    _sringidx = (int)malloc(_num_rings * sizeof(int));

    LoadTextures();


    int i;
    _lp_vertex _curr = _stars;
    srand((unsigned)26351024);
    for (i = 0; i < _num_stars; i++)
    {
        _curr->z = nearest + 1;
        CheckStar(_curr, i);
        _curr++;
    }

    _curr = _rings;
    for (i = 0; i < _num_rings; i++)
    {
        _curr->z = nearest + 1;
        CheckRing(_curr, i, 1);
        _curr++;
    }

    SortRings(0);

    _spin_triggers[0] = SetTrigger(TRIG_SLOWEST, 165);    //delta is in frames
    _spin_triggers[1] = SetTrigger(TRIG_SLOWEST, 200);    //delta is in frames
    _spin_triggers[2] = SetTrigger(TRIG_FASTEST, 1265);    //delta is in frames
}

//clean up any objects created by this "class"
void CleanupAnimation()
{
    free(_stars);
    free(_rings);
    free(_srings);
    free(_sringidx);
    CleanupTriggers();
}

void CheckStar(_lp_vertex star, int idx)
{
    if (idx < _num_stars)
    {
        star->z++;
        if (star->z > nearest)
        {
            star->x = (rand() % Width) - (Width / 2);
            star->y = (rand() % Height) - (Height / 2);
            star->z = (rand() % nearest);

            star->tex_type = -1;    //set to not have texture by default
            star->id = -1;

            int sidx = (rand() % GetTextureCount());
            if (GetTextureCount() == 1)
                sidx = 0;
            star->tex_type = GetTextureType(sidx);
            star->id = GetTextureID(sidx);
        }
    }
}

void CheckRing(_lp_vertex ring, int idx, int initial)
{
    if (idx < _num_rings)
    {
        ring->z++;
        if (ring->z > nearest)
        {
            //int w = Width / 5;
            //int h = Height / 10;
            ring->x = 0;//(rand() % w) - (w / 2);
            ring->y = 0;//(rand() % h) - (h / 2);
            if (initial)
                ring->z = (nearest / _num_rings) * idx;//(rand() % nearest);
            else
                ring->z = _srings[0] - (nearest / _num_rings);

            ring->tex_type = -1;    //set to not have texture by default
            ring->id = -1;

            int ridx = (rand() % GetRingTextureCount());
            if (GetRingTextureCount() == 1)
                ridx = 0;

            //LogI("ring tex idx:", ridx);
            ring->tex_type = GetRingTextureType(ridx);
            //LogI("ring tex type:", ring->tex_type);
            ring->id = GetRingTextureID(ridx);
            //LogI("ring tex id:", ring->id);
        }
    }
}

void SortRings(int print)
{
    int i, n, temp, swapped;
    for (i = 0; i < _num_rings; i++)
    {
        _srings[i] = _rings[i].z;   //sort based on the z-position of each vertex
        _sringidx[i] = i;
    }

   n = _num_rings;
   do {
      swapped = 0;
      for (i = 0; i < _num_rings-1; i++)
      {
         if (_srings[i] > _srings[i + 1])
         {
            temp = _srings[i];
            _srings[i] = _srings[i + 1];
            _srings[i + 1] = temp;

            temp = _sringidx[i];
            _sringidx[i] = _sringidx[i + 1];
            _sringidx[i + 1] = temp;
            swapped = 1;
         }
      } //end for
      --n;
   } while (swapped);

   if (print > 0)
   {
        for (i = 0; i < _num_rings; i++)
        {
            LogI("Ring Idx: ", i);
            LogI("Sorted Z: ", _srings[i]);
            LogI("Sorted Idx: ", _sringidx[i]);
        }
        LogI("Sort Algorithm Complete - ", 0);
   }
}

int CalculateColor(int distance)
{
	//need to make color a percentage of distance
	double percentage = (double)((double)distance / (double)nearest);
	int value = (int)((double)255.0 * percentage);

	if (value > 255)	//sanity check for when distance is > nearest
		return 255;

	if (value < 0)	//sanity check for when distance is < 0
		return 64;

	return value;
}

void DrawStar(_lp_vertex star)
{
    float offset = 0.05;    //determines the size of the star
    if (star->tex_type == TYPE_STAR)
    {
        glBindTexture(GL_TEXTURE_2D, star->id);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(star->x - offset, star->y - offset, star->z);
        glTexCoord2f(0.0, 1.0); glVertex3f(star->x - offset, star->y + offset, star->z);
        glTexCoord2f(1.0, 1.0); glVertex3f(star->x + offset, star->y + offset, star->z);
        glTexCoord2f(1.0, 0.0); glVertex3f(star->x + offset, star->y - offset, star->z);
        glEnd();
    }
}

void DrawPlanet(_lp_vertex star)
{
    if (star->tex_type == TYPE_PLANET)
    {
        glTranslatef(star->x, star->y, star->z);
        glBindTexture(GL_TEXTURE_2D, star->id);
        gluQuadricTexture(GetQuadricPointer(star->tex_idx), GLU_TRUE);
        gluSphere(GetQuadricPointer(star->tex_idx),18,32,32);
        glTranslatef(0.0, 0.0, -(GLfloat)(star->z));
    }
}

//void DrawRing(_lp_vertex ring)
void DrawRing(int i)
{
    float offset = 2.29;    //determines the size of the ring
    _lp_vertex ring = &_rings[i];

    if (ring->tex_type == TYPE_RING)
    {
        glBindTexture(GL_TEXTURE_2D, ring->id);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(ring->x - offset, ring->y - offset, ring->z);
        glTexCoord2f(0.0, 1.0); glVertex3f(ring->x - offset, ring->y + offset, ring->z);
        glTexCoord2f(1.0, 1.0); glVertex3f(ring->x + offset, ring->y + offset, ring->z);
        glTexCoord2f(1.0, 0.0); glVertex3f(ring->x + offset, ring->y - offset, ring->z);
        glEnd();
    }
}

void Render(HDC * hDC) //increment and display
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotated(UpdateTrigger(_spin_triggers[0]), 1.0, 0.0, 0.0);
    glRotated(UpdateTrigger(_spin_triggers[1]), 0.0, 1.0, 0.0);
    glRotated(UpdateTrigger(_spin_triggers[2]), 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);

    int i;
    _lp_vertex _curr = _stars;

    for (i = 0; i < _num_stars; i++)
    {
        DrawStar(_curr);
        CheckStar(_curr, i);
        _curr++;
    }
/*
    _curr = _stars;
    for (i = 0; i < _num_stars; i++)
    {
        DrawPlanet(_curr);
        _curr++;
    }
*/
    if (_display_rings)
    {
        _curr = _rings;
        SortRings(0);
        for (i = 0; i < _num_rings; i++)
        {
            //DrawRing(_curr);
            DrawRing(_sringidx[i]);
            CheckRing(_curr, i, 0);
            _curr++;
        }
    }
    glDisable(GL_TEXTURE_2D);
    SwapBuffers(*hDC);
    glPopMatrix();
}
