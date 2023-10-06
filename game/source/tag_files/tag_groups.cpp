#include "tag_files/tag_groups.hpp"

#include "cache/cache_files.hpp"

#include <stdlib.h>
#include <string.h>

bool const print_reference_updates = false;

void* __cdecl tag_block_get_element_with_size(s_tag_block const* block, long index, long size)
{
	//INVOKE(0x0055AA00, tag_block_get_element_with_size, block, index, size);

	ASSERT(block);
	ASSERT(block->count >= 0);
	ASSERT(VALID_INDEX(index, block->count));
	ASSERT(block->address);

	return block->base + index * size;
}

void* __cdecl tag_data_get_pointer(s_tag_data const* data, long offset, long size)
{
	//INVOKE(0x0055AA60, tag_data_get_pointer, data, offset, size);

	ASSERT(size >= 0);
	ASSERT(offset >= 0 && offset + size <= data->size);

	return data->base + offset;
}

void __cdecl tag_load_missing_tags_report()
{
	// nullsub
	INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

char const* __cdecl tag_name_strip_path(char const* path)
{
	//return INVOKE(0x0055AA90, tag_name_strip_path, path);

	char const* name = strrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

wchar_t const* __cdecl tag_name_strip_path(wchar_t const* path)
{
	wchar_t const* name = wcsrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

tag group_name_to_group_tag(char const* group_name)
{
	// string_id_retrieve
	for (long i = 0; i < global_tag_group_count; i++)
	{
		s_cache_file_tag_group const* group = &global_tag_groups[i];
		if (csstricmp(group_name, group->name.get_string()) == 0)
			return group->group_tags[0];
	}

	return NONE;
}

void* s_tag_reference::get_definition()
{
	return tag_get(group_tag, index);
}

char const* s_tag_reference::get_name()
{
	if (name)
		return name;

	if (!VALID_INDEX(index, g_cache_file_globals.header.debug_tag_name_count))
		return "<unknown>";

	if (char const* _name = tag_get_name_safe(index))
	{
		name = _name;
		name_length = csstrnlen(_name, _MAX_PATH);

		//c_console::write_line("setting initial tag name: '%s.%s'", name, get_group_name());

		return _name;
	}

	return "<unknown>";
}

char const* s_tag_reference::get_group_name()
{
	if (group_tag != NONE)
		return tag_group_get_name(group_tag);

	if (!g_cache_file_globals.tag_instances || !g_cache_file_globals.tag_index_absolute_mapping)
		return "<unknown>";

	return g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[index]]->tag_group.name.get_string();
}

void tag_reference_set(s_tag_reference* reference, tag group_tag, char const* name)
{
	ASSERT(reference);
	ASSERT(strlen(name) < k_tag_file_name_length);
	ASSERT(strlen(name) <= LONG_MAX);

	if (reference->index != NONE)
		c_console::write_line("tags:dependencies:unlink: removing reference to '%s.%s'", reference->get_name(), reference->get_group_name());

	reference->group_tag = group_tag;
	reference->index = tag_name_get_index(group_tag, name);

	if (reference->index != NONE)
		c_console::write_line("tags:dependencies:link: setting reference to '%s.%s'", reference->get_name(), reference->get_group_name());
}

