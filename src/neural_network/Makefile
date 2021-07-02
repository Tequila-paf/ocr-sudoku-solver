# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS=  -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl

SRC = main.c neural_network.c  ../tools/tools.c ../image_processing/image_operations.c ../image_processing/segmentation.c ../image_processing/box_blur.c ../image_processing/hough.c ../image_processing/sobel.c 
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)


all:  main

main: $(OBJ)

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
