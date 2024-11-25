#include "saved_games/saved_game_files.hpp"

#include "saved_games/content/content_item_metadata.hpp"

REFERENCE_DECLARE(0x0189D930, c_saved_game_files_globals, g_saved_game_files_globals);

//.text:00525AB0 ; bool __cdecl read_campaign_save_file_blocking(e_controller_index, wchar_t const*, unsigned int, void*, unsigned int)
//.text:00525B50 ; bool __cdecl saved_game_files_create_saved_game_file_reference(e_controller_index, e_game_content_type, wchar_t const*, wchar_t const*, s_file_reference*, long*)
//.text:00525C40 ; long __cdecl saved_game_files_delete(e_controller_index, long, c_synchronized_long*, c_synchronized_long*)
//.text:00525CA0 ; e_async_completion __cdecl saved_game_files_delete_callback(s_async_task*)
//.text:00525D10 ; 
//.text:00525D20 ; void __cdecl saved_game_files_delete_saved_game_state(e_controller_index)

void __cdecl saved_game_files_dispose()
{
	INVOKE(0x00525DA0, saved_game_files_dispose);
}

void __cdecl saved_game_files_dispose_from_old_map()
{
	INVOKE(0x00525DB0, saved_game_files_dispose_from_old_map);
}

//.text:00525DC0 ; bool __cdecl saved_game_files_does_file_exist(e_controller_index, qword)
//.text:00525E60 ; bool __cdecl saved_game_files_enumeration_in_progress()
//.text:00525E90 ; bool __cdecl saved_game_files_file_exists(e_controller_index, e_game_content_type, wchar_t const*)
//.text:00525EC0 ; bool __cdecl saved_game_files_get_content_item_file_path(e_controller_index, long, wchar_t*, long)
//.text:00525F00 ; 
//.text:00525FB0 ; bool __cdecl saved_game_files_get_default_film_save_name(e_controller_index, e_gui_game_mode, wchar_t const*, wchar_t const*, e_campaign_difficulty_level, bool, wchar_t*, long)
//.text:00526080 ; bool __cdecl saved_game_files_get_default_game_variant_save_description(e_controller_index, e_game_engine_type, wchar_t const*, wchar_t*, long)
//.text:005260F0 ; bool __cdecl saved_game_files_get_default_game_variant_save_name(e_controller_index, e_game_engine_type, wchar_t const*, wchar_t*, long)
//.text:00526180 ; bool __cdecl saved_game_files_get_default_map_variant_save_description(e_controller_index, wchar_t const*, wchar_t*, long)
//.text:005261F0 ; bool __cdecl saved_game_files_get_default_map_variant_save_name(e_controller_index, wchar_t const*, wchar_t*, long)
//.text:00526270 ; 
//.text:00526320 ; bool __cdecl saved_game_files_get_default_screenshot_save_name(e_controller_index, e_gui_game_mode, wchar_t const*, wchar_t const*, e_campaign_difficulty_level, wchar_t*, long)
//.text:005263E0 ; void __cdecl saved_game_files_get_extra_metadata_display_name_string(e_saved_game_file_type, wchar_t*, long)
//.text:005264E0 ; long __cdecl saved_game_files_get_personal_storage_content_item(e_controller_index, bool)
//.text:00526550 ; char const* __cdecl saved_game_files_get_saved_game_file_name(e_saved_game_file_type)
//.text:005265E0 ; bool __cdecl saved_game_files_read_saved_game_state_blocking(e_controller_index, void*, long)
//.text:005266E0 ; bool __cdecl saved_game_files_read_saved_game_state_header_blocking(e_controller_index, void*, unsigned int)
//.text:005267E0 ; bool __cdecl saved_game_files_get_storage_space(e_controller_index, qword*, qword*)

void __cdecl saved_game_files_initialize()
{
	INVOKE(0x00526840, saved_game_files_initialize);
}

void __cdecl saved_game_files_initialize_for_new_map()
{
	INVOKE(0x00526890, saved_game_files_initialize_for_new_map);
}

//.text:005268C0 ; 
//.text:005268E0 ; 
//.text:00526900 ; 
//.text:00526910 ; bool __cdecl saved_game_files_parse_campaign_difficulty(s_saved_game_file_text_parser_input const*, wchar_t*, long)
//.text:00526970 ; bool __cdecl saved_game_files_parse_campaign_map_name(s_saved_game_file_text_parser_input const*, wchar_t*, long)
//.text:005269A0 ; bool __cdecl saved_game_files_parse_game_variant_name(s_saved_game_file_text_parser_input const*, wchar_t*, long)
//.text:005269D0 ; bool __cdecl saved_game_files_parse_map_variant_name(s_saved_game_file_text_parser_input const*, wchar_t*, long)
//.text:00526A00 ; long __cdecl saved_game_files_read(e_controller_index, long, e_saved_game_file_type, s_blffile_saved_game_file*, dword, c_synchronized_long*, c_synchronized_long*)
//.text:00526A90 ; public: void c_saved_game_files_globals::register_text_parsers(s_saved_game_file_text_parser_input const*)
//.text:00526B00 ; 
//.text:00526B30 ; long __cdecl saved_game_files_save_film_snippet(e_controller_index, wchar_t const*, wchar_t const*, c_synchronized_long*, c_synchronized_long*, long*)
//.text:00526B80 ; 
//.text:00526BB0 ; 
//.text:00526BC0 ; void __cdecl saved_game_files_uniquify_name(e_controller_index, e_saved_game_file_type, wchar_t*, long)
//.text:00526D70 ; saved_game_files_unregister_text_parsers

void __cdecl saved_game_files_update()
{
	INVOKE(0x00526DD0, saved_game_files_update);
}

//.text:00526E00 ; long __cdecl saved_game_files_update_metadata(e_controller_index, long, wchar_t const*, wchar_t const*, c_synchronized_long*, c_synchronized_long*)
//.text:00526E90 ; e_async_completion __cdecl saved_game_files_update_metadata_callback(s_async_task*)
//.text:00527040 ; long __cdecl saved_game_files_write(e_controller_index, long, e_saved_game_file_type, s_blffile_saved_game_file const*, dword, c_synchronized_long*, c_synchronized_long*)
//.text:005270F0 ; void __cdecl saved_game_files_write_saved_game_state_blocking(s_game_state_header const*, long, void const*, long)
//.text:00527380 ; bool __cdecl saved_game_read_metadata_from_file_blocking(s_file_reference*, s_saved_game_item_metadata*)
//.text:005273E0 ; bool __cdecl saved_game_read_metadata_from_open_file_internal(s_file_reference*, s_saved_game_item_metadata*)
//.text:005274C0 ; saved_game_synchronize_contents_from_metadata
//.text:005276E0 ; bool __cdecl saved_game_write_metadata_to_file(s_file_reference*, s_saved_game_item_metadata const*, e_game_content_type, bool)
//.text:00527720 ; bool __cdecl saved_game_write_metadata_to_open_file_internal(s_file_reference*, s_saved_game_item_metadata const*, e_game_content_type, bool)
//.text:00527860 ; enum e_async_completion __cdecl saved_games_async_work_slice_finish(bool, bool, e_saved_game_file_operation_result, c_synchronized_long*, c_synchronized_long*)
//.text:005278C0 ; 
//.text:005278F0 ; public: void __cdecl s_saved_game_item_metadata::set(s_ui_saved_game_item_metadata const*)
//.text:00527A30 ; public: void __cdecl s_ui_saved_game_item_metadata::set(s_saved_game_item_metadata const*)

