#include "interface/gui_selected_items_game_variant.hpp"

#include "cseries/async.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/saved_game_files.hpp"

#include <cstdlib>

HOOK_DECLARE_CLASS_MEMBER(0x00ADB590, c_gui_game_variant_selected_item, get_game_variant_);
//HOOK_DECLARE_CLASS_MEMBER(0x00ADBF30, c_gui_game_variant_subitem_selectable_item_datasource, update_autosave_enumeration_);
HOOK_DECLARE_CLASS_MEMBER(0x00ADC0A0, c_gui_game_variant_subitem_selectable_item_datasource, update_content_enumeration_);

bool c_gui_game_variant_selected_item::get_game_variant_(c_game_variant* game_variant)
{
	return c_gui_game_variant_selected_item::get_game_variant(game_variant);
}

void __thiscall c_gui_game_variant_subitem_selectable_item_datasource::update_autosave_enumeration_()
{
	//c_gui_game_variant_subitem_selectable_item_datasource::update_autosave_enumeration();
}

void __thiscall c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration_()
{
	c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration();
}

//.text:00ADAC20 ; public: c_gui_game_variant_selected_item::c_gui_game_variant_selected_item()
//.text:00ADACA0 ; public: c_gui_game_variant_category_datasource::c_gui_game_variant_category_datasource(c_gui_screen_widget*)

c_gui_game_variant_selected_item::c_gui_game_variant_selected_item(const s_ui_saved_game_item_metadata* metadata, e_gui_selected_item_location location, e_controller_index controller_index, const s_file_reference* file_reference, int32 variant_index, int32 sort_order, bool corrupt, bool is_new)
{
	DECLFUNC(0x00ADAD60, void, __thiscall, c_gui_game_variant_selected_item*, const s_ui_saved_game_item_metadata*, e_gui_selected_item_location, e_controller_index, const s_file_reference*, int32, int32, bool, bool)(
		this, metadata, location, controller_index, file_reference, variant_index, sort_order, corrupt, is_new);
}

//.text:00ADADD0 ; public: c_gui_game_variant_selected_item::c_gui_game_variant_selected_item()
//.text:00ADAE30 ; public: c_gui_game_variant_subitem_selectable_item_datasource::c_gui_game_variant_subitem_selectable_item_datasource()
//.text:00ADAEB0 ; public: s_blf_chunk_game_variant::s_blf_chunk_game_variant()
//.text:00ADAEE0 ; public: s_blffile_game_variant::s_blffile_game_variant()
//.text:00ADAF30 ; 
//.text:00ADAF40 ; 
//.text:00ADAF50 ; public: virtual c_gui_game_variant_subitem_selectable_item_datasource::~c_gui_game_variant_subitem_selectable_item_datasource()

c_gui_game_variant_selected_item& c_gui_game_variant_selected_item::operator=(const c_gui_game_variant_selected_item& __that)
{
	return DECLFUNC(0x00ADAFA0, c_gui_game_variant_selected_item&, __thiscall, c_gui_game_variant_selected_item*, const c_gui_game_variant_selected_item&)(
		this, __that);
}

//.text:00ADB020 ; 
//.text:00ADB040 ; 
//.text:00ADB060 ; 
//.text:00ADB080 ; 
//.text:00ADB090 ; public: virtual void* c_gui_game_variant_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADB0C0 ; public: virtual void* c_gui_game_variant_subitem_selectable_item_datasource::`vector deleting destructor'(unsigned int)
//.text:00ADB130 ; private: void c_gui_game_variant_category_datasource::add_game_engine(c_gui_data*, e_game_engine_type)
//.text:00ADB260 ; public: virtual void c_gui_game_variant_subitem_selectable_item_datasource::dispose()
//.text:00ADB280 ; int __cdecl game_variant_category_sort_proc(const void*, const void*)

int __cdecl game_variant_selected_item_sort_proc(const void* a, const void* b)
{
	return INVOKE(0x00ADB2C0, game_variant_selected_item_sort_proc, a, b);
}

//.text:00ADB3B0 ; public: virtual void c_gui_game_variant_category_datasource::get_column_names(int32* const, int32*)
//.text:00ADB3F0 ; public: virtual void c_gui_game_variant_subitem_selectable_item_datasource::get_column_names(int32* const, int32*)
//.text:00ADB480 ; 
//.text:00ADB490 ; protected: virtual int32 c_gui_game_variant_category_datasource::get_current_item_count_internal()
//.text:00ADB4A0 ; protected: virtual int32 c_gui_game_variant_subitem_selectable_item_datasource::get_current_item_count_internal()

bool c_gui_game_variant_selected_item::get_file_path(wchar_t* path, int32 maximum_characters) const
{
	//INVOKE_CLASS_MEMBER(0x00ADB4B0, c_gui_game_variant_selected_item, get_file_path, path, maximum_characters);

	file_reference_get_fullpath_wide(&m_file_reference, path, maximum_characters);
	return true;
}

