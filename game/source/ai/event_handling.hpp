#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"

struct ai_spatial_effect
{
	int16 count;
	real_point3d position;
	int32 last_tick;
};
COMPILE_ASSERT(sizeof(ai_spatial_effect) == 0x14);

struct ai_allegiance_information_data
{
	int16 team1_index;
	int16 team2_index;
	int16 type;
};
COMPILE_ASSERT(sizeof(ai_allegiance_information_data) == 0x6);

struct ai_combat_stimulus_information_data
{
	int32 unused;
};
COMPILE_ASSERT(sizeof(ai_combat_stimulus_information_data) == 0x4);

struct ai_information_target_knowledge_data
{
	int32 object_index;
};
COMPILE_ASSERT(sizeof(ai_information_target_knowledge_data) == 0x4);

struct ai_information_data
{
	int32 information_type;
	int32 pref_index;
	union
	{
		ai_allegiance_information_data allegiance;
		ai_combat_stimulus_information_data combat_stimulus;
		invitation_data invitation;
		ai_information_target_knowledge_data target_knowledge;
	};
};
COMPILE_ASSERT(sizeof(ai_information_data) == 0x18);

struct ai_information_packet
{
	int16 dialogue_index;
	int16 pad0;
	int32 listener_object_index;
	ai_information_data information_data;
};
COMPILE_ASSERT(sizeof(ai_information_packet) == 0x20);

struct ai_sound_data
{
	int16 type;
	int16 volume;
	union
	{
		ai_spatial_effect spatial;
		ai_information_data comm;
	};
};
COMPILE_ASSERT(sizeof(ai_sound_data) == 0x1C);

extern void __cdecl event_handling_initialize();

