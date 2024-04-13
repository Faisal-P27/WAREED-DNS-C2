#pragma once


#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <shlwapi.h>




#pragma comment(lib, "Shlwapi.lib")
#define DS_STREAM_RENAME L":wtfbbq"

// typedef struct _FILE_RENAME_INFO {
//   union {
//     BOOLEAN ReplaceIfExists;
//     DWORD   Flags;
//   } DUMMYUNIONNAME;
//   BOOLEAN ReplaceIfExists1;
//   HANDLE  RootDirectory;
//   DWORD   FileNameLength;
//   WCHAR   FileName[1];
// } FILE_RENAME_INFO, *PFILE_RENAME_INFO;



// typedef enum _FILE_INFO_BY_HANDLE_CLASS {
//   FileBasicInfo,
//   FileStandardInfo,
//   FileNameInfo,
//   FileRenameInfo,
//   FileDispositionInfo,
//   FileAllocationInfo,
//   FileEndOfFileInfo,
//   FileStreamInfo,
//   FileCompressionInfo,
//   FileAttributeTagInfo,
//   FileIdBothDirectoryInfo,
//   FileIdBothDirectoryRestartInfo,
//   FileIoPriorityHintInfo,
//   FileRemoteProtocolInfo,
//   FileFullDirectoryInfo,
//   FileFullDirectoryRestartInfo,
//   FileStorageInfo,
//   FileAlignmentInfo,
//   FileIdInfo,
//   FileIdExtdDirectoryInfo,
//   FileIdExtdDirectoryRestartInfo,
//   FileDispositionInfoEx,
//   FileRenameInfoEx,
//   FileCaseSensitiveInfo,
//   FileNormalizedNameInfo,
//   MaximumFileInfoByHandleClass
// } FILE_INFO_BY_HANDLE_CLASS, *PFILE_INFO_BY_HANDLE_CLASS;

// typedef struct _FILE_DISPOSITION_INFO {
//   BOOLEAN DeleteFile;
// } FILE_DISPOSITION_INFO, *PFILE_DISPOSITION_INFO;

int ExecuteCommand(const char* Command, int* TSize, char** Result);
BOOL SelfDelete();