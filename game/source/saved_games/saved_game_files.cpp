#include "saved_games/saved_game_files.hpp"

#include "interface/gui_selected_items.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/content/content_item_metadata.hpp"
#include "saved_games/saved_film_manager.hpp"

REFERENCE_DECLARE(0x0189D930, s_saved_game_file_globals, g_saved_game_files_globals);

//.text:00524440 ; e_game_engine_type __cdecl game_content_type_to_game_engine_index(e_game_content_type)
//.text:005244C0 ; e_saved_game_file_type __cdecl game_content_type_to_saved_game_file_type(e_game_content_type)
//.text:00524570 ; e_game_content_type __cdecl game_engine_index_to_game_content_type(e_game_engine_type)

e_saved_game_file_type __cdecl game_engine_index_to_saved_game_file_type(e_game_engine_type game_engine_type)
{
	return INVOKE(0x005245F0, game_engine_index_to_saved_game_file_type, game_engine_type);
}

void __cdecl hs_saved_game_files_delete_saved_game_state_test(int16 controller_index)
{
	saved_game_files_delete_saved_game_state_blocking((e_controller_index)controller_index);
}

void __cdecl hs_saved_game_files_enumeration_test(int16 controller_index)
{
}

void __cdecl hs_saved_game_files_save_last_film_test(int16 controller_index)
{
}

e_game_content_type __cdecl saved_game_file_type_to_game_content_type(e_saved_game_file_type saved_game_file_type)
{
	return INVOKE(0x00525330, saved_game_file_type_to_game_content_type, saved_game_file_type);
}

const wchar_t* __cdecl saved_game_filename_extension_by_game_file_type(e_saved_game_file_type game_file_type, bool exclude_dot_character)
{
	return INVOKE(0x005253E0, saved_game_filename_extension_by_game_file_type, game_file_type, exclude_dot_character);
}

bool __cdecl saved_game_files_controller_has_saved_game_state_blocking(e_controller_index controller_index)
{
	return INVOKE(0x005254A0, saved_game_files_controller_has_saved_game_state_blocking, controller_index);
}

//.text:00525500 ; e_async_completion __cdecl saved_game_files_copy_file_into_content_item_callback(s_async_task*)
//.text:005256D0 ; int32 __cdecl saved_game_files_create(e_controller_index, const wchar_t*, e_saved_game_file_type, c_synchronized_long*, c_synchronized_long*, c_synchronized_long*)
//.text:00525760 ; e_async_completion __cdecl saved_game_files_create_callback(s_async_task*)
//.text:005258E0 ; int32 __cdecl saved_game_files_create_content_item_and_copy_file(e_controller_index, e_game_content_type, const wchar_t*, const char*, c_synchronized_long*, c_synchronized_long*, int32*)
//.text:00525AB0 ; bool __cdecl read_campaign_save_file_blocking(e_controller_index, const wchar_t*, unsigned int, void*, unsigned int)
//.text:00525B50 ; bool __cdecl saved_game_files_create_saved_game_file_reference(e_controller_index, e_game_content_type, const wchar_t*, const wchar_t*, s_file_reference*, int32*)
//.text:00525C40 ; int32 __cdecl saved_game_files_delete(e_controller_index, int32, c_synchronized_long*, c_synchronized_long*)
//.text:00525CA0 ; e_async_completion __cdecl saved_game_files_delete_callback(s_async_task*)
//.text:00525D10 ; void __cdecl saved_game_files_delete_last_film()

void __cdecl saved_game_files_delete_saved_game_state_blocking(e_controller_index controller_index)
{
	INVOKE(0x00525D20, saved_game_files_delete_saved_game_state_blocking, controller_index);
}

void __cdecl saved_game_files_dispose()
{
	INVOKE(0x00525DA0, saved_game_files_dispose);

	saved_film_manager_dispose();
}

void __cdecl saved_game_files_dispose_from_old_map()
{
	INVOKE(0x00525DB0, saved_game_files_dispose_from_old_map);

	saved_film_manager_dispose_from_old_map();
}

