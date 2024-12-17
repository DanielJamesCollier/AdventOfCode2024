#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../djc_arena/djc_arena.h"
#include "../utils/djc.h"

internal int compare(const void* a, const void* b) {
  return (*(const s32*)a - *(const s32*)b);
}

int main(void) {
  struct Arena* mem = arena_create("General", 80);
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\day_one\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  djc_convert_crlf_to_lf(file);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  size_t num_lines = djc_count_lines_in_file(file);
  s32* first_col = arena_alloc(mem, sizeof(s32), num_lines);
  s32* second_col = arena_alloc(mem, sizeof(s32), num_lines);

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

  qsort(first_col, num_lines, sizeof(s32), compare);
  qsort(second_col, num_lines, sizeof(s32), compare);

  s32 total_distance = 0;

  for (i = 0; i < num_lines; i++) {
    total_distance += abs(first_col[i] - second_col[i]);
  }

  printf("Answer (d1p1) = %u\n", total_distance);

  free(file);
  arena_free(mem);
  return 0;
}
