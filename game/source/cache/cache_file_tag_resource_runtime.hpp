#pragma once

#include "cseries/cseries.hpp"

struct s_cache_file_shared_resource_usage
{
	byte __data[0x2328];
};
static_assert(sizeof(s_cache_file_shared_resource_usage) == 0x2328);

struct s_cache_file_insertion_point_resource_usage
{
	byte __data[0xB4];
};
static_assert(sizeof(s_cache_file_insertion_point_resource_usage) == 0xB4);

struct s_tag_resource;
extern void* __cdecl tag_resource_get(s_tag_resource const* resource);