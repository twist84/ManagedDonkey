#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/c_gui_widget.hpp"

enum e_model_widget_definition_flags
{
	_model_widget_definition_flag_allow_list_item_to_override_animation_skin_bit = k_core_widget_definition_flags,

	k_model_widget_definition_flags,
};

struct s_model_widget_globals_definition
{
	real_argb_color tron_shader_color;
	real32 tron_shader_intensity;
	real32 fov; // degrees
	real32 zoom_speed; // wu per tick
	s_tag_block zoom_transition_fxn; // keyframe_transition_function_block
	int16 move_left_button;
	int16 move_right_button;
	int16 move_forward_button;
	int16 move_backward_button;
	int16 move_up_button;
	int16 move_down_button;
	int16 zoom_in_button;
	int16 zoom_out_button;
	int16 rotate_left_button;
	int16 rotate_right_button;
	int16 rotate_up_button;
	int16 rotate_down_button;
};
static_assert(sizeof(s_model_widget_globals_definition) == 0x40);

struct s_model_widget_camera_slice
{
	c_string_id name; // use empty name for default
	real32 left;
	real32 right;
	real32 top;
	real32 bottom;
};
static_assert(sizeof(s_model_widget_camera_slice) == 0x14);

struct s_model_widget_definition :
	s_core_widget_definition
{
	s_tag_block camera_settings;
	s_model_widget_globals_definition model_widget_globals;
	c_typed_tag_block<s_model_widget_camera_slice> texture_camera_slices;
};
static_assert(sizeof(s_model_widget_definition) == sizeof(s_core_widget_definition) + 0x58);

struct s_runtime_model_widget_camera_settings
{
	// Camera Settings
	// 
	// These are model-viewing camera parameters that you can control
	// * FOV is the field of view used by the texture camera
	//   if left 0, a suitable default FOV is used
	// * Initial Radial Offset is the initial radial distance of the camera from the target model
	// * Final Radial Offset is the final radial distance of the camera from the target model
	// * Camera Radial Step Size is the incremental change in input to the radial transition function per tick
	// * Initial Vertical Offset is the initial vertical distance of the camera from the target's center
	// * Final Vertical Offset is the final vertical distance of the camera from the target's center
	// * Camera Vertical Step Size is the incremental change in input to the vertical transition function per tick
	// * Camera Rotational Step Size is the incremental change in camera rotation per game tick
	// * The Transition Functions are used to control the camera zoom with controller input
	//   if left empty, a linear interpolation is used for camera zoom

	c_string_id name;

	// arbitrary location in the world to place the model
	real_point3d model_world_position;

	real_point3d minimum_world_position;
	real_point3d maximum_world_position;

	real_point3d minimum_camera_offset; // wu
	real_point3d minimum_camera_focal_offset; // wu
	real_point3d maximum_camera_offset; // wu
	real_point3d maximum_camera_focal_offset; // wu
	real32 initial_zoom; // [0,1]
	real32 movement_speed;
	real32 magnetism_constant;
	s_tag_block movement_scale_fxn;
	real_euler_angles2d initial_rotation; // degrees
	real_euler_angles2d minimum_rotation; // degrees
	real_euler_angles2d maximum_rotation; // degrees
	real32 rotation_speed; // degrees per tick
	s_tag_reference model;
	c_string_id variant;
};
static_assert(sizeof(s_runtime_model_widget_camera_settings) == 0xA0);

struct s_runtime_model_widget_definition :
	s_runtime_core_widget_definition
{
	s_runtime_model_widget_camera_settings camera_settings[32];
	s_model_widget_globals_definition model_widget_globals;
	s_model_widget_camera_slice texture_camera_slice[4];
};
static_assert(sizeof(s_runtime_model_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x1490);

struct c_gui_model_widget :
	public c_gui_widget
{
	int32 __unknownDC;

	s_runtime_model_widget_definition m_definition;

	s_player_appearance m_player_appearance;

	uint32 __unknown1C04;
	real_point3d __point1C08;
	real_point3d __point1C14;
	real_point3d __point1C20;

	bool __unknown1C2C;
	uint8 __pad1C2D[3];

	uint8 __data1C30[0xC];

	//name = get_model()->name;
	//if (name == STRING_ID(global, masterchief) || name == STRING_ID(global, dervish) || m_texture_camera_unit_available)
	//	__rotation1C44 *= get_model()->rotation_speed;
	//else
	//	__rotation1C3C *= get_model()->rotation_speed;
	real_euler_angles2d __rotation1C3C;
	real_euler_angles2d __rotation1C44;

	// m_zoom = get_model()->initial_zoom
	real32 m_zoom;

	int32 m_object_index;
	int32 m_render_window;

	c_string_id m_texture_camera_slice_name;
	real32 m_texture_camera_near_clip_distance;
	uint32 __time1C60;
	bool m_texture_camera_unit_available;
	uint32 m_texture_camera_unit_index;

	uint8 __data1C6C[0x4];
};
static_assert(sizeof(c_gui_model_widget) == sizeof(c_gui_widget) + 0x1B94);

