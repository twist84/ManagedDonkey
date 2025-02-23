#include "interface/user_interface_controller.hpp"

#include "input/input_abstraction.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x05260268, s_user_interface_controller_globals, g_user_interface_controller_globals);

HOOK_DECLARE(0x00A935C0, event_manager_button_pressed);
//HOOK_DECLARE(0x00A936B0, event_manager_tab);
HOOK_DECLARE(0x00A94B40, user_interface_controller_input_event_submit);

//.text:00A934C0 ; bool __cdecl dpad_button_tabbed(bool, byte, word)
//.text:00A93500 ; void __cdecl event_manager_alt_stick_depressed(e_controller_index, point2d const*, e_controller_component)

void __cdecl event_manager_button_pressed(e_controller_index controller_index, char gamepad_button)
{
	//INVOKE(0x00A935C0, event_manager_button_pressed, controller_index, gamepad_button);

	s_event_record event_record = {};

	event_record.type = _event_type_button_press;
	event_record.controller = controller_index;
	switch (gamepad_button)
	{
	case _button_action_left_trigger:
		event_record.component = _controller_component_button_left_trigger;
		break;
	case _button_action_right_trigger:
		event_record.component = _controller_component_button_right_trigger;
		break;
	case _button_action_dpad_up:
	case _button_action_dpad_down:
	case _button_action_dpad_left:
	case _button_action_dpad_right:
		break;
	case _button_action_start:
		event_record.component = _controller_component_button_start;
		break;
	case _button_action_back:
		event_record.component = _controller_component_button_back;
		break;
	case _button_action_left_stick:
		event_record.component = _controller_component_button_left_stick;
		break;
	case _button_action_right_stick:
		event_record.component = _controller_component_button_right_stick;
		break;
	case _button_action_a:
		event_record.component = _controller_component_button_a;
		break;
	case _button_action_b:
		event_record.component = _controller_component_button_b;
		break;
	case _button_action_x:
		event_record.component = _controller_component_button_x;
		break;
	case _button_action_y:
		event_record.component = _controller_component_button_y;
		break;
	case _button_action_right_bumper:
		event_record.component = _controller_component_button_left_bumper;
		break;
	case _button_action_left_bumper:
		event_record.component = _controller_component_button_right_bumper;
		break;
	}
	event_record.value = 0xFF;

	user_interface_controller_input_event_submit(&event_record);
}

void __cdecl event_manager_tab_fix_for_double_press(long gamepad_stick, e_controller_index controller_index, point2d const* a3, dword a4, e_controller_component controller_component)
{
}
HOOK_DECLARE_CALL(0x00A941B9, event_manager_tab_fix_for_double_press);

void __cdecl event_manager_tab(long gamepad_stick, e_controller_index controller_index, point2d const* a3, dword a4, e_controller_component controller_component)
{
	INVOKE(0x00A936B0, event_manager_tab, gamepad_stick, controller_index, a3, a4, controller_component);
}

//.text:00A93A50 ; void __cdecl event_manager_update(dword)
//.text:00A94290 ; void __cdecl send_stick_magnitude_events(e_controller_index, point2d, e_gamepad_stick)
//.text:00A94330 ; 
//.text:00A94340 ; 
//.text:00A94350 ; void __cdecl user_interface_controller_detached(e_controller_index)
//.text:00A94380 ; void __cdecl user_interface_controller_clear_voting_for_all_controllers()
//.text:00A943F0 ; bool __cdecl user_interface_controller_connected(e_controller_index)
//.text:00A94400 ; void __cdecl user_interface_controller_attached(e_controller_index)
//.text:00A94410 ; void __cdecl user_interface_controller_dispose()
//.text:00A94420 ; void __cdecl user_interface_controller_event_manager_suppress(bool)
//.text:00A94430 ; s_user_interface_controller* __cdecl user_interface_controller_get(e_controller_index)
//.text:00A94440 ; bool __cdecl user_interface_controller_get_autolevel_enabled(e_controller_index)
//.text:00A94480 ; c_flags<e_bungienet_user_flags, word, 16> __cdecl user_interface_controller_get_bungienet_user(e_controller_index)
//.text:00A944A0 ; 
//.text:00A94500 ; bool __cdecl user_interface_controller_get_extras_portal_debug(e_controller_index)
//.text:00A94520 ; e_controller_index __cdecl user_interface_controller_get_first_xbox_live_signed_in_non_guest()
//.text:00A94570 ; bool __cdecl user_interface_controller_get_griefer(e_controller_index, bool)
//.text:00A94590 ; byte __cdecl user_interface_controller_get_hopper_access(e_controller_index)
//.text:00A945B0 ; 
//.text:00A94630 ; e_controller_index __cdecl user_interface_controller_get_next_controller(e_controller_index)
//.text:00A94680 ; 
//.text:00A946A0 ; void __cdecl user_interface_controller_get_player_configuration(e_controller_index, s_player_identifier*, s_player_configuration*)
//.text:00A94780 ; wchar_t const* __cdecl user_interface_controller_get_player_profile_name(e_controller_index)
//.text:00A947C0 ; 
//.text:00A947E0 ; bool __cdecl user_interface_controller_get_rumble_enabled(e_controller_index)
//.text:00A947F0 ; 
//.text:00A94860 ; 
//.text:00A948F0 ; short __cdecl user_interface_controller_get_signed_in_live_account_count()
//.text:00A94930 ; 
//.text:00A94980 ; 
//.text:00A94A00 ; long __cdecl user_interface_controller_get_desired_team_index(e_controller_index)
//.text:00A94A30 ; long __cdecl user_interface_controller_get_user_index(e_controller_index)
//.text:00A94A50 ; void __cdecl user_interface_controller_initialize()

