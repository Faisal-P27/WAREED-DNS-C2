
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define strdup _strdup

// Function to generate a random string
void GenerateRandomString(char *str, int length);
void removeLetter(char *str, char letter);
void replaceLetter(char *str,int strlen, char oldLetter, char newLetter);
size_t strlcpy(char *dst, const char *src, size_t n);
char* strndup(char* str, int chars,int* outsize);