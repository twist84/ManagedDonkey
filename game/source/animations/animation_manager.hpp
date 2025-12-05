#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/mixing_board/channels/animation_channel.hpp"
#include "math/real_math.hpp"

struct s_animation_event_data;

struct s_animation_goal_settings
{
	string_id mode;
	string_id weapon_class;
	string_id weapon_type;
	string_id state;
};
static_assert(sizeof(s_animation_goal_settings) == 0x10);

class c_model_animation_graph;
class c_animation_manager
{
public:
	const c_model_animation_graph* get_graph() const;
	const c_animation_channel* get_state_channel() const;
	int32 get_state_name() const;
	bool set_goal(string_id input_mode_name, string_id input_weapon_class_name, string_id input_weapon_type_name, string_id input_state_name, int32 flags, uns32 playback_flags, real32 playback_scale);
	void set_state_position_to_last_frame();
	bool update_state_animation(void(__cdecl* callback)(const s_animation_event_data*, int32), int32 user_param, int32 orientation_count, real_orientation* original_orientations, real_orientation* current_orientations);
	bool valid_graph() const;

	c_animation_channel m_state_channel;
	c_animation_channel m_transition_channel;
	c_interpolator_control m_animation_node_interpolator;
	int32 m_mixing_board_definition_index;
	int32 m_object_index;
	int32 m_graph_tag_index;
	uns16 m_flags;
	uns16 m_internal_flags;
	s_animation_goal_settings m_goal_settings;
	s_animation_goal_settings m_previous_goal;
	real_vector3d m_transition_delta;
};
static_assert(sizeof(c_animation_manager) == 0xC0);

