#include "tag_files/string_ids.hpp"

#include "cseries/cseries.hpp"

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

// this is far from the correct impementation
char const* string_id_get_string_const(long string_id)
{
	if (string_id == _string_id_invalid)
		return "<string id invalid>";

	long namespace_index = STRING_NAMESPACE_FROM_STRING_ID(string_id);
	long string_index = STRING_INDEX_FROM_STRING_ID(string_id);

	assert(namespace_index >= 0 && namespace_index < k_namespace_count);

	switch (namespace_index)
	{
	case _namespace_global:
	{
		// defined_in_tags
		if (string_index >= k_global_string_id_count)
			break;

		assert(string_index >= 0 && string_index < k_global_string_id_count);
		return k_global_string_id_strings[string_index];
	}
	case _namespace_gui:
	{
		assert(string_index >= 0 && string_index < k_gui_string_id_count);
		return k_gui_string_id_strings[string_index];
	}
	case _namespace_gui_alert:
	{
		assert(string_index >= 0 && string_index < k_gui_alert_string_id_count);
		return k_gui_alert_string_id_strings[string_index];
	}
	case _namespace_gui_dialog:
	{
		assert(string_index >= 0 && string_index < k_gui_dialog_string_id_count);
		return k_gui_dialog_string_id_strings[string_index];
	}
	case _namespace_game_engine:
	{
		assert(string_index >= 0 && string_index < k_game_engine_string_id_count);
		return k_game_engine_string_id_strings[string_index];
	}
	case _namespace_game_start:
	{
		assert(string_index >= 0 && string_index < k_game_start_string_id_count);
		return k_game_start_string_id_strings[string_index];
	}
	case _namespace_online:
	{
		assert(string_index >= 0 && string_index < k_online_string_id_count);
		return k_online_string_id_strings[string_index];
	}
	case _namespace_saved_game:
	{
		assert(string_index >= 0 && string_index < k_saved_game_string_id_count);
		return k_saved_game_string_id_strings[string_index];
	}
	case _namespace_gpu:
	{
		assert(string_index >= 0 && string_index < k_gpu_string_id_count);
		return k_gpu_string_id_strings[string_index];
	}
	}

	assert(string_index > g_strings_defined_in_tags_starting_index);

	long const defined_in_tags_index = string_index - g_strings_defined_in_tags_starting_index;
	long const defined_in_tags_count = NUMBEROF(g_strings_defined_in_tags);

	char const* string = nullptr;
	if (defined_in_tags_index >= 0 && defined_in_tags_index < defined_in_tags_count)
		string = g_strings_defined_in_tags[defined_in_tags_index];

	return string;
}

long string_id_retrieve(char const* string)
{
	// #TODO: implement this
	return _string_id_invalid;
}
