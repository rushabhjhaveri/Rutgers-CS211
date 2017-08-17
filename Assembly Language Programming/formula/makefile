CC = gcc
CCFLAGS = -g -Wall -std=c99 -lm -I.
FORMULA = formula


all: $(FORMULA)

$(FORMULA): formula.o nCr.o
	$(CC) $(CCFLAGS) -o formula formula.o nCr.o

$(FORMULA).o: formula.c formula.h
	$(CC) $(CCFLAGS) -c formula.c formula.h

nCr.o: nCr.s nCr.h
	$(CC) $(CCFLAGS) -c nCr.s nCr.h

clean:
	-rm -f *.o
	-rm -f $(FORMULA)
	-rm -f *.h.gch
