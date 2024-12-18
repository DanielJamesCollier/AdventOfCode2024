#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../djc_arena/djc_arena.h"
#include "../utils/djc.h"

#define RULES_SIZE 100

typedef struct {
  s32* values;
  s32 count;
} Sequence;

typedef enum {
  SECTION_RULES = 1,
  SECTION_SEQUENCES = 2,
} Section;

internal void push_rule(s32 rules[RULES_SIZE][RULES_SIZE], s32 from, s32 to) {
  for (size_t i = 0; i < RULES_SIZE; i++) {
    if (rules[from][i] == -1) {
      rules[from][i] = to;
      break;
    }
  }
}

internal bool sequence_valid(Sequence seq, s32 rules[RULES_SIZE][RULES_SIZE]) {
  assert(seq.values);

  for (s32 i = 0; i < seq.count; i++) {
    for (s32 j = i + 1; j < seq.count; j++) {
      for (s32 k = 0; k < RULES_SIZE; k++) {
        s32 rule = rules[seq.values[i]][k];
        if (rule == -1) {
          return false;
        }
        if (seq.values[j] == rule) {
          break;
        }
      }
    }
  }
  return true;
}

// return -1 if the first argument is less than the second,
// return +1 if the first argument is greater than the second
// return  0 if the arguments are equivalent.
internal s32 compare_with_context(void* context, const void* a, const void* b) {
  int(*rules)[RULES_SIZE] = context;

  for (s32 i = 0; i < RULES_SIZE; i++) {
    s32 rule = rules[*(const s32*)a][i];
    if (rule == -1) {
      break;
    }

    if (*(const s32*)b == rule) {
      return 1;
    }
  }

  return -1;
}

s32 main(void) {
  struct Arena* mem = arena_create("General", 80);

  size_t length = 0;
  char* input_file_path =
      djc_get_input_file("\\resources\\day_five\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &length);
  djc_convert_crlf_to_lf(file);

  if (file == NULL) {
    fprintf(stderr, "Failed to load file\n");
    return 1;
  }

  s32 rules[RULES_SIZE][RULES_SIZE];
  memset(rules, -1, sizeof(rules));

  s32 section = SECTION_RULES;
  size_t accum = 0;

  for (char* line = file; line; line = get_next_line(line)) {
    size_t line_length = djc_line_length(line);

    if (line_length == 1) {
      section = SECTION_SEQUENCES;
      continue;
    }

    if (section == SECTION_RULES) {
      s32 from, to;
      if (sscanf(line, "%d|%d", &from, &to) == 2) {
        push_rule(rules, from, to);
      }
    } else if (section == SECTION_SEQUENCES) {
      Sequence seq = {
          .values = mem->current,
          .count = 0,
      };

      while (*line && *line != '\n') {
        s32 value = atoi(line);
        arena_alloc(mem, sizeof(int), 1);
        seq.values[seq.count++] = value;
        s32 digits = djc_count_digits(value);
        line += digits;

        if (*line && *line == ',')
          line++;
      }
      if (!sequence_valid(seq, rules)) {
        qsort_s(seq.values, seq.count, sizeof(int), compare_with_context,
                rules);
        accum += (size_t)seq.values[seq.count / 2];
      }
    }
  }

  printf("Answer (d5p1) = %zu\n", accum);
  arena_free(mem);
  free(file);
  return 0;
}
