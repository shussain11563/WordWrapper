#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "strbuf.h"
#include <sys/stat.h> 
#include <dirent.h>

#ifndef BUFSIZE
#define BUFSIZE 256
#endif

char* generateFilePath(char*, char*, int);
int prefixContains(char*, char*);
int algo(int, int, int);

int main(int argc, char **argv)
{

    int EXIT_STATUS = EXIT_SUCCESS;
    
    for(int i=0; i<strlen(argv[1]); i++){
        if(!isdigit(argv[1][i])){
            EXIT_STATUS = EXIT_FAILURE;
            return EXIT_STATUS;
        }
    }

    int width = atoi(argv[1]); // argv[1] was all digits
    
    if(width<=0)
    {
        EXIT_STATUS = EXIT_FAILURE;
        return EXIT_STATUS;
    }
    
    if(argc == 2){
        // only stdin to stdout stuff
        // don't have to worry about permissions
        int inputFD = STDIN_FILENO;
        int outputFD = open("stdoutStorage.txt", O_WRONLY | O_TRUNC | O_CREAT, 00777);
        EXIT_STATUS = algo(width, inputFD, outputFD);
        close(inputFD);
        close(outputFD);
        // wrote the stdOut stuff over to stdOutStorage.txt
        // now just bring it back 
        inputFD = open("stdoutStorage.txt", O_RDONLY);
        if(inputFD==-1){
            perror("");
        }
        char buffer[1024]; int bytes;
        while ((bytes = read(inputFD, buffer, BUFSIZE)) > 0)
            write(STDOUT_FILENO, buffer, bytes);

        close(inputFD);
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        return EXIT_STATUS;
    }
    else if(argc == 3){

        struct stat data;
        int structSTAT = stat(argv[2], &data);

        if(structSTAT == -1){
            perror("");
            EXIT_STATUS = EXIT_FAILURE;
            return EXIT_STATUS;   
        }
        else if(S_ISDIR(data.st_mode)) // argv2 is a directory, we are guaranteed it exists now so opendir won't return NULL
        {
            //directory logic
            DIR* dirp = opendir(argv[2]);
            if(dirp == NULL){
                perror("Folder");
                EXIT_STATUS = EXIT_FAILURE;
                return EXIT_STATUS;   
            }
            
            struct dirent* de = readdir(dirp);

            while(de != NULL)
            {
                if(prefixContains("wrap.", de->d_name) || strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0)
                {
                    de = readdir(dirp);
                    continue;
                }

                stat(de->d_name, &data);
                if(S_ISREG(data.st_mode))
                { // DT_REG - type of regular files

                    char* currentPath = generateFilePath(argv[2], de->d_name, 1); // <dir_name/<file_name>
                    char* newFilePath = generateFilePath(argv[2], de->d_name, 0); // <dir_name>/wrap.<file_name>
                    puts(newFilePath);
                    puts(currentPath);

                    int inputFD = open(currentPath, O_RDONLY);
                    if(inputFD == -1)   
                    {
                        perror("");
                        //close(inputFD);
                        free(newFilePath);
                        free(currentPath);
                        continue;
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
                perror("File");
                return EXIT_FAILURE;
            }

            EXIT_STATUS = algo(width, inputFD, outputFD);
            close(inputFD);
            close(outputFD);
        }
    }

    return EXIT_STATUS;
}

// wrap.file in that directory for that specific file
char* generateFilePath(char* directoryName, char* filePath, int isCurrPath)
{
    strbuf_t path;
    sb_init(&path, 10);
    sb_concat(&path, directoryName);
    sb_append(&path, '/');
    if(!isCurrPath)
    {
        sb_concat(&path, "wrap.");
    }
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
    int count = 0, newlineDetectedOnce = 0, two_newlineDetected = 0;

    // Here to visualize width and compare output.
    const char dash[1] = {'-'};
    for(int w=0; w<width; w++) write(outputFD, dash, 1);
    write(outputFD, newline, 1);

    int bytes = read(inputFD, buffer, BUFSIZE);
    int somethingWritten = 0;
    while (bytes > 0) {
        for(int i=0; i<bytes; i++){
            char c = buffer[i];
            /* Word longer than column width? */
            if(sb.used > width){ EXIT_STATUS = EXIT_FAILURE;}
            if(count >= width){write(outputFD, newline, 1); count = 0;}

            if(c == '\n'){ /* One newline found*/
                if(!newlineDetectedOnce){
                    newlineDetectedOnce = 1;
                    if(sb.used != 0){ /* This is here to prevent isSpace from writing */
                        if(sb.used < (width-count) && count != 0){ // if count isn't 0, 
                        //then sb.used has to be less than to include whitespace at beginning
                            write(outputFD, whitespace, 1);
                            write(outputFD, sb.data, sb.used);
                            count += sb.used+1;
                            sb_reset(&sb);
                        }
                        else if(sb.used <= width && count == 0){
                            write(outputFD, sb.data, sb.used);
                            count += sb.used;
                            sb_reset(&sb);
                        }
                        else{
                            if(count != 0){ // only newline if necessary
                                write(outputFD, newline, 1);
                            }
                            write(outputFD, sb.data, sb.used);
                            count = sb.used;
                            sb_reset(&sb);
                        }
                        somethingWritten = 1;
                    }
                }
                else{ /* Two consecutive newlines found*/
                    two_newlineDetected = 1;
                }
            }
            else if(isspace(c) && sb.used!=0){
                if(sb.used < (width-count) && count != 0){ // if count isn't 0, 
                //then sb.used has to be less than to include whitespace at beginning
                    write(outputFD, whitespace, 1);
                    write(outputFD, sb.data, sb.used);
                    count += sb.used+1;
                    sb_reset(&sb);
                }
                else if(sb.used <= width && count == 0){
                    write(outputFD, sb.data, sb.used);
                    count += sb.used;
                    sb_reset(&sb);
                }
                else{
                    if(count != 0){ // only newline if necessary
                        write(outputFD, newline, 1);
                    }
                    write(outputFD, sb.data, sb.used);
                    count = sb.used;
                    sb_reset(&sb);
                }
                somethingWritten = 1;
            }
            else if(!isspace(c)){
                if(!somethingWritten){
                    two_newlineDetected = 0;
                    newlineDetectedOnce = 0;
                }
                if(two_newlineDetected){
                    write(outputFD, newline, 1);
                    write(outputFD, newline, 1);
                    count = 0;
                }

                sb_append(&sb, c);
                two_newlineDetected = 0;
                newlineDetectedOnce = 0;
            }
        }

        bytes = read(inputFD, buffer, BUFSIZE);
    }

    // Having parsed last buffer, handle anything left in sb.
    if(sb.used != 0){
        if(sb.used < (width-count) && count != 0){ // if count isn't 0, 
        //then sb.used has to be less than to include whitespace at beginning
            write(outputFD, whitespace, 1);
            write(outputFD, sb.data, sb.used);
            count += sb.used+1;
            sb_reset(&sb);
        }
        else if(sb.used <= width && count == 0){
            write(outputFD, sb.data, sb.used);
            count += sb.used;
            sb_reset(&sb);
        }
        else{
            if(count != 0){ // only newline if necessary
                write(outputFD, newline, 1);
            }
            write(outputFD, sb.data, sb.used);
            count = sb.used;
            sb_reset(&sb);
        }
        somethingWritten = 1;
    }

    write(outputFD, newline, 1);
    sb_destroy(&sb); // clear space allocated for string.

    return EXIT_STATUS;
}
