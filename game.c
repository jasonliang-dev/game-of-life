#include "constants.h"
#include <SDL2/SDL.h>

typedef struct node {
  int x;
  int y;
  int alive;
  struct node *next;
} node_t;

void push(node_t **head, int x, int y, int alive) {
  node_t *temp;
  temp = malloc(sizeof(node_t));
  temp->x = x;
  temp->y = y;
  temp->alive = alive;
  temp->next = *head;
  *head = temp;
}

int countNeighbours(const int *grid, int x, int y) {
  int neighbours = 0;

  for (int r = y - 1; r <= y + 1; r++) {
    for (int c = x - 1; c <= x + 1; c++) {
      if (r != y || c != x) {
        neighbours += grid[r * COLUMNS + c];
      }
    }
  }

  return neighbours;
}

void nextGeneration(int *grid) {
  node_t *patches = NULL;
  int neighbours;
  int self;

  for (int r = 1; r < ROWS - 1; r++) {
    for (int c = 1; c < COLUMNS - 1; c++) {
      neighbours = countNeighbours(grid, c, r);
      self = grid[r * COLUMNS + c];

      // stolen from: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
      // Any live cell with fewer than two live neighbours dies, as if by
      // underpopulation.
      if (self && neighbours < 2) {
        push(&patches, c, r, 0);
      }
      // Any live cell with more than three live neighbours dies, as if by
      // overpopulation.
      else if (self && neighbours > 3) {
        push(&patches, c, r, 0);
      }
      // Any dead cell with exactly three live neighbours becomes a live cell,
      // as if by reproduction.
      else if (!self && neighbours == 3) {
        push(&patches, c, r, 1);
      }
      // Any live cell with two or three live neighbours lives on to the next
      // generation. this case is already covered.
    }
  }

  node_t *temp = NULL;
  while (patches != NULL) {
    grid[patches->y * COLUMNS + patches->x] = patches->alive;
    temp = patches;
    patches = patches->next;
    free(temp);
  }
}

void renderGrid(int *grid, SDL_Renderer *renderer, SDL_Texture *aliveTexture,
                SDL_Texture *deadTexture) {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      SDL_Rect quad = {c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE};
      SDL_RenderCopy(renderer,
                     grid[r * COLUMNS + c] ? aliveTexture : deadTexture, NULL,
                     &quad);
    }
  }
}

void clearGrid(int *grid) {
  for (int i = 0; i < ROWS * COLUMNS; i++) {
    grid[i] = 0;
  }
}
