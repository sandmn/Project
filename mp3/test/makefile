#.PHONY:all

#all:list.o play.o test
#
#list.o:list.c
#	gcc -o $@ -c $^
#
#play.o:play.c
#	gcc -o $@ -c $^

test:test.c list.c 
	gcc -o $@ -g $^


#test:test.c list.c play.c
#	gcc -o $@ -g $^


.PHONY:clean

clean:
	rm list play test
