#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/djc.h"

// Directions for 8 possible movements
static int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
static int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
static size_t found = 0;

// Function to check if a word exists starting from (x, y) in a given direction
static int search_direction(char** grid,
                            int rows,
                            int cols,
                            int x,
                            int y,
                            const char* word,
                            int dir) {
  assert(grid);

  size_t len = strlen(word);

  for (int i = 0; i < (int)len; i++) {
    int nx = x + i * dx[dir];
    int ny = y + i * dy[dir];

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
static void find_word(char** grid, int rows, int cols, const char* word) {
  assert(grid);

  for (int x = 0; x < rows; x++) {
    for (int y = 0; y < cols; y++) {
      for (int dir = 0; dir < 8; dir++) {  // Check in all 8 directions
        if (search_direction(grid, rows, cols, x, y, word, dir)) {
          ++found;
        }
      }
    }
  }
}

static size_t line_length(const char* string) {
  assert(string);
  size_t num = 0;
  while (*string && *string != '\n') {
    ++string;
    ++num;
  }
  return num;
}

int main(void) {
  size_t file_size = 0;
  char* input_file_path =
      djc_get_input_file("\\resources\\day_four\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  int rows = (int)line_length(file);
  int cols = 0;

  {
    char* current = file;

    while (*current && *current != '\r') {
      ++current;
      ++cols;
    }
  }

  char** array = malloc((size_t)rows * sizeof(char*));
  for (int i = 0; i < rows; i++) {
    array[i] = malloc((size_t)cols * sizeof(char));
    if (array[i] == NULL) {
      return 1;
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      array[i][j] = file[i * (cols + 2) + j];
    }
  }

  find_word(array, (int)rows, (int)cols, "XMAS");
  printf("answer=%zu", found);
  return 0;
}
