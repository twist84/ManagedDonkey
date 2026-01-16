#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_bink_resource_definition
{
	s_tag_data resource_data;
};
COMPILE_ASSERT(sizeof(s_bink_resource_definition) == sizeof(s_tag_data));

struct s_bink_definition
{
	int32 frame_count;
	s_tag_resource bink_resource;
};
COMPILE_ASSERT(sizeof(s_bink_definition) == 0xC);

extern void write_all_bink_tag_resources_to_disk();

