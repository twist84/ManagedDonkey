#include "objects/watch_window.hpp"

#include "cache/cache_files.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/object_types.hpp"
#include "objects/objects.hpp"

REFERENCE_DECLARE_ARRAY(0x024E00A8, char, watch_object_description, 512);
REFERENCE_DECLARE(0x024E02A8, s_watch_object_results, watch_object_results);

HOOK_DECLARE(0x0068C600, clear_watch_window_globals);
HOOK_DECLARE(0x0068C630, object_index_valid_for_try_and_get);
HOOK_DECLARE(0x0068C680, watch_object);
HOOK_DECLARE(0x0068C750, watch_object_describe);
HOOK_DECLARE(0x0068C810, watch_object_describe_internal);
//HOOK_DECLARE(0x0068C8E0, watch_window_dispose);
//HOOK_DECLARE(0x0068C8F0, watch_window_initialize);

uns32 __cdecl clear_watch_window_globals()
{
	//INVOKE(0x0068C600, clear_watch_window_globals);

	csstrnzcpy(watch_object_description, "failed to get object information", sizeof(watch_object_description));
	csmemset(&watch_object_results, 0, sizeof(s_watch_object_results));
	return NONE;
}

bool __cdecl object_index_valid_for_try_and_get(int32 object_index)
{
	//INVOKE(0x0068C630, object_index_valid_for_try_and_get, object_index);

	return object_index != NONE && object_header_data && DATUM_INDEX_TO_ABSOLUTE_INDEX(object_index) != object_index;
}

s_watch_object_results& __cdecl watch_object(int32 object_index)
{
	//INVOKE(0x0068C680, watch_object, object_index);

	clear_watch_window_globals();
	watch_object_results.object_information = watch_object_description;
	if (object_index_valid_for_try_and_get(object_index))
	{
		watch_object_describe(object_index);
		watch_object_results.object_information = watch_object_description;
		watch_object_results.object = object_get(object_index);
	}
	return watch_object_results;
}

const char* __cdecl watch_object_describe(int32 object_index)
{
	//INVOKE(0x0068C750, watch_object_describe, object_index);

	clear_watch_window_globals();
	if (object_index_valid_for_try_and_get(object_index))
		watch_object_describe_internal(object_index, watch_object_description, sizeof(watch_object_description));

	return watch_object_description;
}

const char* __cdecl watch_object_describe_internal(int32 object_index, char* buffer, int32 buffer_size)
{
	//INVOKE(0x0068C810, watch_object_describe_internal, object_index, buffer, buffer_size);

	if (object_datum* object = object_get(object_index))
	{
		char object_name[128]{};
		csstrnzcpy(object_name, " ", sizeof(object_name));
		if (DATUM_INDEX_TO_ABSOLUTE_INDEX(object->object.name_index) != 0xFFFF && global_scenario_index_get() != NONE)
		{
			global_scenario->object_names[object->object.name_index].name.copy_to(object_name, sizeof(object_name));
			csstrnzcat(object_name, " ", sizeof(object_name));
		}

		const char* tag_name = tag_get_name_safe(object->definition_index);
		const char* object_tag_name = tag_name ? tag_name : nullptr;
		const char* object_type_name = object_type_get_name(object->object.object_identifier.m_type);

		csnzprintf(buffer, buffer_size, "%s %s%s", object_type_name, object_name, object_tag_name);
	}

	return buffer;
}

void __cdecl watch_window_dispose()
{
	//INVOKE(0x0068C8E0, watch_window_dispose);
}

void __cdecl watch_window_initialize()
{
	//INVOKE(0x0068C8F0, watch_window_initialize);

	watch_object(NONE);
}

