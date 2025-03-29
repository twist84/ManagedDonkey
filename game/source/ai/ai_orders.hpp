#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct orders_definition
{
	char name[32];
	short style;
	word pad1;
	dword_flags flags;
	short_enum force_combat_status;
	short pad2;
	char script_name[32];
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

extern short orders_get_by_name(char const* name);

