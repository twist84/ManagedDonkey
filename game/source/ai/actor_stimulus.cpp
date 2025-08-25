#include "ai/actor_stimulus.hpp"

#include "ai/actors.hpp"
#include "ai/character_definitions.hpp"
#include "ai/props.hpp"
#include "game/cheats.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

HOOK_DECLARE(0x01457200, actor_stimulus_prop_acknowledged);

const char* const g_stimulus_names[k_stimulus_count]
{
	"surprise",
	"surprise_combat_start",
	"friendly_body",
	"shield_deplieted",
	"leader_dead",
	"peer_dead",
	"last_man",
	"stuck_with_grenade",
	"super_detonation",
	"null",
	"destructible_obstacle",
	"cover_friend",
	"activity",
	"changed target",
	"cover failed",
	"broken",
	"leader abandoned",
	"just flanked",
	"enemy vehicle",
	"turtle",
	"flood disperse"
};

//.text:01456700 ; void __cdecl actor_stimuli_update(int32)

void __cdecl actor_stimulus_acknowledged_danger_zone(int32 actor_index, int32 pref_index)
{
	INVOKE(0x014567B0, actor_stimulus_acknowledged_danger_zone, actor_index, pref_index);
}

//.text:01456A40 ; void __cdecl actor_stimulus_allegiance_update(int32, int32)
//.text:01456B20 ; void __cdecl actor_stimulus_bumped(int32, int32)
//.text:01456B30 ; bool __cdecl actor_stimulus_clear(int32, e_behavior_stimulus)
//.text:01456BB0 ; void __cdecl actor_stimulus_damage(int32, int16, int32, int32, real32, const real_vector3d*)
//.text:01456D30 ; void __cdecl actor_stimulus_environmental_noise(int32, int32, const real_point3d*, int16)
//.text:01456DC0 ; void __cdecl actor_stimulus_friend_body_sighted(int32, int32)
//.text:01456FA0 ; void __cdecl actor_stimulus_heard_shooting(int32, int32, int32)

void __cdecl actor_stimulus_prop_acknowledged(int32 actor_index, int32 prop_index, bool first_acknowledgement)
{
	//INVOKE(0x01457200, actor_stimulus_prop_acknowledged, actor_index, prop_index, first_acknowledgement);

	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	prop_ref_datum* pref = DATUM_GET(prop_ref_data, prop_ref_datum, prop_index);

	if (first_acknowledgement && pref->type == 3)
	{
		actor_stimulus_acknowledged_danger_zone(actor_index, prop_index);
	}
	else if (16 * pref->line_of_sight > 0)
	{
		actor_stimulus_prop_sighted(actor_index, prop_index, first_acknowledgement);
	}

	if (cheat.medusa)
	{
		prop_state* state = prop_state_get(pref);
		prop_datum* prop = DATUM_GET(prop_data, prop_datum, pref->prop_index);
		if (prop->player && prop->enemy && !state->dead && actor->meta.type != _actor_mounted_weapon)
			actor_kill(actor_index, false, true);
	}

	if (pref->type == 1 && first_acknowledgement && (pref->flags & 1) != 0)
	{
		if (character_perception_properties* actor_perception_properties = actor_perception_properties_get(actor_index))
		{
			if (actor_perception_properties->first_acknowledgement_surprise_distance > pref->distance || actor->state.combat_status == 0)
			{
				prop_state* state = prop_state_get(pref);
				real_vector3d surprise_vector{};
				vector_from_points3d(&actor->input.position.body_position, &state->body_position, &surprise_vector);
				actor_stimulus_surprise(actor_index, 1, prop_index, &surprise_vector);
			}
		}
	}

}

//.text:01457310 ; void __cdecl actor_stimulus_prop_fleeing(int32, int32)
//.text:01457320 ; void __cdecl actor_stimulus_prop_just_killed(int32, int32, int32)

void __cdecl actor_stimulus_prop_sighted(int32 actor_index, int32 pref_index, bool initial)
{
	INVOKE(0x01457480, actor_stimulus_prop_sighted, actor_index, pref_index, initial);
}

//.text:014575E0 ; void __cdecl actor_stimulus_prop_sound(int32, int32, bool)

void __cdecl actor_stimulus_surprise(int32 actor_index, int16 surprise_level, int32 prop_index, const real_vector3d* surprise_vector)
{
	INVOKE(0x01457620, actor_stimulus_surprise, actor_index, surprise_level, prop_index, surprise_vector);
}

//.text:014577A0 ; void __cdecl actor_stimulus_switched_danger_zone(int32, int32)
//.text:01457900 ; bool __cdecl actor_stimulus_test(int32, e_behavior_stimulus)
//.text:01457950 ; void __cdecl actor_stimulus_tracking_or_locking(int32, int32, int16)
//.text:01457BB0 ; void __cdecl actor_stimulus_vehicle_eviction(int32, int32)
//.text:01457CB0 ; void __cdecl actor_stimulus_weapon_detonation(int32, int32, const real_point3d*, int16)
//.text:01457CE0 ; void __cdecl actor_stimulus_weapon_impact(int32, int32, const real_point3d*, int16)
//.text:01457CF0 ; 
//.text:01457D00 ; void __cdecl stimulus_clear(actor_datum*, behavior_stimulus*)

// $TODO `actor_debug_info`, `actor_debug_array`, `actor_debug_drawstack`

void __cdecl stimuli_debug()
{
	//actor_iterator iterator{};
	//actor_iterator_new(&iterator, true);
	//while (actor_datum* actor = actor_iterator_next(&iterator))
	//{
	//	actor_debug_info* actor_debug = &actor_debug_array[DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.index)];
	//
	//	real_point3d position{};
	//	point_from_line3d(&actor->input.position.head, global_up3d, 0.1f, &position);
	//	for (int32 stimuli_index = 0; stimuli_index < k_stimulus_count; stimuli_index++)
	//	{
	//		if (TEST_BIT(actor_debug->actor_stimulus_flags, stimuli_index))
	//		{
	//			render_debug_string_at_point(ai_debug_drawstack(), g_stimulus_names[stimuli_index], global_real_argb_yellow);
	//		}
	//	}
	//}
}

//.text:01457D30 ; bool __cdecl stimulus_register(int32, e_behavior_stimulus, int16, int16)
