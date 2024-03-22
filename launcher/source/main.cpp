#include "main.hpp"

#ifdef _DEBUG
#define NO_WAIT_FOR_PROCESS
#endif // _DEBUG

#ifdef DEDICATED_SERVER
#define REMOTE_CONSOLE_ENABLED
#endif // DEDICATED_SERVER

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf(usage);

        return 1;
    }

    char* ApplicationName = argv[1];
    char* DllName = argv[2];

    GetCurrentDirectoryA(4096, CurrentDirectory);
    if (GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
        return 2;

    printf("Launcher: Checking `%s` exists\n", ApplicationName);

    PathCombineA(ApplicationPath, CurrentDirectory, ApplicationName);
    if (GetFileAttributesA(ApplicationPath) == INVALID_FILE_ATTRIBUTES)
        return 3;

    printf("Launcher: Checking `%s` exists\n", DllName);

    PathCombineA(DllPath, CurrentDirectory, DllName);
    if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
        return 4;

    printf("Launcher: Creating process `%s`\n", ApplicationName);

    CHAR command_line_buffer[256]{};
    strcpy_s(command_line_buffer, "-centered -cache-memory-increase 1200");
    if (DetourCreateProcessWithDllA(ApplicationPath, command_line_buffer, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
        return 5;

#ifndef REMOTE_CONSOLE_ENABLED
#ifndef NO_WAIT_FOR_PROCESS
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
#endif
#else
    // leave enough time for `remote_command_initialize` to be called
    Sleep(1000);
    printf("Launcher: Creating process `%s`\n", "remote_console.exe");
    CreateProcessA("remote_console.exe", NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo);
#endif // REMOTE_CONSOLE_ENABLED

    return 0;
}