#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

#define STEP_SPEED 50
// #define DEBUG_DRAW

enum type { GUARD_UP, GUARD_DOWN, GUARD_LEFT, GUARD_RIGHT, HASH, DOT, X };

#if defined(DEBUG_DRAW)
static char* print_buffer = NULL;

internal void print_grid_types_as_string(s32* grid, s32 width, s32 height) {
  assert(grid);

  if (print_buffer == NULL) {
    print_buffer = (char*)malloc(
        width * height +
        height);  // Allocate buffer to hold the grid and line breaks
  }

  s32 buffer_index = 0;

  for (s32 y = 0; y < height; y++) {
    for (s32 x = 0; x < width; x++) {
      s32 type = grid[x + y * width];
      char c;
      switch (type) {
        case GUARD_UP:
          c = '^';
          break;
        case GUARD_DOWN:
          c = 'v';
          break;
        case GUARD_LEFT:
          c = '<';
          break;
        case GUARD_RIGHT:
          c = '>';
          break;
        case HASH:
          c = '#';
          break;
        case DOT:
          c = '.';
          break;
        case X:
          c = 'X';
          break;
        default:
          printf("Failed to convert grid element to char");
          exit(1);
      }
      print_buffer[buffer_index++] = c;
    }
    print_buffer[buffer_index++] = '\n';  // Add a newline after each row
  }

  // Null-terminate the string
  print_buffer[buffer_index] = '\0';

  // Move cursor to the top-left of the console
  printf("\x1b[H");  // ANSI escape code to move cursor to (0, 0)

  // Print the entire buffer
  printf("%s", print_buffer);

  // Optional: Flush output to ensure immediate display
  fflush(stdout);

  // Pause for the step duration
  Sleep(STEP_SPEED);
}
#endif

// returns the new direction given the starting direction
internal s32 rotate_right(s32 direction) {
  switch (direction) {
    case GUARD_UP: {
      direction = GUARD_RIGHT;
    } break;
    case GUARD_DOWN: {
      direction = GUARD_LEFT;
    } break;
    case GUARD_LEFT: {
      direction = GUARD_UP;
    } break;
    case GUARD_RIGHT: {
      direction = GUARD_DOWN;
    } break;
    default: {
      printf("direction not valid");
      exit(1);
    } break;
  }
  return direction;
}

// returns -1 if not in grid.
internal s32 get_index_infront(s32 direction,
                               s32 guard_x,
                               s32 guard_y,
                               s32 width,
                               s32 height) {
  s32 new_x = guard_x;
  s32 new_y = guard_y;

  switch (direction) {
    case GUARD_UP:
      new_y -= 1;
      break;
    case GUARD_DOWN:
      new_y += 1;
      break;
    case GUARD_LEFT:
      new_x -= 1;
      break;
    case GUARD_RIGHT:
      new_x += 1;
      break;
    default:
      printf("direction is not valid");
      exit(1);
  }

  if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
    return -1;
  }

  return new_x + new_y * width;
}

internal void simulate(s32* grid, s32 width, s32 height) {
  while (true) {
    // search for the guard.
    s32 x = 0;
    s32 y = 0;
    s32 direction = -1;

    for (; y < height; y++) {
      for (x = 0; x < width; x++) {
        s32 current = grid[x + y * width];
        if (current == GUARD_UP || current == GUARD_DOWN ||
            current == GUARD_LEFT || current == GUARD_RIGHT) {
          direction = current;
          goto found;
        }
      }
    }
  found:
    if (direction == -1) {
      printf("guard not found. exiting\n");
      exit(1);
    }

    s32 infront = get_index_infront(direction, x, y, width, height);

    if (infront == -1) {
      break;
    }

    if (grid[infront] != HASH) {
      grid[infront] = direction;
      grid[x + y * width] = X;
#if defined(DEBUG_DRAW)
      print_grid_types_as_string(grid, width, height);
#endif
    } else {
      s32 new_direction = rotate_right(direction);
      grid[x + y * width] = new_direction;
#if defined(DEBUG_DRAW)
      print_grid_types_as_string(grid, width, height);
#endif
    }
  }
}

internal s32 count_x(s32* grid, s32 width, s32 height) {
  s32 num_x = 0;
  for (s32 y = 0; y < height; y++) {
    for (s32 x = 0; x < width; x++) {
      if (grid[x + y * width] == X) {
        num_x++;
      }
    }
  }

  // + 1 because we do not allow the guard to exit the grid.
  // however, the gaurd should exit the grid and leave and X
  // is his last position.
  return num_x + 1;
}

s32 main(void) {
#if defined(DEBUG_DRAW)
  system("cls");
  Sleep(1000);
#endif
  size_t length = 0;
  char* input_file_path = djc_get_input_file("\\resources\\6\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &length);

  if (file == NULL) {
    printf("Failed to load file\n");
    return 1;
  }

  s32 width = (s32)djc_strlen_until_newline(file) - 1;
  s32 height = (s32)djc_count_lines_in_file(file);

  djc_strip_crlf(file);

  s32* grid = (s32*)malloc(width * height * sizeof(s32));

  for (s32 i = 0; i < width * height; i++) {
    switch (file[i]) {
      case '.': {
        grid[i] = DOT;
      } break;

      case '#': {
        grid[i] = HASH;
      } break;

      case '^': {
        grid[i] = GUARD_UP;
      } break;

      case 'v': {
        grid[i] = GUARD_DOWN;
      } break;

      case '<': {
        grid[i] = GUARD_LEFT;
      } break;

      case '>': {
        grid[i] = GUARD_RIGHT;
      } break;

      default: {
        printf("Parsing failed. Unknown char");
        exit(1);
      } break;
    }
  }

  simulate(grid, width, height);

  printf("Answer (d6p1) = %u\n", count_x(grid, width, height));

  free(grid);
  free(file);
#if defined(DEBUG_DRAW)
  free(print_buffer);
#endif
  return 0;
}
