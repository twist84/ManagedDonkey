#include "interface/gui_roster_data.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B24CA0, c_gui_roster_data, get_integer_value_);
HOOK_DECLARE_CLASS_MEMBER(0x00B24FE0, c_gui_roster_data, get_text_value_);
HOOK_DECLARE_CLASS_MEMBER(0x00B25430, c_gui_active_roster_data, update_);

bool __thiscall c_gui_roster_data::get_integer_value_(int32 element_handle, int32 value_name, int32* value)
{
	return c_gui_roster_data::get_integer_value(element_handle, value_name, value);
}

bool __thiscall c_gui_roster_data::get_text_value_(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* value)
{
	return c_gui_roster_data::get_text_value(element_handle, value_name, value);
}

void __thiscall c_gui_active_roster_data::update_()
{
	HOOK_INVOKE_CLASS_MEMBER(, c_gui_active_roster_data, update_);

	// $IMPLEMENT `c_gui_active_roster_data::update`
	//c_gui_active_roster_data::update();
}

//.text:00B24680 ; public: c_static_array<c_gui_roster_data::s_player_row, 16>::c_static_array<c_gui_roster_data::s_player_row, 16>()

c_gui_active_roster_data::c_gui_active_roster_data(e_controller_index driving_controller) :
	c_gui_roster_data(driving_controller)
{
	//DECLFUNC(0x00B246E0, void, __thiscall, c_gui_active_roster_data*, e_controller_index)(this, driving_controller);
}

c_gui_roster_data::c_gui_roster_data(e_controller_index driving_controller) :
	c_gui_ordered_data(),
	m_matchmaking_last_known_good_extra_slots_searching(false),
	m_matchmaking_last_known_good_extra_slots_found(false),
	m_players(),
	m_player_count(0),
	m_driving_controller(driving_controller),
	m_pause_updating(false)
{
	//DECLFUNC(0x00B24700, void, __thiscall, c_gui_roster_data*, e_controller_index)(this, driving_controller);

	m_players.clear();
}

c_gui_static_roster_data::c_gui_static_roster_data() :
	c_gui_roster_data(k_no_controller)
{
	//DECLFUNC(0x00B247A0, void, __thiscall, c_gui_static_roster_data*)(this);

	c_gui_data::update();
}

c_gui_roster_data::s_player_row::s_player_row()
{
	DECLFUNC(0x00B247F0, void, __thiscall, c_gui_roster_data::s_player_row*)(this);
}

//.text:00B24840 ; 
// 
//.text:00B24850 ; public: virtual c_gui_static_roster_data::~c_gui_static_roster_data()
//.text:00B24860 ; 
//.text:00B24870 ; public: c_gui_roster_data::s_player_row & c_static_array<c_gui_roster_data::s_player_row, 16>::operator[](int32)

//.text:00B24890 ; public: virtual void* c_gui_active_roster_data::`vector deleting destructor'(unsigned int)
//.text:00B24890 ; public: virtual void* c_gui_active_roster_data::`vector deleting destructor'(unsigned int)
//.text:00B248C0 ; public: virtual void* c_gui_roster_data::`vector deleting destructor'(unsigned int)
//.text:00B248F0 ; public: virtual void* __cdecl c_gui_static_roster_data::`vector deleting destructor'(unsigned int)
//.text:00B24920 ; public: bool __cdecl c_gui_static_roster_data::add_player(int32, const s_player_configuration*, e_controller_index)
//.text:00B24940 ; protected: bool c_gui_roster_data::add_player_internal(c_gui_roster_data::e_player_row_type, int32, const s_player_configuration*, e_controller_index, c_gui_roster_data::e_voice_talking_state, bool, bool)
//.text:00B24A20 ; protected: c_gui_roster_data::s_player_row* c_gui_roster_data::add_player_simple(c_gui_roster_data::e_player_row_type, const s_player_identifier*, const s_player_configuration*, e_controller_index)
//.text:00B24A40 ; private: void c_gui_active_roster_data::calculate_group_aggregates_for_sorting()
//.text:00B24B00 ; bool compare_ascending_long(int32, int32, int*)
//.text:00B24B30 ; bool compare_descending_long(int32, int32, int*)

