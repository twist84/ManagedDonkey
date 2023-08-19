#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

#define RECORDED_ANIMATION_VERSION 4

struct recorded_animation_definition
{
	char name[32];
	byte version;
	char raw_animation_data;
	byte unit_control_data_version;

	byte DVU[0x1];

	short length_of_animation; // ticks

	byte DCAU[0x2];
	byte HL[0x4];

	s_tag_data recorded_animation_event_stream;
};
static_assert(sizeof(recorded_animation_definition) == 0x40);

