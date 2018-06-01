/** @file
 * Interfejs klasy obsługującej listę zawierającą bazy przekierowań telefonicznych.
 * Lista jest cyklicza, dwukierunkowa i posiada strażnika (na jej początku), który
 * jest rozpoznawany po tym, że jego nazwa to "$". Lista jest posortowana leksykograficznie
 * wzgledem napisów w polach @p id.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __PHONE_FORWARD_BASE_LIST_H__
#define __PHONE_FORWARD_BASE_LIST_H__

#include "phone_forward.h"




/**
 * Struktura listy zawierająca bazy przekierowań telefonicznych.
 */
typedef struct PFBList
{
    PhoneForward *pfBase;   ///< Wskaźnik na bazę przekierowań.
    char *id;               ///< Wskaźnik na napis reprezentujący identyfikator.
    struct PFBList *next;   ///< Wskaźnik na następny element listy.
    struct PFBList *prev;   ///< Wskaźnik na poprzedni element listy.
}PFBList;




/** @brief Tworzy pustą listę.
 * Tworzy pustą listę, zawiera ona jedynie strażnika.
 * @return wskaźnik na pustą listę (jej strażnika) lub NULL jesli nie udało się zaalokować pamięci.
 */
extern PFBList* initPFBList();

/** @brief Usuwa z listy element.
 * Usuwa z listy element o ile nie jest on strażnikiem. W przypadku, gdy
 * @p pfbList jest strażnikiem lub NULL'em nie robi nic.
 * @return TRUE jesli udało się usunąć, wpp FALSE.
 */
extern bool deleteFromPFBList(PFBList *pfbList, char *id);

/** @brief Usuwa całą listę.
 * Usuwa całą listę, łącznie ze strażnikiem.
 * @param pfbList - wskaźnik na dowolny element listy.
 */
extern void deletePFBList(PFBList *pfbList);

/** @brief Dodaje element do listy.
 * Tworzy nowy element listy, nadając mu wartość @p id i tworząc nową bazę przekierowań.
 * Wstawia go tak, by po dodaniu lista znów była posortowana.
 * Zwraca wskaźnik na nowo utworzony node w liście.
 * Jeśli node o takiej nazwie istnieje nie tworzy nowego tylko zwraca adres już istniejącego.
 * Jesli alokacja pamięci się nie uda zwraca NULL.
 * @param pfbList - wskaźnik na strażnika listy;
 * @param id - wskaźnik na napis reprezentujacy identyfikator;
 * @return wskaźnik na nowy node w lisćie lub NULL.
 */
extern PFBList* addToPFBList(PFBList *pfbList, char *id);

#endif //__PHONE_FORWARD_BASE_LIST_H__