#include "interface/gui_selected_items_saved_film.hpp"

#include "cseries/async.hpp"
#include "cseries/async_helpers.hpp"
#include "game/game_options.hpp"
#include "interface/user_interface.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/saved_game_files.hpp"

#include <cstdlib>

HOOK_DECLARE_CLASS_MEMBER(0x00ADCB00, c_gui_saved_film_selected_item, get_film_details_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADD210, c_gui_saved_film_category_datasource, initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADD3E0, c_gui_saved_film_subitem_datasource, update_autosave_enumeration_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADD560, c_gui_saved_film_subitem_datasource, update_content_enumeration_);

bool __thiscall c_gui_saved_film_selected_item::get_film_details_(game_options* options, int32* out_length_in_ticks, int32* out_start_tick)
{
	return c_gui_saved_film_selected_item::get_film_details(options, out_length_in_ticks, out_start_tick);
}

bool __thiscall c_gui_saved_film_category_datasource::initialize_(int32 name)
{
	if (!c_gui_data::initialize(name))
	{
		return false;
	}

	c_gui_saved_film_category_datasource::add_category(
		STRING_ID(gui, recent_films),
		STRING_ID(gui, recent_films_description),
		_saved_film_category_recent_films,
		c_gui_selected_item::_special_item_type_recent);

	c_gui_saved_film_category_datasource::add_category(
		STRING_ID(gui, file_share_owner),
		STRING_ID(gui, file_share_owner_description),
		_saved_film_category_none,
		c_gui_selected_item::_special_item_type_my_file_share);

	if (!get_alpha_is_internal_beta())
	{
		c_gui_saved_film_category_datasource::add_category(
			STRING_ID(gui, film_clip),
			STRING_ID(gui, film_clip_description),
			_saved_film_category_film_clips,
			c_gui_selected_item::_special_item_type_category);

		c_gui_saved_film_category_datasource::add_category(
			STRING_ID(gui, campaign_film),
			STRING_ID(gui, campaign_film_description),
			_saved_film_category_campaign,
			c_gui_selected_item::_special_item_type_category);
	}

	c_gui_saved_film_category_datasource::add_category(
		STRING_ID(gui, multiplayer_film),
		STRING_ID(gui, multiplayer_film_description),
		_saved_film_category_multiplayer,
		c_gui_selected_item::_special_item_type_category);

	if (!get_alpha_is_internal_beta())
	{
		c_gui_saved_film_category_datasource::add_category(
			STRING_ID(gui, editor_film),
			STRING_ID(gui, editor_film_description),
			_saved_film_category_editor,
			c_gui_selected_item::_special_item_type_category);

		//// ODST
		//c_gui_saved_film_category_datasource::add_category(
		//	STRING_ID(gui, survival_film),
		//	STRING_ID(gui, survival_film_description),
		//	_saved_film_category_survival,
		//	c_gui_selected_item::_special_item_type_category);
	}

	return true;
}

void __thiscall c_gui_saved_film_subitem_datasource::update_autosave_enumeration_()
{
	c_gui_saved_film_subitem_datasource::update_autosave_enumeration();
}

void __thiscall c_gui_saved_film_subitem_datasource::update_content_enumeration_()
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

void c_gui_saved_film_category_datasource::add_category(int32 title, int32 description, e_saved_film_category category, c_gui_selected_item::e_special_item_type special_item_type)
{
	INVOKE_CLASS_MEMBER(0x00ADC7D0, c_gui_saved_film_category_datasource, add_category, title, description, category, special_item_type);
}

//.text:00ADC8D0 ; public: virtual void c_gui_saved_film_subitem_datasource::dispose()

bool c_gui_saved_film_subitem_datasource::film_matches_category(s_saved_game_item_metadata* metadata)
{
	//return INVOKE_CLASS_MEMBER(0x00ADC8F0, c_gui_saved_film_subitem_datasource, film_matches_category, metadata);

	switch (m_category)
	{
	case _saved_film_category_recent_films:
	{
		return true;
	}
	break;
	case _saved_film_category_film_clips:
	{
		return metadata->get_metadata_file_type() == _metadata_file_type_film_clip;
	}
	break;
	case _saved_film_category_campaign:
	{
		return metadata->get_metadata_file_type() == _metadata_file_type_film && metadata->game_engine_index == _game_engine_type_none && !metadata->campaign_survival_enabled;
	}
	break;
	case _saved_film_category_multiplayer:
	{
		return metadata->get_metadata_file_type() == _metadata_file_type_film && metadata->campaign_id == _campaign_id_none && metadata->game_engine_index != _game_engine_type_sandbox;
	}
	break;
	case _saved_film_category_editor:
	{
		return metadata->get_metadata_file_type() == _metadata_file_type_film && metadata->campaign_id == _campaign_id_none && metadata->game_engine_index == _game_engine_type_sandbox;
	}
	break;
	case _saved_film_category_survival:
	{
		return metadata->get_metadata_file_type() == _metadata_file_type_film && metadata->game_engine_index == _game_engine_type_none && metadata->campaign_survival_enabled;
	}
	break;
	}

	return false;
}

