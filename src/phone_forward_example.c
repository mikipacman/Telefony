#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward_interface.h"

int main()
{
    PhoneForwardsCenter *phoneForwardsCenter = initPhoneForwardsCenter();
    Instruction instruction;
    PerformResult performResult;
    bool isEOF;

    if (!phoneForwardsCenter)
        return 1;

    atExitCleanPFC(phoneForwardsCenter);

    do
    {
            instruction = getInstruction();
            isEOF = instructionIsEndOfFile(instruction);

            if (instructionIsError(instruction))
                throwError(getErrorFromInstruction(instruction));

            if (!isEOF)
            {
                performResult = performInstruction(instruction, phoneForwardsCenter);

                if (performResultIsError(performResult))
                    throwError(getErrorFromPerormResult(performResult));
            }
            deleteInstruction(instruction);

    } while (!isEOF);


    return 0;
}
