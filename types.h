#ifndef STATE_H_
#define STATE_H_

#include "constants.h"
#include <stdbool.h>

typedef struct {
  bool quit;
  bool paused;
  bool mouseDown;
  int buttonMakeAlive;
  int grid[ROWS * COLUMNS];
} state_t;

#endif // STATE_H_
