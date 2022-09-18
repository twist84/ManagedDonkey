#include "main.hpp"

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

    printf("Launcher: Checking %s exists\n", ApplicationName);

    PathCombineA(ApplicationPath, CurrentDirectory, ApplicationName);
    if (GetFileAttributesA(ApplicationPath) == INVALID_FILE_ATTRIBUTES)
        return 3;

    printf("Launcher: Checking %s exists\n", DllName);

    PathCombineA(DllPath, CurrentDirectory, DllName);
    if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
        return 4;

    printf("Launcher: Creating process\n");

    if (DetourCreateProcessWithDllA(ApplicationPath, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
        return 5;

    printf("Launcher: Waiting for %s exit\n", ApplicationName);

#ifdef _DEBUG
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
#endif // _DEBUG

    printf("Launcher: Exiting\n");

    return 0;
}