//.text:00B24B60 ; protected: static bool __cdecl c_gui_roster_data::considered_to_be_in_same_party(const c_gui_roster_data::s_player_row*, const c_gui_roster_data::s_player_row*)

void c_gui_roster_data::get_column_names(int32* const column_names, int32* column_count)
{
	//INVOKE_CLASS_MEMBER(0x00B24BC0, c_gui_roster_data, get_column_names, column_names, column_count);

	*column_count = 0;
	column_names[(*column_count)++] = STRING_ID(gui, player_row_type);
	column_names[(*column_count)++] = STRING_ID(gui, player_index);
	column_names[(*column_count)++] = STRING_ID(global, team);
	column_names[(*column_count)++] = STRING_ID(gui, base_color);
	column_names[(*column_count)++] = STRING_ID(gui, controller_index);
	column_names[(*column_count)++] = STRING_ID(gui, voice_output);
	column_names[(*column_count)++] = STRING_ID(gui, button_x);
	column_names[(*column_count)++] = STRING_ID(global, player_name);
	column_names[(*column_count)++] = STRING_ID(gui, service_tag);
	column_names[(*column_count)++] = STRING_ID(global, press_a_to_join);
	column_names[(*column_count)++] = STRING_ID(gui, looking_for_player);
	column_names[(*column_count)++] = STRING_ID(gui, player_found);
	column_names[(*column_count)++] = STRING_ID(gui, experience);
	column_names[(*column_count)++] = STRING_ID(gui, skill_level);
	column_names[(*column_count)++] = STRING_ID(gui, party_bar_length);
	column_names[(*column_count)++] = STRING_ID(gui, special_status);
	//column_names[(*column_count)++] = STRING_ID(gui, matchmaking);
}

//.text:00B24C80 ; public: static int32 __cdecl c_static_array<c_gui_roster_data::s_player_row, 16>::get_count()

int32 c_gui_roster_data::get_current_item_count_internal()
{
	//return INVOKE_CLASS_MEMBER(0x00B24C90, c_gui_roster_data, get_current_item_count_internal);

	return m_player_count;
}

bool c_gui_roster_data::get_integer_value(int32 element_handle, int32 value_name, int32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00B24CA0, c_gui_roster_data, get_integer_value, element_handle, value_name, value);

	if (!VALID_INDEX(element_handle, m_player_count))
	{
		return false;
	}

	s_player_row* player_row = &m_players[element_handle];
	s_player_configuration* player_data = user_interface_session_get_player_data(player_row->session_player_index);

	switch (value_name)
	{
	case STRING_ID(gui, base_color):
	case STRING_ID(gui, base_color_hilite):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			if (player_data)
			{
				*value = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[_color_type_primary].value;
			}
		}

		return true;
	}
	break;
	case STRING_ID(gui, player_index):
	{
		*value = player_row->session_player_index;
		return true;
	}
	break;
	case STRING_ID(gui, player_row_type):
	{
		*value = player_row->player_row_type;
		return true;
	}
	break;
	case STRING_ID(gui, controller_index):
	{
		*value = player_row->local_controller_index;
		return true;
	}
	break;
	case STRING_ID(global, team):
	{
		*value = player_row->player_configuration.host.team_index;
		return true;
	}
	break;
	//case STRING_ID(gui, matchmaking):
	//{
	//	*value = player_row->in_matchmaking;
	//	return true;
	//}
	//break;
	case STRING_ID(gui, voice_output):
	{
		*value = player_row->voice_state;
		return true;
	}
	break;
	case STRING_ID(gui, party_bar_length):
	{
		*value = player_row->party_bar_length;
		return true;
	}
	break;
	case STRING_ID(gui, special_status):
	{
		*value = player_row->special_status;
		return true;
	}
	break;
	case STRING_ID(gui, experience):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			*value = (element_handle % 4) + 1;

			// $TODO pull this from an api?
			if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			{
				*value = 42;
			}
		}
		return true;
	}
	break;
	case STRING_ID(gui, skill_level):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			*value = (element_handle % 4) + 1;

			// $TODO pull this from an api?
			if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			{
				*value = 50;
			}
		}
		return true;
	}
	break;
	case STRING_ID(gui, bungienet_user):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			// Look At Me. I'm The Bungie Now.
			*value |= FLAG(_bungienet_user_default);

			if (player_data)
			{
				*value = player_data->host.weapon.loadouts[0].bungienet_user;
			}
		}
		return true;
	}
	break;
	}

	return false;
}

