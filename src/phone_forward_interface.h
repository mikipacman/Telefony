/** @file
 * Interfejs klasy obsługującej tekstową interakcję z klasą do
 * obsługi przekierowań telefonicznych @p phone_forward.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#ifndef __PHONE_FORWARD_INTERFACE_H__
#define __PHONE_FORWARD_INTERFACE_H__

#include <stdbool.h>
#include "phone_forward_base_list.h"

//----------------------------------------------------------------------------------------------------------------------

///Typ nazw możliwych instrukcji.
typedef enum {NEW_PHFWD_BASE, DEL_PHFWD_BASE, ADD_PHFWD, DEL_PHFWD,
                        GET_PHFWD, REVERSE_PHFWD, COUNT_NON_TRIVIAL, END_OF_FILE, EMPTY} InstructionName;

/// Typ nazw możliwych błędów.
typedef enum {SYNTACTIC_ERROR, EOF_ERROR, NEW_ERROR, DEL_ERROR,
                ARROW_ERROR, Q_MARK_ERROR, OUT_OF_MEMORY_ERROR, MONKEY_ERROR} ErrorType;

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

/// Struktura przechowująca informacje o instrukcji zliczania nietrywialnych numerów.
typedef struct
{
    char *num;          ///< Wskaźnik na napis reprezentujący zbiór dozwolonych cyfr.
}CountNonTrivial;

/// Pomocnicza unia struktur.
typedef union
{
    NewPhfwdBase newPhfwdBase;          ///< Informacje o instrukcji dodawania bazy przekierowań.
    DelPhfwdBase delPhfwdBase;          ///< Informacje o instrukcji usuwania bazy przekierowań.
    AddPhfwd addPhfwd;                  ///< Informacje o instrukcji dodawania przekierowania.
    DelPhfwd delPhfwd;                  ///< Informacje o instrukcji usuwania przekierowania.
    GetPhfwd getPhfwd;                  ///< Informacje o instrukcji brania przekierowania numeru.
    ReversePhfwd reversePhfwd;          ///< Informacje o instrukcji brania przekierowań na numer.
    CountNonTrivial countNonTrivial;    ///< Informacje o instrukcji zliczania nietrywialnych numerów.
}InstructionType;

/**
 * Struktura przechowująca informacje o instrukcji, która ma się wykonać oraz o ewentualnych błędach
 * przechwytywania instrukcji.
 */
typedef struct
{
    InstructionName instrName;      ///< Nazwa instrukcji.
    InstructionType instrType;      ///< Informacje o instrukcji, w zależności od nazwy instrukcji.
    size_t posOfOperator;           ///< Numer pierwszego bitu operatora, o ile instrukcja nie jest EOF'em.
}Instruction;

/**
 * Struktura przechowująca bazy przekierowań telefonicznych.
 */
typedef struct
{
    PFBList *baseList;      ///< Wskaźnik na strażnika listy wszystkich baz przekierowań.
    PFBList *currentBase;   ///< Wskaźnik na node'a bierzącej bazy przekierowań.
}PhoneForwardsCenter;

//----------------------------------------------------------------------------------------------------------------------

/** @brief Tworzy nowe centrum baz przekierowań.
 * Tworzy nowe centrum baz przekierowań. Wprzypadku gdy nie udało się
 * zaalokować pamięci zwraca NULL.
 * @return wskaźnik na nową strukturę.
 */
extern PhoneForwardsCenter* initPhoneForwardsCenter();

/** @brief Usuwa centrum przekierowań.
 * @param[in] phoneForwardsCenter - wskaźnik na centrum przekierowań.
 */
extern void deletePhoneForwardCenter(PhoneForwardsCenter *phoneForwardsCenter);

/** @brief Tworzy pustą instrukcję.
 * @return wskaźnik na instrukcję.
 */
extern Instruction* initInstruction();

/** @brief Usuwa całą instrukcję.
 * @param instruction - wskaźnik na instrukcję.
 */
extern void deleteInstruction(Instruction *instruction);

/** @brief Pobiera instrukcję.
 * O ile to możliwe sczytuje ze standardowego wejścia instrukcję
 * i zwraca ją. Jeśli nie jest możliwe prawidłowe sczytanie
 * instrukcji przez błędy składniowe, wypisuje odpowiedni komunikat
 * na standardowe wyjście diagnostyczne i kończy działanie programu.
 * @param[in, out] instruction - wskaźnik na instrukcję, na którą
 * zostaną zapisane informacje.
 * @return TRUE jeśli poprawnie sczytano instrukcję, FALSE jeśli nie ma już
 * instrukcji do sczytania czyli wystąpił EOF.
 */
extern bool getInstruction(Instruction *instruction);

/** @brief Usuwa instrukcję.
 * Usuwa z instrukcji wszystkie zaalokowane napisy, ale nie
 * samą instrukcję.
 * @param[in] instruction - instrukcja do wyczyszczenia.
 */
extern void clearInstruction(Instruction *instruction);

/** @brief Wykonuje instrukcję.
 * Wykonuje instrukcję @p instruction na centrum baz przekierowań @p phoneForwardsCenter.
 * @param[in] instruction - instrukcja;
 * @param[in,out] phoneForwardsCenter - wskaźnik na centrum baz przekierowań.
 */
extern void performInstruction(Instruction *instruction, PhoneForwardsCenter *phoneForwardsCenter);

/** @brief Dodaje centrum baz przekierowań i instrukcję do @p atexit.
 * Dodaje centrum baz przekierowań i instrukcję do listy struktur, które zostaną automatycznie
 * usunięte przy zakończeniu programu.
 * @param[in] pfc - wskaźnik na centrum baz przekierowań;
 * @param[in] ins - wskaźnik na instrukcję.
 */
extern void atExitClean(PhoneForwardsCenter *pfc, Instruction *ins);

/** @brief Wyrzuca błąd.
 * W zależności o rodzaju błędu wypisuje na standardowe wyjście diagnostyczne
 * stosowny komunikat o błędzie i kończy działanie programu.
 * @param[in] errorType - rodzaj błędu;
 * @param[in] charNum - numer bitu w którym wystąpił błąd.
 */
extern void throwError(ErrorType errorType, size_t charNum);

#endif // __PHONE_FORWARD_INTERFACE_H__