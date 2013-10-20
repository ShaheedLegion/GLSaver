#include "animation.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "logger.h"
#include "trigger.h"
#include "texture.h"

//This file contains "private" declarations used internally by this file.
int _num_stars = 7200;   //how many stars to display
int nearest = 400;
int farthest = 1;
int Width = 0;
int Height = 0;

int _spin_triggers[3];

/*
We start with the 'simple case' of a traditional star which
only contains location data.
*/
typedef struct _tag_star
{
    int x;
    int y;
    int z;
    int tex_type;
    int tex_idx;
    int id;
} _star, * _lp_star;
_lp_star _stars = NULL;

void CheckStar(_lp_star star, int idx);  //forward declaration

void SetupAnimation(int w, int h)
{
    Width = w / 10;
    Height = h / 6;

    glShadeModel(GL_FLAT);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, (GLdouble)w/(GLdouble)h,(GLdouble)nearest, (GLdouble)farthest);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, (GLdouble)(nearest - 20), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //camera xyz, the xyz to look at, and the up vector (+y is up)
    LoadTextures();
    _stars = (_lp_star)malloc(_num_stars * sizeof(_star));
    int i;
    _lp_star _curr = _stars;
    srand((unsigned)26351024);
    for (i = 0; i < _num_stars; i++)
    {
        _curr->z = nearest + 1;
        CheckStar(_curr, i/* + (_num_stars / 2)*/);
        _curr++;
    }

    _spin_triggers[0] = SetTrigger(TRIG_MEDIUM, 465);    //delta is in frames
    _spin_triggers[1] = SetTrigger(TRIG_SLOW, 800);    //delta is in frames
    _spin_triggers[2] = SetTrigger(TRIG_FAST, 1265);    //delta is in frames
}

//clean up any objects created by this "class"
void CleanupAnimation()
{
    free(_stars);
    CleanupTriggers();
}

void CheckStar(_lp_star star, int idx)
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

            int idx = (rand() % GetTextureCount());
            star->tex_type = GetTextureType(idx);
            star->id = GetTextureID(idx);
/*
            int control = (rand() % 100);
            if (control == 0)
            {
                int idx = (rand() % GetPlanetTextureCount());
                star->tex_type = GetPlanetTextureType(idx);
                star->id = GetPlanetTextureID(idx);
                star->tex_idx = idx;
                star->z = farthest;
            }
            else
            {
                int idx = (rand() % GetTextureCount());
                star->tex_type = GetTextureType(idx);
                star->id = GetTextureID(idx);
            }
*/
        }
    }
/*
    else if (idx > _num_stars)
    {
            star->x = (rand() % Width) - (Width / 2);
            star->y = (rand() % Height) - (Height / 2);
            star->z = (rand() % nearest);
            int idx = (rand() % GetTextureCount());
            star->tex_type = GetTextureType(idx);
            star->id = GetTextureID(idx);
    }
*/
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

void DrawStar(_lp_star star)
{
    float offset = 0.05;    //determines the size of the star
    if (star->tex_type == TYPE_NEB || star->tex_type == TYPE_STAR)
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

void DrawPlanet(_lp_star star)
{
    if (star->tex_type == TYPE_PLANET)
    {
        glTranslatef(star->x, star->y, star->z);
        glBindTexture(GL_TEXTURE_2D, star->id);
        gluQuadricTexture(GetQuadricPointer(star->tex_idx), GLU_TRUE);
        gluSphere(GetQuadricPointer(star->tex_idx),4,20,20);
    }
}

void Render(HDC * hDC) //increment and display
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotated(UpdateTrigger(_spin_triggers[0]), 1.0, 0.0, 0.0);
    glRotated(UpdateTrigger(_spin_triggers[1]), 0.0, 1.0, 0.0);
    double rotationz = UpdateTrigger(_spin_triggers[2]);
    glRotated(rotationz, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);

    int i;
    _lp_star _curr = _stars;

    for (i = 0; i < _num_stars; i++)
    {
        DrawStar(_curr);
        CheckStar(_curr, i);
        _curr++;
    }

    _curr = _stars;
    for (i = 0; i < _num_stars; i++)
    {
        DrawPlanet(_curr);
        _curr++;
    }
    glDisable(GL_TEXTURE_2D);
    //glFlush();
    SwapBuffers(*hDC);
/*
    BeginLog();
    for (i = 0; i < GetTextureCount(); i++)
    {
        LogScreenI(*hDC, "Texture Type:", GetTextureType(i));
        LogScreenI(*hDC, "Texture ID:", GetTextureID(i));
    }
    EndLog();
*/
    //LogScreenD(*hDC, "Current Rotation:", rotationz);
    glPopMatrix();
}
