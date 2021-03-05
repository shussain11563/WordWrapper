#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strbuf.h"
#include <sys/stat.h> //can just write stat.h, cause fctnl.h is also in sys
#include <dirent.h>
/*#include "isNumber.h" */
//code to check whether the width is an integer // will test to make sure
//we have duplicate header files in strbuf.h and isNumber.h

#ifndef BUFSIZE
#define BUFSIZE 256
#endif



int algo(int, int, int);
int algoTEST(int, int, int);

/*
    parts missing:
        error checks on program call input
        determining what the input file and output file is
        if directory -> a loop to complete the step for all files.
            -> will need O_CREATE flag to make the outputfile.wrap files.

*/
int main(int argc, char **argv)
{

    int EXIT_STATUS;

    /*if(!isNumber(argv[1])){
        return EXIT_FAILURE;
    }*/

    int width = atoi(argv[1]);

    //int mode = stat(argv[1]);



    int inputFD= open(argv[2], O_RDONLY);
    if(inputFD == -1){
        perror("Invalid Input File Given.");
        return EXIT_FAILURE;
    }

    

    int outputFD = 1;

    // algorithm (width, inputfile fd, outputfile fd)
    EXIT_STATUS = algoTEST(width, inputFD, outputFD);

    close(inputFD);
    close(outputFD);

    return EXIT_STATUS;
}

/*
    @var width - argv[1]
    @var inputFD - fileDescriptor of file to read from
    @var outputFD - fileDescriptor of file to write to

    This function does not handle opening and closing of files.
    That is a task in main method. This is a silo-d algorithm
    which does the word wrapping job.
*/
int algoTEST(int width, int inputFD, int outputFD){
    int EXIT_STATUS = EXIT_SUCCESS;

    strbuf_t sb; 
    sb_init(&sb, 12);

    const char newline[1] = {'\n'};
    const char whitespace[1] = {' '};
    
    char buffer[BUFSIZE]; 
    int count = 0, newlineDetectedOnce = 0;

    // Here to visualize width and compare output.
    const char dash[1] = {'-'};
    for(int w=0; w<width; w++) write(outputFD, dash, 1);
    write(1, newline, 1);

    int bytes = read(inputFD, buffer, BUFSIZE);
    while (bytes > 0) {
        for(int i=0; i<bytes; i++){
            char c = buffer[i];
            /* Word longer than column width? */
            if(sb.used > width){ EXIT_STATUS = EXIT_FAILURE;}
            if(count >= width){write(outputFD, newline, 1); count = 0;}

            if(c == '\n'){
                if(!newlineDetectedOnce){
                    newlineDetectedOnce = 1;
                    if(sb.used != 0){
                        if(sb.used < (width-count)){
                            if(count != 0){
                                write(outputFD, whitespace, 1);
                            }
                            write(outputFD, sb.data, sb.used);
                            count += sb.used+1;
                            sb_reset(&sb);
                        }
                        else{
                            write(outputFD, newline, 1);
                            write(outputFD, sb.data, sb.used);
                            count = sb.used;
                            sb_reset(&sb);
                        }
                    }
                }
                else{
                    /* Two consecutive newlines found*/
                    write(outputFD, newline, 1);
                    write(outputFD, newline, 1);
                    count = 0;
                }
            }
            else if(isspace(c) && sb.used!=0){
                if(newlineDetectedOnce){ // If only one line separator, reset it.
                    newlineDetectedOnce = 0;
                }

                if(sb.used < (width-count)){
                    if(count != 0){
                        write(outputFD, whitespace, 1);
                    }
                    write(outputFD, sb.data, sb.used);
                    count += sb.used+1;
                    sb_reset(&sb);
                }
                else{
                    write(outputFD, newline, 1);
                    write(outputFD, sb.data, sb.used);
                    count = sb.used;
                    sb_reset(&sb);
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

    // Having parsed last buffer, handle anything left in sb.
    if(sb.used != 0){
       if(sb.used < (width-count)){
            if(count != 0){
                write(outputFD, whitespace, 1);
            }
            write(outputFD, sb.data, sb.used);
            sb_reset(&sb);
        }
        else{
            write(outputFD, newline, 1);
            write(outputFD, sb.data, sb.used);
            sb_reset(&sb);
        }
    }

    write(outputFD, newline, 1);
    sb_destroy(&sb); // clear space allocated for string.

    return EXIT_STATUS;
}
