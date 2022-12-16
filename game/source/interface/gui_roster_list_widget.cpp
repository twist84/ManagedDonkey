#include "interface/gui_roster_list_widget.hpp"

#include "cseries/console.hpp"
#include "game/game_engine_display.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_roster_data.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

#include <assert.h>

HOOK_DECLARE_CLASS(0x00B25C60, c_gui_roster_list_widget, update);
HOOK_DECLARE_CLASS(0x00B26100, c_gui_roster_list_widget, update_render_state);

void __fastcall c_gui_roster_list_widget::update(c_gui_roster_list_widget* _this, void* unused, dword a2)
{
	//HOOK_INVOKE_CLASS(, c_gui_roster_list_widget, update, void(__thiscall*)(c_gui_roster_list_widget*, dword), _this, a2);

	long selectable_item_cap_count = 0;

	c_gui_roster_data* roster_data = static_cast<c_gui_roster_data*>(_this->get_data());
	if (roster_data)
	{
		_this->update_team_mode();

		for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(_this->get_first_child_widget_by_type(_gui_widget_type_list_item_widget));
			list_item_widget;
			list_item_widget = list_item_widget->get_next_list_item_widget(true))
		{
			c_gui_bitmap_widget* base_color_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color));
			c_gui_bitmap_widget* base_color_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color_hilite));
			c_gui_text_widget* name_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, name));
			c_gui_text_widget* name_hilite_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, name_hilite));
			c_gui_bitmap_widget* ring_of_light_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, ring_of_light));
			c_gui_text_widget* press_a_to_join_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, press_a_to_join));
			c_gui_bitmap_widget* press_a_to_join_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(global, press_a_to_join));
			c_gui_text_widget* looking_for_player_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, looking_for_player));
			c_gui_bitmap_widget* looking_for_player_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, looking_for_player));
			c_gui_bitmap_widget* looking_for_player2_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, looking_for_player2));
			c_gui_bitmap_widget* looking_for_player3_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, looking_for_player3));
			c_gui_text_widget* player_found_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, player_found));
			c_gui_bitmap_widget* player_found_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, player_found));
			c_gui_bitmap_widget* party_up_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, party_up));
			c_gui_bitmap_widget* check_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, check));
			c_gui_bitmap_widget* player_emblem_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, player_emblem));
			c_gui_bitmap_widget* player_emblem_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, player_emblem_hilite));
			c_gui_bitmap_widget* skill_level_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, skill_level));
			c_gui_bitmap_widget* skill_level_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, skill_level_hilite));
			c_gui_bitmap_widget* experience_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, experience));
			c_gui_bitmap_widget* experience_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, experience_hilite));
			c_gui_bitmap_widget* rank_tray_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, rank_tray));
			c_gui_bitmap_widget* rank_tray_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, rank_tray_hilite));
			c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));

			long element_handle = list_item_widget->get_element_handle();

			long session_player_index = -1;
			long player_row_type = c_gui_roster_data::_player_row_type_player;
			long controller_index = -1;
			long voice_output = 0;
			long special_status = 0;
			long experience = -1;
			long skill_level = -1;
			long bungienet_user = 0;
			long matchmaking = 0;

			c_static_wchar_string<1024> player_found;
			c_static_wchar_string<1024> looking_for_player;
			c_static_wchar_string<1024> press_a_to_join;
			c_static_wchar_string<1024> service_tag;

			if (base_color_bitmap_widget
				&& base_color_hilite_bitmap_widget
				&& name_text_widget
				&& name_hilite_text_widget
				&& ring_of_light_bitmap_widget
				&& press_a_to_join_text_widget
				&& press_a_to_join_bitmap_widget
				&& looking_for_player_text_widget
				&& looking_for_player_bitmap_widget
				&& looking_for_player2_bitmap_widget
				&& looking_for_player3_bitmap_widget
				&& player_found_text_widget
				&& player_found_bitmap_widget
				&& party_up_bitmap_widget
				&& check_bitmap_widget
				&& player_emblem_bitmap_widget
				&& player_emblem_hilite_bitmap_widget
				&& skill_level_bitmap_widget
				&& skill_level_hilite_bitmap_widget
				&& experience_bitmap_widget
				&& experience_hilite_bitmap_widget
				&& rank_tray_bitmap_widget
				&& rank_tray_hilite_bitmap_widget
				&& service_tag_text_widget
				&& roster_data->get_text_value(element_handle, STRING_ID(gui, service_tag), &service_tag)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, player_index), &session_player_index)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, controller_index), &controller_index)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, voice_output), &voice_output)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, special_status), &special_status)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, experience), &experience)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, skill_level), &skill_level)
				&& roster_data->get_integer_value(element_handle, STRING_ID(gui, bungienet_user), &bungienet_user)
				//&& roster_data->get_integer_value(element_handle, STRING_ID(gui, matchmaking), &matchmaking) // halo 3 tag test
				)
			{
				bool player_row_type0 = player_row_type == c_gui_roster_data::_player_row_type_player;
				bool player_row_type1 = player_row_type == c_gui_roster_data::_player_row_type_player_found;
				bool player_row_type2 = player_row_type == c_gui_roster_data::_player_row_type_looking_for_player;
				bool player_row_type3 = player_row_type == c_gui_roster_data::_player_row_type_press_a_to_join;

				assert((session_player_index == NONE) || VALID_INDEX(session_player_index, NUMBEROF(m_temporary_team)));

				bool show_experience = matchmaking > 0;
				bool show_rank_tray = experience != -1;

				bool is_team_game = _this->m_is_team_game;

				if (player_row_type0)
					selectable_item_cap_count++;

				list_item_widget->set_enabled(player_row_type0);

				player_found_text_widget->set_text(player_found.get_string());
				player_found_bitmap_widget->set_visible(player_row_type1);
				player_found_text_widget->set_visible(player_row_type1);

				looking_for_player_text_widget->set_text(looking_for_player.get_string());
				looking_for_player_bitmap_widget->set_visible(player_row_type2);
				looking_for_player2_bitmap_widget->set_visible(player_row_type2);
				looking_for_player3_bitmap_widget->set_visible(player_row_type2);
				looking_for_player_text_widget->set_visible(player_row_type2);

				press_a_to_join_text_widget->set_text(press_a_to_join.get_string());
				press_a_to_join_bitmap_widget->set_visible(player_row_type3);
				press_a_to_join_text_widget->set_visible(player_row_type3);

				long nameplate_sprite_frame = TEST_BIT(bungienet_user, 0);
				if (TEST_BIT(bungienet_user, 1))
				{
					nameplate_sprite_frame = 2;
				}
				else if (TEST_BIT(bungienet_user, 2))
				{
					nameplate_sprite_frame = 3;
				}

				base_color_bitmap_widget->m_sprite_frame = nameplate_sprite_frame;
				base_color_hilite_bitmap_widget->m_sprite_frame = nameplate_sprite_frame;

				name_text_widget->set_visible(player_row_type0);
				name_hilite_text_widget->set_visible(player_row_type0);

				player_emblem_bitmap_widget->set_visible(player_row_type0);
				player_emblem_hilite_bitmap_widget->set_visible(player_row_type0);

				service_tag_text_widget->set_visible(player_row_type0);
				service_tag_text_widget->set_text(service_tag.get_string());
				//DECLFUNC(0x00AA63A0, void, __cdecl, c_gui_widget*, bool, wchar_t const*)(list_item_widget, player_row_type0, service_tag.get_string());

				skill_level_bitmap_widget->set_visible(skill_level != -1);
				skill_level_hilite_bitmap_widget->set_visible(skill_level != -1);
				if (skill_level != -1)
				{
					skill_level_bitmap_widget->m_sprite_frame = skill_level;
					skill_level_hilite_bitmap_widget->m_sprite_frame = skill_level;
				}

				experience_bitmap_widget->set_visible(experience != -1);
				experience_hilite_bitmap_widget->set_visible(experience != -1);
				if (experience != -1)
				{
					experience_bitmap_widget->m_sprite_frame = experience;
					experience_hilite_bitmap_widget->m_sprite_frame = experience;

					// halo 3 tag test
					//sub_1409C7C80(experience_bitmap_widget, show_experience);
					//sub_1409C7C80(experience_hilite_bitmap_widget, show_experience);

					show_rank_tray = show_experience;
				}
				else
				{
					show_rank_tray = show_experience;
				}

				rank_tray_bitmap_widget->set_visible(player_row_type0);
				rank_tray_hilite_bitmap_widget->set_visible(player_row_type0);
				if (show_rank_tray)
				{
					// halo 3 tag test
					//sub_1409C7CB0(rank_tray_bitmap_widget, show_rank_tray);
					//sub_1409C7CB0(rank_tray_hilite_bitmap_widget, show_rank_tray);
				}

				check_bitmap_widget->set_visible(true);
				party_up_bitmap_widget->set_visible(true);
				switch (special_status)
				{
				case 2:
					check_bitmap_widget->m_sprite_frame = 0;
					party_up_bitmap_widget->m_sprite_frame = 0;
					break;
				case 3:
					check_bitmap_widget->m_sprite_frame = 1;
					party_up_bitmap_widget->m_sprite_frame = 1;
					break;
				case 4:
					check_bitmap_widget->m_sprite_frame = 2;
					party_up_bitmap_widget->m_sprite_frame = 2;
					break;
				default:
					check_bitmap_widget->set_visible(false);
					party_up_bitmap_widget->set_visible(false);
				}

				if (controller_index != -1)
				{
					ring_of_light_bitmap_widget->set_visible(true);
					ring_of_light_bitmap_widget->m_sprite_frame = controller_index;
				}
				else
				{
					switch (voice_output)
					{
					case 1:
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->m_sprite_frame = 4;
						break;
					case 2:
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->m_sprite_frame = 5;
						break;
					case 3:
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->m_sprite_frame = 6;
						break;
					case 4:
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->m_sprite_frame = 7;
						break;
					default:
						ring_of_light_bitmap_widget->set_visible(false);
						break;
					}
				}


				c_static_wchar_string<1024> name;
				c_static_wchar_string<256> team_name;

				bool team_change_active = false;
				if (is_team_game)
				{
					if (session_player_index != -1 && _this->m_temporary_team[session_player_index].temporary_team_change_active)
					{
						game_engine_get_team_name(_this->m_temporary_team[session_player_index].temporary_team_index, &team_name);
						team_name;

						union
						{
							dword left_bumper_value = 0x0000E104;
							wchar_t left_bumper[4];
						};

						union
						{
							dword right_bumper_value = 0x0000E105;
							wchar_t right_bumper[4];
						};

						name.append_print(L"%ls %s %ls", left_bumper, team_name.get_string(), right_bumper);

						team_change_active = true;
					}
				}

				// set name font to the correct id
				static_cast<c_gui_sized_text_widget<48>*>(name_text_widget)->set_font(1);

				if (team_change_active)
				{
					name_text_widget->set_text(name.get_string());
					name_hilite_text_widget->set_text(name.get_string());
				}
				else
				{
					roster_data->get_text_value(element_handle, STRING_ID(global, player_name), &name);
					name_text_widget->set_text(name.get_string());
					name_hilite_text_widget->set_text(name.get_string());
				}
			}
		}
	}

	_this->set_selectable_item_cap_count(selectable_item_cap_count);
	DECLFUNC(0x00B16650, void, __thiscall, c_gui_list_widget*, dword)(_this, a2);
}

