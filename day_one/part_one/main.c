#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>  // For GetModuleFileName.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/djc.h"

internal int compare(const void* a, const void* b) {
  return (*(const int*)a - *(const int*)b);
}

int main(void) {
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\day_one\\input.txt");
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

  qsort(first_col, num_lines, sizeof(int), compare);
  qsort(second_col, num_lines, sizeof(int), compare);

  int total_distance = 0;

  for (i = 0; i < num_lines; i++) {
    total_distance += abs(first_col[i] - second_col[i]);
  }

  printf("day one part one = %u\n", total_distance);
  fflush(stdout);  // Will now print everything in the stdout buffer

  free(first_col);
  free(second_col);
  free(file);

  return 0;
}
