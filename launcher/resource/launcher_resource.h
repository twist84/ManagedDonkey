
//#define DLL_FROM_RESOURCE

#if defined(DLL_FROM_RESOURCE)

#define DLL_RESOURCE_ID 101
#define DLL_RESOURCE_PATH "..\\..\\bin\\game.dll"

enum e_resource_type
{
	_resource_type_dll = 0,

	k_number_of_resource_types
};

static bool embedded_resource_extract(long resource_id, e_resource_type resource_type, char const* resource_path)
{
	HRSRC resource_handle = FindResource(nullptr, MAKEINTRESOURCE(resource_id), RT_RCDATA);
	if (!resource_handle)
	{
		printf("Launcher: Failed to find resource '%d'", resource_id);
		return false;
	}

	HGLOBAL resource_data_handle = LoadResource(nullptr, resource_handle);
	if (!resource_data_handle)
	{
		printf("Launcher: Failed to load resource '%d'", resource_id);
		return false;
	}

	DWORD resource_size = SizeofResource(nullptr, resource_handle);
	void* resource_data = LockResource(resource_data_handle);

	if (!CreateDirectoryA("bin\\", nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		printf("Launcher: Failed to create directory for resource '%d'", resource_id);
		return false;
	}

	HANDLE resource_file_handle = CreateFileA(resource_path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (resource_file_handle == INVALID_HANDLE_VALUE)
	{
		printf("Launcher: Failed to create resource '%d'", resource_id);
		return false;
	}

	DWORD written;
	WriteFile(resource_file_handle, resource_data, resource_size, &written, nullptr);
	CloseHandle(resource_file_handle);

	if (resource_type == _resource_type_dll)
	{
		HMODULE dll_handle = LoadLibraryExA(resource_path, nullptr, DONT_RESOLVE_DLL_REFERENCES);
		if (!dll_handle)
		{
			printf("Launcher: Failed to load DLL '%d'", resource_id);
			return false;
		}
		FreeLibrary(dll_handle);
	}

	return true;
}

#endif

