#include "visibility/visibility_collection.hpp"

#include "memory/module.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00686E80, c_visibility_collection, prepare_collection_for_build);

REFERENCE_DECLARE(0x018BF6B0, int32, c_visible_items::m_marker_count);
REFERENCE_DECLARE(0x018BF6E8, s_visible_items, c_visible_items::m_items);
REFERENCE_DECLARE_ARRAY(0x024B0A58, c_visible_items::s_marker_indices, c_visible_items::m_marker_indices, k_maximum_item_markers);
REFERENCE_DECLARE(0x024B0AC8, c_visibility_globals_keeper, g_visibility_globals_keeper);

bool debug_pvs = false;
bool debug_pvs_render_all = false;
bool debug_pvs_activation = false;
bool debug_portals = false;
bool visibility_debug_visible_clusters = false;

//.text:00686780 ; public: s_visible_items::s_visible_items()

void __cdecl c_visible_items::clear()
{
	INVOKE(0x006868F0, c_visible_items::clear);
}

void __cdecl c_visible_items::clear_all()
{
	INVOKE(0x00686950, c_visible_items::clear_all);
}

c_visibility_collection* __cdecl get_global_camera_collection()
{
	return INVOKE(0x00686B90, get_global_camera_collection);
}

s_visible_items& __cdecl get_global_items()
{
	return INVOKE(0x00686BA0, get_global_items);
}

//.text:00686BB0 ; 
//.text:00686BC0 ; public: uns32* __cdecl c_visibility_collection::get_part_bitvector_space(int32)

void __cdecl c_visible_items::pop_marker()
{
	INVOKE(0x00686DE0, c_visible_items::pop_marker);
}

e_collection_shape c_visibility_collection::prepare_collection_for_build(int32 flags, e_collection_type collection_type, const visibility_projection* projections, int32 projection_count, s_cluster_reference initial_cluster_reference, int32 intersection_marker_index, const real_point3d* sphere_center, real32 sphere_radius, int32 user_index, int32 player_window_index)
{
	//return INVOKE_CLASS_MEMBER(0x00686E80, c_visibility_collection, prepare_collection_for_build, flags, collection_type, projections, projection_count, initial_cluster_reference, intersection_marker_index, sphere_center, sphere_radius, user_index, player_window_index);

	ASSERT(sphere_center);

	m_input->cluster_reference = initial_cluster_reference;
	m_input->collection_type = collection_type;
	m_input->user_index = user_index;
	m_input->player_window_index = player_window_index;
	m_input->flags = flags;
	m_input->projection_count = (int16)projection_count;
	m_input->sphere_center = *sphere_center;
	m_input->sphere_radius = sphere_radius;
	m_input->visible_items_marker_index = c_visible_items::m_marker_count;

	if (projections != m_input->region.projections)
	{
		csmemcpy(m_input->region.projections, projections, sizeof(visibility_projection) * projection_count);
	}
	m_input->region.projection_count = (int16)projection_count;

	if (initial_cluster_reference.bsp_index == 0xFF)
	{
		s_location location{};
		scenario_location_from_point(&location, sphere_center);
		m_input->cluster_reference = location.cluster_reference;
	}

	if (m_input->collection_type == _visibility_collection_type_camera && (TEST_BIT(m_input->flags, 6) || debug_pvs))
	{
		m_input->collection_shape = _visibility_collection_shape_pvs;
	}
	else if (projection_count && (sphere_radius >= 3.0f || SLOBYTE(m_input->flags) >= 0))
	{
		m_input->collection_shape = _visibility_collection_shape_projections;
	}
	else
	{
		m_input->collection_shape = _visibility_collection_shape_sphere;
	}

	return m_input->collection_shape;
}

void __cdecl c_visible_items::push_marker()
{
	INVOKE(0x00686FD0, c_visible_items::push_marker);
}

void __cdecl visibility_collection_dispose()
{
	//INVOKE(0x006874E0, visibility_collection_dispose);
}

void __cdecl visibility_collection_initialize()
{
	//INVOKE(0x006874F0, visibility_collection_initialize);

	g_visibility_globals_keeper.initialize();
	g_visibility_globals_keeper.get()->m_visibility_globals.camera_visibility.m_input = &g_visibility_globals_keeper.get()->m_visibility_globals.g_camera_visibility_input;
}

uns16 __cdecl c_visible_items::get_cluster_starting_index()
{
	return m_marker_indices[m_marker_count].cluster_starting_index;
}

uns16 __cdecl c_visible_items::get_instance_list_starting_index()
{
	return m_marker_indices[m_marker_count].instance_list_starting_index;
}

uns16 __cdecl c_visible_items::get_instances_starting_index()
{
	return m_marker_indices[m_marker_count].instances_starting_index;
}

uns16 __cdecl c_visible_items::get_light_starting_index()
{
	return m_marker_indices[m_marker_count].light_starting_index;
}

uns16 __cdecl c_visible_items::get_objects_starting_index()
{
	return m_marker_indices[m_marker_count].objects_starting_index;
}

uns16 __cdecl c_visible_items::get_root_objects_starting_index()
{
	return m_marker_indices[m_marker_count].root_objects_starting_index;
}

uns16 __cdecl c_visible_items::get_sky_starting_index()
{
	return m_marker_indices[m_marker_count].sky_starting_index;
}

