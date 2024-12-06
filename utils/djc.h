#ifndef DJC_H_
#define DJC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

#define internal static
#define persist static

inline char* djc_load_entire_file(char* filename, size_t* plen) {
  assert(filename);

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

inline size_t djc_count_lines_in_file(const char* lines) {
  assert(lines);

  size_t num_lines = 0;
  while (*lines++) {
    if (*lines == '\n' || *lines == '\0') {
      ++num_lines;
    }
  }
  return num_lines;
}

inline int djc_count_digits(int n) {
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

inline char* djc_get_input_file(const char* exe_relative_path) {
  assert(exe_relative_path);

  persist char filePath[MAX_PATH];  // MAX_PATH is more appropriate than 260

  // Get the full path of the executable
  if (GetModuleFileName(NULL, filePath, sizeof(filePath)) == 0) {
    return NULL;  // Return NULL if there's an error
  }

  // Find the last occurrence of the backslash ('\') in the path
  char* lastBackslash = strrchr(filePath, '\\');
  if (lastBackslash != NULL) {
    size_t inputFileLength =
        strlen(exe_relative_path) + 1;  // +1 for null-terminator

    // Ensure that we have enough space to copy the new path
    size_t offset = (size_t)(lastBackslash - filePath);  // Cast to size_t
    if (lastBackslash + 1 + inputFileLength <= filePath + sizeof(filePath)) {
      snprintf(lastBackslash + 1, sizeof(filePath) - offset - 1, "%s",
               exe_relative_path);
    } else {
      return NULL;  // Buffer overflow protection
    }
  }

  return filePath;  // Return the modified file path
}

//internal int* remove_at(int* input_array, size_t input_array_length, size_t index_to_remove, size_t* new_array_length) { 
//  size_t new_length = input_array_length - 1;
//
//  int* new_array = (int*)malloc(new_length * sizeof(int));
//  if (new_array == NULL) {
//    printf("malloc failed");
//    exit(EXIT_FAILURE);
//  }
//
//  if (index_to_remove > input_array_length) {
//    printf("index out of bounds in remove_at(). index=%zu", index_to_remove);
//    exit(EXIT_FAILURE);
//  }
// 
//  for (size_t i = 0, j = 0; i < input_array_length; i++) {
//      if (index_to_remove != i) {
//          new_array[j++] = input_array[i];
//      }
//  }
//
//  *new_array_length = new_length;
//  return new_array;
//}

#endif  // DJC_H_