//.text:00525DC0 ; bool __cdecl saved_game_files_does_file_exist(e_controller_index, uns64)
//.text:00525E60 ; bool __cdecl saved_game_files_enumeration_in_progress()
//.text:00525E90 ; bool __cdecl saved_game_files_file_exists(e_controller_index, e_game_content_type, const wchar_t*)
//.text:00525EC0 ; bool __cdecl saved_game_files_get_content_item_file_path(e_controller_index, int32, wchar_t*, int32)

bool __cdecl saved_game_files_get_default_film_save_description(e_controller_index controller_index, e_gui_game_mode game_mode, const wchar_t* map_name, const wchar_t* game_variant_name, e_campaign_difficulty_level difficulty, wchar_t* description, int32 maximum_characters)
{
	return INVOKE(0x00525F00, saved_game_files_get_default_film_save_description, controller_index, game_mode, map_name, game_variant_name, difficulty, description, maximum_characters);
}

bool __cdecl saved_game_files_get_default_film_save_name(e_controller_index controller_index, e_gui_game_mode game_mode, const wchar_t* map_name, const wchar_t* game_variant_name, e_campaign_difficulty_level difficulty, bool uniquify, wchar_t* name, int32 maximum_characters)
{
	return INVOKE(0x00525FB0, saved_game_files_get_default_film_save_name, controller_index, game_mode, map_name, game_variant_name, difficulty, uniquify, name, maximum_characters);
}

//.text:00526080 ; bool __cdecl saved_game_files_get_default_game_variant_save_description(e_controller_index, e_game_engine_type, const wchar_t*, wchar_t*, int32)
//.text:005260F0 ; bool __cdecl saved_game_files_get_default_game_variant_save_name(e_controller_index, e_game_engine_type, const wchar_t*, wchar_t*, int32)
//.text:00526180 ; bool __cdecl saved_game_files_get_default_map_variant_save_description(e_controller_index, const wchar_t*, wchar_t*, int32)
//.text:005261F0 ; bool __cdecl saved_game_files_get_default_map_variant_save_name(e_controller_index, const wchar_t*, wchar_t*, int32)
//.text:00526270 ; bool __cdecl saved_game_files_get_default_screenshot_save_description(e_controller_index, e_gui_game_mode, const wchar_t*, const wchar_t*, e_campaign_difficulty_level, wchar_t*, int32)
//.text:00526320 ; bool __cdecl saved_game_files_get_default_screenshot_save_name(e_controller_index, e_gui_game_mode, const wchar_t*, const wchar_t*, e_campaign_difficulty_level, wchar_t*, int32)
//.text:005263E0 ; void __cdecl saved_game_files_get_extra_metadata_display_name_string(e_saved_game_file_type, wchar_t*, int32)
//.text:005264E0 ; int32 __cdecl saved_game_files_get_personal_storage_content_item(e_controller_index, bool)
//.text:00526550 ; const char* __cdecl saved_game_files_get_saved_game_file_name(e_saved_game_file_type)
//.text:005265E0 ; bool __cdecl saved_game_files_read_saved_game_state_blocking(e_controller_index, void*, int32)
//.text:005266E0 ; bool __cdecl saved_game_files_read_saved_game_state_header_blocking(e_controller_index, void*, unsigned int)
//.text:005267E0 ; bool __cdecl saved_game_files_get_storage_space(e_controller_index, uns64*, uns64*)

void __cdecl saved_game_files_initialize()
{
	INVOKE(0x00526840, saved_game_files_initialize);

	saved_film_manager_initialize();
}

void __cdecl saved_game_files_initialize_for_new_map()
{
	INVOKE(0x00526890, saved_game_files_initialize_for_new_map);

	saved_film_manager_initialize_for_new_map();
}

