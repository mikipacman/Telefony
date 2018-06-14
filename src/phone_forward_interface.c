/** @file
 * Implementacja interfejsu klasy obsługującej tekstową interakcję z klasą do
 * obsługi przekierowań telefonicznych @p phone_forward.
 *
 * @author Mikołaj Pacek <miki.pacman@gmail.com>
 */

#include "easyString.h"
#include "phone_forward_base_list.h"
#include "phone_forward_interface.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>

/// Typ nazw możliwych rodzaji tokenów.
typedef enum {NEW, DEL, ARROW, Q_MARK, ID, NUM, INVALID_TOKEN, MONKEY, EOF_TOKEN} TokenType;

/// Typ komend do licznika pozycji bitów.
typedef enum {GET_CURRENT_POS, GET_LAST_TOKEN_POS, SET_CURRENT_POS_TO,
    SET_LAST_TOKEN_POS_TO, INCREASE} BitCounterCommand;

/** @brief Licznik sczytanych bitów.
 * Funkcja zawiera dwie zmienne statyczne przechowujące informacje o numerze pierwszego
 * bitu ostatnio sczytanego tokena i o numerze bitu ostatnio sczytanego znaku.
 * Za pomocą komend @p BitCounterCommand można odpowiednio modyfikować i otrzymywać
 * wartosci tych zmiennych:
 *
 * GET_CURRENT_POS - zwraca numer ostatnio sczytanego bitu.
 * GET_LAST_TOKEN_POS - zwraca numer pierwszego bitu ostatnio sczytanego tokena.
 * SET_CURRENT_POS_TO - ustawia numer aktualnego bitu na @p num.
 * SET_LAST_TOKEN_POS_TO - ustawia numer bitu ostatnio sczytanego tokena na @p num.
 * INCREASE - zwiększa numer aktualnego bitu o @p num.
 *
 * Zwracana jest wartość modyfikowanej zmiennej. Czyli w przypadku komendy INCREASE
 * zostanie zwrócony numer ostatnio sczytanego bitu (już po zwiększeniu go).
 *
 * @param[in] bitCounterCommand - komenda;
 * @param[in] num - liczba jako argument do komendy. W przypadku komend typu GET jej
 * wartość nie ma znaczenia.
 * @return wartość odpowiedniej zmiennej (w zalezności od komendy).
 */
