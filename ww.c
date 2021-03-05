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

//add to seperate header file or .c file or put in isNumber.c
int isDir(char* file)
{
    int isDir = 0;
    struct stat data;
    int fileMode = stat(file, &data);

    if(S_ISDIR(data.st_mode))
    {
        printf("This is a directory\n");
        isDir = 1;

    }
    else if(S_ISREG(data.st_mode))
    {
        printf("This is a regular file\n");
        isDir = 0;
    }

    return isDir;
}

char* generateFilePath(char* directoryName, char* filePath)
{
    strbuf_t path;
    sb_init(&path, 10);
    sb_concat(&path, directoryName);
    sb_append(&path, '/');
    sb_concat(&path, "wrap.");
    sb_concat(&path, filePath);
    char* ret = malloc(sizeof(char)*path.length);
    strcpy(ret, path.data);
    sb_destroy(&path);
    return ret;
}

int prefixContains(char* prefix, char* word)
{
    return strncmp(prefix, word, strlen(prefix)) == 0;
}

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

    //int mode = stat(argv[2]);
    int inputFD;
    if(isDir(argv[2]))
    {
        //directory logic
        DIR* dirp = opendir(argv[2]);
        struct dirent* de;

        while(de = readdir(dirp))
        {
            
            if(prefixContains("wrap.", de->d_name))
            {
                continue;
            }
            if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0 && DT_REG==de->d_type)
            {

                char* newFilePath = generateFilePath(argv[1], de->d_name);

                int inputFD = open()

                int outputFD = open(newFilePath,  O_WRONLY | O_APPEND | O_CREAT, 0777); //O_APPEND --> O_TRUNC
                close(fd);

                //create char** and use as a lookup table to prevent duplicates
                //creates wrap.wrap.wrap bar
                //ignore files with "wrap." prefix
                //DEPENDS ON ASSUMPTIONS !!!!!

                free(newFilePath);

                /*
                if(inputFD == -1)
                {
                    perror("Cannot Create File.")
                    //return EXIT_FAILURE;
                }
                */
            }

            
            
        }
        closedir(dirp);
    }
    else
    {
        inputFD = open(argv[2], O_RDONLY);
        if(inputFD == -1)   
        {
            perror("Invalid Input File Given.");
            return EXIT_FAILURE;
        }
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

    write(outputFD, newline, 1); //<-----
    sb_destroy(&sb); // clear space allocated for string.

    return EXIT_STATUS;
}
