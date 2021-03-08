OUTPUT=ww
CFLAGS= -g -std=c99 -Wall -Wvla -fsanitize=address,undefined 
#remove -c flag
#remove -WVLA ---> "Warn if a variable-length array is used in the code. -Wno-vla prevents the -Wpedantic warning of the variable-length array.
LFLAGS=-lm
REG_FILE=output.txt
NO_PERMISSION_FILE= bar
DIR=some_text
NO_PERMISSION_DIR=
EMPTY_DIRECTORY=foo
BLANK_FILE=blank.txt
DIR_IN_DIR=some_text





#gcc -g -o -std=c99 -Wall -Wvla -fsanitize=address test.c test

%.o: %.c %.h
	gcc $(CFLAGS) -c $@ $< $(LFLAGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

all: $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)

test1: ww
	./ww 49 RunThis/endgame.txt | cmp RunThis/Solution/endgameSol2.txt 

test2: ww
	./ww 35 RunThis/bohemianR.txt | cmp RunThis/Solution/bohemianSol1.txt 

test3: ww
	./ww 24 RunThis/jnjArticle.txt | cmp RunThis/Solution/articleSol1.txt 

#test4: ww
#	./ww 24 RunThis/jnjArticle.txt | diff --normal RunThis/Solution/articleSol1.txt -

#Negative Width
test5: ww
	./ww -24 $(REG_FILE)

#Invalid Width 
test6: ww
	./ww 20c $(REG_FILE)

#Zero Width
test7: ww
	./ww 0 $(REG_FILE)

#File with No Permission
test8: ww
	./ww 40 $(NO_PERMISSION_FILE)

#Directory with No Permission
test9: ww
	./ww 40 $(NO_PERMISSION_DIRECTORY)

#Directory with at least one file (THIS WORKS WITH *SOME_DIRECTORY_NAME* & *SOME_DIRECTORY_NAME/*)
test10: ww
	./ww 40 $(DIRECTORY)

#Empty Directory
test11: ww
	./ww 40 $(EMPTY_DIRECTORY)

#Blank File
test12: ww
	./ww 20 $(BLANK_FILE)

#DIRECTORY WITHIN A DIRECTORY ALONG WITH FILES
test13: ww
	./ww 20 $(DIR_IN_DIR)

#DIRECTORY WITH n FILES AND WITH x FILES THAT HAVE NO PERMISSIONS (WHERE x < n)
test20: ww
	./ww 20 $(DIR_IN_DIR)

#WIDTH OF 1
test14: ww
	./ww 1 $(REG_FILE)

#WIDTH OF 2
test15: ww
	./ww 2 $(REG_FILE)

#FILE WITH AT LEAST A CHARACTER AT THE BEGINNING WITH MANY SPACES AND NEW LINES SUCCEEDING 
test16: ww
	./ww 20 $(REG_FILE)

#FILE WITH AT LEAST A CHARACTER AT THE END WITH MANY SPACES AND NEW LINES PRECEDING 
test17: ww
	./ww 2 $(REG_FILE)





ww: ww.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o ww ww.o strbuf.o

ww.o: ww.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined ww.c

strbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined strbuf.c



