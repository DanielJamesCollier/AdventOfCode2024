#ifndef DJC_H_
#define DJC_H_

#include <Windows.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double f64;
#define internal static
#define persist static
#define merge inline
#define fold __forceinline

merge char* djc_load_entire_file(char* filename, size_t* plen) {
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

merge size_t djc_count_lines_in_file(const char* lines) {
  assert(lines);

  size_t num_lines = 0;
  while (*lines) {
    if (*lines == '\n' && *(lines + 1) != '\0') {
      num_lines++;
    }
    lines++;
  }
  num_lines++;
  return num_lines;
}

merge size_t djc_strlen_until_newline(const char* string) {
  size_t count = 0;
  while (string[count] != '\0' && string[count] != '\n') {
    count++;
  }
  return count;
}

merge int djc_count_digits(int n) {
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

merge char* get_next_line(char* string) {
  if (string == NULL) {
    return NULL;
  }

  while (*string && *string != '\n')
    ++string;

  if (*string == '\n')
    ++string;

  return *string ? string : NULL;
}

merge size_t djc_line_length(char* string) {
  if (string == NULL) {
    return 0;
  }

  size_t length = 0;

  while (*string && *string != '\n') {
    ++length;
    ++string;
  }

  if (*string == '\n')
    ++length;

  return length;
}

merge char* djc_get_input_file(const char* exe_relative_path) {
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
    size_t offset = (size_t)(lastBackslash - filePath);
    if (lastBackslash + 1 + inputFileLength <= filePath + sizeof(filePath)) {
      snprintf(lastBackslash + 1, sizeof(filePath) - offset - 1, "%s",
               exe_relative_path);
    } else {
      return NULL;
    }
  }

  return filePath;  // Return the modified file path
}

// From:
//   The quick brown fox\r\n
// To:
//   The quick brown fox\n\0
merge void djc_convert_crlf_to_lf(char* string) {
  if (string == NULL) {
    return;
  }

  char* read = string;
  char* write = string;

  while (*read) {
    if (*read == '\r' && *(read + 1) == '\n') {
      read++;
      continue;
    } else {
      *write = *read;
      write++;
    }
    read++;
  }

  *write = '\0';
}

// From:
//   The quick brown fox\n\0
// To:
//   The quick brown fox\0
merge void djc_strip_lf(char* string) {
  if (string == NULL) {
    return;
  }

  char* read = string;
  char* write = string;

  while (*read) {
    if (*(read) == '\n') {
      read++;
      continue;
    } else {
      *write = *read;
      write++;
    }
    read++;
  }

  *write = '\0';
}

// From:
//   The quick brown fox\r\n\0
// To:
//   The quick brown fox\0
merge void djc_strip_crlf(char* string) {
  if (string == NULL) {
    return;
  }

  char* read = string;
  char* write = string;

  while (*read) {
    if (*read == '\r' && *(read + 1) == '\n') {
      read += 2;
      continue;
    } else {
      *write = *read;
      write++;
    }
    read++;
  }

  *write = '\0';
}

merge bool djc_is_space(char c) {
  return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
         c == '\v';
}

merge bool djc_is_digit(char c) {
  return c >= '0' && c <= '9';
}

// End returns the character after the last digit.
merge s32 djc_atoi(const char* string, const char** end) {
  assert(string);

  bool minus = false;
  s32 result = 0;

  while (djc_is_space(*string))
    string++;

  if (*string == '+')
    ++string;
  else if (*string == '-') {
    ++string;
    minus = true;
  }

  while (djc_is_digit(*string)) {
    s32 digit = *string - '0';
    result *= 10;
    result -= digit;  // Support INT_MIN.
    ++string;
  }

  if (end) {
    *end = string;
  }

  return minus ? result : -result;
}

#endif  // DJC_H_
