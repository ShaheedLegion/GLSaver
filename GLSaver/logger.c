#include "logger.h"
#include <stdio.h>

char * logmsg = 0;
int loglen = 256;

FILE * OpenLogFile()
{
    FILE * f = fopen("C:\\GLStarsLog.txt", "a+");
    return f;
}

void CloseLogFile(FILE * _f)
{
    if (_f != 0)
    {
        fflush(_f);
        fclose(_f);
    }
}

void Log(char * msg)
{
    FILE * _f = OpenLogFile();
    if (_f != 0)
    {
        fprintf(_f, "%s \n", msg);
        CloseLogFile(_f);
    }
}

void LogI(char * msg, int i)
{
    FILE * _f = OpenLogFile();
    if (_f != 0)
    {
        fprintf(_f, "%s %d\n", msg, i);
        CloseLogFile(_f);
    }
}

void LogScreenD(HDC dc, char * msg, double i)
{
    if (logmsg == 0)
        logmsg = (char *)malloc(loglen * sizeof(char));
    sprintf(logmsg, "%s %f", msg, i);
    TextOut(dc, 10, 10, logmsg, strlen(logmsg));
}

void LogScreenI(HDC dc, char * msg, int i)
{
    if (logmsg == 0)
        logmsg = (char *)malloc(loglen * sizeof(char));
    sprintf(logmsg, "%s %d", msg, i);
    TextOut(dc, 10, 30, logmsg, strlen(logmsg));
}
