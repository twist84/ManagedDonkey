#pragma once

#include <windows.h>
#include <detours.h>
#include <shlwapi.h>
#include <stdio.h>

char CurrentDirectory[4096]{};
char ApplicationPath[4096]{};
char DllPath[4096]{};

STARTUPINFOA StartupInfo = { .cb = sizeof(STARTUPINFOA) };
PROCESS_INFORMATION ProcessInfo = { };

char usage[] =
R"(
Usage:
    launcher.exe <executable_name> <dll_name>

Example:
    launcher.exe eldorado.exe game.dll
)";
