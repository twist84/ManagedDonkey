#include "ai/ai_dialogue.hpp"

#include "ai/actors.hpp"
#include "render/render_debug.hpp"

//.text:01437180 ; 
//.text:014371A0 ; 
//.text:014371C0 ; public: __cdecl c_dialogue_event::c_dialogue_event(int16, int32, int32, int32, int16, ai_information_data*)
//.text:01437400 ; 
//.text:01437410 ; 
//.text:01437420 ; 
//.text:01437430 ; 
//.text:01437440 ; 
//.text:01437460 ; void __cdecl __tls_set_g_team_dialogue_state_allocator(void*)
//.text:01437480 ; void __cdecl __tls_set_g_vocalization_record_allocator(void*)
//.text:014374B0 ; bool __cdecl actor_can_vocalize(int32, const c_dialogue_event*, const vocalization*)
//.text:01437600 ; bool __cdecl actor_grunt(int32, int16)
//.text:01437640 ; bool __cdecl actor_scream(int32, int16)
//.text:01437690 ; bool __cdecl actor_should_play_dialogue_animation(int32)
//.text:014376F0 ; bool __cdecl actor_vocalize(int32, int16, int16)

void __cdecl ai_dialogue_dispose_from_old_map()
{
	INVOKE(0x01437740, ai_dialogue_dispose_from_old_map);
}

//.text:01437760 ; int16 __cdecl ai_dialogue_event(c_dialogue_option*, int16)
//.text:01437810 ; bool __cdecl ai_dialogue_event(c_dialogue_event*)
//.text:01437870 ; void __cdecl ai_dialogue_event_aggregate_options(c_dialogue_event*, c_dialogue_option*, int16*, int16)
//.text:014378F0 ; bool __cdecl ai_dialogue_event_evaluate_pattern(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, c_dialogue_option*, int16, int16*)
//.text:01437FD0 ; bool __cdecl ai_dialogue_event_internal(ai_dialogue_definitions*, c_dialogue_event*, c_dialogue_option*, int16, int16*)
//.text:01438090 ; void __cdecl ai_dialogue_finished(int32, int16, int16, int32, ai_information_packet*)
//.text:014380B0 ; int16 __cdecl ai_dialogue_get_involuntary_vocalization_index(int16)
//.text:01438120 ; ai_dialogue_definitions* __cdecl ai_dialogue_globals_get()
//.text:01438170 ; void __cdecl ai_dialogue_handle_ai_reset()

void __cdecl ai_dialogue_initialize()
{
	INVOKE(0x01438200, ai_dialogue_initialize);
}

void __cdecl ai_dialogue_initialize_for_new_map()
{
	INVOKE(0x01438270, ai_dialogue_initialize_for_new_map);
}

//.text:014382D0 ; void __cdecl ai_dialogue_notify(int32, int16, int16, int32, ai_information_packet*)
//.text:014385C0 ; void __cdecl ai_dialogue_started(int32, unit_speech_item*)
//.text:014389D0 ; bool __cdecl ai_dialogue_test(c_dialogue_event*)

void __cdecl ai_dialogue_update()
{
	INVOKE(0x01438AB0, ai_dialogue_update);
}

//.text:01438AC0 ; 
//.text:01438B00 ; 
//.text:01438B40 ; 
//.text:01438B60 ; 
//.text:01438B90 ; 
//.text:01438BC0 ; bool __cdecl dialogue_category_survives_suppression(int16)
//.text:01438C30 ; int16 __cdecl dialogue_choose_option(c_dialogue_option*, int16)
//.text:01438D40 ; void __cdecl dialogue_emotion(int32, int16, int16)
//.text:01438DA0 ; int32 __cdecl dialogue_find_speaker(const ai_dialogue_definitions*, c_dialogue_event*, const vocalization_pattern*, bool, bool, bool, int32*, int16*, real32*, real32*)
//.text:01439060 ; int32 __cdecl dialogue_find_speaker_clump(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, int32*, int16*, real32*, real32*)
//.text:01439190 ; int32 __cdecl dialogue_find_speaker_joint(const ai_dialogue_definitions*, c_dialogue_event*, const vocalization_pattern*, bool, int32*, int16*, real32*, real32*)
//.text:014393D0 ; int32 __cdecl dialogue_find_speaker_vehicle(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, bool, int32*, int16*, real32*, real32*)
//.text:014395B0 ; int32 __cdecl dialogue_get_team_vocalization_delay(const ai_dialogue_definitions*, team_dialogue_state*, int16)

real64 __cdecl dialogue_player_weighting(int32 unit_index)
{
	return INVOKE(0x014395E0, dialogue_player_weighting, unit_index);
}

//.text:01439A00 ; bool __cdecl dialogue_reply_event(int32, int32, int32, c_dialogue_option*, int32*)
//.text:01439D20 ; bool __cdecl dialogue_speak(ai_dialogue_definitions*, int32, c_dialogue_option*, int32*)
//.text:01439E70 ; int32 __cdecl dialogue_task_find_speaker(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, int32*, int16*, real32*, real32*)

int16 __cdecl dialogue_team_get(int16 team)
{
	return INVOKE(0x01439FE0, dialogue_team_get, team);
}

//.text:0143A010 ; int16 __cdecl dialogue_unit_can_vocalize(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, int32, int32*, real32*)
//.text:0143AC00 ; 
//.text:0143AC10 ; 
//.text:0143AC40 ; 
//.text:0143AC70 ; 
//.text:0143AC80 ; 
//.text:0143AC90 ; 
//.text:0143ACB0 ; bool __cdecl pattern_match_ai_type_name(int32, int32, real32*)
//.text:0143AE30 ; bool __cdecl pattern_match_object_type(int16, int32, real32*)
//.text:0143B7D0 ; bool __cdecl pattern_match_spatial(int32, int32, int16)
//.text:0143BB30 ; 
//.text:0143BB40 ; 
//.text:0143BB80 ; 
//.text:0143BBC0 ; 
//.text:0143BC00 ; team_dialogue_state* __cdecl team_dialogue_state_get(int16)
//.text:0143BC50 ; 
//.text:0143BC70 ; bool __cdecl vehicle_is_turret(int32)
//.text:0143BC90 ; int16 __cdecl vocalization_get_ultimate_parent(ai_dialogue_definitions*, int16)
//.text:0143BCE0 ; void __cdecl vocalization_pattern_initialize(vocalization_pattern*, int16, int16, int32)
//.text:0143BD30 ; 

void ai_dialogue_render_player_weights()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		const real_argb_color* color = dialogue_team_get(actor->meta.team_index) == 1 ? global_real_argb_red : global_real_argb_green;

		char string[50]{};
		csnzprintf(string, sizeof(string), "%.2f", dialogue_player_weighting(actor->meta.unit_index));
		render_debug_string_at_point(&actor->input.position.head_position, string, color);
	}
}

