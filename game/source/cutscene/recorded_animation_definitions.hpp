#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

#define RECORDED_ANIMATION_VERSION 4

struct recorded_animation_definition
{
	char name[32];
	uint8 version;
	char raw_animation_data;
	uint8 unit_control_data_version;

	uint8 DVU[0x1];

	short length_of_animation; // ticks

	uint8 DCAU[0x2];
	uint8 HL[0x4];

	s_tag_data recorded_animation_event_stream;
};
static_assert(sizeof(recorded_animation_definition) == 0x40);

