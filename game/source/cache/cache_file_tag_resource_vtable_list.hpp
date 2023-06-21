#pragma once

#include "cseries/cseries.hpp"

struct s_tag_persistent_identifier
{
	dword parts[4];
};
static_assert(sizeof(s_tag_persistent_identifier) == 0x10);

struct s_cache_file_tag_resource_vtable
{
	s_tag_persistent_identifier type_identifier;
	long(__cdecl* register_resource)(long resource_owner, void*);
	void(__cdecl* unregister_resource)(long resource_owner, void*);
};
static_assert(sizeof(s_cache_file_tag_resource_vtable) == 0x18);

