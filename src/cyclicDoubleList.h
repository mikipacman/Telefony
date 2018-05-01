/** @file
 * Strukura cyklicznej dwukierunkowej listy ze strażnikiem zawierającej
 * adresy do struktury PhoneForward.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __CYCLIC_DOUBLE_LIST_H__
#define __CYCLIC_DOUBLE_LIST_H__

#include "phone_forward.h"

/**
 * Struktura listy. Przyjmujemy, że strażnika, będzie rozpoznawać po tym,
 * że wskaźnik @p forward będzie ustawiony na NULL.
 */
typedef struct CD
{
    PhoneForward *forward;
    struct CD *next, *prev;
}CDList;

/** @brief Inicjuje nową listę.
 * Tworzy nową listę z jednym elementem, którym jest strażnik.
 * @return Wskaźnik na nową listę lub NULL, jeśli nie udało się zaalokować pamięci.
 */
extern CDList* initCDList();

/** @brief Dodaje to istniejącej listy podany element.
 * Dodaje do niepustej listy @p list element @p toAdd.
 * @param[in] list - wskaźnik na niepustą listę.
 * @param[in] toAdd - wskaźnik na element do dodania (nie może być NULL).
 * @return Wskaźnik na nowy element listy lub NULL jeśli
 * nie udało się zaalokować pamięci lub wskaźnik @p toAdd jest NULL.
 */
extern CDList* addToCDList(CDList *list, PhoneForward *toAdd);


/** @brief Usuwa element z listy.
 * Usuwa element @p toDelete z listy w której się on znajduje.
 * @param[in] toDelete - wskaźnik na element do usunięcia.
 */
extern void deleteFromCDList(CDList *toDelete);

/** @brief Określa czy lista jest pusta.
 * Określa czy lista wskazana przez @p list zawiera jedynie strażnika.
 * @param list - wskaźnik na listę.
 * @return TRUE jeśli ma tylko strażnika, wpp FALSE.
 */
extern bool cdListIsEmpty(CDList *list);

///** @brief Wypisuje na ekranie listę.
// * Wypisuje na ekranie wszystkie elementy listy łacznie ze strażnikami.
// * @param list - wskaźnik na listę.
// */
//extern void printList(CDList *list);

#endif // __CYCLIC_DOUBLE_LIST_H__