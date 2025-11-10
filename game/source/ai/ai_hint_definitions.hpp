#pragma once

#include "ai/ai_actions.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct pathfinding_data;

enum
{
	_user_hint_well_bidirectional_bit = 0,

	k_user_hint_well_flags_count,
};

enum
{
	_user_hint_geometry_bidirectional = 0,
	_user_hint_geometry_closed,

	k_user_hint_geometry_flag_count,
};

enum
{
	_user_hint_well_point_jump = 0,
	_user_hint_well_point_invalid,
	_user_hint_well_point_hoist,

	k_user_hint_well_point_type_count,
};

enum e_hint_type
{
	_hint_type_intersection = 0,
	_hint_type_jump,
	_hint_type_climb,
	_hint_type_vault,
	_hint_type_mount,
	_hint_type_hoist,
	_hint_type_wall_jump,
	_hint_type_breakable_floor,
	_hint_type_rail,
	_hint_type_seam,
	_hint_type_door,

	k_hint_type_count
};

enum
{
	_climb_hint_hoist_step_bit = 0,
	_climb_hint_hoist_crouch_bit,
	_climb_hint_hoist_stand_bit,

	k_climb_hint_hoist_count,
};

enum
{
	_jump_height_down_bit = 0,
	_jump_height_step_bit,
	_jump_height_crouch_bit,
	_jump_height_stand_bit,
	_jump_height_storey_bit,
	_jump_height_tower_bit,

	k_jump_height_count,
};

enum
{
	_vault_height_step_bit = 0,
	_vault_height_crouch_bit,

	k_vault_height_count,
};

struct hint_breakable_data
{
	int32 surface_index;
	int32 instanced_geometry_index;
};
static_assert(sizeof(hint_breakable_data) == 0x8);

struct hint_door_data
{
	int32 m_pathfinding_door_index;
	int32 m_scenario_object_index;
};
static_assert(sizeof(hint_door_data) == 0x8);

struct hint_hoist_data
{
	uns32 sector_index : 16;
	uns32 point_index0 : 16;
	uns32 point_index1 : 16;
	uns32 normal_index : 16;
	int16 flags : 16;
};
static_assert(sizeof(hint_hoist_data) == 0xC);

struct hint_intersection_data
{
	int32 link_index;
	int32 deprecated;
	int32 deprecateds[2];
};
static_assert(sizeof(hint_intersection_data) == 0x10);

struct hint_jump_data
{
	uns32 point_index0 : 16;
	uns32 point_index1x : 16;
	uns32 point_index2 : 16;
	uns32 point_index3x : 16;
	int32 control_flags : 8;
	int32 flags : 8;
	uns32 destination_sector_index : 16;
	int32 reference_frame0 : 16;
	int32 reference_frame1 : 16;
};
static_assert(sizeof(hint_jump_data) == 0x10);

struct hint_rail_data
{
	uns32 sector_index : 16;
	uns32 point_index : 16;
	int32 flags : 16;
};
static_assert(sizeof(hint_rail_data) == 0x8);

struct hint_seam_data
{
	int32 seam_index;
	int32 seam_edge_index;
	int32 walkable_sector_index;
};
static_assert(sizeof(hint_seam_data) == 0xC);

struct pathfinding_hint_data
{
	int16 type; // e_hint_type
	int16 next_hint;
	union
	{
		hint_intersection_data intersection;
		hint_jump_data jump;
		hint_hoist_data hoist;
		hint_breakable_data breakable;
		hint_rail_data rail;
		hint_seam_data seam;
		hint_door_data door;
		int32 pad1[4];
	};
};
static_assert(sizeof(pathfinding_hint_data) == 0x14);

struct user_hint_climb
{
	int16 flags;
	int16 geometry_index;
	int16 forced_climb_height;
	int16 pad0;
};
static_assert(sizeof(user_hint_climb) == 0x8);

struct user_hint_flight_point
{
	real_point3d point;
};
static_assert(sizeof(user_hint_flight_point) == 0xC);

struct user_flight_hint
{
	c_typed_tag_block<user_hint_flight_point> points;
};
static_assert(sizeof(user_flight_hint) == 0xC);
static_assert(sizeof(user_flight_hint) == sizeof(s_tag_block));

struct user_hint_jump
{
	int16 flags;
	int16 geometry_index;
	int16 forced_jump_height;
	int16 user_flags;
};
static_assert(sizeof(user_hint_jump) == 0x8);

struct user_hint_line_segment
{
	int32 flags;
	union
	{
		struct
		{
			c_ai_point3d point0;
			c_ai_point3d point1;
		};
		c_ai_point3d points[2];
	};
};

