#pragma once

enum
{
	k_max_activity_variants = 10
};

enum e_activity
{
	_activity_none = 0,
	_activity_patrol,
	_activity_stand,
	_activity_crouch,
	_activity_stand_drawn,
	_activity_crouch_drawn,
	_activity_corner,
	_activity_corner_open,
	_activity_bunker,
	_activity_bunker_open,
	_activity_combat,
	_activity_backup,
	_activity_guard,
	_activity_guard_crouch,
	_activity_guard_wall,
	_activity_typing,
	_activity_kneel,
	_activity_gaze,
	_activity_poke,
	_activity_sniff,
	_activity_track,
	_activity_watch,
	_activity_examine,
	_activity_sleep,
	_activity_at_ease,
	_activity_cower,
	_activity_tai_chi,
	_activity_pee,
	_activity_doze,
	_activity_eat,
	_activity_medic,
	_activity_work,
	_activity_cheering,
	_activity_injured,
	_activity_captured,

	k_activity_count
};

enum e_ai_posture
{
	_ai_posture_none = 0,
	_ai_posture_corner_cover_left,
	_ai_posture_corner_cover_right,
	_ai_posture_corner_open_left,
	_ai_posture_corner_open_right,
	_ai_posture_bunker_cover,
	_ai_posture_bunker_open,
	_ai_posture_act_combat,
	_ai_posture_act_backup_left,
	_ai_posture_act_backup_right,
	_ai_posture_act_asleep_1,
	_ai_posture_act_guard_1,
	_ai_posture_act_guard_2,
	_ai_posture_act_guard_3,
	_ai_posture_act_guard_4,
	_ai_posture_act_guard_crouch,
	_ai_posture_act_guard_crouch_1,
	_ai_posture_act_guard_wall_1,
	_ai_posture_act_typing,
	_ai_posture_act_kneel_1,
	_ai_posture_act_kneel_2,
	_ai_posture_act_gaze_1,
	_ai_posture_act_gaze_2,
	_ai_posture_act_poke_1,
	_ai_posture_act_sniff_1,
	_ai_posture_act_track_1,
	_ai_posture_act_watch_1,
	_ai_posture_act_examine_1,
	_ai_posture_asleep,
	_ai_posture_act_smoking,
	_ai_posture_act_cards,
	_ai_posture_act_cower_1,
	_ai_posture_act_cower_2,
	_ai_posture_act_tai_chi,
	_ai_posture_act_pee_1,
	_ai_posture_act_doze_sit_1,
	_ai_posture_act_doze_stand_1,
	_ai_posture_act_eat_1,
	_ai_posture_act_cheering_1,
	_ai_posture_act_cheering_2,
	_ai_posture_act_cheering_3,
	_ai_posture_act_cheering_4,
	_ai_posture_act_cheering_5,
	_ai_posture_act_injured_ankle,
	_ai_posture_act_injured_sit_wall,
	_ai_posture_act_injured_shoulder,
	_ai_posture_act_injured_gut,
	_ai_posture_act_laser,
	_ai_posture_act_laser1,
	_ai_posture_act_laser2,
	_ai_posture_act_laser3,
	_ai_posture_act_laser4,
	_ai_posture_act_laser5,
	_ai_posture_act_medic,
	_ai_posture_act_work,
	_ai_posture_act_work_stand,
	_ai_posture_act_captured_sit_wall,
	_ai_posture_act_captured_cuffed_wall,
	_ai_posture_act_captured_kneel,
	_ai_posture_act_bugger_lunch,

	k_ai_posture_count
};

enum e_posture_category
{
	_posture_category_none = 0,
	_posture_category_open,
	_posture_category_closed,

	k_posture_category_count
};

struct s_posture_info
{
	e_ai_posture posture;
	long posture_name;
	e_posture_category category;
	short involuntary_vocalization_index;
	bool use_posture_physics;
	bool disallow_targeting;
	bool debilitated;
};
static_assert(sizeof(s_posture_info) == 0x14);

extern e_ai_posture const(&g_activity_posture_variants)[k_activity_count][k_max_activity_variants];
extern long const(&g_activity_names)[k_activity_count];
extern s_posture_info const(&g_posture_info)[k_ai_posture_count];
extern short(&g_variant_counts)[k_activity_count];

extern void __cdecl activities_initialize();

