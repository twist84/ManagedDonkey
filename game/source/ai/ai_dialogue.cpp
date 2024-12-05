#include "ai/ai_dialogue.hpp"

#include "ai/actors.hpp"
#include "render/render_debug.hpp"

//.text:01437180 ; 
//.text:014371A0 ; 
//.text:014371C0 ; public: __cdecl c_dialogue_event::c_dialogue_event(short, long, long, long, short, ai_information_data*)
//.text:01437400 ; 
//.text:01437410 ; 
//.text:01437420 ; 
//.text:01437430 ; 
//.text:01437440 ; 
//.text:01437460 ; void __cdecl __tls_set_g_team_dialogue_state_allocator(void*)
//.text:01437480 ; void __cdecl __tls_set_g_vocalization_record_allocator(void*)
//.text:014374B0 ; bool __cdecl actor_can_vocalize(long, c_dialogue_event const*, vocalization const*)
//.text:01437600 ; bool __cdecl actor_grunt(long, short)
//.text:01437640 ; bool __cdecl actor_scream(long, short)
//.text:01437690 ; bool __cdecl actor_should_play_dialogue_animation(long)
//.text:014376F0 ; bool __cdecl actor_vocalize(long, short, short)
//.text:01437740 ; void __cdecl ai_dialogue_dispose_from_old_map()
//.text:01437760 ; short __cdecl ai_dialogue_event(c_dialogue_option*, short)
//.text:01437810 ; bool __cdecl ai_dialogue_event(c_dialogue_event*)
//.text:01437870 ; void __cdecl ai_dialogue_event_aggregate_options(c_dialogue_event*, c_dialogue_option*, short*, short)
//.text:014378F0 ; bool __cdecl ai_dialogue_event_evaluate_pattern(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, c_dialogue_option*, short, short*)
//.text:01437FD0 ; bool __cdecl ai_dialogue_event_internal(ai_dialogue_definitions*, c_dialogue_event*, c_dialogue_option*, short, short*)
//.text:01438090 ; void __cdecl ai_dialogue_finished(long, short, short, long, ai_information_packet*)
//.text:014380B0 ; short __cdecl ai_dialogue_get_involuntary_vocalization_index(short)
//.text:01438120 ; ai_dialogue_definitions* __cdecl ai_dialogue_globals_get()
//.text:01438170 ; void __cdecl ai_dialogue_handle_ai_reset()
//.text:01438200 ; void __cdecl ai_dialogue_initialize()
//.text:01438270 ; void __cdecl ai_dialogue_initialize_for_new_map()
//.text:014382D0 ; void __cdecl ai_dialogue_notify(long, short, short, long, ai_information_packet*)
//.text:014385C0 ; void __cdecl ai_dialogue_started(long, unit_speech_item*)
//.text:014389D0 ; bool __cdecl ai_dialogue_test(c_dialogue_event*)
//.text:01438AB0 ; void __cdecl ai_dialogue_update()
//.text:01438AC0 ; 
//.text:01438B00 ; 
//.text:01438B40 ; 
//.text:01438B60 ; 
//.text:01438B90 ; 
//.text:01438BC0 ; bool __cdecl dialogue_category_survives_suppression(short)
//.text:01438C30 ; short __cdecl dialogue_choose_option(c_dialogue_option*, short)
//.text:01438D40 ; void __cdecl dialogue_emotion(long, short, short)
//.text:01438DA0 ; long __cdecl dialogue_find_speaker(ai_dialogue_definitions const*, c_dialogue_event*, vocalization_pattern const*, bool, bool, bool, long*, short*, real*, real*)
//.text:01439060 ; long __cdecl dialogue_find_speaker_clump(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, long*, short*, real*, real*)
//.text:01439190 ; long __cdecl dialogue_find_speaker_joint(ai_dialogue_definitions const*, c_dialogue_event*, vocalization_pattern const*, bool, long*, short*, real*, real*)
//.text:014393D0 ; long __cdecl dialogue_find_speaker_vehicle(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, bool, long*, short*, real*, real*)
//.text:014395B0 ; long __cdecl dialogue_get_team_vocalization_delay(ai_dialogue_definitions const*, team_dialogue_state*, short)

double __cdecl dialogue_player_weighting(long unit_index)
{
	return INVOKE(0x014395E0, dialogue_player_weighting, unit_index);
}

//.text:01439A00 ; bool __cdecl dialogue_reply_event(long, long, long, c_dialogue_option*, long*)
//.text:01439D20 ; bool __cdecl dialogue_speak(ai_dialogue_definitions*, long, c_dialogue_option*, long*)
//.text:01439E70 ; long __cdecl dialogue_task_find_speaker(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, long*, short*, real*, real*)

short __cdecl dialogue_team_get(short team)
{
	return INVOKE(0x01439FE0, dialogue_team_get, team);
}

//.text:0143A010 ; short __cdecl dialogue_unit_can_vocalize(ai_dialogue_definitions*, c_dialogue_event*, vocalization_pattern*, long, long*, real*)
//.text:0143AC00 ; 
//.text:0143AC10 ; 
//.text:0143AC40 ; 
//.text:0143AC70 ; 
//.text:0143AC80 ; 
//.text:0143AC90 ; 
//.text:0143ACB0 ; bool __cdecl pattern_match_ai_type_name(long, long, real*)
//.text:0143AE30 ; bool __cdecl pattern_match_object_type(short, long, real*)
//.text:0143B7D0 ; bool __cdecl pattern_match_spatial(long, long, short)
//.text:0143BB30 ; 
//.text:0143BB40 ; 
//.text:0143BB80 ; 
//.text:0143BBC0 ; 
//.text:0143BC00 ; team_dialogue_state* __cdecl team_dialogue_state_get(short)
//.text:0143BC50 ; 
//.text:0143BC70 ; bool __cdecl vehicle_is_turret(long)
//.text:0143BC90 ; short __cdecl vocalization_get_ultimate_parent(ai_dialogue_definitions*, short)
//.text:0143BCE0 ; void __cdecl vocalization_pattern_initialize(vocalization_pattern*, short, short, long)
//.text:0143BD30 ; 

void ai_dialogue_render_player_weights()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_argb_color const* color = dialogue_team_get(actor->meta.team_index) == 1 ? global_real_argb_red : global_real_argb_green;

		char string[50]{};
		csnzprintf(string, sizeof(string), "%.2f", dialogue_player_weighting(actor->meta.unit_index));
		render_debug_string_at_point(&actor->input.position.head_position, string, color);
	}
}

