#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

internal u64 count_x_mas(char** grid, s32 rows, s32 cols) {
  assert(grid);
  u64 found = 0;

  for (s32 x = 0; x < rows; x++) {
    for (s32 y = 0; y < cols; y++) {
      if (grid[y][x] != 'A')
        continue;

      s32 dx = 0;
      s32 dy = 0;

      dx = x - 1;
      dy = y - 1;
      if (dx < 0 || dy < 0)
        continue;
      char top_left = grid[dy][dx];

      dx = x + 1;
      dy = y - 1;
      if (dx > cols || dy > rows)
        continue;
      char top_right = grid[dy][dx];

      dx = x - 1;
      dy = y + 1;
      if (dx < 0 || dy > rows)
        continue;
      char bot_left = grid[dy][dx];

      dx = x + 1;
      dy = y + 1;
      if (dx > cols || dy > rows)
        continue;
      char bot_right = grid[dy][dx];

      u32 ms = 'M' + 'S';
      if ((u32)top_left + (u32)bot_right == ms &&
          (u32)bot_left + (u32)top_right == ms) {
        ++found;
      }
    }
  }
  return found;
}

internal u64 line_length(const char* string) {
  assert(string);

  u64 num = 0;
  while (*string && *string != '\n') {
    ++string;
    ++num;
  }
  return num;
}

s32 main(void) {
  u64 file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\4\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  s32 rows = (int)line_length(file);
  s32 cols = 0;

  {
    char* current = file;

    while (*current && *current != '\r') {
      ++current;
      ++cols;
    }
  }

  char** array = malloc((u64)rows * sizeof(char*));
  DJC_MALLOC_CHECK(array);

  for (s32 i = 0; i < rows; i++) {
    array[i] = malloc((u64)cols * sizeof(char));
    DJC_MALLOC_CHECK(array[i]);
  }

  for (s32 i = 0; i < rows; i++) {
    for (s32 j = 0; j < cols; j++) {
      array[i][j] = file[i * (cols + 2) + j];
    }
  }

  printf("Answer (d4p2) = %zu\n", count_x_mas(array, (int)rows, (int)cols));

  // OS will do cleanup.
  return 0;
}