//.text:00ADC9C0 ; public: virtual void c_gui_saved_film_category_datasource::get_column_names(int32* const, int32*)
//.text:00ADCA00 ; public: virtual void c_gui_saved_film_subitem_datasource::get_column_names(int32* const, int32*)
//.text:00ADCAA0 ; 
//.text:00ADCAB0 ; 
//.text:00ADCAC0 ; protected: virtual int32 c_gui_saved_film_category_datasource::get_current_item_count_internal()
//.text:00ADCAD0 ; protected: virtual int32 c_gui_saved_film_subitem_datasource::get_current_item_count_internal()

bool c_gui_saved_film_selected_item::get_file_path(wchar_t* path, int32 maximum_characters) const
{
	//return INVOKE_CLASS_MEMBER(0x00ADCAE0, c_gui_saved_film_selected_item, get_file_path, path, maximum_characters);

	file_reference_get_fullpath_wide(&m_file_reference, path, maximum_characters);
	return true;
}

bool c_gui_saved_film_selected_item::get_film_details(game_options* options, int32* out_length_in_ticks, int32* out_start_tick) const
{
	//return INVOKE_CLASS_MEMBER(0x00ADCB00, c_gui_saved_film_selected_item, get_film_details, options, out_length_in_ticks, out_start_tick);

	s_blf_saved_film film_on_disk{};

	switch (c_gui_saved_film_selected_item::get_location())
	{
	case _gui_stored_item_location_saved_game_file:
	case _gui_stored_item_location_autosave_queue:
	{
		c_synchronized_long success = 0;
		c_synchronized_long done = 0;
		if (autosave_queue_read_file(&m_file_reference, &film_on_disk, sizeof(s_blf_saved_film), &success, &done) == NONE)
		{
			break;
		}
		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
		if (success.peek() != 1)
		{
			break;
		}

		bool was_valid = false;
		if (!film_on_disk.copy_to_and_validate(options, out_length_in_ticks, out_start_tick, &was_valid))
		{
			printf("");
		}
		return was_valid;
	}
	break;
	}

	return false;
}

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

	static s_find_file_data find_file_data{};
	static s_file_reference saved_films_files[512]{};
	static int32 saved_films_file_count = 0;
	static c_synchronized_long success;
	static c_synchronized_long done;
	static int32 task_id = NONE;

	task_id = async_enumerate_files(
		0,
		"saved_films",
		512,
		&find_file_data,
		saved_films_files,
		&saved_films_file_count,
		_async_category_saved_games,
		_async_priority_very_important_non_blocking,
		&success,
		&done);

	m_enumeration_complete = success.peek();
	if (!m_enumeration_complete)
	{
		return;
	}

	s_saved_game_item_enumeration_data item{};
	for (int32 saved_films_file_index = 0; saved_films_file_index < saved_films_file_count; saved_films_file_index++)
	{
		if (m_saved_film_count >= k_maximum_saved_films_shown)
		{
			break;
		}

		s_file_reference* saved_films_file = &saved_films_files[saved_films_file_index];
		if (file_is_directory(saved_films_file))
		{
			continue;
		}

		item.file = *saved_films_file;

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

		bool exists = false;
		for (int32 saved_film_index = 0; !exists && saved_film_index < m_saved_film_count; saved_film_index++)
		{
			exists = item.metadata.unique_id == m_saved_films[saved_film_index].m_metadata.unique_id;
		}

		if (exists)
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
			item.metadata.date > get_current_time_in_seconds(_one_week_in_seconds));
	}

	//m_saved_films.sort(m_saved_film_count, saved_film_sort_proc);
	qsort(&m_saved_films, m_saved_film_count, sizeof(c_gui_saved_film_selected_item), saved_film_sort_proc);

	csmemset(&find_file_data, 0, sizeof(s_find_file_data));
	csmemset(saved_films_files, 0, sizeof(s_file_reference) * 512);
	saved_films_file_count = 0;
}

