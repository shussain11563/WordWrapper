OUTPUT=ww
CFLAGS= -g -std=c99 -Wall -Wvla -fsanitize=address,undefined 
#remove -c flag
#remove -WVLA ---> "Warn if a variable-length array is used in the code. -Wno-vla prevents the -Wpedantic warning of the variable-length array.
LFLAGS=-lm

%.o: %.c %.h
	gcc $(CFLAGS) -c $@ $< $(LFLAGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

all: $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)



ww: ww.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o ww ww.o strbuf.o

ww.o: ww.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined ww.c

strbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined strbuf.c

