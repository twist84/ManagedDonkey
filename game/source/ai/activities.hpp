#pragma once

struct s_posture_info
{
	e_ai_posture posture;
	int32 posture_name;
	e_posture_category category;
	int16 involuntary_vocalization_index;
	bool use_posture_physics;
	bool disallow_targeting;
	bool debilitated;
};
COMPILE_ASSERT(sizeof(s_posture_info) == 0x14);

extern e_ai_posture const(&g_activity_posture_variants)[k_activity_count][k_max_activity_variants];
extern int32 const(&g_activity_names)[k_activity_count];
extern s_posture_info const(&g_posture_info)[k_ai_posture_count];
extern int16(&g_variant_counts)[k_activity_count];

extern void __cdecl activities_initialize();

