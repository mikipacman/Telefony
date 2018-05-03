/** @file
 * Parę prostych funkcji ułatwiających pracę na stringach.
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __EASY_STRING_H__
#define __EASY_STRING_H__

#include <stdbool.h>

/** @brief Zwraca wskaźnik na pustego stringa
 * Tworzy, a nastepnie zwraca wskaźnik na pustego stringa.
 * @return wskaźnik na pustego stringa.
 */
extern char* emptyString();

/** @brief Dodaj znak do stringa.
 * Tworzy nowy napis string ze string @p string i znaku @p c (dodaje go na koniec).
 * @param string - wskaźnik na napis.
 * @param c - znak.
 * @return wskaźnik na nowy string.
 */
extern char* addToString(char *string, char c);

/** @brief Określa czy napis składa się cyfr.
 * Określa czy napis składa się z samych cyfr.
 * @param[in] string - wskaźnik na napis.
 * @return TRUE jeśli ma w sobie tylko cyfry, wpp FALSE.
 */
extern bool hasOnlyDigits(char const *string);

/** @brief Zamienia znak na cyfrę.
 * Zamienia znak @p c na cyfrę.
 * @param c - znak reprezentujący cyfrę.
 * @return cyfra z zakresu [0...9].
 */
extern int charToInt(char c);

/** @brief Zwraca długość stringa.
 * Zwraca długość stringa.
 * @param string - wskaźnik na napis.
 * @return długość napisu.
 */
extern int getStringLength(char const *string);

/** @brief Dodaj znak to stringa.
 * Zwraca nowy napis stworzony ze stringa i znaku.
 * @param[in] string - wskaźnik na napis.
 * @param[in] c - znak.
 * @param[in] - długość @p string.
 * @return wskaźnik na nowy napis lub NULL jeśli nie udało się zaalokować pamięci.
 */
extern char* addCharToString(char *string, char c, int stringLength);

/** @brief Zmienia cyfrę na znak.
 * Zamienia cyfrę na znak.
 * @param i - cyfra.
 * @return znak odpowiadający cyfrze.
 */
extern char intToChar(int i);

#endif //__EASY_STRING_H__
