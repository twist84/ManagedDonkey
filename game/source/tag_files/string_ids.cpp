#include "tag_files/string_ids.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/files.hpp"

#include <assert.h>

// max string characters 128, string_id_retrieve

#include "string_ids/string_ids.global_strings.inl"
#include "string_ids/string_ids.gui_strings.inl"
#include "string_ids/string_ids.gui_alert_strings.inl"
#include "string_ids/string_ids.gui_dialog_strings.inl"
#include "string_ids/string_ids.game_engine_strings.inl"
#include "string_ids/string_ids.game_start_strings.inl"
#include "string_ids/string_ids.online_strings.inl"
#include "string_ids/string_ids.saved_game_strings.inl"
#include "string_ids/string_ids.gpu_strings.inl"
#include "string_ids/string_ids.defined_in_tags.inl"

constexpr dword k_string_namespace_count_mapping[k_string_namespace_count] =
{
	k_gui_string_id_count,
	k_gui_alert_string_id_count,
	k_gui_dialog_string_id_count,
	k_game_start_string_id_count,
	k_game_engine_string_id_count,
	k_online_string_id_count,
	k_saved_game_string_id_count,
	k_gpu_string_id_count,
	k_global_string_id_count
};

constexpr e_string_namespace k_string_index_namespace_mapping[k_string_namespace_count] =
{
	_string_namespace_gui,
	_string_namespace_gui_alert,
	_string_namespace_gui_dialog,
	_string_namespace_game_start,
	_string_namespace_game_engine,
	_string_namespace_online,
	_string_namespace_saved_game,
	_string_namespace_gpu,
	_string_namespace_global
};

const char* const k_string_namespace_names[k_string_namespace_count]
{
	"global",
	"gui",
	"gui_alert",
	"gui_dialog",
	"game_engine",
	"game_start",
	"online",
	"saved_game",
	"gpu"
};

static dword g_string_count = 0;
static dword g_string_ascii_storage_offsets[1 << 16]{};
static char g_string_ascii_storage[(1 << 16) * 128]{};

// this is far from the correct impementation
char const* string_id_get_string_const(long string_id)
{
	//string_id_initialize();

	if (string_id == _string_id_invalid)
		return "<string id invalid>";

	long namespace_index = STRING_NAMESPACE_FROM_STRING_ID(string_id);
	long string_index = STRING_INDEX_FROM_STRING_ID(string_id);

	assert(namespace_index >= 0 && namespace_index < k_string_namespace_count);

	char const* string = nullptr;

	switch (namespace_index)
	{
	case _string_namespace_global:
	{
		// defined_in_tags
		if (string_index >= k_global_string_id_count)
			break;
	
		assert(string_index >= 0 && string_index < k_global_string_id_count);
		string = k_global_string_id_strings[string_index];
		break;
	}
	case _string_namespace_gui:
	{
		assert(string_index >= 0 && string_index < k_gui_string_id_count);
		string = k_gui_string_id_strings[string_index];
		break;
	}
	case _string_namespace_gui_alert:
	{
		assert(string_index >= 0 && string_index < k_gui_alert_string_id_count);
		string = k_gui_alert_string_id_strings[string_index];
		break;
	}
	case _string_namespace_gui_dialog:
	{
		assert(string_index >= 0 && string_index < k_gui_dialog_string_id_count);
		string = k_gui_dialog_string_id_strings[string_index];
		break;
	}
	case _string_namespace_game_engine:
	{
		assert(string_index >= 0 && string_index < k_game_engine_string_id_count);
		string = k_game_engine_string_id_strings[string_index];
		break;
	}
	case _string_namespace_game_start:
	{
		assert(string_index >= 0 && string_index < k_game_start_string_id_count);
		string = k_game_start_string_id_strings[string_index];
		break;
	}
	case _string_namespace_online:
	{
		assert(string_index >= 0 && string_index < k_online_string_id_count);
		string = k_online_string_id_strings[string_index];
		break;
	}
	case _string_namespace_saved_game:
	{
		assert(string_index >= 0 && string_index < k_saved_game_string_id_count);
		string = k_saved_game_string_id_strings[string_index];
		break;
	}
	case _string_namespace_gpu:
	{
		assert(string_index >= 0 && string_index < k_gpu_string_id_count);
		string = k_gpu_string_id_strings[string_index];
		break;
	}
	}

	if (string)
		return string;
	
	assert(string_index > g_strings_defined_in_tags_starting_index);
	
	long const defined_in_tags_index = string_index - g_strings_defined_in_tags_starting_index;
	long const defined_in_tags_count = NUMBEROF(g_strings_defined_in_tags);
	
	if (defined_in_tags_index >= 0 && defined_in_tags_index < defined_in_tags_count)
		string = g_strings_defined_in_tags[defined_in_tags_index];
	
	return string;
}

long string_id_retrieve(char const* string)
{
	// #TODO: implement this
	return _string_id_invalid;
}

struct cache_file_strings_header
{
	dword string_count;
	dword string_buffer_size;
};
static_assert(sizeof(cache_file_strings_header) == 0x8);

void string_id_initialize()
{
	static bool initialized = false;
	if (!initialized)
	{
		cache_file_strings_header strings_header;

		s_file_reference string_ids_file;
		file_reference_create_from_path(&string_ids_file, "maps\\string_ids.dat", false);
		if (file_exists(&string_ids_file))
		{
			dword error = 0;
			if (file_open(&string_ids_file, FLAG(_file_open_flag_desired_access_read), &error))
			{
				dword file_size = 0;
				if (file_get_size(&string_ids_file, &file_size))
				{
					if (file_read(&string_ids_file, sizeof(strings_header), false, &strings_header))
					{
						dword string_offsets_size = strings_header.string_count * sizeof(dword);
						dword string_buffer_size = strings_header.string_buffer_size;

						g_string_count = strings_header.string_count;
						file_read(&string_ids_file, string_offsets_size, false, g_string_ascii_storage_offsets);
						file_read(&string_ids_file, string_buffer_size, false, g_string_ascii_storage);
					}
				}

				file_close(&string_ids_file);
			}
		}
	}

	initialized = true;
}
