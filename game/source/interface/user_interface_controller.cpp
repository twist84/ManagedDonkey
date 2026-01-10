#include "interface/user_interface_controller.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "input/input_abstraction.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"

REFERENCE_DECLARE(0x05260268, s_user_interface_controller_globals, g_user_interface_controller_globals);

HOOK_DECLARE(0x00A935C0, event_manager_button_pressed);
//HOOK_DECLARE(0x00A936B0, event_manager_tab);
HOOK_DECLARE(0x00A94350, user_interface_controller_attached);
HOOK_DECLARE(0x00A94400, user_interface_controller_detached);
HOOK_DECLARE(0x00A94B40, user_interface_controller_input_event_submit);
HOOK_DECLARE(0x00A94BD0, user_interface_controller_reset);
HOOK_DECLARE(0x00A95170, user_interface_controller_update_network_properties);

//.text:00A934C0 ; bool __cdecl dpad_button_tabbed(bool, uns8, uns16)
//.text:00A93500 ; void __cdecl event_manager_alt_stick_depressed(e_controller_index, const point2d*, e_controller_component)

void __cdecl event_manager_button_pressed(e_controller_index controller_index, uns8 button)
{
	//INVOKE(0x00A935C0, event_manager_button_pressed, controller_index, gamepad_button);

	s_event_record event = {};

	event.type = _event_type_button_press;
	event.controller = controller_index;
	switch (button)
	{
	case _button_left_trigger:
		event.component = _controller_component_button_left_trigger;
		break;
	case _button_right_trigger:
		event.component = _controller_component_button_right_trigger;
		break;
	case _button_start:
		event.component = _controller_component_button_start;
		break;
	case _button_back:
		event.component = _controller_component_button_back;
		break;
	case _button_left_stick:
		event.component = _controller_component_button_left_thumb;
		break;
	case _button_right_stick:
		event.component = _controller_component_button_right_thumb;
		break;
	case _button_a:
		event.component = _controller_component_button_a;
		break;
	case _button_b:
		event.component = _controller_component_button_b;
		break;
	case _button_x:
		event.component = _controller_component_button_x;
		break;
	case _button_y:
		event.component = _controller_component_button_y;
		break;
	case _button_right_bumper:
		event.component = _controller_component_button_right_shoulder;
		break;
	case _button_left_bumper:
		event.component = _controller_component_button_left_shoulder;
		break;
	}
	event.value = 0xFF;

	user_interface_controller_input_event_submit(&event);
}

void __cdecl event_manager_tab_fix_for_double_press(int32 gamepad_stick, e_controller_index controller_index, const point2d* vector, uns32 now, e_controller_component component)
{
}
HOOK_DECLARE_CALL(0x00A941B9, event_manager_tab_fix_for_double_press);

void __cdecl event_manager_tab(int32 gamepad_stick, e_controller_index controller_index, const point2d* vector, uns32 now, e_controller_component component)
{
	INVOKE(0x00A936B0, event_manager_tab, gamepad_stick, controller_index, vector, now, component);
}

//.text:00A93A50 ; void __cdecl event_manager_update(uns32)
//.text:00A94290 ; void __cdecl send_stick_magnitude_events(e_controller_index, point2d, e_gamepad_stick)
//.text:00A94330 ; 
//.text:00A94340 ; 

void __cdecl user_interface_controller_attached(e_controller_index controller_index)
{
	//INVOKE(0x00A94350, user_interface_controller_attached, controller_index);

	g_user_interface_controller_globals.controller_removed_dialog_required[controller_index] = false;
	user_interface_error_manager_get()->clear_error(STRING_ID(gui_alert, controller_detached), controller_index);
	user_interface_controller_update_network_properties(controller_index);
}

//.text:00A94380 ; void __cdecl user_interface_controller_clear_voting_for_all_controllers()
//.text:00A943F0 ; bool __cdecl user_interface_controller_connected(e_controller_index)

void __cdecl user_interface_controller_detached(e_controller_index controller_index)
{
	//INVOKE(0x00A94400, user_interface_controller_detached, controller_index);

	if (!user_interface_controller_is_player_profile_valid(controller_index) ||
		//user_interface_automation_is_active() ||
		controller_get(controller_index)->is_unsigned_in_user()
		&& game_in_progress() &&
		game_is_ui_shell())
	{
		g_user_interface_controller_globals.controller_removed_dialog_required[controller_index] = false;
	}
	else
	{
		g_user_interface_controller_globals.controller_removed_dialog_required[controller_index] = true;
		//if (!network_globals.disable_detatched_controller_check)
		if (false)
		{
			user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, controller_detached), controller_index, true);
		}
	}
	user_interface_controller_update_network_properties(controller_index);
}

