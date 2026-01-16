#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_editor_folder
{
	int32 parent_folder; // long_block_index

	c_static_string<k_tag_long_string_length> name;
};
COMPILE_ASSERT(sizeof(s_scenario_editor_folder) == 0x104);

