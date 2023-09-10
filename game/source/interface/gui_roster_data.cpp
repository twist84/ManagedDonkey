#include "interface/gui_roster_data.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B24CA0, c_gui_roster_data, _get_integer_value);
HOOK_DECLARE_CLASS_MEMBER(0x00B24FE0, c_gui_roster_data, _get_text_value);

// #TODO: reimplement `c_gui_active_roster_data::update`

bool __thiscall c_gui_roster_data::_get_integer_value(long player_row_index, long name, long* integer_value)
{
	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_roster_data, _get_integer_value, player_row_index, name, integer_value);

	s_player_configuration* player_data = user_interface_session_get_player_data(m_players[player_row_index].player_index);
	switch (name)
	{
	case STRING_ID(gui, base_color):
	case STRING_ID(gui, base_color_hilite):
	{
		if (player_data)
			*integer_value = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[_color_type_primary].value;

		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, player_index):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, player_row_type):
	{
		// simulate added controllers
		if (player_row_index >= 1 && player_row_index < 4)
		{
			*integer_value = c_gui_roster_data::_player_row_type_player;
			//*integer_value = c_gui_roster_data::_player_row_type_player_found;
			//*integer_value = c_gui_roster_data::_player_row_type_looking_for_player;
			//*integer_value = c_gui_roster_data::_player_row_type_press_a_to_join;
		}

		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, controller_index):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, voice_output):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, special_status):
	{
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, experience):
	{
		*integer_value = (player_row_index % 4) + 1;

		// #TODO: pull this from an api?
		if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			*integer_value = 42;

		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, skill_level):
	{
		*integer_value = (player_row_index % 4) + 1;

		// #TODO: pull this from an api?
		if (player_data && player_data->host.weapon.loadouts[0].bungienet_user.test(_bungienet_user_bungie))
			*integer_value = 50;

		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, bungienet_user):
	{
		// Look At Me. I'm The Bungie Now.
		*integer_value |= FLAG(_bungienet_user_default);

		if (player_data)
			*integer_value = player_data->host.weapon.loadouts[0].bungienet_user;

		if (!result)
			return true;
	}
	break;
	}

	return result;
}

bool __thiscall c_gui_roster_data::_get_text_value(long player_row_index, long name, c_static_wchar_string<1024>* text_value)
{
	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_roster_data, _get_text_value, player_row_index, name, text_value);

	switch (name)
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
		text_value->set(L"PRESS A TO JOIN");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, looking_for_player):
	{
		text_value->set(L"LOOKING FOR PLAYER");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, player_found):
	{
		text_value->set(L"PLAYER FOUND");
		if (!result)
			return true;
	}
	break;
	case STRING_ID(gui, service_tag):
	{
		if (!m_players[player_row_index].player_data.host.appearance.service_tag.length())
		{
			long player_index = m_players[player_row_index].player_index;
			if (user_interface_session_is_local_player(player_index))
			{
				c_controller_interface* controller = controller_get(m_players[player_row_index].controller_index);

				if (!text_value->length())
				{
					text_value->print(L"%s - %s", L"SPARTAN", controller->m_player_profile.desired_service_tag);
				}
				else
				{
					text_value->append(controller->m_player_profile.desired_service_tag);
				}
			}
			else
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(player_index);
				if (player_data)
				{
					if (!text_value->length())
					{
						text_value->print(L"%s - %s", L"SPARTAN", player_data->host.appearance.service_tag.get_string());
					}
					else
					{
						text_value->append(player_data->host.appearance.service_tag.get_string());
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

