#pragma once

#include <windows.h>
#include <detours.h>
#include <shlwapi.h>
#include <stdio.h>

CHAR ApplicationPath[4096]{};
CHAR CurrentDirectory[4096]{};
CHAR CommandLine[4096]{};
CHAR DllPath[4096]{};

STARTUPINFOA StartupInfo = { .cb = sizeof(STARTUPINFOA) };
PROCESS_INFORMATION ProcessInfo = { };

char const usage[] =
R"(
Info:
    this launcher is required to be in the executable directory
    things denoted with <> are required
    things denoted with [] are optional

Usage:
    launcher.exe <executable_name> <dll_name> [executable command line]

Example:
    launcher.exe eldorado.exe game.dll -centered -cache-memory-increase 1200
)";

