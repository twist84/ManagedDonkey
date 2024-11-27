#include "saved_games/content_catalogue.hpp"

REFERENCE_DECLARE(0x0240A340, s_content_catalogue_globals, g_content_catalogue_globals);

//.text:00479C00 ; public: bool __cdecl c_content_catalogue::valid() const
//.text:005A4A20 ; public: __cdecl c_content_catalogue::c_content_catalogue()
//.text:005A4AC0 ; public: __cdecl c_saved_game_scope_lock::c_saved_game_scope_lock()
//.text:005A4AE0 ; public: __cdecl s_content_catalogue_globals::s_content_catalogue_globals()
//.text:005A4B50 ; public: __cdecl c_content_catalogue::~c_content_catalogue()
//.text:005A4BB0 ; 
//.text:005A4BC0 ; 
//.text:005A4BF0 ; 
//.text:005A4C00 ; 
//.text:005A4C10 ; public: __cdecl s_content_catalogue_globals::~s_content_catalogue_globals()
//.text:005A4C80 ; 
//.text:005A4C90 ; 
//.text:005A4CA0 ; 
//.text:005A4CD0 ; protected: void __cdecl c_content_catalogue::allocate_content_item_data_array()
//.text:005A4D00 ; 
//.text:005A4D20 ; public: bool __cdecl c_content_catalogue::begin_enumeration()
//.text:005A4D30 ; public: void __cdecl c_content_catalogue::block_until_finished(s_content_item const*, bool, bool)
//.text:005A4E00 ; 
//.text:005A4E10 ; public: void __cdecl c_content_catalogue::close_all_content_items_of_type(e_game_content_type)
//.text:005A4EE0 ; public: void __cdecl c_content_catalogue::close_all_dlc_packages(e_campaign_id const*, e_map_id const*, long)
//.text:005A50A0 ; bool __cdecl content_catalog_is_initialized()
//.text:005A50B0 ; char const* __cdecl content_catalogue_build_new_saved_game_file_type_container_name(e_game_content_type, char*, long)
//.text:005A52A0 ; void __cdecl content_catalogue_close_all_dlc(bool)
//.text:005A5390 ; bool __cdecl content_catalogue_device_selection_active(e_controller_index)
//.text:005A53D0 ; 
//.text:005A53F0 ; void __cdecl content_catalogue_display_device_selection_guide_interface(e_controller_index)
//.text:005A5490 ; void __cdecl content_catalogue_dispose()
//.text:005A5530 ; void __cdecl content_catalogue_dispose_from_old_map()
//.text:005A5540 ; bool __cdecl content_catalogue_enumeration_active_on_controllers(long*)
//.text:005A5600 ; c_content_catalogue* __cdecl content_catalogue_get_interface(e_controller_index)
//.text:005A5620 ; e_game_content_type __cdecl content_catalogue_get_saved_game_file_type_from_container_name(char const*)
//.text:005A57B0 ; 
//.text:005A57D0 ; bool __cdecl content_catalogue_handle_dialog_result_message(c_dialog_result_message const*)
//.text:005A5800 ; void __cdecl content_catalogue_initialize()
//.text:005A58A0 ; void __cdecl content_catalogue_initialize_for_new_map()
//.text:005A5990 ; bool __cdecl content_catalogue_open_dlc(wchar_t const*, bool)
//.text:005A5AB0 ; content_catalogue_retrieve_content_item_metadata_callback
//.text:005A5C20 ; content_catalogue_retrieve_metadata_for_content_items
//.text:005A5CA0 ; void __cdecl content_catalogue_storage_devices_changed()
//.text:005A5CF0 ; protected: void __cdecl c_content_catalogue::content_enumeration_complete()
//.text:005A5D30 ; protected: void __cdecl c_content_catalogue::content_enumeration_failed()
//.text:005A5DB0 ; protected: void __cdecl c_content_catalogue::content_enumeration_success(struct _XCONTENT_DATA*, long, long)
//.text:005A5E70 ; public: s_content_item* __cdecl c_content_catalogue::create_content_item(wchar_t const*, e_game_content_type, long, long, e_content_item_status*, bool)
//.text:005A5FD0 ; public: void __cdecl c_content_catalogue::content_item_delete(long, bool)
//.text:005A6130 ; public: void __cdecl c_content_catalogue::content_item_flush(long, bool)
//.text:005A6210 ; 
//.text:005A6230 ; public: bool __cdecl c_content_catalogue::content_item_get_dashboard_display_name(long, wchar_t*, long) const
//.text:005A6310 ; public: bool __cdecl c_content_catalogue::content_item_get_directory_file_path(long, wchar_t*, long, bool) const
//.text:005A6400 ; public: bool __cdecl c_content_catalogue::content_item_get_directory_file_reference(long, s_file_reference*) const
//.text:005A64E0 ; public: bool __cdecl c_content_catalogue::content_item_get_display_name(long, wchar_t*, long) const
//.text:005A65C0 ; public: e_game_content_type __cdecl c_content_catalogue::content_item_get_game_content_type(long) const
//.text:005A6690 ; public: bool __cdecl c_content_catalogue::content_item_get_game_file_path(long, wchar_t*, long) const
//.text:005A6780 ; public: bool __cdecl c_content_catalogue::content_item_get_game_file_reference(long, s_file_reference*) const
//.text:005A6860 ; 
//.text:005A6940 ; public: bool __cdecl c_content_catalogue::content_item_get_saved_game_metadata(long, s_saved_game_item_metadata*) const
//.text:005A6A30 ; public: bool __cdecl c_content_catalogue::content_item_is_corrupt(long) const
//.text:005A6B10 ; public: bool __cdecl c_content_catalogue::content_item_is_new(long) const
//.text:005A6BF0 ; public: bool __cdecl c_content_catalogue::content_item_mount(long, bool)
//.text:005A6CE0 ; public: bool __cdecl c_content_catalogue::content_item_name_unique(wchar_t const*, e_game_content_type) const
//.text:005A6D40 ; public: bool __cdecl c_content_catalogue::content_item_of_name_and_type_exists(wchar_t const*, e_game_content_type) const
//.text:005A6D60 ; public: long __cdecl c_content_catalogue::content_item_overwrite(wchar_t const*, wchar_t const*, e_game_content_type, bool, long, bool)
//.text:005A6E60 ; public: bool __cdecl c_content_catalogue::content_item_retrieve_saved_game_metadata_internal(long) const
//.text:005A6F40 ; 
//.text:005A7020 ; public: bool __cdecl c_content_catalogue::content_item_set_saved_game_metadata(long, s_saved_game_item_metadata const*)
//.text:005A70D0 ; protected: s_content_item* __cdecl c_content_catalogue::content_item_try_and_get(long) const
//.text:005A7120 ; protected: s_content_item* __cdecl c_content_catalogue::content_item_try_and_get_absolute(long) const
//.text:005A71B0 ; protected: s_content_item* __cdecl c_content_catalogue::content_item_try_and_get_by_file_reference(s_file_reference const*, long*) const
//.text:005A7360 ; public: long __cdecl c_content_catalogue::content_item_try_and_get_from_file_reference(s_file_reference const*) const
//.text:005A73E0 ; public: long __cdecl c_content_catalogue::content_item_try_and_get_personal_storage_item(wchar_t const*) const
//.text:005A7440 ; public: void __cdecl c_content_catalogue::content_item_unmount(long, bool) const
//.text:005A7520 ; protected: bool __cdecl c_content_catalogue::data_array_usable() const
//.text:005A7560 ; 
//.text:005A7570 ; public: void __cdecl c_content_catalogue::dispose()
//.text:005A76E0 ; protected: void __cdecl c_content_catalogue::dispose_content_item_array()
//.text:005A7760 ; 
//.text:005A7790 ; public: bool __cdecl c_content_catalogue::enumerating_content_metadata() const
//.text:005A77A0 ; 
//.text:005A77C0 ; private: long __cdecl c_content_catalogue::find_content_item_index_from_name_and_type(wchar_t const*, e_game_content_type) const
//.text:005A78A0 ; public: long __cdecl c_content_catalogue::first_content_item_index() const
//.text:005A7970 ; public: long __cdecl c_content_catalogue::get_content_count() const
//.text:005A7A30 ; 
//.text:005A7A40 ; 
//.text:005A7A50 ; 
//.text:005A7A60 ; 
//.text:005A7A70 ; 
//.text:005A7A80 ; protected: unsigned long __cdecl c_content_catalogue::get_device_id() const
//.text:005A7AA0 ; 
//.text:005A7AB0 ; public: long __cdecl c_content_catalogue::get_free_slots_for_content(e_game_content_type, bool) const
//.text:005A7C10 ; public: qword __cdecl c_content_catalogue::get_free_space_bytes() const
//.text:005A7CB0 ; 
//.text:005A7CC0 ; 
//.text:005A7CD0 ; 
//.text:005A7CE0 ; 
//.text:005A7CF0 ; 
//.text:005A7D00 ; public: bool __cdecl c_content_catalogue::get_storage_space(qword*, qword*)
//.text:005A7E00 ; 
//.text:005A7E10 ; public: void __cdecl c_content_catalogue::initialize(e_controller_index)
//.text:005A7EA0 ; public: void __cdecl c_content_catalogue::initialize_lite(e_controller_index)
//.text:005A7F30 ; public: void __cdecl c_content_catalogue::invalidate()
//.text:005A7F40 ; 
//.text:005A7F50 ; 
//.text:005A7F80 ; 
//.text:005A7F90 ; public: long __cdecl c_content_catalogue::last_content_item_index() const
//.text:005A8060 ; private: static bool __cdecl c_content_catalogue::name_is_built_in_game_variant(wchar_t const*, e_game_content_type)
//.text:005A8100 ; private: static bool __cdecl c_content_catalogue::name_is_built_in_map(wchar_t const*)
//.text:005A8180 ; 
//.text:005A81A0 ; public: long __cdecl c_content_catalogue::next_content_item_index(long) const
//.text:005A8270 ; public: bool __cdecl c_content_catalogue::open_dlc_package(s_file_reference const*, bool)
//.text:005A8330 ; public: void __cdecl c_content_catalogue::render()
//.text:005A8340 ; 
//.text:005A8370 ; 
//.text:005A83A0 ; 
//.text:005A83C0 ; 
//.text:005A83D0 ; 
//.text:005A83E0 ; 
//.text:005A8400 ; 
//.text:005A8430 ; 
//.text:005A8480 ; public: void __cdecl c_content_catalogue::update()
//.text:005A85E0 ; public: bool __cdecl c_content_catalogue::verify_name_or_show_error(e_controller_index, wchar_t const*, e_game_content_type) const


