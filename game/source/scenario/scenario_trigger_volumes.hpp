#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

#define MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO 1024

struct s_real_sector_point;
struct s_trigger_volume_triangle;
struct scenario_trigger_volume
{
	c_string_id name;

	// scenario_object_name
	short object_name; // short_block_index

	short runtime_node_index;
	c_string_id node_name;

	short type;

	// pad
	byte padding[2];

	vector3d forward;
	vector3d up;
	real_point3d position;
	real_point3d extents;

	// this is only valid for sector type trigger volumes
	real z_sink;

	c_typed_tag_block<s_real_sector_point> sector_points;
	c_typed_tag_block<s_trigger_volume_triangle> runtime_triangles;

	real_rectangle3d runtime_sector_bounds;

	// radius
	real C;

	// s_scenario_kill_trigger_volume
	short kill_trigger_volume_index; // short_block_index

	// s_scenario_editor_folder
	short editor_folder; // short_block_index
};
static_assert(sizeof(scenario_trigger_volume) == 0x7C);

struct s_real_sector_point
{
	real_point3d position;
	euler_angles2d normal;
};
static_assert(sizeof(s_real_sector_point) == 0x14);

struct s_trigger_volume_triangle
{
	plane3d lower_plane;
	plane3d upper_plane;
	real_point2d vertex[3];
	real_rectangle3d bounds;
};
static_assert(sizeof(s_trigger_volume_triangle) == 0x50);

struct c_trigger_volume_query
{
	scenario_trigger_volume* m_trigger_volume;

	// #TODO: confirm names
	real_matrix4x3 m_matrix;
	bool m_has_matrix;
};
static_assert(sizeof(c_trigger_volume_query) == 0x3C);

extern bool __cdecl trigger_volume_build_faces(scenario_trigger_volume const* volume, real_point3d(&faces)[k_faces_per_cube_count][4]);
extern bool __cdecl trigger_volume_get_center(scenario_trigger_volume const* volume, union real_point3d* center);
extern bool __cdecl trigger_volume_get_matrix(scenario_trigger_volume const* volume, real_matrix4x3* matrix);
extern real __cdecl trigger_volume_get_radius(scenario_trigger_volume const* volume);

