#pragma once

#include "tag_files/tag_groups.hpp"

enum
{
	_chud_scripting_class_undefined = 0,
	_chud_scripting_class_weapon_stats,
	_chud_scripting_class_crosshair,
	_chud_scripting_class_shield,
	_chud_scripting_class_grenades,
	_chud_scripting_class_messages,
	_chud_scripting_class_motion_sensor,
	_chud_scripting_class_grenade_spike,
	_chud_scripting_class_grenade_fire,

	// $TODO are there more?

	k_chud_scripting_class_count,
};

struct s_chud_widget_base
{
	string_id artist_name;
	int16 scripting_class;
	uns8 base_flags;
	uns8 sort_layer;
	s_tag_block state_data;
	s_tag_block placement_data;
	s_tag_block animation_data;
	s_tag_block render_data;
};
COMPILE_ASSERT(sizeof(s_chud_widget_base) == 0x38);

struct s_chud_widget_collection :
	s_chud_widget_base
{
	s_chud_widget_base* get_widget_by_index(int32 widget_index, bool* is_text_widget);
	int32 get_widget_count() const;
	int32 get_widget_runtime_index(int32 widget_index);

	s_tag_reference parallax;
	s_tag_block bitmap_widgets;
	s_tag_block text_widgets;
};
COMPILE_ASSERT(sizeof(s_chud_widget_collection) == 0x60);
COMPILE_ASSERT(sizeof(s_chud_widget_collection) == sizeof(s_chud_widget_base) + 0x28);

struct s_chud_widget_placement_data
{
	uns16 anchor_type;
	uns16 anchor_flags;
	real_point2d widget_origin;
	real_point2d origin_offset;
	real_point2d widget_scale;
};
COMPILE_ASSERT(sizeof(s_chud_widget_placement_data) == 0x1C);

struct s_chud_definition_info
{
	int32 low_ammo_loaded_threshold;
	int32 low_ammo_reserve_threshold;
	int32 low_battery_threshold;
};
COMPILE_ASSERT(sizeof(s_chud_definition_info) == 0xC);

struct s_chud_definition
{
	s_tag_block widget_collections;
	s_chud_definition_info info;
};
COMPILE_ASSERT(sizeof(s_chud_definition) == 0x18);

