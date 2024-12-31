#include "djc.h"

#include <stdio.h>
#include <stdlib.h>

#define SELECT_FORMAT(type)      \
  _Generic((type),               \
      int: "%d",                 \
      unsigned: "%u",            \
      enum djc_atoi_error: "%d", \
      default: "%p")

#define CHECK_EQUAL(actual, expected)                                     \
  do {                                                                    \
    if ((actual) != (expected)) {                                         \
      printf("Assertion failed: file %s, line %d\n", __FILE__, __LINE__); \
      printf("  Actual: ");                                               \
      printf(SELECT_FORMAT(actual), (actual));                            \
      printf(", Expected: ");                                             \
      printf(SELECT_FORMAT(expected), (expected));                        \
      printf("\n");                                                       \
      exit(EXIT_FAILURE);                                                 \
    }                                                                     \
  } while (0)

internal void test_djc_count_lines_in_file() {
  {
    static char* a = "test\0";
    CHECK_EQUAL((int)djc_count_lines_in_file(a), 1);
  }

  {
    static char* a = "test\n\0";
    CHECK_EQUAL((int)djc_count_lines_in_file(a), 1);
  }

  {
    static char* a =
        "test\n"
        "test\n\0";
    CHECK_EQUAL((int)djc_count_lines_in_file(a), 2);
  }

  {
    static char* a =
        "test\n"
        "test\0";
    CHECK_EQUAL((int)djc_count_lines_in_file(a), 2);
  }
}

size_t index_after_char(const char* str, s32 c) {
  assert(str);
  size_t i = 0;
  size_t len = strlen(str);

  for (i = 0; i < len; i++) {
    if (str[i] == c) {
      break;
    }
  }
  if (i == len) {
    puts("Error: char not found");
    exit(1);
  }
  return i + 1;
}

internal void test_djc_atoi() {
  {
    char* a = "1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "10";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 10);
    CHECK_EQUAL(end, a + index_after_char(a, '0'));
  }
  {
    char* a = "01";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "1a";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "-2147483648";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, INT_MIN);
    CHECK_EQUAL(end, a + 11);
  }
  {
    char* a = "2147483647";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, INT_MAX);
    CHECK_EQUAL(end, a + 10);
  }
  {
    char* a = "2147483648";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_RANGE);
    CHECK_EQUAL(result.value, INT_MAX);
    CHECK_EQUAL(end, a + 9);
  }
  {
    char* a = "-2147483649";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_RANGE);
    CHECK_EQUAL(result.value, INT_MIN);
    CHECK_EQUAL(end, a + 10);
  }
  {
    char* a = "+1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "-1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, -1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_EOL);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "\0";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_EOL);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "\n";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_EOL);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "1\n";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "\0 1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_EOL);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "\t\t  1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "   1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_SUCCESS);
    CHECK_EQUAL(result.value, 1);
    CHECK_EQUAL(end, a + index_after_char(a, '1'));
  }
  {
    char* a = "+ 1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "- 1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "--1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "++1";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "+";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
  {
    char* a = "-";
    char* end = NULL;
    struct djc_atoi_result result = djc_atoi(a, &end);

    CHECK_EQUAL(result.success, DJC_ATOI_ERROR_INVALID_CHAR);
    CHECK_EQUAL(result.value, 0);
    CHECK_EQUAL(end, a);
  }
}

s32 main(void) {
  test_djc_count_lines_in_file();
  test_djc_atoi();
  puts("All tests passed!");
  return 0;
}
