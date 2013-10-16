#include "logger.h"
#include <stdio.h>

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
