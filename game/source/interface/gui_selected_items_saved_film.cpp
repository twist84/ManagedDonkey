#include "interface/gui_selected_items_saved_film.hpp"

#include "memory/module.hpp"
#include "saved_games/saved_game_files.hpp"

#include <cstdlib>

HOOK_DECLARE_CLASS_MEMBER(0x00ADD3E0, c_gui_saved_film_subitem_datasource, update_autosave_enumeration_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADD560, c_gui_saved_film_subitem_datasource, update_content_enumeration_);

void c_gui_saved_film_subitem_datasource::update_autosave_enumeration_()
{
	c_gui_saved_film_subitem_datasource::update_autosave_enumeration();
}

void c_gui_saved_film_subitem_datasource::update_content_enumeration_()
{
	c_gui_saved_film_subitem_datasource::update_content_enumeration();
}

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

bool c_gui_saved_film_subitem_datasource::film_matches_category(s_saved_game_item_metadata* metadata)
{
	return INVOKE_CLASS_MEMBER(0x00ADC8F0, c_gui_saved_film_subitem_datasource, film_matches_category, metadata);
}

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

int __cdecl saved_film_sort_proc(const void* a, const void* b)
{
	return INVOKE(0x00ADD2E0, saved_film_sort_proc, a, b);
}

//.text:00ADD3A0 ; public: void c_static_array<c_gui_saved_film_selected_item, 151>::sort(long, int(__cdecl*)(const void *, const void*))
//.text:00ADD3C0 ; public: virtual void c_gui_saved_film_subitem_datasource::update()

void c_gui_saved_film_subitem_datasource::update_autosave_enumeration()
{
	//INVOKE_CLASS_MEMBER(0x00ADD3E0, c_gui_saved_film_subitem_datasource, update_autosave_enumeration);

	if (m_enumeration_complete)
	{
		return;
	}

	while (m_autosave_enumerator.are_items_available())
	{
		s_saved_game_item_enumeration_data item{};
		if (!m_autosave_enumerator.dequeue_item(&item))
		{
			break;
		}

		if (item.state != s_saved_game_item_enumeration_data::_item_state_ready)
		{
			continue;
		}

		if (!c_gui_saved_film_subitem_datasource::film_matches_category(&item.metadata))
		{
			continue;
		}

		if (m_saved_film_count >= k_maximum_saved_films_shown)
		{
			break;
		}

		m_saved_films[m_saved_film_count++] = c_gui_saved_film_selected_item(
			&item.metadata,
			m_category,
			_gui_stored_item_location_autosave_queue,
			m_controller_index,
			&item.file,
			0,
			item.state == s_saved_game_item_enumeration_data::_item_state_corrupt,
			false);
	}

	//m_saved_films.sort(m_saved_film_count, saved_film_sort_proc);
	qsort(&m_saved_films, m_saved_film_count, sizeof(c_gui_saved_film_selected_item), saved_film_sort_proc);

	if (!m_autosave_enumerator.is_busy() && !m_autosave_enumerator.are_items_available())
	{
		m_enumeration_complete = true;
	}
}

void c_gui_saved_film_subitem_datasource::update_content_enumeration()
{
	//INVOKE_CLASS_MEMBER(0x00ADD560, c_gui_saved_film_subitem_datasource, update_content_enumeration);

	// $TODO: properly implement content items and un/reimplement this function at that time

	if (m_enumeration_complete)
	{
		return;
	}

	s_file_reference saved_films_directory{};
	file_reference_create_from_path_wide(&saved_films_directory, L"saved_films", true);

	s_find_file_data find_file_data{};
	find_files_start_with_search_spec(&find_file_data, 0, &saved_films_directory, "*.film");

	s_saved_game_item_enumeration_data item{};
	while (find_files_next(&find_file_data, &item.file, NULL))
	{
		if (m_saved_film_count >= k_maximum_saved_films_shown)
		{
			break;
		}

		if (!saved_game_read_metadata_from_file(&item.file, &item.metadata))
		{
			continue;
		}

		if (item.metadata.file_type != _saved_game_film && item.metadata.file_type != _saved_game_film_clip)
		{
			continue;
		}

		if (!c_gui_saved_film_subitem_datasource::film_matches_category(&item.metadata))
		{
			continue;
		}

		m_saved_films[m_saved_film_count++] = c_gui_saved_film_selected_item(
			&item.metadata,
			m_category,
			_gui_stored_item_location_saved_game_file,
			m_controller_index,
			&item.file,
			0,
			item.state == s_saved_game_item_enumeration_data::_item_state_corrupt,
			true);
	}
	find_files_end(&find_file_data);

	//m_saved_films.sort(m_saved_film_count, saved_film_sort_proc);
	qsort(&m_saved_films, m_saved_film_count, sizeof(c_gui_saved_film_selected_item), saved_film_sort_proc);

	m_enumeration_complete = true;
}

