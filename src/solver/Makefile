# Makefile

CC=gcc -fsanitize=address
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= main.c solver.c file_handling.c solver_tools.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

-include ${DEP}

solver: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} main

# END
