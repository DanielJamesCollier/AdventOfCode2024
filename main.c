// A short program to demonstrate dynamic memory allocation
// using a structured exception handler.

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>             // For exit

#define PAGELIMIT 80            // Number of pages to ask for

LPTSTR lpNxtPage;               // Address of the next page to ask for
DWORD dwPages = 0;              // Count of pages gotten so far
DWORD dwPageSize;               // Page size on this computer

INT PageFaultExceptionFilter(DWORD dwCode)
{
    LPVOID lpvResult;

    // If the exception is not a page fault, exit.

    if (dwCode != EXCEPTION_ACCESS_VIOLATION)
    {
        printf("Exception code = %lu.\n", dwCode);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    printf("Exception is a page fault.\n");

    // If the reserved pages are used up, exit.

    if (dwPages >= PAGELIMIT)
    {
        printf("Exception: out of pages.\n");
        return EXCEPTION_EXECUTE_HANDLER;
    }

    // Otherwise, commit another page.

    lpvResult = VirtualAlloc(
        (LPVOID)lpNxtPage, // Next page to commit
        dwPageSize,         // Page size, in bytes
        MEM_COMMIT,         // Allocate a committed page
        PAGE_READWRITE);    // Read/write access
    if (lpvResult == NULL)
    {
        printf("VirtualAlloc failed.\n");
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        printf("Allocating another page.\n");
    }

    // Increment the page count, and advance lpNxtPage to the next page.

    dwPages++;
    lpNxtPage = (LPTSTR)((PCHAR)lpNxtPage + dwPageSize);

    // Continue execution where the page fault occurred.

    return EXCEPTION_CONTINUE_EXECUTION;
}

VOID ErrorExit(const char* string)
{
    printf("Error! %s with error code of %ld.\n", string, GetLastError());
    exit(0);
}

int main(void)
{
    LPVOID lpvBase;               // Base address of the test memory
    LPTSTR lpPtr;                 // Generic character pointer
    BOOL bSuccess;                // Flag
    DWORD i;                      // Generic counter
    SYSTEM_INFO sSysInfo;         // Useful information about the system

    GetSystemInfo(&sSysInfo);     // Initialize the structure.

    printf("This computer has page size %lu.\n", sSysInfo.dwPageSize);

    dwPageSize = sSysInfo.dwPageSize;

    // Reserve pages in the virtual address space of the process.

    lpvBase = VirtualAlloc(
        NULL,                 // System selects address
        PAGELIMIT * dwPageSize, // Size of allocation
        MEM_RESERVE,          // Allocate reserved pages
        PAGE_NOACCESS);       // Protection = no access
    if (lpvBase == NULL)
        ErrorExit("VirtualAlloc reserve failed.");

    lpPtr = lpNxtPage = (LPTSTR)lpvBase;

    // Use structured exception handling when accessing the pages.
    // If a page fault occurs, the exception filter is executed to
    // commit another page from the reserved block of pages.

    for (i = 0; i < PAGELIMIT * dwPageSize; i++)
    {
        __try
        {
            // Write to memory.

            lpPtr[i] = 'a';
        }

        // If there's a page fault, commit another page and try again.

        __except (PageFaultExceptionFilter(GetExceptionCode()))
        {

            // This code is executed only if the filter function
            // is unsuccessful in committing the next page.

            printf("Exiting process.\n");

            ExitProcess(GetLastError());

        }

    }

    // Release the block of pages when you are finished using them.

    bSuccess = VirtualFree(
        lpvBase,       // Base address of block
        0,             // Bytes of committed pages
        MEM_RELEASE);  // Decommit the pages
    return 0;
}
