#include "saved_games/saved_game_files.hpp"

#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x0189D930, c_saved_game_files_globals, g_saved_game_files_globals);

//.text:00525AB0 ; 
//.text:00525B50 ; 
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

//.text:00525DC0 ; 
//.text:00525E60 ; bool __cdecl saved_game_files_enumeration_in_progress()
//.text:00525E90 ; 
//.text:00525EC0 ; 
//.text:00525F00 ; 
//.text:00525FB0 ; 
//.text:00526080 ; 
//.text:005260F0 ; 
//.text:00526180 ; 
//.text:005261F0 ; 
//.text:00526270 ; 
//.text:00526320 ; 
//.text:005263E0 ; 
//.text:005264E0 ; long __cdecl saved_game_files_get_personal_storage_content_item(e_controller_index, bool)
//.text:00526550 ; char const * __cdecl saved_game_files_get_saved_game_file_name(e_saved_game_file_type)
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
//.text:00526B30 ; 
//.text:00526B80 ; 
//.text:00526BB0 ; 
//.text:00526BC0 ; 
//.text:00526D70 ; 

void __cdecl saved_game_files_update()
{
	INVOKE(0x00526DD0, saved_game_files_update);
}

//.text:00526E00 ; long __cdecl saved_game_files_update_metadata(e_controller_index, long, wchar_t const*, wchar_t const*, c_synchronized_long*, c_synchronized_long*)
//.text:00526E90 ; e_async_completion __cdecl saved_game_files_update_metadata_callback(s_async_task*)
//.text:00527040 ; long __cdecl saved_game_files_write(e_controller_index, long, e_saved_game_file_type, s_blffile_saved_game_file const*, dword, c_synchronized_long*, c_synchronized_long*)
//.text:005270F0 ; void __cdecl saved_game_files_write_saved_game_state_blocking(s_game_state_header const*, long, void const*, long)
//.text:00527380 ; bool __cdecl saved_game_read_metadata_from_file_blocking(s_file_reference*, s_content_item_metadata*)
//.text:005273E0 ; bool __cdecl saved_game_read_metadata_from_open_file_internal(s_file_reference*, s_content_item_metadata*)
//.text:005274C0 ; 
//.text:005276E0 ; bool __cdecl saved_game_write_metadata_to_file(s_file_reference*, s_saved_game_item_metadata const*, e_game_content_type, bool)
//.text:00527720 ; bool __cdecl saved_game_write_metadata_to_open_file_internal(s_file_reference*, s_saved_game_item_metadata const*, e_game_content_type, bool)
//.text:00527860 ; enum e_async_completion __cdecl saved_games_async_work_slice_finish(bool, bool, e_saved_game_file_operation_result, c_synchronized_long*, c_synchronized_long*)
//.text:005278B2 ; 
//.text:005278F0 ; 
//.text:00527A30 ; 

void s_content_item_metadata::byteswap()
{
	bswap_qword_inplace(unique_id);
	for (long i = 0; i < NUMBEROF(name); i++) bswap_word_inplace(name[i]);
	bswap_dword_inplace(file_type);
	ASSERT(array_is_zeroed(pad0));
	bswap_qword_inplace(author_id);
	bswap_qword_inplace(size_in_bytes);
	bswap_qword_inplace(date);
	bswap_dword_inplace(length_seconds);
	bswap_dword_inplace(campaign_id);
	bswap_dword_inplace(map_id);
	bswap_dword_inplace(game_engine_type);
	bswap_dword_inplace(campaign_difficulty);
	ASSERT(array_is_zeroed(pad));
	bswap_qword_inplace(game_id);
}

bool s_content_item_metadata::is_valid() const
{
	return false; //content_item_metadata_valid(this);
}

void s_content_item_metadata::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(is_valid());

	packet->write_qword("unique-id", unique_id, 64);
	packet->write_string_wchar("name", name, NUMBEROF(name));
	packet->write_string_utf8("description", description, NUMBEROF(description));
	packet->write_string("author", author, NUMBEROF(author));
	packet->write_integer("file-type", static_cast<long>(file_type) + 1, 5);
	packet->write_bool("author-is-xuid-online", author_is_xuid_online);
	packet->write_qword("author-xuid", author_id, 64);
	packet->write_qword("size-in-bytes", size_in_bytes, 64);
	packet->write_qword("date", date, 64);
	packet->write_integer("length-seconds", length_seconds, 32);
	packet->write_integer("campaign-id", campaign_id, 32);
	packet->write_integer("map-id", map_id, 32);
	packet->write_integer("game-engine-type", game_engine_type, 4);
	packet->write_integer("campaign-difficulty", campaign_difficulty + 1, 3);
	packet->write_integer("hopper-id", 0xFF, 16);
	packet->write_qword("game-id", game_id, 64);
}

void s_content_item_metadata::decode_from_mcc(c_bitstream* packet)
{
	long const k_name_size = NUMBEROF(name);
	long const k_description_size = NUMBEROF(description);
	long const k_author_size = NUMBEROF(author);

	unique_id = packet->read_qword("unique-id", 64);
	packet->read_string_wchar("name", name, k_name_size);
	packet->read_string_utf8("description", description, k_description_size);
	packet->read_string("author", author, k_author_size);
	file_type = static_cast<e_saved_game_file_type>(packet->read_integer("file-type", 5) - 1);
	author_is_xuid_online = packet->read_bool("author-is-xuid-online");
	author_id = packet->read_qword("author-xuid", 64);
	size_in_bytes = packet->read_qword("size-in-bytes", 64);
	date = packet->read_qword("date", 64);
	length_seconds = packet->read_integer("length-seconds", 32);
	campaign_id = packet->read_integer("campaign-id", 32);
	map_id = packet->read_integer("map-id", 32);
	game_engine_type = packet->read_integer("game-engine-type", 4);
	campaign_difficulty = packet->read_integer("campaign-difficulty" + 1, 3);
	word hopper_id = static_cast<word>(packet->read_integer("hopper-id", 16));
	game_id = packet->read_qword("game-id", 64);
}

