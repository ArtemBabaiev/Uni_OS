#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>             
#include <iostream>

LPTSTR lpNxtPage;              
DWORD dwPages = 0;             
DWORD dwPageSize;      
LPVOID lpvResult;

VOID _tmain(VOID)
{
    LPVOID lpvBase;               
    LPTSTR lpPtr;                 
    BOOL bSuccess;                
    DWORD i;                      
    SYSTEM_INFO sSysInfo;         

    GetSystemInfo(&sSysInfo);     

    //_tprintf(TEXT("This computer has page size %d.\n"), sSysInfo.dwPageSize);

    dwPageSize = sSysInfo.dwPageSize;

    std::cin.ignore();

    lpvBase = VirtualAlloc(
        NULL,                 
        10485760,
        MEM_RESERVE,          
        PAGE_READWRITE);  

    lpvResult = VirtualAlloc(
        lpvBase,
        10485760,
        MEM_COMMIT,
        PAGE_READWRITE);

    memmove(lpvBase, (const void*)"1337", 4);

    printf("The data which is stored in the memory is %s\n", lpvBase);
    std::cin.ignore();

    bSuccess = VirtualFree(lpvBase,
        10485760,
        MEM_RELEASE);

    _tprintf(TEXT("Release %s.\n"), bSuccess ? TEXT("succeeded") : TEXT("failed"));

}