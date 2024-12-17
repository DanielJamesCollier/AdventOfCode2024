#ifndef DJC_ARENA_H_
#define DJC_ARENA_H_

#include <stddef.h> // size_t
#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <excpt.h>

#define ARENA_PAGE_LIMIT 80

typedef struct Arena {
  char* name;
  void* base_address;
  void* current;
  size_t page_size;
  size_t pages_committed;
} Arena;


INT arena_page_fault_handler(Arena* arena, DWORD dwCode) {
  if (dwCode != EXCEPTION_ACCESS_VIOLATION) {
    printf("Exception code = %lu.\n", dwCode);
    return EXCEPTION_EXECUTE_HANDLER;
  }

  printf("Exception is a page fault.\n");

  if (arena->pages_committed >= ARENA_PAGE_LIMIT) {
    printf("Exception: out of pages.\n");
    return EXCEPTION_EXECUTE_HANDLER;
  }

  void* result = VirtualAlloc(arena->current, arena->page_size, MEM_COMMIT,
                              PAGE_READWRITE);

  if (result == NULL) {
    printf("VirtualAlloc failed.\n");
    return EXCEPTION_EXECUTE_HANDLER;
  } else {
    printf("Allocating another page.\n");
  }

  arena->pages_committed++;
  return EXCEPTION_CONTINUE_EXECUTION;
}

Arena* arena_create(char* name) {
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);

  Arena* arena = (Arena*)malloc(sizeof(Arena));
  arena->name = name;
  arena->page_size = sys_info.dwPageSize;
  arena->pages_committed = 0;
  arena->base_address = VirtualAlloc(NULL, ARENA_PAGE_LIMIT * arena->page_size,
                                    MEM_RESERVE, PAGE_NOACCESS);

  if (arena->base_address == NULL) {
      printf("Virtual alloc failed. calling exit(1)");
      exit(1);
  }

  arena->current = arena->base_address;
  return arena;
}

void* arena_alloc(Arena* arena, size_t size) {
  size_t offset = (size_t)((char*)arena->current - (char*)arena->base_address);

  if (offset + size > ARENA_PAGE_LIMIT * arena->page_size) {
    printf("%s allocation failed. Limit reached", arena->name);
    exit(1);
    return NULL;
  }

  void* allocated_memory = (char*)arena->current;

  __try {
    volatile char* test = (char*)allocated_memory;
    for (size_t i = 0; i < size; i++) {
      test[i] = 0;
    }
  } __except (arena_page_fault_handler(arena, GetExceptionCode())) {
    // This code is executed only if the filter function
    // is unsuccessful in committing the next page.
    printf("Exiting process.\n");
    ExitProcess(GetLastError());
    return NULL;
  }

  arena->current = (char*)arena->current + size;
  return allocated_memory;
}

void arena_free(Arena* arena) {
  if (arena->base_address) {
    VirtualFree(arena->base_address, 0, MEM_RELEASE);
  }
}

//Arena arena_create(char* name);
//void* arena_alloc(Arena* arena, size_t size);
//void arena_free(Arena* arena);

#endif  // DJC_ARENA_H_
