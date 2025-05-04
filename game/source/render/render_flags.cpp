#include "render/render_flags.hpp"

REFERENCE_DECLARE(0x0524B6D4, uns32, c_render_flags::g_this_frame);
REFERENCE_DECLARE(0x0524B6D8, uns32, c_render_flags::g_last_frame);
REFERENCE_DECLARE(0x0524B6DC, uns32, c_render_flags::g_this_window);
REFERENCE_DECLARE(0x0524B6E0, uns32, c_render_flags::g_last_window);
REFERENCE_DECLARE_ARRAY(0x0524B6E4, uns32, c_render_flags::g_last_windows, 4);
REFERENCE_DECLARE(0x0524B6F4, uns32, c_render_flags::g_active_player_window_index);

void __cdecl c_render_flags::prepare_for_frame()
{
	INVOKE(0x00A29680, c_render_flags::prepare_for_frame);

	//c_render_flags::g_last_frame = c_render_flags::g_this_frame;
	//c_render_flags::g_this_frame = 0;
}

void __cdecl c_render_flags::prepare_for_player_window(int32 player_window_index)
{
	INVOKE(0x00A296A0, c_render_flags::prepare_for_player_window, player_window_index);

	//ASSERT(VALID_INDEX(player_window_index, MAXIMUM_PLAYER_WINDOWS));
	//
	//c_render_flags::g_last_windows[c_render_flags::g_active_player_window_index] = c_render_flags::g_this_window;
	//c_render_flags::g_active_player_window_index = player_window_index;
	//
	//c_render_flags::g_last_window = c_render_flags::g_last_windows[c_render_flags::g_active_player_window_index];
	//c_render_flags::g_this_window = 0;
}

void __cdecl c_render_flags::set_frame_bit(e_frame_bit frame_bit, uns32 flags)
{
	INVOKE(0x00A3B760, c_render_flags::set_frame_bit, frame_bit, flags);

	//SET_BIT(c_render_flags::g_this_frame, frame_bit, flags);
}

bool __cdecl c_render_flags::test_frame_bit(e_frame_bit frame_bit)
{
	return INVOKE(0x00A3BF90, c_render_flags::test_frame_bit, frame_bit);

	//return TEST_BIT(c_render_flags::g_this_frame, frame_bit);
}

bool __cdecl c_render_flags::test_last_window_bit(e_window_bit window_bit)
{
	return INVOKE(0x00A3BFB0, c_render_flags::test_last_window_bit, window_bit);

	//return TEST_BIT(c_render_flags::g_last_window, window_bit);
}

void __cdecl c_render_flags::or_window_bit(e_window_bit window_bit, uns32 flags)
{
	INVOKE(0x00A781A0, c_render_flags::or_window_bit, window_bit, flags);

	//c_render_flags::g_this_window |= (flags & 1) << window_bit;
}

