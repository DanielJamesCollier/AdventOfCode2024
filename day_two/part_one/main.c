#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>  // For GetModuleFileName.
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* load_entire_file(char* filename, size_t* plen) {
  char* text = NULL;
  size_t len = 0;
  FILE* f = fopen(filename, "rb");

  if (f == 0)
    return NULL;

  fseek(f, 0, SEEK_END);
  len = (size_t)ftell(f);

  if (plen)
    *plen = len;

  text = (char*)malloc(len + 1);

  if (text == NULL)
    return NULL;

  fseek(f, 0, SEEK_SET);
  fread(text, 1, len, f);
  fclose(f);
  text[len] = '\0';

  return text;
}

static char* get_input_file(void) {
  static char filePath[MAX_PATH];  // MAX_PATH is more appropriate than 260

  // Get the full path of the executable
  if (GetModuleFileName(NULL, filePath, sizeof(filePath)) == 0) {
    return NULL;  // Return NULL if there's an error
  }

  // Find the last occurrence of the backslash ('\') in the path
  char* lastBackslash = strrchr(filePath, '\\');
  if (lastBackslash != NULL) {
    const char* inputFile = "\\resources\\day_two\\input.txt";
    size_t inputFileLength = strlen(inputFile) + 1;  // +1 for null-terminator

    // Ensure that we have enough space to copy the new path
    size_t offset = (size_t)(lastBackslash - filePath);  // Cast to size_t
    if (lastBackslash + 1 + inputFileLength <= filePath + sizeof(filePath)) {
      snprintf(lastBackslash + 1, sizeof(filePath) - offset - 1, "%s",
               inputFile);
    } else {
      return NULL;  // Buffer overflow protection
    }
  }

  return filePath;  // Return the modified file path
}

static size_t count_lines_in_file(const char* lines) {
  size_t num_lines = 0;
  while (*lines++) {
    if (*lines == '\n' || *lines == '\0') {
      ++num_lines;
    }
  }
  return num_lines;
}

static int count_digits(int n) {
  int count = 0;

  if (n == 0) {
    return 1;
  }

  if (n < 0) {
    n = -n;
  }

  while (n > 0) {
    n /= 10;
    count++;
  }

  return count;
}

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
  char* input_file_path = get_input_file();
  char* file = load_entire_file(input_file_path, &file_size);

  struct reports reports_list;
  reports_list.num_reports = count_lines_in_file(file);
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
    int digits = count_digits(current_level);
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

    bool all_decreasing = true;
    for (size_t j = 0; j < j_bounds - 1; j++) {
      if (reports_list.rep[i].level[j] > reports_list.rep[i].level[j + 1]) {
        all_decreasing = false;
        break;
      }
    }

    bool all_increasing = true;
    for (size_t j = 0; j < j_bounds - 1; j++) {
      if (reports_list.rep[i].level[j] < reports_list.rep[i].level[j + 1]) {
        all_increasing = false;
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
