#include "saved_games/content_catalogue.hpp"

#include "interface/user_interface_messages.hpp"
#include "tag_files/string_ids.hpp"

REFERENCE_DECLARE(0x0240A340, s_content_catalogue_globals, g_content_catalogue_globals);

//.text:005A4A20 ; public: c_content_catalogue::c_content_catalogue()
//.text:005A4AC0 ; public: c_saved_game_scope_lock::c_saved_game_scope_lock()
//.text:005A4AE0 ; public: s_content_catalogue_globals::s_content_catalogue_globals()
//.text:005A4B50 ; public: c_content_catalogue::~c_content_catalogue()
//.text:005A4BB0 ; 
//.text:005A4BC0 ; 
//.text:005A4BF0 ; 
//.text:005A4C00 ; 
//.text:005A4C10 ; public: s_content_catalogue_globals::~s_content_catalogue_globals()
//.text:005A4C80 ; 
//.text:005A4C90 ; 
//.text:005A4CA0 ; 
//.text:005A4CD0 ; protected: void c_content_catalogue::allocate_content_item_data_array()
//.text:005A4D00 ; 
//.text:005A4D20 ; public: bool c_content_catalogue::begin_enumeration()
//.text:005A4D30 ; public: void c_content_catalogue::block_until_finished(const s_content_item*, bool, bool)
//.text:005A4E00 ; 
//.text:005A4E10 ; public: void c_content_catalogue::close_all_content_items_of_type(e_game_content_type)
//.text:005A4EE0 ; public: void c_content_catalogue::close_all_dlc_packages(const e_campaign_id*, const e_map_id*, int32)
//.text:005A50A0 ; bool __cdecl content_catalog_is_initialized()
//.text:005A50B0 ; const char* __cdecl content_catalogue_build_new_saved_game_file_type_container_name(e_game_content_type, char*, int32)

void __cdecl content_catalogue_close_all_dlc(bool allow_exceptions)
{
	INVOKE(0x005A52A0, content_catalogue_close_all_dlc, allow_exceptions);
}

//.text:005A5390 ; bool __cdecl content_catalogue_device_selection_active(e_controller_index)
//.text:005A53D0 ; 

void __cdecl content_catalogue_display_device_selection_guide_interface(e_controller_index controller_index)
{
	return INVOKE(0x005A53F0, content_catalogue_display_device_selection_guide_interface, controller_index);
}

void __cdecl content_catalogue_dispose()
{
	INVOKE(0x005A5490, content_catalogue_dispose);
}

void __cdecl content_catalogue_dispose_from_old_map()
{
	INVOKE(0x005A5530, content_catalogue_dispose_from_old_map);
}

bool __cdecl content_catalogue_enumeration_active_on_controllers(int32* out_controller_mask)
{
	return INVOKE(0x005A5540, content_catalogue_enumeration_active_on_controllers, out_controller_mask);
}

c_content_catalogue* __cdecl content_catalogue_get_interface(e_controller_index controller_index)
{
	return INVOKE(0x005A5600, content_catalogue_get_interface, controller_index);
}

//.text:005A5620 ; e_game_content_type __cdecl content_catalogue_get_saved_game_file_type_from_container_name(const char*)
//.text:005A57B0 ; 

bool __cdecl content_catalogue_handle_dialog_result_message(const c_dialog_result_message* dialog_result_message)
{
	//return INVOKE(0x005A57D0, content_catalogue_handle_dialog_result_message, dialog_result_message);

	ASSERT(dialog_result_message != nullptr);

	if (dialog_result_message->get_screen_name() == STRING_ID(gui, controller_no_storage_device_chosen))
	{
		return false;
	}

	if (dialog_result_message->get_dialog_result() == _gui_dialog_choice_first)
	{
		content_catalogue_display_device_selection_guide_interface(dialog_result_message->get_controller());
	}

	return true;
}

//.text:005A5800 ; void __cdecl content_catalogue_initialize()
//.text:005A58A0 ; void __cdecl content_catalogue_initialize_for_new_map()

bool __cdecl content_catalogue_open_dlc(const wchar_t* path, bool block)
{
	return INVOKE(0x005A5990, content_catalogue_open_dlc, path, block);
}

