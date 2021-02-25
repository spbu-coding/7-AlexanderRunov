#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _StringItem {
    char *str;
    struct _StringItem *next;
} StringItem;

struct StringItem *radix1(StringItem *pList, unsigned int iDigit) {
    const int iRange = 256;

    StringItem *front[iRange];
    memset(front, 0, sizeof(front));

    StringItem **ppNextItem[iRange];
    for (int i = 0; i < iRange; i++)
        ppNextItem[i] = &front[i];

    while (pList) {
        StringItem *temp = pList;
        pList = pList->next;

        temp->next = NULL;

        unsigned char c = (unsigned char) temp->str[iDigit];
        *ppNextItem[c] = temp;
        ppNextItem[c] = &temp->next;
    }

    StringItem *pResult = NULL;
    StringItem **ppNext = &pResult;

    *ppNext = front[0];
    while (*ppNext)
        ppNext = &((*ppNext)->next);

    for (int i = 1; i < iRange; i++) {
        if (!front[i])
            continue;
        if (front[i]->next == NULL)
            *ppNext = front[i];
        else
            *ppNext = (StringItem *) radix1(front[i], iDigit + 1);

        while (*ppNext)
            ppNext = &((*ppNext)->next);
    }
    return (struct StringItem *) pResult;
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "rt");
    StringItem *pHead, *pTail;
    pHead = NULL;
    pTail = NULL;
    while (!feof(file)) {
        char *s = (char *) calloc(1001, sizeof(char));
        fgets(s, 1000, file);
        for (unsigned int i = strlen(s) - 1; i > 0; i--)
            if (s[i] == 10 || s[i] == 13 || s[i] == '.' || s[i] == ',' || s[i] == ';' ||
                s[i] == ':' || s[i] == '!' || s[i] == '?')
                for (unsigned int j = i; s[j] != 0; j++)
                    s[j] = s[j + 1];
        if (pHead == NULL) {
            pHead = malloc(sizeof(StringItem));
            pHead->str = s;
            pHead->next = NULL;
            pTail = pHead;
        } else {
            StringItem *si = malloc(sizeof(StringItem));
            si->str = s;
            si->next = NULL;
            pTail->next = si;
            pTail = si;
        }
    }
    fclose(file);
    pHead = (StringItem *) radix1(pHead, 0);
    pTail = pHead;
    for (int i = 0; i < 100 && pTail != NULL; i++) {
        printf("%s\n", pTail->str);
        pTail = pTail->next;
    }
    pTail = pHead;
    while (pTail != NULL) {
        StringItem *si = pTail->next;
        free(pTail->str);
        free(pTail);
        pTail = si;
    }
    return 0;
}
