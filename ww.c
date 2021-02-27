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



int algo(int, int, int);


/*
    parts missing:
        error checks on program call input
        determining what the input file and output file is
        if directory -> a loop to complete the step for all files.
            -> will need O_CREATE flag to make the outputfile.wrap files.

*/
int main(int argc, char **argv)
{

    /*if(!isNumber(argv[1])){
        return EXIT_FAILURE;
    }*/

    int EXIT_STATUS = EXIT_SUCCESS;
    int width = atoi(argv[1]);

    int inputFD= open(argv[2], O_RDONLY);
    if(inputFD == -1){
        perror("Invalid Input File Given.");
        return EXIT_FAILURE;
    }

    int outputFD = 1;

    // algorithm (width, inputfile fd, outputfile fd)
    algo(width, inputFD, outputFD);

    close(inputFD);
    close(outputFD);

    return EXIT_STATUS;
}

/* Does not concern itself with opening/closing files */
int algo(int width, int inputFD, int outputFD){
    int EXIT_STATUS = EXIT_SUCCESS;
    
    // inputFD = input File Descriptor to read from.
    // outputFD = File Descriptor to write to.

    strbuf_t sb; 
    sb_init(&sb, 12);
    const char newline[1] = {'\n'};
    const char whitespace[1] = {' '};
    const char dash[1] = {'-'};
    char buffer[BUFSIZE]; 
    int bytes = read(inputFD, buffer, BUFSIZE);
    int count = 0, newlineDetectedOnce = 0;

    for(int w=0; w<width; w++) write(outputFD, dash, 1);
    write(1, newline, 1);

    while (bytes > 0) {
        for(int i=0; i<bytes; i++){
            char c = buffer[i];
            /* Word longer than column width? */
            if(sb.used > width){ EXIT_STATUS = EXIT_FAILURE;}
            if(count >= width){write(outputFD, newline, 1); count = 0;}

            if(c == '\n'){
                if(!newlineDetectedOnce){
                    newlineDetectedOnce = 1;
                }
                else{
                    /* Two consecutive newlines found*/
                    if(sb.used <= (width-count)){
                        write(outputFD, sb.data, sb.used);
                        count += (sb.used + 1);
                        sb_reset(&sb);
                        write(outputFD, whitespace, 1);
                    }
                    else{
                        write(outputFD, newline, 1);
                        write(outputFD, sb.data, sb.used);
                        count = sb.used+1;
                        sb_reset(&sb);
                        write(outputFD, whitespace, 1);
                        if(count >= width){
                            write(outputFD, newline, 1);
                            count = 0;
                        }
                    }
                    write(outputFD, newline, 1);
                    write(outputFD, newline, 1);
                    count = 0;
                    newlineDetectedOnce = 0;
                }
            }
            else if(isspace(c) && sb.used!=0){
                if(sb.used <= (width-count)){
                    write(outputFD, sb.data, sb.used);
                    count += (sb.used + 1);
                    sb_reset(&sb);
                    write(outputFD, whitespace, 1);
                }
                else{
                    write(outputFD, newline, 1);
                    write(outputFD, sb.data, sb.used);
                    count = sb.used+1;
                    sb_reset(&sb);
                    write(outputFD, whitespace, 1);
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

        bytes = read(inputFD, buffer, BUFSIZE);
    }


    if(sb.used != 0){
        if(sb.used > (width-count)){
            write(outputFD, newline, 1);
        }

        write(outputFD, sb.data, sb.used);
        sb_reset(&sb);
    }

    write(outputFD, newline, 1);

    sb_destroy(&sb);
    return EXIT_STATUS;
}