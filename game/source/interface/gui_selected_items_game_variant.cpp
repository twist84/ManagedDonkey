#include "interface/gui_selected_items_game_variant.hpp"

//.text:00ADAC20 ; public: c_gui_game_variant_selected_item::c_gui_game_variant_selected_item()
//.text:00ADACA0 ; public: c_gui_game_variant_category_datasource::c_gui_game_variant_category_datasource(c_gui_screen_widget*)
//.text:00ADAD60 ; public: c_gui_game_variant_selected_item::c_gui_game_variant_selected_item(const s_ui_saved_game_item_metadata*, e_gui_selected_item_location, e_controller_index, const s_file_reference*, int32, int32, bool, bool)
//.text:00ADADD0 ; public: c_gui_game_variant_selected_item::c_gui_game_variant_selected_item()
//.text:00ADAE30 ; public: c_gui_game_variant_subitem_selectable_item_datasource::c_gui_game_variant_subitem_selectable_item_datasource()
//.text:00ADAEB0 ; public: s_blf_chunk_game_variant::s_blf_chunk_game_variant()
//.text:00ADAEE0 ; public: s_blffile_game_variant::s_blffile_game_variant()
//.text:00ADAF30 ; 
//.text:00ADAF40 ; 
//.text:00ADAF50 ; public: virtual c_gui_game_variant_subitem_selectable_item_datasource::~c_gui_game_variant_subitem_selectable_item_datasource()
//.text:00ADAFA0 ; public: c_gui_game_variant_selected_item& c_gui_game_variant_selected_item::operator=(const c_gui_game_variant_selected_item&)
//.text:00ADB020 ; 
//.text:00ADB040 ; 
//.text:00ADB060 ; 
//.text:00ADB080 ; 
//.text:00ADB090 ; public: virtual void* c_gui_game_variant_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADB0C0 ; public: virtual void* c_gui_game_variant_subitem_selectable_item_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADB130 ; private: void c_gui_game_variant_category_datasource::add_game_engine(c_gui_data*, e_game_engine_type)
//.text:00ADB260 ; public: virtual void c_gui_game_variant_subitem_selectable_item_datasource::dispose()
//.text:00ADB280 ; int __cdecl game_variant_category_sort_proc(const void*, const void*)
//.text:00ADB2C0 ; int __cdecl game_variant_selected_item_sort_proc(const void*, const void*)
//.text:00ADB3B0 ; public: virtual void c_gui_game_variant_category_datasource::get_column_names(int32* const, int32*)
//.text:00ADB3F0 ; public: virtual void c_gui_game_variant_subitem_selectable_item_datasource::get_column_names(int32* const, int32*)
//.text:00ADB480 ; 
//.text:00ADB490 ; protected: virtual int32 c_gui_game_variant_category_datasource::get_current_item_count_internal()
//.text:00ADB4A0 ; protected: virtual int32 c_gui_game_variant_subitem_selectable_item_datasource::get_current_item_count_internal()
//.text:00ADB4B0 ; public: virtual bool c_gui_game_variant_selected_item::get_file_path(wchar_t*, int32) const
//.text:00ADB4F0 ; private: int32 c_gui_game_variant_category_datasource::get_game_engine_sort_order(c_gui_data*, int32)
//.text:00ADB580 ; 
//.text:00ADB590 ; public: bool c_gui_game_variant_selected_item::get_game_variant(c_game_variant*) const
//.text:00ADB780 ; public: virtual const c_gui_selected_item* c_gui_game_variant_subitem_selectable_item_datasource::get_gui_selected_item(int32) const
//.text:00ADB7B0 ; public: virtual bool c_gui_game_variant_category_datasource::get_integer_value(int32, int32, int32*)
//.text:00ADB870 ; public: virtual bool c_gui_game_variant_subitem_selectable_item_datasource::get_integer_value(int32, int32, int32*)
//.text:00ADB9F0 ; 
//.text:00ADBA00 ; public: virtual bool c_gui_game_variant_category_datasource::get_string_id_value(int32, int32, int32*)
//.text:00ADBA20 ; public: virtual bool c_gui_game_variant_subitem_selectable_item_datasource::get_string_id_value(int32, int32, int32*)
//.text:00ADBAD0 ; public: virtual bool c_gui_game_variant_category_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00ADBB30 ; public: virtual bool c_gui_game_variant_subitem_selectable_item_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00ADBB80 ; public: virtual bool c_gui_game_variant_category_datasource::initialize(int32)
//.text:00ADBD20 ; public: virtual bool c_gui_game_variant_subitem_selectable_item_datasource::initialize(int32, e_game_engine_type, e_controller_index)
//.text:00ADBEC0 ; public: virtual bool c_gui_game_variant_subitem_selectable_item_datasource::is_busy() const
//.text:00ADBED0 ; 
//.text:00ADBEF0 ; 
//.text:00ADBF10 ; public: virtual void c_gui_game_variant_subitem_selectable_item_datasource::update()
//.text:00ADBF30 ; protected: void c_gui_game_variant_subitem_selectable_item_datasource::update_autosave_enumeration()
//.text:00ADC0A0 ; protected: void c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration()

