#include "ai/event_handling.hpp"

//.text:01498720 ; void __cdecl actor_handle_bump(int32, int32)
//.text:01498860 ; void __cdecl actor_handle_sound(int32, int32, int32, int32, const ai_sound_data*)
//.text:01498940 ; bool __cdecl actor_handle_speech(int32, int32, const ai_information_data*)
//.text:01498A20 ; 
//.text:01498A30 ; void __cdecl ai_handle_grenade_release(int32, int32)
//.text:01498B20 ; bool __cdecl ai_handle_phase_start(int32, real_point3d*)
//.text:01498B70 ; void __cdecl ai_information_packet_new(ai_information_packet*)
//.text:01498BB0 ; s_game_cluster_bit_vectors* __cdecl calculate_pas(s_cluster_reference)
//.text:01498D20 ; void __cdecl clump_handle_sound(int32, int32, int32, bool, s_game_cluster_bit_vectors*, ai_sound_data*)

void __cdecl event_handling_initialize()
{
	INVOKE(0x01498F20, event_handling_initialize);
}

//.text:01498F30 ; void __cdecl handle_intuition(int32, int32, int32, int16)
//.text:01498F80 ; void __cdecl handle_sound(int32, int32, int16, bool, ai_sound_data*)
//.text:01499260 ; 
//.text:01499270 ; 
//.text:01499280 ; bool __cdecl test_actor_audibility(int32, int32, s_game_cluster_bit_vectors*, real_point3d*, s_location*, ai_sound_data*)