void __fastcall c_gui_roster_list_widget::update_render_state(c_gui_roster_list_widget* _this, void* unused, dword a2)
{
	//HOOK_INVOKE_CLASS(, c_gui_roster_list_widget, update_render_state, void(__thiscall*)(c_gui_roster_list_widget*, dword), _this, a2);

	//c_gui_list_widget::update_render_state
	DECLFUNC(0x00B16650, void, __thiscall, c_gui_list_widget*, dword)(_this, a2);

	c_gui_data* data = _this->get_data();
	if (data)
	{
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
			long player_row_type = c_gui_roster_data::_player_row_type_player;
			long party_bar_length = 0;
			long base_color = 0;

			if (base_color_bitmap_widget
				&& base_color_hilite_bitmap_widget
				&& party_bar_player_bitmap_widget
				&& name_text_widget
				&& service_tag_text_widget
				)
			{
				data->get_integer_value(element_handle, STRING_ID(gui, player_index), &session_player_index);
				data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type);
				data->get_integer_value(element_handle, STRING_ID(gui, party_bar_length), &party_bar_length);
				data->get_integer_value(element_handle, STRING_ID(gui, base_color), &base_color);

				assert((session_player_index == NONE) || VALID_INDEX(session_player_index, NUMBEROF(m_temporary_team)));

				bool player_row_type0 = player_row_type == c_gui_roster_data::_player_row_type_player;

				bool show_party_bar = player_row_type0 && _this->m_show_party_bar && party_bar_length > 0;

				base_color_bitmap_widget->set_visible(player_row_type0);
				base_color_hilite_bitmap_widget->set_visible(player_row_type0);
				party_bar_player_bitmap_widget->set_visible(show_party_bar);

				bool is_team_game = _this->m_is_team_game;

				long team_index = -1;
				long color_list_index = base_color;
				if (is_team_game)
				{
					if (session_player_index == -1
						// swap this for `get_current_team_change_team_index`?
						|| !_this->m_temporary_team[session_player_index].temporary_team_change_active
						&& _this->m_temporary_team->lying_begin_time < a2)
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
	}
}

void __cdecl c_gui_roster_list_widget::update_team_mode()
{
	DECLFUNC(0x00B26210, void, __thiscall, c_gui_roster_list_widget*)(this);
}

