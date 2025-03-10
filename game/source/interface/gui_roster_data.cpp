#include "interface/gui_roster_data.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B24CA0, c_gui_roster_data, _get_integer_value);
HOOK_DECLARE_CLASS_MEMBER(0x00B24FE0, c_gui_roster_data, _get_text_value);

// $TODO: reimplement `c_gui_active_roster_data::update`

bool __thiscall c_gui_roster_data::_get_integer_value(long element_handle, long value_name, long* value)
{
	//bool result = false;
	//HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_roster_data, _get_integer_value, element_handle, value_name, value);

	if (!VALID_INDEX(element_handle, m_player_count))
		return false;

	s_player_row* player_row = &m_players[element_handle];
	s_player_configuration* player_data = user_interface_session_get_player_data(player_row->session_player_index);

	switch (value_name)
	{
	case STRING_ID(gui, base_color):
	case STRING_ID(gui, base_color_hilite):
	{
		if (player_data)
			*value = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[_color_type_primary].value;

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
	case STRING_ID(global , team):
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
		*value = (element_handle % 4) + 1;

		// $TODO: pull this from an api?
		if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			*value = 42;

		return true;
	}
	break;
	case STRING_ID(gui, skill_level):
	{
		*value = (element_handle % 4) + 1;

		// $TODO: pull this from an api?
		if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			*value = 50;

		return true;
	}
	break;
	case STRING_ID(gui, bungienet_user):
	{
		// Look At Me. I'm The Bungie Now.
		*value |= FLAG(_bungienet_user_default);

		if (player_data)
			*value = player_data->host.weapon.loadouts[0].bungienet_user;

		return true;
	}
	break;
	}

	return false;
}

bool __thiscall c_gui_roster_data::_get_text_value(long element_handle, long value_name, c_static_wchar_string<1024>* value)
{
	//bool result = false;
	//HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_roster_data, _get_text_value, element_handle, value_name, value);

	if (!VALID_INDEX(element_handle, m_player_count))
		return false;

	c_gui_roster_data::s_player_row* player_row = &m_players[element_handle];

	switch (value_name)
	{
	case STRING_ID(global, player_name):
	{
		if (player_row->player_row_type == _player_row_type_player)
		{
			value->set(player_row->player_configuration.host.name.get_string());
			return true;
		}
	}
	break;
	case STRING_ID(global, press_a_to_join):
	{
		if (player_row->player_row_type == _player_row_type_press_a_to_join)
		{
			user_interface_global_string_get(STRING_ID(global, press_a_to_join), value);
			return true;
		}
	}
	break;
	case STRING_ID(gui, looking_for_player):
	{
		if (player_row->player_row_type == _player_row_type_searching)
		{
			user_interface_global_string_get(STRING_ID(gui, looking_for_player), value);
			return true;
		}
	}
	break;
	case STRING_ID(gui, player_found):
	{
		if (player_row->player_row_type == _player_row_type_found)
		{
			user_interface_global_string_get(STRING_ID(gui, player_found), value);
			return true;
		}
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
			return true;
		}
	}
	break;
	}

	return false;
}

