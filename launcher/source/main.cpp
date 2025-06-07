#include "main.hpp"

#include "launcher_resource.h"

#if defined(_DEBUG)
#if !defined(DLL_FROM_RESOURCE)
#define NO_WAIT_FOR_PROCESS
#endif // DLL_FROM_RESOURCE
#endif // _DEBUG

char k_exe_name[] = "eldorado.exe";

#if defined(DLL_FROM_RESOURCE)
char k_dll_name[] = "bin\\game.dll";
#else
char k_dll_name[] = "game.dll";
#endif // DLL_FROM_RESOURCE

int main(int argc, char* argv[])
{
	char* ExecutableName = k_exe_name;
	char* DllName = k_dll_name;

	bool print_usage = false;
	for (int argi = 1; !print_usage && argi < argc && argv[argi]; argi++)
	{
		print_usage |= strcmp(argv[argi], "/?") == 0;
		print_usage |= strcmp(argv[argi], "/h") == 0;
		print_usage |= strcmp(argv[argi], "-h") == 0;
		print_usage |= strcmp(argv[argi], "/help") == 0;
		print_usage |= strcmp(argv[argi], "--help") == 0;
	}

	if (print_usage)
	{
		printf(usage);
		return 1;
	}

#if defined(DLL_FROM_RESOURCE)
	if (!embedded_resource_extract(DLL_RESOURCE_ID, _resource_type_dll, k_dll_name))
#endif
	{
		for (int argi = 1; argi < argc; argi++)
		{
			if (STRING_ENDS_WITH(argv[argi], ".exe"))
				ExecutableName = argv[argi];

			if (STRING_ENDS_WITH(argv[argi], ".dll"))
				DllName = argv[argi];
		}
	}

	GetCurrentDirectoryA(4096, CurrentDirectory);
	if (GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
		return 2;

	if (strstr(ExecutableName, ":\\") != 0)
		strcpy_s(ExecutablePath, ExecutableName);
	else
		PathCombineA(ExecutablePath, CurrentDirectory, ExecutableName);

	printf("Launcher: Checking `%s` exists\n", ExecutableName);

	if (GetFileAttributesA(ExecutablePath) == INVALID_FILE_ATTRIBUTES)
		return 3;

	if (strstr(DllName, ":\\") != 0)
		strcpy_s(DllPath, DllName);
	else
		PathCombineA(DllPath, CurrentDirectory, DllName);

	printf("Launcher: Checking `%s` exists\n", DllName);

	if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
		return 4;

	strcpy_s(CurrentDirectory, ExecutablePath);
	if (PathRemoveFileSpecA(CurrentDirectory) == FALSE && GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
		return 5;

	printf("Launcher: Parsing process commandline '");

	for (int argi = 0; argi < argc; argi++)
	{
		if (STRING_ENDS_WITH(argv[argi], ".exe"))
			continue;

		if (STRING_ENDS_WITH(argv[argi], ".dll"))
			continue;

		strcat_s(CommandLine, argv[argi]);
		printf("%s", argv[argi]);

		if (argi < argc - 1)
		{
			strcat_s(CommandLine, " ");
			printf(" ");
		}
	}
	printf("'");

	printf("Launcher: Creating executable `%s`\n", ExecutableName);
	if (DetourCreateProcessWithDllA(ExecutablePath, CommandLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
		return 6;

#ifndef REMOTE_CONSOLE_ENABLED
#ifndef NO_WAIT_FOR_PROCESS
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

#if defined(DLL_FROM_RESOURCE)
	DeleteFileA(k_dll_name);
#endif // DLL_FROM_RESOURCE
#endif
#else
	// leave enough time for `remote_command_initialize` to be called
	Sleep(1000);
	printf("Launcher: Creating process `%s`\n", "remote_console.exe");
	CreateProcessA("remote_console.exe", NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo);
#endif // REMOTE_CONSOLE_ENABLED

	return 0;
}