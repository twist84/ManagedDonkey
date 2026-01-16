#pragma once

#include "cseries/cseries.hpp"

enum e_emblem_info_flags
{
	_emblem_info_flag_alternate_foreground_channel_off = 0,
	_emblem_info_flag_flip_foreground,
	_emblem_info_flag_flip_background,

	k_emblem_info_flags_count
};

class c_bitstream;
struct s_emblem_info
{
	s_emblem_info() :
		foreground_emblem_index(),
		background_emblem_index(),
		emblem_info_flags(),
		primary_color_index(),
		secondary_color_index(),
		background_color_index(),
		pad(0)
	{
	}

	void __thiscall decode(c_bitstream* packet);
	void __thiscall encode(c_bitstream* packet);
	void __thiscall encode_const(c_bitstream* packet) const;

	uns8 foreground_emblem_index;
	uns8 background_emblem_index;
	c_flags<e_emblem_info_flags, uns8, k_emblem_info_flags_count> emblem_info_flags;
	c_enum<e_player_color_index, int8, _player_color_none, k_player_color_index_count> primary_color_index;
	c_enum<e_player_color_index, int8, _player_color_none, k_player_color_index_count> secondary_color_index;
	c_enum<e_player_color_index, int8, _player_color_none, k_player_color_index_count> background_color_index;
	uns16 pad;
};
COMPILE_ASSERT(sizeof(s_emblem_info) == 0x8);

extern s_emblem_info g_user_interface_emblem_info;

extern s_emblem_info* emblem_get_render_constants_emblem_info_from_user_interface(s_emblem_info* emblem_info);
extern bool emblem_set_render_constants_for_local_user(e_controller_index controller_index, int16* foreground_emblem, int16* background_emblem);
extern bool emblem_set_render_constants_from_user_interface(const s_emblem_info* emblem_info);

