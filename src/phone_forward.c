#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "phone_forward.h"
#include "cyclicDoubleList.h"
#include "easyString.h"


//----------------------------------------------------------------------------------------------------------------------

PhoneForward* phfwdNew(void)
{
    PhoneForward *newPF = malloc(sizeof(PhoneForward));

    if(!newPF)
        return NULL;

    if ((newPF->listOfFwdToThisNum = initCDList()) == NULL)
        return NULL;

    for (int i = 0; i < 10; i++)
        newPF->nextDigit[i] = NULL;

    newPF->forward = NULL;
    newPF->placeInForwardList = NULL;

    return newPF;
}

//----------------------------------------------------------------------------------------------------------------------

void phfwdDelete(PhoneForward *pf) // TODO: być może poprawić szybkość tego.
{
    if (!pf)
        return;

    free(pf->forward);
    deleteList(pf->listOfFwdToThisNum);

    for (int i = 0; i < 10; i++)
        if (pf->nextDigit[i] != NULL)
            phfwdDelete(pf->nextDigit[i]);

    free(pf);
}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Zwraca pustą strukturę PhoneForward.
 * Zwraca wskaźnik na pusty node struktury PhoneForward.
 * @return wskaźnik na strukturę PhoneForward.
 */
static PhoneForward* emptyPhoneForwardNode(char num)
{
    PhoneForward *pf = (PhoneForward*)malloc(sizeof(PhoneForward));

    if (!pf)
        return NULL;

    if ((pf->listOfFwdToThisNum = initCDList()) == NULL)
        return NULL;

    for (int i = 0; i < 10; i++) // Czy na pewno to trzeba robić?
        pf->nextDigit[i] = NULL;

    pf->currentChar = num;
    pf->forward = NULL;
    pf->placeInForwardList = NULL;

    return pf;
}

/** @brief Zwraca wskaźnik na node'a odpowiadającego numerowi.
 * Zwraca wskaźnik na node'a w strukturze @p pf odpowiadającego numerowi @p num.
 * Jeśli odpowiednich node'ów nie ma w strukturze to je tworzy.
 * @param pf - wskaźnik na strukturę PhoneNumber.
 * @param num - wskaźnik na stringa z numerem.
 * @return wskaźnik na node'a odpowiadającemu numerowie
 * lub NULL jeśli nie udało się zaalokować pamięci.
 */
static PhoneForward* getPtrForNumber(PhoneForward *pf, char const* num)
{
    PhoneForward *foo = pf;

    for (int i = 0, ind = 0; num[i] != '\0'; i++)
    {
        ind = charToInt(num[i]);
        if (foo->nextDigit[ind] != NULL)
        {
            foo = foo->nextDigit[ind];
        }
        else
        {
            if ((foo->nextDigit[ind] = emptyPhoneForwardNode(num[i])) == NULL)
                return NULL;

            foo = foo->nextDigit[ind];
        }
    }

    return foo;
}

/** @brief Rekurencyjnie ustawia przekierowania.
 * O ile w wierzchołku @p node1 istnieje przekierowanie w polu @p forward (czyli pole to jest różne od NULL)
 * ustawia przekierowanie numeru @p num1 na @p num2. Poprzez ustawienie przekierowania rozumiemy ustawienie
 * pól @p node1->forward, @p node1->placeInForwardList, @p node2->listOfFwdToThisNum. Następnie funkcja zapuszczana
 * jest dla synów @p node1. W miarę potrzeb dodawani są synowie @p node2.
 * @param[in] pf - struktura przechowująca przekierowania.
 * @param[in] num1 - wskaźnik na napis reprezentujący numer przekierowany.
 * @param[in] num2 - wskaźnik na napis reprezentujący numer na które ustawiane jest przekierowanie.
 * @param[in] node1 - node struktury @p pf odpowiadający numerowi @p num1.
 * @param[in] node2 - node struktury @p pf odpowiadający numerowi @p num2.
 * @param[in] num1Length - długość napisu @p num1.
 * @param[in] num2Length - długość napisu @p num2.
 * @return TRUE operacja się powiedzie, FALSE wpp.
 */
