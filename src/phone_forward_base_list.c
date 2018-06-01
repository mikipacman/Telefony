/** @file
 * Implementacja klasy obsługującej listę zawierającą bazy przekierowań telefonicznych.
 * Lista jest cyklicza, dwukierunkowa i posiada strażnika (na jej początku), który
 * jest rozpoznawany po tym, że jego nazwa to "$". Lista jest posortowana leksykograficznie
 * wzgledem napisów w polach @p id.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include "phone_forward_base_list.h"
#include <string.h>
#include <stdbool.h>
#include <printf.h>
#include <stdio.h>

extern PFBList* initPFBList()
{
    PFBList *pfbList = (PFBList*)malloc(sizeof(PFBList));
    if (!pfbList)
        return NULL;

    char *id = (char*)malloc(sizeof(char) * 2);
    if (!id)
        return NULL;

    strcpy(id, "$");
    pfbList->pfBase = NULL;
    pfbList->id = id;
    pfbList->next = pfbList;
    pfbList->prev = pfbList;

    return pfbList;
}

/// Usuwa pojedynczego node'a z listy.
static void deleteNodeFromPFBList(PFBList *pfbList)
{
    free(pfbList->id);
    phfwdDelete(pfbList->pfBase);
    PFBList *prev = pfbList->prev, *next = pfbList->next;
    prev->next = next;
    next->prev = prev;
    free(pfbList);
}

extern bool deleteFromPFBList(PFBList *pfbList, char *id)
{
    PFBList *temp = pfbList;
    if (strcmp(id, pfbList->id) == 0)
    {
        deleteNodeFromPFBList(pfbList);
        return true;
    }
    pfbList = pfbList->next;
    while (pfbList != temp && strcmp(id, pfbList->id) != 0)
        pfbList = pfbList->next;

    if (strcmp(id, pfbList->id) == 0)
    {
        deleteNodeFromPFBList(pfbList);
        return true;
    }
    return false;
}

extern void deletePFBList(PFBList *pfbList)
{
    PFBList *temp = pfbList->next;

    while (temp != pfbList)
    {
        temp = temp->next;
        if (temp->prev->pfBase)
            phfwdDelete(temp->prev->pfBase);
        free(temp->prev->id);
        free(temp->prev);
    }
    if (pfbList->pfBase)
        phfwdDelete(pfbList->pfBase);
    free(pfbList->id);
    free(pfbList);
}

/// Sprawdza czy jest strażnikiem.
static bool isGuard(PFBList *pfbList)
{
    return strcmp(pfbList->id, "$") == 0;
}

extern PFBList* addToPFBList(PFBList *pfbList, char *id)
{
    PFBList *current = pfbList;

    // Szukamy miejsca, w którym mamy się dodać.

    while (strcmp(id, current->next->id) > 0 && !isGuard(current->next))
        current = current->next;

    // Jeśli już taki istnieje to zwracamy jego adres.

    if (strcmp(id, current->next->id) == 0)
        return current->next;

    // Tworzymy node'a.

    PFBList *new = (PFBList*)malloc((sizeof(PFBList)));
    if (!new)
        return NULL;

    size_t idLen = strlen(id);
    char *idCopy = (char*)malloc(sizeof(char) * (idLen + 1));
    strcpy(idCopy, id);
    if (!idCopy)
        return NULL;

    new->id = idCopy;
    new->pfBase = phfwdNew();
    if (!(new->pfBase))
        return NULL;

    // Dodajemy go.

    current->next->prev = new;
    new->next = current->next;
    current->next = new;
    new->prev = current;

    return new;
}