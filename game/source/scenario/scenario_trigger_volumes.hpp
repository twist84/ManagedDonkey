#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

#define MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO 1024

enum e_trigger_volume_side
{
	_trigger_volume_side_position = 0,
	_trigger_volume_side_extent,

	k_number_of_trigger_volume_sides
};

struct s_real_sector_point;
struct s_trigger_volume_triangle;
struct scenario_trigger_volume
{
	c_string_id name;

	// scenario_object_name
	int16 object_name; // short_block_index

	int16 runtime_node_index;
	c_string_id node_name;

	int16 type;

	// pad
	byte padding[0x2];
	
	real_vector3d forward;
	real_vector3d up;
	real_point3d position;
	real_vector3d extents;

	// this is only valid for sector type trigger volumes
	real32 z_sink;

	c_typed_tag_block<s_real_sector_point> sector_points;
	c_typed_tag_block<s_trigger_volume_triangle> runtime_triangles;

	real_rectangle3d runtime_sector_bounds;

	real32 runtime_radius;

	// s_scenario_kill_trigger_volume
	int16 kill_trigger_volume_index; // short_block_index

	// s_scenario_editor_folder
	int16 editor_folder_index; // short_block_index
};
static_assert(sizeof(scenario_trigger_volume) == 0x7C);

struct s_real_sector_point
{
	real_point3d position;
	real_euler_angles2d normal;
};
static_assert(sizeof(s_real_sector_point) == 0x14);

struct s_trigger_volume_triangle
{
	real_plane3d lower_plane;
	real_plane3d upper_plane;
	real_point2d vertex[3];
	real_rectangle3d bounds;
};
static_assert(sizeof(s_trigger_volume_triangle) == 0x50);

class c_trigger_volume_query
{
public:
	scenario_trigger_volume* m_trigger_volume;
	real_matrix4x3 m_transform;
	bool m_valid;
};
static_assert(sizeof(c_trigger_volume_query) == 0x3C);

extern int32 __cdecl scenario_trigger_volume_return_objects(int32 trigger_volume_index);
extern int32 __cdecl scenario_trigger_volume_return_objects_by_type(int32 trigger_volume_index, int32 type_mask);
extern bool __cdecl scenario_trigger_volume_test_object(int32 trigger_volume_index, int32 object_index);
extern bool __cdecl scenario_trigger_volume_test_players(int32 trigger_volume_index);
extern bool __cdecl scenario_trigger_volume_test_players_all(int32 trigger_volume_index);
extern bool __cdecl trigger_volume_build_faces(const scenario_trigger_volume* volume, real_point3d(&face_vertices)[k_faces_per_cube_count][4]);
extern bool __cdecl trigger_volume_get_center(const scenario_trigger_volume* volume, real_point3d* out_center_point);
extern bool __cdecl trigger_volume_get_matrix(const scenario_trigger_volume* volume, real_matrix4x3* matrix);
extern real32 __cdecl trigger_volume_get_radius(const scenario_trigger_volume* volume);

