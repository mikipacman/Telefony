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
#include "phone_forward_base_list.h"


extern char* getId();extern char* getNum();extern bool eatAllCommentsAndWhiteSpaces();

///Typ nazw możliwych instrukcji.
typedef enum {NEW_PHFWD_BASE, DEL_PHFWD_BASE, ADD_PHFWD, DEL_PHFWD,
                        GET_PHFWD, REVERSE_PHFWD, ERROR, END_OF_FILE} InstructionName;

/// Typ nazw możliwych błędów.
typedef enum {SYNTACTIC_ERROR, EOF_ERROR, NEW_ERROR, DEL_ERROR, ARROW_ERROR, Q_MARK_ERROR, OUT_OF_MEMORY_ERROR, TRIED_TO_PERFORM_EOF_ERROR} ErrorType;

/// Struktura przechowująca informacje o instrukcji dodawania bazy przekierowań.
typedef struct
{
    char *id;           ///< Wskaźnik na napis reprezentujący identyfikator.
}NewPhfwdBase;

/// Struktura przechowująca informacje o instrukcji usuwania bazy przekierowań.
typedef struct
{
    char *id;           ///< Wskaźnik na napis reprezentujący identyfikator.
}DelPhfwdBase;

/// Struktura przechowująca informacje o instrukcji dodawania przekierowania.
typedef struct
{
    char *num1;         ///< Wskaźnik na napis reprezentujący numer do przekierowania.
    char *num2;         ///< Wskaźnik na napis reprezentujący numer na który ma nastąpić przekierowanie.
}AddPhfwd;

/// Struktura przechowująca informacje o instrukcji usuwania przekierowania.
typedef struct
{
    char *num;          ///< Wskaźnik na napis reprezentujący prefiks numerów, których przekierowania zostaną usunięte.
}DelPhfwd;

/// Struktura przechowująca informacje o instrukcji brania przekierowania numeru.
typedef struct
{
    char *num;          ///< Wskaźnik na napis reprezentujący numer z którego zostanie podane przekierowanie.
}GetPhfwd;

/// Struktura przechowująca informacje o instrukcji brania przekierowań na numer.
typedef struct
{
    char *num;          ///< Wskaźnik na napis reprezentujący numer na który zostaną wygenerowane przekierowania.
}ReversePhfwd;

/// Struktura przechowująca informacje o błędzie.
typedef struct
{
    ErrorType errorType;    ///< Nazwa błędu.
    size_t charNum;         ///< Numer znaku w którym błąd nastąpił.
}Error;

/// Pomocnicza unia struktur.
typedef union
{
    NewPhfwdBase newPhfwdBase;  ///< Informacje o instrukcji dodawania bazy przekierowań.
    DelPhfwdBase delPhfwdBase;  ///< Informacje o instrukcji usuwania bazy przekierowań.
    AddPhfwd addPhfwd;          ///< Informacje o instrukcji dodawania przekierowania.
    DelPhfwd delPhfwd;          ///< Informacje o instrukcji usuwania przekierowania.
    GetPhfwd getPhfwd;          ///< Informacje o instrukcji brania przekierowania numeru.
    ReversePhfwd reversePhfwd;  ///< Informacje o instrukcji brania przekierowań na numer.
    Error error;                ///< Informacje o błędzie.
}InstructionType;

/**
 * Struktura przechowująca infomacje o instrukcji, która ma się wykonać oraz o ewentualnych błędach
 * przechwytywania instrukcji.
 */
typedef struct
{
    InstructionName instrName;      ///< Nazwa instrukcji.
    InstructionType instrType;      ///< Informacje o instrukcji, w zależności od nazwy instrukcji.
    size_t posOfOperator;           ///< Numer pierwszego bitu operatora, o ile instrukcja nie jest błędem lub eof.
}Instruction;

/// Typ nazw możliwych wyników wykonywania instrukcji.
typedef enum {OK, PERFORM_ERROR} PerformResultName;

/**
 * Struktura przechowująca informacje o przebiegu wykonywania instrukcji.
 */
typedef struct
{
    PerformResultName resultName;    ///< Nazwa wyniku wykonania instrukcji.
    Error error;                     ///< Ewentualne informacje o błędzie.
}PerformResult;

/**
 * Struktura przechowująca bazy przekierowań telefonicznych.
 */
typedef struct
{
    PFBList *baseList;
    PFBList *currentBase;
}PhoneForwardsCenter;




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

/// Typ komend do licznika pozycji bitów.
typedef enum {GET_CURRENT_POS, GET_LAST_TOKEN_POS, SET_CURRENT_POS_TO, SET_LAST_TOKEN_POS_TO, INCREASE} BitCounterCommand;

extern size_t bitCounter(BitCounterCommand bitCounterCommand, size_t num);

/// Usuwa instrukcję
extern void deleteInstruction(Instruction instruction);

/** @brief Wykonuje instrukcję.
 * Wykonuje instrukcję @p instruction na centrum przekierowań
 * @p phoneForwardsCenter.
 * @param instruction - instruckja;
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

extern void atExitCleanPFC(PhoneForwardsCenter *pfc);

#endif // __PHONE_FORWARD_INTERFACE_H__