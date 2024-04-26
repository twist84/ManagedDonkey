#pragma once

#include "cseries/cseries.hpp"

// #TODO: confirm this
enum e_collision_test_flag
{
	_collision_test_structure_bit = 0,
	_collision_test_water_bit = 1,
	_collision_test_instanced_geometry_bit = 3,
	_collision_test_render_only_bsps_bit = 4,
	_collision_test_try_to_keep_location_valid_bit = 17,
	_collision_test_objects_bit = 32,
};

using s_collision_test_flags = c_static_flags<64>;

