#pragma once

#include "animations/animation_id.hpp"
#include "animations/mixing_board/channels/channel_base.hpp"
#include "cseries/cseries.hpp"

enum e_animation_frame_event_types
{
	_animation_frame_event_primary_key_frame_bit = 0,
	_animation_frame_event_secondary_key_frame_bit,
	_animation_frame_event_left_foot_bit,
	_animation_frame_event_right_foot_bit,
	_animation_frame_event_allow_interruption_bit,
	_animation_frame_event_transition_frame_a_bit,
	_animation_frame_event_transition_frame_b_bit,
	_animation_frame_event_transition_frame_c_bit,
	_animation_frame_event_transition_frame_d_bit,
	_animation_frame_event_both_feet_shuffle_bit,
	_animation_frame_event_body_impact_bit,
	_animation_frame_event_left_back_foot_bit,
	_animation_frame_event_right_back_foot_bit,

	k_total_animation_frame_event_types,
};
typedef c_flags<e_animation_frame_event_types, uns32, k_total_animation_frame_event_types> c_animation_frame_event_flags;

enum e_animation_state_flag_bits
{
	_animation_state_paused_bit = 0,
	_animation_state_reached_last_frame_bit,
	_animation_state_playback_complete_bit,
	_animation_state_change_requested_bit,
	_animation_state_active_bit,
	_animation_state_blending_locked_bit,

	k_number_of_animation_state_bits,
};
typedef c_flags<e_animation_state_flag_bits, uns8, k_number_of_animation_state_bits> c_animation_state_flags;

class c_animation_channel :
	public c_channel_base
{
public:
	bool playback_complete() const;

	real32 m_playback_speed;
	real32 m_frame_position;
	real32 m_playback_ratio;
	c_animation_id m_additional_id[2];
	int16 m_additional_blend_animations[2]; // t_short_fixed<13>
	c_animation_frame_event_flags m_event_flags;
	c_animation_frame_event_flags m_future_event_flags;
	c_animation_frame_event_flags m_past_event_flags;
	c_animation_state_flags m_state_flags;
};
static_assert(sizeof(c_animation_channel) == 0x3C);

