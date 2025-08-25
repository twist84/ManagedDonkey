#include "interface/gui_selected_items_map.hpp"

#include "cseries/async.hpp"
#include "cseries/async_helpers.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/saved_game_files.hpp"

#include <cstdlib>

HOOK_DECLARE_CLASS_MEMBER(0x00AD9A30, c_gui_map_selected_item, get_map_variant_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADA360, c_gui_map_subitem_selectable_item_datasource, update_autosave_enumeration_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADA550, c_gui_map_subitem_selectable_item_datasource, update_content_enumeration_);

bool __thiscall c_gui_map_selected_item::get_map_variant_(c_map_variant* map_variant)
{
	return c_gui_map_selected_item::get_map_variant(map_variant);
}

void __thiscall c_gui_map_subitem_selectable_item_datasource::update_autosave_enumeration_()
{
	c_gui_map_subitem_selectable_item_datasource::update_autosave_enumeration();
}

void __thiscall c_gui_map_subitem_selectable_item_datasource::update_content_enumeration_()
{
	c_gui_map_subitem_selectable_item_datasource::update_content_enumeration();
}

//.text:00AD9140 ; public: c_static_array<c_gui_map_selected_item, 151>::c_static_array<c_gui_map_selected_item, 151>()
//.text:00AD9190 ; 
//.text:00AD91B0 ; public: c_gui_map_category_datasource::c_gui_map_category_datasource(const wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*)

c_gui_map_selected_item::c_gui_map_selected_item(const s_ui_saved_game_item_metadata* metadata, e_gui_selected_item_location location, e_controller_index controller_index, const s_file_reference* file_reference, int32 sort_order, e_special_item_type special_item_type, bool corrupt, bool is_new)
{
	DECLFUNC(0x00AD9270, void, __thiscall, c_gui_map_selected_item*, const s_ui_saved_game_item_metadata*, e_gui_selected_item_location, e_controller_index, const s_file_reference*, int32, e_special_item_type, bool, bool)(
		this, metadata, location, controller_index, file_reference, sort_order, special_item_type, corrupt, is_new);
}

//.text:00AD92F0 ; public: c_gui_map_selected_item::c_gui_map_selected_item()
//.text:00AD9320 ; public: c_gui_map_subitem_selectable_item_datasource::c_gui_map_subitem_selectable_item_datasource()
//.text:00AD93A0 ; public: s_blf_chunk_map_variant::s_blf_chunk_map_variant()
//.text:00AD9400 ; public: s_blffile_map_variant::s_blffile_map_variant()
//.text:00AD9480 ; 
//.text:00AD9490 ; 
//.text:00AD94A0 ; public: virtual c_gui_map_subitem_selectable_item_datasource::~c_gui_map_subitem_selectable_item_datasource()
//.text:00AD94F0 ; 
//.text:00AD9500 ; 

c_gui_map_selected_item& c_gui_map_selected_item::operator=(const c_gui_map_selected_item& __that)
{
	return DECLFUNC(0x00AD9510, c_gui_map_selected_item&, __thiscall, c_gui_map_selected_item*, const c_gui_map_selected_item&)(
		this, __that);
}

//.text:00AD9590 ; 
//.text:00AD95B0 ; 
//.text:00AD95D0 ; 
//.text:00AD95F0 ; public: virtual void* c_gui_map_category_datasource::`scalar deleting destructor'(unsigned int)
//.text:00AD9620 ; public: virtual void* c_gui_map_subitem_selectable_item_datasource::`scalar deleting destructor'(unsigned int)
//.text:00AD9690 ; public: virtual void c_gui_map_subitem_selectable_item_datasource::dispose()
//.text:00AD96B0 ; public: virtual void c_gui_map_category_datasource::get_column_names(int32* const, int32*)
//.text:00AD96F0 ; public: virtual void c_gui_map_subitem_selectable_item_datasource::get_column_names(int32* const, int32*)
//.text:00AD9780 ; 
//.text:00AD9790 ; protected: virtual int32 c_gui_map_category_datasource::get_current_item_count_internal()
//.text:00AD97A0 ; protected: virtual int32 c_gui_map_subitem_selectable_item_datasource::get_current_item_count_internal()

bool c_gui_map_selected_item::get_file_path(wchar_t* path, int32 maximum_characters) const
{
	//return INVOKE_CLASS_MEMBER(0x00AD97B0, c_gui_map_selected_item, get_file_path, path, maximum_characters);

	file_reference_get_fullpath_wide(&m_file_reference, path, maximum_characters);
	return true;
}
//.text:00AD97F0 ; public: virtual const c_gui_selected_item* c_gui_map_subitem_selectable_item_datasource::get_gui_selected_item(int32) const
//.text:00AD9820 ; public: virtual bool c_gui_map_category_datasource::get_integer_value(int32, int32, int32*)
//.text:00AD98D0 ; public: virtual bool c_gui_map_subitem_selectable_item_datasource::get_integer_value(int32, int32, int32*)

bool c_gui_map_selected_item::get_map_variant(c_map_variant* map_variant) const
{
	//return INVOKE_CLASS_MEMBER(0x00AD9A30, c_gui_map_selected_item, get_map_variant, map_variant);

	s_blffile_map_variant variant_on_disk{};

	switch (c_gui_map_selected_item::get_location())
	{
	case _gui_stored_item_location_saved_game_file:
	case _gui_stored_item_location_autosave_queue:
	{
		c_synchronized_long success = 0;
		c_synchronized_long done = 0;
		if (autosave_queue_read_file(&m_file_reference, &variant_on_disk, sizeof(s_blffile_map_variant), &success, &done) == NONE)
		{
			break;
		}
		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
		if (success.peek() != 1)
		{
			break;
		}

		bool was_valid = false;
		if (variant_on_disk.copy_to_and_validate(map_variant, &was_valid))
		{
			return was_valid;
		}

		if (variant_on_disk.variant.map_variant.save_to(map_variant))
		{
			was_valid = true;
		}
		return was_valid;
	}
	break;
	}

	return false;
}