static size_t bitCounter(BitCounterCommand bitCounterCommand, size_t num)
{
    static size_t currentPos = 0;
    static size_t lastTokenPos = 0;

    switch (bitCounterCommand)
    {
        case GET_CURRENT_POS:
            return currentPos;
        case GET_LAST_TOKEN_POS:
            return lastTokenPos;
        case SET_CURRENT_POS_TO:
            currentPos = num;
            return currentPos;
        case SET_LAST_TOKEN_POS_TO:
            lastTokenPos = num;
            return lastTokenPos;
        case INCREASE:
            currentPos += num;
            return currentPos;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

extern PhoneForwardsCenter* initPhoneForwardsCenter()
{
    PhoneForwardsCenter *pfc = (PhoneForwardsCenter*)malloc(sizeof(PhoneForwardsCenter));
    if (!pfc)
        return NULL;

    pfc->baseList = initPFBList();
    if (!(pfc->baseList))
    {
        free(pfc);
        return NULL;
    }

    pfc->currentBase = NULL;

    return pfc;
}

//----------------------------------------------------------------------------------------------------------------------

extern void deletePhoneForwardCenter(PhoneForwardsCenter *phoneForwardsCenter)
{
    deletePFBList(phoneForwardsCenter->baseList);
    free(phoneForwardsCenter);
}

//----------------------------------------------------------------------------------------------------------------------

extern Instruction* initInstruction()
{
    Instruction *ins = (Instruction*)malloc(sizeof(Instruction));
    ins->instrName = EMPTY;
    return ins;
}

//----------------------------------------------------------------------------------------------------------------------

extern void deleteInstruction(Instruction *instruction)
{
    clearInstruction(instruction);
    free(instruction);
}

//----------------------------------------------------------------------------------------------------------------------

extern void throwError(ErrorType errorType, size_t charNum)
{
    switch (errorType)
    {
        case SYNTACTIC_ERROR:
            fprintf(stderr, "ERROR %zu\n", charNum);
            break;
        case EOF_ERROR:
            fprintf(stderr, "ERROR EOF\n");
            break;
        case NEW_ERROR:
            fprintf(stderr, "ERROR NEW %zu\n", charNum);
            break;
        case DEL_ERROR:
            fprintf(stderr, "ERROR DEL %zu\n", charNum);
            break;
        case ARROW_ERROR:
            fprintf(stderr, "ERROR > %zu\n", charNum);
            break;
        case Q_MARK_ERROR:
            fprintf(stderr, "ERROR ? %zu\n", charNum);
            break;
        case MONKEY_ERROR:
            fprintf(stderr, "ERROR @ %zu\n", charNum);
            break;
        case OUT_OF_MEMORY_ERROR:
            fprintf(stderr, "OUT OF MEMORY ERROR\n");
            break;
        default:
            break;
    }
    getchar();  // Ten getchar wygląda na niepotrzebny, jednak jest on po to, aby w skrajnym przypadku, kiedy w momencie
    // kiedy strumień wejścia jest pusty, a my wrzucimy na niego ungetc'em jakiś znak, a następnie go
    // sczytamy (takie sytuacje zdażają się gdy na wejściu otrzymujemy "$$$$$") i w tym momencie
    // program się zakończy, musimy sczytać jeszcze jeden znak (czyli już EOF), żeby funkcja
    // (wywoływana wewnątrz ungetc) zwolniła pamięć. Zauważyłem, że jeśli się tego nie zrobi,
    // to funkcja ta nie zwalnia pamięci. Był to jeden z moich najdziwniejszych wycieków pamięci.
    // Jeśli istnieje jakieś bardziej eleganckie rozwiązanie, zachęcam do wprowadzenia go tutaj.
    exit(1);
}

/** @brief "Zjada" wszystke komentarze i białe znaki.
 * Pobiera znaki ze standardowego wejścia dopóki są one
 * białymi znakami, albo komentarzami. W przypadku komentarzy
 * wczytuje znaki dopóki komentarz nie zostanie "zamknięty".
 * Jeśli przed zamknięciem komentarza wystąpi koniec pliku
 * zwraca FALSE.
 * @return FALSE jeśli EOF wystąpi przed zamknięciem komentarza, wpp TRUE.
 */
static bool eatAllCommentsAndWhiteSpaces()
{
    int c = getchar();
    bool _isComment;

    while (((_isComment = isComment(c)) || isWhiteSpace(c)) && (!isEOF(c)))
    {
        bitCounter(INCREASE, 1);

        // Jeśli otworzyliśmy komentarz, "jemy" go aż do zamknięcia albo końca pliku.
        if (_isComment)
        {
            bitCounter(INCREASE, 1);
            while ((!isComment(c = getchar())))
            {
                bitCounter(INCREASE, 1);
                if (isEOF(c))
                    return false;
            }
            bitCounter(INCREASE, 2);
        }
        c = getchar();
    }
    ungetc(c, stdin);

    return true;
}

/** @brief Zwraca typ kolejnego tokena.
 * Zwraca typ kolejnego tokena jaki wystepuje na standardowym wejściu.
 * Jeśli typ następnego tokena to ID lub NUM następny znak standardowego wejścia
 * ustawiany jest na pierwszy znak tego tokena (jest on gotowy do szczytania).
 * W przypadku pozostałych tokenów, są one szczytywane w całości ze standardowego wejścia.
 * @return typ kolejnego tokena.
 */
static TokenType getTokenType()
{
    int a = getchar(), b, c, d;
    size_t currentPos = bitCounter(INCREASE, 1);
    bitCounter(SET_LAST_TOKEN_POS_TO, currentPos);

    if (a == '@')
    {
        return MONKEY;
    }
    if (a == '?')
    {
        return Q_MARK;
    }
    if (a == '>')
    {
        return ARROW;
    }
    if (a == 'N')
    {
        if ((b = getchar()) == 'E')
        {
            bitCounter(INCREASE, 1);
            if ((c = getchar()) == 'W')
            {
                bitCounter(INCREASE, 1);
                if ((!isSpecialDigit(d = getchar())) && (!isLetter(d)))
                {
                    ungetc(d, stdin);
                    return NEW;
                }
                ungetc(d, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(b, stdin);
        bitCounter(SET_CURRENT_POS_TO, currentPos);
    }
    if (a == 'D')
    {
        if ((b = getchar()) == 'E')
        {
            bitCounter(INCREASE, 1);
            if ((c = getchar()) == 'L')
            {
                bitCounter(INCREASE, 1);
                if ((!isSpecialDigit(d = getchar())) && (!isLetter(d)))
                {
                    ungetc(d, stdin);
                    return DEL;
                }
                ungetc(d, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(b, stdin);
        bitCounter(SET_CURRENT_POS_TO, currentPos);
    }
    if (isLetter(a))
    {
        bitCounter(SET_CURRENT_POS_TO, currentPos - 1);
        ungetc(a, stdin);
        return ID;
    }
    if (isSpecialDigit(a))
    {
        bitCounter(SET_CURRENT_POS_TO, currentPos - 1);
        ungetc(a, stdin);
        return NUM;
    }
    if (isEOF(a))
    {
        return EOF_TOKEN;
    }
    else
    {
        return INVALID_TOKEN;
    }
}

/** @brief Sczytuje liczbę.
 * Sczytuje liczbę ze standardowego wejścia. Ustawia zmienną odpowiadającą
 * numerowi pierwszego znaku ostatniego tokena z funkcji @p bitCounter.
 * Po sczytaniu liczby można za pomocą funkcji @p bitCounter odczytać tą zmienną.
 * @return wskaźnik na sczytaną liczbę lub NULL jesli nie udało się zaalokować pamięci.
 */
static char* getNum()
{
    size_t numSize = 10, currentPosition = 0;
    int currentChar;
    char *num = (char*)malloc(sizeof(char) * numSize);
    if (!num)
        return NULL;

    // Sczytujemy cyfry, póki to możliwe, w miarę potrzeb zwiększamy tablice.
    while (isSpecialDigit(currentChar = getchar()))
    {
        if (currentPosition == numSize)
        {
            numSize *= 2;
            num = realloc(num, sizeof(char) * numSize);
            if (!num)
                return NULL;
        }
        num[currentPosition++] = (char)currentChar;
    }
    ungetc(currentChar,stdin);

    // Dopisujemy jeszcze znak '\0'
    if (currentPosition == numSize)
    {
        numSize *= 2;
        num = realloc(num, sizeof(char) * numSize);
        if (!num)
            return NULL;
    }
    num[currentPosition] = '\0';

    // Ustawiamy aktualna pozycję, na ostatnią cyfrę.
    bitCounter(INCREASE, currentPosition);

    return num;
}

/** @brief Sczytuje identyfikator.
 * Sczytuje identyfikator ze standardowego wejścia. Ustawia zmienną odpowiadającą
 * numerowi pierwszego znaku ostatniego tokena z funkcji @p bitCounter.
 * Po sczytaniu liczby można za pomocą funkcji @p bitCounter odczytać tą zmienną.
 * @return wskaźnik na sczytany identyfikator lub NULL jeśli nie udało się zaalokować pamięci.
 */
static char* getId()
{
    size_t numSize = 10, currentPosition = 0;
    int currentChar;
    char *num = (char*)malloc(sizeof(char) * numSize);
    if (!num)
        return NULL;

    // Sczytujemy cyfry i litery, póki to możliwe, w miarę potrzeb zwiększamy tablice.
    while (isLetter(currentChar = getchar()) || isdigit(currentChar))
    {

        if (currentPosition == numSize)
        {
            numSize *= 2;
            num = realloc(num, sizeof(char) * numSize);
            if (!num)
                return NULL;
        }
        num[currentPosition++] = (char)currentChar;
    }
    ungetc(currentChar, stdin);

    // Dodajemy znak '\0' na końcu. W miarę potrzeby powiększamy tablicę.
    if (currentPosition == numSize)
    {
        numSize *= 2;
        num = realloc(num, sizeof(char) * numSize);
        if (!num)
            return NULL;
    }
    num[currentPosition] = '\0';

    // Ustawiamy aktualną pozycję, na ostatni znak.
    bitCounter(INCREASE, currentPosition);

    return num;
}

/// Zwalnia pamięć z napisów zawartych w tablicy, ale nie samą tablicę.
static void freeStringArray(char **arr, int length)
{
    for (int i = 0; i < length; i++)
        if (arr[i] != NULL)
            free(arr[i]);
}

/** @brief Próbuje utworzyć instrkucję.
 * Próbuje utworzyć instrukcję, na podstawie schematu tokenów.
 * Jeśli schemat nie pasuje, rzuca błąd syntaktyczny drugiego tokena.
 * @param[in,out] instruction - wskaźnik na instrukcję, która będzie nadpisana;
 * @param[in] token - tablica tokenów;
 * @param[in] word - tablica sczytanych słów (numery lub ID);
 * @param[in] amountOfTokens - liczba tokenów to 2 lub 3;
 * @param[in] posOfToken - tablica pozycji tokenów.
 * @return instrukcja gotowa do wykonania funkcją @p performInstruction.
 */
static void tryBuildInstruction(Instruction *instruction, TokenType const token[2], char *word[2],
                                int amountOfTokens, size_t posOfToken[2])
{
    if (amountOfTokens == 3)
    {
        instruction->instrName = ADD_PHFWD;
        instruction->instrType.addPhfwd.num1 = word[0];
        instruction->instrType.addPhfwd.num2 = word[1];
        instruction->posOfOperator = posOfToken[1];
    }
    else if (token[0] == NEW && token[1] == ID)
    {
        instruction->instrName = NEW_PHFWD_BASE;
        instruction->instrType.newPhfwdBase.id = word[1];
        instruction->posOfOperator = posOfToken[0];
    }
    else if (token[0] == DEL && token[1] == ID)
    {
        instruction->instrName = DEL_PHFWD_BASE;
        instruction->instrType.delPhfwdBase.id = word[1];
        instruction->posOfOperator = posOfToken[0];
    }
    else if (token[0] == NUM && token[1] == Q_MARK)
    {
        instruction->instrName = GET_PHFWD;
        instruction->instrType.getPhfwd.num = word[0];
        instruction->posOfOperator = posOfToken[1];
    }
    else if (token[0] == Q_MARK && token[1] == NUM)
    {
        instruction->instrName = REVERSE_PHFWD;
        instruction->instrType.reversePhfwd.num = word[1];
        instruction->posOfOperator = posOfToken[0];
    }
    else if (token[0] == DEL && token[1] == NUM)
    {
        instruction->instrName = DEL_PHFWD;
        instruction->instrType.delPhfwd.num = word[1];
        instruction->posOfOperator = posOfToken[0];
    }
    else if (token[0] == MONKEY && token[1] == NUM)
    {
        instruction->instrName = COUNT_NON_TRIVIAL;
        instruction->instrType.countNonTrivial.num = word[1];
        instruction->posOfOperator = posOfToken[0];
    }
    else
    {
        freeStringArray(word, 2);
        throwError(SYNTACTIC_ERROR, posOfToken[1]);
    }
}

/// Funkcja pomocnicza do @p getInstruction.
static inline void throwErrorAndFreeArr(ErrorType errorType, size_t numOfChar, char **arr, int sizeOfCharArr)
{
    freeStringArray(arr, sizeOfCharArr);
    throwError(errorType, numOfChar);
}

extern bool getInstruction(Instruction *instruction)
{
    TokenType token[2];
    int amountOfTokens = 2;
    char *word[2];
    word[0] = NULL;
    word[1] = NULL;
    size_t posOfToken[2]; // tablica numerów pierwszych bitów tokenów.
    clearInstruction(instruction);

    // Zjadamy komentarze i białe znaki.
    if (!eatAllCommentsAndWhiteSpaces())
        throwError(EOF_ERROR, 0);

    // Sczytujemy dwa tokeny, lub rzucamy błędem w momencie gdy jest już oczywisty.
    for (int i = 0; i < 2; i++)
    {
        token[i] = getTokenType();
        posOfToken[i] = bitCounter(GET_LAST_TOKEN_POS, 0);

        switch (token[i])
        {
            case INVALID_TOKEN:
                throwErrorAndFreeArr(SYNTACTIC_ERROR, posOfToken[i], word, 2);
                break;
            case EOF_TOKEN:
                if (i == 0)
                    return false;   // Jedyny przypadek gdy zwracamy false.
                else
                    throwErrorAndFreeArr(EOF_ERROR, posOfToken[i], word, 2);
                break;
            case ID:
                if (i == 0)
                    throwError(SYNTACTIC_ERROR, posOfToken[i]);
                else if ((word[i] = getId()) == NULL)
                    throwErrorAndFreeArr(OUT_OF_MEMORY_ERROR, posOfToken[i], word, 2);
                break;
            case NUM:
                if ((word[i] = getNum()) == NULL)
                    throwErrorAndFreeArr(OUT_OF_MEMORY_ERROR, posOfToken[i], word, 2);
                break;
            case ARROW:
                if (i == 0)
                    throwError(SYNTACTIC_ERROR, posOfToken[i]);
                break;
            case NEW:
            case DEL:
            case Q_MARK:
            case MONKEY:
            default:
                break;
        }
        if (i == 0 && !eatAllCommentsAndWhiteSpaces())
            throwErrorAndFreeArr(EOF_ERROR, posOfToken[i], word, 2);
    }

    // Jedyny przypadek gdy instrukcja ma trzy tokeny.
    if (token[0] == NUM && token[1] == ARROW)
    {
        if (!eatAllCommentsAndWhiteSpaces())
            throwErrorAndFreeArr(EOF_ERROR, posOfToken[1], word, 2);

        TokenType tokenType = getTokenType();
        size_t numOfThirdToken = bitCounter(GET_LAST_TOKEN_POS, 0);
        if (tokenType == NUM)
        {
            amountOfTokens++;
            if ((word[1] = getNum()) == NULL)
            {
                freeStringArray(word, 2);
                throwError(OUT_OF_MEMORY_ERROR, bitCounter(GET_LAST_TOKEN_POS, 0));
            }
        }
        else if (tokenType == EOF_TOKEN)
        {
            throwErrorAndFreeArr(EOF_ERROR, numOfThirdToken, word, 2);
        }
        else
        {
            throwErrorAndFreeArr(SYNTACTIC_ERROR, numOfThirdToken, word, 2);
        }
    }
    // Próbujemy zbudować instrukcję z tokenów.
    tryBuildInstruction(instruction, token, word, amountOfTokens, posOfToken);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

extern void performInstruction(Instruction *ins, PhoneForwardsCenter *pfc)
{
    const PhoneNumbers *pn;

    // Obsługujemy przypadek, gdy nie ma ustawionej aktualnej bazy przekierowań.
    if (pfc->currentBase == NULL)
    {
        switch (ins->instrName)
        {
            case ADD_PHFWD:
                throwError(ARROW_ERROR, ins->posOfOperator);
                break;
            case DEL_PHFWD:
                throwError(DEL_ERROR, ins->posOfOperator);
                break;
            case GET_PHFWD:
                throwError(Q_MARK_ERROR, ins->posOfOperator);
                break;
            case REVERSE_PHFWD:
                throwError(Q_MARK_ERROR, ins->posOfOperator);
                break;
            case COUNT_NON_TRIVIAL:
                throwError(MONKEY_ERROR, ins->posOfOperator);
                break;
            default:
                break;
        }
    }

    size_t temp = 0;

    // Wykonujemy odpowiednie funkcje.
    switch (ins->instrName)
    {
        case NEW_PHFWD_BASE:
            if (!(pfc->currentBase = addToPFBList(pfc->baseList, ins->instrType.newPhfwdBase.id)))
                throwError(NEW_ERROR, ins->posOfOperator);
            break;
        case DEL_PHFWD_BASE:
            if (pfc->currentBase && strcmp(pfc->currentBase->id, ins->instrType.delPhfwdBase.id) == 0)
                pfc->currentBase = NULL;
            if (!deleteFromPFBList(pfc->baseList, ins->instrType.delPhfwdBase.id))
                throwError(DEL_ERROR, ins->posOfOperator);
            break;
        case ADD_PHFWD:
            if (!phfwdAdd(pfc->currentBase->pfBase, ins->instrType.addPhfwd.num1, ins->instrType.addPhfwd.num2))
                throwError(ARROW_ERROR, ins->posOfOperator);
            break;
        case DEL_PHFWD:
            phfwdRemove(pfc->currentBase->pfBase, ins->instrType.delPhfwd.num);
            break;
        case GET_PHFWD:
            pn = phfwdGet(pfc->currentBase->pfBase, ins->instrType.getPhfwd.num);
            if (!pn)
                throwError(Q_MARK_ERROR, ins->posOfOperator);
            phnumPrint(pn);
            phnumDelete(pn);
            break;
        case REVERSE_PHFWD:
            pn = phfwdReverse(pfc->currentBase->pfBase, ins->instrType.reversePhfwd.num);
            if (!pn)
                throwError(Q_MARK_ERROR, ins->posOfOperator);
            phnumPrint(pn);
            phnumDelete(pn);
            break;
        case COUNT_NON_TRIVIAL:
            temp = strlen(ins->instrType.countNonTrivial.num);
            if (temp > NUM_OF_DIGITS)
                temp -= NUM_OF_DIGITS;
            else
                temp = 0;
            printf("%zu\n", phfwdNonTrivialCount(pfc->currentBase->pfBase, ins->instrType.countNonTrivial.num, temp));
            break;
        case END_OF_FILE:
            throwError(EOF_ERROR, ins->posOfOperator);
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

extern void clearInstruction(Instruction *instruction)
{
    switch (instruction->instrName)
    {
        case NEW_PHFWD_BASE:
            free(instruction->instrType.newPhfwdBase.id);
            break;
        case DEL_PHFWD_BASE:
            free(instruction->instrType.delPhfwdBase.id);
            break;
        case ADD_PHFWD:
            free(instruction->instrType.addPhfwd.num1);
            free(instruction->instrType.addPhfwd.num2);
            break;
        case GET_PHFWD:
            free(instruction->instrType.getPhfwd.num);
            break;
        case REVERSE_PHFWD:
            free(instruction->instrType.reversePhfwd.num);
            break;
        case DEL_PHFWD:
            free(instruction->instrType.delPhfwd.num);
            break;
        case COUNT_NON_TRIVIAL:
            free(instruction->instrType.countNonTrivial.num);
            break;
        case EMPTY:
        default:
            break;
    }
    instruction->instrName = EMPTY;
}

//----------------------------------------------------------------------------------------------------------------------

/// Funkcja pomocnicza do funkcji @p atExitCleanPFC.
static void clean(void)
{
    atExitClean(NULL, NULL);
}

extern void atExitClean(PhoneForwardsCenter *pfc, Instruction *ins)
{
    static PhoneForwardsCenter *x;
    static Instruction *y;


    if (pfc != NULL || ins != NULL)
    {
        x = pfc;
        y = ins;
        atexit(clean);
    }
    else
    {
        deletePhoneForwardCenter(x);
        deleteInstruction(y);
    }
}
