#pragma once

#include "animations/mixing_board/channels/channel_base.hpp"
#include "cseries/cseries.hpp"

class c_animation_channel :
	public c_channel_base
{
public:
	real32 m_playback_speed;
	real32 m_frame_position;
	real32 m_playback_ratio;
	c_animation_id m_additional_id[2];
	int16 m_additional_blend_animations[2]; // t_short_fixed<13>
	uns32 m_event_flags;
	uns32 m_future_event_flags;
	uns32 m_past_event_flags;
	uns8 m_state_flags;
};
static_assert(sizeof(c_animation_channel) == 0x3C);

