#include "animations/mixing_board/channels/animation_channel.hpp"

bool c_animation_channel::playback_complete() const
{
	return m_state_flags.test(_animation_state_playback_complete_bit);
}

