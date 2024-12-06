#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>  // For GetModuleFileName.
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

static void push_level(struct report* report, int level) {
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
    size_t j_bounds = reports_list.rep[i].num_levels;

    bool all_increasing = true;
    for (size_t j = 0; j < j_bounds - 1; j++) {
      if (reports_list.rep[i].level[j] > reports_list.rep[i].level[j + 1]) {
        all_increasing = false;
        break;
      }
    }

    bool all_decreasing = true;
    for (size_t j = 0; j < j_bounds - 1; j++) {
      if (reports_list.rep[i].level[j] < reports_list.rep[i].level[j + 1]) {
        all_decreasing = false;
        break;
      }
    }

    // Any two adjacent levels should differ by at least one and at most three.
    bool difference_ok = true;
    for (size_t j = 0; j < j_bounds - 1; j++) {
      int diff =
          abs(reports_list.rep[i].level[j] - reports_list.rep[i].level[j + 1]);
      if (diff == 0) {
        difference_ok = false;
        break;
      }

      if (diff > 3) {
        difference_ok = false;
        break;
      }
    }

    bool safe = (all_decreasing || all_increasing) && difference_ok;

    if (safe) {
      ++num_safe_reports;
    }
  }

  printf("num_safe_reports=%zu", num_safe_reports);

  return 0;
}