//.text:00ADB4F0 ; private: int32 c_gui_game_variant_category_datasource::get_game_engine_sort_order(c_gui_data*, int32)
//.text:00ADB580 ; 

bool c_gui_game_variant_selected_item::get_game_variant(c_game_variant* game_variant) const
{
	//return INVOKE_CLASS_MEMBER(0x00ADB590, c_gui_game_variant_selected_item, get_game_variant, game_variant);

	s_blffile_game_variant variant_on_disk{};

	e_gui_selected_item_location location = c_gui_game_variant_selected_item::get_location();
	switch (location)
	{
	case _gui_stored_item_location_built_in:
	{
		return game_engine_tag_defined_variant_get_built_in_variant(m_metadata.game_engine_index, m_variant_index, game_variant);
	}
	break;
	case _gui_stored_item_location_saved_game_file:
	case _gui_stored_item_location_autosave_queue:
	{
		if (location == _gui_stored_item_location_saved_game_file &&
			game_engine_index_to_saved_game_file_type(m_metadata.game_engine_index) == k_saved_game_file_type_none)
		{
			break;
		}

		c_synchronized_long success = 0;
		c_synchronized_long done = 0;
		if (autosave_queue_read_file(&m_file_reference, &variant_on_disk, sizeof(s_blffile_game_variant), &success, &done) == NONE)
		{
			break;
		}
		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
		if (success.peek() != 1)
		{
			break;
		}

		bool was_valid = false;
		if (variant_on_disk.copy_to_and_validate(game_variant, &was_valid))
		{
			return was_valid;
		}

		game_variant->copy_from_and_validate(&variant_on_disk.variant.game_variant);
		return true;
	}
	break;
	}

	return false;
}

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

void c_gui_game_variant_subitem_selectable_item_datasource::update_autosave_enumeration()
{
	INVOKE_CLASS_MEMBER(0x00ADBF30, c_gui_game_variant_subitem_selectable_item_datasource, update_autosave_enumeration);
}

void c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration()
{
	//INVOKE_CLASS_MEMBER(0x00ADC0A0, c_gui_game_variant_subitem_selectable_item_datasource, update_content_enumeration);

	// $TODO: properly implement content items and un/reimplement this function at that time

	if (m_enumeration_complete)
	{
		return;
	}

	s_file_reference game_variants_directory{};
	file_reference_create_from_path_wide(&game_variants_directory, L"game_variants", true);
	find_files_recursive(this, &game_variants_directory, 0, c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration_proc);

	//m_game_variants.sort(m_game_variant_count, game_variant_selected_item_sort_proc);
	qsort(&m_game_variants, m_game_variant_count, sizeof(c_gui_game_variant_selected_item), game_variant_selected_item_sort_proc);

	m_enumeration_complete = true;
}

bool c_gui_game_variant_subitem_selectable_item_datasource::update_content_enumeration_proc(void* userdata, s_file_reference* found_file)
{
	c_gui_game_variant_subitem_selectable_item_datasource* _this = (c_gui_game_variant_subitem_selectable_item_datasource*)userdata;

	if (_this->m_game_variant_count >= k_maximum_game_variants_shown)
	{
		return false;
	}

	wchar_t found_file_extension[256]{};
	file_reference_get_name_wide(found_file, FLAG(_name_extension_bit), found_file_extension, NUMBEROF(found_file_extension));

	e_saved_game_file_type game_file_type = game_engine_index_to_saved_game_file_type(_this->m_game_engine_type);
	const wchar_t* filename_extension = saved_game_filename_extension_by_game_file_type(game_file_type, true);
	if (ustricmp(found_file_extension, filename_extension) != 0)
	{
		return false;
	}

	s_saved_game_item_enumeration_data item{};
	{
		s_blffile_saved_game_file variant_on_disk{};
		c_synchronized_long success = 0;
		c_synchronized_long done = 0;
		if (autosave_queue_read_file(found_file, &variant_on_disk, sizeof(s_blffile_saved_game_file), &success, &done) == NONE)
		{
			return false;
		}

		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

		if (success.peek() != 1)
		{
			return false;
		}

		item.file = *found_file;
		item.metadata = variant_on_disk.content_header.metadata;
	}

	if (!IN_RANGE_INCLUSIVE(item.metadata.file_type, _saved_game_file_type_first_game_variant, _saved_game_file_type_last_game_variant) || _this->m_game_engine_type != item.metadata.game_engine_index)
	{
		return false;
	}

	s_ui_saved_game_item_metadata metadata{};
	metadata.set(&item.metadata);

	_this->m_game_variants[_this->m_game_variant_count++] = c_gui_game_variant_selected_item(
		&metadata,
		_gui_stored_item_location_saved_game_file,
		_this->m_controller_index,
		&item.file,
		0,
		c_gui_selected_item::_special_item_type_none,
		item.state == s_saved_game_item_enumeration_data::_item_state_corrupt,
		false);

	return true;
};

