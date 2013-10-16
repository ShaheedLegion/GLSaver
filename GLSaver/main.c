#include <windows.h>
#include  <scrnsave.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "animation.h"

void InitGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC);
void OnTimer(HDC hDC);

int TIMER = 1;

// Shut down OpenGL
void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( hRC );

  ReleaseDC( hWnd, hDC );
}

// Initialize OpenGL
void InitGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{

  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory( &pfd, sizeof pfd );
  pfd.nSize = sizeof pfd;
  pfd.nVersion = 1;
  //pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //blaine's
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;

  *hDC = GetDC( hWnd );

  int i = ChoosePixelFormat( *hDC, &pfd );
  SetPixelFormat( *hDC, i, &pfd );

  *hRC = wglCreateContext( *hDC );
  wglMakeCurrent( *hDC, *hRC );
}

// Screen Saver Procedure
LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;
  static HGLRC hRC;
  static RECT rect;
  int Width;
  int Height;

  switch ( message ) {

  case WM_CREATE:
    // get window dimensions
    GetClientRect( hWnd, &rect );
    Width = rect.right;
    Height = rect.bottom;

    // setup OpenGL, then animation
    InitGL( hWnd, &hDC, &hRC );
    SetupAnimation(Width, Height);

    //set timer to tick every 10 ms
    SetTimer( hWnd, TIMER, 10, NULL );
    return 0;

  case WM_DESTROY:
    KillTimer( hWnd, TIMER );
        CleanupAnimation();
    CloseGL( hWnd, hDC, hRC );
    return 0;

  case WM_TIMER:
    OnTimer(hDC);
    return 0;

  }

  return DefScreenSaverProc(hWnd, message, wParam, lParam );

}

// needed for SCRNSAVE.LIB
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}

void OnTimer(HDC hDC)   //render the animation contents
{
    Render(&hDC);
}
