#include "interface/c_controller.hpp"

s_controller_globals& g_controller_globals = *reinterpret_cast<s_controller_globals*>(0x0524EC48);

c_controller_interface* __cdecl controller_get(e_controller_index controller_index)
{
	return &g_controller_globals.m_controllers[controller_index];
}

bool c_controller_interface::in_use()
{
	return is_signed_in_to_machine() || TEST_BIT(m_flags, 2);
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