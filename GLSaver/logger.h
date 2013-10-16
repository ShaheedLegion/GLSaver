#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <Windows.h>

void Log(char * msg);
void LogI(char * msg, int i);
void LogScreenD(HDC dc, char * msg, double i);
void LogScreenI(HDC dc, char * msg, int i);
#endif // LOGGER_H_INCLUDED
