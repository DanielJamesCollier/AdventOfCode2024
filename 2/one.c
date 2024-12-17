#define _CRT_SECURE_NO_WARNINGS

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

internal bool is_safe(struct report* report) {
  assert(report);
  assert(report->level);

  bool increasing = true;
  bool decreasing = true;
  for (size_t i = 0; i < report->num_levels - 1; i++) {
    s32 diff = abs(report->level[i] - report->level[i + 1]);
    if (diff < 1 || diff > 3) {
      return false;
    }

    if (report->level[i] >= report->level[i + 1]) {
      increasing = false;
    }

    if (report->level[i] <= report->level[i + 1]) {
      decreasing = false;
    }

    // small optimisation to skip some unneeded loops.
    if (!increasing && !decreasing) {
      return false;
    }
  }
  return increasing || decreasing;
}

s32 main(void) {
  struct Arena* mem = arena_create("General", 80);
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\day_two\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);
  djc_convert_crlf_to_lf(file);

  struct reports reports_list;
  reports_list.num_reports = djc_count_lines_in_file(file);
  reports_list.rep = (struct report*)arena_alloc(
      mem, reports_list.num_reports * sizeof(struct report));

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

      arena_alloc(mem, sizeof(int));
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
    bool safe = is_safe(report);

    if (safe) {
      num_safe_reports++;
    }
  }

  printf("Answer (d2p1) = %zu\n", num_safe_reports);
  arena_free(mem);
  return 0;
}
