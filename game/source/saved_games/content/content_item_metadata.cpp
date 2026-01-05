#include "saved_games/content/content_item_metadata.hpp"

#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

void s_saved_game_item_metadata::byteswap()
{
	bswap_uns64_inplace(unique_id);
	for (int32 i = 0; i < NUMBEROF(display_name); i++) bswap_uns16_inplace(display_name[i]);
	file_type = (e_saved_game_file_type)bswap_uns32(file_type);
	ASSERT(array_is_zeroed(pad0));
	bswap_uns64_inplace(author_xuid);
	bswap_uns64_inplace(size_in_bytes);
	bswap_uns64_inplace(date);
	bswap_uns32_inplace(length_seconds);
	campaign_id = (e_campaign_id)bswap_uns32(campaign_id);
	map_id = (e_map_id)bswap_uns32(map_id);
	game_engine_index = (e_game_engine_type)bswap_uns32(game_engine_index);
	campaign_difficulty = (e_campaign_difficulty_level)bswap_uns32(campaign_difficulty);
	ASSERT(array_is_zeroed(pad));
	bswap_uns64_inplace(game_id);
}

void s_saved_game_item_metadata::decode(c_bitstream* packet)
{
	//INVOKE_CLASS_MEMBER(0x00523FE0, s_saved_game_item_metadata, decode, packet);

	int32 const k_display_name_size = NUMBEROF(display_name);
	int32 const k_description_size = NUMBEROF(description);
	int32 const k_author_size = NUMBEROF(author);

	unique_id = packet->read_qword("unique-id", 64);
	packet->read_string_wchar("name", display_name, k_display_name_size);
	packet->read_string_utf8("description", description, k_description_size);
	packet->read_string("author", author, k_author_size);
	file_type = static_cast<e_saved_game_file_type>(packet->read_integer("file-type", 5) - 1);
	author_is_xuid_online = packet->read_bool("author-is-xuid-online");
	author_xuid = packet->read_qword("author-xuid", 64);
	size_in_bytes = packet->read_qword("size-in-bytes", 64);
	date = packet->read_qword("date", 64);
	length_seconds = packet->read_integer("length-seconds", 32);
	campaign_id = (e_campaign_id)packet->read_integer("campaign-id", 32);
	map_id = (e_map_id)packet->read_integer("map-id", 32);
	game_engine_index = (e_game_engine_type)packet->read_integer("game-engine-type", 4);
	campaign_difficulty = (e_campaign_difficulty_level)packet->read_integer("campaign-difficulty" + 1, 3);
	campaign_insertion_point = (uns8)packet->read_integer("campaign-insertion-point", 4);
	campaign_survival_enabled = packet->read_bool("campaign-survival-enabled");
	game_id = packet->read_qword("game-id", 64);
}

void s_saved_game_item_metadata::encode(c_bitstream* packet) const
{
	//INVOKE_CLASS_MEMBER(0x00524120, s_saved_game_item_metadata, encode, packet);

	ASSERT(is_valid());

	int32 const k_display_name_size = NUMBEROF(display_name);
	int32 const k_description_size = NUMBEROF(description);
	int32 const k_author_size = NUMBEROF(author);

	packet->write_qword("unique-id", unique_id, 64);
	packet->write_string_wchar("name", display_name, k_display_name_size);
	packet->write_string_utf8("description", description, k_description_size);
	packet->write_string("author", author, k_author_size);
	packet->write_integer("file-type", static_cast<int32>(file_type) + 1, 5);
	packet->write_bool("author-is-xuid-online", author_is_xuid_online);
	packet->write_qword("author-xuid", author_xuid, 64);
	packet->write_qword("size-in-bytes", size_in_bytes, 64);
	packet->write_qword("date", date, 64);
	packet->write_integer("length-seconds", length_seconds, 32);
	packet->write_integer("campaign-id", campaign_id, 32);
	packet->write_integer("map-id", map_id, 32);
	packet->write_integer("game-engine-type", game_engine_index, 4);
	packet->write_integer("campaign-difficulty", int32(campaign_difficulty) + 1, 3);
	packet->write_integer("campaign-insertion-point", campaign_insertion_point, 4);
	packet->write_bool("campaign-survival-enabled", campaign_survival_enabled);
	packet->write_qword("game-id", game_id, 64);
}

//.text:00524430 ; 
//.text:00524440 ; e_game_engine_type __cdecl game_content_type_to_game_engine_index(e_game_content_type)
//.text:005244C0 ; e_saved_game_file_type __cdecl game_content_type_to_saved_game_file_type(e_game_content_type)
//.text:00524570 ; e_game_content_type __cdecl game_engine_index_to_game_content_type(e_game_engine_type)
//.text:005245F0 ; e_saved_game_file_type __cdecl game_engine_index_to_saved_game_file_type(e_game_engine_type)

