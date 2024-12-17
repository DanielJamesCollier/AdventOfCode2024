#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../djc_arena/djc_arena.h"
#include "../utils/djc.h"

internal int compare(const void* a, const void* b) {
  return (*(const s32*)a - *(const s32*)b);
}

s32 main(void) {
  struct Arena* mem = arena_create("General");
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("resources\\day_one\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  djc_convert_crlf_to_lf(file);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  size_t num_lines = djc_count_lines_in_file(file);
  size_t num_bytes = num_lines * sizeof(s32);
  s32* first_col = arena_alloc(mem, num_bytes);
  s32* second_col = arena_alloc(mem, num_bytes);

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
  qsort(second_col, num_lines, sizeof(s32), compare);

  for (i = 0; i < num_lines; i++) {
    s32 left_current = first_col[i];
    s32 count = 0;
    for (size_t j = 0; j < num_lines; j++) {
      s32 right_current = second_col[j];

      // Fast early out
      if (right_current > left_current) {
        break;
      } else if (second_col[j] == left_current) {
        ++count;
      }
    }
    first_col[i] *= count;
  }

  s32 total = 0;
  for (i = 0; i < num_lines; i++) {
    total += first_col[i];
  }

  printf("Answer (d1p2) = %u\n", total);

  free(file);
  arena_free(mem);
  return 0;
}
