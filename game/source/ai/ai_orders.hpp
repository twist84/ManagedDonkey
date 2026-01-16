#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct orders_definition
{
	char name[32];
	int16 style;
	uns16 pad1;
	uns32 flags;
	int16 force_combat_status;
	int16 pad2;
	char script_name[32];
	int16 script_index;
	int16 follow_squad;
	real32 follow_radius;
	s_tag_block primary_area_set;
	s_tag_block secondary_area_set;
	s_tag_block secondary_set_trigger;
	s_tag_block special_movement;
	s_tag_block order_endings;
	s_tag_block pureform_distribution;
};
COMPILE_ASSERT(sizeof(orders_definition) == 0x9C);

struct trigger_definition;

extern int16 orders_get_by_name(const char* name);

