#pragma once
//#define DEBUG
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define _WIN32_WINNT_WIN10 0x0A00
#define WINVER 0x0A00

#if defined(DEBUG)
#include <stdio.h>
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DPRINT(...) { \
   fprintf(stderr, "\nDEBUG: %s:%d:%s(): ", __FILENAME__, __LINE__, __FUNCTION__); \
   fprintf(stderr, __VA_ARGS__); \
}
#define RED_DPRINT(...) { \
   fprintf(stderr, ANSI_COLOR_RED"\nDEBUG: %s:%d:%s(): "ANSI_COLOR_RESET, __FILENAME__, __LINE__, __FUNCTION__); \
   fprintf(stderr, __VA_ARGS__); \
}
#define GREEN_DPRINT(...) { \
   fprintf(stderr, ANSI_COLOR_GREEN"\nDEBUG: %s:%d:%s(): "ANSI_COLOR_RESET, __FILENAME__, __LINE__, __FUNCTION__); \
   fprintf(stderr, __VA_ARGS__); \
}
#define YELLOW_DPRINT(...) { \
   fprintf(stderr, ANSI_COLOR_YELLOW"\nDEBUG: %s:%d:%s(): "ANSI_COLOR_RESET, __FILENAME__, __LINE__, __FUNCTION__); \
   fprintf(stderr, __VA_ARGS__); \
}

#else
#define DPRINT(...);
#define RED_DPRINT(...);
#define GREEN_DPRINT(...);
#define YELLOW_DPRINT(...); // Don't do anything in release builds
#endif
