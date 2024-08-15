#pragma once

#include "animations/mixing_board/channels/channel_base.hpp"
#include "cseries/cseries.hpp"

struct c_animation_channel :
	public c_channel_base
{
	real __unknown14;

	real m_frame_position;
	real m_playback_ratio;

	struct
	{
		c_animation_id __unknown0[2];
		short __unknown8;
	} m_additional_blend_animations;

	dword_flags __flags2C;
	dword_flags __flags30;
	dword_flags __flags34;
	byte __flags38;
	byte __data39[0x3];
};
static_assert(sizeof(c_animation_channel) == 0x3C);

