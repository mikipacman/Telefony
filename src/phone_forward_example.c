#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward_interface.h"

int main()
{
    PhoneForwardsCenter *phoneForwardsCenter = initPhoneForwardsCenter();
    Instruction instruction;
    PerformResult performResult;

    if (!phoneForwardsCenter)
        return 1;

    do
    {
            instruction = getInstruction();

            if (instructionIsError(instruction))
                throwError(getErrorFromInstruction(instruction));

            if (!instructionIsEndOfFile(instruction))
            {
                performResult = performInstruction(instruction, phoneForwardsCenter);

                if (performResultIsError(performResult))
                    throwError(getErrorFromPerormResult(performResult));
            }

    } while (!instructionIsEndOfFile(instruction));

    deletePhoneForwardCenter(phoneForwardsCenter);

    return 0;
}
