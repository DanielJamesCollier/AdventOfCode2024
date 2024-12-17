#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

internal int compare(const void* a, const void* b) {
  return (*(const int*)a - *(const int*)b);
}

int main(void) {
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("resources\\day_one\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  size_t num_lines = djc_count_lines_in_file(file);
  int* first_col = (int*)malloc(num_lines * sizeof(int));
  int* second_col = (int*)malloc(num_lines * sizeof(int));

  char* current = file;
  size_t i = 0;
  while (i < num_lines) {
    first_col[i] = atoi(current);

    while (*current != ' ')
      current++;

    second_col[i] = atoi(current);

    while (*current != '\n')
      current++;

    current++;
    i++;
  }

  // sort the aray so we can early out. See "Fast early out"
  qsort(second_col, num_lines, sizeof(int), compare);

  for (i = 0; i < num_lines; i++) {
    int left_current = first_col[i];
    int count = 0;
    for (size_t j = 0; j < num_lines; j++) {
      int right_current = second_col[j];

      // Fast early out
      if (right_current > left_current) {
        break;
      } else if (second_col[j] == left_current) {
        ++count;
      }
    }
    first_col[i] *= count;
  }

  int total = 0;
  for (i = 0; i < num_lines; i++) {
    total += first_col[i];
  }

  printf("Answer (d1p2) = %u\n", total);

  free(first_col);
  free(second_col);
  free(file);

  return 0;
}
