#include "ai/activities.hpp"

#include "cseries/cseries.hpp"
#include "tag_files/string_ids.hpp"

REFERENCE_DECLARE_2D_ARRAY(0x01764FD0, e_ai_posture const, g_activity_posture_variants, k_activity_count, k_max_activity_variants);
REFERENCE_DECLARE_ARRAY(0x01765548, int32 const, g_activity_names, k_activity_count);
REFERENCE_DECLARE_ARRAY(0x01765808, s_posture_info const, g_posture_info, k_ai_posture_count);
REFERENCE_DECLARE_ARRAY(0x058406E0, int16, g_variant_counts, k_activity_count);

void __cdecl activities_initialize()
{
	INVOKE(0x01457E60, activities_initialize);
}

//.text:01457EA0 ; int32 __cdecl activity_get_posture_name(int32, int32)
//.text:01457EF0 ; void __cdecl activity_handle_spawn_point_change(int16, int16)
//.text:01458240 ; bool __cdecl activity_index_get_by_name(int32, int16*, int16*)
//.text:014582F0 ; void __cdecl actor_activity_reset(int32)
//.text:01458330 ; bool __cdecl actor_activity_set(int32, int16, int16, int16, e_patrol_definition_types)
//.text:014583F0 ; bool __cdecl actor_activity_set(int32, int32, int16, e_patrol_definition_types)
//.text:01458450 ; s_tag_block* __cdecl actor_get_patrol_block(int32, int16*)
//.text:01458560 ; 
//.text:01458580 ; 
//.text:01458590 ; 
//.text:014585B0 ; 
//.text:014585C0 ; e_ai_posture __cdecl posture_get_by_name(int32)
//.text:014585F0 ; const s_posture_info* __cdecl posture_info_get(e_ai_posture)
//.text:01458610 ; const s_posture_info* __cdecl posture_info_get_by_name(int32)

/*e_ai_posture const g_activity_posture_variants[k_activity_count][k_ai_action_count]
{
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_none
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_patrol
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_stand
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_crouch
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_stand_drawn
	{ _ai_posture_none,                  _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_crouch_drawn
	{ _ai_posture_corner_cover_left,     _ai_posture_corner_cover_right,       _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_corner
	{ _ai_posture_corner_open_left,      _ai_posture_corner_open_right,        _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_corner_open
	{ _ai_posture_bunker_cover,          _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_bunker
	{ _ai_posture_bunker_open,           _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_bunker_open
	{ _ai_posture_act_combat,            _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_combat
	{ _ai_posture_act_backup_left,       _ai_posture_act_backup_right,         _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_backup
	{ _ai_posture_act_guard_1,           _ai_posture_act_guard_2,              _ai_posture_act_guard_3,          _ai_posture_act_guard_4,     _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_guard
	{ _ai_posture_act_guard_crouch,      _ai_posture_act_guard_crouch_1,       _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_guard_crouch
	{ _ai_posture_act_guard_wall_1,      _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_guard_wall
	{ _ai_posture_act_typing,            _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_typing
	{ _ai_posture_act_kneel_1,           _ai_posture_act_kneel_2,              _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_kneel
	{ _ai_posture_act_gaze_1,            _ai_posture_act_gaze_2,               _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_gaze
	{ _ai_posture_act_poke_1,            _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_poke
	{ _ai_posture_act_sniff_1,           _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_sniff
	{ _ai_posture_act_track_1,           _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_track
	{ _ai_posture_act_watch_1,           _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_watch
	{ _ai_posture_act_examine_1,         _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_examine
	{ _ai_posture_asleep,                _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_sleep
	{ _ai_posture_act_smoking,           _ai_posture_act_cards,                _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_at_ease
	{ _ai_posture_act_cower_1,           _ai_posture_act_cower_2,              _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_cower
	{ _ai_posture_act_tai_chi,           _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_tai_chi
	{ _ai_posture_act_pee_1,             _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_pee
	{ _ai_posture_act_doze_sit_1,        _ai_posture_act_doze_stand_1,         _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_doze
	{ _ai_posture_act_eat_1,             _ai_posture_act_bugger_lunch,         _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_eat
	{ _ai_posture_act_medic,             _ai_posture_none,                     _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_medic
	{ _ai_posture_act_work,              _ai_posture_act_work_stand,           _ai_posture_none,                 _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_work
	{ _ai_posture_act_cheering_1,        _ai_posture_act_cheering_2,           _ai_posture_act_cheering_3,       _ai_posture_act_cheering_4,  _ai_posture_act_cheering_5, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_cheering
	{ _ai_posture_act_injured_ankle,     _ai_posture_act_injured_sit_wall,     _ai_posture_act_injured_shoulder, _ai_posture_act_injured_gut, _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_injured
	{ _ai_posture_act_captured_sit_wall, _ai_posture_act_captured_cuffed_wall, _ai_posture_act_captured_kneel,   _ai_posture_none,            _ai_posture_none,           _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none, _ai_posture_none }, // _activity_captured
};*/

