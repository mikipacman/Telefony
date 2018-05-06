#include "easyString.h"
#include <stdlib.h>

extern char* emptyString()
{
    char *toReturn = (char*)malloc(sizeof(char));
    toReturn[0] = '\n';
    return toReturn;
}

extern char* addToString(char *string, char c)
{
    int length = 0;

    while (string[length] != '\n')
        length++;

    char *toReturn = (char*)malloc(sizeof(char) * (length + 2));

    for (int i = 0; i < length; i++)
    {
        toReturn[i] = string[i];
    }
    toReturn[length + 1] = c;
    toReturn[length + 2] = '\n';

    return toReturn;
}

extern bool hasOnlyDigits(char const *string)
{
    int i = -1;
    while  (string[++i] != '\0')
        if ('0' > string[i] || string[i] > '9')
            return false;

    return true;
}

extern int charToInt(char c)
{
    return c - '0';
}

extern char* addCharToString(char *string, char c, int stringLength)
{
    char *toReturn;

    if ((toReturn = (char*)malloc(sizeof(char) * (stringLength + 2))) == NULL)
        return NULL;

    int i = 0;

    while (string[i] != '\0')
        toReturn[i] = string[i++];

    toReturn[stringLength] = c;
    toReturn[stringLength + 1] = '\0';

    return toReturn;
}

extern char intToChar(int i)
{
    return i + '0';
}
