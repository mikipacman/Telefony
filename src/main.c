//#include <assert.h>
//#include <stdio.h>
//#include <string.h>
//#include "phone_forward.h"
//
//static void rev(PhoneForward *pf, const char* string)
//{
//    printf("reverse %s\n", string);
//    PhoneNumbers const* pnum = phfwdReverse(pf, string);
//    for (int i = 0; i < pnum->numOfPN; i++)
//    {
//        printf("%s\n", phnumGet(pnum, i));
//    }
//    printf("\n");
//    phnumDelete(pnum);
//}
//
//int main()
//{
//    printf("CUSTOM BAD CASES\n");
//    PhoneForward * pf = phfwdNew();
//    PhoneNumbers const *pnum;
//
//    pnum = phfwdGet(pf, "$$$%");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, " 1");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "pro");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "1223333O2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "^");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "1 + 2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "1+2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, " ");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "555-");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "+22");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "-2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "00000 2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, " 2");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "2)");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "]");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "1!");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "\n");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdGet(pf, "13212312\n");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "]");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "13123312\n");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "12 ");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, " 12");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "112313132132312132%");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "12331213213200000O31230013");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//    pnum = phfwdReverse(pf, "'''<");
//    if (phnumGet(pnum, 0) == NULL) printf("GOOD\n");
//    else printf("%s\n", phnumGet(pnum, 0));
//    phnumDelete(pnum);
//
//
//    assert(phfwdAdd(pf, "13212", "23322O") == false);
//    assert(phfwdAdd(pf, "1326^12", "23322O") == false);
//    assert(phfwdAdd(pf, "13212", "") == false);
//    assert(phfwdAdd(pf, "", "23322O") == false);
//    assert(phfwdAdd(pf, "13 ", "2332 2") == false);
//    assert(phfwdAdd(pf, "13212", "2332m2") == false);
//    assert(phfwdAdd(pf, "       ", "23322") == false);
//    assert(phfwdAdd(pf, "13\n&", "23322") == false);
//    assert(phfwdAdd(pf, "12", "12") == false);
//    assert(phfwdAdd(pf, "12", "133") == true);
//    assert(phfwdAdd(pf, "13", "12") == true);
//    assert(phfwdAdd(pf, "1", "12") == true);
//
//    phfwdRemove(pf, "1323132 ");
//    phfwdRemove(pf, "1 ");
//    phfwdRemove(pf, " 1");
//    phfwdRemove(pf, "1\n3");
//    phfwdRemove(pf, "%%$#$#$%$");
//    phfwdRemove(pf, "Lorem Ipsum");
//    phfwdRemove(pf, "1|");
//    phfwdRemove(pf, "13");
//
//    pnum = phfwdGet(pf, "12");
//    if (phnumGet(pnum, 0) != NULL) printf("GOOD\n");
//    else printf("BAD");
//    phnumDelete(pnum);
//
//    phfwdDelete(pf);
//
//    return 0;
//}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward.h"

int main() {
    char * input = (char *)malloc(sizeof(char) * 5000);
    char * first = (char *)malloc(sizeof(char) * 5000);
    char * second = (char *)malloc(sizeof(char) * 5000);
    int n;
    PhoneForward *pf = phfwdNew();

    if (!scanf("%d", &n)) printf("BUG\n");

    for (int i = 0; i < n; i++) {

        char const *num;
        if (!scanf("%s", input)) printf("BUG\n");
        PhoneNumbers const *pnum;
        size_t idx = 0;
        //printf("ACTION %s\n",input);
        if (strcmp(input, "ADD") == 0) {
            if (!scanf("%s %s", first, second)) printf("BUG\n");
            phfwdAdd(pf, first, second);
        } else if (strcmp(input, "REM") == 0) {
            if (!scanf("%s", first)) printf("BUG\n");
            phfwdRemove(pf, first);
        } else if (strcmp(input, "GET") == 0) {
            if (!scanf("%s", first)) printf("BUG\n");
            pnum = phfwdGet(pf, first);

            while ((num = phnumGet(pnum, idx++)) != NULL) {
                if (idx == 1)
                    printf("%s", num);
                else
                    printf(" %s", num);
            }

            printf("\n");
            phnumDelete(pnum);
        } else if (strcmp(input, "REV") == 0) {
            if (!scanf("%s", first)) printf("BUG\n");
            pnum = phfwdReverse(pf, first);

            while ((num = phnumGet(pnum, idx++)) != NULL) {
                if (idx == 1)
                    printf("%s", num);
                else
                    printf(" %s", num);
            }

            printf("\n");
            phnumDelete(pnum);
        }
    }

    phfwdDelete(pf);
    free(input);
    free(first);
    free(second);
}