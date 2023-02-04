#include <windows.h>
#include <iostream>
#include <stdio.h>



int main(void)

{

	LPCWSTR szDirPath = L"c:\\testdir";

	HANDLE hFile;

	LPCWSTR fname = L"c:\\testdir\\testfile.txt";

	if (!CreateDirectory(szDirPath, NULL))
		printf("\nCouldn't create %S directory.\n", szDirPath);
	else
		printf("\n%S directory successfully created.\n", szDirPath);

	hFile = CreateFile(fname, 
		GENERIC_READ, 
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_ARCHIVE | SECURITY_IMPERSONATION,
		NULL);



	if (hFile == INVALID_HANDLE_VALUE)
		printf("\nCould not open %S file, error %d)\n", fname, GetLastError());

	else

	{
		printf("\n%S file HANDLE is OK!\n", fname);
		printf("\n%S opened successfully!\n", fname);
	}

	if (CloseHandle(hFile) != 0)
		printf("\nCloseHandle() for %S file succeeded!\n", fname);
	else
		printf("\nCloseHandle() for %S file failed!\n", fname);
	
	std::cin.ignore();

	if (DeleteFile(fname) != 0)
		printf("\n%S file successfully deleted!\n", fname);
	else
		printf("\n%S file deletion failed!\n", fname);
	
	if (RemoveDirectory(szDirPath) != 0)
		printf("\n%S directory successfully deleted.\n", szDirPath);
	else
		printf("\n%S directory deletion failed.\n", szDirPath);
	return 0;

}
