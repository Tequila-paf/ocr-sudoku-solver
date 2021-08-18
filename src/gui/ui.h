#ifndef UI_H_
#define UI_H_

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../image_processing/image_operations.h"
#include "../image_processing/segmentation.h"
#include "../neural_network/neural_network.h"

#include "../solver/solver_tools.h"
#include "../solver/file_handling.h"
#include "../solver/solver.h"

#include "../tools/tools.h"

#include "../image_processing/sobel.h"
#include "../image_processing/hough.h"
#include "../image_processing/rotate.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

int LaunchUserInterface(int argc, char **argv);

void Rescale();

#endif
