#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "shell/shell.hpp"

struct c_life_cycle_state_handler_joining : c_life_cycle_state_handler
{
	byte m_flags;
	e_gui_game_mode m_game_mode;
	e_life_cycle_state m_enter_state;
	byte __data[0x4];
};
static_assert(sizeof(c_life_cycle_state_handler_joining) == 0x38);

