# Makefile

CC=gcc -fsanitize=address
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl`-Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0  sdl` -lSDL_image -lm -ldl

SRC = ui.c ../image_processing/image_operations.c ../image_processing/segmentation.c ../neural_network/neural_network.c  ../solver/file_handling.c  ../tools/tools.c ../image_processing/box_blur.c ../image_processing/sobel.c  ../image_processing/hough.c ../image_processing/rotate.c ../solver/solver.c ../solver/solver_tools.c ../solver/file_handling.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all:  ui

ui: $(OBJ)


clean:
	${RM} *.o
	${RM} *.d
	${RM} ui

# END
