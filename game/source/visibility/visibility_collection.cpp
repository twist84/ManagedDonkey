#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE(0x018BF6B0, long, c_visible_items::m_marker_count);
REFERENCE_DECLARE(0x018BF6E8, s_visible_items, c_visible_items::m_items);
REFERENCE_DECLARE_ARRAY(0x024B0A58, c_visible_items::s_marker_indices, c_visible_items::m_marker_indices, k_maximum_item_markers);

void __cdecl visibility_collection_dispose()
{
	INVOKE(0x006874E0, visibility_collection_dispose);
}

void __cdecl visibility_collection_initialize()
{
	INVOKE(0x006874F0, visibility_collection_initialize);
}

void __cdecl c_visible_items::clear_all()
{
	INVOKE(0x00686950, c_visible_items::clear_all);
}

word __cdecl c_visible_items::get_root_objects_starting_index()
{
	return m_marker_indices[m_marker_count].root_objects_starting_index;
}

word __cdecl c_visible_items::get_objects_starting_index()
{
	return m_marker_indices[m_marker_count].objects_starting_index;
}

word __cdecl c_visible_items::get_instance_list_starting_index()
{
	return m_marker_indices[m_marker_count].instance_list_starting_index;
}

word __cdecl c_visible_items::get_instances_starting_index()
{
	return m_marker_indices[m_marker_count].instances_starting_index;
}

word __cdecl c_visible_items::get_cluster_starting_index()
{
	return m_marker_indices[m_marker_count].cluster_starting_index;
}

word __cdecl c_visible_items::get_light_starting_index()
{
	return m_marker_indices[m_marker_count].light_starting_index;
}

word __cdecl c_visible_items::get_sky_starting_index()
{
	return m_marker_indices[m_marker_count].sky_starting_index;
}