//.text:00AD9C30 ; 
//.text:00AD9C40 ; 
//.text:00AD9C50 ; public: virtual bool c_gui_map_category_datasource::get_string_id_value(int32, int32, int32*)
//.text:00AD9C70 ; public: virtual bool c_gui_map_subitem_selectable_item_datasource::get_string_id_value(int32, int32, int32*)
//.text:00AD9D20 ; public: virtual bool c_gui_map_category_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00AD9D80 ; public: virtual bool c_gui_map_subitem_selectable_item_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00AD9DD0 ; public: virtual bool c_gui_map_category_datasource::initialize(int32)
//.text:00ADA050 ; public: virtual bool c_gui_map_subitem_selectable_item_datasource::initialize(int32, e_map_id, e_controller_index)
//.text:00ADA1C0 ; public: virtual bool c_gui_map_subitem_selectable_item_datasource::is_busy() const
//.text:00ADA1D0 ; int __cdecl map_category_sort_proc(const void*, const void*)

int __cdecl map_selected_item_sort_proc(const void* a, const void* b)
{
	return INVOKE(0x00ADA210, map_selected_item_sort_proc, a, b);
}

//.text:00ADA300 ; 
//.text:00ADA320 ; 
//.text:00ADA340 ; public: virtual void c_gui_map_subitem_selectable_item_datasource::update()

void c_gui_map_subitem_selectable_item_datasource::update_autosave_enumeration()
{
	//INVOKE_CLASS_MEMBER(0x00ADA360, c_gui_map_subitem_selectable_item_datasource, update_autosave_enumeration);

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

		if (m_map_count >= k_maximum_maps_shown)
		{
			break;
		}

		s_ui_saved_game_item_metadata metadata{};
		metadata.set(&item.metadata);

		m_maps[m_map_count++] = c_gui_map_selected_item(
			&metadata,
			_gui_stored_item_location_autosave_queue,
			m_controller_index,
			&item.file,
			0,
			c_gui_selected_item::_special_item_type_none,
			item.state == s_saved_game_item_enumeration_data::_item_state_corrupt,
			false);
	}

	//m_maps.sort(m_map_count, map_selected_item_sort_proc);
	qsort(&m_maps, m_map_count, sizeof(c_gui_map_selected_item), map_selected_item_sort_proc);

	if (!m_autosave_enumerator.is_busy() && !m_autosave_enumerator.are_items_available())
	{
		m_enumeration_complete = true;
	}
}

void c_gui_map_subitem_selectable_item_datasource::update_content_enumeration()
{
	//INVOKE_CLASS_MEMBER(0x00ADA550, c_gui_map_subitem_selectable_item_datasource, update_content_enumeration);

	// $TODO properly implement content items and un/reimplement this function at that time

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
		FLAG(_find_files_recursive_bit),
		"map_variants",
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

	for (int32 saved_films_file_index = 0; saved_films_file_index < saved_films_file_count; saved_films_file_index++)
	{
		if (m_map_count >= k_maximum_maps_shown)
		{
			continue;
		}

		s_file_reference* saved_films_file = &saved_films_files[saved_films_file_index];
		if (file_is_directory(saved_films_file))
		{
			continue;
		}

		wchar_t found_file_extension[256]{};
		file_reference_get_name_wide(saved_films_file, FLAG(_name_extension_bit), found_file_extension, NUMBEROF(found_file_extension));

		const wchar_t* filename_extension = L"map";
		if (ustricmp(found_file_extension, filename_extension) != 0)
		{
			continue;
		}

		s_saved_game_item_enumeration_data item{};
		{
			s_blffile_saved_game_file variant_on_disk{};
			c_synchronized_long success = 0;
			c_synchronized_long done = 0;
			if (autosave_queue_read_file(saved_films_file, &variant_on_disk, sizeof(s_blffile_saved_game_file), &success, &done) == NONE)
			{
				continue;
			}

			internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

			if (success.peek() != 1)
			{
				continue;
			}

			item.file = *saved_films_file;
			item.metadata = variant_on_disk.content_header.metadata;
		}

		if (item.metadata.file_type != _saved_game_map_variant || m_enumeration_map_id != item.metadata.map_id)
		{
			continue;
		}

		bool exists = false;
		for (int32 map_index = 0; !exists && map_index < m_map_count; map_index++)
		{
			exists = item.metadata.unique_id == m_maps[map_index].m_metadata.unique_id;
		}

		if (exists)
		{
			continue;
		}

		s_ui_saved_game_item_metadata metadata{};
		metadata.set(&item.metadata);

		m_maps[m_map_count++] = c_gui_map_selected_item(
			&metadata,
			_gui_stored_item_location_saved_game_file,
			m_controller_index,
			&item.file,
			0,
			c_gui_selected_item::_special_item_type_none,
			item.state == s_saved_game_item_enumeration_data::_item_state_corrupt,
			false);
	}

	//m_maps.sort(m_map_count, map_selected_item_sort_proc);
	qsort(&m_maps, m_map_count, sizeof(c_gui_map_selected_item), map_selected_item_sort_proc);

	csmemset(&find_file_data, 0, sizeof(s_find_file_data));
	csmemset(saved_films_files, 0, sizeof(s_file_reference) * 512);
	saved_films_file_count = 0;
}

