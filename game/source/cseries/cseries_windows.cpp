#include "cseries/cseries_windows.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "main/main.hpp"
#include "shell/shell_windows.hpp"
#include "text/font_loading.hpp"

#include <windows.h>
#include <time.h>

c_static_string<256> g_cache_path_directory;
c_static_string<256> g_cache_path_format;
c_static_string<256> g_cache_strings_file;
c_static_string<256> g_cache_tags_file;
c_static_string<256> g_cache_tag_list_file;
c_static_string<256> g_cache_resources_file;
c_static_string<256> g_cache_textures_file;
c_static_string<256> g_cache_textures_b_file;
c_static_string<256> g_cache_audio_file;
c_static_string<256> g_cache_video_file;
c_static_string<256> g_hard_drive_font_directory;
c_static_string<256> g_dvd_font_directory;

void __cdecl display_debug_string(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	if (IsDebuggerPresent())
	{
		c_static_string<4096> output = {};
		output.append_print_va(format, list);
		output.append("\n");

		OutputDebugStringA(output.get_string());
	}

	va_end(list);
}

uns32 __cdecl system_get_current_thread_id()
{
	//return INVOKE(0x004EBF60, system_get_current_thread_id);

	return GetCurrentThreadId();
}

void __cdecl system_get_date_and_time(char* buffer, int16 buffer_size, bool short_date_and_time)
{
	INVOKE(0x004EBF70, system_get_date_and_time, buffer, buffer_size, short_date_and_time);
}

void __cdecl system_memory_information_get(s_system_memory_information* information)
{
	INVOKE(0x004EC0E0, system_memory_information_get, information);

	// 32-bit
	//csmemset(information, 0, sizeof(s_system_memory_information));
	//information->free = 0x20000000;
	//information->total = 0x40000000;

	// 64-bit
	//MEMORYSTATUSEX status;
	//csmemset(&status.dwMemoryLoad, 0, sizeof(MEMORYSTATUSEX) - OFFSETOF(MEMORYSTATUSEX, dwMemoryLoad));
	//status.dwLength = sizeof(MEMORYSTATUSEX);
	//GlobalMemoryStatusEx(&status);
	//csmemset(information, 0, sizeof(s_system_memory_information));
	//information->free = status.ullAvailPhys;
	//information->total = status.ullTotalPhys;
}

uns32 __cdecl system_milliseconds()
{
	//return INVOKE(0x004EC110, system_milliseconds);

	return timeGetTime();
}

uns32 __cdecl system_seconds()
{
	//return INVOKE(0x004EC260, system_seconds);

	return static_cast<uns32>(_time64(0));
}

void __cdecl system_abort()
{
	main_halt_and_catch_fire();
}

// __trap()
void __cdecl system_exit()
{
	exit(1);
}

void __cdecl system_set_dll_directory()
{
	//INVOKE(0x0051CF30, system_set_dll_directory);

	char dll_directory[1024]{};
	csstrnzcat(dll_directory, ".\\bin", 1024);
	SetDllDirectoryA(dll_directory);

	//event(_event_message, "system: dll directory={ %s }", dll_directory);
	printf("system: dll directory={ %s }\n", dll_directory);
}

template<int32 k_maximum_count>
bool shell_get_command_line_parameter(char* command_line, const char* parameter_name, c_static_string<k_maximum_count>* value, const char* default_value)
{
	if (!command_line)
	{
		return false;
	}

	if (value)
	{
		*value = default_value;
	}

	char* parameter_offset = strstr(command_line, parameter_name);
	if (!parameter_offset)
	{
		return false;
	}

	do
	{
		parameter_offset += strlen(parameter_name) + 1;
		c_static_string<k_maximum_count> parameter = parameter_offset;
		int32 separator = parameter.index_of(" ");
		if (separator != NONE)
		{
			parameter.set_bounded(parameter_offset, separator);
		}

		if (value)
		{
			*value = parameter;
		}
	}
	while (parameter_offset = strstr(parameter_offset, parameter_name));

	return true;
}

void system_set_maps_directory()
{
	c_static_string<256> map_directory = "maps\\";
	if (shell_get_command_line_parameter(g_windows_params.cmd_line, "-maps", &map_directory, map_directory.get_string()))
	{
		if (!map_directory.ends_with("\\") && !map_directory.ends_with("/"))
		{
			map_directory.append("\\");
		}

		s_file_reference maps_file{};
		if (!file_exists(file_reference_create_from_path(&maps_file, map_directory.get_string(), true)))
		{
			return;
		}
	}

	g_cache_file_globals.map_directory = g_cache_path_directory.print(map_directory.get_string());
	k_cache_path_format = g_cache_path_format.print("%s%s", g_cache_file_globals.map_directory, "%s.map");

	k_cache_strings_file = g_cache_strings_file.print("%s%s", g_cache_file_globals.map_directory, "string_ids.dat");
	k_cache_tags_file = g_cache_tags_file.print("%s%s", g_cache_file_globals.map_directory, "tags.dat");
	k_cache_tag_list_file = g_cache_tag_list_file.print("%s%s", g_cache_file_globals.map_directory, "tag_list.csv");

	k_cache_resources_file = g_cache_resources_file.print("%s%s", g_cache_file_globals.map_directory, "resources.dat");
	k_cache_textures_file = g_cache_textures_file.print("%s%s", g_cache_file_globals.map_directory, "textures.dat");
	k_cache_textures_b_file = g_cache_textures_b_file.print("%s%s", g_cache_file_globals.map_directory, "textures_b.dat");
	k_cache_audio_file = g_cache_audio_file.print("%s%s", g_cache_file_globals.map_directory, "audio.dat");
	k_cache_video_file = g_cache_video_file.print("%s%s", g_cache_file_globals.map_directory, "video.dat");

	k_hard_drive_font_directory = g_hard_drive_font_directory.print("%s%s", g_cache_file_globals.map_directory, "fonts\\");
	k_dvd_font_directory = g_dvd_font_directory.print("%s%s", g_cache_file_globals.map_directory, "fonts\\");

	s_file_reference video_file{};
	if (!file_exists(file_reference_create_from_path(&video_file, k_cache_video_file, true)))
	{
		k_cache_video_file = g_cache_video_file.print("%s%s", g_cache_file_globals.map_directory, "resources_b.dat");
	}

	g_cache_file_globals.resource_files[0] = k_cache_resources_file;
	g_cache_file_globals.resource_files[1] = k_cache_textures_file;
	g_cache_file_globals.resource_files[2] = k_cache_textures_b_file;
	g_cache_file_globals.resource_files[3] = k_cache_audio_file;
	g_cache_file_globals.resource_files[4] = k_cache_video_file;

	printf("system: maps directory={ %s }\n", map_directory.get_string());
}

