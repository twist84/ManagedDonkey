#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_player_profile.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct s_player_identifier;
struct c_player_marketplace;
struct c_controller_interface
{
public:
	c_player_profile_interface* get_player_profile_interface();
	bool is_attached() const;
	bool in_use() const;
	bool is_signed_in_to_machine() const;
	short get_user_index() const;
	s_player_identifier const* get_player_identifier(s_player_identifier* out_player_identifier) const;
	qword const get_player_xuid() const;
	wchar_t const* get_display_name() const;

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

	c_flags<e_controller_state_flags, short, k_number_of_controller_state_flags> m_state_flags;
	short m_user_index;
	dword __unknown4;
	c_player_profile_interface m_player_profile;
	c_static_wchar_string<16> m_display_name;
	qword m_hash_bits;
	qword m_hash_bits_last_game_instance;
	c_static_flags<k_achievement_count> m_achievements;
	c_player_marketplace* m_marketplace;
	byte __data143C[0x5];
	byte vidmaster;
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

enum e_window_index;
enum e_controller_index;

extern c_controller_interface* __cdecl controller_get(e_controller_index controller_index);
extern e_window_index __cdecl controller_get_game_render_window(e_controller_index controller_index);
extern e_controller_index __cdecl controller_index_from_user_index(long user_index);
extern void __cdecl controllers_render();
extern bool __cdecl controller_has_centered_crosshair(e_controller_index controller_index);

