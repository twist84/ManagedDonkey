#pragma once

#include "cseries/cseries.hpp"

struct s_start_menu_breadcrumb
{
	long __unknown0;
	long __unknown4;
};
static_assert(sizeof(s_start_menu_breadcrumb) == 0x8);

struct c_controller_input_message;
enum e_controller_index;
struct s_player_identifier;
struct s_service_record_identity;

struct c_start_menu_screen_widget
{
	bool __thiscall handle_controller_input_message(c_controller_input_message* input_message);
	static bool __cdecl handle_global_start_button_press(c_controller_input_message* input_message);
	static bool __cdecl load_start_menu(e_controller_index controller_index, s_player_identifier const* player_identifier, qword const* player_xuid, s_service_record_identity const* identity, s_start_menu_breadcrumb const* breadcrumbs, long breadcrumb_count);
};
