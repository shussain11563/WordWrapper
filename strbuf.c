#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"


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



/*
void dump(strbuf_t *sb){
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
*/