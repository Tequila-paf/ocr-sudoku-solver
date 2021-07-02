#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "math.h"

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

void SDL_FreeSurface(SDL_Surface *surface);

void copy_paste(SDL_Surface *in, SDL_Surface *out,int x1,int x2,int y1,int y2)
{
    for(int i =0;i<x2-x1;i++)
    {
        for(int j=0;j<y2-y1;j++)
        {
            Uint32 pixel = get_pixel(in,x1+i,y1+j);
            put_pixel(out,x1+i,y1+j,pixel);
        }
    }
}

void hugh_test(SDL_Surface *image)
{
    //base pseudo
    // for all x
    //  for all y
    //   if edge point (x,y)
    //     for all theta(0-180)
    //      rho = x*cos(theta)+y*sin(theta)
    //      H(theta,rho)+=1;
    int width = image->w;
    int heigth= image->h;
    int rho=0;
    int H[width][heigth];
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<heigth;j++)
        {
            H[i][j]=0;
        }
    }
    for(int x=0; x<width;x++)
    {
        for(int y=0; y<heigth; y++)
        {
            //if (edge(*image,x,y)==1)
            //{
                for(int t=0;t<180;t++)
                {
                    rho = x*cos(t)+y*sin(t);
                    H[t][rho]+=1;
                }
            //}
        }
    }
}


    


int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    SDL_Surface* image_out;
    image_surface = load_image("my_image.jpg");
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    int width = image_surface->w;
    int height = image_surface->h;   

    image_out =load_image("out.jpg");
    wait_for_keypressed();
    display_image(image_out);
    copy_paste(image_surface,image_out,0,width/2,0,height/2);
    
    SDL_SaveBMP(image_out,"out.bmp");

    display_image(image_out);
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
