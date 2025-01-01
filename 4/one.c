#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

internal s32 num_movements = 4;
internal s32 dx[] = {0, 1, 0, -1};
internal s32 dy[] = {1, 0, -1, 0};
internal size_t found = 0;

// Function to check if a word exists starting from (x, y) in a given direction
internal s32 search_direction(char** grid,
                              s32 rows,
                              s32 cols,
                              s32 x,
                              s32 y,
                              const char* word,
                              s32 dir) {
  assert(grid);

  size_t len = strlen(word);

  for (s32 i = 0; i < (int)len; i++) {
    s32 nx = x + i * dx[dir];
    s32 ny = y + i * dy[dir];

    // Check if the position is out of bounds
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols)
      return 0;

    // Check if the character matches
    if (grid[nx][ny] != word[i])
      return 0;
  }

  return 1;
}

// Function to search for a word in the grid
internal void find_word(char** grid, s32 rows, s32 cols, const char* word) {
  assert(grid);

  for (s32 x = 0; x < rows; x++) {
    for (s32 y = 0; y < cols; y++) {
      for (s32 dir = 0; dir < num_movements; dir++) {
        if (search_direction(grid, rows, cols, x, y, word, dir)) {
          ++found;
        }
      }
    }
  }
}

s32 main(void) {
  size_t file_size = 0;
  char* input_file_path =
      djc_get_input_file("\\resources\\day_four\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  //  djc_convert_ctrl_to_lf(file);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  s32 rows = (int)djc_line_length(file);
  s32 cols = 0;

  {
    char* current = file;

    while (*current && *current != '\r') {
      ++current;
      ++cols;
    }
  }

  char** array = malloc((size_t)rows * sizeof(char*));
  if (array == NULL) {
    fprintf(stderr, "malloc failed.");
    exit(1);
  }

  for (s32 i = 0; i < rows; i++) {
    array[i] = malloc((size_t)cols * sizeof(char));
    if (array[i] == NULL) {
      fprintf(stderr, "malloc failed.");
      exit(1);
    }
  }

  for (s32 i = 0; i < rows; i++) {
    for (s32 j = 0; j < cols; j++) {
      array[i][j] = file[i * (cols + 2) + j];
    }
  }

  find_word(array, (int)rows, (int)cols, "MAS");
  printf("Answer (d4p1) = %zu\n", found);
  return 0;
}
