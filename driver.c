#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <string.h>
#include "strbuf.h"


int isDir(char* file)
{
    int isDir = 0;
    struct stat data;
    int fileMode = stat(file, &data);

    if(S_ISDIR(data.st_mode))
    {
        isDir = 1;
    }
    else if(S_ISREG(data.st_mode))
    {
        printf("This is a regular file\n");
        isDir = 0;
    }
    return isDir;
}

char* generateFilePath(char* directoryName, char* filePath, int isSol)
{
    strbuf_t path;
    sb_init(&path, 10);
    sb_concat(&path, directoryName);
    sb_append(&path, '/');
    if(!isSol)
    {
        sb_concat(&path, "wrap.");
    }
    else
    {
        sb_concat(&path, "sol.");
    }
    sb_concat(&path, filePath);
    char* ret = malloc(sizeof(char)* (path.used+1));
    strcpy(ret, path.data);
    sb_destroy(&path);
    return ret;
}


int prefixContains(char* prefix, char* word)
{
    return strncmp(prefix, word, strlen(prefix)) == 0;
}

int main(int argc, char **argv)
{
    int fileCounter = 0;
    int correctMatches = 0;

    if(isDir(argv[1]))
    {
       DIR* dirp = opendir(argv[1]);
        struct dirent* de;
        while(de = readdir(dirp))
        {
            if(prefixContains("wrap.", de->d_name)||strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0)
            {
                continue;
            }
            else //check permission
            {   
                fileCounter++;
                char* filePathWithWrap = generateFilePath(argv[1], de->d_name, 0); // dir/wrap.test
                struct stat data;
                
                int statRet = stat(filePathWithWrap, &data);
                if(statRet==-1)
                {
                    printf("Cannot open %s for reading or does not exist\n", filePathWithWrap);
                    free(filePathWithWrap);
                    continue;
                }

                char* sol = generateFilePath("solutions", de->d_name, 1);
                int statRetSol = stat(sol, &data);
                if(statRet==-1)
                {
                    printf("Cannot open %s for reading or does not exist\n", sol);
                    free(sol);
                    continue;
                }

             //puts(de->d_name);
                FILE *fp1 = fopen(filePathWithWrap, "r");
                FILE *fp2 = fopen(sol, "r");
                int ch1, ch2;
                
                if (fp1 == NULL) 
                {
                    printf("Cannot open %s for reading \n", filePathWithWrap);
                    continue;
                } 
                else if (fp2 == NULL) 
                {
                    printf("Cannot open %s for reading \n", sol);
                    continue;
                } 
                else 
                {
                    ch1 = getc(fp1);
                    ch2 = getc(fp2);
                }
                while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) 
                {
                    ch1 = getc(fp1);
                    ch2 = getc(fp2);
                }

                if (ch1 == ch2)
                {
                    printf("%s and %s are identical\n", filePathWithWrap, sol);
                    correctMatches++;
                }
                else if (ch1 != ch2)
                {
                    printf("%s and %s are NOT identical\n", filePathWithWrap, sol);
                }
                    
                free(filePathWithWrap);
                free(sol);

                fclose(fp1);
                fclose(fp2);
                

            }
        }
        closedir(dirp);

    }

    printf("%d/%d Files Matched\n", correctMatches, fileCounter);
    return 0;
}