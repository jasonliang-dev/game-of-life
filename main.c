#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ASSET_DIRECTORY "../assets/"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *helloAgain = NULL;

int init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize. SDL_Init Error: %s\n", SDL_GetError());
    return 0;
  }

  window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return 0;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
    return 0;
  }

  return 1;
}

SDL_Texture *loadTexture(char *path) {
  SDL_Surface *loadedSurface = IMG_Load(path);

  if (loadedSurface == NULL) {
    printf("IMG_Load Error: %s\n", IMG_GetError());
    return NULL;
  }

  SDL_Texture *newTexture =
      SDL_CreateTextureFromSurface(renderer, loadedSurface);

  if (newTexture == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

int loadMedia() {
  helloAgain = loadTexture(ASSET_DIRECTORY "preview.png");

  if (helloAgain == NULL) {
    return 0;
  }

  return 1;
}

void cleanup() {
  SDL_DestroyTexture(helloAgain);
  helloAgain = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  IMG_Quit();
  SDL_Quit();
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

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, helloAgain, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  cleanup();

  return 0;
}