//.text:00B24E30 ; private: bool c_gui_active_roster_data::get_local_party_voted_to_party_up(const s_life_cycle_matchmaking_progress*)

bool c_gui_roster_data::get_player_appearance(int32 element_handle, s_player_appearance* appearance)
{
	return INVOKE_CLASS_MEMBER(0x00B24EA0, c_gui_roster_data, get_player_appearance, element_handle, appearance);
}

bool c_gui_roster_data::get_player_configuration_value(int32 element_handle, s_player_configuration* player_configuration)
{
	return INVOKE_CLASS_MEMBER(0x00B24F40, c_gui_roster_data, get_player_configuration_value, element_handle, player_configuration);
}

bool c_gui_roster_data::get_player_identifier_value(int32 element_handle, s_player_identifier* player_identifier)
{
	return INVOKE_CLASS_MEMBER(0x00B24F90, c_gui_roster_data, get_player_identifier_value, element_handle, player_identifier);
}

bool c_gui_roster_data::get_text_value(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* value)
{
	//return INVOKE_CLASS_MEMBER(0x00B24FE0, c_gui_roster_data, _get_text_value, element_handle, value_name, value);

	if (!VALID_INDEX(element_handle, m_player_count))
	{
		return false;
	}

	c_gui_roster_data::s_player_row* player_row = &m_players[element_handle];

	switch (value_name)
	{
	case STRING_ID(global, player_name):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			value->set(player_row->player_configuration.host.name.get_string());
		}
		return true;
	}
	break;
	case STRING_ID(global, press_a_to_join):
	{
		if (player_row->player_row_type == _player_row_type_press_a_to_join)
		{
			user_interface_global_string_get(STRING_ID(global, press_a_to_join), value);
		}
		return true;
	}
	break;
	case STRING_ID(gui, looking_for_player):
	{
		if (player_row->player_row_type == _player_row_type_searching)
		{
			user_interface_global_string_get(STRING_ID(gui, looking_for_player), value);
		}
		return true;
	}
	break;
	case STRING_ID(gui, player_found):
	{
		if (player_row->player_row_type == _player_row_type_found)
		{
			user_interface_global_string_get(STRING_ID(gui, player_found), value);
		}
		return true;
	}
	break;
	case STRING_ID(gui, service_tag):
	{
		bool is_elite = player_row->player_configuration.host.appearance.player_model_choice == _player_model_choice_elite;
		c_static_wchar_string<1024> player_model_text;
		if (user_interface_global_string_get(is_elite ? STRING_ID(global, dervish) : STRING_ID(global, masterchief), &player_model_text))
		{
			c_static_wchar_string<256> service_tag_text;
			service_tag_text.set(player_row->player_configuration.host.appearance.service_tag.get_string());
			value->print(L"%s - %s", player_model_text.get_string(), service_tag_text.get_string());
		}
		return true;
	}
	break;
	}

	return false;
}

//.text:00B25180 ; public: static c_gui_roster_data::e_voice_talking_state __cdecl c_gui_roster_data::get_voice_for_player(e_controller_index driving_controller_index, int32 session_player_index)
//.text:00B25190 ; int __cdecl group_roster_sort_proc(const void*, const void*)
//.text:00B252D0 ; public: void __cdecl c_static_array<c_gui_roster_data::s_player_row, 16>::sort(int32, int(__cdecl*)(const void*, const void*))
//.text:00B252F0 ; int __cdecl squad_roster_sort_proc(const void*, const void*)

