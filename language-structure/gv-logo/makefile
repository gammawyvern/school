CFLAGS = -Wall -Wextra -g
LDFLAGS = -lSDL2 -lfl -lm

all: gvlogo

gvlogo: gvlogo.tab.c gvlogo.tab.h lex.yy.c
	clang $(CFLAGS) *.c -o $@ $(LDFLAGS)

gvlogo.tab.c: gvlogo.y
	bison -d gvlogo.y

gvlogo.tab.h:

lex.yy.c:
	flex gvlogo.l

clean:
	rm -f gvlogo gvlogo.tab.* lex.yy.c

.PHONY: all clean
.INTERMEDIATE: lex.yy.c gvlogo.tab.c gvlogo.tab.h
