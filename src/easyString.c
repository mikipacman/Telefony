/** @file
 * Implementacja paru prostych funkcji ułatwiających pracę na stringach.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include "easyString.h"
#include <stdlib.h>

extern bool hasOnlyDigits(char const *string)
{
    if (!string)
        return false;

    if (string[0] == '\0')
        return false;

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
