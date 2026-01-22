#pragma once

#include "interface/c_gui_screen_widget.hpp"

class c_gui_screen_campaign_settings :
	public c_gui_screen_widget
{
public:
	enum e_focus_state
	{
		_focus_state_none = 0,
		_focus_state_primary,
		_focus_state_secondary,
	};

public:
	virtual ~c_gui_screen_campaign_settings() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* dialog_result_message) override;

public:
	c_gui_screen_campaign_settings(int32 name);
	void set_focus(e_focus_state new_state, int16 new_index);
	bool warn_about_loss_of_saved_game_progress(e_controller_index controller_index);
};
COMPILE_ASSERT(sizeof(c_gui_screen_campaign_settings) == sizeof(c_gui_screen_widget));

