#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_player_profile.hpp"
#include "text/unicode.hpp"

struct s_player_identifier;
class c_player_marketplace;
class c_controller_interface
{
public:
	bool in_use() const;
	e_controller_index get_controller_index() const;
	const wchar_t* get_display_name() const;
	int16 get_or_create_user_index();
	const s_player_identifier* get_player_identifier(s_player_identifier* out_player_identifier) const;
	c_player_profile_interface* get_player_profile_interface();
	uns64 const get_player_xuid() const;
	int16 get_user_index() const;
	bool is_attached() const;
	bool is_signed_in_to_machine() const;
	bool is_unsigned_in_user() const;

//private:
	void remove_controller_from_network_session();

//public:
	void reset_user_index();

//private:
	void sign_in_controller(const s_player_identifier* player_identifier, bool is_temporary);
	void sign_out_controller(bool sign_out_for_sign_in_change);
	void update_controller_properties();
	void update_for_sign_in_change();

//protected:
	// 4066
	// 0000 1111 1110 0010
	enum e_controller_state_flags
	{
		_attached_bit = 0,
		_has_voice_bit,
		_temporary_bit,
		_storage_device_selection_performed_bit,

		_online_communications_allowed_everyone_bit,
		_online_communications_allowed_friends_only_bit,

		_online_profile_viewing_allowed_everyone_bit,
		_online_profile_viewing_allowed_friends_only_bit,

		_user_created_content_allowed_everyone_bit,
		_user_created_content_allowed_friends_only_bit,

		_online_content_purchase_allowed_bit,

		_online_presence_allowed_everyone_bit,
		_online_presence_allowed_friends_only_bit,

		k_number_of_controller_state_flags
	};

	c_flags<e_controller_state_flags, int16, k_number_of_controller_state_flags> m_state_flags;
	int16 m_user_index;
	uns32 __unknown4;
	c_player_profile_interface m_player_profile;
	wchar_t m_display_name[16];
	uns64 m_hash_bits;
	uns64 m_hash_bits_last_game_instance;
	c_static_flags<k_achievement_count> m_achievements;
	c_player_marketplace* m_marketplace;
	uns32 m_time_controller_signed_out;
	byte __data1440[0x1];
	uns8 vidmaster;
	byte __data1442[0x6];
};
static_assert(sizeof(c_controller_interface) == 0x1448);

struct s_controller_globals
{
	c_controller_interface m_controllers[k_number_of_controllers];
};
static_assert(sizeof(s_controller_globals) == 0x5120);

extern s_controller_globals& g_controller_globals;
extern bool controller_centered_crosshair;

extern void __cdecl controller_activate_as_unsigned_in_user(int16 controller_index, bool activate);
extern void __cdecl controller_display_storage_device_selection(int16 controller_index);
extern c_controller_interface* __cdecl controller_get(e_controller_index controller_index);
extern e_controller_index __cdecl controller_get_first_non_guest_signed_in_controller();
extern e_controller_index __cdecl controller_get_first_signed_in_controller();
extern e_window_index __cdecl controller_get_game_render_window(e_controller_index controller_index);
extern e_controller_index __cdecl controller_index_from_user_index(int32 user_index);
extern void __cdecl controllers_render();
extern bool __cdecl controller_has_centered_crosshair(e_controller_index controller_index);

extern void debug_set_background_emblem_color(int16 controller_index, int16 color);
extern void debug_set_button_preset(int16 controller_index, int16 preset);
extern void debug_set_controller_auto_center_look(int16 controller_index, bool auto_center);
extern void debug_set_controller_crouch_lock(int16 controller_index, bool crouch_lock);
extern void debug_set_controller_flight_stick_aircraft_controls(int16 controller_index, bool aircraft_controls);
extern void debug_set_controller_look_inverted(int16 controller_index, bool invert);
extern void debug_set_controller_vibration_enabled(int16 controller_index, bool vibrate);
extern void debug_set_emblem_info(int16 controller_index, int16 foreground_emblem_index, int16 background_emblem_index);
extern void debug_set_joystick_preset(int16 controller_index, int16 preset);
extern void debug_set_look_sensitivity(int16 controller_index, int16 sensitivity);
extern void debug_set_player_character_type(int16 controller_index, int16 type);
extern void debug_set_popup_message_index(int16 controller_index, int32 message_index);
extern void debug_set_primary_change_color(int16 controller_index, int16 color);
extern void debug_set_primary_emblem_color(int16 controller_index, int16 color);
extern void debug_set_secondary_change_color(int16 controller_index, int16 color);
extern void debug_set_secondary_emblem_color(int16 controller_index, int16 color);
extern void debug_set_single_player_level_completed(int16 controller_index, int32 level_index, bool coop, int16 difficulty, bool completed);
extern void debug_set_single_player_level_unlocked(int16 controller_index, int16 level_index, bool unlocked);
extern void debug_set_subtitle_setting(int16 controller_index, int16 setting);
extern void debug_set_tertiary_change_color(int16 controller_index, int16 color);
extern void debug_set_voice_mask(int16 controller_index, int16 mask);
extern void debug_set_voice_output_setting(int16 controller_index, int16 setting);