//.text:005A5AB0 ; content_catalogue_retrieve_content_item_metadata_callback
//.text:005A5C20 ; content_catalogue_retrieve_metadata_for_content_items
//.text:005A5CA0 ; void __cdecl content_catalogue_storage_devices_changed()
//.text:005A5CF0 ; protected: void c_content_catalogue::content_enumeration_complete()
//.text:005A5D30 ; protected: void c_content_catalogue::content_enumeration_failed()
//.text:005A5DB0 ; protected: void c_content_catalogue::content_enumeration_success(struct _XCONTENT_DATA*, int32, int32)
//.text:005A5E70 ; public: s_content_item* c_content_catalogue::create_content_item(const wchar_t*, e_game_content_type, int32, int32, e_content_item_status*, bool)
//.text:005A5FD0 ; public: void c_content_catalogue::content_item_delete(int32, bool)
//.text:005A6130 ; public: void c_content_catalogue::content_item_flush(int32, bool)
//.text:005A6210 ; 
//.text:005A6230 ; public: bool c_content_catalogue::content_item_get_dashboard_display_name(int32, wchar_t*, int32) const
//.text:005A6310 ; public: bool c_content_catalogue::content_item_get_directory_file_path(int32, wchar_t*, int32, bool) const
//.text:005A6400 ; public: bool c_content_catalogue::content_item_get_directory_file_reference(int32, s_file_reference*) const
//.text:005A64E0 ; public: bool c_content_catalogue::content_item_get_display_name(int32, wchar_t*, int32) const
//.text:005A65C0 ; public: e_game_content_type c_content_catalogue::content_item_get_game_content_type(int32) const
//.text:005A6690 ; public: bool c_content_catalogue::content_item_get_game_file_path(int32, wchar_t*, int32) const
//.text:005A6780 ; public: bool c_content_catalogue::content_item_get_game_file_reference(int32, s_file_reference*) const
//.text:005A6860 ; 
//.text:005A6940 ; public: bool c_content_catalogue::content_item_get_saved_game_metadata(int32, s_saved_game_item_metadata*) const
//.text:005A6A30 ; public: bool c_content_catalogue::content_item_is_corrupt(int32) const
//.text:005A6B10 ; public: bool c_content_catalogue::content_item_is_new(int32) const
//.text:005A6BF0 ; public: bool c_content_catalogue::content_item_mount(int32, bool)
//.text:005A6CE0 ; public: bool c_content_catalogue::content_item_name_unique(const wchar_t*, e_game_content_type) const
//.text:005A6D40 ; public: bool c_content_catalogue::content_item_of_name_and_type_exists(const wchar_t*, e_game_content_type) const
//.text:005A6D60 ; public: int32 c_content_catalogue::content_item_overwrite(const wchar_t*, const wchar_t*, e_game_content_type, bool, int32, bool)
//.text:005A6E60 ; public: bool c_content_catalogue::content_item_retrieve_saved_game_metadata_internal(int32) const
//.text:005A6F40 ; 
//.text:005A7020 ; public: bool c_content_catalogue::content_item_set_saved_game_metadata(int32, const s_saved_game_item_metadata*)
//.text:005A70D0 ; protected: s_content_item* c_content_catalogue::content_item_try_and_get(int32) const
//.text:005A7120 ; protected: s_content_item* c_content_catalogue::content_item_try_and_get_absolute(int32) const
//.text:005A71B0 ; protected: s_content_item* c_content_catalogue::content_item_try_and_get_by_file_reference(const s_file_reference*, int32*) const
//.text:005A7360 ; public: int32 c_content_catalogue::content_item_try_and_get_from_file_reference(const s_file_reference*) const
//.text:005A73E0 ; public: int32 c_content_catalogue::content_item_try_and_get_personal_storage_item(const wchar_t*) const
//.text:005A7440 ; public: void c_content_catalogue::content_item_unmount(int32, bool) const
//.text:005A7520 ; protected: bool c_content_catalogue::data_array_usable() const
//.text:005A7560 ; 
//.text:005A7570 ; public: void c_content_catalogue::dispose()
//.text:005A76E0 ; protected: void c_content_catalogue::dispose_content_item_array()
//.text:005A7760 ; 
//.text:005A7790 ; public: bool c_content_catalogue::enumerating_content_metadata() const

bool c_content_catalogue::enumeration_has_completed()
{
	return m_flags.test(_enumeration_completed_bit);
}

//.text:005A77A0 ; 
//.text:005A77C0 ; private: int32 c_content_catalogue::find_content_item_index_from_name_and_type(const wchar_t*, e_game_content_type) const
//.text:005A78A0 ; public: int32 c_content_catalogue::first_content_item_index() const
//.text:005A7970 ; public: int32 c_content_catalogue::get_content_count() const
//.text:005A7A30 ; 
//.text:005A7A40 ; 
//.text:005A7A50 ; 
//.text:005A7A60 ; 
//.text:005A7A70 ; 
//.text:005A7A80 ; protected: uns32 c_content_catalogue::get_device_id() const
//.text:005A7AA0 ; 
//.text:005A7AB0 ; public: int32 c_content_catalogue::get_free_slots_for_content(e_game_content_type, bool) const
//.text:005A7C10 ; public: uns64 c_content_catalogue::get_free_space_bytes() const
//.text:005A7CB0 ; 
//.text:005A7CC0 ; 
//.text:005A7CD0 ; 
//.text:005A7CE0 ; 
//.text:005A7CF0 ; 
//.text:005A7D00 ; public: bool c_content_catalogue::get_storage_space(uns64*, uns64*)
//.text:005A7E00 ; 

void c_content_catalogue::initialize(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x005A7E10, c_content_catalogue, initialize, controller_index);
}

//.text:005A7EA0 ; public: void c_content_catalogue::initialize_lite(e_controller_index)
//.text:005A7F30 ; public: void c_content_catalogue::invalidate()
//.text:005A7F40 ; 
//.text:005A7F50 ; 
//.text:005A7F80 ; 
//.text:005A7F90 ; public: int32 c_content_catalogue::last_content_item_index() const
//.text:005A8060 ; private: static bool __cdecl c_content_catalogue::name_is_built_in_game_variant(const wchar_t*, e_game_content_type)
//.text:005A8100 ; private: static bool __cdecl c_content_catalogue::name_is_built_in_map(const wchar_t*)
//.text:005A8180 ; 
//.text:005A81A0 ; public: int32 c_content_catalogue::next_content_item_index(int32) const
//.text:005A8270 ; public: bool c_content_catalogue::open_dlc_package(const s_file_reference*, bool)
//.text:005A8330 ; public: void c_content_catalogue::render()
//.text:005A8340 ; 
//.text:005A8370 ; 
//.text:005A83A0 ; 
//.text:005A83C0 ; 
//.text:005A83D0 ; 
//.text:005A83E0 ; 
//.text:005A8400 ; 
//.text:005A8430 ; 
//.text:005A8480 ; public: void c_content_catalogue::update()
//.text:005A85E0 ; public: bool c_content_catalogue::verify_name_or_show_error(e_controller_index, const wchar_t*, e_game_content_type) const


