#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char* generateFilePath(char*, char*);
char* generateCurrentPath(char*, char*);
int prefixContains(char*, char*);
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

    int EXIT_STATUS = EXIT_SUCCESS;
    
    for(int i=0; i<strlen(argv[1]); i++){
        if(!isdigit(argv[1][i])){
            perror("Width not a positive integer");
            EXIT_STATUS = EXIT_FAILURE;
            return EXIT_STATUS;
        }
    }
    int width = atoi(argv[1]); // argv[1] was all digits

    // yet to do - permissions
    if(argc == 2){
        // only stdin to stdout stuff
        // don't have to worry about permissions
        int inputFD = STDIN_FILENO;
        int outputFD = STDOUT_FILENO;
        EXIT_STATUS = algo(width, inputFD, outputFD);
        return EXIT_STATUS;
    }
    else if(argc == 3){

        struct stat data;
        stat(argv[2], &data);

        if(S_ISDIR(data.st_mode)) // argv2 is a directory
        {
            //directory logic
            DIR* dirp = opendir(argv[2]);
            struct dirent* de = readdir(dirp);

            while(!de)
            {
                
                if(prefixContains("wrap.", de->d_name) || strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0)
                {
                    continue;
                }

                stat(de->d_name, &data);
                if(S_ISREG(data.st_mode))
                { // DT_REG - type of regular files

                    char* newFilePath = generateFilePath(argv[2], de->d_name);
                    char* currentPath = generateCurrentPath(argv[2], de->d_name);
                    
                    int inputFD = open(currentPath, O_RDONLY);
                    if(inputFD == -1)   
                    {
                        perror("Invalid Input File Given.");
                        return EXIT_FAILURE; //<--------------remove this????
                    }

                    int outputFD = open(newFilePath,  O_WRONLY | O_TRUNC | O_CREAT, 0777); 

                    int exit_temp = algo(width, inputFD, outputFD);

                    if(exit_temp == EXIT_FAILURE){
                        EXIT_STATUS = EXIT_FAILURE;
                    }

                    close(inputFD);
                    close(outputFD);
                    free(newFilePath);
                    free(currentPath);
                }

                de = readdir(dirp);
            } 
            closedir(dirp);
        }
        else if(S_ISREG(data.st_mode)) // argv2 is a file
        {
            int inputFD = open(argv[2], O_RDONLY);
            int outputFD = STDOUT_FILENO;
            if(inputFD == -1)   
            {
                perror("Invalid Input File Given.");
                return EXIT_FAILURE;
            }

            EXIT_STATUS = algo(width, inputFD, outputFD);
            close(inputFD);
            close(outputFD);
        }
        else // else neither a file or a dir
        { 
            perror("argv[2] neither a file nor dir");
            EXIT_STATUS = EXIT_FAILURE;
            return EXIT_STATUS;   
        }
    }

    return EXIT_STATUS;
}

// wrap.file in that directory for that specific file
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
//add third argument regarding whether to add "wrap." and delete duplicate
char* generateCurrentPath(char* directoryName, char* filePath)
{
    strbuf_t path;
    sb_init(&path, 10);
    sb_concat(&path, directoryName);
    sb_append(&path, '/');
    //sb_concat(&path, "wrap.");
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

/* If there are multiple \n, consume only 2. this\n     \n\n\n\n\n    \nasoemth
    @var width - argv[1]
    @var inputFD - fileDescriptor of file to read from
    @var outputFD - fileDescriptor of file to write to

    This function does not handle opening and closing of files.
    That is a task in main method. This is a silo-d algorithm
    which does the word wrapping job.
*/
int algo(int width, int inputFD, int outputFD){
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
