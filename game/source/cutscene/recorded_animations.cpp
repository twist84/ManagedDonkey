#include "cutscene/recorded_animations.hpp"

#include "cseries/cseries.hpp"

bool debug_recording = false;

bool __cdecl recorded_animation_controlling_unit(int32 object_index)
{
	return INVOKE(0x00681AF0, recorded_animation_controlling_unit, object_index);
}

//.text:00681B70 ; int32 __cdecl recorded_animation_get_time_left_for_scripting(int32)
//.text:00681C10 ; void __cdecl recorded_animation_kill(int32)
//.text:00681C90 ; bool __cdecl recorded_animation_play(int32, int32)
//.text:00681CB0 ; bool __cdecl recorded_animation_play_and_delete(int32, int32)
//.text:00681CD0 ; bool __cdecl recorded_animation_play_and_hover(int32, int32)
//.text:00681CF0 ; bool __cdecl recorded_animation_play_internal(int32, int32, uns16)
//.text:00681E50 ; void __cdecl recorded_animation_verify(const recorded_animation_definition*)
//.text:00681ED0 ; void __cdecl recorded_animations_clear_debug_storage(int32)

void __cdecl recorded_animations_dispose()
{
	INVOKE(0x00681EE0, recorded_animations_dispose);
}

void __cdecl recorded_animations_dispose_from_old_map()
{
	INVOKE(0x00681EF0, recorded_animations_dispose_from_old_map);
}

void __cdecl recorded_animations_initialize()
{
	INVOKE(0x00681F10, recorded_animations_initialize);
}

void __cdecl recorded_animations_initialize_for_new_map()
{
	INVOKE(0x00681F50, recorded_animations_initialize_for_new_map);
}

void __cdecl recorded_animations_update()
{
	INVOKE(0x00681F70, recorded_animations_update);
}

//.text:006820C0 ; 

void render_debug_recording()
{
	if (debug_recording)
	{
		// $IMPLEMENT
	}
}

