#include "constants.h"
#include "state.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Texture *cellTexture = NULL;
SDL_Texture *cellMutedTexture = NULL;

bool init(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize. SDL_Init Error: %s\n", SDL_GetError());
    return false;
  }

  *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (*window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return false;
  }

  *renderer = SDL_CreateRenderer(
      *window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (*renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0x00);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
    return false;
  }

  return true;
}

SDL_Texture *loadTexture(char *path, SDL_Renderer *renderer) {
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

bool loadMedia(SDL_Renderer *renderer) {
  cellTexture = loadTexture(ASSET_DIRECTORY "cell.png", renderer);
  cellMutedTexture = loadTexture(ASSET_DIRECTORY "cell-muted.png", renderer);

  if (cellTexture == NULL) {
    return false;
  }

  if (cellMutedTexture == NULL) {
    return false;
  }

  return true;
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
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

void updateCellByEvent(const SDL_Event *event, int *grid, int value) {
  int x = event->button.x / CELL_SIZE;
  int y = event->button.y / CELL_SIZE;
  grid[y * COLUMNS + x] = value;
}

void handleEvent(SDL_Event *event, state_t *state) {
  while (SDL_PollEvent(event)) {
    switch (event->type) {
    case SDL_QUIT:
      state->quit = 1;
      break;
    case SDL_MOUSEMOTION:
      if (state->mouseDown) {
        updateCellByEvent(event, state->grid, state->buttonMakeAlive);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      state->mouseDown = true;
      state->buttonMakeAlive = event->button.button == 1;
      updateCellByEvent(event, state->grid, state->buttonMakeAlive);
      break;
    case SDL_MOUSEBUTTONUP:
      state->mouseDown = false;
      break;
    }
  }
}

int main() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (!init(&window, &renderer)) {
    return EXIT_FAILURE;
  }

  if (!loadMedia(renderer)) {
    return EXIT_FAILURE;
  }

  state_t state = {
      .quit = false,
      .paused = true,
      .mouseDown = false,
      .buttonMakeAlive = 0,
  };

  for (int i = 0; i < ROWS * COLUMNS; i++) {
    state.grid[i] = 0;
  }

  SDL_Event event;
  Uint32 ticks;
  Uint32 now;
  while (!state.quit) {
    handleEvent(&event, &state);
    now = SDL_GetTicks();

    if (now - ticks < TICK_SPEED) {
      continue;
    }

    ticks = now;

    SDL_RenderClear(renderer);

    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLUMNS; c++) {
        SDL_Rect quad = {c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer,
                       state.grid[r * COLUMNS + c] ? cellTexture
                                                   : cellMutedTexture,
                       NULL, &quad);
      }
    }

    SDL_RenderPresent(renderer);
  }

  cleanup(window, renderer);

  return 0;
}
