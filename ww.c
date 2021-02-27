<<<<<<< HEAD
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strbuf.h"
#include "isNumber.h" //code to check whether the width is an integer // will test to make sure
//we have duplicate header files in strbuf.h and isNumber.h

#ifndef BUFSIZE
#define BUFSIZE 256
#endif

int main(int argc, char **argv)
{
    //condense and remove redunancy
    int fileDescriptor;
    if(argc!=3)
    {
        return EXIT_FAILURE;
    }
    else if(!isNumber(argv[1]))
    {
        return EXIT_FAILURE;
    }

    fileDescripter = open(argv[2], O_RDONLY)

    if()
    {

    }

    //add error check for if first argc is 


}
=======
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

    //ADD CASE OF ZERO DESCRIPTER <---- 0 IS EOF

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
>>>>>>> 8ea7f25bd9f276bd17597914fe647ce4233f4798
}