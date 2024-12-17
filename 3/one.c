#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/djc.h"

int main(void) {
  size_t file_size = 0;
  char* input_file_path =
      djc_get_input_file("\\resources\\day_three\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  char* current = file;
  long int accumulator = 0;

  while (*current) {
    while (*current && *current != 'm')
      current++;

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
          long int num_one = strtol(current, &end, 10);
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
  printf("Answer (d3p1) = %ld\n", accumulator);

  return 0;
}
