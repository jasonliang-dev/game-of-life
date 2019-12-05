#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

void nextGeneration(int *grid);

void renderGrid(int *grid, SDL_Renderer *renderer, SDL_Texture *aliveTexture,
                SDL_Texture *deadTexture);

void clearGrid(int *grid);

#endif // GAME_H_