/*int32 const g_activity_names[k_activity_count]
{
	STRING_ID(global, none),
	STRING_ID(global, patrol),
	STRING_ID(global, stand),
	STRING_ID(global, crouch),
	STRING_ID(global, stand_drawn),
	STRING_ID(global, crouch_drawn),
	STRING_ID(global, corner),
	STRING_ID(global, corner_open),
	STRING_ID(global, bunker),
	STRING_ID(global, bunker_open),
	STRING_ID(global, combat),
	STRING_ID(global, backup),
	STRING_ID(global, guard),
	STRING_ID(global, guard_crouch),
	STRING_ID(global, guard_wall),
	STRING_ID(global, typing),
	STRING_ID(global, kneel),
	STRING_ID(global, gaze),
	STRING_ID(global, poke),
	STRING_ID(global, sniff),
	STRING_ID(global, track),
	STRING_ID(global, watch),
	STRING_ID(global, examine),
	STRING_ID(global, sleep),
	STRING_ID(global, at_ease),
	STRING_ID(global, cower),
	STRING_ID(global, tai_chi),
	STRING_ID(global, pee),
	STRING_ID(global, doze),
	STRING_ID(global, eat),
	STRING_ID(global, medic),
	STRING_ID(global, work),
	STRING_ID(global, cheering),
	STRING_ID(global, injured),
	STRING_ID(global, captured),
};*/

