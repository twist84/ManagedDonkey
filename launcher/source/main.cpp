#include "main.hpp"

#include "launcher_resource.h"

#if defined(_DEBUG)
#if !defined(DLL_FROM_RESOURCE)
#define NO_WAIT_FOR_PROCESS
#endif // DLL_FROM_RESOURCE
#endif // _DEBUG

#if defined(DEDICATED_SERVER)
#define REMOTE_CONSOLE_ENABLED
#endif // DEDICATED_SERVER

char k_app_name[] = "eldorado.exe";

#if defined(DLL_FROM_RESOURCE)
char k_dll_name[] = "bin\\game.dll";
#else
char k_dll_name[] = "game.dll";
#endif // DLL_FROM_RESOURCE

int main(int argc, char* argv[])
{
	char* ApplicationName = k_app_name;
	char* DllName = k_dll_name;

	int arg_start_index = 1;

	if (argc > arg_start_index)
	{
		bool print_usage = false;
		for (int argi = arg_start_index; !print_usage && argi < argc && argv[argi]; argi++)
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
	}

#if defined(DLL_FROM_RESOURCE)
	if (!embedded_resource_extract(DLL_RESOURCE_ID, _resource_type_dll, k_dll_name))
#endif
	{
		// explicit arg checks
		if (argc == 2)
		{
			if (strstr(argv[1], ".exe") != 0)
			{
				ApplicationName = argv[arg_start_index++];
			}
			else if (strstr(argv[1], ".dll") != 0)
			{
				DllName = argv[arg_start_index++];
			}
		}
		else if (argc == 3)
		{
			if (strstr(argv[1], ".exe") != 0 && strstr(argv[2], ".dll") != 0)
			{
				ApplicationName = argv[arg_start_index++];
				DllName = argv[arg_start_index++];
			}
			else if (strstr(argv[1], ".dll") != 0 && strstr(argv[2], ".exe") != 0)
			{
				DllName = argv[arg_start_index++];
				ApplicationName = argv[arg_start_index++];
			}
		}
	}

	if (argc > arg_start_index)
	{
		for (int argi = arg_start_index; argi < argc; argi++)
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

	if (strstr(ApplicationName, ":\\") != 0)
		strcpy_s(ApplicationPath, ApplicationName);
	else
		PathCombineA(ApplicationPath, CurrentDirectory, ApplicationName);

	if (GetFileAttributesA(ApplicationPath) == INVALID_FILE_ATTRIBUTES)
		return 3;

	printf("Launcher: Checking `%s` exists\n", DllName);

	if (strstr(DllName, ":\\") != 0)
		strcpy_s(DllPath, DllName);
	else
		PathCombineA(DllPath, CurrentDirectory, DllName);

	if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
		return 4;

	strcpy_s(CurrentDirectory, ApplicationPath);
	if (PathRemoveFileSpecA(CurrentDirectory) == FALSE && GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
		return 5;

	printf("Launcher: Creating process `%s`\n", ApplicationName);

	if (DetourCreateProcessWithDllA(ApplicationPath, CommandLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, NULL) == FALSE)
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