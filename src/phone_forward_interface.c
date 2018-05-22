/** @file
 * Implementacja interfejsu klasy obsługującej tekstową interakcję z klasą do
 * obsługi przekierowań telefonicznych @p phone_forward.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include "phone_forward_interface.h"
#include <stddef.h>

///Typ nazw możliwych instrukcji.
typedef enum {NEW_PHFWD_BASE, DEL_PHFWD_BASE, ADD_PHFWD, DEL_PHFWD, GET_PHFWD, REVERSE_PHFWD, ERROR, END_OF_FILE} InstructionName;

/// Typ nazw możliwych błędów.
typedef enum {SYNTACTIC_ERROR, EOF_ERROR, ADD_ERROR, DEL_ERROR, GET_ERROR, REVERSE_ERROR} ErrorType;

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
}NewPhfwd;

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
struct Error
{
    ErrorType errorType;    ///< Nazwa błędu.
    size_t charNum;         ///< Numer znaku w którym błąd nastąpił.
};

/// Pomocnicza unia struktur.
typedef union
{
    NewPhfwdBase newPhfwdBase;  ///< Informacje o instrukcji dodawania bazy przekierowań.
    DelPhfwdBase delPhfwdBase;  ///< Informacje o instrukcji usuwania bazy przekierowań.
    NewPhfwd newPhfwd;          ///< Informacje o instrukcji dodawania przekierowania.
    DelPhfwd delPhfwd;          ///< Informacje o instrukcji usuwania przekierowania.
    GetPhfwd getPhfwd;          ///< Informacje o instrukcji brania przekierowania numeru.
    ReversePhfwd reversePhfwd;  ///< Informacje o instrukcji brania przekierowań na numer.
    Error error;                ///< Informacje o błędzie.
}InstructionType;

/**
 * Struktura przechowująca infomacje o instrukcji, która ma się wykonać oraz o ewentualnych błędach
 * przechwytywania instrukcji.
 */
struct Instruction
{
    InstructionName instrName;      ///< Nazwa instrukcji.
    InstructionType instrType;      ///< Informacje o instrukcji, w zależności od nazwy instrukcji.
};