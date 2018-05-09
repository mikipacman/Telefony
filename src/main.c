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
//    PhoneForward *pf = phfwdNew();
//
//
//    phfwdAdd(pf, "3", "44");
//    phfwdAdd(pf, "3", "444");
//    phfwdAdd(pf, "3", "4444");
//    phfwdAdd(pf, "3", "4");
//    phfwdRemove(pf, "444");
//    phfwdAdd(pf, "3", "444");
//    phfwdAdd(pf, "3", "444");
//    rev(pf, "44444");
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