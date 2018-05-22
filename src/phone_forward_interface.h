/** @file
 * Interfejs klasy obsługującej tekstową interakcję z klasą do
 * obsługi przekierowań telefonicznych @p phone_forward.
 *
 * // TODO: dodać przykładowe użycie i bardziej rozległy opis.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __PHONE_FORWARD_INTERFACE_H__
#define __PHONE_FORWARD_INTERFACE_H__

#include <stdbool.h>

/**
 * Struktura przechowująca infomacje o instrukcji, która ma się wykonać oraz o ewentualnych błędach
 * przechwytywania instrukcji.
 */
typedef struct Instruction Instruction;

/// Struktura przechowujaca o przebiegu wykonywania instrukcji.
typedef struct PerformResult PerformResult;

/// Struktura przechowująca informacje o błędach.
typedef struct Error Error;

/// Struktura przechowująca bazy przekierowań telefonicznych.
typedef struct PhoneForwardsCenter PhoneForwardsCenter;

/**
 * Tworzy nowe centrum przekierowań. W wypadku gdy nie udało się
 * zaalokować pamięci zwraca NULL.
 * @return wskaźnik na nową strukturę.
 */
extern PhoneForwardsCenter* initPhoneForwardsCenter();

/**
 * Usuwa centrum przekierowań.
 * @param phoneForwardsCenter - wskaźnik na centrum przekierowań.
 */
extern void deletePhoneForwardCenter(PhoneForwardsCenter *phoneForwardsCenter);

/** @brief Pobiera instrukcję.
 * O ile to możliwe sczytuje ze standardowego wejścia instrukcję
 * i zwraca ją. Jeśli nie jest możliwe prawidłowe sczytanie
 * instrukcji przez np. błędy składniowe lub koniec pliku, zwraca odpowiednio
 * instrukcję typu Error (którą można obsłużyć funkcją @p throwError) oraz
 * instrukcję typu EndOfFile.
 * @return instrukcja.
 */
extern Instruction getInstruction();

/** @brief Wykonuje instrukcję.
 * Wykonuje instrukcję @p instruction na centrum przekierowań
 * @p phoneForwardsCenter;
 * @param instruction - instruckja.
 * @param phoneForwardsCenter - wskaźnik na centrum przekierowań.
 */
extern PerformResult performInstruction(Instruction instruction, PhoneForwardsCenter *phoneForwardsCenter);

/**
 * Sprawdza czy instrukcja informuje o końcu pliku.
 * @param instruction - instrukcja.
 * @return TRUE jeśli instrukcja jest typu EndOfFile, wpp FALSE.
 */
extern bool instructionIsEndOfFile(Instruction instruction);

/**
 * Sprawdza czy instrukcja jest błędem.
 * @param instruction - instrukcja.
 * @return TRUE jeśli instrukcja jest błędem, wpp FALSE.
 */
extern bool instructionIsError(Instruction instruction);

/**
 * Sprawdza czy wynik wykonania instrukcji jest błędem.
 * @param performResult - wynik wykonania instrukcji.
 * @return TRUE jeśli wynik wykonania instrukcji jest błęde, wpp FALSE.
 */
extern bool performResultIsError(PerformResult performResult);

/**
 * Pobiera błąd z instrukcji.
 * @param instruction - wskaźnik na instrukcję.
 * @return informacje o błędzie.
 */
extern Error getErrorFromInstruction(Instruction instruction);

/**
 * Pobiera bład z wyniku wykonania instrukcji.
 * @param performResult - wskaźnik na wynik wykonania instrukcji.
 * @return informacje o błędzie.
 */
extern Error getErrorFromPerormResult(PerformResult performResult);

/**
 * Wypisuje na standardowe wyjście diagnostyczne komunikat
 * informujący o błędzie @p error i wychodzi z programu.
 * @param error - informacje o błędzie
 */
extern void throwError(Error error);

// TODO: dodać usuwanie PhoneForwardsCenter atExit.

#endif // __PHONE_FORWARD_INTERFACE_H__