static bool setForwardsForChildren(PhoneForward *pf, char *num1, char *num2,
                                   PhoneForward *node1, PhoneForward *node2, int num1Length, int num2Length)
{
    if (node1->forward != NULL) // Jeśli numer jest w rejestrze to przekierowujemy go.
    {

        char *forward;

        // TODO: ogarnąć co się dzieje z tym mallociem przy wywołaniu takiego samego prefiksu dwa razy.
        if ((forward = (char*)malloc(sizeof(char) * (num2Length + 1))) == NULL)
            return false;

        free(node1->forward);
        deleteFromCDList(node1->placeInForwardList);

        strcpy(forward, num2);      // Tworzymy nowy napis.
        node1->forward = forward;

        if ((node1->placeInForwardList = addToCDList(node2->listOfFwdToThisNum, num1, num1Length)) == NULL)
            return false;
    }

    /**
     * Zakładam, że w każdym liściu drzewa jest numer.
     */

    for (int i = 0; i < 10; i++)
    {
        if (node1->nextDigit[i] != NULL)
        {
            if (node2->nextDigit[i] == NULL)
                if ((node2->nextDigit[i] = emptyPhoneForwardNode(i)) == NULL)
                    return false;

            char *num1Extended, *num2Extended;

            if ((num1Extended = addCharToString(num1, intToChar(i), num1Length)) == NULL)
                return false;
            if ((num2Extended = addCharToString(num2, intToChar(i), num2Length)) == NULL)
                return false;

            if (setForwardsForChildren(pf, num1Extended, num2Extended, node1->nextDigit[i],
                                       node2->nextDigit[i], num1Length + 1, num2Length + 1) == false)
            {
                free(num1Extended);
                free(num2Extended);
                return false;
            }

            free(num1Extended);
            free(num2Extended);
        }
    }

    return true;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{

    if (strcmp(num1, num2) == 0)
        return false;
    if (!hasOnlyDigits(num1) || !hasOnlyDigits(num2))
        return false;

    PhoneForward *ptrForPrefixNum1, *ptrForPrefixNum2;  // Wskaźniki na node'y odpowiadające num1 i num2.

    if ((ptrForPrefixNum1 = getPtrForNumber(pf, num1)) == NULL)
        return false;
    if ((ptrForPrefixNum2 = getPtrForNumber(pf, num2)) == NULL)
        return false;

    int num1Length = getStringLength(num1), num2Length = getStringLength(num2);

    // Ustawiamy nie-NULL'a w przekierowaniu ptrForPrefixNum1, aby zaznaczyć dla funkcji
    // setForwardsForChildren, że numer ten ma się przekierować.
    char *foo;

    if ((foo = (char*)malloc(sizeof(char))) == NULL)
        return false;

    ptrForPrefixNum1->forward = foo;

    char *num1Cpy, *num2Cpy;

    if ((num1Cpy = (char*)malloc(sizeof(char) * (num1Length + 1))) == NULL)
        return false;
    if ((num2Cpy = (char*)malloc(sizeof(char) * (num2Length + 1))) == NULL)
        return false;

    strcpy(num1Cpy, num1);
    strcpy(num2Cpy, num2);

    bool toReturn = setForwardsForChildren(pf, num1Cpy, num2Cpy, ptrForPrefixNum1, ptrForPrefixNum2, num1Length, num2Length);

    free(num1Cpy);
    free(num2Cpy);

    return toReturn;
}

//----------------------------------------------------------------------------------------------------------------------

void phfwdRemove(PhoneForward *pf, char const *num)
{

}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Inicjujce strukturę PhoneNumbers.
 * Tworzy strukturę przechowującą numery telefonów o pojemności @p n.
 * @param size - pojemność struktury.
 * @return wskaźnik na strukturę lub NULL gdy nie udało się zaalokować pamięci.
 */
static PhoneNumbers* initPhoneNumbers(int size)
{
    PhoneNumbers *pn;

    if ((pn = (PhoneNumbers*)malloc(sizeof(PhoneNumbers))) == NULL)
        return NULL;

    if ((pn->arrOfPN = (char**)malloc(sizeof(char*) * size)) == NULL)
        return NULL;

    pn->numOfPN = size;

    return pn;
}

/** @brief Dodaje do struktury numer.
 * Dodaje do struktury numer @p string na miejsce @p ind.
 * @param pn - struktura przechowująca numery.
 * @param string - wskaźnik na napis.
 * @param ind - indeks miejsca w strukturze.
 * @return wskaźnik na strukturę z dodanym numerem, albo NULL jeśli
 * nie udało się zaalokować pamięci.
 */
static PhoneNumbers* addToPhoneNumbers(PhoneNumbers *pn, char const *string, int ind)
{
    if (!string)
        return NULL;

    char *foo;

    if ((foo = (char*)malloc(sizeof(char) * getStringLength(string))) == NULL)
        return NULL;

    strcpy(foo, string);

    pn->arrOfPN[ind] = foo;

    return pn;
}

/** @brief Łączy dwa stringi.
 * Tworzy nowy napis łącząc całego stringa @p string1 i stringa
 * @p string2 od pozycji @p string2Ind.
 * @param string1 - wskaźnik na pierwszy napis.
 * @param string2 - wskaźnik na drugi napis.
 * @param string2Ind - indeks od którego ma być przepisany @p string2 (włącznie)
 * @return wkaźnik na napis , albo NULL jeśli nie udało się zaalokować pamięci.
 */
static char* mergeStrings(char *string1, char const *string2, int string2Ind)
{
    char *toReturn;             // TODO: być może zmienić po prostu na strlen();
    int stringLength = getStringLength(string1) + getStringLength(string2) - string2Ind;

    if ((toReturn = (char*)malloc(sizeof(char) * stringLength)) == NULL)
        return NULL;

    int i = 0;

    while (string1[i] != '\0')
        toReturn[i] = string1[i++];

    int j = string2Ind;

    while (string2[j] != '\0')
        toReturn[i++] = string2[j++];

    return toReturn;
}

PhoneNumbers const* phfwdGet(PhoneForward *pf, char const *num)
{
    PhoneNumbers *pn = initPhoneNumbers(1);

    if (!pn)
        return NULL;

    if (!hasOnlyDigits(num))
        return NULL;

    int i = 0, ind = 0, biggestInd = 0;

    PhoneForward *biggestPrefix = NULL;

    while (num[i] != '\0')
    {
        ind = charToInt(num[i]);
        pf = pf->nextDigit[ind];

        if (pf->forward != NULL)
        {
            biggestInd = i;
            biggestPrefix = pf;
        }
    }

    if (biggestPrefix == NULL)
        pn = addToPhoneNumbers(pn, num, 0);
    else
        pn = addToPhoneNumbers(pn, mergeStrings(pf->forward, num, biggestInd + 1), 0);

    return pn;
}

//----------------------------------------------------------------------------------------------------------------------

PhoneNumbers const* phfwdReverse(PhoneForward *pf, char const *num)
{

}

//----------------------------------------------------------------------------------------------------------------------

char const* phnumGet(PhoneNumbers const *pnum, size_t idx)
{
    if (!pnum)
        return NULL;

    if (idx >= pnum->numOfPN || idx < 0)
        return NULL;

    char *toReturn;

    if ((toReturn == (char*)malloc(sizeof(char) * getStringLength(pnum->arrOfPN[idx]))) == NULL)
        return NULL;

    strcpy(toReturn, pnum->arrOfPN[idx]);

    return toReturn;
}