/*s_posture_info const g_posture_info[k_ai_posture_count]
{
	{
		.posture = _ai_posture_none,
		.posture_name = STRING_ID(global, none),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_corner_cover_left,
		.posture_name = STRING_ID(global, corner_cover_left),
		.category = _posture_category_closed,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_corner_cover_right,
		.posture_name = STRING_ID(global, corner_cover_right),
		.category = _posture_category_closed,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_corner_open_left,
		.posture_name = STRING_ID(global, corner_open_left),
		.category = _posture_category_open,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_corner_open_right,
		.posture_name = STRING_ID(global, corner_open_right),
		.category = _posture_category_open,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_bunker_cover,
		.posture_name = STRING_ID(global, bunker_cover),
		.category = _posture_category_closed,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_bunker_open,
		.posture_name = STRING_ID(global, bunker_open),
		.category = _posture_category_open,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_combat,
		.posture_name = STRING_ID(global, act_combat),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_backup_left,
		.posture_name = STRING_ID(global, act_backup_left),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_backup_right,
		.posture_name = STRING_ID(global, act_backup_right),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_asleep_1,
		.posture_name = STRING_ID(global, act_asleep_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 40,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_1,
		.posture_name = STRING_ID(global, act_guard_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_2,
		.posture_name = STRING_ID(global, act_guard_2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_3,
		.posture_name = STRING_ID(global, act_guard_3),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_4,
		.posture_name = STRING_ID(global, act_guard_4),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_crouch,
		.posture_name = STRING_ID(global, act_guard_crouch),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_crouch_1,
		.posture_name = STRING_ID(global, act_guard_crouch_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_guard_wall_1,
		.posture_name = STRING_ID(global, act_guard_wall_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_typing,
		.posture_name = STRING_ID(global, act_typing),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_kneel_1,
		.posture_name = STRING_ID(global, act_kneel_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_kneel_2,
		.posture_name = STRING_ID(global, act_kneel_2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_gaze_1,
		.posture_name = STRING_ID(global, act_gaze_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_gaze_2,
		.posture_name = STRING_ID(global, act_gaze_2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_poke_1,
		.posture_name = STRING_ID(global, act_poke_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_sniff_1,
		.posture_name = STRING_ID(global, act_sniff_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_track_1,
		.posture_name = STRING_ID(global, act_track_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_watch_1,
		.posture_name = STRING_ID(global, act_watch_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_examine_1,
		.posture_name = STRING_ID(global, act_examine_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_asleep,
		.posture_name = STRING_ID(global, asleep),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 40,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_smoking,
		.posture_name = STRING_ID(global, act_smoking),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cards,
		.posture_name = STRING_ID(global, act_cards),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cower_1,
		.posture_name = STRING_ID(global, act_cower_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cower_2,
		.posture_name = STRING_ID(global, act_cower_2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_tai_chi,
		.posture_name = STRING_ID(global, act_tai_chi),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_pee_1,
		.posture_name = STRING_ID(global, act_pee_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 39,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_doze_sit_1,
		.posture_name = STRING_ID(global, act_doze_sit_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_doze_stand_1,
		.posture_name = STRING_ID(global, act_doze_stand_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_eat_1,
		.posture_name = STRING_ID(global, act_eat_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cheering_1,
		.posture_name = STRING_ID(global, act_cheering_1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cheering_2,
		.posture_name = STRING_ID(global, act_cheering_2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cheering_3,
		.posture_name = STRING_ID(global, act_cheering_3),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cheering_4,
		.posture_name = STRING_ID(global, act_cheering_4),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_cheering_5,
		.posture_name = STRING_ID(global, act_cheering_5),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_injured_ankle,
		.posture_name = STRING_ID(global, act_injured_ankle),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 37,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_injured_sit_wall,
		.posture_name = STRING_ID(global, act_injured_sit_wall),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 37,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_injured_shoulder,
		.posture_name = STRING_ID(global, act_injured_shoulder),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 37,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_injured_gut,
		.posture_name = STRING_ID(global, act_injured_gut),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 37,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser,
		.posture_name = STRING_ID(global, act_laser),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser1,
		.posture_name = STRING_ID(global, act_laser1),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser2,
		.posture_name = STRING_ID(global, act_laser2),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser3,
		.posture_name = STRING_ID(global, act_laser3),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser4,
		.posture_name = STRING_ID(global, act_laser4),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_laser5,
		.posture_name = STRING_ID(global, act_laser5),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = false,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_medic,
		.posture_name = STRING_ID(global, act_medic),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_work,
		.posture_name = STRING_ID(global, act_work),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_work_stand,
		.posture_name = STRING_ID(global, act_work_stand),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
	{
		.posture = _ai_posture_act_captured_sit_wall,
		.posture_name = STRING_ID(global, act_captured_sit_wall),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 38,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_captured_cuffed_wall,
		.posture_name = STRING_ID(global, act_captured_cuffed_wall),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 38,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_captured_kneel,
		.posture_name = STRING_ID(global, act_captured_kneel),
		.category = _posture_category_none,
		.involuntary_vocalization_index = 38,
		.use_posture_physics = true,
		.disallow_targeting = true,
		.debilitated = true
	},
	{
		.posture = _ai_posture_act_bugger_lunch,
		.posture_name = STRING_ID(global, act_bugger_lunch),
		.category = _posture_category_none,
		.involuntary_vocalization_index = NONE,
		.use_posture_physics = true,
		.disallow_targeting = false,
		.debilitated = false
	},
};*/

