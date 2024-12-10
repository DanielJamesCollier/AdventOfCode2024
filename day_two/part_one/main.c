#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../../utils/djc.h"

struct report {
  int* level;
  size_t num_levels;
};

struct reports {
  struct report* rep;
  size_t num_reports;
};

internal void push_level(struct report* report, int level) {
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

internal bool is_safe(int* levels, size_t num_levels) {
  assert(levels);

  bool increasing = true;
  bool decreasing = true;
  for (size_t i = 0; i < num_levels - 1; i++) {
    int diff = abs(levels[i] - levels[i + 1]);
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

int main(void) {
  size_t file_size = 0;
  char* input_file_path = djc_get_input_file("\\resources\\day_two\\input.txt");
  char* file = djc_load_entire_file(input_file_path, &file_size);

  struct reports reports_list;
  reports_list.num_reports = djc_count_lines_in_file(file);
  reports_list.rep =
      (struct report*)calloc(reports_list.num_reports, sizeof(struct report));

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  char* current = file;
  size_t i = 0;

  while (*current != '\0') {
    int current_level = atoi(current);
    push_level(&reports_list.rep[i], current_level);
    int digits = djc_count_digits(current_level);
    current += digits;

    if (*current == ' ') {
      current++;
    }

    if (*current == '\r') {
      current++;
    }

    if (*current == '\n') {
      current++;
      i++;
    }
  }

  size_t num_safe_reports = 0;

  for (i = 0; i < reports_list.num_reports; i++) {
    bool safe =
        is_safe(reports_list.rep[i].level, reports_list.rep[i].num_levels);

    if (safe) {
      num_safe_reports++;
    }
  }

  printf("Answer (d2p1) = %zu\n", num_safe_reports);

  return 0;
}