uns64 __cdecl s_saved_game_item_metadata::generate_unique_id()
{
	return INVOKE(0x00524670, s_saved_game_item_metadata::generate_unique_id);
}

//.text:005246C0 ; public: c_game_engine_base_variant* c_game_variant::get_active_variant_writeable()
//.text:005246E0 ; public: static e_metadata_file_type s_saved_game_item_metadata::get_file_type_from_saved_game_type(e_saved_game_file_type)
//.text:00524740 ; public: e_game_content_type s_saved_game_item_metadata::get_game_content_type() const

e_gui_game_mode s_saved_game_item_metadata::get_gui_game_mode() const
{
	return INVOKE_CLASS_MEMBER(0x00524750, s_saved_game_item_metadata, get_gui_game_mode);
}

//.text:005247A0 ; public: bool s_saved_game_item_metadata::get_map_image_filename(c_static_string<256>*) const

e_metadata_file_type s_saved_game_item_metadata::get_metadata_file_type() const
{
	return INVOKE_CLASS_MEMBER(0x00524820, s_saved_game_item_metadata, get_metadata_file_type);
}

//.text:00524830 ; public: bool s_saved_game_item_metadata::get_metadata_integer(int32, int32*) const
//.text:00524940 ; public: bool s_saved_game_item_metadata::get_metadata_text(int32, c_static_wchar_string<1024>*) const

void s_saved_game_item_metadata::initialize_from_current_game_settings(e_controller_index controller_index, e_saved_game_file_type file_type, const wchar_t* in_name, const wchar_t* in_description, uns64 size)
{
	INVOKE_CLASS_MEMBER(0x00525000, s_saved_game_item_metadata, initialize_from_current_game_settings, controller_index, file_type, in_name, in_description, size);
}

bool s_saved_game_item_metadata::is_valid() const
{
	return false; //content_item_metadata_valid(this);
}

void s_saved_game_item_metadata::decode_from_mcc(c_bitstream* packet)
{
	int32 const k_display_name_size = NUMBEROF(display_name);
	int32 const k_description_size = NUMBEROF(description);
	int32 const k_author_size = NUMBEROF(author);

	unique_id = packet->read_qword("unique-id", 64);
	packet->read_string_wchar("name", display_name, k_display_name_size);
	packet->read_string_utf8("description", description, k_description_size);
	packet->read_string("author", author, k_author_size);
	file_type = static_cast<e_saved_game_file_type>(packet->read_integer("file-type", 5) - 1);
	author_is_xuid_online = packet->read_bool("author-is-xuid-online");
	author_xuid = packet->read_qword("author-xuid", 64);
	size_in_bytes = packet->read_qword("size-in-bytes", 64);
	date = packet->read_qword("date", 64);
	length_seconds = packet->read_integer("length-seconds", 32);
	campaign_id = (e_campaign_id)packet->read_integer("campaign-id", 32);
	map_id = (e_map_id)packet->read_integer("map-id", 32);
	game_engine_index = (e_game_engine_type)packet->read_integer("game-engine-type", 4);
	campaign_difficulty = (e_campaign_difficulty_level)packet->read_integer("campaign-difficulty" + 1, 3);
	uns16 hopper_id = static_cast<uns16>(packet->read_integer("hopper-id", 16));
	game_id = packet->read_qword("game-id", 64);
}

void s_saved_game_item_metadata::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(is_valid());

	int32 const k_display_name_size = NUMBEROF(display_name);
	int32 const k_description_size = NUMBEROF(description);
	int32 const k_author_size = NUMBEROF(author);

	packet->write_qword("unique-id", unique_id, 64);
	packet->write_string_wchar("name", display_name, k_display_name_size);
	packet->write_string_utf8("description", description, k_description_size);
	packet->write_string("author", author, k_author_size);
	packet->write_integer("file-type", int32(file_type) + 1, 5);
	packet->write_bool("author-is-xuid-online", author_is_xuid_online);
	packet->write_qword("author-xuid", author_xuid, 64);
	packet->write_qword("size-in-bytes", size_in_bytes, 64);
	packet->write_qword("date", date, 64);
	packet->write_integer("length-seconds", length_seconds, 32);
	packet->write_integer("campaign-id", campaign_id, 32);
	packet->write_integer("map-id", map_id, 32);
	packet->write_integer("game-engine-type", game_engine_index, 4);
	packet->write_integer("campaign-difficulty", int32(campaign_difficulty) + 1, 3);
	packet->write_integer("hopper-id", 0xFF, 16);
	packet->write_qword("game-id", game_id, 64);
}

