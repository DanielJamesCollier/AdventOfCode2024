#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

s32 main(void) {
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\3\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  djc_convert_crlf_to_lf(file);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  char* current = file;
  s64 accumulator = 0;

  while (*current) {
    while (*current && *current != 'm')
      current++;

    if (*current == '\0') {
      break;
    }

    current++;

    if (*current == '\0') {
      break;
    }

    if (*current == 'u') {
      current++;
      if (*current == 'l') {
        current++;
        if (*current == '(') {
          current++;
          if (*current == '\0') {
            break;
          }

          char* end = NULL;
          s64 num_one = strtol(current, &end, 10);
          if (*end != ',')
            continue;

          end++;
          current = end;

          long int num_two = strtol(current, &end, 10);
          if (*end == ')') {
            accumulator += num_one * num_two;
          }
          current = end++;
        }
      }
    }
    ++current;
  }
  printf("Answer (d3p1) = %I64d\n", accumulator);

  return 0;
}
