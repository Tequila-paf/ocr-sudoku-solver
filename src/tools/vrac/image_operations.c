#include "image_operations.h"

void grayscale(SDL_Surface* image_surface)
{
	Uint32 pixel;
	Uint8 r, g, b;
	Uint8 average;

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = 0.3 * r + 0.59 * g + 0.11 * b;
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface, j, i, pixel);	
		}
	}
}

Uint8 otsuThreshold(SDL_Surface* image_surface)
{
	unsigned long histo[256];
	Uint8 r, g, b;

 	for (int x = 0; x < 256; x++)
		histo[x] = 0;

 	for (int i = 0; i < image_surface->h; i++)
 	{
	 	for (int j = 0; j < image_surface->w; j++)
	 	{	
		 	Uint32 pixel = get_pixel(image_surface, j, i);
		       	SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

		 	histo[r] += 1;
	 	}
 	}

 	unsigned long sum = 0, sum1 = 0, w1 = 0, w2 = 0;
 	unsigned long m1, m2;
 	float between = 0, max = 0;
 	Uint8 threshold1 = 0, threshold2 = 0;
 	unsigned long total = image_surface->h * image_surface->w;

 	for (int i = 0; i < 256; i++)
	 	sum += i * histo[i];

 	for (int i = 0; i < 256; i++)
 	{
	 	w1 += histo[i];
	 	if (w1 == 0)
		 	continue;

	 	w2 = total - w1;
	 	if (w2 == 0)
		 	break;

	 	sum1 += i * histo[i];
	 	m1 = sum1 / w1;
	 	m2 = (sum - sum1) / w2;
	 	between = w1 * w2 * (m1 - m2) * (m1 - m2);
	 	if (between >= max)
	 	{
		 	threshold1 = i;
		 	if (between > max)
			 	threshold2 = i;

		 	max = between;
	 	}
 	}

 	Uint8 threshold = (threshold1 + threshold2) / 2;
 	return threshold;
}

void binarize(SDL_Surface* image_surface)
{
	Uint8 r, g, b;
	Uint32 pixel;
	Uint8 threshold = otsuThreshold(image_surface);

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

			if (r > threshold)
			{
				pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                        	put_pixel(image_surface, j, i, pixel);
			}

			else
			{
				pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
                                put_pixel(image_surface, j, i, pixel);
			}
		}
	}
}


void dilate(SDL_Surface* image_surface, int dil)
{
	int board[image_surface->h][image_surface->w];

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
			board[i][j] = 0;
	}

	Uint32 pixel;
	Uint8 r, g, b;

	for (int di = 0; di < image_surface->h; di++)
  	{
	  	for (int dj = 0; dj < image_surface->w; dj++)
	  	{
		  	pixel = get_pixel(image_surface, dj, di);
                  	SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

		  	if (r == 0)
		  	{
			  	int dk = 0;

			  	while (dk <= dil)
			  	{
				  	if (di + dk < image_surface->h)
				  	{
					  	if (dj + dk < image_surface->w)
					  	{
						  	board[di + dk][dj + dk] = 1;
					  	}

					  	if (dj - dk >= 0)
                                          	{
                                                  	board[di + dk][dj - dk] = 1;
                                          	}
				  	}

				  	if (di - dk >= 0)
                                  	{
                                          	if (dj + dk < image_surface->w)
                                          	{
                                                  	board[di - dk][dj + dk] = 1;
                                          	}

                                          	if (dj - dk >= 0)
                                          	{
                                                  	board[di - dk][dj - dk] = 1;
                                          	}
                                  	}

				  	dk += 1;
			  	}
		  	}
	  	}
  	}

	Uint32 newpixel;

  	for (int i = 0; i < image_surface->h; i++)
  	{
	  	for (int j = 0; j < image_surface->w; j++)
	  	{
		  	if (board[i][j] == 1)
		  	{	
				newpixel = SDL_MapRGB(image_surface->format,0,0,0);
                        	put_pixel(image_surface, j, i,newpixel);
		  	}
	  	}
  	}
}

void increaseLux(SDL_Surface* image_surface, int lux)
{
	Uint32 pixel;
	Uint8 r, g, b;

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
                        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			if (r > 175)
			{
                        	pixel = SDL_MapRGB(image_surface->format, r - lux, r - lux, r - lux);
                        	put_pixel(image_surface, j, i, pixel);
			}
		}
	}
}

