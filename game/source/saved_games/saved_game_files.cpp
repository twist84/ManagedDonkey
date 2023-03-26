#include "saved_games/saved_game_files.hpp"

#include "memory/byte_swapping.hpp"


void s_saved_game_item_metadata::byteswap()
{
	unique_id = _byteswap_uint64(unique_id);
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

