#include <stdlib.h>
#include <stdio.h>
#include "cyclicDoubleList.h"
#include "phone_forward.h"
#include "easyString.h"
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

    try(phfwdAdd(pf, "1", "66664"));
    try(phfwdAdd(pf, "1", "5"));
    try(phfwdAdd(pf, "123", "5"));
    try(phfwdAdd(pf, "124", "5"));
    try(phfwdAdd(pf, "125", "5"));
    try(phfwdAdd(pf, "55", "5"));
    try(phfwdAdd(pf, "55", "5"));
    try(phfwdAdd(pf, "567", "5"));
    try(phfwdAdd(pf, "66", "5"));

    phfwdRemove(pf, "55");

    PhoneNumbers const *pn = phfwdReverse(pf, "55");
    char const *num = NULL;
    for (int i = 0; i < pn->numOfPN; i++)
    {
        num = phnumGet(pn, (size_t)i);
        printf("%s\n", num);
    }


    phfwdDelete(pf);
    phnumDelete(pn);

    return 0;
}