#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include <math.h>
#include "../image_processing/image_operations.h"


void init_sdl();

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

SDL_Surface* copy_image(SDL_Surface* img);

void SaveImages(SDL_Surface* image_source, size_t corner_x, size_t corner_y, size_t width, size_t height);

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

void SDL_FreeSurface(SDL_Surface* image_surface);

#endif
