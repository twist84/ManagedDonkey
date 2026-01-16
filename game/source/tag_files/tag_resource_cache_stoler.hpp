#pragma once

#include "cseries/cseries.hpp"

class c_tag_resource_cache_stoler
{
public:
	struct s_stolen_memory_user
	{
		byte __data[0x14];
	};
	COMPILE_ASSERT(sizeof(s_stolen_memory_user) == 0x14);

	int32 m_age;
	c_static_array<s_stolen_memory_user, 6> m_stolen_blocks;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_stoler) == 0x7C);

