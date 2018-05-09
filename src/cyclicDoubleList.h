/** @file
 * Strukura cyklicznej dwukierunkowej listy ze strażnikiem zawierającej napisy.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __CYCLIC_DOUBLE_LIST_H__
#define __CYCLIC_DOUBLE_LIST_H__

#include "phone_forward.h"

// TODO: ogarnąć co się dzieje z tymi structami i typedefami.
// TODO: Czemu nie mogę w tym pliku użyć PhoneForward tylko struct PF?

/** @brief Struktura cyklicznej listy.
 * Przyjmujemy, że strażnika, będzie rozpoznawać po tym, że wskaźnik @p num będzie NULL'em.
 */
typedef struct CD
{
    char *num;          ///< Wskaźnik na napis.
    struct CD *next;    ///< Wskaźnik na następny element listy.
    struct CD *prev;    ///< Wskaźnik na  poprzedni element listy.
}CDList;

/** @brief Inicjuje nową listę.
 * Tworzy nową listę z jednym elementem, którym jest strażnik.
 * @return Wskaźnik na nową listę lub NULL, jeśli nie udało się zaalokować pamięci.
 */
extern CDList* initCDList();

/** @brief Dodaje to istniejącej listy podany element.
 * Dodaje do niepustej listy @p list element @p toAdd. Napis @p toAdd jest kopiowany.
 * @param[in] list - wskaźnik na niepustą listę.
 * @param[in] toAdd - wskaźnik na element do dodania (nie może być NULL).
 * @param[in] toAddLength - długość napisu @p toAdd.
 * @return Wskaźnik na nowy element listy lub NULL jeśli
 * nie udało się zaalokować pamięci lub wskaźnik @p toAdd jest NULL.
 */
extern CDList* addToCDList(CDList *list, char const *toAdd, int toAddLength);

/** @brief Usuwa element z listy.
 * Usuwa element @p toDelete z listy w której się on znajduje.
 * @param[in] toDelete - wskaźnik na element do usunięcia.
 */
extern void deleteFromCDList(CDList *toDelete);

/** @brief Określa czy lista jest pusta.
 * Określa czy lista wskazana przez @p list zawiera jedynie strażnika.
 * @param[in] list - wskaźnik na listę.
 * @return TRUE jeśli ma tylko strażnika, wpp FALSE.
 */
extern bool cdListIsEmpty(CDList *list);

/** @brief Usuwa całą listę.
 * Usuwa całą listę włącznie ze strażnikiem.
 * @param[in] list - wskaźnik na listę.
 */
extern void deleteList(CDList *list);

/** @brief Zwraca długość listy.
 * Zwraca ilość elementów znajdujących sie w liście nie licząc strażnika.
 * @param[in] list - wksaźnik na listę.
 * @return długość listy lub -1 gdy parametr @p list to NULL.
 */
extern int getCDListLength(CDList *list);

#endif // __CYCLIC_DOUBLE_LIST_H__