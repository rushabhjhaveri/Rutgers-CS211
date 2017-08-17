CC = gcc
CCFLAGS = -g -Wall -std=c99 -lm  -I.
RM = rm -f
Y86EMUL = y86emul
all: $(Y86EMUL)

#Build an executable program y86emul from y86emul.c
$(Y86EMUL): y86emul.c
		$(CC) $(CCFLAGS) -o y86emul y86emul.c

clean:
		$(RM) $(Y86EMUL)
