#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <windows.h>

char *replace(char *str) {
    char *buff = (char *) malloc(sizeof(char *) * 512);
    char *po = buff;
    for (; *str != '\\'; ++str) {
        *po++ = *str;
    }
    if (buff[1] != ':') {
        free(buff);
        return NULL;
    }
    char low = (char) tolower(*buff);
    *buff = '\0';
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

int main() {
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