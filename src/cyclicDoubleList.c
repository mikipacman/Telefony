/** @file
 * Implementacja strukury cyklicznej dwukierunkowej listy ze strażnikiem zawierającej napisy.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cyclicDoubleList.h"
#include "phone_forward.h"

extern CDList* initCDList()
{
    CDList* toReturn = (CDList*)malloc(sizeof(CDList));

    if (!toReturn)
        return NULL;
    toReturn->num = NULL;
    toReturn->next = toReturn;
    toReturn->prev = toReturn;

    return toReturn;
}

extern CDList* addToCDList(CDList *list, char const *toAdd, int toAddLength)
{
    CDList* newNode = malloc(sizeof(CDList));
    if (!newNode || !toAdd)
        return NULL;

    char *numCopy;
    if ((numCopy = (char*)malloc(sizeof(char) * (toAddLength + 1))) == NULL)
        return NULL;

    strcpy(numCopy, toAdd);
    newNode->num = numCopy;
    CDList *foo = list->next;
    list->next = newNode;
    newNode->prev = list;
    newNode->next = foo;
    foo->prev = newNode;

    return newNode;
}

extern void deleteFromCDList(CDList *toDelete)
{
    if (!toDelete)
        return;

    free(toDelete->num);
    CDList *prev = toDelete->prev, *next = toDelete->next;
    prev->next = next;
    next->prev = prev;

    free(toDelete);
}

extern bool cdListIsEmpty(CDList *list)
{
    if (!list)
        return false;

    return list->num == NULL && list->next->num == NULL;
}

extern void deleteList(CDList *list)
{
    if (!list)
        return;

    CDList *foo = list->next;
    while (foo != list)
    {
        foo = foo->next;
        free(foo->prev->num);
        free(foo->prev);
    }

    free(list->num);
    free(list);
}

extern int getCDListLength(CDList *list)
{
    if (!list)
        return -1;

    int counter = 0;
    CDList *foo = list->next;

    if (list->num != NULL)
        counter++;

    while (foo != list)
    {
        if (foo->num != NULL)
            counter++;
        foo = foo->next;
    }

    return counter;
}