#include "interface/c_controller.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0524EC48, s_controller_globals, g_controller_globals);

bool controller_centered_crosshair = false;

c_controller_interface* __cdecl controller_get(e_controller_index controller_index)
{
	//return INVOKE(0x00A7CD50, controller_get, controller_index);

	return &g_controller_globals.m_controllers[controller_index];
}

e_window_index __cdecl controller_get_game_render_window(e_controller_index controller_index)
{
	return INVOKE(0x00A7CE80, controller_get_game_render_window, controller_index);

	//ASSERT(VALID_CONTROLLER(controller_index));
	//
	//return controller_game_render_windows[controller_index];
}

e_controller_index __cdecl controller_index_from_output_user_index(e_output_user_index output_user_index)
{
	return INVOKE(0x00A7CF00, controller_index_from_output_user_index, output_user_index);
}

void __cdecl controllers_render()
{
	INVOKE(0x00A7D180, controllers_render);
}

bool __cdecl controller_has_centered_crosshair(e_controller_index controller_index)
{
	return controller_centered_crosshair;
}

bool c_controller_interface::is_attached() const
{
	return m_state_flags.test(_controller_state_flag_attached);
}

bool c_controller_interface::in_use() const
{
	//return DECLFUNC(0x00480590, bool, __thiscall, c_controller_interface const*)(this);

	return is_signed_in_to_machine() || m_state_flags.test(_controller_state_flag_unsigned_in_user);
}

bool c_controller_interface::is_signed_in_to_machine() const
{
	return DECLFUNC(0x00A7D8A0, bool, __thiscall, c_controller_interface const*)(this);
}

short c_controller_interface::get_user_index() const
{
	return m_user_index;
}

s_player_identifier const* c_controller_interface::get_player_identifier(s_player_identifier* out_player_identifier) const
{
	return DECLFUNC(0x00A7D500, s_player_identifier const*, __thiscall, c_controller_interface const*, s_player_identifier*)(this, out_player_identifier);
}

qword const c_controller_interface::get_player_xuid() const
{
	return DECLFUNC(0x00A7D540, qword const, __thiscall, c_controller_interface const*)(this);
}

wchar_t const* c_controller_interface::get_display_name() const
{
	return m_display_name.get_string();
}

