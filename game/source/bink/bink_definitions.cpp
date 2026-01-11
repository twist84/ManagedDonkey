#include "bink/bink_definitions.hpp"

#include "cache/cache_files.hpp"
#include "main/main.hpp"

bool write_bink_tag_resource_to_disk(int32 tag_index, const char* filename)
{
	TAG_RESOURCES_GAME_LOCK();

	s_bink_definition* bink_definition = TAG_GET(BINK_TAG, s_bink_definition, tag_index);
	if (bink_definition == NULL)
	{
		return false;
	}

	s_bink_resource_definition* bink_resource = TAG_RESOURCE_TRY_TO_GET(s_bink_resource_definition, &bink_definition->bink_resource);
	if (bink_resource == NULL || bink_resource->resource_data.address == NULL)
	{
		return false;
	}

	s_file_reference info{};
	file_reference_create_from_path(&info, filename, false);

	if (!file_create_parent_directories_if_not_present(&info))
	{
		return false;
	}

	if (!file_create(&info))
	{
		return false;
	}

	uns32 error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_write), &error))
	{
		return false;
	}

	bool result = file_write(&info, bink_resource->resource_data.size, bink_resource->resource_data.address);

	if (!file_close(&info))
	{
		return false;
	}

	return result;
}

void write_all_bink_tag_resources_to_disk()
{
	tag_iterator iterator{};
	tag_iterator_new(&iterator, BINK_TAG);
	for (int32 tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		const char* name = tag_name_strip_path(tag_get_name(tag_index));
		if (name[0] != 0)
		{
			c_static_string<260> string{};
			const char* file_name = string.print("bink\\%s.bik", name);
			write_bink_tag_resource_to_disk(tag_index, name);
		}
	}
}

