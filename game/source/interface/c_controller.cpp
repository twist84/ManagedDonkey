#include "interface/c_controller.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0524EC48, s_controller_globals, g_controller_globals);

#if defined(_DEBUG)

// simulate added controllers
c_controller_interface* __cdecl controller_get_hook(e_controller_index controller_index)
{
	c_controller_interface* controller = controller_get(controller_index);
	if (false && !controller->is_attached())
	{
		// set attached bit
		controller->m_state_flags.set(c_controller_interface::_controller_state_flag_attached, true);
		controller->m_user_index = (short)controller_index;
		controller->m_display_name.print(L"Player%hd", controller->m_user_index + 1);
	}

	return controller;
}

// c_gui_active_roster_data::update_press_a_to_join_slots
HOOK_DECLARE_CALL(0x00B25952, controller_get_hook);

#endif // _DEBUG

c_controller_interface* __cdecl controller_get(e_controller_index controller_index)
{
	return &g_controller_globals.m_controllers[controller_index];
}

void __cdecl controllers_render()
{
	INVOKE(0x00A7D180, controllers_render);
}

bool c_controller_interface::is_attached()
{
	return m_state_flags.test(_controller_state_flag_attached);
}

bool c_controller_interface::in_use()
{
	return is_signed_in_to_machine() || m_state_flags.test(_controller_state_flag_unsigned_in_user);
}

bool c_controller_interface::is_signed_in_to_machine()
{
	return DECLFUNC(0x00A7D8A0, bool, __thiscall, c_controller_interface*)(this);
}

short c_controller_interface::get_user_index()
{
	return m_user_index;
}

void c_controller_interface::get_player_identifier(s_player_identifier* out_player_identifier)
{
	DECLFUNC(0x00A7D500, void, __thiscall, c_controller_interface*, s_player_identifier*)(this, out_player_identifier);
}

qword c_controller_interface::get_player_xuid()
{
	return DECLFUNC(0x00A7D540, qword, __thiscall, c_controller_interface*)(this);
}

