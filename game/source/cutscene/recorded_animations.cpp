#include "cutscene/recorded_animations.hpp"

#include "cseries/cseries.hpp"

bool debug_recording = false;

bool __cdecl recorded_animation_controlling_unit(long object_index)
{
	return INVOKE(0x00681AF0, recorded_animation_controlling_unit, object_index);
}

//.text:00681B70 ; long __cdecl recorded_animation_get_time_left_for_scripting(long)
//.text:00681C10 ; 
//.text:00681C90 ; bool __cdecl recorded_animation_play(long, long)
//.text:00681CB0 ; bool __cdecl recorded_animation_play_and_delete(long, long)
//.text:00681CD0 ; bool __cdecl recorded_animation_play_and_hover(long, long)
//.text:00681CF0 ; bool __cdecl recorded_animation_play_internal(long, long, word)
//.text:00681E50 ; 
//.text:00681ED0 ; void __cdecl recorded_animations_clear_debug_storage(long)
//.text:00681EE0 ; void __cdecl recorded_animations_dispose()
//.text:00681EF0 ; void __cdecl recorded_animations_dispose_from_old_map()
//.text:00681F10 ; void __cdecl recorded_animations_initialize()
//.text:00681F50 ; void __cdecl recorded_animations_initialize_for_new_map()

void __cdecl recorded_animations_update()
{
	INVOKE(0x00681F70, recorded_animations_update);
}

void render_debug_recording()
{
	if (debug_recording)
	{
		// #TODO: implement
	}
}

