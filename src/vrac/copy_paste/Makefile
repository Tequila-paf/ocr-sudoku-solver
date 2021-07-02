# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: display cut_paste

display: display.o
cut_paste: pixel_operations.o cut_paste.o

cut_paste.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} cut_paste

# END
