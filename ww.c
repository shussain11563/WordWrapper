#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include "strbuf.h"

#ifndef BUFSIZE
#define BUFSIZE 256
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc, char **argv)
{

    int EXIT_STATUS = EXIT_SUCCESS;
    int fileDescriptor, bytes, newlineDetectedOnce;
    char buffer[BUFSIZE];
    strbuf_t sb;
    sb_init(&sb, 12);

    if (argc != 3)  // temporary check, we're only working with file given and width given for this one
        return EXIT_FAILURE;
    
    fileDescriptor = open(argv[2], O_RDONLY);

    if (fileDescriptor == -1) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    bytes = read(fileDescriptor, buf, BUFSIZE);
    while (() > 0) {
        if (DEBUG) {
            printf("%d bytes: |", bytes);
            for (i = 0; i < bytes; ++i) {
            putchar(buf[i]);
            }
            putchar('|');
            putchar('\n');
            continue;
        }

        for (i = 0; i < bytes; ++i) {
            buf[i] = toupper(buf[i]);
        }

        write(1, sb.data, sb.used);
        }

        if (bytes < 0) {
            perror("Read error");
    }

    close(fileDescriptor);
    sb_destroy(&sb);
    return EXIT_STATUS;
}