struct user_hint_parallelogram
{
	int32 flags;
	union
	{
		struct
		{
			c_ai_point3d point0;
			c_ai_point3d point1;
			c_ai_point3d point2;
			c_ai_point3d point3;
		};
		c_ai_point3d points[4];
	};
	int16 points_invalid_flags;
	int16 pad0;
};

struct user_hint_well_point
{
	int16 type;
	int16 pad0;
	c_ai_point3d position;
	real_euler_angles2d normal;
};
static_assert(sizeof(user_hint_well_point) == 0x1C);

struct user_hint_well
{
	int32 flags;
	c_typed_tag_block<user_hint_well_point> points;
};
static_assert(sizeof(user_hint_well) == 0x10);

struct s_user_hint_sector_point
{
	c_ai_point3d position;
	real_euler_angles2d normal;
};
static_assert(sizeof(s_user_hint_sector_point) == 0x18);

struct s_user_hint_sector
{
	c_typed_tag_block<s_user_hint_sector_point> points;
};
static_assert(sizeof(s_user_hint_sector) == 0xC);

struct s_user_hint_flood
{
	c_typed_tag_block<s_user_hint_sector> flood_sector_hints;
};
static_assert(sizeof(s_user_hint_flood) == 0xC);

struct s_user_hint_giant_rail
{
	int16 geometry_index;
	int16 pad0;
};
static_assert(sizeof(s_user_hint_giant_rail) == 0x4);

struct s_user_hint_giant
{
	s_tag_block giant_sector_hints;
	c_typed_tag_block<s_user_hint_giant_rail> giant_rail_hints;
};
static_assert(sizeof(s_user_hint_giant) == 0x18);

struct user_hint_data
{
	c_typed_tag_block<user_hint_line_segment> line_segment_geometry;
	c_typed_tag_block<user_hint_parallelogram> parallelogram_geometry;
	c_typed_tag_block<user_hint_jump> jump_hints;
	c_typed_tag_block<user_hint_climb> climb_hints;
	c_typed_tag_block<user_hint_well> well_hints;
	c_typed_tag_block<user_flight_hint> flight_hints;

	// cookie cutters
	s_tag_block volumes;

	c_typed_tag_block<s_user_hint_giant> giant_hints;
	c_typed_tag_block<s_user_hint_flood> flood_hints;
};
static_assert(sizeof(user_hint_data) == 0x6C);

extern const char* g_hoist_height_names[k_climb_hint_hoist_count];
extern const char* g_jump_height_names[k_jump_height_count];

extern bool ai_point_on_structure(const c_ai_point3d* point, int16 structure_index, bool* ambiguous);
extern void ai_render_hints();
extern void ai_render_object_hints(bool active_only);
//extern void ai_render_object_properties();
extern void ai_render_user_hints();
extern void render_arrow(const real_point3d* point, const real_vector3d* vector, real32 length, const real_argb_color* color, bool bidirectional);
extern void render_arrow(real_point3d* point0, const real_point3d* point1, const real_argb_color* color, bool bidirectional);
extern void render_flight_hint(const user_flight_hint* flight_hint);
extern void render_flight_hints();
extern void render_giant_rail_hints(const real_argb_color* color);
extern void render_giant_sector_hints(const real_argb_color* color);
extern void render_hoist_hint(const pathfinding_data* pf_data, int32 hint_index);
extern void render_jump_hint(int16 structure_index, int32 hint_index);
extern void render_sector_hint(const s_user_hint_sector* sector_hint, const real_argb_color* color);
extern void render_sector_hints(const real_argb_color* color);
extern void render_well_hint(const user_hint_well* well, const real_argb_color* color, bool full);
extern void render_well_hints(const real_argb_color* color, bool full);
extern void user_hint_handle_parallelogram_point_move(user_hint_parallelogram* parallelogram, int16 point_index, const real_point3d* point);
extern bool user_hint_line_segment_on_active_structure(user_hint_line_segment* line);
extern bool user_hint_line_segment_on_structure(user_hint_line_segment* line, int16 structure_index, bool strict, bool* ambiguous);
extern bool user_hint_render_jump(int16 jump_hint_index);
extern void user_hint_render_jump_vault_internal(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3);
extern void user_hint_render_line_segment(const user_hint_line_segment* line_segment, const real_argb_color* color);
extern void user_hint_render_parallelogram(const user_hint_parallelogram* parallelogram, bool bidirectional);
extern bool user_hint_sector_on_active_structure(s_user_hint_sector* sector_hint);
extern bool user_hint_sector_on_structure(const s_user_hint_sector* sector_hint, int16 structure_index, bool strict, bool* ambiguous);
extern bool user_hint_well_on_active_structure(user_hint_well* well);
extern bool user_hint_well_on_structure(const user_hint_well* well, int16 structure_index, bool strict, bool* ambiguous);

