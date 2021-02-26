#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strbuf.h"
#include <strbuf.c">

int main(int argc, char**argv){

}

/*
    input: two arguments
        either a directory or a file
        if second argument missing: stdin -> stdout

    may only use open,read,write for files
    don't read the entire file in a single call to read


BE RIGHT BACK <---

    error checks

    if argc == 2
        stdin for input -> dumped into stdout

    else if(argc == 3)
        determine whether argv[2] is file / directory
        using stat and S_ISDIR and S_ISREG
        if file:
        {
            

        }
        else its directory:
        {
            open each regular file -> sameFileName.wrap
            ignore . files or those whos names start with wrap
        }
    else
        error
*/

