#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

char *replace(char *str)
{
    if (str == NULL) return NULL;
    if (str[1] != ':') {
        return NULL;
    }
    char low = (char) tolower(*str);
    char *po;
    char *buff = (char *) malloc(sizeof(char *) * 512);
    *buff = '\0';
    str = &str[2];
    strcpy(buff, "\"/mnt/");
    for (po = buff; *po != '\0'; ++po);
    *po++ = low;
    for (; *str != '\0'; ++str) {
        if (*str == '\\') {
            *po++ = '/';
            continue;
        }
        *po++ = *str;
    }
    *po++ = '\"';
    *po = '\0';
    return buff;
}

int withClipboard()
{
    char *pBuf;
    if (OpenClipboard(NULL)) {
        if (IsClipboardFormatAvailable(CF_TEXT)) {
            HANDLE hClip;
            hClip = GetClipboardData(CF_TEXT);
            pBuf = (char *) GlobalLock(hClip);
            GlobalUnlock(hClip);
            char *r = replace(pBuf);
            if (r == NULL) {
                CloseClipboard();
                return EXIT_FAILURE;
            }
            EmptyClipboard();
            hClip = GlobalAlloc(GMEM_MOVEABLE, strlen(r) + 1);
            pBuf = (char *) GlobalLock(hClip);
            strcpy(pBuf, r);
            free(r);
            GlobalUnlock(hClip);
            SetClipboardData(CF_TEXT, hClip);
            CloseClipboard();
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    opterr = 0;
    char LineArg = (char) getopt(argc, argv, "p:");
    if (LineArg == -1) {
        return withClipboard();
    }
    printf("%s", replace(optarg));
    return EXIT_SUCCESS;
}