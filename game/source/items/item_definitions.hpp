#pragma once

#include "cseries/cseries.hpp"
#include "objects/object_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum e_item_definition_flags
{
	_item_definition_flag_always_maintains_z_up_bit = 0,
	_item_definition_flag_destroyed_by_explosions_bit,
	_item_definition_flag_unaffected_by_gravity_bit,
	_item_definition_flag_crate_style_collision_filter_bit,

	k_item_definition_flags
};

struct s_predicted_bitmaps;
struct _item_definition
{
	// $$$ ITEM $$$

	c_flags<e_item_definition_flags, uns32, k_item_definition_flags> flags;
	int16 old_message_index;
	int16 sort_order;
	real32 old_multiplayer_on_ground_scale;
	real32 old_campaign_on_ground_scale;


	// NEW hud messages
	// everything you need to display stuff

	c_string_id pickup_message;
	c_string_id swap_message;
	c_string_id pickup_message_dual;
	c_string_id swap_message_dual;
	c_string_id picked_up_msg;
	c_string_id switch_to_msg;
	c_string_id switch_to_from_ai_msg;
	c_string_id notify_empty_msg;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> collision_sound;
	c_typed_tag_block<s_predicted_bitmaps> predicted_bitmaps;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> detonation_damage_effect;
	real_bounds detonation_delay; // seconds
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> detonating_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> detonation_effect;


	// Item scale settings
	// because grenades look better as coconuts. 0==1

	real32 single_player_ground;
	real32 multiplayer_ground;
	real32 small_unit_armed;
	real32 small_unit_stowed;
	real32 medium_unit_armed;
	real32 medium_unit_stowed;
	real32 player_unit_armed;
	real32 player_unit_stowed;
	real32 large_unit_armed;
	real32 large_unit_stowed;


	// Damping settings
	// how much do we damp the velocity when the item is in contact with a slow

	// ~30 == complete damping, 0 == defaults
	real32 grounded_angular_damping;

	// ~30 == complete damping, 0 == defaults
	real32 grounded_linear_damping;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(_item_definition) == 0xB4);

struct item_definition
{
	static tag const k_group_tag = ITEM_TAG;

	_object_definition object;
	_item_definition item;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(item_definition) == sizeof(_object_definition) + sizeof(_item_definition));

struct s_predicted_bitmaps
{
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> bitmap;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_predicted_bitmaps) == sizeof(s_tag_reference));