/*
SDL_Surface* rotate(SDL_Surface* img, double angle)
{
	double sin_val = fabs(sin(angle));
	double cos_val = fabs(cos(angle));

	size_t new_width = (int) ((sin_val) * ((img -> h)) + (cos_val) * ((img -> w)));
	size_t new_height = (int) ((sin_val) * ((img -> w))  + (cos_val) * ((img -> h)));

	SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_HWSURFACE, new_width, new_height, 32, 0, 0, 0, 0);

	printf("%f %f", sin_val * (img->h) + cos_val * (img->w), sin_val * (img->w) + cos_val * (img -> h));

	size_t origin_x = (img -> w) / 2;
	size_t origin_y = (img -> h) / 2;

	size_t x;
	size_t y;

	Uint32 pixel;

	for (size_t i = 0; i < (size_t) (img -> w); i++)
	{
		for (size_t j = 0; j < (size_t) (img -> h); j++)
		{
			x = cos(angle) * (j - origin_x) - sin(angle) * (i - origin_y) + origin_x;
			y = sin(angle) * (j - origin_x) + cos(angle) * (i - origin_y) + origin_y;

			pixel = get_pixel(img, i, j);
			
			if (x < new_width && y < new_height)
			{
				put_pixel(new_img, x, y, pixel);
			}



		}
	}

	return new_img;
}
*/

SDL_Surface* rotate(SDL_Surface* img, double angle)
{
	angle =  (M_PI * angle / 180);

	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	int new_height = fabs(img -> w * sin_angle) + fabs(img -> h * cos_angle);
	int new_width = fabs(img -> w * cos_angle) + fabs(img -> h * sin_angle);
	SDL_Surface *result = SDL_CreateRGBSurface(SDL_HWSURFACE, new_width, new_height, 32, 0, 0, 0, 0);

	for (int i = 0; i < new_height; i++)
	{
		for (int j = 0; j < new_width; j++)
		{
			int x = (i - result -> h / 2) * cos_angle - (j - result -> w / 2) * sin_angle + (img -> h) / 2;

			int y = (j - result -> w / 2) * cos_angle + (i - result -> h / 2) * sin_angle + (img -> w) / 2;


			if (x >= 0 && x < img -> h && y >= 0 && y < img -> w)
			{
				Uint32 pixel = get_pixel(img, y, x);
				put_pixel(result, j, i, pixel);
			}
			else
			{
				Uint32 pixel = SDL_MapRGB(result->format, 255, 255, 255);
				put_pixel(result, j, i, pixel);
			}
		}
	}

	

	return result;
}
SDL_Surface* noiseReduction(SDL_Surface* img)
{
	int neighbours[] = {-1, -1, -1, 1, 1, -1, 0, -1, -1, 0, 1, 0, 0, 1, 1, 1};

	Uint32 pixel;

	Uint8 r, g, b;

	SDL_Surface* result = SDL_CreateRGBSurface(SDL_HWSURFACE, img -> w, img -> h, 32, 0, 0, 0, 0);


	for (int i = 0; i < (result -> w); i++)
	{
		for (int j = 0; j < (result -> h); j++)
		{
			int x = 0;
			int y = 1;

			int nbPixelOn = 0;

			while (y < 16)
			{
				pixel = get_pixel(img, i, j);

				SDL_GetRGB(pixel, img->format, &r, &g, &b);
				if (((i + neighbours[x] < (img -> w))
				&& (i + neighbours[x] >= 0))
				&& ((j + neighbours[y] >= 0)
				&& (j + neighbours[y] < (img -> h))))
				{
					pixel = get_pixel(img, i + neighbours[x], j + neighbours[y]);
					SDL_GetRGB(pixel, img->format, &r, &g, &b);
					if (r == 255 && g == 255 && b == 255)
					{
						nbPixelOn += 1;
					}
				}
				x += 2;
				y += 2;	 
			}

			if (nbPixelOn > 4)
			{
				pixel = SDL_MapRGB(img->format, 255, 255, 255);
				put_pixel(result, i, j, pixel);
			}
			else
			{
				pixel = SDL_MapRGB(img -> format, 0, 0, 0);
				put_pixel(result, i, j, pixel);
			}
		}
	}

	return result;
}


