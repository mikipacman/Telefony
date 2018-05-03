#include <stdlib.h>
#include <stdio.h>
#include "cyclicDoubleList.h"
#include "phone_forward.h"
#include <string.h>

void try(bool b)
{
    if (b)
        printf("gut\n");
    else
        printf("lipa\n");
}

int main(void)
{
    PhoneForward *pf = phfwdNew();

    try(phfwdAdd(pf, "123", "2"));

    try(phfwdAdd(pf, "12", "44"));

    PhoneNumbers const *pn = phfwdGet(pf, "123");

    //printf("%s\n", phnumGet(pn, 0));

    phfwdDelete(pf);

    return 0;
}