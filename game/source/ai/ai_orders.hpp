#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct orders_definition
{
	c_static_string<k_tag_string_length> name;
	short style;

	// pad
	byte YATIWNRNR[0x2];

	dword_flags flags;
	short_enum force_combat_status;

	// pad
	byte PWY[0x2];

	c_static_string<k_tag_string_length> entry_script;
	short script_index;
	short follow_squad;
	real follow_radius;

	s_tag_block primary_area_set;
	s_tag_block secondary_area_set;
	s_tag_block secondary_set_trigger;
	s_tag_block special_movement;
	s_tag_block order_endings;
	s_tag_block pureform_distribution;
};
static_assert(sizeof(orders_definition) == 0x9C);

struct trigger_definition;

