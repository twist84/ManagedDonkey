#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

#define RECORDED_ANIMATION_VERSION 4

struct recorded_animation_definition
{
	char name[32];
	uns8 version;
	int8 raw_animation_data;
	uns8 unit_control_data_version;

	byte DVU[0x1];

	int16 length_of_animation; // ticks

	byte DCAU[0x2];
	byte HL[0x4];

	s_tag_data recorded_animation_event_stream;
};
COMPILE_ASSERT(sizeof(recorded_animation_definition) == 0x40);

