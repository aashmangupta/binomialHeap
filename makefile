OBJS = scanner.o vertex.o edge.o integer.o real.o string.o sll.o dll.o queue.o bst.o avl.o binomial.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all : prim test-binomial

prim : prim.c $(OBJS)
		gcc $(LOPTS) prim.c $(OBJS) -o prim -lm

test-binomial : test-binomial.c $(OBJS)
		gcc $(LOPTS) test-binomial.c $(OBJS) -o test-binomial -lm

binomial.o : binomial.c binomial.h
		gcc $(OOPTS) binomial.c

vertex.o : vertex.c vertex.h
		gcc $(OOPTS) vertex.c

edge.o : edge.c edge.h
		gcc $(OOPTS) edge.c

avl.o : avl.c avl.h
		gcc $(OOPTS) avl.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

sll.o : sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o : dll.c dll.h
		gcc $(OOPTS) dll.c

queue.o : queue.c queue.h
		gcc $(OOPTS) queue.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

real.o : real.c real.h
		gcc $(OOPTS) real.c

string.o : string.c string.h
		gcc $(OOPTS) string.c

valgrind : all
		echo valgrind on binomial
		valgrind ./test-binomial

test : all
		echo testing prim
		prim -v
		prim p-0-0.data
		echo testing binomial
		./test-binomial

clean :
	rm -rf $(OBJS) *.dSYM *.o prim test-binomial
