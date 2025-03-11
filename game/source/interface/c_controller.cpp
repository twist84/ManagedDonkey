#include "interface/c_controller.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0524EC48, s_controller_globals, g_controller_globals);

bool controller_centered_crosshair = false;

bool c_controller_interface::in_use() const
{
	//return INVOKE_CLASS_MEMBER(0x00480590, c_controller_interface, in_use);

	return is_signed_in_to_machine() || m_state_flags.test(_temporary_bit);
}

//.text:0074D520 ; public: bool __cdecl c_controller_interface::get_always_show_device_selector_ui() const
//.text:0074D5A0 ; public: bool __cdecl c_controller_interface::get_storage_device_selection_performed()
//.text:00A7CBA0 ; public: __cdecl c_controller_interface::c_controller_interface()
//.text:00A7CC20 ; public: __cdecl c_signed_in_users_watcher::c_signed_in_users_watcher(c_signed_in_users_watcher::e_watched_state)
//.text:00A7CC50 ; public: __cdecl s_controller_globals::s_controller_globals()
//.text:00A7CC70 ; public: __cdecl c_controller_interface::~c_controller_interface()
//.text:00A7CC80 ; public: __cdecl c_player_profile_interface::~c_player_profile_interface()
//.text:00A7CC90 ; public: __cdecl s_controller_globals::~s_controller_globals()
//.text:00A7CCB0 ; 
//.text:00A7CCC0 ; 
//.text:00A7CCD0 ; void __cdecl controller_activate_as_unsigned_in_user(short, bool)
//.text:00A7CD30 ; void __cdecl controller_display_storage_device_selection(short)

c_controller_interface* __cdecl controller_get(e_controller_index controller_index)
{
	//return INVOKE(0x00A7CD50, controller_get, controller_index);

	return &g_controller_globals.m_controllers[controller_index];
}

//.text:00A7CD70 ; public: void __cdecl c_controller_interface::controller_get_achievements(s_launch_and_dlc_achievment_bit_vector*) const
//.text:00A7CD90 ; e_controller_index __cdecl controller_get_first_non_guest_signed_in_controller()
//.text:00A7CE20 ; e_controller_index __cdecl controller_get_first_signed_in_controller()

e_window_index __cdecl controller_get_game_render_window(e_controller_index controller_index)
{
	return INVOKE(0x00A7CE80, controller_get_game_render_window, controller_index);

	//ASSERT(VALID_CONTROLLER(controller_index));
	//
	//return controller_game_render_windows[controller_index];
}

//.text:00A7CE90 ; protected: void __cdecl c_controller_interface::controller_grant_achievement(e_achievement)
//.text:00A7CEC0 ; public: bool __cdecl c_controller_interface::controller_has_achievement(e_achievement) const

bool __cdecl controller_has_centered_crosshair(e_controller_index controller_index)
{
	return controller_centered_crosshair;
}

e_controller_index __cdecl controller_index_from_user_index(long user_index)
{
	return INVOKE(0x00A7CF00, controller_index_from_user_index, user_index);
}

//.text:00A7CF80 ; protected: void __cdecl c_controller_interface::controller_reset_achievements()
//.text:00A7CF90 ; void __cdecl controllers_dispose()
//.text:00A7CFD0 ; void __cdecl controllers_initialize()
//.text:00A7D050 ; void __cdecl controllers_process_messages()

void __cdecl controllers_render()
{
	INVOKE(0x00A7D180, controllers_render);
}

//.text:00A7D1C0 ; void __cdecl controllers_update(dword)
//.text:00A7D290 ; public: void __cdecl c_controller_interface::dispose()
//.text:00A7D2A0 ; private: void __cdecl c_controller_interface::generate_random_display_name()
//.text:00A7D360 ; // vidmaster related
//.text:00A7D380 ; public: long __cdecl c_controller_interface::get_button_down_frames(e_gamepad_button)
//.text:00A7D3E0 ; public: c_content_catalogue* __cdecl c_controller_interface::get_content_catalogue()
//.text:00A7D410 ; public: e_controller_index __cdecl c_controller_interface::get_controller_index() const

