#pragma once

#include "interface/c_gui_widget.hpp"

struct s_list_item_widget_block :
	s_core_widget_definition
{
	c_string_id item_text_identifier;
};
COMPILE_ASSERT(sizeof(s_list_item_widget_block) == sizeof(s_core_widget_definition) + 0x4);

struct s_runtime_list_item_widget_definition :
	s_runtime_core_widget_definition
{
	s_runtime_list_item_widget_definition();

	c_string_id item_text_identifier;
};
COMPILE_ASSERT(sizeof(s_runtime_list_item_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x4);

class c_gui_list_item_widget :
	public c_gui_widget
{
public:
	virtual ~c_gui_list_item_widget() = default;
	virtual int32 get_element_handle() override;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual bool should_render(bool* add_to_render_list) override;
	virtual bool can_receive_focus() override;
	virtual c_gui_text_widget* create_text_widget(const s_runtime_text_widget_definition* definition) override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual c_gui_group_widget* create_group_widget(const s_group_widget_definition* definition) override;
	virtual c_gui_button_key_widget* create_button_key_widget(const s_button_key_definition* definition) override;
	virtual c_gui_list_widget* create_list_widget(const s_list_widget_block* definition) override;
	virtual c_gui_list_item_widget* create_list_item_widget(const s_list_item_widget_block* definition) override;
	virtual c_gui_model_widget* create_model_widget(const s_model_widget_block* definition) override;
	virtual bool handle_widget_back_out() override;
	virtual bool handle_widget_selected() override;
	virtual int32 get_list_item_index();
	virtual void initialize(const s_list_item_widget_block* item_block);

public:
	c_gui_list_item_widget();
	int32 get_item_label();
	bool invokes_submenu(int32* submenu_name);
	void set_list_item_index(int32 list_item_index);

//protected:
	s_runtime_list_item_widget_definition m_definition;
	int32 m_list_item_index;
	bool __unknown11C;
	c_static_wchar_string<200> __string11E;
};
COMPILE_ASSERT(sizeof(c_gui_list_item_widget) == 0x2B0);
COMPILE_ASSERT(sizeof(c_gui_list_item_widget) == sizeof(c_gui_widget) + 0x1D0);
COMPILE_ASSERT(0x0E0 == OFFSETOF(c_gui_list_item_widget, m_definition));
COMPILE_ASSERT(0x118 == OFFSETOF(c_gui_list_item_widget, m_list_item_index));
COMPILE_ASSERT(0x11C == OFFSETOF(c_gui_list_item_widget, __unknown11C));
COMPILE_ASSERT(0x11E == OFFSETOF(c_gui_list_item_widget, __string11E));