//.text:005268C0 ; bool __cdecl saved_game_files_is_content_item_corrupt(e_controller_index, int32)
//.text:005268E0 ; void __cdecl saved_game_files_last_film_saved_record_success(bool)
//.text:00526900 ; bool __cdecl saved_game_files_last_film_was_saved_by_user()
//.text:00526910 ; bool __cdecl saved_game_files_parse_campaign_difficulty(const s_saved_game_file_text_parser_input*, wchar_t*, int32)
//.text:00526970 ; bool __cdecl saved_game_files_parse_campaign_map_name(const s_saved_game_file_text_parser_input*, wchar_t*, int32)
//.text:005269A0 ; bool __cdecl saved_game_files_parse_game_variant_name(const s_saved_game_file_text_parser_input*, wchar_t*, int32)
//.text:005269D0 ; bool __cdecl saved_game_files_parse_map_variant_name(const s_saved_game_file_text_parser_input*, wchar_t*, int32)
//.text:00526A00 ; int32 __cdecl saved_game_files_read(e_controller_index, int32, e_saved_game_file_type, s_blffile_saved_game_file*, uns32, c_synchronized_long*, c_synchronized_long*)
//.text:00526A90 ; public: void s_saved_game_file_globals::register_text_parsers(const s_saved_game_file_text_parser_input*)
//.text:00526B00 ; void __cdecl saved_game_files_remove_corrupt_content_item(e_controller_index, int32)
//.text:00526B30 ; int32 __cdecl saved_game_files_save_film_snippet(e_controller_index, const wchar_t*, const wchar_t*, c_synchronized_long*, c_synchronized_long*, int32*)

int32 __cdecl saved_game_files_save_last_film(e_controller_index controller_index, c_synchronized_long* success, c_synchronized_long* done, wchar_t* out_display_name, int32 maximum_display_name_characters)
{
	return INVOKE(0x00526B80, saved_game_files_save_last_film, controller_index, success, done, out_display_name, maximum_display_name_characters);
}

int32 __cdecl saved_game_files_save_last_film_to_debugging_hard_drive(e_controller_index controller_index, void* copy_buffer, uns32 copy_buffer_size, c_synchronized_long* success, c_synchronized_long* done)
{
	return INVOKE(0x00526BB0, saved_game_files_save_last_film_to_debugging_hard_drive, controller_index, copy_buffer, copy_buffer_size, success, done);
}

void __cdecl saved_game_files_uniquify_name(e_controller_index controller_index, e_saved_game_file_type saved_game_file_type, wchar_t* name, int32 maximum_characters)
{
	INVOKE(0x00526BC0, saved_game_files_uniquify_name, controller_index, saved_game_file_type, name, maximum_characters);
}

//.text:00526D70 ; void __cdecl saved_game_files_unregister_text_parsers()

void __cdecl saved_game_files_update()
{
	INVOKE(0x00526DD0, saved_game_files_update);
}

//.text:00526E00 ; int32 __cdecl saved_game_files_update_metadata(e_controller_index, int32, const wchar_t*, const wchar_t*, c_synchronized_long*, c_synchronized_long*)
//.text:00526E90 ; e_async_completion __cdecl saved_game_files_update_metadata_callback(s_async_task*)
//.text:00527040 ; int32 __cdecl saved_game_files_write(e_controller_index, int32, e_saved_game_file_type, const s_blffile_saved_game_file*, uns32, c_synchronized_long*, c_synchronized_long*)
//.text:005270F0 ; void __cdecl saved_game_files_write_saved_game_state(const s_game_state_header*, int32, const void*, int32)

bool __cdecl saved_game_read_metadata_from_file(s_file_reference* file, s_saved_game_item_metadata* out_metadata)
{
	return INVOKE(0x00527380, saved_game_read_metadata_from_file, file, out_metadata);
}

//.text:005273E0 ; bool __cdecl saved_game_read_metadata_from_open_file_internal(s_file_reference*, s_saved_game_item_metadata*)
//.text:005274C0 ; saved_game_synchronize_contents_from_metadata
//.text:005276E0 ; bool __cdecl saved_game_write_metadata_to_file(s_file_reference*, const s_saved_game_item_metadata*, e_game_content_type, bool)
//.text:00527720 ; bool __cdecl saved_game_write_metadata_to_open_file_internal(s_file_reference*, const s_saved_game_item_metadata*, e_game_content_type, bool)
//.text:00527860 ; e_async_completion __cdecl saved_games_async_work_slice_finish(bool, bool, e_saved_game_file_operation_result, c_synchronized_long*, c_synchronized_long*)
//.text:005278C0 ; 
//.text:005278F0 ; public: void __cdecl s_saved_game_item_metadata::set(const s_ui_saved_game_item_metadata*)

void s_ui_saved_game_item_metadata::set(const s_saved_game_item_metadata* metadata)
{
	INVOKE_CLASS_MEMBER(0x00527A30, s_ui_saved_game_item_metadata, set, metadata);
}

