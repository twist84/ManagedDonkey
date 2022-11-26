#pragma once

#include "camera/observer.hpp"

#define MAXIMUM_PLAYER_WINDOWS 4

struct c_player_render_camera_iterator
{
public:
	c_player_render_camera_iterator();
	bool next();
	long get_window_count() const;
	long get_window_arrangement() const;
	long get_user_index() const;
	s_observer_result const* get_observer_result() const;

protected:
	long m_window_count;
	long m_window_arrangement;
	long m_next;
	long m_user_index;
	s_observer_result const* m_current_observer_result;
};
static_assert(sizeof(c_player_render_camera_iterator) == 0x14);

extern void __cdecl main_render_pregame(long main_pregame_frame, char const* loading_status);
extern void __cdecl main_render_pregame_loading_screen();

