CC = gcc
CCFLAGS = -g -Wall -std=c99 -lm -I.
RM = rm -f
CALC = calc
FORMAT = format

all: $(CALC) $(FORMAT)

#Build an executable program calc from calc.c
$(CALC): calc.c
		$(CC) $(CCFLAGS) -o calc calc.c

$(FORMAT): format.c
		$(CC) $(CCFLAGS) -o format format.c

clean:
		$(RM) $(CALC)
		$(RM) $(FORMAT)
