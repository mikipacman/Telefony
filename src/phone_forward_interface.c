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

/// Typ nazw możliwych rodzaji tokenów.
typedef enum {NEW, DEL, ARROW, Q_MARK, ID, NUM, INVALID_TOKEN, EOF_TOKEN} TokenType;

static void myUngetc(int c)
{
   // if (!isEOF(c)) // TODO:rozkminić ocb.
        ungetc(c, stdin);
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

/** @brief Zlicza sczytane bity.
 * Funkcja zawiera zmienną statyczną, która ma reprezentować numer (począwszy od 1)
 * ostatniego sczytanego ze standardowego wejścia bitu. Funkcja zawsze zwraca aktualny
 * stan tej zmiennej. W zalezności od komendy można:
 * INCREASE - zwiększyć zmienną o 1 i otrzymać jej wartość.
 * GET - nic z nią nie robić, tylko otrzymać jej wartość.
 * @param bitCounterCommand - komenda.
 * @return aktualna wartość zmiennej.
 */
extern size_t bitCounter(BitCounterCommand bitCounterCommand, size_t num)
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

/** @brief Zwraca typ kolejnego tokena.
 * Zwraca typ kolejnego tokena jaki wystepuje na standardowym wejściu.
 * Jeśli typ następnego tokena to ID lub NUM następny znak standardowego wejścja
 * ustawiany jest na pierwszy znak tego tokena (jest on gotowy do szczytania).
 * W przypadku pozostałych tokenów, są one szczytywane w całości ze standardowego wejścia.
 *
 * Zjada tokena i wszystkie komentarze/białe znaki.
 *
 * @return typ kolejnego tokena.
 */
static TokenType getTokenType()
{
    int a = getchar(), b, c, d;
    size_t currentPos = bitCounter(INCREASE, 1);
    bitCounter(SET_LAST_TOKEN_POS_TO, currentPos);

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
                if ((!isDigit(d = getchar())) && (!isLetter(d))) // Musi wystąpić spacja lub komentarz.
                {
                    ungetc(d, stdin);
                    eatAllCommentsAndWhiteSpaces();
                    return NEW;
                }
                myUngetc(d);
            }
            myUngetc(c);
        }
        myUngetc(b);
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
                if ((!isDigit(d = getchar())) && (!isLetter(d))) // Musi wystąpić spacja lub komentarz.
                {
                    ungetc(d, stdin);
                    eatAllCommentsAndWhiteSpaces();
                    return DEL;
                }
                myUngetc(d);
            }
            myUngetc(c);
        }
        myUngetc(b);
        bitCounter(SET_CURRENT_POS_TO, currentPos);
    }
    if (isLetter(a))
    {
        bitCounter(SET_CURRENT_POS_TO, currentPos - 1);
        myUngetc(a);
        return ID;
    }
    if (isDigit(a))
    {
        bitCounter(SET_CURRENT_POS_TO, currentPos - 1);
        myUngetc(a);
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

/** "Zjada" wszystke komentarze i białe znaki.
 * Pobiera znaki ze standardowego wejścia dopóki są one
 * białymi znakami, albo komentarzami. W przypadku komentarzy
 * wczytuje znaki dopóki komentarz nie zostanie "zamknięty".
 * zwraca true gdy coś zjadło.
 */
extern bool eatAllCommentsAndWhiteSpaces()
{
    int c = getchar();
    bool _isComment;
    bool ateSomething = false;

    while (((_isComment = isComment(c)) || isWhiteSpace(c)) && (!isEOF(c)))
    {
        bitCounter(INCREASE, 1);
        ateSomething = true;
        if (_isComment)
        {
            bitCounter(INCREASE, 1);
            while ((!isComment(c = getchar())))
            {
                bitCounter(INCREASE, 1);
                if (isEOF(c))
                {
                    fprintf(stderr, "ERROR EOF");
                    exit(1);
                }
            }

            bitCounter(INCREASE, 2);
        }
        c = getchar();
    }
    myUngetc(c);
    return ateSomething;
}

/// Zwraca instrukcję błędu.
static Instruction errorInstruction(ErrorType errorType, size_t numOfBit)
{
    Instruction instruction;
    instruction.instrName = ERROR;
    instruction.instrType.error.errorType = errorType;
    instruction.instrType.error.charNum = numOfBit;
    return instruction;
}

/// Zwraca instrukcję końca pliku.
static Instruction endOfFile()
{
    Instruction instruction;
    instruction.instrName = END_OF_FILE;
    return instruction;
}

/// Sczytuje ze standardowego wejścia napis numeru.
extern char* getNum()
{
    size_t numSize = 10, currentPosition = 0;
    int currentChar;
    char *num = (char*)malloc(sizeof(char) * numSize);
    if (!num)
        return NULL;

    while (isDigit(currentChar = getchar()))
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
    myUngetc(currentChar);

    if (currentPosition == numSize)
    {
        numSize *= 2;
        num = realloc(num, sizeof(char) * numSize);
        if (!num)
            return NULL;
    }
    num[currentPosition] = '\0';

    bitCounter(INCREASE, currentPosition);

    return num;
}

/// Sczytuje ze standardowego wejścia napis identyfikatora.
extern char* getId()
{
    size_t numSize = 10, currentPosition = 0;
    int currentChar;
    char *num = (char*)malloc(sizeof(char) * numSize);
    if (!num)
        return NULL;

    while (isLetter(currentChar = getchar()) || isDigit(currentChar))
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
    myUngetc(currentChar);

    // Dodajemy znak '\0' na końcu. W miarę potrzeby powiększamy tablicę.
    if (currentPosition == numSize)
    {
        numSize *= 2;
        num = realloc(num, sizeof(char) * numSize);
        if (!num)
            return NULL;
    }
    num[currentPosition] = '\0';

    bitCounter(INCREASE, currentPosition);

    return num;
}

/// Próbuje zbudować instrukcję.
static Instruction tryBuildInstruction(TokenType token[2], char *word[2], int amountOfTokens, size_t posOfToken[2])
{
    Instruction instruction;

    if (amountOfTokens == 3)
    {
        instruction.instrName = ADD_PHFWD;
        instruction.instrType.addPhfwd.num1 = word[0];
        instruction.instrType.addPhfwd.num2 = word[1];
        instruction.posOfOperator = posOfToken[1];
    }
    else if (token[0] == NEW && token[1] == ID)
    {
        instruction.instrName = NEW_PHFWD_BASE;
        instruction.instrType.newPhfwdBase.id = word[1];
        instruction.posOfOperator = posOfToken[0];
    }
    else if (token[0] == DEL && token[1] == ID)
    {
        instruction.instrName = DEL_PHFWD_BASE;
        instruction.instrType.delPhfwdBase.id = word[1];
        instruction.posOfOperator = posOfToken[0];
    }
    else if (token[0] == NUM && token[1] == Q_MARK)
    {
        instruction.instrName = GET_PHFWD;
        instruction.instrType.getPhfwd.num = word[0];
        instruction.posOfOperator = posOfToken[1];
    }
    else if (token[0] == Q_MARK && token[1] == NUM)
    {
        instruction.instrName = REVERSE_PHFWD;
        instruction.instrType.reversePhfwd.num = word[1];
        instruction.posOfOperator = posOfToken[0];
    }
    else if (token[0] == DEL && token[1] == NUM)
    {
        instruction.instrName = DEL_PHFWD;
        instruction.instrType.delPhfwd.num = word[1];
        instruction.posOfOperator = posOfToken[0];
    }
    else
    {
        return errorInstruction(SYNTACTIC_ERROR, posOfToken[1]);
    }
    return instruction;
}

extern Instruction getInstruction()
{
    TokenType token[2];
    int amountOfTokens = 2;
    char *word[2];
    size_t posOfToken[2]; // tablica numerów pierwszych bitów tokenów.

    eatAllCommentsAndWhiteSpaces();

    for (int i = 0; i < 2; i++)
    {
        token[i] = getTokenType();
        posOfToken[i] = bitCounter(GET_LAST_TOKEN_POS, 0);

        switch (token[i])
        {
            case INVALID_TOKEN:
                return errorInstruction(SYNTACTIC_ERROR, posOfToken[i]);
            case EOF_TOKEN:
                if (i == 0)
                    return endOfFile();
                else
                    return errorInstruction(EOF_ERROR, posOfToken[i]);
            case ID:
                if (i == 0)
                    return errorInstruction(SYNTACTIC_ERROR, posOfToken[i]);
                else if ((word[i] = getId()) == NULL)
                    return errorInstruction(OUT_OF_MEMORY_ERROR, posOfToken[i]);
                break;
            case NUM:
                if ((word[i] = getNum()) == NULL)
                    return errorInstruction(OUT_OF_MEMORY_ERROR, posOfToken[i]);
                break;
            case ARROW:
                if (i == 0)
                    return errorInstruction(SYNTACTIC_ERROR, posOfToken[i]);
                break;
            case NEW:
            case DEL:
            case Q_MARK:
            default:
                break;
        }
        eatAllCommentsAndWhiteSpaces();
    }

//    printf("1(%s)\n", word[0]);
//    printf("2(%s)\n", word[1]);

    // Jedyny przypadek gdy instrukcja ma trzy tokeny.
    if (token[0] == NUM && token[1] == ARROW)
    {
        TokenType tokenType = getTokenType();
        size_t numOfThirdToken = bitCounter(GET_LAST_TOKEN_POS, 0);
        if (tokenType == NUM)
        {
            amountOfTokens++;
            word[1] = getNum();
        }
        else if (tokenType == EOF_TOKEN)
        {
            return errorInstruction(EOF_ERROR, numOfThirdToken);
        }
        else
        {
            return errorInstruction(SYNTACTIC_ERROR, numOfThirdToken);
        }
    }
    return tryBuildInstruction(token, word, amountOfTokens, posOfToken);
}

//----------------------------------------------------------------------------------------------------------------------

static PerformResult errorPerformResult(Instruction instruction)//zmienić nazwe
{
    PerformResult performResult;
    performResult.resultName = PERFORM_ERROR;

    switch (instruction.instrName)
    {
        case NEW_PHFWD_BASE:
            performResult.error.errorType = NEW_ERROR;
            break;
        case ADD_PHFWD:
            performResult.error.errorType = ARROW_ERROR;
            break;
        case DEL_PHFWD_BASE:
        case DEL_PHFWD:
            performResult.error.errorType = DEL_ERROR;
            break;
        case GET_PHFWD:
        case REVERSE_PHFWD:
            performResult.error.errorType = Q_MARK_ERROR;
            break;
        default:
            break;
    }
    performResult.error.charNum = instruction.posOfOperator;

    return performResult;
}


extern PerformResult performInstruction(Instruction ins, PhoneForwardsCenter *pfc)
{
    const PhoneNumbers *pn;

    if (pfc->currentBase == NULL)
    {
        switch (ins.instrName)
        {
            case ADD_PHFWD:
            case DEL_PHFWD:
            case GET_PHFWD:
            case REVERSE_PHFWD:
                return errorPerformResult(ins);
            default:
                break;
        }
    }

    switch (ins.instrName)
    {
        case NEW_PHFWD_BASE:
            if (!(pfc->currentBase = addToPFBList(pfc->baseList, ins.instrType.newPhfwdBase.id)))
                return errorPerformResult(ins);
            break;
        case DEL_PHFWD_BASE:
            if (pfc->currentBase && strcmp(pfc->currentBase->id, ins.instrType.delPhfwdBase.id) == 0)
                pfc->currentBase = NULL;
            if (!deleteFromPFBList(pfc->baseList, ins.instrType.delPhfwdBase.id))
                return errorPerformResult(ins);
            break;
        case ADD_PHFWD:
            if (!phfwdAdd(pfc->currentBase->pfBase, ins.instrType.addPhfwd.num1, ins.instrType.addPhfwd.num2))
                return errorPerformResult(ins);
            break;
        case DEL_PHFWD:
            phfwdRemove(pfc->currentBase->pfBase, ins.instrType.delPhfwd.num);
            break;
        case GET_PHFWD:
            pn = phfwdGet(pfc->currentBase->pfBase, ins.instrType.getPhfwd.num);
            if (!pn)
                return errorPerformResult(ins);
            phnumPrint(pn);
            phnumDelete(pn);
            break;
        case REVERSE_PHFWD:
            pn = phfwdReverse(pfc->currentBase->pfBase, ins.instrType.reversePhfwd.num);
            if (!pn)
                return errorPerformResult(ins);
            phnumPrint(pn);
            phnumDelete(pn);
            break;
        case ERROR:
        case END_OF_FILE:
            return errorPerformResult(ins);
            break;
        default:
            break;
    }

    PerformResult pr;
    pr.resultName = OK;
    return pr;
}

//----------------------------------------------------------------------------------------------------------------------

extern bool instructionIsEndOfFile(Instruction instruction)
{
    return instruction.instrName == END_OF_FILE;
}

//----------------------------------------------------------------------------------------------------------------------

extern bool instructionIsError(Instruction instruction)
{
    return instruction.instrName == ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

extern bool performResultIsError(PerformResult performResult)
{
    return performResult.resultName == PERFORM_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

extern Error getErrorFromInstruction(Instruction instruction)
{
    return instruction.instrType.error;
}

//----------------------------------------------------------------------------------------------------------------------

extern Error getErrorFromPerormResult(PerformResult performResult)
{
    return performResult.error;
}

//----------------------------------------------------------------------------------------------------------------------

extern void throwError(Error error)
{
    switch (error.errorType)
    {
        case SYNTACTIC_ERROR:
            fprintf(stderr, "ERROR %zu", error.charNum);
            break;
        case EOF_ERROR:
            fprintf(stderr, "ERROR EOF");
            break;
        case NEW_ERROR:
            fprintf(stderr, "ERROR NEW %zu", error.charNum);
            break;
        case DEL_ERROR:
            fprintf(stderr, "ERROR DEL %zu", error.charNum);
            break;
        case ARROW_ERROR:
            fprintf(stderr, "ERROR > %zu", error.charNum);
            break;
        case Q_MARK_ERROR:
            fprintf(stderr, "ERROR ? %zu", error.charNum);
        default:
            break;
    }
    exit(1);
}

extern void deleteInstruction(Instruction instruction)
{
    switch (instruction.instrName)
    {
        case NEW_PHFWD_BASE:
            free(instruction.instrType.newPhfwdBase.id);
            break;
        case DEL_PHFWD_BASE:
            free(instruction.instrType.delPhfwdBase.id);
            break;
        case ADD_PHFWD:
            free(instruction.instrType.addPhfwd.num1);
            free(instruction.instrType.addPhfwd.num2);
            break;
        case GET_PHFWD:
            free(instruction.instrType.getPhfwd.num);
            break;
        case REVERSE_PHFWD:
            free(instruction.instrType.reversePhfwd.num);
            break;
        case DEL_PHFWD:
            free(instruction.instrType.delPhfwd.num);
            break;
        default:
            break;
    }
}

static void clean(void)
{
    atExitCleanPFC(NULL);
}

// Dodaje rzeczy do usunięcia przy wychodzeniu z programu.
extern void atExitCleanPFC(PhoneForwardsCenter *pfc)
{
    static void *x;

    if (pfc)
    {
        x = pfc;
        atexit(clean);
    }
    else
    {
        deletePhoneForwardCenter(x);
    }
}