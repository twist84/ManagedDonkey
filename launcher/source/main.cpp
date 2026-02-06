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

int main(int argc, const char* argv[])
{
	c_simple_span<const char*> args = c_simple_span<const char*>(argv, argv + argc);

	const char* ExecutableName = k_exe_name;
	const char* DllName = k_dll_name;

	bool print_usage = false;
	for (const char* arg : args)
	{
		if (!args.is_front(arg))
		{
			print_usage = print_usage || strcmp(arg, "/?") == 0;
			print_usage = print_usage || strcmp(arg, "/h") == 0;
			print_usage = print_usage || strcmp(arg, "-h") == 0;
			print_usage = print_usage || strcmp(arg, "/help") == 0;
			print_usage = print_usage || strcmp(arg, "--help") == 0;
		}

		if (print_usage)
		{
			break;
		}
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
		for (const char* arg : args)
		{
			if (!args.is_front(arg))
			{
				if (STRING_ENDS_WITH(arg, ".exe"))
				{
					ExecutableName = arg;
				}

				if (STRING_ENDS_WITH(arg, ".dll"))
				{
					DllName = arg;
				}
			}
		}
	}

	GetCurrentDirectoryA(4096, CurrentDirectory);
	if (GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
	{
		return 2;
	}

	if (strstr(ExecutableName, ":\\") != 0)
	{
		strcpy_s(ExecutablePath, ExecutableName);
	}
	else
	{
		PathCombineA(ExecutablePath, CurrentDirectory, ExecutableName);
	}

	printf("Launcher: Checking `%s` exists\n", ExecutableName);

	if (GetFileAttributesA(ExecutablePath) == INVALID_FILE_ATTRIBUTES)
	{
		return 3;
	}

	if (strstr(DllName, ":\\") != 0)
	{
		strcpy_s(DllPath, DllName);
	}
	else
	{
		PathCombineA(DllPath, CurrentDirectory, DllName);
	}

	printf("Launcher: Checking `%s` exists\n", DllName);

	if (GetFileAttributesA(DllPath) == INVALID_FILE_ATTRIBUTES)
	{
		return 4;
	}

	strcpy_s(CurrentDirectory, ExecutablePath);
	if (PathRemoveFileSpecA(CurrentDirectory) == FALSE && GetFileAttributesA(CurrentDirectory) == INVALID_FILE_ATTRIBUTES)
	{
		return 5;
	}

	for (const char* arg : args)
	{
		if (!STRING_ENDS_WITH(arg, ".exe") && !STRING_ENDS_WITH(arg, ".dll"))
		{
			strcat_s(CommandLine, arg);

			if (!args.is_back(arg))
			{
				strcat_s(CommandLine, " ");
			}
		}
	}

	printf("Launcher: Creating process for `%s` with commandline '%s'\n", ExecutableName, CommandLine);
	if (DetourCreateProcessWithDllA(ExecutablePath, CommandLine, nullptr, nullptr, TRUE, CREATE_DEFAULT_ERROR_MODE, nullptr, CurrentDirectory, &StartupInfo, &ProcessInfo, DllPath, nullptr) == FALSE)
	{
		return 6;
	}

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
	CreateProcessA("remote_console.exe", nullptr, nullptr, nullptr, TRUE, CREATE_DEFAULT_ERROR_MODE, nullptr, CurrentDirectory, &StartupInfo, &ProcessInfo);
#endif // REMOTE_CONSOLE_ENABLED

	return 0;
}