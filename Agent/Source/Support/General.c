


#include "Macro/Macro.h"
#include "Debug.h"
#include "Support/Memory.h"
char TaskNotFoundCode[] = {0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1};


int VerifyTask(char * IPv6){

    if (!memcmp(IPv6, TaskNotFoundCode, sizeof(TaskNotFoundCode)))
        {
            RED_DPRINT("Task Not Found!\n");
            return 0;
        }
        return 1;
}