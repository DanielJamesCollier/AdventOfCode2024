
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
        char* local = ++current;
        if (*local && *local == 'o') {
          local++;
          if (*local && *local == '(') {
            local++;
            if (*local && *local == ')') {
              local++;
              enable_muls = true;
              current = local;
            }
          } else if (*local && *local == 'n') {
            local++;
            if (*local && *local == '\'') {
              local++;
              if (*local && *local == 't') {
                local++;
                if (*local && *local == '(') {
                  local++;
                  if (*local && *local == ')') {
                    local++;
                    enable_muls = false;
                    current = local;
                  }
                }
              }
            }
          }
        }
      } break;

      case 'm': {
        if (enable_muls) {
          char* local = ++current;
          if (*local && *local == 'u') {
            local++;
            if (*local && *local == 'l') {
              local++;
              if (*local && *local == '(') {
                local++;
                if (*local != '\0') {
                  char* end = NULL;
                  long int num_one = strtol(local, &end, 10);
                  if (*end != ',')
                    continue;

                  end++;
                  local = end;

                  long int num_two = strtol(local, &end, 10);
                  if (*end == ')') {
                      printf("mul(%ld,%ld)\n", num_one, num_two);
                    accumulator += num_one * num_two;
                  }
                  local = end;
                  current = local;
                }
              }
            }
          }
        }

      } break;
    }
    current++;
  }

  printf("answer=%ld", accumulator);

  return 0;
}
