#include "interface/gui_roster_list_widget.hpp"

#include "game/game_engine_display.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_roster_data.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B25C60, c_gui_roster_list_widget, update_);
HOOK_DECLARE_CLASS_MEMBER(0x00B26100, c_gui_roster_list_widget, update_render_state_);

void __thiscall c_gui_roster_list_widget::update_(uns32 current_milliseconds)
{
	c_gui_roster_list_widget::update(current_milliseconds);
}

void __thiscall c_gui_roster_list_widget::update_render_state_(uns32 current_milliseconds)
{
	c_gui_roster_list_widget::update_render_state(current_milliseconds);
}

c_gui_roster_list_widget::c_gui_roster_list_widget(bool game_roster_is_local_party, bool remote_team_game) :
	c_gui_list_widget(),
	m_game_roster_is_local_party_internal(game_roster_is_local_party),
	m_is_team_game_internal(remote_team_game),
	m_show_party_bar_internal(false)
{
	//DECLFUNC(0x00B25A60, void, __thiscall, c_gui_roster_list_widget*, bool, bool)(this, game_roster_is_local_party, remote_team_game);

	for (int32 team_index = 0; team_index < NUMBEROF(m_temporary_team); team_index++)
	{
		m_temporary_team[team_index].temporary_team_change_active = false;
		m_temporary_team[team_index].desired_team = NONE;
		m_temporary_team[team_index].set_time = 0;
		m_temporary_team[team_index].lying_end_time = 0;
	}
}

//.text:00B25AC0 ; 
//.text:00B25AD0 ; public: virtual void* c_gui_roster_list_widget::`vector deleting destructor'(unsigned int)

void c_gui_roster_list_widget::end_team_change(int32 session_player_index, int32 lying_end_time)
{
	INVOKE_CLASS_MEMBER(0x00B25B00, c_gui_roster_list_widget, end_team_change, session_player_index, lying_end_time);
}

bool c_gui_roster_list_widget::get_current_team_change_is_active(int32 session_player_index)
{
	return INVOKE_CLASS_MEMBER(0x00B25B30, c_gui_roster_list_widget, get_current_team_change_is_active, session_player_index);
}

bool c_gui_roster_list_widget::get_current_team_change_is_lying(int32 session_player_index)
{
	return INVOKE_CLASS_MEMBER(0x00B25B50, c_gui_roster_list_widget, get_current_team_change_is_lying, session_player_index);
}

int32 c_gui_roster_list_widget::get_current_team_change_team_index(int32 session_player_index)
{
	return INVOKE_CLASS_MEMBER(0x00B25B70, c_gui_roster_list_widget, get_current_team_change_team_index, session_player_index);
}

int32 c_gui_roster_list_widget::get_current_team_change_time(int32 session_player_index)
{
	return INVOKE_CLASS_MEMBER(0x00B25BB0, c_gui_roster_list_widget, get_current_team_change_time, session_player_index);
}

bool c_gui_roster_list_widget::get_game_roster_is_local_party() const
{
	return INVOKE_CLASS_MEMBER(0x00B25BD0, c_gui_roster_list_widget, get_game_roster_is_local_party);
}

bool c_gui_roster_list_widget::get_team_switching_enabled() const
{
	return INVOKE_CLASS_MEMBER(0x00B25BE0, c_gui_roster_list_widget, get_team_switching_enabled);
}

//.text:00B25BF0 ; 

void c_gui_roster_list_widget::set_is_team_game(bool team_game)
{
	INVOKE_CLASS_MEMBER(0x00B25C00, c_gui_roster_list_widget, set_is_team_game, team_game);
}

void c_gui_list_widget::set_selectable_item_cap_count(int32 value)
{
	//INVOKE_CLASS_MEMBER(0x00B25C10, c_gui_list_widget, set_selectable_item_cap_count, value);

	m_selectable_item_cap = value;
}

//.text:00B25C20 ; public: void c_gui_roster_list_widget::set_team_switching_enabled(bool enabled)
//.text:00B25C30 ; public: void c_gui_roster_list_widget::start_team_change(int32 session_player_index, int32 team_index, int32 current_time)

