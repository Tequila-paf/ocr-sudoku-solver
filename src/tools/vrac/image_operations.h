#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include "pixel_operations.h"
#include <math.h>
void grayscale(SDL_Surface* image_surface);
Uint8 otsuThreshold(SDL_Surface* image_surface);
void binarize(SDL_Surface* image_surface);
void increaseLux(SDL_Surface* image_surface, int lux);
void dilate(SDL_Surface* image_surfacen, int dil);
SDL_Surface* rotate(SDL_Surface* img, double angle);
SDL_Surface* noiseReduction(SDL_Surface* img);
#endif
