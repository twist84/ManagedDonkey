#include "tag_files/tag_groups.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"

#include <stdlib.h>
#include <string.h>

bool const print_reference_updates = false;

void* __cdecl tag_block_get_element_with_size(const s_tag_block* block, int32 index, int32 size)
{
	//INVOKE(0x0055AA00, tag_block_get_element_with_size, block, index, size);

	ASSERT(block);
	ASSERT(block->count >= 0);
	ASSERT(VALID_INDEX(index, block->count));
	ASSERT(block->address);

	//// due to bad caches the block address needs checking
	//if (!IN_RANGE_INCLUSIVE((uns32)block->address, (uns32)g_cache_file_globals.tag_cache_base_address, (uns32)g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_cache_size))
	//{
	//	return nullptr;
	//}

	return block->base + index * size;
}

void* __cdecl tag_data_get_pointer(const s_tag_data* data, int32 offset, int32 size)
{
	//INVOKE(0x0055AA60, tag_data_get_pointer, data, offset, size);

	ASSERT(size >= 0);
	ASSERT(offset >= 0 && offset + size <= data->size);

	// due to bad caches the data address needs checking
	if (!IN_RANGE_INCLUSIVE((uns32)data->address, (uns32)g_cache_file_globals.tag_cache_base_address, (uns32)g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_cache_size))
	{
		return nullptr;
	}

	return data->base + offset;
}

void __cdecl tag_load_missing_tags_report()
{
	//INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

const char* __cdecl tag_name_strip_path(const char* path)
{
	//return INVOKE(0x0055AA90, tag_name_strip_path, path);

	const char* name = strrchr(path, '\\');
	if (name)
	{
		return name + 1;
	}
	return path;
}

const wchar_t* __cdecl tag_name_strip_path(const wchar_t* path)
{
	const wchar_t* name = wcsrchr(path, '\\');
	if (name)
	{
		return name + 1;
	}
	return path;
}

tag group_name_to_group_tag(const char* group_name)
{
	// string_id_retrieve
	for (int32 i = 0; i < global_tag_group_count; i++)
	{
		const s_cache_file_tag_group* group = &global_tag_groups[i];
		if (csstricmp(group_name, group->name.get_string()) == 0)
		{
			return group->group_tag;
		}
	}

	return NONE;
}

void* s_tag_reference::get_definition()
{
	if (index == NONE)
	{
		return nullptr;
	}

	return tag_get(group_tag, index);
}

const char* s_tag_reference::get_name()
{
	if (name)
	{
		return name;
	}

	if (!VALID_INDEX(index, g_cache_file_globals.header.debug_tag_name_count))
	{
		return "<unknown>";
	}

	if (const char* _name = tag_get_name_safe(index))
	{
		name = _name;
		name_length = csstrnlen(_name, _MAX_PATH);

		//c_console::write_line("setting initial tag name: '%s.%s'", name, get_group_name());

		return _name;
	}

	return "<unknown>";
}

const char* s_tag_reference::get_group_name()
{
	if (group_tag != NONE)
	{
		return tag_group_get_name(group_tag);
	}

	if (!g_cache_file_globals.tag_instances || !g_cache_file_globals.tag_index_absolute_mapping)
	{
		return "<unknown>";
	}

	return g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[index]]->tag_group.name.get_string();
}

void tag_reference_set(s_tag_reference* reference, tag group_tag, const char* name)
{
	ASSERT(reference);
	ASSERT(strlen_debug(name) < k_tag_file_name_length);
	ASSERT(strlen_debug(name) <= LONG_MAX);

	if (reference->index != NONE)
	{
		event(_event_message, "tags:dependencies:unlink: removing reference to '%s.%s'", reference->get_name(), reference->get_group_name());
	}

	reference->group_tag = group_tag;
	reference->index = tag_name_get_index(group_tag, name);

	if (reference->index != NONE)
	{
		event(_event_message, "tags:dependencies:link: setting reference to '%s.%s'", reference->get_name(), reference->get_group_name());
	}
}

void tag_block_set_elements(s_tag_block* block, void* elements)
{
	block->address = elements;
}

void tag_block_set_element_count(s_tag_block* block, int32 count)
{
	block->count = count;
}

