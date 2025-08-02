#include "interface/gui_selected_items_saved_film.hpp"


//.text:00ADC230 ; public: c_static_array<c_gui_saved_film_selected_item, 151>::c_static_array<c_gui_saved_film_selected_item, 151>()
//.text:00ADC2C0 ; public: c_gui_saved_film_category_datasource::c_gui_saved_film_category_datasource(c_gui_screen_widget*)

c_gui_saved_film_selected_item::c_gui_saved_film_selected_item(const s_saved_game_item_metadata* metadata, e_saved_film_category category, e_gui_selected_item_location location, e_controller_index controller_index, const s_file_reference* file_reference, int32 sort_order, bool corrupt, bool is_new)
{
	DECLFUNC(0x00ADC2F0, void, __thiscall, c_gui_saved_film_selected_item*, const s_saved_game_item_metadata*, e_saved_film_category, e_gui_selected_item_location, e_controller_index, const s_file_reference*, int32, bool, bool)(
		this, metadata, category, location, controller_index, file_reference, sort_order, corrupt, is_new);
}

//.text:00ADC360 ; public: c_gui_saved_film_selected_item::c_gui_saved_film_selected_item(const wchar_t*, const wchar_t*, e_saved_film_category, c_gui_selected_item::e_special_item_type, bool, bool)
//.text:00ADC3B0 ; public: c_gui_saved_film_selected_item::c_gui_saved_film_selected_item()
//.text:00ADC420 ; public: c_gui_saved_film_subitem_datasource::c_gui_saved_film_subitem_datasource(e_controller_index, e_saved_film_category)
//.text:00ADC4A0 ; public: s_blf_saved_film::s_blf_chunk_saved_film_data::s_blf_chunk_saved_film_data()
//.text:00ADC4C0 ; public: s_blf_saved_film::s_blf_chunk_saved_film_header::s_blf_chunk_saved_film_header()
//.text:00ADC520 ; public: s_blf_saved_film::s_blf_saved_film()
//.text:00ADC5C0 ; 
//.text:00ADC5D0 ; public: virtual c_gui_saved_film_subitem_datasource::~c_gui_saved_film_subitem_datasource()
//.text:00ADC620 ; 
//.text:00ADC630 ; 

c_gui_saved_film_selected_item& c_gui_saved_film_selected_item::operator=(const c_gui_saved_film_selected_item& __that)
{
	return DECLFUNC(0x00ADC640, c_gui_saved_film_selected_item&, __thiscall, c_gui_saved_film_selected_item*, const c_gui_saved_film_selected_item&)(
		this, __that);
}

//.text:00ADC6D0 ; 
//.text:00ADC6F0 ; 
//.text:00ADC710 ; 
//.text:00ADC730 ; public: virtual void* c_gui_saved_film_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADC760 ; public: virtual void* c_gui_saved_film_subitem_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADC7D0 ; private: void c_gui_saved_film_category_datasource::add_category(int32, int32, e_saved_film_category, c_gui_selected_item::e_special_item_type)
//.text:00ADC8D0 ; public: virtual void c_gui_saved_film_subitem_datasource::dispose()

//.text:00ADC9C0 ; public: virtual void c_gui_saved_film_category_datasource::get_column_names(int32* const, int32*)
//.text:00ADCA00 ; public: virtual void c_gui_saved_film_subitem_datasource::get_column_names(int32* const, int32*)
//.text:00ADCAA0 ; 
//.text:00ADCAB0 ; 
//.text:00ADCAC0 ; protected: virtual int32 c_gui_saved_film_category_datasource::get_current_item_count_internal()
//.text:00ADCAD0 ; protected: virtual int32 c_gui_saved_film_subitem_datasource::get_current_item_count_internal()
//.text:00ADCAE0 ; public: virtual bool c_gui_saved_film_selected_item::get_file_path(wchar_t*, int32) const
//.text:00ADCB00 ; public: bool c_gui_saved_film_selected_item::get_film_details(game_options*, int32*, int32*) const
//.text:00ADCCD0 ; public: e_game_engine_type c_gui_saved_film_selected_item::get_game_engine_type() const
//.text:00ADCCE0 ; public: virtual const c_gui_selected_item* c_gui_saved_film_subitem_datasource::get_gui_selected_item(int32) const
//.text:00ADCD10 ; public: virtual bool c_gui_saved_film_category_datasource::get_integer_value(int32, int32, int32*)
//.text:00ADCDD0 ; public: virtual bool c_gui_saved_film_subitem_datasource::get_integer_value(int32, int32, int32*)
//.text:00ADD070 ; public: uns64 c_gui_saved_film_selected_item::get_sort_order() const
//.text:00ADD080 ; public: virtual bool c_gui_saved_film_category_datasource::get_string_id_value(int32, int32, int32*)
//.text:00ADD0A0 ; public: virtual bool c_gui_saved_film_subitem_datasource::get_string_id_value(int32, int32, int32*)
//.text:00ADD160 ; public: virtual bool c_gui_saved_film_category_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00ADD1C0 ; public: virtual bool c_gui_saved_film_subitem_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00ADD210 ; public: virtual bool c_gui_saved_film_category_datasource::initialize(int32)
//.text:00ADD290 ; public: virtual bool c_gui_saved_film_subitem_datasource::initialize(int32)
//.text:00ADD2D0 ; public: virtual bool c_gui_saved_film_subitem_datasource::is_busy() const

//.text:00ADD3A0 ; public: void c_static_array<c_gui_saved_film_selected_item, 151>::sort(long, int(__cdecl*)(const void *, const void*))
//.text:00ADD3C0 ; public: virtual void c_gui_saved_film_subitem_datasource::update()