void __cdecl user_interface_controller_input_event_submit(s_event_record* event_record)
{
	//INVOKE(0x00A94B40, user_interface_controller_input_event_submit, event_record);

	if (input_globals.input_suppressed)
		return;

	c_window_manager* window_manager = window_manager_get();
	window_manager->handle_global_controller_event(event_record);
}

//.text:00A94B60 ; 
//.text:00A94BA0 ; bool __cdecl user_interface_controller_is_player_profile_valid(e_controller_index)
//.text:00A94BD0 ; void __cdecl user_interface_controller_reset(e_controller_index)
//.text:00A94C60 ; 
//.text:00A94CC0 ; user_interface_controller_set_armor_loadout_index
//.text:00A94CF0 ; void __cdecl user_interface_controller_set_bungienet_user(e_controller_index, c_flags<e_bungienet_user_flags, word, 16>)
//.text:00A94D30 ; void __cdecl user_interface_controller_set_desired_multiplayer_team(e_controller_index, e_game_team)
//.text:00A94D60 ; void __cdecl user_interface_controller_set_desires_rematch(e_controller_index, bool)
//.text:00A94D90 ; void __cdecl user_interface_controller_set_extras_portal_debug(e_controller_index, bool)
//.text:00A94DE0 ; void __cdecl user_interface_controller_set_griefer(e_controller_index, bool)
//.text:00A94E00 ; void __cdecl user_interface_controller_set_hopper_access(e_controller_index, byte)
//.text:00A94E40 ; void __cdecl user_interface_controller_set_desires_veto(e_controller_index, bool)
//.text:00A94E70 ; 
//.text:00A94E90 ; 
//.text:00A94EC0 ; void __cdecl user_interface_controller_set_user_index(e_controller_index, long)
//.text:00A94EE0 ; user_interface_controller_set_weapon_loadout_index
//.text:00A94F10 ; 

void __cdecl user_interface_controller_update()
{
	INVOKE(0x00A94F50, user_interface_controller_update);
}

//.text:00A95170 ; void __cdecl user_interface_controller_update_network_properties(e_controller_index)
//.text:00A95230 ; 
//.text:00A95240 ; 
//.text:00A95250 ; 
//.text:00A95260 ; bool __cdecl user_interface_controller_xbox_live_account_signed_in(e_controller_index)
//.text:00A95280 ; bool __cdecl user_interface_controller_xbox_live_is_guest_account(e_controller_index)
//.text:00A952A0 ; 
//.text:00A95300 ; void __cdecl user_interface_get_last_campaign_level(e_campaign_id*, e_map_id*)
//.text:00A95340 ; char* __cdecl user_interface_get_last_campaign_level_path(char*, long)
//.text:00A953D0 ; 
//.text:00A95420 ; void __cdecl user_interface_queue_event(s_event_record*)
//.text:00A95460 ; void __cdecl user_interface_set_last_campaign_level(e_campaign_id, e_map_id)
//.text:00A95520 ; 
//.text:00A95560 ; 

