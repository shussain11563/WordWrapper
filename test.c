#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> //can just write stat.h, cause fctnl.h is also in sys
#include <dirent.h>



int main(int argc, char **argv)
{
    struct stat data;
    //Here const char *path specifies the name of the file. If the path of file is a symbolic link then you need to specify the link instead of file name.
    int fileMode = stat(argv[1], &data);

    int isDir = 0;
    if(S_ISDIR(data.st_mode))
    {
        printf("This is a directory\n");
        isDir = 1;

    }
    else if(S_ISREG(data.st_mode))
    {
        printf("This is a regular file\n");
        isDir = 0;
        return 0;
    }
    
    DIR* dirp = opendir(argv[1]);
    struct dirent* de;

    de = readdir(dirp);
    while(de!=NULL)
    {
        puts(de->d_name);
        de = readdir(dirp);
    }



    closedir(dirp);
    return 0;
   
}
