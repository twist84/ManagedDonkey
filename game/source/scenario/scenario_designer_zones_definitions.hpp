#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_scenario_designer_zone
{
	c_string_id name;
	uns32 flags;

	s_tag_block biped;
	s_tag_block vehicle;
	s_tag_block weapon;
	s_tag_block equipment;
	s_tag_block scenery;
	s_tag_block machine;
	s_tag_block terminal;
	s_tag_block arg_device;
	s_tag_block control;
	s_tag_block sound_scenery;
	s_tag_block crate;
	s_tag_block creature;
	s_tag_block giant;
	s_tag_block effect_scenery;
	s_tag_block character;
	s_tag_block budget_reference;
};
COMPILE_ASSERT(sizeof(s_scenario_designer_zone) == 0xC8);

