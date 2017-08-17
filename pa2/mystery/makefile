CC = gcc
CCFLAGS = -g -Wall -std=c99 -lm -I.
MYSTERY = mystery

all: $(MYSTERY)

$(MYSTERY): mystery.o
	$(CC) $(CCFLAGS) -o mystery mystery.o

$(MYSTERY).o: mystery.c mystery.h
	$(CC) $(CCFLAGS) -c mystery.c mystery.h

clean:
	-rm -f *.o
	-rm -f $(MYSTERY)
	-rm -f *.h.gch
