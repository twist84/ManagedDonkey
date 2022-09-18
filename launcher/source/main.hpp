#pragma once

#include <windows.h>
#include <detours.h>
#include <shlwapi.h>

char CurrentDirectory[4096]{};
char ApplicationPath[4096]{};
char DllPath[4096]{};

STARTUPINFOA StartupInfo = { .cb = sizeof(STARTUPINFOA) };
PROCESS_INFORMATION ProcessInfo = { };
