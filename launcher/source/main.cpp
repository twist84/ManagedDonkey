#include "main.hpp"

#if defined(_DEBUG)
#define NO_WAIT_FOR_PROCESS
#endif // _DEBUG

#if defined(DEDICATED_SERVER)
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

	if (argc > 3)
	{
		for (int argi = 3; argi < argc; argi++)
		{
			strcat_s(CommandLine, argv[argi]);

			if (argi < argc - 1)
				strcat_s(CommandLine, " ");
		}
	}

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

	if (DetourCreateProcessWithDllA(ApplicationPath, CommandLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
		return 5;

#if !defined(REMOTE_CONSOLE_ENABLED)
#if !defined(NO_WAIT_FOR_PROCESS)
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
#endif // NO_WAIT_FOR_PROCESS
#else
	// leave enough time for `remote_command_initialize` to be called
	Sleep(1000);
	printf("Launcher: Creating process `%s`\n", "remote_console.exe");
	CreateProcessA("remote_console.exe", NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo);
#endif // REMOTE_CONSOLE_ENABLED

	return 0;
}