void __cdecl user_interface_controller_dispose()
{
	INVOKE(0x00A94410, user_interface_controller_dispose);
}

//.text:00A94420 ; void __cdecl user_interface_controller_event_manager_suppress(bool)

s_user_interface_controller* __cdecl user_interface_controller_get(e_controller_index controller_index)
{
	return INVOKE(0x00A94430, user_interface_controller_get, controller_index);

	//return &g_user_interface_controller_globals.controllers[controller_index];
}

//.text:00A94440 ; bool __cdecl user_interface_controller_get_autolevel_enabled(e_controller_index)
//.text:00A94480 ; c_flags<e_bungienet_user_flags, uns16, 16> __cdecl user_interface_controller_get_bungienet_user(e_controller_index)
//.text:00A944A0 ; e_controller_index __cdecl user_interface_controller_get_controller_for_user(int32)
//.text:00A94500 ; bool __cdecl user_interface_controller_get_extras_portal_debug(e_controller_index)
//.text:00A94520 ; e_controller_index __cdecl user_interface_controller_get_first_xbox_live_signed_in_non_guest()
//.text:00A94570 ; bool __cdecl user_interface_controller_get_griefer(e_controller_index, bool)
//.text:00A94590 ; uns8 __cdecl user_interface_controller_get_hopper_access(e_controller_index)
//.text:00A945B0 ; int16 __cdecl user_interface_controller_get_live_account_controllers_mask()
//.text:00A94630 ; e_controller_index __cdecl user_interface_controller_get_next_controller(e_controller_index)
//.text:00A94680 ; bool __cdecl user_interface_controller_get_notification_pending(e_controller_index)

bool __cdecl user_interface_controller_get_player_configuration(e_controller_index controller_index, s_player_identifier* player_identifier, s_player_configuration* player_data)
{
	return INVOKE(0x00A946A0, user_interface_controller_get_player_configuration, controller_index, player_identifier, player_data);
}

//.text:00A94780 ; const wchar_t* __cdecl user_interface_controller_get_player_profile_name(e_controller_index)
//.text:00A947C0 ; user_interface_controller_get_? // offset 0x6
//.text:00A947E0 ; bool __cdecl user_interface_controller_get_rumble_enabled(e_controller_index)
//.text:00A947F0 ; int16 __cdecl user_interface_controller_get_signed_in_controller_count()
//.text:00A94860 ; int16 __cdecl user_interface_controller_get_signed_in_controllers_mask()
//.text:00A948F0 ; int16 __cdecl user_interface_controller_get_signed_in_live_account_count()
//.text:00A94930 ; int16 __cdecl user_interface_controller_get_signed_in_master_live_account_count()
//.text:00A94980 ; int16 __cdecl user_interface_controller_get_signed_in_non_guest_controller_count()

int32 __cdecl user_interface_controller_get_team_index(e_controller_index controller_index)
{
	//return INVOKE(0x00A94A00, user_interface_controller_get_team_index, controller_index);

	int32 team_index = NONE;

	c_controller_interface* controller_interface = controller_get(controller_index);
	int32 user_index = controller_interface->get_user_index();
	if (user_index != NONE)
	{
		team_index = network_session_interface_get_team_index(user_index);
	}

	return team_index;
}

//.text:00A94A30 ; int32 __cdecl user_interface_controller_get_user_index(e_controller_index)
//.text:00A94A50 ; void __cdecl user_interface_controller_initialize()

void __cdecl user_interface_controller_input_event_submit(s_event_record* event)
{
	//INVOKE(0x00A94B40, user_interface_controller_input_event_submit, event);

	if (input_globals.input_suppressed)
		return;

	c_window_manager* window_manager = window_manager_get();
	window_manager->handle_global_controller_event(event);
}

//.text:00A94B60 ; bool __cdecl user_interface_controller_is_anyone_signed_into_xbox_live()

bool __cdecl user_interface_controller_is_player_profile_valid(e_controller_index controller_index)
{
	//return INVOKE(0x00A94BA0, user_interface_controller_is_player_profile_valid, controller_index);

	return controller_get(controller_index)->in_use();
}

void __cdecl user_interface_controller_reset(e_controller_index controller_index)
{
	//INVOKE(0x00A94BD0, user_interface_controller_reset, controller_index);

	c_controller_interface* controller_interface = controller_get(controller_index);
	if (user_interface_controller_is_player_profile_valid(controller_index))
	{
		event(_event_message, "ui:controller: controller #%d signed out",
			controller_index);
	}
	user_interface_controller_switch_to_offline(controller_index);
	if (controller_interface->in_use())
		network_session_interface_remove_local_user(controller_interface->get_user_index());
	if (controller_interface->is_unsigned_in_user())
		controller_activate_as_unsigned_in_user(controller_index, false);
	controller_interface->reset_user_index();
}

//.text:00A94C60 ; void __cdecl user_interface_controller_reset_any_offline_controllers()
//.text:00A94CC0 ; user_interface_controller_set_armor_loadout_index
//.text:00A94CF0 ; void __cdecl user_interface_controller_set_bungienet_user(e_controller_index, c_flags<e_bungienet_user_flags, uns16, 16>)
//.text:00A94D30 ; void __cdecl user_interface_controller_set_desired_multiplayer_team(e_controller_index, e_game_team)
//.text:00A94D60 ; void __cdecl user_interface_controller_set_desires_rematch(e_controller_index, bool)
//.text:00A94D90 ; void __cdecl user_interface_controller_set_extras_portal_debug(e_controller_index, bool)
//.text:00A94DE0 ; void __cdecl user_interface_controller_set_griefer(e_controller_index, bool)
//.text:00A94E00 ; void __cdecl user_interface_controller_set_hopper_access(e_controller_index, uns8)
//.text:00A94E40 ; void __cdecl user_interface_controller_set_desires_veto(e_controller_index, bool)
//.text:00A94E70 ; void __cdecl user_interface_controller_set_notification_pending(e_controller_index, bool)
//.text:00A94E90 ; user_interface_controller_set_? // offset 0x6

void __cdecl user_interface_controller_set_user_index(e_controller_index controller_index, int32 user_index)
{
	INVOKE(0x00A94EC0, user_interface_controller_set_user_index, controller_index, user_index);
}

//.text:00A94EE0 ; user_interface_controller_set_weapon_loadout_index

void __cdecl user_interface_controller_switch_to_offline(e_controller_index controller_index)
{
	INVOKE(0x00A94F10, user_interface_controller_switch_to_offline, controller_index);
}

void __cdecl user_interface_controller_update()
{
	INVOKE(0x00A94F50, user_interface_controller_update);
}

void __cdecl user_interface_controller_update_network_properties(e_controller_index controller_index)
{
	//INVOKE(0x00A95170, user_interface_controller_update_network_properties, controller_index);

	const c_controller_interface* controller_interface = controller_get(controller_index);
	int16 user_index = controller_interface->get_user_index();
	if (user_index != NONE && network_session_interface_local_user_exists(user_index))
	{
		s_player_configuration player_data;
		uns32 player_voice_settings = 0; // voice_get_controller_settings(controller_index);
		s_player_identifier network_user_identifier;
		if (user_interface_controller_get_player_configuration(controller_index, &network_user_identifier, &player_data))
		{
			// $TODO `network_user_identifier` is set from the 
			//s_player_identifier controller_user_identifier;
			//ASSERT(csmemcmp(&controller_user_identifier, &network_user_identifier, sizeof(s_player_identifier)) == 0);

			network_session_interface_set_local_user_properties(user_index, controller_index, &player_data, player_voice_settings);
		}
		else
		{
			csmemset(&network_user_identifier, 0, sizeof(network_user_identifier));
			event(_event_warning, "ui:controller: controller #%d had no session local user but tried to set network properties",
				controller_index);
		}
	}
}

//.text:00A95230 ; bool __cdecl user_interface_controller_voice_allowed(e_controller_index)
//.text:00A95240 ; bool __cdecl user_interface_controller_voice_is_for_friends_only(e_controller_index)
//.text:00A95250 ; void __cdecl user_interface_controller_xbox_live_account_set_signed_in(e_controller_index, bool)
//.text:00A95260 ; bool __cdecl user_interface_controller_xbox_live_account_signed_in(e_controller_index)
//.text:00A95280 ; bool __cdecl user_interface_controller_xbox_live_is_guest_account(e_controller_index)
//.text:00A952A0 ; void __cdecl user_interface_controllers_perform_storage_device_selection_checks()
//.text:00A95300 ; void __cdecl user_interface_get_last_campaign_level(e_campaign_id*, e_map_id*)
//.text:00A95340 ; char* __cdecl user_interface_get_last_campaign_level_path(char*, int32)
//.text:00A953D0 ; int32 __cdecl user_interface_get_player_last_used_hopper(e_controller_index)
//.text:00A95420 ; void __cdecl user_interface_queue_event(s_event_record*)

void __cdecl user_interface_set_last_campaign_level(e_campaign_id campaign_id, e_map_id map_id)
{
	INVOKE(0x00A95460, user_interface_set_last_campaign_level, campaign_id, map_id);
}

//.text:00A95520 ; void __cdecl user_interface_set_player_last_used_hopper(e_controller_index, uns16)
//.text:00A95560 ; 

