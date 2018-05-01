#include <stdbool.h>
#include <stdio.h>"
#include "cyclicDoubleList.h"
#include "phone_forward.h"

extern CDList* initCDList()
{
    CDList* toReturn = (CDList*)malloc(sizeof(CDList));

    if (!toReturn)
        return NULL;
    toReturn->forward = NULL;
    toReturn->next = toReturn;
    toReturn->prev = toReturn;

    return toReturn;
}

extern CDList* addToCDList(CDList *list, PhoneForward *toAdd)
{
    CDList* newNode = malloc(sizeof(CDList));

    if (!newNode || !toAdd)
        return NULL;

    newNode->forward = toAdd;

    CDList *foo = list->next;
    list->next = newNode;
    newNode->prev = list;
    newNode->next = foo;
    foo->prev = newNode;

    return newNode;
}

extern void deleteFromCDList(CDList *toDelete)
{
    CDList *prev = toDelete->prev, *next = toDelete->next;
    prev->next = next;
    next->prev = prev;

    free(toDelete);
}

extern bool cdListIsEmpty(CDList *list)
{
    return list->forward == NULL && list->next->forward == NULL;
}

//extern void printList(CDList *list)
//{
//    CDList *foo = list;
//
//    printf("%d ", list->val);
//    list = list->next;
//    while(list != foo)
//    {
//        printf(",%d ", list->val);
//        list = list->next;
//    }
//    printf("\n");
//}