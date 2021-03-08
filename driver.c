#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> //can just write stat.h, cause fctnl.h is also in sys
#include <dirent.h>
#include <string.h>


 typedef struct {
    size_t length;
    size_t used;
    char *data;
} strbuf_t;

int sb_append(strbuf_t *sb, char item){
    if((sb->used+1) == sb->length){
        size_t size = sb->length * 2;
        char *p = realloc(sb->data, sizeof(char)* size);
        if(!p) return 1;
        // successful
        sb->data = p;
        sb->length = size;

    }

    sb->data[sb->used] = item;
    ++sb->used;
    sb->data[sb->used] = '\0';
    return 0;
}


int sb_init(strbuf_t *sb, size_t length)
{
    if(length < 1){
        return 1;
    }
    
    sb->data = malloc(sizeof(char) * length);
    if (!sb->data) return 1;

    sb->length = length;
    sb->used   = 0;
    sb->data[sb->used] = '\0';

    return 0;
}

void sb_destroy(strbuf_t *sb)
{
    free(sb->data);
}

/*
    sb_concat adds the string str to the end of the string held in sb. 
    Assume that str is a null-terminated C string. 
    Return 0 if successful, and 1 otherwise.
*/
int sb_concat(strbuf_t *sb, char *str){
    if((sb->used + strlen(str)) >= sb->length){
        size_t size = ((sb->used + strlen(str) + 1) > 2 * sb->length) ? (sb->used + strlen(str) + 1) : 2*sb->length;
        char *p = realloc(sb->data, sizeof(char)* size);
        if(!p) return 1;
        // successful
        sb->data = p;
        sb->length = size;
        //if (DEBUG) idk what this is for
        //    printf("Increased size to %lu\n", size);
    }
    
    int count = 0;
    while(count < strlen(str)){
        sb->data[sb->used+count] = *(str+count);
        ++count;
    }
    sb->data[sb->used+count] = '\0';
    sb->used += count;
    return 0;
}

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

void dump(strbuf_t *sb)
{
    int i=0;
    printf("[%ld/%ld]\t", sb->used, sb->length);
    for(; i<sb->used; i++){
        printf("%c ", sb->data[i]);
    }
    printf("/0 "); ++i;
    for(; i<sb->length; i++){
        printf("_ ");
    }
    putchar('\n');
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

//test1 ---> append wrap.text -----> check with sol.text

int main(int argc, char **argv)
{
    
    //Here const char *path specifies the name of the file. If the path of file is a symbolic link then you need to specify the link instead of file name.
    //run command through make
    //we now pass the directory
   // wrap.test1       sol.test1
    //wrap.test2  ---> sol.test2

    if(isDir(argv[1]))
    {
       DIR* dirp = opendir(argv[1]);
        struct dirent* de;
        int fileCounter = 0;
        puts("Hello")
        while(de = readdir(dirp))
        {

            if(prefixContains("wrap.", de->d_name)||strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0)
            {
                continue;
            }
            else //check permission
            {   
                char* filePathWithWrap = generateFilePath(argv[2], de->d_name, 0); // dir/wrap.test
                struct stat data;
                int statRet = stat(generateFilePath(argv[2], de->d_name, 0), &data);
                char* sol = generateFilePath("solutions", de->d_name, 1);

            
                FILE *fp1 = fopen(filePathWithWrap, "r");
                FILE *fp2 = fopen(sol, "r");
                int ch1, ch2;
                if (fp1 == NULL) 
                {
                    printf("Cannot open %s for reading ", filePathWithWrap);
                    continue;
                } 
                else if (fp2 == NULL) 
                {
                    printf("Cannot open %s for reading ", sol);
                    continue;
                } 
                else 
                {
                    printf("Hello\n");
                    ch1 = getc(fp1);
                    ch2 = getc(fp2);
                }
                while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) 
                {
                    ch1 = getc(fp1);
                    ch2 = getc(fp2);
                }

                if (ch1 == ch2)
                    printf("Files are identical n");
                else if (ch1 != ch2)
                    printf("Files are Not identical n");

                fclose(fp1);
                fclose(fp2);
                
               // return (0);

            }
        }
        closedir(dirp);

    }
    return 0;
}