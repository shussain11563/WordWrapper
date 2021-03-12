OUTPUT=ww
CFLAGS= -g -std=c99 -Wall -Wvla -fsanitize=address,undefined 
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

driver: driver.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o driver driver.o strbuf.o

driver.o: driver.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined driver.c






