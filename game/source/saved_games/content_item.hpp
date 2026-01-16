#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/content/content_item_metadata.hpp"

class c_content_catalogue;

struct s_content_item
{
	enum
	{
		k_sizeof_xcontent_data = 0x138,
	};

	enum e_content_item_flags
	{
		_valid_bit = 0,
		_corrupt_bit,
		_open_bit,
		_busy_bit,
		_metadata_valid_bit,
		_autosave_queue_item_bit,
		_new_item_bit,

		k_number_of_content_item_flags,
	};

	int16 identifier;
	uns16 pad;
	c_synchronized_long m_flags;
	e_game_content_type m_game_content_type;
	c_content_catalogue* m_content_catalogue;
	s_saved_game_item_metadata m_metadata;
	byte _content_data_space[k_sizeof_xcontent_data];
};
COMPILE_ASSERT(sizeof(s_content_item) == 0x240);

