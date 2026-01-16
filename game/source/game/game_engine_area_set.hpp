#pragma once

#include "cseries/cseries.hpp"
#include "objects/multiplayer_game_objects.hpp"

class c_area
{
public:
	s_multiplayer_object_boundary_geometry_data m_boundary_data;
	real_argb_color m_area_color;
	int32 m_object_index;
};
COMPILE_ASSERT(sizeof(c_area) == 0x64);

template<typename t_area_type, int32 k_area_max_count>
class c_area_set
{
	COMPILE_ASSERT(__is_base_of(c_area, t_area_type));

public:
	c_static_array<t_area_type, k_area_max_count> m_areas;
	int32 m_number_of_areas;
	int32 m_team_designator;
	int32 m_area_object_type_flags;
	int32 m_area_iteration_match_flags;
};
COMPILE_ASSERT(sizeof(c_area_set<c_area, 1>) == 0x74);

