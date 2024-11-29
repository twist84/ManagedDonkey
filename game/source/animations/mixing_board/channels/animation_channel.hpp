#pragma once

#include "animations/mixing_board/channels/channel_base.hpp"
#include "cseries/cseries.hpp"

struct c_animation_channel :
	public c_channel_base
{
	real m_playback_speed;
	real m_frame_position;
	real m_playback_ratio;
	c_animation_id m_additional_id[2];
	short m_additional_blend_animations[2]; // t_short_fixed<13>
	dword_flags m_event_flags;
	dword_flags m_future_event_flags;
	dword_flags m_past_event_flags;
	byte_flags m_state_flags;
};
static_assert(sizeof(c_animation_channel) == 0x3C);

