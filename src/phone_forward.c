/** @file
 * Implementacja interfejsu klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

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

    /// Tworzymy strukturę.
    /// Jeżeli nie udało się zaalokować pamięci, zwracamy NULL.

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

void phfwdDelete(PhoneForward *pf)
{
    if (!pf)
        return;

    /// Zwalniamy atrybuty node'a.

    free(pf->forward);
    deleteList(pf->listOfFwdToThisNum);

    /// Usuwamy rekurencyjnie jego dzieci.

    for (int i = 0; i < 10; i++)
        if (pf->nextDigit[i] != NULL)
            phfwdDelete(pf->nextDigit[i]);

    /// Zwalniamy node'a.

    free(pf);
}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Usuwa niepotrzebne node'y w gałęzi.
 * Usuwa node'y niepotrzebne, czyli takie które nie mają dzieci, przekirowania i
 * nie są przekierowaniem żadnego innego node'a. Usuwanie zapuszczane jest
 * rekurencyjnie dla ojca node'a. Funkcja "idzie" w stronę korzenia i usuwa co może.
 * @param[in] node - wskaźnik na node'a do usunięcia.
 */
void removeUnnecessaryNodes(PhoneForward *node)
{
    PhoneForward *nextNode;
    if (!node)
        return;

    /// Jeśli dojdziemy do korzenia, przestajemy usuwać.

    if ((nextNode = node->father) == NULL)
        return;

    /// Sprawdzamy czy node ma jakieś dzieci, przekierowanie lub coś jest na niego przekierowane.

    bool hasAnyChildren = false;
    for (int i = 0; i < 10 && !hasAnyChildren; i++)
            if (node->nextDigit[i] != NULL)
                hasAnyChildren = true;

    if (hasAnyChildren)
        return;
    if (!cdListIsEmpty(node->listOfFwdToThisNum))
        return;
    if (node->forward)
        return;

    /// Tutaj wiemy, że node nie ma dzieci, przekierowania i nic nie jest na niego przekierowane.
    /// Zatem możemy go usunąć. Najpierw usuwamy się z dzieci ojca.

    node->father->nextDigit[charToInt(node->currentChar)] = NULL;

    /// Usuwamy atrybuty node'a z node'em.

    deleteList(node->listOfFwdToThisNum);
    node->listOfFwdToThisNum = NULL;
    free(node);

    /// Zapuszczamy rekurencje dla ojca.

    removeUnnecessaryNodes(nextNode);
}

/** @brief Zwraca pustą strukturę PhoneForward.
 * Zwraca wskaźnik na pusty node struktury PhoneForward, czyli taki,
 * który nie ma żadnych przekierowań i nie jest niczyim przekierowaniem.
 * @param[in] c - znak reprezentujacy krawędź pomiędzy nowym node'em, a jego ojcem;
 * @param[in] father - wskaźnik na ojca.
 * @return wskaźnik na strukturę PhoneForward.
 */
static PhoneForward* emptyPhoneForwardNode(char c, PhoneForward *father)
{
    PhoneForward *pf = (PhoneForward*)malloc(sizeof(PhoneForward));

    /// Jeśli nie udało sie zaalokować pamięci zwracamy NULL.

    if (!pf)
        return NULL;
    if ((pf->listOfFwdToThisNum = initCDList()) == NULL)
        return NULL;

    /// Ustawiamy wszystkie atrybuty struktury.

    pf->currentChar = c;
    pf->father = father;
    pf->forward = NULL;
    pf->placeInForwardList = NULL;
    pf->fwdNode = NULL;
    for (int i = 0; i < 10; i++)
        pf->nextDigit[i] = NULL;

    return pf;
}

/** @brief Zwraca wskaźnik na node'a odpowiadającego numerowi.
 * Zwraca wskaźnik na node'a w strukturze @p pf odpowiadającego numerowi @p num.
 * Jeśli odpowiednich node'ów nie ma w strukturze to je tworzy.
 * @param[in] pf - wskaźnik na strukturę PhoneNumber;
 * @param[in] num - wskaźnik na stringa z numerem.
 * @return wskaźnik na node'a odpowiadającemu numerowie
 * lub NULL jeśli nie udało się zaalokować pamięci.
 */
