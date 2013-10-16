#include "animation.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "logger.h"
#include "trigger.h"

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
} _star, * _lp_star;
_lp_star _stars = NULL;

void CheckStar(_lp_star star, int idx);  //forward declaration

void SetupAnimation(int w, int h)
{
    Width = w / 10;
    Height = h / 8;

    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLdouble)w/(GLdouble)h,(GLdouble)nearest, (GLdouble)farthest);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, (GLdouble)(nearest / 2), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //camera xyz, the xyz to look at, and the up vector (+y is up)

    _stars = (_lp_star)malloc(_num_stars * sizeof(_star));
    int i;
    _lp_star _curr = _stars;
    srand((unsigned)26351024);
    for (i = 0; i < _num_stars; i++)
    {
        _curr->z = nearest + 1;
        CheckStar(_curr, i + (_num_stars / 2));
        _curr++;
    }

    _spin_triggers[0] = SetTrigger(465);    //delta is in frames
    _spin_triggers[1] = SetTrigger(800);    //delta is in frames
    _spin_triggers[2] = SetTrigger(1265);    //delta is in frames
}

//clean up any objects created by this "class"
void CleanupAnimation()
{
    free(_stars);
    CleanupTriggers();
}

void CheckStar(_lp_star star, int idx)
{
    if (idx > (_num_stars / 2))
    {
        star->z++;
        if (star->z > nearest)
        {
            star->x = (rand() % Width) - (Width / 2);
            star->y = (rand() % Height) - (Height / 2);
            star->z = (rand() % (nearest / 2));
        }
    }
    else if (idx > _num_stars)
    {
            star->x = (rand() % Width) - (Width / 2);
            star->y = (rand() % Height) - (Height / 2);
            star->z = (rand() % nearest);
    }
}

void Render(HDC * hDC) //increment and display
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        //glRotatef(UpdateTrigger(_spin_triggers[0]), 1.0, 0.0, 0.0);
        //glRotatef(UpdateTrigger(_spin_triggers[1]), 0.0, 1.0, 0.0);
        glRotatef(UpdateTrigger(_spin_triggers[2]), 0.0, 0.0, 1.0);

        glBegin(GL_POINTS);

        int i, color;
        _lp_star _curr = _stars;
        for (i = 0; i < _num_stars; i++)
        {
            glVertex3i(_curr->x, _curr->y, _curr->z);
            color = 50 + (_curr->z / 2);
            glColor3ub(color, color, color);
            CheckStar(_curr, i);
            _curr++;
        }

        glEnd();

        //glFlush();
        SwapBuffers(*hDC);
        glPopMatrix();
}