void c_gui_roster_list_widget::update(uns32 current_milliseconds)
{
	//HOOK_INVOKE_CLASS(, c_gui_roster_list_widget, update, void(__thiscall*)(c_gui_roster_list_widget*, uns32), _this, current_milliseconds);

	int32 selectable_item_cap_count = 0;

	c_gui_roster_data* data = (c_gui_roster_data*)c_gui_list_widget::get_data();
	if (data)
	{
		c_gui_roster_list_widget::update_team_mode();

		for (c_gui_list_item_widget* list_item_widget = (c_gui_list_item_widget*)get_first_child_widget_by_type(_gui_list_item);
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

			// $TODO: figure out why this is necessary
			c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));

			int32 element_handle = list_item_widget->get_element_handle();

			int32 session_player_index = NONE;
			int32 player_row_type_index = c_gui_roster_data::_player_row_type_player;
			int32 controller_index = NONE;
			int32 voice_state_index = 0;
			int32 special_status_value = 0;
			int32 experience = NONE;
			int32 skill_level = NONE;
			int32 bungienet_user_flags = 0;
			int32 in_matchmaking_value = 0;

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

				// $TODO: figure out why this is necessary
				&& service_tag_text_widget

				&& data->get_text_value(element_handle, STRING_ID(gui, player_found), &player_found)
				&& data->get_text_value(element_handle, STRING_ID(gui, looking_for_player), &looking_for_player)
				&& data->get_text_value(element_handle, STRING_ID(global, press_a_to_join), &press_a_to_join)
				&& data->get_text_value(element_handle, STRING_ID(gui, service_tag), &service_tag)
				&& data->get_integer_value(element_handle, STRING_ID(gui, player_index), &session_player_index)
				&& data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type_index)
				&& data->get_integer_value(element_handle, STRING_ID(gui, controller_index), &controller_index)
				&& data->get_integer_value(element_handle, STRING_ID(gui, voice_output), &voice_state_index)
				&& data->get_integer_value(element_handle, STRING_ID(gui, special_status), &special_status_value)
				&& data->get_integer_value(element_handle, STRING_ID(gui, experience), &experience)
				&& data->get_integer_value(element_handle, STRING_ID(gui, skill_level), &skill_level)
				&& data->get_integer_value(element_handle, STRING_ID(gui, bungienet_user), &bungienet_user_flags)
				//&& data->get_integer_value(element_handle, STRING_ID(gui, matchmaking), &in_matchmaking_value)
				)
			{
				bool row_is_player = player_row_type_index == c_gui_roster_data::_player_row_type_player;
				bool row_is_found = player_row_type_index == c_gui_roster_data::_player_row_type_found;
				bool row_is_searching = player_row_type_index == c_gui_roster_data::_player_row_type_searching;
				bool row_is_press_a_to_join = player_row_type_index == c_gui_roster_data::_player_row_type_press_a_to_join;

				ASSERT((session_player_index == NONE) || VALID_INDEX(session_player_index, NUMBEROF(m_temporary_team)));

				bool in_matchmaking = in_matchmaking_value > 0;
				bool show_rank_tray = experience != NONE;

				bool is_team_game = m_is_team_game_internal;

				if (row_is_player)
				{
					selectable_item_cap_count++;
				}

				for (c_gui_widget* list_item_child_widget = list_item_widget->get_children();
					list_item_child_widget != NULL;
					list_item_child_widget = list_item_child_widget->get_next())
				{
					list_item_child_widget->set_use_alternate_ambient_state(special_status_value == c_gui_roster_data::_special_status_error);
				}

				list_item_widget->set_enabled(row_is_player);

				player_found_text_widget->set_text(player_found.get_string());
				player_found_text_widget->set_visible(row_is_found);
				player_found_bitmap_widget->set_visible(row_is_found);

				looking_for_player_text_widget->set_text(looking_for_player.get_string());
				looking_for_player_text_widget->set_visible(row_is_searching);
				looking_for_player_bitmap_widget->set_visible(row_is_searching);
				looking_for_player2_bitmap_widget->set_visible(row_is_searching);
				looking_for_player3_bitmap_widget->set_visible(row_is_searching);

				press_a_to_join_text_widget->set_text(press_a_to_join.get_string());
				press_a_to_join_text_widget->set_visible(row_is_press_a_to_join);
				press_a_to_join_bitmap_widget->set_visible(row_is_press_a_to_join);

				bool special_status_party_up = IN_RANGE_INCLUSIVE(special_status_value, c_gui_roster_data::_special_status_party_up_undecided_leader, c_gui_roster_data::_special_status_party_up_chosen_member);
				check_bitmap_widget->set_visible(special_status_party_up);
				party_up_bitmap_widget->set_visible(special_status_party_up);

				int32 nameplate_sprite_frame = TEST_BIT(bungienet_user_flags, 0); // _bungienet_user_registered_bit
				if (TEST_BIT(bungienet_user_flags, 1)) // _bungienet_user_pro_member_bit
				{
					nameplate_sprite_frame = 2;
				}
				else if (TEST_BIT(bungienet_user_flags, 2)) // _bungienet_user_staff_bit
				{
					nameplate_sprite_frame = 3;
				}
				base_color_bitmap_widget->set_sprite_frame(nameplate_sprite_frame);
				base_color_hilite_bitmap_widget->set_sprite_frame(nameplate_sprite_frame);

				name_text_widget->set_visible(row_is_player);
				name_hilite_text_widget->set_visible(row_is_player);

				player_emblem_bitmap_widget->set_visible(row_is_player);
				player_emblem_hilite_bitmap_widget->set_visible(row_is_player);

				// $TODO: figure out why these are necessary
				service_tag_text_widget->set_visible(row_is_player);
				service_tag_text_widget->set_text(service_tag.get_string());
				//DECLFUNC(0x00AA63A0, void, __cdecl, c_gui_widget*, bool, const wchar_t*)(list_item_widget, row_is_player, service_tag.get_string());

				skill_level_bitmap_widget->set_visible(skill_level != NONE);
				skill_level_hilite_bitmap_widget->set_visible(skill_level != NONE);
				if (skill_level != NONE)
				{
					skill_level_bitmap_widget->set_sprite_frame(skill_level);
					skill_level_hilite_bitmap_widget->set_sprite_frame(skill_level);
				}

				experience_bitmap_widget->set_visible(experience != NONE);
				experience_hilite_bitmap_widget->set_visible(experience != NONE);
				if (experience != NONE)
				{
					experience_bitmap_widget->set_sprite_frame(experience);
					experience_hilite_bitmap_widget->set_sprite_frame(experience);
					//experience_bitmap_widget->set_render_as_au2_playlist_rating(in_matchmaking);
					//experience_hilite_bitmap_widget->set_render_as_au2_playlist_rating(in_matchmaking);
				}

				rank_tray_bitmap_widget->set_visible(row_is_player);
				rank_tray_hilite_bitmap_widget->set_visible(row_is_player);
				if (row_is_player)
				{
					//rank_tray_bitmap_widget->set_render_as_au2_rank_tray(in_matchmaking);
					//rank_tray_hilite_bitmap_widget->set_render_as_au2_rank_tray(in_matchmaking);
				}

				switch (special_status_value)
				{
				case c_gui_roster_data::_special_status_party_up_undecided_leader:
				{
					check_bitmap_widget->set_sprite_frame(0);
				}
				break;
				case c_gui_roster_data::_special_status_party_up_chosen_leader:
				{
					check_bitmap_widget->set_sprite_frame(1);
				}
				break;
				case c_gui_roster_data::_special_status_party_up_chosen_member:
				{
					check_bitmap_widget->set_sprite_frame(2);
				}
				break;
				}

				if (voice_state_index == c_gui_roster_data::_voice_state_talking)
				{
					ring_of_light_bitmap_widget->set_visible(true);
					ring_of_light_bitmap_widget->set_sprite_frame(5);
				}
				else if (voice_state_index == c_gui_roster_data::_voice_state_away_in_private_chat)
				{
					ring_of_light_bitmap_widget->set_visible(true);
					ring_of_light_bitmap_widget->set_sprite_frame(6);
				}
				else if (controller_index == NONE)
				{
					if (voice_state_index == c_gui_roster_data::_voice_state_has_voice)
					{
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->set_sprite_frame(4);
					}
					else if (voice_state_index == c_gui_roster_data::_voice_state_muted)
					{
						ring_of_light_bitmap_widget->set_visible(true);
						ring_of_light_bitmap_widget->set_sprite_frame(7);
					}
					else
					{
						ring_of_light_bitmap_widget->set_visible(false);
					}
				}
				else
				{
					ring_of_light_bitmap_widget->set_visible(true);
					ring_of_light_bitmap_widget->set_sprite_frame(controller_index);
				}

				c_static_wchar_string<1024> name;
				c_static_wchar_string<256> team_name;

				bool team_change_active = false;
				if (is_team_game)
				{
					if (session_player_index != NONE && m_temporary_team[session_player_index].temporary_team_change_active)
					{
						game_engine_get_team_name(m_temporary_team[session_player_index].desired_team, &team_name);

						UNICODE_PRIVATE_FONT_ICON_STRING(left_bumper);
						UNICODE_PRIVATE_FONT_ICON_STRING(right_bumper);
						name.append_print(L"%ls %s %ls", left_bumper, team_name.get_string(), right_bumper);

						team_change_active = true;
					}
				}

				if (!team_change_active)
				{
					data->get_text_value(element_handle, STRING_ID(global, player_name), &name);
				}

				name_text_widget->set_text(name.get_string());
				name_hilite_text_widget->set_text(name.get_string());
			}
		}
	}

	if (selectable_item_cap_count)
	{
		c_gui_list_widget::set_selectable_item_cap_count(selectable_item_cap_count);
	}

	//INVOKE_CLASS_MEMBER(0x00B16510, c_gui_list_widget, update, current_milliseconds);
	c_gui_list_widget::update(current_milliseconds);
}

