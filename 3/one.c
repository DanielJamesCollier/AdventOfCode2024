#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "../utils/djc.h"

internal const char mul[] = "mul(";

internal s64 solve(char* current) {
  char* end = NULL;
  u64 i = 0;
  s64 num_one = 0;
  s64 num_two = 0;
  s64 accumulator = 0;

  while (*current) {
    i = 0;
    while (mul[i] != '\0') {
      if (*current == mul[i]) {
        ++current;
        ++i;
      } else {
        break;
      }
    }
    if (i == 4) {
      end = NULL;
      num_one = strtol(current, &end, 10);
      if (*end != ',') {
        continue;
      }
      current = ++end;
      num_two = strtol(current, &end, 10);
      if (*end == ')') {
        accumulator += num_one * num_two;
      }
      current = end;
    } else {
      ++current;
    }
  }
  return (accumulator);
}

s32 main(void) {
  u64 file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\3\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  djc_convert_crlf_to_lf(file);
  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return (0);
  }
  printf("Answer (d3p1) = %I64d\n", solve(file));
  return (0);
}
