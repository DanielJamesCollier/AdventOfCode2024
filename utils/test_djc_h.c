#include "djc.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK_EQUAL(actual, expected)                                     \
  do {                                                                    \
    if ((actual) != (expected)) {                                         \
      printf("Assertion failed: file %s, line %d\n", __FILE__, __LINE__); \
      printf("  Actual: %d, Expected: %d\n", actual, expected);           \
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

s32 main(void) {
  test_djc_count_lines_in_file();
  puts("All tests passed!");
  return 0;
}
