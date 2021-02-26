#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strbuf.h"
#include <strbuf.c>

int main(int argc, char**argv){

    // @var buffer = char Array to read from a file
    // @macro BUFFSIZE = (6 Notes: use a macro to define its length), BUFFSIZE >= 1.
    // @var sb = strbuf_t used to store words
    // @var newlineDetectedOnce = bool value -> 0 if false, 1 if true
    // @var pageWidth = argv[2]
    // @var exit_status -> initially 0, set to 1 if we encounter a EXIT_Failure (strlen(word) > pageWidth)
    // @var count -> how many chars we've printed (including whitespace)

}

/*
    Algorithm
    ----------
    Notes and Requirements:
    -> Only ever print one whitespace, consume all others
    -> If you find a newline, traverse the buffer until you find another
        -> If you can't find two consecutive newlines (could be separated by whitespace), then ignore that first newline
    -> You determine when to put newline when you've filled up the page width
    -> Two consecutive new lines means you can print \n\n
    -> Each \n that you print resets the count for page width

    -> Summary: Whitespace are ignored, \n not followed by another \n (could be separated by newlines) are ignored

    -> Keep a running string_buffer of every string in buffer, print that string only after you get a whitespace
        -> continue to concat until you find a whitespace, then do some dump logic
    -> For words that span across buffers (hard), continue to put sb_concat that last string into the sb from Buffer
    -> Every time you find a whitespace, provided enough of 'count allowance' print the contents of sb to the output line
    
    -> Must maintain a check to see whether sb->used ever exceed width, if so, we must print it and continue but return EXIT_FAILURE 5: Erorr Conditions

    Variables used to accomplish the task:
    -> string Buffer -> continually append each char you find. Concat would be tougher since you don't have a \0 string so append
    -> newLineDetectedOnce 
    -> pageWidth
    -> Count for each line
    -> exit_status to set to EXIT_FAILURE if fail
    -> 

*/


/*
    File/Directory Set UP

    input: two arguments
        either a directory or a file
        if second argument missing: stdin -> stdout

    may only use open,read,write for files
    don't read the entire file in a single call to read


BE RIGHT BACK <---

    error checks

    if argc == 2
        stdin for input -> dumped into stdout
        Check 5: Error Conditions

    else if(argc == 3)
        determine whether argv[2] is file / directory
        using stat and S_ISDIR and S_ISREG
        if file:
        {
            
            ALGORITHM
        }
        else its directory:
        {
            open each regular file -> sameFileName.wrap
            ignore . files or those whos names start with wrap

            ALGORITHM
        }
    else
        error
*/

