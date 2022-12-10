#include "interface/gui_roster_list_widget.hpp"

#include "cseries/console.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS(0x00B26100, c_gui_roster_list_widget, update_render_state);

void __fastcall c_gui_roster_list_widget::update_render_state(c_gui_roster_list_widget* _this, void* unused, dword a2)
{
	//HOOK_INVOKE_CLASS(, c_gui_roster_list_widget, update_render_state, void(__thiscall*)(c_gui_roster_list_widget*, dword), _this, a2);

	//c_gui_list_widget::update_render_state
	DECLFUNC(0x00B16650, void, __thiscall, c_gui_list_widget*, dword)(_this, a2);

	c_gui_data* data = _this->get_data();
	if (!data)
		return;

	for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(_this->get_first_child_widget_by_type(_gui_widget_type_list_item_widget));
		list_item_widget;
		list_item_widget = list_item_widget->get_next_list_item_widget(true))
	{
		c_gui_bitmap_widget* base_color_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color));
		c_gui_bitmap_widget* base_color_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color_hilite));
		c_gui_bitmap_widget* party_bar_player_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, party_bar_player));
		c_gui_text_widget* name_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, name));
		c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));

		long element_handle = list_item_widget->get_element_handle();

		long session_player_index = -1;
		data->get_integer_value(element_handle, STRING_ID(gui, player_index), &session_player_index);
		//assert((session_player_index == NONE) || VALID_INDEX(session_player_index, NUMBEROF(m_temporary_team)));

		long player_row_type = 0;
		data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type);
		bool show_base_color = player_row_type == 0;

		long party_bar_length = 0;
		data->get_integer_value(element_handle, STRING_ID(gui, party_bar_length), &party_bar_length);

		long base_color = 0;
		data->get_integer_value(element_handle, STRING_ID(gui, base_color), &base_color);

		base_color_bitmap_widget->set_visible(show_base_color);
		base_color_hilite_bitmap_widget->set_visible(show_base_color);

		bool show_party_bar = show_base_color && _this->m_show_party_bar && party_bar_length > 0;
		party_bar_player_bitmap_widget->set_visible(show_party_bar);

		bool is_team_game = _this->m_is_team_game;

		long team_index = -1;
		long color_list_index = base_color;
		if (is_team_game)
		{
			if (session_player_index == -1 || _this->m_temporary_team[session_player_index].temporary_team_change_active)
			{
				long team = -1;
				if (data)
					data->get_integer_value(element_handle, STRING_ID(global, team), &team);
			
				team_index = team;
			}
			else
			{
				team_index = _this->m_temporary_team[session_player_index].temporary_team_index;
			}

			color_list_index = team_index;
		}

		tint_widget_to_change_color(base_color_bitmap_widget, color_list_index, is_team_game);
		tint_widget_to_change_color(base_color_hilite_bitmap_widget, color_list_index, is_team_game);
		tint_widget_to_change_color(party_bar_player_bitmap_widget, color_list_index, is_team_game);
		tint_widget_to_change_color(name_text_widget, color_list_index, is_team_game);
		tint_widget_to_change_color(service_tag_text_widget, color_list_index, is_team_game);
		party_bar_player_bitmap_widget->m_sprite_frame = party_bar_length - 1;
	}
}

