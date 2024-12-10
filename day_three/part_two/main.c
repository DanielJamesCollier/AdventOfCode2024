
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>  // For GetModuleFileName.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/djc.h"

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

  bool enable_muls = true;

  while (*current) {
    switch (*current) {
      case 'd': {
        ++current;
        if (*current && *current == 'o') {
          ++current;
          if (*current && *current == '(') {
            ++current;
            if (*current && *current == ')') {
              enable_muls = true;
            }
          } else if (*current && *current == 'n') {
            ++current;
            if (*current && *current == '\'') {
              ++current;
              if (*current && *current == 't') {
                ++current;
                if (*current && *current == '(') {
                  ++current;
                  if (*current && *current == ')') {
                    enable_muls = false;
                  }
                }
              }
            }
          }
        }
      } break;

      case 'm': {
        if (enable_muls) {
          ++current;
          if (*current && *current == 'u') {
            ++current;
            if (*current && *current == 'l') {
              ++current;
              if (*current && *current == '(') {
                ++current;
                if (*current != '\0') {
                  char* end = NULL;
                  long int num_one = strtol(current, &end, 10);

                  // e.g. mul(29* is not valid.
                  if (*end != ',')
                    continue;

                  current = end + 1;
                  long int num_two = strtol(current, &end, 10);

                  // e.g. mul(29,9\0 or mul(29,9adsjf are not valid.
                  if (*end == ')') {
                    accumulator += num_one * num_two;
                  }
                }
              }
            }
          }
        }

      } break;
    }

    if (*current != '\0') {
      current++;
    }
  }

  printf("Answer (d3p2) = %ld\n", accumulator);

  return 0;
}
