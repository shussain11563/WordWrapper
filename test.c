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



int main(int argc, char **argv)
{
    struct stat data;
    //Here const char *path specifies the name of the file. If the path of file is a symbolic link then you need to specify the link instead of file name.
    int fileMode = stat(argv[1], &data);

    if(isDir(argv[1]))
    {
       DIR* dirp = opendir(argv[1]);
        struct dirent* de;

        de = readdir(dirp);
        while(de)
        {
            char* test = de->d_name;
            puts(test);

            strbuf_t cursor; 
            sb_init(&cursor, 10);
            sb_concat(&cursor, "wrap.");
            sb_concat(&cursor, test);
            dump(&cursor);
            printf("NewFileName is %s \n", cursor.data);

            strbuf_t path; 
            sb_init(&path, 10);
            //char* curr =
            sb_concat(&path, argv[1]);
            sb_append(&path, '/');
            sb_concat(&path, cursor.data);
            printf("NewFileName is %s \n", path.data);
            //get file directory name
            //example/data
            int fd = open(path.data,  O_WRONLY | O_APPEND | O_CREAT, 0777); //O_APPEND --> O_TRUNC

            //ignore files with "wrap." prefix
            close(fd);


            de = readdir(dirp);

            
            

            //call append function to //de->d_name with 'wrap'--->tempname
            //add check whether file descripter returns -1 if fails
           //int fd = open(tempname,  O_WRONLY | O_APPEND | O_CREAT, 0777); <----add permissions
        }
        closedir(dirp);
        return 0; 
    }
    else
    {
       
    }


}
