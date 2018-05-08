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
    newPF->fwdNode = NULL;
    newPF->father = NULL;

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

void removeUnnecessaryNodes(PhoneForward *node)
{
    if (!node)
        return;

    PhoneForward *nextNode = NULL;

    if ((nextNode = node->father) == NULL) // Będzie tak gdy dojedziemy do korzenia.
        return;

    bool hasAnyChildren = false;

    for (int i = 0; i < 10 && !hasAnyChildren; i++)
            if (node->nextDigit[i] != NULL)
                hasAnyChildren = true;

    if (hasAnyChildren) // jeżeli ma dzieci nie usuwamy
        return;

    if (!cdListIsEmpty(node->listOfFwdToThisNum)) // Jeżeli coś jest na to przekierrowane nie usuwamy
        return;

    if (node->forward) // jeżeli jest na coś przekierowany nie usuwamy
        return;

    // lista jest pusta i nie ma żadnych dzieci  i nie ma przekierowań więc usuwamy go

    node->father->nextDigit[charToInt(node->currentChar)] = NULL; // usuwamy się u ojca

    deleteList(node->listOfFwdToThisNum);
    node->listOfFwdToThisNum = NULL;

    free(node);

    removeUnnecessaryNodes(nextNode); // ewentualnie zwinąć do pętli.
}

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
    pf->fwdNode = NULL;
    pf->father = NULL;

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
static PhoneForward* getPtrForNumberWithConstructing(PhoneForward *pf, char const* num)
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

            foo->nextDigit[ind]->father = foo;
            foo = foo->nextDigit[ind];
        }
    }

    return foo;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{

    if (strcmp(num1, num2) == 0)
        return false;
    if (!hasOnlyDigits(num1) || !hasOnlyDigits(num2))
        return false;

    PhoneForward *ptrForNodeNum1, *ptrForNodeNum2;  // Wskaźniki na node'y odpowiadające num1 i num2.

    if ((ptrForNodeNum1 = getPtrForNumberWithConstructing(pf, num1)) == NULL)
        return false;
    if ((ptrForNodeNum2 = getPtrForNumberWithConstructing(pf, num2)) == NULL)
        return false;


    int num1Length = (int)strlen(num1), num2Length = (int)strlen(num2);
    char *forward;

    if ((forward = (char*)malloc(sizeof(char) * (num2Length + 1))) == NULL)
        return false;

    free(ptrForNodeNum1->forward);
    ptrForNodeNum1->forward = NULL;
    deleteFromCDList(ptrForNodeNum1->placeInForwardList);

    removeUnnecessaryNodes(ptrForNodeNum1->fwdNode);

    ptrForNodeNum1->fwdNode = ptrForNodeNum2;
    strcpy(forward, num2);      // Tworzymy nowy napis.
    ptrForNodeNum1->forward = forward;

    // Dodajemy się na liście node'a 2.
    if ((ptrForNodeNum1->placeInForwardList = addToCDList(ptrForNodeNum2->listOfFwdToThisNum, num1, num1Length)) == NULL)
        return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

void removeNodes(PhoneForward *node)
{

    if (!node)
        return;

    if (node->forward != NULL)
    {
        free(node->forward);
        node->forward = NULL;
        deleteFromCDList(node->placeInForwardList);
        node->placeInForwardList = NULL;
        removeUnnecessaryNodes(node->fwdNode);
        node->fwdNode = NULL;
    }

    for (int i = 0; i < 10; i++)
        if (node->nextDigit[i] != NULL)
            removeNodes(node->nextDigit[i]);
//
    //---------

    bool hasAnyChildren = false;

    for (int i = 0; i < 10 && !hasAnyChildren; i++)
        if (node->nextDigit[i] != NULL)
            hasAnyChildren = true;

    if (hasAnyChildren) // jeżeli ma dzieci nie usuwamy
        return;

    if (!cdListIsEmpty(node->listOfFwdToThisNum)) // Jeżeli coś jest na to przekierrowane nie usuwamy
        return;

    if (node->father != NULL)
    {
        node->father->nextDigit[charToInt(node->currentChar)] = NULL;
    }

    deleteList(node->listOfFwdToThisNum);
    node->listOfFwdToThisNum = NULL;

    free(node);
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
    if (!hasOnlyDigits(num))
        return;

    PhoneForward *ptrForNum = pf;

    int i = 0, idx = 0;

    while (num[i] != '\0' && ptrForNum != NULL)
    {
        idx = charToInt(num[i++]);
        ptrForNum = ptrForNum->nextDigit[idx];
    }

    if (!ptrForNum)
        return;

    // Usuwamy się z list przekierowań na.
    // Oraz jeśli się da usuwamy niepotrzebne gałęzie przekierowań.

    removeNodes(ptrForNum);
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
 * @return TRUE jeśli udało się dodać numer, wpp FALSE.
 */
static bool addToPhoneNumbers(PhoneNumbers *pn, char const *string, int ind)
{
    if (!string)
        return false;

    if (ind >= pn->numOfPN || ind < 0)
        return false;

    char *foo;

    if ((foo = (char*)malloc(sizeof(char) * (strlen(string)) + 1)) == NULL)
        return false;

    strcpy(foo, string);

    pn->arrOfPN[ind] = foo;

    return true;
}

/** @brief Łączy dwa stringi.
 * Tworzy nowy napis łącząc całego stringa @p string1 i stringa
 * @p string2 od pozycji @p string2Ind (włącznie).
 * @param string1 - wskaźnik na pierwszy napis.
 * @param string2 - wskaźnik na drugi napis.
 * @param string2Ind - indeks od którego ma być przepisany @p string2 (włącznie)
 * @return wkaźnik na napis , albo NULL jeśli nie udało się zaalokować pamięci.
 */
static char* mergeStrings(char *string1, char const *string2, int string2Ind)
{
    char *toReturn;
    int stringLength = (int)(strlen(string1) + strlen(string2) - string2Ind + 1);   //TODO rozważyc size_t

    if ((toReturn = (char*)malloc(sizeof(char) * stringLength)) == NULL)
        return NULL;

    int i = 0;

    while (string1[i] != '\0')
    {
        toReturn[i] = string1[i];
        i++;    // poprawić estetycznie
    }


    int j = string2Ind;

    while (string2[j] != '\0')
        toReturn[i++] = string2[j++];

    toReturn[i] = '\0';

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
    ind = charToInt(num[0]);
    pf = pf->nextDigit[ind];
    PhoneForward *biggestPrefix = NULL;

    while (num[i++] != '\0' && pf != NULL)
    {

        if (pf->forward != NULL)
        {
            biggestInd = i;
            biggestPrefix = pf;
        }

        ind = charToInt(num[i]);

        if (0 <= ind && ind <= 9)
            pf = pf->nextDigit[ind];
    }

    if (biggestPrefix == NULL)
    {
        addToPhoneNumbers(pn, num, 0);
    }
    else
    {
        char *toAdd = mergeStrings(biggestPrefix->forward, num, biggestInd);
        addToPhoneNumbers(pn, toAdd, 0);
        free(toAdd);
    }

    return pn;
}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Usuwa tablicę napisów.
 * Usuwa tablicę napisów do indeksu @p arrInd włącznie.
 * Tablica musi być wypełniona na wszystkich polach od 0 do arrInd włącznie i tylko tam.
 * @param arr - wksaźnik na tablicę napisów.
 * @param arrInd - indeks ostatniego napisu, który ma być usunięty
 */
static void freeCharArr(char **arr, int arrInd)
{
    for (int i = 0; i <= arrInd; i++)
        free(arr[i]);

    free(arr);
}

/** @brief Pomocnicza funkcja porównująca.
 * Porównuje dwa napisy pod względem porządku leksograficznego.
 * @param name1 - wskaźnik na pierwszy napis.
 * @param name2 - wskaźnik na drugi napis.
 * @return wynik z porównania funkcją strcmp napisów @p name1, @p name2.
 */
static int compareFunction(const void* name1, const void* name2)
{
    const char* name1_ = *(const char**)name1;
    const char* name2_ = *(const char**)name2;
    return strcmp(name1_, name2_);
}

/** @brief Usuwa duplikaty z posortowanej tablict napisów.
 * Jeśli w tablicy są duplikaty to tworzy nową tablicę napisów, przepisuje
 * tam napisy bez powtórzeń i usuwa starą tablicę.
 * @param arr - wskaźnik na talicę z której usuwane będą duplikaty.
 * @param arrLength - długość tej tablicy.
 * @param newArrLength - wskaźnik na ...
 * @return wskaźnik na tablicę bez duplikatów lub NULL jeśli nie udało się
 * zaalokować pamięci.
 */
static char** removeDuplicatsFromCharArr(char **arr, int arrLength, int *newArrLength)
{
    (*newArrLength) = arrLength;

    if (arrLength <= 1)
        return arr;

    int numOnTheRightInd, numOfRemovedStrings = 0;

    for (int numOnTheLeftInd = 0; numOnTheLeftInd < arrLength;)
    {
        numOnTheRightInd = numOnTheLeftInd + 1;

        while (numOnTheRightInd < arrLength && strcmp(arr[numOnTheLeftInd], arr[numOnTheRightInd]) == 0 )
        {
            numOfRemovedStrings++;
            free(arr[numOnTheRightInd]);
            arr[numOnTheRightInd] = NULL;
            numOnTheRightInd++;
        }

        numOnTheLeftInd = numOnTheRightInd;
    }

    if (numOfRemovedStrings == 0)
        return arr;

    char **toReturn;
    char *stringCopy;

    if ((toReturn = (char**)malloc(sizeof(char*) * (arrLength - numOfRemovedStrings))) == NULL)
        return NULL;

    int toReturnArrInd = 0;

    for (int i = 0; i < arrLength; i++)
    {
        if (arr[i] != NULL)
        {
            if ((stringCopy = (char*)malloc(sizeof(char) * (strlen(arr[i]) + 1))) == NULL)
            {
                freeCharArr(toReturn, toReturnArrInd - 1);
                return NULL;
            }

            strcpy(stringCopy, arr[i]);

            toReturn[toReturnArrInd++] = stringCopy;
        }
    }

    free(arr);

    (*newArrLength) = arrLength - numOfRemovedStrings;

    return toReturn;
}

PhoneNumbers const* phfwdReverse(PhoneForward *pf, char const *num)
{
    int ind = 0, i = 0, maxArrLength = 0;
    PhoneForward *currentNode = pf;

    while (num[i] != '\0' && currentNode != NULL)
    {
        ind = charToInt(num[i++]);
        currentNode = currentNode->nextDigit[ind];

        if (currentNode != NULL)
            maxArrLength += getCDListLength(currentNode->listOfFwdToThisNum);
    }

    maxArrLength ++; // należy pamiętać o tym, że dodajemy też sam numer num.



    char **currentArray;
    if ((currentArray = (char**)malloc(sizeof(char*) * maxArrLength)) == NULL)
        return NULL;

    CDList *currentCDList, *currentGuard;
    int arrInd = 0;
    i = 0;
    currentNode = pf;

    while (num[i] != '\0' && currentNode != NULL)
    {
        ind = charToInt(num[i++]);
        currentNode = currentNode->nextDigit[ind];

        if (currentNode != NULL)
        {
            currentCDList = currentNode->listOfFwdToThisNum->next;
            currentGuard = currentNode->listOfFwdToThisNum;

            while (currentCDList != currentGuard)
            {
                if ((currentArray[arrInd] = mergeStrings(currentCDList->num, num, i)) == NULL)
                {
                    freeCharArr(currentArray, arrInd - 1);
                    return NULL;
                }
                arrInd++;
                currentCDList = currentCDList->next;
            }
        }
    }

    char *copyOfNum;

    if ((copyOfNum = (char*)malloc(sizeof(char) * (strlen(num) + 1))) == NULL)
    {
        freeCharArr(currentArray, maxArrLength - 2);
        return NULL;
    }

    strcpy(copyOfNum, num);
    currentArray[maxArrLength - 1] = copyOfNum;

    qsort(currentArray, (size_t)maxArrLength, sizeof(char*), compareFunction);

    char **foo;
    int newArrLength = 0;

    if ((foo = removeDuplicatsFromCharArr(currentArray, maxArrLength, &newArrLength)) == NULL)
    {
        freeCharArr(currentArray, maxArrLength - 1);
        return NULL;
    }

    PhoneNumbers *pn = (PhoneNumbers*)malloc(sizeof(PhoneNumbers));
    if (!pn)
        return NULL;
    pn->numOfPN = newArrLength;

    pn->arrOfPN = foo;

    return  pn;
}

//----------------------------------------------------------------------------------------------------------------------

char const* phnumGet(PhoneNumbers const *pnum, size_t idx)
{
    if (!pnum)
        return NULL;

//    if (idx < 0)
//        return NULL;

    if (idx >= (size_t)pnum->numOfPN)
        return NULL;

    return pnum->arrOfPN[idx];
}