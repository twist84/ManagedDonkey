#pragma once

#include "cseries/cseries.hpp"

class c_scenario_resource_registry
{
public:
	c_static_flags<20000>* get_tag_instance_flags();
	bool is_tag_available(int32 tag_index) const;

protected:
	c_static_flags<20000> m_active_tags_flags;
};
COMPILE_ASSERT(sizeof(c_scenario_resource_registry) == 0x9C4);

extern void __cdecl scenario_resources_prepare_for_next_map();
extern void __cdecl scenario_resources_unload_active_zone_set();

