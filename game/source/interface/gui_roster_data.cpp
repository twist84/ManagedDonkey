#include "interface/gui_roster_data.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_session.hpp"
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
	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_roster_data, _get_text_value, element_handle, value_name, value);

	switch (value_name)
	{
	case STRING_ID(global, player_name):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, name_hilite):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(global, press_a_to_join):
	{
		value->set(L"PRESS A TO JOIN");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, looking_for_player):
	{
		value->set(L"LOOKING FOR PLAYER");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, player_found):
	{
		value->set(L"PLAYER FOUND");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, service_tag):
	{
		if (!m_players[element_handle].player_configuration.host.appearance.service_tag.length())
		{
			long player_index = m_players[element_handle].session_player_index;
			if (user_interface_session_is_local_player(player_index))
			{
				c_controller_interface* controller = controller_get(m_players[element_handle].local_controller_index);

				if (!value->length())
				{
					value->print(L"%s - %s", L"SPARTAN", controller->m_player_profile.m_appearance.desired_service_tag);
				}
				else
				{
					value->append(controller->m_player_profile.m_appearance.desired_service_tag);
				}
			}
			else
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(player_index);
				if (player_data)
				{
					if (!value->length())
					{
						value->print(L"%s - %s", L"SPARTAN", player_data->host.appearance.service_tag.get_string());
					}
					else
					{
						value->append(player_data->host.appearance.service_tag.get_string());
					}
				}
			}
		}

		if (!result)
			return true;
	}
	break;
	}

	return result;
}

