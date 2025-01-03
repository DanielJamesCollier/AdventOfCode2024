#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>

#include "../utils/djc.h"

struct equation {
  s32* array;
  s32 count;
  s32 capacity;
  s32 answer;
};

struct dynamic_equation_array {
  struct equation* array;
  s32 count;
  s32 capacity;
};

#define PUSH_BACK(equation_ptr, value)                                         \
  do {                                                                         \
    if ((equation_ptr)->count >= (equation_ptr)->capacity) {                   \
      (equation_ptr)->capacity =                                               \
          (equation_ptr)->capacity ? (equation_ptr)->capacity * 2 : 4;         \
      (equation_ptr)->array = realloc((equation_ptr)->array,                   \
                                      (equation_ptr)->capacity * sizeof(s32)); \
      if (!(equation_ptr)->array) {                                            \
        fprintf(stderr, "Memory allocation failed!\n");                        \
        exit(EXIT_FAILURE);                                                    \
      }                                                                        \
    }                                                                          \
    (equation_ptr)->array[(equation_ptr)->count++] = (value);                  \
  } while (0)

#define APPEND_EQUATION(dynamic_array_ptr, equation_ptr)                      \
  do {                                                                        \
    if ((dynamic_array_ptr)->count >= (dynamic_array_ptr)->capacity) {        \
      (dynamic_array_ptr)->capacity = (dynamic_array_ptr)->capacity           \
                                          ? (dynamic_array_ptr)->capacity * 2 \
                                          : 4;                                \
      (dynamic_array_ptr)->array =                                            \
          realloc((dynamic_array_ptr)->array,                                 \
                  (dynamic_array_ptr)->capacity * sizeof(struct equation));   \
      if (!(dynamic_array_ptr)->array) {                                      \
        fprintf(stderr, "Memory allocation failed!\n");                       \
        exit(EXIT_FAILURE);                                                   \
      }                                                                       \
    }                                                                         \
    (dynamic_array_ptr)->array[(dynamic_array_ptr)->count++] =                \
        *(equation_ptr);                                                      \
  } while (0)

internal void parse_file(char* file) {
  char* line = NULL;
  char* curr = NULL;
  char* end = NULL;
  struct equation eq = {.array = NULL, .count = 0, .capacity = 0, .answer = 0};
  struct dynamic_equation_array eq_array = {0};

  for (line = file; line; line = get_next_line(line)) {
    curr = line;
    eq.answer = atoi(line);

    while (*curr && *curr != ':') {
      ++curr;
    }
    ++curr;

    struct djc_atoi_result result;
    while ((result = djc_atoi(curr, &end)).success == DJC_ATOI_SUCCESS) {
      printf("Parsed number: %d\n", result.value);
      PUSH_BACK(&eq, result.value);
      APPEND_EQUATION(&eq_array, &eq);
      curr = end;
    }
    printf("answer=%d\n", eq.answer);
  }
}

s32 main(void) {
  u6 length = 0;
  char* input_file_path = djc_get_input_file("\\resources\\7\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &length);

  if (file == NULL) {
    puts("Failed to load file.");
    return 1;
  }

  parse_file(file);
  free(file);
  return 0;
}
