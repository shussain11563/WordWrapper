 typedef struct {
    size_t length;
    size_t used;
    char *data;
} strbuf_t;

int sb_init(strbuf_t *, size_t);
void sb_destroy(strbuf_t *);
int sb_append(strbuf_t *, char); // put char at used, ++used (double if nec)
int sb_remove(strbuf_t *, char*); // removes last char -> used-1
int sb_insert(strbuf_t *, int, char);
int sb_reset(strbuf_t *); // used for project 1

/* additional one */
int sb_concat(strbuf_t *, char*); // append cannot append strings
// hence we use concat

//void dump(strbuf_t *);

/*
Menendez notes:

Piazza @41: 

Let's say the buffer contains "ab" and its backing array contains {'a', 'b', '\0'}.
If we insert 'c' at index 4, then the backing array will become 
{'a', 'b', _, _, 'c', '\0'}. The values at indices 2 and 3 
are undetermined, meaning they could be any char value. 
Since '\0' is a possible char value, 
you don't need to overwrite the old terminator.

Piazza @77
sb_init can return 1 if its argument is nonsensical (length -> 0)

Piazza @72
after sb_init(strbuf_t *sb, 5) we get  [\0, _ , _ , _ , _ ]

Piazza @71 
sb_concat concatanates src to dest only till src's \0 is found

Piazza @88
remove replaces the last character b4 the null terminator 
with the null terminator

Piazza @102
Assume that the string does not contain '\0' before the terminator.
So the only time we insert/append/ etc, used will contain \0
It is not my concern to check that any char at index [0, used)
contains \0. My only concern is that \0 appears at index = used.

Piazza @95
Al_insert shifts eveeyrthing right by 1
*/