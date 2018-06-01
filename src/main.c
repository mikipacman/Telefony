#include <stdio.h>
#include <string.h>
#include "phone_forward_interface.h"

int main()
{
    PhoneForwardsCenter *phoneForwardsCenter = initPhoneForwardsCenter();
    if (!phoneForwardsCenter)
    {
        throwError(OUT_OF_MEMORY_ERROR, 0);
        return 1;
    }

    Instruction *instruction = initInstruction();
    if (!instruction)
    {
        throwError(OUT_OF_MEMORY_ERROR, 0);
        return 1;
    }

    bool isNotEOF;

    atExitClean(phoneForwardsCenter, instruction);

    do
    {
        isNotEOF = getInstruction(instruction);
        if (isNotEOF)
            performInstruction(instruction, phoneForwardsCenter);

        clearInstruction(instruction);
    } while (isNotEOF);

    return 0;
}