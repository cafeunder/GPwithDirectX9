#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define  DEBUG_PRINT_LENGTH 1024
void DebugPrint(const char* format, ...){
    va_list args;
    int     len;
    char    *buffer;

    va_start( args, format );
    len = _vscprintf( format, args )+1;
    buffer = (char*)malloc(len*sizeof(char));

    vsprintf(buffer, format, args);
	OutputDebugString(TEXT(buffer));

    free(buffer);
}

void DebugPrintLine(const char* format, ...){
    va_list args;
    int     len;
    char    *buffer;

    va_start( args, format );
    len = _vscprintf( format, args )+1;
    buffer = (char*)malloc(len*sizeof(char)+1+1);//改行記号と終端記号

    vsprintf(buffer, format, args);
	buffer[len-1] = '\n';
	buffer[len] = '\0';
	OutputDebugString(TEXT(buffer));

    free(buffer);
}