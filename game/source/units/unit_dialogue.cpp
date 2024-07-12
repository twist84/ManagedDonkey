#include "units/unit_dialogue.hpp"

#include "cseries/cseries.hpp"

//.text:00BA7070 ; bool __cdecl unit_can_vocalize(long, short, long*)
//.text:00BA7100 ; short __cdecl unit_dialogue_get_speech_duration(short, long, s_game_sound_deterministic_permutation const*)
//.text:00BA7160 ; void __cdecl unit_dialogue_handle_object_deleted(long, long)
//.text:00BA71C0 ; void __cdecl unit_dialogue_handle_unit_death(long)
//.text:00BA7220 ; void __cdecl unit_dialogue_interrupt(long)
//.text:00BA7270 ; void __cdecl unit_dialogue_set(long, long, long)
//.text:00BA72B0 ; void __cdecl unit_dialogue_setup(long, long, long)

void __cdecl unit_dialogue_update(long unit_index)
{
	INVOKE(0x00BA7330, unit_dialogue_update, unit_index);
}

//.text:00BA7560 ; long __cdecl unit_get_dialogue(long)
//.text:00BA75A0 ; long __cdecl unit_get_dialogue_variant(long)
//.text:00BA75F0 ; bool __cdecl unit_grunt(long, short)
//.text:00BA7610 ; bool __cdecl unit_grunt(long, short, short, short)
//.text:00BA7630 ; bool __cdecl unit_grunt(long, short, short, short, s_game_sound_deterministic_permutation*)
//.text:00BA76C0 ; void __cdecl unit_handle_animation_dialogue_event(long, s_animation_event_data const*)
//.text:00BA7870 ; bool __cdecl unit_is_speaking(long)
//.text:00BA78B0 ; bool __cdecl unit_make_damage_sound(long, long, bool, bool, real, real)
//.text:00BA7CD0 ; void __cdecl unit_notify_impulse_sound(long, long, long)
//.text:00BA7DA0 ; bool __cdecl unit_scream(long, short, short, short, ai_information_packet const*)
//.text:00BA7DC0 ; bool __cdecl unit_scream(long, short, short, short, ai_information_packet const*, s_game_sound_deterministic_permutation*)
//.text:00BA7F10 ; bool __cdecl unit_scream(long, short, short, ai_information_packet const*)
//.text:00BA7F30 ; void __cdecl unit_speak(long, unit_speech_item*)
//.text:00BA8040 ; bool __cdecl unit_test_speech(long, short, short, long*, long*)

