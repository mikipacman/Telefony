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

/** @brief Zamienia znak na cyfrę.
 * Zamienia znak @p c na cyfrę.
 * @param[in] c - znak reprezentujący cyfrę.
 * @return cyfra z zakresu [0...9].
 */
extern int charToInt(char c);

// TODO: dokumentacja funkcji poniżej.

extern bool isDigit(int c);

extern bool isLetter(int c);

extern bool isEOF(int c);

extern bool isWhiteSpace(int c);

extern bool isComment(int c);

#endif //__EASY_STRING_H__
