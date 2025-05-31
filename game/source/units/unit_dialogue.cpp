#include "units/unit_dialogue.hpp"

#include "cseries/cseries.hpp"

//.text:00BA7070 ; bool __cdecl unit_can_vocalize(int32, int16, int32*)
//.text:00BA7100 ; int16 __cdecl unit_dialogue_get_speech_duration(int16, int32, const s_game_sound_deterministic_permutation*)
//.text:00BA7160 ; void __cdecl unit_dialogue_handle_object_deleted(int32, int32)
//.text:00BA71C0 ; void __cdecl unit_dialogue_handle_unit_death(int32)
//.text:00BA7220 ; void __cdecl unit_dialogue_interrupt(int32)
//.text:00BA7270 ; void __cdecl unit_dialogue_set(int32, int32, int32)
//.text:00BA72B0 ; void __cdecl unit_dialogue_setup(int32, int32, int32)

void __cdecl unit_dialogue_update(int32 unit_index)
{
	INVOKE(0x00BA7330, unit_dialogue_update, unit_index);
}

//.text:00BA7560 ; int32 __cdecl unit_get_dialogue(int32)
//.text:00BA75A0 ; int32 __cdecl unit_get_dialogue_variant(int32)
//.text:00BA75F0 ; bool __cdecl unit_grunt(int32, int16)
//.text:00BA7610 ; bool __cdecl unit_grunt(int32, int16, int16, int16)
//.text:00BA7630 ; bool __cdecl unit_grunt(int32, int16, int16, int16, s_game_sound_deterministic_permutation*)
//.text:00BA76C0 ; void __cdecl unit_handle_animation_dialogue_event(int32, const s_animation_event_data*)
//.text:00BA7870 ; bool __cdecl unit_is_speaking(int32)
//.text:00BA78B0 ; bool __cdecl unit_make_damage_sound(int32, int32, bool, bool, real32, real32)
//.text:00BA7CD0 ; void __cdecl unit_notify_impulse_sound(int32, int32, int32)
//.text:00BA7DA0 ; bool __cdecl unit_scream(int32, int16, int16, int16, const ai_information_packet*)
//.text:00BA7DC0 ; bool __cdecl unit_scream(int32, int16, int16, int16, const ai_information_packet*, s_game_sound_deterministic_permutation*)
//.text:00BA7F10 ; bool __cdecl unit_scream(int32, int16, int16, const ai_information_packet*)
//.text:00BA7F30 ; void __cdecl unit_speak(int32, unit_speech_item*)
//.text:00BA8040 ; bool __cdecl unit_test_speech(int32, int16, int16, int32*, int32*)

