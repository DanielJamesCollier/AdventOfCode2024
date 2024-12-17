#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/djc.h"
#include "../utils/djc_arena.h"

#define RULES_SIZE 100

typedef struct {
  i32* values;
  i32 count;
} Sequence;

typedef enum {
  SECTION_RULES = 1,
  SECTION_SEQUENCES = 2,
} Section;

internal void push_rule(i32 rules[RULES_SIZE][RULES_SIZE], i32 from, i32 to) {
  for (size_t i = 0; i < RULES_SIZE; i++) {
    if (rules[from][i] == -1) {
      rules[from][i] = to;
      break;
    }
  }
}

internal bool sequence_valid(Sequence seq, i32 rules[RULES_SIZE][RULES_SIZE]) {
  assert(seq.values);
  
  for (i32 i = 0; i < seq.count; i++) {
    for (i32 j = i + 1; j < seq.count; j++) {
      for (i32 k = 0; k < RULES_SIZE; k++) {
        int rule = rules[seq.values[i]][k];
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

int main(void) {
  Arena* mem = arena_create("General");
  
  size_t length = 0;
  char* input_file_path = djc_get_input_file("\\resources\\day_five\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &length);
  if (file == NULL) {
    fprintf(stderr, "Failed to load file\n");
    return 1;
  }
  printf("File loaded successfully.\n");
  
  i32 rules[RULES_SIZE][RULES_SIZE];
  memset(rules, -1, sizeof(rules));
  
  i32 section = SECTION_RULES;
  size_t accum = 0;
  
  for (char *line = file; line; line = get_next_line(line)) {
    size_t line_length = djc_line_length(line);

    // Mac and Windows e.g. \r\n vs \n.
    if (line_length == 2 || line_length == 1) {
      section = SECTION_SEQUENCES;
      continue;
    }
    
    if (section == SECTION_RULES) {
      int from, to;
      if (sscanf(line, "%d|%d", &from, &to) == 2) {
        push_rule(rules, from, to);
      }
    } else if (section == SECTION_SEQUENCES) {
      Sequence seq = { .values=mem->current, .count = 0, };
      
      while (*line && *line != '\n') {
        int value = atoi(line);
        //printf("before arena alloc\n");
        arena_alloc(mem, sizeof(int));
        //printf("after arena alloc\n");
        seq.values[seq.count++] = value;
        int digits = djc_count_digits(value);
        line += digits;
        
        if (*line && *line == ',')
          line++;
      }
      if (sequence_valid(seq, rules)) {
        accum += (size_t)seq.values[seq.count / 2];
      }
    }
  }
  
  printf("answer: %zu\n", accum);
  arena_free(mem);
  free(file);
  return 0;
}
