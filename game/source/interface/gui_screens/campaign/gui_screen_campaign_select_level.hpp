#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"

class c_gui_insertion_point_data :
	public c_gui_ordered_data
{
public:
	virtual ~c_gui_insertion_point_data() = default;
	virtual bool initialize(string_id name) override;
	virtual void update() override;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;

protected:
	virtual int32 get_current_item_count_internal() override;

public:
	c_gui_insertion_point_data(int32 string_list_index);
	int16 set_campaign_level_id(e_campaign_id campaign_id, e_map_id map_id);

protected:
	wchar_t m_insertion_point_names[k_max_campaign_insertion_points][32];
	wchar_t m_insertion_point_descriptions[k_max_campaign_insertion_points][128];
	int16 m_insertion_point_count;
	int16 m_insertion_point_unlocked;
	int32 m_string_list_index;
};
//static_assert(sizeof(c_gui_insertion_point_data) == sizeof(c_gui_ordered_data) + 0xB48);

class c_gui_screen_campaign_select_level :
	public c_gui_screen_widget
{
public:
	void __thiscall post_initialize_();
	void __thiscall initialize_datasource_();
	bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);

public:
	virtual ~c_gui_screen_campaign_select_level() = default;
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;

public:
	c_gui_screen_campaign_select_level(int32 name);

private:
	void level_chosen_immediate(e_controller_index controller_index);

public:
	void setup(e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, int16 campaign_insertion_point);

//protected:
	e_gui_campaign_level_setup_mode m_campaign_setup_mode;
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	int16 m_campaign_insertion_point;
	e_controller_index m_controller_index;
	bool m_saved_game;
	bool m_coop_saved_game;
};
static_assert(sizeof(c_gui_screen_campaign_select_level) == sizeof(c_gui_screen_widget) + 0x18);

