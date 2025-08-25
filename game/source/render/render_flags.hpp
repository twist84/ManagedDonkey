#pragma once

#include "cseries/cseries.hpp"

class c_render_flags
{
public:
	enum e_frame_bit
	{
		_frame_bit0 = 0,

		// $TODO find all bits
	};
	
	enum e_window_bit
	{
		_window_bit0 = 0,

		// $TODO find all bits
	};

	static void __cdecl prepare_for_frame();
	static void __cdecl prepare_for_player_window(int32 player_window_index);
	static void __cdecl set_frame_bit(e_frame_bit frame_bit, uns32 flags);
	static bool __cdecl test_frame_bit(e_frame_bit frame_bit);
	static bool __cdecl test_last_window_bit(e_window_bit window_bit);
	static void __cdecl or_window_bit(e_window_bit window_bit, uns32 flags);

	static uns32& g_this_frame;
	static uns32& g_last_frame;
	static uns32& g_this_window;
	static uns32& g_last_window;
	static uns32(&g_last_windows)[4];
	static uns32& g_active_player_window_index;
};

