/** @file
 * Implementacja paru prostych funkcji ułatwiających pracę na stringach.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include "easyString.h"
#include "phone_forward_base_list.h"
#include <stdlib.h>
#include <stdio.h>

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

extern bool isDigit(int c)
{
    return '0' <= c && c <= '9';
}

extern bool isLetter(int c)
{
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

extern bool isEOF(int c)
{
    return c == EOF;
}

extern  bool isWhiteSpace(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';// TODO: dodać więcej znaków.
}

extern bool isComment(int c)
{
    int a;

    if (c == '$')
    {
        if ((a = getchar()) == '$')
        {
            return true;
        }
        if (!isEOF(a))
            ungetc(a, stdin);
    }
    return false;
}