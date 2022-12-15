#include "gui_roster_data.hpp"

#include "cseries/console.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_widget.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS(0x00B24CA0, c_gui_roster_data, _get_integer_value);
HOOK_DECLARE_CLASS(0x00B24FE0, c_gui_roster_data, _get_text_value);

bool __fastcall c_gui_roster_data::_get_integer_value(c_gui_roster_data* _this, void* unused, long player_row_index, long name, long* integer_value)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_roster_data, _get_integer_value, bool(__thiscall*)(c_gui_roster_data*, long, long, long*), _this, player_row_index, name, integer_value);
    
    switch (name)
    {
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
        if (!result)
            return true;
    }
    break;
	case STRING_ID(gui, skill_level):
    {
        if (!result)
            return true;
    }
    break;
	case STRING_ID(gui, bungienet_user):
    {
        // Look At Me. I'm The Bungie Now.
        *integer_value |= FLAG(player_row_index % 4);
        if (!result)
            return true;
    }
    break;
    }

    return result;
}

bool __fastcall c_gui_roster_data::_get_text_value(c_gui_roster_data* _this, void* unused, long player_row_index, long name, c_static_wchar_string<1024>* text_value)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_roster_data, _get_text_value, bool(__thiscall*)(c_gui_roster_data*, long, long, c_static_wchar_string<1024>*), _this, player_row_index, name, text_value);

    switch (name)
    {
    case STRING_ID(global, player_name):
    {
        if (player_row_index >= 1 && player_row_index < 4)
            text_value->set(controller_get(static_cast<e_controller_index>(player_row_index))->m_display_name.get_string());

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
        if (!result)
            return true;
    }
    break;
    }

    return result;
}