void c_gui_roster_list_widget::update_render_state(uns32 current_milliseconds)
{
	//HOOK_INVOKE_CLASS(, c_gui_roster_list_widget, update_render_state, void(__thiscall*)(c_gui_roster_list_widget*, uns32), _this, current_milliseconds);

	c_gui_data* data = c_gui_list_widget::get_data();

	//INVOKE_CLASS_MEMBER(0x00B165D0, c_gui_list_widget, update_render_state, current_milliseconds);
	c_gui_list_widget::update_render_state(current_milliseconds);

	if (!data)
	{
		return;
	}

	for (c_gui_list_item_widget* list_item_widget = (c_gui_list_item_widget*)get_first_child_widget_by_type(_gui_list_item);
		list_item_widget;
		list_item_widget = list_item_widget->get_next_list_item_widget(true))
	{
		c_gui_bitmap_widget* base_color_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color));
		c_gui_bitmap_widget* base_color_hilite_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color_hilite));
		c_gui_bitmap_widget* party_bar_player_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, party_bar_player));
		c_gui_text_widget* name_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, name));
		c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));

		int32 element_handle = list_item_widget->get_element_handle();

		int32 session_player_index = NONE;
		int32 player_row_type_index = c_gui_roster_data::_player_row_type_player;
		int32 party_bar_length = 0;
		int32 player_color = 0;

		if (base_color_bitmap_widget != NULL
			&& base_color_hilite_bitmap_widget != NULL
			&& party_bar_player_bitmap_widget != NULL
			&& name_text_widget != NULL
			&& service_tag_text_widget != NULL
			&& data->get_integer_value(element_handle, STRING_ID(gui, player_index), &session_player_index)
			&& data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type_index)
			&& data->get_integer_value(element_handle, STRING_ID(gui, party_bar_length), &party_bar_length)
			&& data->get_integer_value(element_handle, STRING_ID(gui, base_color), &player_color))
		{
			ASSERT((session_player_index == NONE) || VALID_INDEX(session_player_index, NUMBEROF(m_temporary_team)));

			bool row_is_player = player_row_type_index == c_gui_roster_data::_player_row_type_player;
			bool show_party_bar = row_is_player && m_show_party_bar_internal && party_bar_length > 0;

			base_color_bitmap_widget->set_visible(row_is_player);
			base_color_hilite_bitmap_widget->set_visible(row_is_player);
			party_bar_player_bitmap_widget->set_visible(show_party_bar);

			bool is_team_game = m_is_team_game_internal;

			int32 desired_team = NONE;
			if (is_team_game)
			{
				if (session_player_index == NONE
					// swap this for `get_current_team_change_team_index`?
					|| /*!get_current_team_change_is_active(session_player_index)*/ !m_temporary_team[session_player_index].temporary_team_change_active
					&& /*get_current_team_change_is_lying(session_player_index)*/ (uns32)m_temporary_team[session_player_index].lying_end_time < current_milliseconds)
				{
					int32 temporary_team_color = NONE;
					if (data)
						data->get_integer_value(element_handle, STRING_ID(global, team), &temporary_team_color);

					desired_team = temporary_team_color;
				}
				else
				{
					desired_team = m_temporary_team[session_player_index].desired_team;
				}
			}

			int32 color_list_index = player_color;
			if (is_team_game)
				color_list_index = desired_team;

			tint_widget_to_change_color(base_color_bitmap_widget, color_list_index, is_team_game);
			tint_widget_to_change_color(base_color_hilite_bitmap_widget, color_list_index, is_team_game);
			tint_widget_to_change_color(party_bar_player_bitmap_widget, color_list_index, is_team_game);
			tint_widget_to_change_color(name_text_widget, color_list_index, is_team_game);
			tint_widget_to_change_color(service_tag_text_widget, color_list_index, is_team_game);
			party_bar_player_bitmap_widget->set_sprite_frame(party_bar_length - 1);

			if (!is_team_game)
			{
				player_color |= (200 << 24); // set alpha
				tint_widget_to_change_argb_color(base_color_bitmap_widget, { .value = (uns32)player_color });

				player_color |= (150 << 24); // set alpha
				tint_widget_to_change_argb_color(base_color_hilite_bitmap_widget, { .value = (uns32)player_color });
			}
		}
	}
}

void c_gui_roster_list_widget::update_team_mode()
{
	INVOKE_CLASS_MEMBER(0x00B26210, c_gui_roster_list_widget, update_team_mode);
}

