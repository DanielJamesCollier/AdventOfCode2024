#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>  // For GetModuleFileName.
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
    const char* inputFile = "\\resources\\day_one\\input.txt";
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

static int compare(const void* a, const void* b) {
  return (*(const int*)a - *(const int*)b);
}

int main(void) {
  size_t file_size = 0;
  char* input_file_path = get_input_file();
  char* file = load_entire_file(input_file_path, &file_size);

  if (file == NULL) {
    printf("Failed to load the file input.txt: %s", input_file_path);
    return 0;
  }

  free(first_col);
  free(second_col);
  free(file);

  return 0;
}
