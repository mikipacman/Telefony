/** @file
 * Parę prostych funkcji ułatwiających pracę na stringach.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __EASY_STRING_H__
#define __EASY_STRING_H__

#include <stdbool.h>

/** @brief Określa czy napis składa się cyfr.
 * Określa czy napis składa się z samych cyfr.
 * @param[in] string - wskaźnik na napis.
 * @return TRUE jeśli ma w sobie tylko cyfry, wpp FALSE.
 */
extern bool hasOnlyDigits(char const *string);

/** @brief Sprawdza, czy napis ma jakąś cyfrę.
 * Sprawdza czy napis zawiera w sobie choć jeden znak
 * ze zbioru {1, 2,...9, 0, :, ;}.
 * @param string - wskaźnik na napis.
 * @return TRUE jeśli zawiera, wpp FALSE.
 */
extern bool hasAnyDigits(char const *string);

/** @brief Zamienia znak na cyfrę.
 * Zamienia znak @p c na cyfrę.
 * @param[in] c - znak reprezentujący cyfrę.
 * @return cyfra z zakresu [0...9].
 */
extern int charToInt(char c);

/** @brief Sprawdza czy znak jest cyfrą.
 * @param c - numer znaku
 * @return TRUE jeśli jest, wpp FALSE.
 */
extern bool isDigit(int c);

/** @brief Sprawdza czy znak jest literą.
 * @param c - numer znaku
 * @return TRUE jeśli jest, wpp FALSE.
 */
extern bool isLetter(int c);

/** @brief Sprawdza czy znak jest znakiem końca pliku.
 * @param c - numer znaku
 * @return TRUE jeśli jest, wpp FALSE.
 */
extern bool isEOF(int c);

/** @brief Sprawdza czy znak jest białym znakiem.
 * @param c - numer znaku
 * @return TRUE jeśli jest, wpp FALSE.
 */
extern bool isWhiteSpace(int c);

/** @brief Sprawdza czy znak jest komentarzem.
 * Jeśli znak jest '$' i znak sczytany ze standardowego wejścia też zwraca TRUE.
 * Jeśli ten drugi nie jest '$' to jest on zwracany na standardowe wejście.
 * @param c - numer znaku
 * @return TRUE jeśli jest, wpp FALSE.
 */
extern bool isComment(int c);

#endif //__EASY_STRING_H__
