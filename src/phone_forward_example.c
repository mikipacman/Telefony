#include <stdio.h>
#include <string.h>
#include "phone_forward_interface.h"

int main()
{
    PhoneForwardsCenter *phoneForwardsCenter = initPhoneForwardsCenter();
    if (!phoneForwardsCenter)
        return 1;

    Instruction *instruction = initInstruction();
    if (!instruction)
        return 1;

    bool isNotEOF;

    atExitClean(phoneForwardsCenter, instruction);

    do
    {
        isNotEOF = getInstruction(instruction);
        if (isNotEOF)
            performInstruction(instruction, phoneForwardsCenter);

        clearInstruction(instruction);
    } while (isNotEOF);

    getInstruction(instruction);

    return 0;
}