#include "tools.h"

/*-------------------------------------------------Images-------------------------------------------------------*/

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);

/*    
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
*/
	
    return img;
}


SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

SDL_Surface* copy_image(SDL_Surface *img)
{
	Uint32 pixel;
	SDL_Surface* copy;

	copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
			img->w,
			img->h,
			img->format->BitsPerPixel, 0, 0, 0, 0);
	for (int i = 0; i < img->w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img, i, j);
			put_pixel(copy, i, j, pixel);
		}
	}

	return copy;
}


double GetStandardDeviation(SDL_Surface *img)
{
	double std_dev = 0;
	int pix_val;
	double mean = 0;
	double squared_sum = 0;
	int n;
	int nb_on = 0;

	Uint32 pixel;
	Uint8 r, g, b;

	n = (img -> w) * (img -> h);

	for (int i = 0; i < img -> h; i++)
	{
		for (int j = 0; j < img -> w; j++)
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				nb_on += 1;
			}
		}
	}

	mean = nb_on / n;

	for (int i = 0; i < img -> h; i++)
	{
		for (int j = 0; j < img -> w; j++)
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
				pix_val = 1000;
			else
				pix_val = -50;

			squared_sum += (pix_val - mean) * (pix_val - mean);
		}
	}

	std_dev = sqrt(squared_sum / (n - 1));

	return std_dev;

}


void _save_image(SDL_Surface* image_source, size_t x, size_t y, int width, int height, char* path)
{
	SDL_Surface* img = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	SDL_Surface* scaled_img = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);

	Uint32 pixel;

	for (size_t i = 0; i < (size_t) (img -> w); i++)
	{
		for (size_t j = 0; j < (size_t) (img -> h); j++)
		{
			pixel = get_pixel(image_source, x + i, y + j);
		       put_pixel(img, i, j, pixel);	
		}
	}

	SDL_SoftStretch(img, NULL, scaled_img, NULL);

	NoiseReduction(scaled_img);

	SDL_SaveBMP(scaled_img, path);

	free(img);
	free(scaled_img);
}

double GetAverageStandardDeviation()
{
	char path[31] = "../image_processing/digits/00\0";

	double avg_stdev = 0;
	SDL_Surface *img;

	for (int i = 0; i < 81; i++)
	{
		path[27] = (i / 10) + 48;
		path[28] = (i % 10) + 48;

		if (i <= 9)
		{
			path[27] = i + 48;
			path[28] = '\0';
		}

		img = load_image(path);

		if (img != NULL)
			avg_stdev += GetStandardDeviation(img);
	}

	return avg_stdev / 81;
}

void RemoveBlankImages()
{
	char path[31] = "../image_processing/digits/00\0";

	SDL_Surface *img;

	double avg_stdev = GetAverageStandardDeviation();

	for (int i = 0; i < 81; i++)
	{
		path[27] = (i / 10) + 48;
		path[28] = (i % 10) + 48;

		if (i <= 9)
		{
			path[27] = i + 48;
			path[28] = '\0';
		}

		img = load_image(path);

		if (img != NULL)
		{
			if (GetStandardDeviation(img) < avg_stdev)
			{
				remove(path);
			}
		}
	}
}

void SaveImages(SDL_Surface* image_source, size_t corner_x, size_t corner_y, size_t width, size_t height)
{

	// remplacer 50 par taille des cases, une fois detectees


	char name[20]; // nom du fichier, empty array
	char path[31] = "../image_processing/digits/00\0"; //fichier ou stocker images
	int a = 0; // numero du fichier (nom)

	for (size_t i = corner_y; i < (size_t) (corner_y + height - (height / 9 - 1)); i += (int) (height) / 9)
	{
		for (size_t j = corner_x; j < (size_t) (corner_x + width - (width / 9 - 1)); j += (int) (width) / 9)
		{
			sprintf(name, "%i", a);// convertir en string le nombre

			// creation du nom de l'image
			if (a < 10)
			{
				path[27] = name[0];
				path[28] = '\0';
			}
			else
			{
				path[27] = name[0];
				path[28] = name[1];
			}
			a += 1; // incremente le numero de l'image
			_save_image(image_source, j, i, width / 9, height / 9, path);
		}
	}


	RemoveBlankImages();

}


