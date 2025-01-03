#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "../djc_arena/djc_arena.h"
#include "../utils/djc.h"

struct report {
  s32* level;
  u64 num_levels;
};

struct reports {
  struct report* rep;
  u64 num_reports;
};

internal void push_level(struct report* report, s32 level) {
  assert(report);

  u64 new_num_levels = report->num_levels + 1;
  int* new_level =
      (int*)realloc(report->level, new_num_levels * sizeof(report->level));

  if (new_level == NULL) {
    exit(EXIT_FAILURE);
  }

  report->level = new_level;
  report->level[report->num_levels] = level;
  report->num_levels = new_num_levels;
}

internal b8 is_safe(s32* levels, u64 num_levels) {
  b8 increasing = TRUE;
  b8 decreasing = TRUE;
  for (u64 i = 0; i < num_levels - 1; i++) {
    s32 diff = abs(levels[i] - levels[i + 1]);
    if (diff < 1 || diff > 3) {
      return FALSE;
    }

    if (levels[i] >= levels[i + 1]) {
      increasing = FALSE;
    }

    if (levels[i] <= levels[i + 1]) {
      decreasing = FALSE;
    }

    // small optimisation to skip some unneeded loops.
    if (!increasing && !decreasing) {
      return FALSE;
    }
  }
  return increasing || decreasing;
}

internal s32* remove_at(int* input_array,
                        u64 input_array_length,
                        u64 index_to_remove,
                        u64* new_array_length) {
  u64 new_length = input_array_length - 1;

  int* new_array = (int*)malloc(new_length * sizeof(int));
  DJC_MALLOC_CHECK(new_array);

  if (index_to_remove > input_array_length) {
    printf("index out of bounds in remove_at(). index=%zu", index_to_remove);
    exit(EXIT_FAILURE);
  }

  for (u64 i = 0, j = 0; i < input_array_length; i++) {
    if (index_to_remove != i) {
      new_array[j++] = input_array[i];
    }
  }

  *new_array_length = new_length;
  return new_array;
}

internal b8 check(struct report* report) {
  assert(report);
  assert(report->level);

  if (is_safe(report->level, report->num_levels)) {
    return TRUE;
  }

  for (u64 i = 0; i < report->num_levels; i++) {
    u64 new_array_length = 0;
    int* new_array =
        remove_at(report->level, report->num_levels, i, &new_array_length);
    b8 now_safe = is_safe(new_array, new_array_length);
    free(new_array);
    if (now_safe) {
      return TRUE;
    }
  }
  return FALSE;
}

s32 main(void) {
  struct Arena* mem = arena_create("General", 80);
  u64 file_size = 0;
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

  u64 i = 0;
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

  u64 num_safe_reports = 0;

  for (i = 0; i < reports_list.num_reports; i++) {
    struct report* report = &reports_list.rep[i];
    b8 safe = check(report);

    if (safe) {
      num_safe_reports++;
    }
  }

  printf("Answer (d2p2) = %zu\n", num_safe_reports);

  return 0;
}
