#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

#define STEP_SPEED 100
#define DEBUG_DRAW

enum type { UP, RIGHT, DOWN, LEFT, HASH, DOT, HORIZONTAL, VERTICAL, WALL };

#if defined(DEBUG_DRAW)
static char* print_buffer = NULL;

internal void print_grid_types_as_string(s32* grid, s32 width, s32 height) {
  assert(grid);

  if (print_buffer == NULL) {
    print_buffer = (char*)malloc(width * height + height + 1);
  }

  s32 buffer_index = 0;

  for (s32 y = 0; y < height; y++) {
    for (s32 x = 0; x < width; x++) {
      s32 type = grid[x + y * width];
      char c;
      switch (type) {
        case UP:
          c = '^';
          break;
        case DOWN:
          c = 'v';
          break;
        case LEFT:
          c = '<';
          break;
        case RIGHT:
          c = '>';
          break;
        case HASH:
          c = '#';
          break;
        case DOT:
          c = '.';
          break;
        case HORIZONTAL:
          c = '-';
          break;
        case VERTICAL:
          c = '|';
          break;
        case WALL:
          c = '0';
          break;
        default:
          printf("Failed to convert grid element to char: %c", type);
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

internal s32 rotate_right(s32 direction) {
  // nice trick due to the ordering of the direction enum.
  return direction = (direction + 1) % 4;
}

internal void get_relative_index(s32 guard_direction,
                                 s32 direction,
                                 s32 guard_x,
                                 s32 guard_y,
                                 s32* out_x,
                                 s32* out_y) {
  static const s32 offsets[4][2] = {
      {0, -1},  // UP
      {1, 0},   // RIGHT
      {0, 1},   // DOWN
      {-1, 0}   // LEFT
  };

  if (guard_direction < UP || guard_direction > LEFT) {
    printf("guard_direction is not valid. dir=%i", guard_direction);
    exit(1);
  }

  if (direction < UP || direction > LEFT) {
    printf("direction is not valid. dir=%i", direction);
    exit(1);
  }

  s32 index = (guard_direction + direction) % 4;
  *out_x = guard_x + offsets[index][0];
  *out_y = guard_y + offsets[index][1];
}

internal s32 is_trail(s32 val) {
  return val == HORIZONTAL || val == VERTICAL;
}

internal s32 guard_to_adjecent_trail(s32 direction) {
  s32 trail;
  switch (direction) {
    case DOWN:
    case UP:
      trail = HORIZONTAL;
      break;
    case RIGHT:
    case LEFT:
      trail = VERTICAL;
      break;
    default:
      printf("Failed to map guard direction to trail direction.");
      exit(1);
  }
  return trail;
}

internal s32 dir_to_trail(s32 direction) {
  s32 trail;
  switch (direction) {
    case UP:
    case DOWN:
      trail = VERTICAL;
      break;
    case RIGHT:
    case LEFT:
      trail = HORIZONTAL;
      break;
    default:
      printf("Failed to convert dir to trail: dir = %u", direction);
      exit(1);
  }
  return trail;
}

internal s32 simulate(s32* grid, s32 width, s32 height) {
#if defined(DEBUG_DRAW)
  print_grid_types_as_string(grid, width, height);
#endif
  s32 num_obstructions = 0;

  s32* obs = (s32*)malloc(sizeof(s32) * width * height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      obs[x + y * width] = DOT;
    }
  }

  // search for the guard.
  // ----
  s32 x = 0;
  s32 y = 0;
  s32 direction = -1;

  for (; y < height; y++) {
    for (x = 0; x < width; x++) {
      s32 current = grid[x + y * width];
      if (current >= UP && current <= RIGHT) {
        direction = current;
        goto found_guard;
      }
    }
  }
found_guard:
  if (direction == -1) {
    printf("Guard not found.");
    exit(1);
  }
  // ----

  while (true) {
    s32 front_x;
    s32 front_y;
    get_relative_index(direction, UP, x, y, &front_x, &front_y);
    s32 index_infront = front_x + front_y * width;

    // If we are next to the edge of the grid and our direction is pointing
    // out of the grid. Then we are finished.
    if (front_x < 0 || front_x >= width || front_y < 0 || front_y >= height) {
      break;
    }

    if (grid[index_infront] == DOT || is_trail(grid[index_infront])) {
      grid[index_infront] = direction;
      grid[x + y * width] = dir_to_trail(direction);
      x = front_x;
      y = front_y;
      print_grid_types_as_string(grid, width, height);

      // Part 2: check if we can make it into an infinite loop.
      // ----
      {
        s32 right_x;
        s32 right_y;
        get_relative_index(direction, RIGHT, x, y, &right_x, &right_y);
        s32 right_index = right_x + right_y * width;

        if (right_x >= 0 && right_x < width && right_y >= 0 &&
            right_y < height &&
            grid[right_index] == guard_to_adjecent_trail(direction)) {
          ++num_obstructions;

          get_relative_index(direction, UP, x, y, &front_x, &front_y);
          index_infront = front_x + front_y * width;
          obs[index_infront] = WALL;  // we need to plus 1 in the direction tha
                                      // tthe guard is facing.
#if defined(DEBUG_DRAW)
          print_grid_types_as_string(obs, width, height);

#endif
        }
      }
      // ----
#if defined(DEBUG_DRAW)
      print_grid_types_as_string(grid, width, height);
#endif
    } else if (grid[index_infront] == HASH) {
      direction = rotate_right(direction);
      grid[x + y * width] = direction;
#if defined(DEBUG_DRAW)
      print_grid_types_as_string(grid, width, height);
#endif
    }
  }
#if defined(DEBUG_DRAW)
  print_grid_types_as_string(obs, width, height);
#endif
  return num_obstructions;
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
        grid[i] = UP;
      } break;

      case 'v': {
        grid[i] = DOWN;
      } break;

      case '<': {
        grid[i] = LEFT;
      } break;

      case '>': {
        grid[i] = RIGHT;
      } break;

      case '-': {
        grid[i] = HORIZONTAL;
      } break;

      default: {
        printf("Parsing failed. Unknown char = %c", file[i]);
        exit(1);
      } break;
    }
  }

  s32 answer = simulate(grid, width, height);

  printf("Answer (d6p2) = %i\n", answer);

  free(grid);
  free(file);
#if defined(DEBUG_DRAW)
  free(print_buffer);
#endif
  return 0;
}
