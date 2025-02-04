#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"

struct ai_spatial_effect
{
	short count;
	real_point3d position;
	long last_tick;
};
static_assert(sizeof(ai_spatial_effect) == 0x14);

struct ai_allegiance_information_data
{
	short team1_index;
	short team2_index;
	short type;
};
static_assert(sizeof(ai_allegiance_information_data) == 0x6);

struct ai_combat_stimulus_information_data
{
	long unused;
};
static_assert(sizeof(ai_combat_stimulus_information_data) == 0x4);

struct ai_information_target_knowledge_data
{
	long object_index;
};
static_assert(sizeof(ai_information_target_knowledge_data) == 0x4);

struct ai_information_data
{
	long information_type;
	long pref_index;
	union
	{
		ai_allegiance_information_data allegiance;
		ai_combat_stimulus_information_data combat_stimulus;
		invitation_data invitation;
		ai_information_target_knowledge_data target_knowledge;
	};
};
static_assert(sizeof(ai_information_data) == 0x18);

struct ai_sound_data
{
	short type;
	short volume;
	union
	{
		ai_spatial_effect spatial;
		ai_information_data comm;
	};
};
static_assert(sizeof(ai_sound_data) == 0x1C);

extern void __cdecl event_handling_initialize();

