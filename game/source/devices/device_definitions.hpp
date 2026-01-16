#pragma once

#include "cseries/cseries.hpp"
#include "objects/object_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum // flags
{
	_device_position_loops_bit = 0,
	_device_unused,
	_device_allow_interpolation,

	NUMBER_OF_DEVICE_DEFINITION_FLAGS
};

enum // lightmap_flags
{
	_device_dont_use_in_lightmap_bit = 0,
	_device_dont_use_in_lightprobe,

	NUMBER_OF_DEVICE_DEFINITION_LIGHTMAP_FLAGS
};

struct _device_definition
{
	// $$$ DEVICE $$$

	uns32 flags;
	real32 power_transition_time; // seconds
	real32 power_acceleration_time; // seconds
	real32 powered_position_transition_time; // seconds
	real32 powered_position_acceleration_time; // seconds
	real32 depowered_position_transition_time; // seconds
	real32 depowered_position_acceleration_time; // seconds
	uns16 lightmap_flags;
	uns16 pad;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> positive_start_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> negative_start_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> positive_stop_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> negative_stop_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> depowered_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> repowered_effect;
	real32 delay_time; // seconds
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> delay_effect;
	real32 automatic_activation_radius; // world units
};
COMPILE_ASSERT(sizeof(_device_definition) == 0x98);

struct device_definition
{
	static tag const k_group_tag = DEVICE_TAG;

	_object_definition object;
	_device_definition device;
};
COMPILE_ASSERT(sizeof(device_definition) == sizeof(_object_definition) + sizeof(_device_definition));

enum // type
{
	_control_trigger_player = 0,
	_control_trigger_destruction,

	NUMBER_OF_CONTROL_TRIGGERS
};

enum // trigger
{
	_control_toggle_switch = 0,
	_control_on_button,
	_control_off_button,
	_control_call_button,

	NUMBER_OF_CONTROL_TYPES
};

struct _control_definition
{
	// $$$ CONTROL $$$

	int16 type;
	int16 trigger;
	real32 call_value; // [0,1]
	string_id action_string;

	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> on_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> off_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> deny_effect;
};
COMPILE_ASSERT(sizeof(_control_definition) == 0x3C);

struct control_definition
{
	static tag const k_group_tag = DEVICE_CONTROL_TAG;

	_object_definition object;
	_device_definition device;
	_control_definition control;
};
COMPILE_ASSERT(sizeof(control_definition) == sizeof(_object_definition) + sizeof(_device_definition) + sizeof(_control_definition));

enum // type
{
	_machine_door = 0,
	_machine_platform,
	_machine_gear,

	NUMBER_OF_MACHINE_TYPES
};

enum // flags
{
	_machine_is_pathfinding_obstacle_bit = 0,
	_machine_is_not_pathfinding_obstacle_when_open_bit,

	// lighting based on what's around, rather than what's below
	_machine_is_elevator_bit,

	// machines of type "door" and all other machines with this flag checked can block a door portal
	_machine_is_door_portal_blocker_bit,

	_machine_is_pathfinding_stationary_bit,
	_machine_using_default_occlusion_bounds_bit,

	NUMBER_OF_MACHINE_FLAGS
};

struct _machine_definition
{
	int16 type;
	uns16 flags;
	real32 door_open_time; // seconds

	// maps position [0,1] to occlusion
	real32 door_occlusion_lower_bound;
	real32 door_occlusion_upper_bound;

	int16 collision_response;
	int16 elevator_node_index;
	int16 pathfinding_policy;
	int16 pad0;
};
COMPILE_ASSERT(sizeof(_machine_definition) == 0x18);

struct machine_definition
{
	_object_definition object;
	_device_definition device;
	_machine_definition machine;
};
COMPILE_ASSERT(sizeof(machine_definition) == sizeof(_object_definition) + sizeof(_device_definition) + sizeof(_machine_definition));

struct terminal_difficulty_setting
{
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> dummy_strings;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> story_strings;
	int16 dummy_content_flags;
	int16 story_content_flags;
	int16 dummy_image_frame_index;
	int16 story_image_frame_index;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> error_strings;
};
COMPILE_ASSERT(sizeof(terminal_difficulty_setting) == 0x38);

struct _terminal_definition
{
	// $$$ TERMINAL $$$

	uns32 flags;
	string_id action_string;
	int16 terminal_number;
	int16 pad0;

	// set the exposure to the this when viewing terminal
	real32 exposure_value;

	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> activation_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> deactivation_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> translation_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> untranslation_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> error_sound;
	terminal_difficulty_setting settings[4];
};
COMPILE_ASSERT(sizeof(_terminal_definition) == 0x140);

struct terminal_definition
{
	static tag const k_group_tag = DEVICE_TERMINAL_TAG;

	_object_definition object;
	_device_definition device;
	_terminal_definition terminal;
};
COMPILE_ASSERT(sizeof(terminal_definition) == sizeof(_object_definition) + sizeof(_device_definition) + sizeof(_terminal_definition));

struct _arg_device_definition
{
	// $$$ ARG DEVICE $$$

	string_id action_string;
};
COMPILE_ASSERT(sizeof(_arg_device_definition) == 0x4);

struct arg_device_definition
{
	static tag const k_group_tag = DEVICE_ARG_DEVICE_TAG;

	_object_definition object;
	_device_definition device;
	_arg_device_definition arg_device;
};
COMPILE_ASSERT(sizeof(arg_device_definition) == sizeof(_object_definition) + sizeof(_device_definition) + sizeof(_arg_device_definition));

