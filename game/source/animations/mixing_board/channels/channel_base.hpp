#pragma once

#include "cseries/cseries.hpp"

struct c_base_channel_settings
{
	long m_model_animation_graph_index;

	long __unknown4;

	c_animation_id m_animation_id;
	word_flags m_animation_playback_flags;
	char m_animation_graph_location;

	byte __unknownF; // pad?
};
static_assert(sizeof(c_base_channel_settings) == 0x10);

struct c_channel_base
{
	// is this `c_base_channel_settings`
	long m_model_animation_graph_index;
	long __unknown4;
	c_animation_id m_animation_id;
	word_flags m_animation_playback_flags;
	char m_animation_graph_location;
	byte __unknownF;

	byte __unknown10;
	byte __unknown11;
	byte __data12[0x2];
};
static_assert(sizeof(c_channel_base) == 0x14);

