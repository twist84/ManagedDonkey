#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_dialogue_definition
{
	c_typed_tag_reference<AI_DIALOGUE_GLOBALS_TAG, INVALID_TAG> global_dialogue_info;
	uns32 flags;
	s_tag_block vocalizations;

	// 3-letter mission dialogue designator name
	c_string_id mission_dialogue_designator;
};
COMPILE_ASSERT(sizeof(s_dialogue_definition) == 0x24);

// unit_datum: object_header_block_reference seat_storage;
struct s_seat_storage
{
	int32 dialogue_definition_index;
	c_string_id mission_dialogue_designator;
	c_string_id dialogue_effect;

	byte __dataC[0x50];
};
COMPILE_ASSERT(sizeof(s_seat_storage) == 0x5C);