void c_gui_active_roster_data::update()
{
	INVOKE_CLASS_MEMBER(0x00B25430, c_gui_active_roster_data, update_);

	//c_gui_data::update();
	//
	//s_life_cycle_matchmaking_progress matchmaking_progress{};
	//if (user_interface_squad_in_matchmaking())
	//	user_interface_matchmaking_get_matchmaking_progress(&matchmaking_progress);
	//
	//if (!m_pause_updating &&
	//	(user_interface_interactive_session_is_squad() || user_interface_interactive_session_is_group()) &&
	//	(!user_interface_squad_in_matchmaking() || matchmaking_progress.progress_type))
	//{
	//	if (user_interface_squad_in_matchmaking() &&
	//		matchmaking_progress.progress_type == _life_cycle_matchmaking_progress_post_match &&
	//		matchmaking_progress.configuring_match.configuring_progress.encountered_load_failure)
	//	{
	//		int32 player_index = 0;
	//		while (player_index < 16 && !user_interface_session_is_local_player(player_index))
	//		{
	//			player_index++;
	//		}
	//
	//		if (player_index < 16)
	//		{
	//			user_interface_get_player_party_nonce(player_index);
	//		}
	//	}
	//
	//	m_player_count = 0;
	//	uns32 player_error_mask = 0;
	//	e_session_game_start_status session_game_start_status = user_interface_get_session_game_start_status(NULL, &player_error_mask);
	//
	//	for (int32 session_player_index = 0; session_player_index < 16; session_player_index++)
	//	{
	//		if (!user_interface_session_is_player_valid(session_player_index))
	//			continue;
	//
	//		uns32 player_mask = FLAG(session_player_index);
	//		e_special_status special_status = _special_status_none;
	//		e_controller_index local_controller_index = k_no_controller;
	//
	//		if (user_interface_interactive_session_is_squad())
	//		{
	//			if (session_game_start_status == _session_game_start_status_error && TEST_MASK(player_mask, player_error_mask))
	//				special_status = _special_status_error;
	//		}
	//		else if (matchmaking_progress.progress_type == _life_cycle_matchmaking_progress_post_match)
	//		{
	//			if (TEST_MASK(player_mask, matchmaking_progress.searching.search_criteria.searching_skill_range_max))
	//			{
	//				special_status = _special_status_party_up_chosen_leader;
	//			}
	//			else if (TEST_MASK(player_mask, matchmaking_progress.searching.search_criteria.searching_skill_range_min))
	//			{
	//				special_status = _special_status_party_up_undecided_leader;
	//			}
	//			else if (TEST_MASK(player_mask, matchmaking_progress.searching.search_criteria.searching_party_size_min))
	//			{
	//				special_status = _special_status_party_up_chosen_member;
	//			}
	//		}
	//		if (user_interface_session_is_local_player(session_player_index))
	//			local_controller_index = user_interface_session_get_controller_index(session_player_index);
	//		s_player_row* player_row = c_gui_roster_data::add_player_internal(_player_row_type_player,
	//			session_player_index,
	//			user_interface_squad_get_player_identifier(session_player_index),
	//			user_interface_session_get_player_data(session_player_index),
	//			local_controller_index,
	//			_voice_state_none,
	//			special_status,
	//			user_interface_networking_should_show_skill());
	//	}
	//
	//	c_gui_active_roster_data::update_press_a_to_join_slots();
	//	c_gui_roster_data::update_matchmaking_slots();
	//
	//	if (user_interface_interactive_session_is_squad())
	//	{
	//		m_players.sort(m_player_count, squad_roster_sort_proc);
	//	}
	//	else
	//	{
	//		c_gui_active_roster_data::calculate_group_aggregates_for_sorting();
	//		m_players.sort(m_player_count, group_roster_sort_proc);
	//	}
	//
	//	c_gui_active_roster_data::update_party_bar();
	//}
}

//.text:00B25650 ; protected: void c_gui_roster_data::update_matchmaking_slots()
//.text:00B257F0 ; private: void c_gui_active_roster_data::update_party_bar()
//.text:00B25880 ; private: static void __cdecl c_gui_active_roster_data::update_player_with_group_session_state(c_gui_roster_data::s_player_row*)
//.text:00B258B0 ; private: static void __cdecl c_gui_active_roster_data::update_player_with_squad_session_state(c_gui_roster_data::s_player_row*)
//.text:00B25930 ; private: void c_gui_active_roster_data::update_press_a_to_join_slots()

