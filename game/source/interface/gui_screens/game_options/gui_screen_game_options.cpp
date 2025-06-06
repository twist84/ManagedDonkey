#include "interface/gui_screens/game_options/gui_screen_game_options.hpp"

//.text:00B0AEA0 ; 
//.text:00B0AEC0 ; 
//.text:00B0AEE0 ; 
//.text:00B0AEF0 ; 
//.text:00B0AF00 ; public: c_game_options_category_datasource::c_game_options_category_datasource(e_game_engine_category, e_game_engine_category, const c_game_variant*)
//.text:00B0AF40 ; public: c_game_options_parameter_datasource::c_game_options_parameter_datasource(e_game_variant_parameter)
//.text:00B0AF70 ; public: c_gui_screen_game_options::c_gui_screen_game_options(int32)
//.text:00B0B140 ; public: c_load_game_options_screen_message::c_load_game_options_screen_message(int32, e_controller_index, e_window_index, int32, const c_game_variant*, const c_game_variant*, e_game_engine_category, e_game_engine_category, int32, s_game_options_history_stack_node*, int32)
//.text:00B0B270 ; 
//.text:00B0B280 ; 
//.text:00B0B290 ; 
//.text:00B0B2A0 ; public: virtual c_gui_screen_game_options::~c_gui_screen_game_options()
//.text:00B0B300 ; 
//.text:00B0B310 ; public: virtual void* c_game_options_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00B0B340 ; public: virtual void* c_game_options_parameter_datasource::`vector deleting destructor'(unsigned int)
//.text:00B0B370 ; public: virtual void* c_gui_screen_game_options::`vector deleting destructor'(unsigned int)
//.text:00B0B3E0 ; public: virtual void* c_load_game_options_screen_message::`vector deleting destructor'(unsigned int)
//.text:00B0B410 ; private: bool c_gui_screen_game_options::allow_user_to_transition()
//.text:00B0B4D0 ; public: void c_gui_screen_game_options::apply_initial_state(const c_game_variant*, const c_game_variant*, e_game_engine_category, e_game_engine_category, int32, const s_game_options_history_stack_node*, int32, int32)
//.text:00B0B590 ; public: virtual void c_load_game_options_screen_message::apply_initial_state(c_gui_screen_widget*) const
//.text:00B0B5D0 ; protected: bool c_gui_screen_game_options::back_out_from_current_editing_category(e_controller_index)
//.text:00B0B780 ; protected: void c_gui_screen_game_options::begin_rename_edited_variant(e_controller_index)
//.text:00B0B920 ; protected: void c_gui_screen_game_options::begin_save_as_current_edited_variant(e_controller_index)
//.text:00B0BAE0 ; protected: void c_gui_screen_game_options::begin_save_current_edited_variant(e_controller_index)
//.text:00B0BC30 ; 
//.text:00B0BC40 ; 
//.text:00B0BC70 ; 
//.text:00B0BC80 ; protected: void c_gui_screen_game_options::finish_editing_setting()
//.text:00B0BC90 ; public: virtual void c_game_options_category_datasource::get_column_names(int32* const, int32*)
//.text:00B0BCB0 ; public: virtual void c_game_options_parameter_datasource::get_column_names(int32* const, int32*)
//.text:00B0BCF0 ; protected: virtual int32 c_game_options_category_datasource::get_current_item_count_internal()
//.text:00B0BD20 ; protected: virtual int32 c_game_options_parameter_datasource::get_current_item_count_internal()
//.text:00B0BD40 ; 
//.text:00B0BD50 ; public: virtual bool c_game_options_category_datasource::get_element(int32, void*, int32)
//.text:00B0BDC0 ; public: virtual bool c_game_options_parameter_datasource::get_integer_value(int32, int32, int32*)
//.text:00B0BE10 ; public: e_game_engine_category c_gui_screen_game_options::get_interface_setting() const
//.text:00B0BE30 ; public: virtual bool c_game_options_category_datasource::get_string_id_value(int32, int32, int32*)
//.text:00B0C010 ; public: virtual bool c_game_options_parameter_datasource::get_string_id_value(int32, int32, int32*)
//.text:00B0C090 ; public: e_game_engine_category c_gui_screen_game_options::get_template_setting() const
//.text:00B0C0B0 ; public: virtual bool c_gui_screen_game_options::handle_controller_input_message(const c_controller_input_message*)
//.text:00B0C200 ; public: virtual bool c_gui_screen_game_options::handle_dialog_result(const c_dialog_result_message*)
//.text:00B0C470 ; public: virtual bool c_gui_screen_game_options::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00B0CA70 ; public: virtual void c_gui_screen_game_options::initialize()
//.text:00B0CB20 ; public: virtual void c_gui_screen_game_options::initialize_datasource()
//.text:00B0CD10 ; protected: void c_gui_screen_game_options::load_progress_ui(e_controller_index)
//.text:00B0CDB0 ; bool __cdecl parse_current_selected_item_nugget(void*, int32, wchar_t*, int32)
//.text:00B0CEA0 ; bool __cdecl parse_current_selected_name(void*, wchar_t*, int32)
//.text:00B0CEC0 ; bool __cdecl parse_current_selected_value(void*, wchar_t*, int32)
//.text:00B0CEE0 ; public: virtual void c_gui_screen_game_options::post_initialize()
//.text:00B0CF30 ; protected: void c_gui_screen_game_options::propagate_changes_from_newly_created_game_variant_file(e_controller_index, int32, e_saved_game_file_type)
//.text:00B0D070 ; protected: void c_gui_screen_game_options::propagate_changes_to_currently_selected_game_variant_file(e_controller_index)
//.text:00B0D180 ; protected: void c_gui_screen_game_options::revert_current_edited_variant(e_controller_index)
//.text:00B0D190 ; protected: bool c_gui_screen_game_options::saved_game_variant_file_already_exists(e_controller_index, const wchar_t*) const
//.text:00B0D1C0 ; protected: void c_gui_screen_game_options::set_editing_parameter(e_game_variant_parameter)
//.text:00B0D1D0 ; 
//.text:00B0D1F0 ; private: void c_gui_screen_game_options::setup_game_variant_for_write(s_blffile_game_variant*, const wchar_t*, const c_game_variant*)
//.text:00B0D330 ; private: bool c_gui_screen_game_options::start_create_game_variant(const wchar_t*)
//.text:00B0D3E0 ; 
//.text:00B0D410 ; public: virtual void c_gui_screen_game_options::update(uns32)
//.text:00B0D520 ; public: void c_gui_screen_game_options::update_rename_operation()
//.text:00B0D8C0 ; public: void c_gui_screen_game_options::update_revert_changes_operation()
//.text:00B0D900 ; public: void c_gui_screen_game_options::update_save_as_new_operation()
//.text:00B0DED0 ; public: void c_gui_screen_game_options::update_save_operation()
//.text:00B0DFF0 ; protected: bool c_gui_screen_game_options::user_is_editing_setting() const

