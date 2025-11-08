#pragma once

#include "ai/ai_actions.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct pathfinding_data;

struct hint_hoist_data
{
	uns32 sector_index : 16;
	uns32 point_index0 : 16;
	uns32 point_index1 : 16;
	uns32 normal_index : 16;
	int16 flags : 16;
};
static_assert(sizeof(hint_hoist_data) == 0xC);

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

struct user_hint_jump;

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

enum
{
	_user_hint_well_bidirectional_bit = 0,

	k_user_hint_well_flags_count,
};

enum
{
	_user_hint_well_point_jump = 0,
	_user_hint_well_point_invalid,
	_user_hint_well_point_hoist,

	k_user_hint_well_point_type_count,
};

struct user_hint_well
{
	int32 flags;
	c_typed_tag_block<user_hint_well_point> points;
};
static_assert(sizeof(user_hint_well) == 0x10);

struct user_hint_data
{
	c_typed_tag_block<user_hint_line_segment> line_segment_geometry;
	c_typed_tag_block<user_hint_parallelogram> parallelogram_geometry;
	s_tag_block jump_hints;
	s_tag_block climb_hints;
	c_typed_tag_block<user_hint_well> well_hints;
	c_typed_tag_block<user_flight_hint> flight_hints;

	// cookie cutters
	s_tag_block volumes;

	s_tag_block giant_hints;
	s_tag_block flood_hints;
};
static_assert(sizeof(user_hint_data) == 0x6C);

struct s_user_hint_flood;
struct s_user_hint_giant;
struct s_user_hint_giant_rail;
struct s_user_hint_sector;
struct s_user_hint_sector_point;

extern void ai_render_hints();
extern void ai_render_object_hints(bool active_only);
extern void ai_render_object_properties();
extern void ai_render_user_hints();
extern void render_arrow(const real_point3d* point, const real_vector3d* vector, real32 length, const real_argb_color* color, bool bidirectional);
extern void render_arrow(real_point3d* point0, const real_point3d* point1, const real_argb_color* color, bool bidirectional);
extern void render_flight_hint(const user_flight_hint* flight_hint);
extern void render_flight_hints();
extern void render_giant_sector_hints(const real_argb_color* color);
extern void render_giant_rail_hints(const real_argb_color* color);
extern void render_hoist_hint(const pathfinding_data* pf_data, int32 hint_index);
extern void render_sector_hint(const s_user_hint_sector* sector_hint, const real_argb_color* color);
extern void render_sector_hints(const real_argb_color* color);
extern void render_well_hint(const user_hint_well* well, const real_argb_color* color, bool full);
extern void render_well_hints(const real_argb_color* color, bool full);
extern void user_hint_handle_parallelogram_point_move(user_hint_parallelogram* parallelogram, int16 point_index, const real_point3d* point);
extern bool user_hint_render_jump(int16 jump_hint_index);
extern void user_hint_render_line_segment(const user_hint_line_segment* line_segment, const real_argb_color* color);
extern void user_hint_render_parallelogram(const user_hint_parallelogram* parallelogram, bool bidirectional);

