/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 09.04.2018
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "cyclicDoubleList.h"

// TODO: TESTY POZOSTAŁE medium test16 - pamięć. Test [1] from [/home/tit/Programs/IPP/Telefony/IPP-mp394669/release/bigTest/bigtest0]: ./testerka.sh: linia 21: 12457 Unicestwiony
// valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q --log-file="valgrind.txt" ./${2} < ${f} > temp.out
// smallTest 112

/** @brief Struktura przechowująca przekierowania numerów telefonów.
 * Struktura będąca drzewem typu trie, która przechowuje przekierowania
 * numerów telefonów.
 */
typedef struct PF
{
    char currentChar;               ///< Znak odpowiadający krawędzi pomiędzy node'em, a jego ojcem.
    char *forward;                  ///< Wskaźnik na napis reprezentujący przekierowanie.
    struct PF *fwdNode;             ///< Wskaźnik na node'a reprezentującego numer przekierowania.
    struct PF *father;              ///< Wskaźnik na ojca.
    struct PF *nextDigit[10];       ///< Tablica wskaźników na dzieci.
    struct CD *listOfFwdToThisNum;  ///< Wskaźnik na strażnika listy zawierającej numery przekierowań na danego node'a.
    struct CD *placeInForwardList;  /**< Wskaźnik na miejsce napisu odpowiadającego danemu node'owi
                                     * w liście @p listOfFwdToThisNum node'a @p fwdNode.*/
}PhoneForward;

/** @brief Struktura przechowująca ciąg numerów telefonów.
 * Struktura będąca tablicą napisów, która przechowuje numery telefonów.
 */
typedef struct PN
{
    char** arrOfPN;                 ///< Wskaźnik na tablicę napisów.
    int numOfPN;                    ///< Liczba elementów w tablicy.
}PhoneNumbers;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
PhoneForward* phfwdNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * @param[in] pf   – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num1 – wskaźnik na napis reprezentujący prefiks numerów
 *                   przekierowywanych;
 * @param[in] num2 – wskaźnik na napis reprezentujący prefiks numerów, na które
 *                   jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się zaalokować pamięci.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru, nic nie robi.
 *
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest co najwyżej jeden numer. Jeśli dany numer nie został
 * przekierowany, to wynikiem jest ten numer. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers,która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się zaalokować pamięci.
 */
PhoneNumbers const* phfwdGet(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza wszystkie przekierowania na podany numer. Wynikowy ciąg zawiera też
 * dany numer. Wynikowe numery są posortowane leksykograficznie i nie mogą się
 * powtarzać. Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty
 * ciąg. Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się zaalokować pamięci.
 */
PhoneNumbers const* phfwdReverse(PhoneForward *pf, char const *num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
static inline void phnumDelete(PhoneNumbers const *pnum) // TODO: przenieść ewentualnie do gdzie indziej.
{
    if (!pnum)
        return;

    for (int i = 0; i < pnum->numOfPN; i++)
        free(pnum->arrOfPN[i]);

    free(pnum->arrOfPN);
    free((void*)pnum);
}

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg napisów;
 * @param[in] idx  – indeks napisu.
 * @return Wskaźnik na napis. Wartość NULL, jeśli wskaźnik @p pnum ma wartość
 *         NULL lub indeks ma za dużą wartość.
 */
char const* phnumGet(PhoneNumbers const *pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */
