#pragma once

#include "interface/c_controller.hpp"

enum e_event_type
{
	_event_type_none = 0,
	_event_type_tab_up,
	_event_type_tab_left,
	_event_type_tab_down,
	_event_type_tab_right,
	_event_type_alt_stick_up,
	_event_type_alt_stick_left,
	_event_type_alt_stick_down,
	_event_type_alt_stick_right,
	_event_type_alt_tab_up,
	_event_type_alt_tab_left,
	_event_type_alt_tab_down,
	_event_type_alt_tab_right,
	_event_type_button_press,

	k_event_type_count
};

enum e_controller_component
{
	_controller_component_button_a = 0,
	_controller_component_button_b,
	_controller_component_button_x,
	_controller_component_button_y,

	_controller_component_button_left_shoulder,
	_controller_component_button_right_shoulder,

	_controller_component_button_left_trigger,
	_controller_component_button_right_trigger,

	_controller_component_button_dpad_up,
	_controller_component_button_dpad_down,
	_controller_component_button_dpad_left,
	_controller_component_button_dpad_right,

	_controller_component_button_start,
	_controller_component_button_back,

	_controller_component_button_left_thumb,
	_controller_component_button_right_thumb,

	_controller_component_any_stick,

	_controller_component_unknown17,

	_controller_component_left_stick_left,
	_controller_component_left_stick_right,
	_controller_component_left_stick_up,
	_controller_component_left_stick_down,

	_controller_component_right_stick_left,
	_controller_component_right_stick_right,
	_controller_component_right_stick_up,
	_controller_component_right_stick_down,

	_controller_component_left_stick_x,
	_controller_component_left_stick_y,

	_controller_component_right_stick_x,
	_controller_component_right_stick_y,
};

struct s_event_record
{
	e_event_type type;
	e_controller_index controller;
	e_controller_component component;
	int16 value;
};
static_assert(sizeof(s_event_record) == 0x10);

enum e_bungienet_user_flags
{
	_bungienet_user_registered_bit = 0,
	_bungienet_user_pro_member_bit,
	_bungienet_user_staff_bit,
	_bungienet_user_community0_bit,
	_bungienet_user_community1_bit,
	_bungienet_user_community2_bit,

	k_bungienet_user_flag_count,

	_bungienet_user_is_blue_disk_bit = 31
};

struct s_user_interface_controller
{
	uint32 bungienet_user_flags;
	uint8 hopper_access_flags;
	bool extras_portal_debug;

	uint8 __unknown6;

	bool desires_veto;
	int32 armor_loadout_index;
	int32 weapon_loadout_index;
	bool desires_rematch;
	bool griefer;
	bool notification_pending;
	uint32 online_presence_flags;
	bool online_session_id_valid;
	//__declspec(align(1)) s_transport_secure_identifier online_session_id;
	uint8 online_session_id[0x10];
};
static_assert(sizeof(s_user_interface_controller) == 0x2C);
static_assert(0x00 == OFFSETOF(s_user_interface_controller, bungienet_user_flags));
static_assert(0x04 == OFFSETOF(s_user_interface_controller, hopper_access_flags));
static_assert(0x05 == OFFSETOF(s_user_interface_controller, extras_portal_debug));
static_assert(0x06 == OFFSETOF(s_user_interface_controller, __unknown6));
static_assert(0x07 == OFFSETOF(s_user_interface_controller, desires_veto));
static_assert(0x08 == OFFSETOF(s_user_interface_controller, armor_loadout_index));
static_assert(0x0C == OFFSETOF(s_user_interface_controller, weapon_loadout_index));
static_assert(0x10 == OFFSETOF(s_user_interface_controller, desires_rematch));
static_assert(0x11 == OFFSETOF(s_user_interface_controller, griefer));
static_assert(0x12 == OFFSETOF(s_user_interface_controller, notification_pending));
static_assert(0x14 == OFFSETOF(s_user_interface_controller, online_presence_flags));
static_assert(0x18 == OFFSETOF(s_user_interface_controller, online_session_id_valid));
static_assert(0x19 == OFFSETOF(s_user_interface_controller, online_session_id));

struct s_user_interface_controller_globals
{
	s_user_interface_controller controllers[k_number_of_controllers];
	s_event_record queued_events[k_number_of_controllers];
	bool controller_removed_dialog_required[k_number_of_controllers];
	bool suppressed;
};
static_assert(sizeof(s_user_interface_controller_globals) == 0xF8);

extern s_user_interface_controller_globals& g_user_interface_controller_globals;

extern void __cdecl event_manager_button_pressed(e_controller_index controller_index, uint8 button);
extern void __cdecl event_manager_tab(int32 gamepad_stick, e_controller_index controller_index, point2d const* vector, uint32 now, e_controller_component component);
extern void __cdecl user_interface_controller_attached(e_controller_index controller_index);
extern void __cdecl user_interface_controller_detached(e_controller_index controller_index);
extern s_user_interface_controller* __cdecl user_interface_controller_get(e_controller_index controller_index);
extern bool __cdecl user_interface_controller_get_player_configuration(e_controller_index controller_index, s_player_identifier* player_identifier, s_player_configuration* player_data);
extern void __cdecl user_interface_controller_input_event_submit(s_event_record* event);
extern bool __cdecl user_interface_controller_is_player_profile_valid(e_controller_index controller_index);
extern void __cdecl user_interface_controller_reset(e_controller_index controller_index);
extern void __cdecl user_interface_controller_set_user_index(e_controller_index controller_index, int32 user_index);
extern void __cdecl user_interface_controller_switch_to_offline(e_controller_index controller_index);
extern void __cdecl user_interface_controller_update();
extern void __cdecl user_interface_controller_update_network_properties(e_controller_index controller_index);

