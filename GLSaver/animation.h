#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <Windows.h>
/*
This file contains external forward declarations for the functions which
will be used by the main file to run the complete animation.
*/
//enum { TRIG_SLOW = 0, TRIG_MEDIUM, TRIG_FAST };

void SetupAnimation(int Width, int Height);
void CleanupAnimation();
void Render(HDC * hdc);
#endif // ANIMATION_H_INCLUDED
