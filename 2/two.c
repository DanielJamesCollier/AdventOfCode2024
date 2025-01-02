#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../djc_arena/djc_arena.h"
#include "../utils/djc.h"

struct report {
  s32* level;
  size_t num_levels;
};

struct reports {
  struct report* rep;
  size_t num_reports;
};

internal void push_level(struct report* report, s32 level) {
  assert(report);

  size_t new_num_levels = report->num_levels + 1;
  int* new_level =
      (int*)realloc(report->level, new_num_levels * sizeof(report->level));

  if (new_level == NULL) {
    exit(EXIT_FAILURE);
  }

  report->level = new_level;
  report->level[report->num_levels] = level;
  report->num_levels = new_num_levels;
}

internal bool is_safe(s32* levels, size_t num_levels) {
  bool increasing = true;
  bool decreasing = true;
  for (size_t i = 0; i < num_levels - 1; i++) {
    s32 diff = abs(levels[i] - levels[i + 1]);
    if (diff < 1 || diff > 3) {
      return false;
    }

    if (levels[i] >= levels[i + 1]) {
      increasing = false;
    }

    if (levels[i] <= levels[i + 1]) {
      decreasing = false;
    }

    // small optimisation to skip some unneeded loops.
    if (!increasing && !decreasing) {
      return false;
    }
  }
  return increasing || decreasing;
}

internal s32* remove_at(int* input_array,
                        size_t input_array_length,
                        size_t index_to_remove,
                        size_t* new_array_length) {
  size_t new_length = input_array_length - 1;

  int* new_array = (int*)malloc(new_length * sizeof(int));
  DJC_MALLOC_CHECK(new_array);

  if (index_to_remove > input_array_length) {
    printf("index out of bounds in remove_at(). index=%zu", index_to_remove);
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0, j = 0; i < input_array_length; i++) {
    if (index_to_remove != i) {
      new_array[j++] = input_array[i];
    }
  }

  *new_array_length = new_length;
  return new_array;
}

internal bool check(struct report* report) {
  assert(report);
  assert(report->level);

  if (is_safe(report->level, report->num_levels)) {
    return true;
  }

  for (size_t i = 0; i < report->num_levels; i++) {
    size_t new_array_length = 0;
    int* new_array =
        remove_at(report->level, report->num_levels, i, &new_array_length);
    bool now_safe = is_safe(new_array, new_array_length);
    free(new_array);
    if (now_safe) {
      return true;
    }
  }
  return false;
}

s32 main(void) {
  struct Arena* mem = arena_create("General", 80);
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\2\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  djc_convert_crlf_to_lf(file);

  struct reports reports_list;
  reports_list.num_reports = djc_count_lines_in_file(file);
  reports_list.rep = (struct report*)arena_alloc(mem, sizeof(struct report),
                                                 reports_list.num_reports);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  size_t i = 0;
  for (char* line = file; line; line = get_next_line(line)) {
    struct report* report = &reports_list.rep[i];
    report->level = mem->current;
    while (*line && *line != '\n') {
      s32 current_level = atoi(line);
      s32 digits = djc_count_digits(current_level);

      arena_alloc(mem, sizeof(int), 1);
      report->level[report->num_levels++] = current_level;

      line += digits;
      if (*line == ' ') {
        line++;
      }
    }
    i++;
  }

  size_t num_safe_reports = 0;

  for (i = 0; i < reports_list.num_reports; i++) {
    struct report* report = &reports_list.rep[i];
    bool safe = check(report);

    if (safe) {
      num_safe_reports++;
    }
  }

  printf("Answer (d2p2) = %zu\n", num_safe_reports);

  return 0;
}
