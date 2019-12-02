#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ASSET_DIRECTORY "../assets/"

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Surface *hello = NULL;

int init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize SDL_Init Error: %s\n", SDL_GetError());
    return 0;
  }

  window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return 0;
  }

  screenSurface = SDL_GetWindowSurface(window);

  return 1;
}

int loadMedia() {
  hello = SDL_LoadBMP(ASSET_DIRECTORY "hello.bmp");
  if (hello == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    return 0;
  }

  return 1;
}

void cleanup() {
  SDL_FreeSurface(hello);
  hello = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}

SDL_Surface *loadSurface(char *path) {
  SDL_Surface *surface = SDL_LoadBMP(path);

  if (surface == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
  }

  return surface;
}

int main() {
  if (!init()) {
    return EXIT_FAILURE;
  }

  if (!loadMedia()) {
    return EXIT_FAILURE;
  }

  SDL_Event event;
  int quit = 0;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      }
    }

    SDL_BlitSurface(hello, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
  }

  cleanup();

  return 0;
}
