#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

int square_root_int(int n);
int standard_deviation(int data[], int mean);

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
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

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

//void SDL_FreeSurface(SDL_Surface *surface);

int square_root_int(int n)
{
	int r = n;
	while (r * r > n)
	{
		r = r + n / r;
		r = r / 2;
	}

	return r;
}

int standard_deviation(int data[], int mean)  //only use with gray[], len = 9
{
	int sd = 0;
	int div = 1;

	for (size_t i = 0; i < 9; i++)
	{
		if (data[i] != -1)
		{
			sd += (data[i] - mean) * (data[i] - mean);
			div += 1;
		}
	}

	div -= 1;
	sd *= 1 / div;
	return square_root_int(sd);
}

int main(){
  SDL_Surface* image_surface;
  SDL_Surface* screen_surface;
 

  init_sdl();
  
  image_surface = load_image("sudo2.jpeg");
  screen_surface = display_image(image_surface);

  wait_for_keypressed();

  Uint8 r,g,b;
  for(int i = 0; i < image_surface ->h; ++i)
  {
	  for(int j = 0; j < image_surface ->w; ++j)
	  {
		  Uint32 pixel = get_pixel(image_surface, j,i);

		  SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
		  Uint8 average = 0.3*r+0.59*g+0.11*b;
		  r = average;
		  g = average;
		  b = average;
		  Uint32 newpixel = SDL_MapRGB(image_surface->format,r,g,b);
		  put_pixel(image_surface,j,i,newpixel);
	  }
  }
  
 update_surface(screen_surface,image_surface);
 wait_for_keypressed();

 //window len = 9;

 //mean 

 printf("okay\n");

 int mean[image_surface->h][image_surface->w];

 for (int i = 0; i < image_surface->h; i++)
 {
	 for (int j = 0; j < image_surface->w; j++)
	 {
		 mean[i][j] = 0;
		 int gray[9] = {-1, -1, -1, -1, -1, -1, -1, -1};

		 if (i - 1 >= 0)
		 {
			 if (j - 1 >= 0)
			 {
				 Uint32 pixel = get_pixel(image_surface, j - 1, i - 1);
				 SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
				 gray[0] = r;
			 }

			 Uint32 pixel = get_pixel(image_surface, j, i - 1);
                         SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                         gray[1] = r;

			 if (j + 1 < image_surface->w)
                         {
                                 Uint32 pixel = get_pixel(image_surface, j + 1, i - 1);
                                 SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                                 gray[2] = r;
                         }
		 }

		if (i + 1 < image_surface->h)
                 {
                         if (j - 1 >= 0)
                         {
                                 Uint32 pixel = get_pixel(image_surface, j - 1, i + 1);
                                 SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                                 gray[6] = r;
                         }

                         Uint32 pixel = get_pixel(image_surface, j, i + 1);
                         SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                         gray[7] = r;

                         if (j + 1 < image_surface->w)
                         {
                                 Uint32 pixel = get_pixel(image_surface, j + 1, i + 1);
                                 SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                                 gray[8] = r;
                         }
                 }

		if (j - 1 >= 0)
                {
			Uint32 pixel = get_pixel(image_surface, j - 1, i);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                        gray[3] = r;
                }

                Uint32 pixel = get_pixel(image_surface, j, i);
                SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                gray[4] = r;

                if (j + 1 < image_surface->w)
                {
			Uint32 pixel = get_pixel(image_surface, j + 1, i);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
                        gray[5] = r;
                }

		int total_neighbours = 1;

		for (size_t k = 0; k < 9; k++)
		{
			if (gray[k] != -1)
			{
				mean[i][j] += gray[k];
				total_neighbours += 1;
			}
		}

		total_neighbours -= 1;
		mean[i][j] = mean[i][j] / total_neighbours;
		int s = standard_deviation(gray, mean[i][j]);
		mean[i][j] = mean[i][j]*(1 + 0.2*(s/128 - 1));
	 }
 }

 printf("okay2\n");
 Uint8 rf,gf,bf;

 for (int fi = 0; fi < image_surface->h; fi++)
 {
         for (int fj = 0; fj < image_surface->w; fj++)
         {
                Uint32 pixel = get_pixel(image_surface, fj, fi);
                SDL_GetRGB(pixel,image_surface->format,&rf,&gf,&bf);

                if (rf > mean[fi][fj])
		{
			r = 255;
      			g = 255;
      			b = 255;
      			Uint32 newpixel = SDL_MapRGB(image_surface->format,r,g,b);
      			put_pixel(image_surface,fj,fi,newpixel);
		}

		else
		{
			r = 0;
                        g = 0;
                        b = 0;
                        Uint32 newpixel = SDL_MapRGB(image_surface->format,r,g,b);
                        put_pixel(image_surface,fj,fi,newpixel);
		}
         }
 }

  update_surface(screen_surface,image_surface);
  wait_for_keypressed();

  int dil = 4;

  for (int di = 0; di < image_surface->h; di++)
  {
	  for (int dj = 0; dj < image_surface->w; dj++)
	  {
		  Uint32 pixel = get_pixel(image_surface, dj, di);
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
						  Uint32 newpixel = SDL_MapRGB(image_surface->format,0,0,0);
						  put_pixel(image_surface,dj + dk, di + dk,newpixel);
					  }

					  if (dj - dk >= 0)
                                          {
                                                  Uint32 newpixel = SDL_MapRGB(image_surface->format,0,0,0);
                                                  put_pixel(image_surface,dj + dk, di + dk,newpixel);
                                          }
				  }

				  if (di - dk >= 0)
                                  {
                                          if (dj - dk < image_surface->w)
                                          {
                                                  Uint32 newpixel = SDL_MapRGB(image_surface->format,0,0,0);
                                                  put_pixel(image_surface,dj + dk, di + dk,newpixel);
                                          }

                                          if (dj + dk >= 0)
                                          {
                                                  Uint32 newpixel = SDL_MapRGB(image_surface->format,0,0,0);
                                                  put_pixel(image_surface,dj + dk, di + dk,newpixel);
                                          }
                                  }

				  dk += 1;
			  }
		  }
	  }
  }

  update_surface(screen_surface,image_surface);
  wait_for_keypressed();
  SDL_FreeSurface(image_surface);

  return 0;
}
