#pragma once

#include "cseries/cseries.hpp"
#include "objects/multiplayer_game_objects.hpp"

struct c_area
{
	s_multiplayer_object_boundary_geometry_data m_geometry_data;
	real_argb_color m_color;
	long m_object_index;
};
static_assert(sizeof(c_area) == 0x64);

template<typename t_area_type, long k_area_max_count>
struct c_area_set
{
	static_assert(__is_base_of(c_area, t_area_type));

	c_static_array<t_area_type, k_area_max_count> m_areas;
	long m_area_count;

	// enum e_multiplayer_team_designator
	long m_team_designator;

	long m_object_type_flags;
	long m_object_match_flags;
};
static_assert(sizeof(c_area_set<c_area, 1>) == 0x74);

