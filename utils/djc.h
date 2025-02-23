#ifndef DJC_H_
#define DJC_H_

#include <Windows.h>

#include <assert.h>
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
typedef int8_t b8;
typedef int16_t b16;
typedef int32_t b32;
typedef int64_t b64;
#define TRUE 1
#define FALSE 0

_Static_assert(sizeof(u8) == 1);
_Static_assert(sizeof(u16) == 2);
_Static_assert(sizeof(u32) == 4);
_Static_assert(sizeof(u64) == 8);
_Static_assert(sizeof(s8) == 1);
_Static_assert(sizeof(s16) == 2);
_Static_assert(sizeof(s32) == 4);
_Static_assert(sizeof(s64) == 8);
_Static_assert(sizeof(f32) == 4);
_Static_assert(sizeof(f64) == 8);
_Static_assert(sizeof(b8) == 1);
_Static_assert(sizeof(b16) == 2);
_Static_assert(sizeof(b32) == 4);
_Static_assert(sizeof(b64) == 8);
_Static_assert(sizeof(size_t) == 8);  // Use s64 in for loops.

#define internal static
#define persist static
#define merge inline
#define fold __forceinline

#define DJC_MALLOC_CHECK(memory)       \
  if (memory == NULL) {                \
    fprintf(stderr, "malloc failed."); \
    exit(EXIT_FAILURE);                \
  }

merge char* djc_load_entire_file(char* filename, u64* plen) {
  assert(filename);

  char* text = NULL;
  u64 len = 0;
  FILE* f = fopen(filename, "rb");

  if (f == 0)
    return NULL;

  fseek(f, 0, SEEK_END);
  len = (u64)ftell(f);

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

merge u64 djc_count_lines_in_file(const char* lines) {
  assert(lines);

  u64 num_lines = 0;
  while (*lines) {
    if (*lines == '\n' && *(lines + 1) != '\0') {
      num_lines++;
    }
    lines++;
  }
  num_lines++;
  return num_lines;
}

merge u64 djc_strlen_until_newline(const char* string) {
  u64 count = 0;
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

merge u64 djc_line_length(char* string) {
  if (string == NULL) {
    return 0;
  }

  u64 length = 0;

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
    u64 inputFileLength =
        strlen(exe_relative_path) + 1;  // +1 for null-terminator

    // Ensure that we have enough space to copy the new path
    u64 offset = (u64)(lastBackslash - filePath);
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

merge b8 djc_is_whitespace(char c) {
  return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' ||
         c == '\v';
}

merge b8 djc_is_horizontal_space(char c) {
  return c == ' ' || c == '\t';
}

merge b8 djc_is_digit(char c) {
  return c >= '0' && c <= '9';
}

enum djc_atoi_error {
  DJC_ATOI_SUCCESS = 1,
  DJC_ATOI_EOL,
  DJC_ATOI_ERROR_INVALID_CHAR,
  DJC_ATOI_ERROR_RANGE
};

struct djc_atoi_result {
  s32 value;
  enum djc_atoi_error success;
};

merge struct djc_atoi_result djc_atoi(const char* restrict string,
                                      const char** restrict end) {
  assert(string);
  b8 minus = FALSE;
  s32 digit = 0;
  struct djc_atoi_result result = {.value = 0, .success = DJC_ATOI_SUCCESS};
  const char* begin = string;

  if (*string == '\0') {
    result.success = DJC_ATOI_EOL;
    if (end)
      *end = begin;
    return result;
  }

  while (djc_is_horizontal_space(*string))
    string++;

  if (djc_is_whitespace(*string)) {
    result.success = DJC_ATOI_EOL;
    if (end)
      *end = string;
    return result;
  }

  if (*string == '+') {
    ++string;
    // Error Check: check if the next character is a non-digit
    if (*string == '\0' || !djc_is_digit(*string)) {
      if (end)
        *end = begin;
      result.success = DJC_ATOI_ERROR_INVALID_CHAR;
      return result;
    }
  } else if (*string == '-') {
    ++string;
    minus = TRUE;

    // Error Check: check if the next character is a non-digit
    if (*string == '\0' || !djc_is_digit(*string)) {
      if (end)
        *end = begin;
      result.success = DJC_ATOI_ERROR_INVALID_CHAR;
      return result;
    }
  }

  while (djc_is_digit(*string)) {
    digit = *string - '0';
    if (minus) {
      if (result.value < (INT_MIN + digit) / 10) {
        result.success = DJC_ATOI_ERROR_RANGE;
        result.value = INT_MIN;
        if (end) {
          *end = string;
        }
        return result;
      }
      result.value = result.value * 10 - digit;
    } else {
      if (result.value > (INT_MAX - digit) / 10) {
        result.success = DJC_ATOI_ERROR_RANGE;
        result.value = INT_MAX;
        if (end) {
          *end = string;
        }
        return result;
      }
      result.value = result.value * 10 + digit;
    }
    ++string;
  }

  if (end) {
    *end = string;
  }

  return result;
}

merge s32 djc_strncmp(const char* restrict s1, const char* restrict s2, s64 n) {
  while (n && *s1 && (*s1 == *s2)) {
    ++s1;
    ++s2;
    --n;
  }

  if (n == 0) {
    return 0;
  } else {
    // 7.21.4/1 (C99)
    // The sign of a nonzero value returned by the comparison functions memcmp,
    // strcmp, and strncmp is determined by the sign of the difference between
    // the values of the first pair of characters (both interpreted as unsigned
    // char) that differ in the objects being compared.
    return (*(unsigned char*)s1 - *(unsigned char*)s2);
  }
}

#endif  // DJC_H_
