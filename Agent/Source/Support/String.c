#include "Support/String.h"
#include "Debug.h"
#include "Support/Memory.h"


// Function to generate a random string
void GenerateRandomString(char *str, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz123456789";
    int charsetLength = sizeof(charset) - 1;


    for (int i = 0; i < length; i++) {
        int index = rand() % charsetLength;
        str[i] = charset[index];
    }

    str[length] = '\0'; // Null-terminate the string
}

unsigned char hexCharToValue(char c) {
    if (c >= '0' && c <= '9') return (unsigned char)(c - '0');
    if (c >= 'a' && c <= 'f') return (unsigned char)(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return (unsigned char)(c - 'A' + 10);
    return 0; // Invalid character
}


void removeLetter(char *str, char letter) {
    int i, j = 0, len = strlen(str);
    
    for (i = 0; i < len; i++) {
        if (str[i] != letter) {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';  // Terminate the modified string
}


void replaceLetter(char *str,int strlen, char oldLetter, char newLetter) {
    int i, len = strlen;
    
    for (i = 0; i < len; i++) {
        if (str[i] == oldLetter) {
            str[i] = newLetter;
        }
    }
}



size_t strlcpy(char *dst, const char *src, size_t n) {
    char *p = dst;

    if (n != 0) {
        for (; --n != 0; p++, src++) {
            if ((*p = *src) == '\0')
                return p - dst;
        }
        *p = '\0';
    }
    return (p - dst) + strlen(src);
}

size_t strlcat(char *dst, const char *src, size_t n) {
    char *p = dst;

    while (n != 0 && *p != '\0') {
        p++;
        n--;
    }
    if (n != 0) {
        for (; --n != 0; p++, src++) {
            if ((*p = *src) == '\0')
                return p - dst;
        }
        *p = '\0';
    }
    return (p - dst) + strlen(src);
}

void convertToLowercase(char *str) {
    if (!str) return;  // Check if the string is NULL

    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


char* strndup(char* str, int chars,int* outsize)
{
    char* buffer = 0;
    int n = 0;
    int size=chars + 1;  
    buffer = (char*)HMalloc(size);
    if (buffer)
    {
        *outsize=size;
        for (n = 0; ((n < chars) && (str[n] != 0)); n++) buffer[n] = str[n];
        buffer[n] = 0;
    }

    return buffer;
}
