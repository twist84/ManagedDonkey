#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_animation.hpp"

enum e_gui_widget_type
{
	_gui_widget_type_text_widget = 0,
	_gui_widget_type_bitmap_widget,
	_gui_widget_type_model_widget,
	_gui_widget_type_group_widget,
	_gui_widget_type_button_key_widget,
	_gui_widget_type_list_item_widget,

	// unused?
	_gui_widget_type_unknown_widget,

	_gui_widget_type_list_widget,
	_gui_widget_type_screen_widget,

	k_gui_widget_type_count
};

struct c_gui_widget
{
	void* __vftable;

	long __unknown4;
	c_enum<e_gui_widget_type, long, k_gui_widget_type_count> m_type;

	bool m_visible;
	bool m_enabled;
	bool __unknownE;
	bool m_use_alternate_ambient_state;

	struct s_unknown
	{
		c_gui_widget* __unknown0;
		c_gui_widget* __unknown4;
		c_gui_widget* __unknown8;
	};

	s_unknown m_parent;
	s_unknown m_children;
	s_unknown m_next;
	s_unknown m_previous;

	//c_gui_widget* m_parent;
	//c_gui_widget* m_children;
	//c_gui_widget* m_next;
	//c_gui_widget* m_previous;

	c_string_id m_name;
	real __unknown44;
	dword_flags m_flags;
	long __unknown4C;
	s_animation_transform m_full_animation_state;
	long __unknownD8;
};
static_assert(sizeof(c_gui_widget) == 0xDC);
