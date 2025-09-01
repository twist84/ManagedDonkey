#pragma once

#include "interface/gui_selected_items.hpp"

enum e_network_mode_category
{
	_network_mode_xbox_live_private = 0,
	_network_mode_system_link,
	_network_mode_offline,
	k_network_mode_category_count,
};

enum e_network_mode_choice
{
	_network_mode_choice_xbox_live_advertise_public = 0,
	_network_mode_choice_xbox_live_advertise_friends,
	_network_mode_choice_xbox_live_advertise_invite_only,
	_network_mode_choice_system_link_advertise,
	_network_mode_choice_system_link_browse,
	_network_mode_choice_offline,
};


class c_gui_network_mode_selected_item :
	public c_gui_selected_item
{
public:
	e_network_mode_choice get_network_choice() const
	{
		return m_network_choice;
	}

	e_network_mode_category get_network_mode() const
	{
		return m_network_mode;
	}

//protected:
	e_network_mode_category m_network_mode;
	e_network_mode_choice m_network_choice;
};
static_assert(sizeof(c_gui_network_mode_selected_item) == sizeof(c_gui_selected_item) + 0x8);
static_assert(0x190 == OFFSETOF(c_gui_network_mode_selected_item, m_network_mode));
static_assert(0x194 == OFFSETOF(c_gui_network_mode_selected_item, m_network_choice));

