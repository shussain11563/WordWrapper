#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strbuf.h"
/*#include "isNumber.h" */
//code to check whether the width is an integer // will test to make sure
//we have duplicate header files in strbuf.h and isNumber.h

#ifndef BUFSIZE
#define BUFSIZE 256
#endif

int main(int argc, char **argv)
{

    /*if(!isNumber(argv[1])){
        return EXIT_FAILURE;
    }*/

    int EXIT_STATUS = EXIT_SUCCESS;
    int width = atoi(argv[1]);

    int fileDescriptor = open(argv[2], O_RDONLY);
    if(fileDescriptor == -1){
        perror("Invalid File Given.");
        return EXIT_FAILURE;
    }

    /* assuming that correct argv[1] and argv[2] */
    strbuf_t sb; 
    sb_init(&sb, 12);
    const char newline[1] = {'\n'};
    const char whitespace[1] = {' '};
    const char dash[1] = {'-'};

    char buffer[BUFSIZE]; 
    int bytes = read(fileDescriptor, buffer, BUFSIZE);
    int count = 0, newlineDetectedOnce = 0;

    for(int w=0; w<width; w++) write(1, dash, 1);
    write(1, newline, 1);

    while (bytes > 0) {
        for(int i=0; i<BUFSIZE; i++){
            char c = buffer[i];
            /* Word longer than column width? */
            if(sb.used > width){ EXIT_STATUS = EXIT_FAILURE;}
            if(count >= width){write(1, newline, 1); count = 0;}

            if(c == '\n'){
                if(!newlineDetectedOnce){
                    newlineDetectedOnce = 1;
                }
                else{
                    /* Two consecutive newlines found*/
                    write(1, newline, 1);
                    write(1, newline, 1);
                    count = 0;
                    newlineDetectedOnce = 0;
                }
            }
            else if(isspace(c) && sb.used!=0){
                if(sb.used <= (width-count)){
                    write(1, sb.data, sb.used);
                    count += (sb.used + 1);
                    sb_reset(&sb);
                    write(1, whitespace, 1);
                }
                else{
                    write(1, newline, 1);
                    write(1, sb.data, sb.used);
                    count = sb.used+1;
                    sb_reset(&sb);
                    write(1, whitespace, 1);
                    if(count >= width){
                        write(1, newline, 1);
                        count = 0;
                    }
                }

                if(newlineDetectedOnce){
                    newlineDetectedOnce = 0;
                }
            }
            else if(!isspace(c)){
                sb_append(&sb, c);
                if(newlineDetectedOnce){
                    newlineDetectedOnce = 0;
                }
            }
        }
        if(sb.used != 0){
            if(sb.used <= (width-count)){
                write(1, sb.data, sb.used);
                sb_reset(&sb);
                write(1, whitespace, 1);
            }
            else{
                write(1, newline, 1);
                write(1, sb.data, sb.used);
                sb_reset(&sb);
                write(1, whitespace, 1);
            }
        }

        bytes = read(fileDescriptor, buffer, BUFSIZE);
    }

    write(1, newline, 1);
    close(fileDescriptor);
    sb_destroy(&sb);
    return EXIT_STATUS;
}