static PhoneForward* getPtrForNumberWithConstructing(PhoneForward *pf, char const* num)
{
    PhoneForward *foo = pf;

    /// Idziemy po ścieżce do node'a reprezentującego napis @p num póki możemy.

    for (int i = 0, ind = 0; num[i] != '\0'; i++)
    {
        ind = charToInt(num[i]);

        if (foo->nextDigit[ind] != NULL)    /// Jeśli jest syn, idziemy dalej.
        {
            foo = foo->nextDigit[ind];
        }
        else    /// Jeśli nie ma syna, tworzymy go i idziemy dalej.
        {
            if ((foo->nextDigit[ind] = emptyPhoneForwardNode(num[i], foo)) == NULL)
                return NULL;
            foo = foo->nextDigit[ind];
        }
    }
    return foo;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{

    /// Jeśli napisy są identyczne lub któryś z nich nie jest numerem, zwracamy false.

    if (strcmp(num1, num2) == 0)
        return false;
    if (!hasOnlyDigits(num1) || !hasOnlyDigits(num2))
        return false;

    PhoneForward *ptrForNodeNum1, *ptrForNodeNum2;  /// Wskaźniki na node'y odpowiadające @p num1 i @p num2.

    /// Jeśli wcześniej node odpowiadający @p num1 nie istnieje to tworzymy go.

    if ((ptrForNodeNum1 = getPtrForNumberWithConstructing(pf, num1)) == NULL)
        return false;

    /// Jeśli wcześniej było przekierowanie usuwamy je i czyścimy ewentualnie powstałe
    /// niepotrzebne node'y na gałezi z poprzednim przekierowaniem.

    if (ptrForNodeNum1->fwdNode != NULL)
    {
        free(ptrForNodeNum1->forward);
        ptrForNodeNum1->forward = NULL;
        deleteFromCDList(ptrForNodeNum1->placeInForwardList);
        removeUnnecessaryNodes(ptrForNodeNum1->fwdNode);
        ptrForNodeNum1->fwdNode = NULL;
    }

    /// Jeśli wcześniej node odpowiadający @p num2 nie istnieje to tworzymy go.

    if ((ptrForNodeNum2 = getPtrForNumberWithConstructing(pf, num2)) == NULL)
        return false;

    /// Ustawiamy nowe przekierowanie czyli podpinamy pod node'a 1 nowy napis i wstawiamy się do listy node'a 2.

    int num1Length = (int)strlen(num1), num2Length = (int)strlen(num2);
    char *forward;
    if ((forward = (char*)malloc(sizeof(char) * (num2Length + 1))) == NULL)
        return false;

    strcpy(forward, num2);
    ptrForNodeNum1->forward = forward;
    ptrForNodeNum1->fwdNode = ptrForNodeNum2;

    if ((ptrForNodeNum1->placeInForwardList = addToCDList(ptrForNodeNum2->listOfFwdToThisNum, num1, num1Length)) == NULL)
        return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Usuwa przekierowania z podrzewa @p node.
 * Usuwa rekurencyjnie przekierowania w poddrzewie (do podrzewa zaliczamy też samego node'a)
 * node'a @p node. Jeśli w wyniku usuwania jakiekolwiek node'y staną się niepotrzebe (nie mają
 * ani dzieci ani przekierowania ani nic się nie nie przekierowywuje) usuwa je.
 * @param[in] node - wskaźnik na node'a, którego poddrzewo mamy usunąć.
 */
void removeNodes(PhoneForward *node)
{
    if (!node)
        return;

    /// Usuwamy przekierowanie i czyścimy ewentualnie powstałe
    /// niepotrzebne node'y na gałezi z poprzednim przekierowaniem.

    if (node->forward != NULL) //TODO: to się potarza też w add.
    {
        free(node->forward);
        node->forward = NULL;
        deleteFromCDList(node->placeInForwardList);
        node->placeInForwardList = NULL;
        removeUnnecessaryNodes(node->fwdNode);
        node->fwdNode = NULL;
    }

    /// Usuwamy rekurencyjnie poddrzewa dzieci node'a.

    for (int i = 0; i < 10; i++)
        if (node->nextDigit[i] != NULL)
            removeNodes(node->nextDigit[i]);

    /// Sprawdzamy czy node ma jakieś dzieci lub coś jest na niego przekierowane.

    bool hasAnyChildren = false;
    for (int i = 0; i < 10 && !hasAnyChildren; i++)
        if (node->nextDigit[i] != NULL)
            hasAnyChildren = true;
    if (hasAnyChildren)
        return;
    if (!cdListIsEmpty(node->listOfFwdToThisNum))
        return;

    /// Jeżeli nie ma dzieci i nic nie jest na niego przekierowane (przekierowania też już nie ma)
    /// i o ile nie jest korzeniem to go usuwamy.

    if (node->father != NULL)
    {
        node->father->nextDigit[charToInt(node->currentChar)] = NULL;
        deleteList(node->listOfFwdToThisNum);
        node->listOfFwdToThisNum = NULL;
        free(node);
    }
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
    /// Jeżeli napis nie jest numerem, nie robimy nic.

    if (!hasOnlyDigits(num))
        return;

    /// Próbujemy znaleźć node odpowiadajacy numerowi @p num.

    PhoneForward *ptrForNum = pf;
    int i = 0, idx = 0;
    while (num[i] != '\0' && ptrForNum != NULL)
    {
        idx = charToInt(num[i++]);
        ptrForNum = ptrForNum->nextDigit[idx];
    }

    /// Jeżeli taki node istnieje, usuwamy rekurencyjnie jego poddrzewo.

    if (ptrForNum)
        removeNodes(ptrForNum);
}

//----------------------------------------------------------------------------------------------------------------------

/** @brief Inicjujce strukturę PhoneNumbers.
 * Tworzy strukturę przechowującą numery telefonów o pojemności @p n.
 * @param[in] size - pojemność struktury.
 * @return wskaźnik na strukturę lub NULL gdy nie udało się zaalokować pamięci.
 */
static PhoneNumbers* initPhoneNumbers(int size)
{
    /// Jeśli nie uda się zaalokować pamięci zwracamy NULL.

    PhoneNumbers *pn;
    if ((pn = (PhoneNumbers*)malloc(sizeof(PhoneNumbers))) == NULL)
        return NULL;
    if ((pn->arrOfPN = (char**)malloc(sizeof(char*) * size)) == NULL)
        return NULL;

    /// Jeśli wszystko się uda ustawiamy rozmiar tablicy i zwracamy strukturę.

    pn->numOfPN = size;
    return pn;
}

/** @brief Dodaje do struktury numer.
 * Dodaje do struktury kopię numeru @p string na miejsce @p idx.
 * @param[in] pn - struktura przechowująca numery;
 * @param[in] string - wskaźnik na napis;
 * @param[in] idx - indeks miejsca w strukturze.
 * @return TRUE jeśli udało się dodać numer, wpp FALSE.
 */
static bool addToPhoneNumbers(PhoneNumbers *pn, char const *string, int idx)
{
    /// Jeśli nie uda się zaalować pamięci lub @p idx wykracza
    /// poza rozmiar tablicy zwracamy false.

    char *foo;
    if (!string)
        return false;
    if (idx >= pn->numOfPN || idx < 0)
        return false;
    if ((foo = (char*)malloc(sizeof(char) * (strlen(string)) + 1)) == NULL)
        return false;

    /// Kopiujemy napis i dodajemy go do tablicy.

    strcpy(foo, string);
    pn->arrOfPN[idx] = foo;

    return true;
}

/** @brief Łączy dwa stringi.
 * Tworzy nowy napis łącząc całego stringa @p string1 i stringa
 * @p string2 od pozycji @p string2Ind (włącznie).
 * @param[in] string1 - wskaźnik na pierwszy napis;
 * @param[in] string2 - wskaźnik na drugi napis;
 * @param[in] string2Ind - indeks od którego ma być przepisany @p string2 (włącznie).
 * @return wkaźnik na napis , albo NULL jeśli nie udało się zaalokować pamięci.
 */
static char* mergeStrings(char *string1, char const *string2, int string2Ind) // TODO: spróbować zrobić to funkcjami ze string.h
{
    /// Dobieramy odpowiednią długość tablicy i próbujemy ją zaalokować.

    char *toReturn;
    int i = 0, j = string2Ind;
    int stringLength = (int)(strlen(string1) + strlen(string2) - string2Ind + 1);   //TODO rozważyc size_t
    if ((toReturn = (char*)malloc(sizeof(char) * stringLength)) == NULL)
        return NULL;

    /// Przepisujemy pierwsze słowo.

    while (string1[i] != '\0')
    {
        toReturn[i] = string1[i];
        i++;
    }

    /// Przepisujemy drugie słowo.

    while (string2[j] != '\0')
    {
        toReturn[i] = string2[j];
        i++;
        j++;
    }

    /// Ustawiamy ostatni znak i zwracamy.

    toReturn[i] = '\0';
    return toReturn;
}

PhoneNumbers const* phfwdGet(PhoneForward *pf, char const *num)
{
    /// Obsługujemy przypadki gdy @p num nie reprezentuje niepustego numeru.

    PhoneNumbers *pn = initPhoneNumbers(1);
    if (!pn)
        return NULL;
    if (!hasOnlyDigits(num))
    {
        pn->arrOfPN[0] = NULL;
        return pn;
    }

    /// Szukamy najdłuższego pasującego do @p num prefiksu, który ma przekierowanie.

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


    if (biggestPrefix == NULL)    /// Jeśli nie znaleziono takiego prefiksu dodajemy do struktury @p num.
    {
        addToPhoneNumbers(pn, num, 0);
    }
    else    /// Jeśli udało się znaleźć, łączymy przekierowany prefiks z resztą numeru i dodajemy do struktury,
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
 * @param[in] arr - wksaźnik na tablicę napisów;
 * @param[in] arrInd - indeks ostatniego napisu, który ma być usunięty
 */
static void freeCharArr(char **arr, int arrInd)
{
    for (int i = 0; i <= arrInd; i++)
        free(arr[i]);

    free(arr);
}

/** @brief Pomocnicza funkcja porównująca.
 * Porównuje dwa napisy pod względem porządku leksograficznego.
 * @param[in] name1 - wskaźnik na pierwszy napis;
 * @param[in] name2 - wskaźnik na drugi napis.
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
 * @param[in] arr - wskaźnik na talicę z której usuwane będą duplikaty;
 * @param[in] arrLength - długość tej tablicy;
 * @param[in,out] newArrLength - wskaźnik na miejsce w którym zostanie zapisana długość zwracanj tablicy.
 * @return wskaźnik na tablicę bez duplikatów lub NULL jeśli nie udało się zaalokować pamięci.
 */
static char** removeDuplicatsFromCharArr(char **arr, int arrLength, int *newArrLength)
{
    /// Jeśli tablica będzie odpowiednio krótka, nie będzie w niej duplikatów.

    (*newArrLength) = arrLength;
    if (arrLength <= 1)
        return arr;

    /// Przechodząc przez tablicę @p arr usuwamy sąsiadujące duplikaty.
    /// Jeśli usuwamy jakiś napis, zwalniamy go, a pozostałe po nim pole
    /// w tablicy ustawiamy na NULL.

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

    /// Jeśli niczego nie usunęliśmy to możemy zwrócić oryginalną tablicę.

    if (numOfRemovedStrings == 0)
        return arr;

    /// Allokujemy nowa tablicę.

    char **toReturn;
    int toReturnArrInd = 0;
    if ((toReturn = (char**)malloc(sizeof(char*) * (arrLength - numOfRemovedStrings))) == NULL)
        return NULL;

    /// Przepisujemy starą tablicę do nowej.

    for (int i = 0; i < arrLength; i++)
        if (arr[i] != NULL)
            toReturn[toReturnArrInd++] = arr[i];

    /// Usuwamy starą tablicę, ustawiamy długość nowej.

    free(arr);
    (*newArrLength) = arrLength - numOfRemovedStrings;
    return toReturn;
}

PhoneNumbers const* phfwdReverse(PhoneForward *pf, char const *num) // TODO: spróbować podzielić na podfunkcje.
{
    /// Obsługujemy przypadek, gdy @p num nie reprezentuje numeru.

    if (!hasOnlyDigits(num))
    {
        PhoneNumbers *pn;
        if ((pn = initPhoneNumbers(1)) == NULL)
            return NULL;
        pn->arrOfPN[0] = NULL;
        return pn;
    }

    /// Liczymy wymaganą długość tablicy jako sumę długości wszystkich list na ścieżce
    /// od korzenia do node'a odpowiadającego numerowi @p num.

    int ind = 0, i = 0, maxArrLength = 0;
    PhoneForward *currentNode = pf;
    while (num[i] != '\0' && currentNode != NULL)
    {
        ind = charToInt(num[i++]);
        currentNode = currentNode->nextDigit[ind];
        if (currentNode != NULL)
            maxArrLength += getCDListLength(currentNode->listOfFwdToThisNum);
    }
    maxArrLength++;     /// Należy pamiętać o tym, że dodajemy też sam numer @p num.

    /// Allokujemy tablicę.

    CDList *currentCDList, *currentGuard;
    int arrInd = 0;
    i = 0;
    currentNode = pf;
    char **currentArray;
    if ((currentArray = (char**)malloc(sizeof(char*) * maxArrLength)) == NULL)
        return NULL;

    /// Dla każdego node'a przepisujemy każdy napis z listy powiększony o sufiks,
    /// który dzieli tego node'a z node'em odpowiadającym numerowi @p num (nawet jeśl taki fizycznie nie istnieje).

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

    /// Dodajemy jeszcze sam numer @p num.

    char *copyOfNum;
    if ((copyOfNum = (char*)malloc(sizeof(char) * (strlen(num) + 1))) == NULL)
    {
        freeCharArr(currentArray, maxArrLength - 2);
        return NULL;
    }
    strcpy(copyOfNum, num);
    currentArray[maxArrLength - 1] = copyOfNum;

    /// Sortujemy.

    qsort(currentArray, (size_t)maxArrLength, sizeof(char*), compareFunction);

    /// Usuwamy duplikaty.

    char **foo;
    int newArrLength = 0;
    if ((foo = removeDuplicatsFromCharArr(currentArray, maxArrLength, &newArrLength)) == NULL)
    {
        freeCharArr(currentArray, maxArrLength - 1);
        return NULL;
    }

    /// Allokujemy strukturę i zwracamy ją.

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
    /// Jeżeli struktura jest NULL'em lub indeks @p idx jest nieodpowiedni zwracamy NULL.
    if (!pnum)
        return NULL;
    if (idx >= (size_t)pnum->numOfPN)
        return NULL;

    return pnum->arrOfPN[idx];
}