wchar_t const* c_controller_interface::get_display_name() const
{
	return m_display_name.get_string();
}

//.text:00A7D430 ; 
//.text:00A7D440 ; public: dword __cdecl c_controller_interface::get_hash_bits(long)
//.text:00A7D450 ; // vidmaster related
//.text:00A7D470 ; public: short __cdecl c_controller_interface::get_or_create_user_index()

s_player_identifier const* c_controller_interface::get_player_identifier(s_player_identifier* out_player_identifier) const
{
	return INVOKE_CLASS_MEMBER(0x00A7D500, c_controller_interface, get_player_identifier, out_player_identifier);
}

c_player_profile_interface* c_controller_interface::get_player_profile_interface()
{
	return &m_player_profile;
}

qword const c_controller_interface::get_player_xuid() const
{
	return INVOKE_CLASS_MEMBER(0x00A7D540, c_controller_interface, get_player_xuid);
}

short c_controller_interface::get_user_index() const
{
	return m_user_index;
}

//.text:00A7D580 ; 
//.text:00A7D590 ; 
//.text:00A7D5A0 ; public: bool __cdecl c_signed_in_users_watcher::have_users_changed()
//.text:00A7D670 ; public: void __cdecl c_controller_interface::initialize()
//.text:00A7D6B0 ; public: bool __cdecl c_controller_interface::is_allowed_communications(bool*) const
//.text:00A7D6E0 ; public: bool __cdecl c_controller_interface::is_allowed_online_multiplayer_gaming() const
//.text:00A7D710 ; public: bool __cdecl c_controller_interface::is_allowed_presence(bool*) const
//.text:00A7D750 ; public: bool __cdecl c_controller_interface::is_allowed_profile_viewing(bool*) const

bool c_controller_interface::is_attached() const
{
	return m_state_flags.test(_attached_bit);
}

//.text:00A7D780 ; public: bool __cdecl c_controller_interface::is_gold_live() const
//.text:00A7D810 ; public: bool __cdecl c_controller_interface::is_guest() const
//.text:00A7D850 ; public: bool __cdecl c_controller_interface::is_signed_in_to_live() const

bool c_controller_interface::is_signed_in_to_machine() const
{
	return INVOKE_CLASS_MEMBER(0x00A7D8A0, c_controller_interface, is_signed_in_to_machine);
}

//.text:00A7D8E0 ; public: bool __cdecl c_controller_interface::is_silver_or_gold_live() const
//.text:00A7D930 ; public: bool __cdecl c_controller_interface::is_zombie() const
//.text:00A7D950 ; 
//.text:00A7D960 ; 
//.text:00A7D970 ; 
//.text:00A7D990 ; 
//.text:00A7D9A0 ; 
//.text:00A7D9E0 ; public: void __cdecl c_controller_interface::render()
//.text:00A7DA20 ; public: void __cdecl c_controller_interface::reset_user_index()
//.text:00A7DA30 ; public: void __cdecl c_controller_interface::set_as_unsigned_in_user(bool)
//.text:00A7DA80 ; // vidmaster related
//.text:00A7DA90 ; 
//.text:00A7DAA0 ; // vidmaster related
//.text:00A7DAB0 ; public: void __cdecl c_controller_interface::set_user_index(short, bool)
//.text:00A7DB00 ; private: void __cdecl c_controller_interface::sign_in_controller(s_player_identifier const*, bool)
//.text:00A7DBF0 ; public: static void __cdecl c_controller_interface::sign_in_state_changed()
//.text:00A7DC00 ; private: void __cdecl c_controller_interface::sign_out_controller(bool)
//.text:00A7DCA0 ; public: bool __cdecl c_controller_interface::storage_device_valid(void)
//.text:00A7DCE0 ; public: bool __cdecl c_flags_no_init<c_controller_interface::e_controller_state_flags, short, 12>::test(c_controller_interface::e_controller_state_flags) const
//.text:00A7DD00 ; public: void __cdecl c_controller_interface::update(dword)
//.text:00A7DE30 ; private: void __cdecl c_controller_interface::update_controller_properties()
//.text:00A7E150 ; private: void __cdecl c_controller_interface::update_for_sign_in_change()

