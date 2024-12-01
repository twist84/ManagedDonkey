#include "scenario/scenario_zone_resources.hpp"

#include "cache/cache_files.hpp"

c_static_flags<20000>* c_scenario_resource_registry::get_tag_instance_flags()
{
	//return INVOKE_CLASS_MEMBER(0x005FB0E0, c_scenario_resource_registry, get_tag_instance_flags);

	return &m_active_tags_flags;
}

bool c_scenario_resource_registry::is_tag_available(long tag_index) const
{
	//return INVOKE_CLASS_MEMBER(0x005FB0F0, c_scenario_resource_registry, is_tag_available, tag_index);

	//ASSERT(tag_get(NONE, tag_index));
	//return m_active_tags_flags.test(DATUM_INDEX_TO_ABSOLUTE_INDEX(tag_index));

	return true;
}

void __cdecl scenario_resources_prepare_for_next_map()
{
	//INVOKE(0x005FB100, scenario_resources_prepare_for_next_map);

	cache_file_tag_resources_prepare_for_next_map();
}

void __cdecl scenario_resources_unload_active_zone_set()
{
	INVOKE(0x005FB110, scenario_resources_unload_active_zone_set);

	//s_scenario_zone_state zone_state{};
	//cache_file_tag_resources_set_zone_state(NONE, k_string_id_empty_string, &zone_state);
}

