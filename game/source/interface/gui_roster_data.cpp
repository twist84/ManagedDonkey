#include "gui_roster_data.hpp"

#include "cseries/console.hpp"
#include "interface/c_gui_widget.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS(0x00B24FE0, c_gui_roster_data, get_text_value);

bool __fastcall c_gui_roster_data::get_text_value(c_gui_roster_data* _this, void* unused, long player_row_index, long name, c_static_wchar_string<1024>* text_value)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_roster_data, get_text_value, bool(__thiscall*)(c_gui_roster_data*, long, long, c_static_wchar_string<1024>*), _this, player_row_index, name, text_value);

    if (name == STRING_ID(gui, service_tag))
    {
        text_value->append(L"TEST");
        //c_console::write_line("");
    }

    return result;
}

