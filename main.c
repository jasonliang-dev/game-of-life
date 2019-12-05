#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ASSET_DIRECTORY "../assets/"
#define CELL_SIZE 20

const int ROWS = SCREEN_HEIGHT / CELL_SIZE;
const int COLUMNS = SCREEN_WIDTH / CELL_SIZE;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *cellTexture = NULL;
SDL_Texture *cellMutedTexture = NULL;

bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize. SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }

  window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return false;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
    return false;
  }

  return true;
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

bool loadMedia() {
  cellTexture = loadTexture(ASSET_DIRECTORY "cell.png");
  cellMutedTexture = loadTexture(ASSET_DIRECTORY "cell-muted.png");

  if (cellTexture == NULL) {
    return false;
  }

  if (cellMutedTexture == NULL) {
    return false;
  }

  return true;
}

void cleanup() {
  SDL_DestroyTexture(cellTexture);
  SDL_DestroyTexture(cellMutedTexture);
  cellTexture = NULL;
  cellMutedTexture = NULL;

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
  bool quit = false;
  bool paused = true;
  bool mouseDown = false;
  int buttonMakeAlive = 0;
  int grid[ROWS * COLUMNS];

  for (int i = 0; i < ROWS * COLUMNS; i++) {
    grid[i] = 0;
  }

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_MOUSEMOTION: {
        int x = event.button.x / CELL_SIZE;
        int y = event.button.y / CELL_SIZE;
        grid[y * COLUMNS + x] = buttonMakeAlive;
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
        mouseDown = true;
        buttonMakeAlive = event.button.button == 1;
        break;
      }
    }

    SDL_RenderClear(renderer);

    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLUMNS; c++) {
        SDL_Rect quad = {c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer,
                       grid[r * COLUMNS + c] ? cellTexture : cellMutedTexture,
                       NULL, &quad);
      }
    }

    SDL_RenderPresent(renderer);
  }

  cleanup();

  return 0;
}
