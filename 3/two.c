#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "../utils/djc.h"

internal const char begin_muls[] = "do()";
internal const char end_muls[] = "don't()";
internal const char mul[] = "mul(";

internal s64 solve(char* current) {
  char* end = NULL;
  s64 accumulator = 0;
  struct djc_atoi_result num_one;
  struct djc_atoi_result num_two;
  b8 enable_muls = TRUE;

  while (*current) {
    if (djc_strncmp(current, begin_muls, sizeof(begin_muls) - 1) == 0) {
      enable_muls = TRUE;
      current += sizeof(begin_muls);
    } else if (djc_strncmp(current, end_muls, sizeof(end_muls) - 1) == 0) {
      enable_muls = FALSE;
      current += sizeof(end_muls);
    } else if (djc_strncmp(current, mul, sizeof(mul) - 1) == 0) {
      current += sizeof(mul);
      end = NULL;
      num_one = djc_atoi(current, &end);
      if (num_one.success != DJC_ATOI_SUCCESS) {
        exit(EXIT_FAILURE);
      }
      if (*end != ',') {
        continue;
      }
      current = ++end;
      num_two = djc_atoi(current, &end);
      if (num_two.success != DJC_ATOI_SUCCESS) {
        exit(EXIT_FAILURE);
      }
      if (*end == ')') {
        accumulator += num_one.value * num_two.value;
      }
      current = end;
    }
    ++current;
  }
  return (accumulator);
}

s32 main(void) {
  u64 file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\3\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return (0);
  }
  printf("Answer (d3p2) = %lld\n", solve(file));
  return (0);
}
