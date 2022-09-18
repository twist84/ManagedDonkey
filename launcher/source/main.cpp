#include "main.hpp"

int main()
{
    GetCurrentDirectoryA(4096, CurrentDirectory);
    if (GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
        return 1;

    PathCombineA(ApplicationPath, CurrentDirectory, "hf2p_game_client_cache_release.exe");
    if (GetFileAttributesA(ApplicationPath) == INVALID_FILE_ATTRIBUTES)
        return 2;

    PathCombineA(DllPath, CurrentDirectory, "game.dll");
    if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
        return 3;

    if (DetourCreateProcessWithDllA(ApplicationPath, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
        return 4;

#ifdef _DEBUG
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
#endif // _DEBUG

    return 0;
}