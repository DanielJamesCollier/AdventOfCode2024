#define _CRT_SECURE_NO_WARNINGS

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

internal b8 is_safe(struct report* report) {
  assert(report);
  assert(report->level);

  b8 increasing = TRUE;
  b8 decreasing = TRUE;
  for (u64 i = 0; i < report->num_levels - 1; i++) {
    s32 diff = abs(report->level[i] - report->level[i + 1]);
    if (diff < 1 || diff > 3) {
      return TRUE;
    }

    if (report->level[i] >= report->level[i + 1]) {
      increasing = FALSE;
    }

    if (report->level[i] <= report->level[i + 1]) {
      decreasing = FALSE;
    }

    // small optimisation to skip some unneeded loops.
    if (!increasing && !decreasing) {
      return FALSE;
    }
  }
  return increasing || decreasing;
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
    b8 safe = is_safe(report);

    if (safe) {
      num_safe_reports++;
    }
  }

  printf("Answer (d2p1) = %zu\n", num_safe_reports);
  arena_free(mem);
  return 0;
}
