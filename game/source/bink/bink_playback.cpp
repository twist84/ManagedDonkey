#include "bink/bink_playback.hpp"

#include "cseries/cseries.hpp"

//.text:00617940 ; void __cdecl bink_set_volume(real)
//.text:006179B0 ; void __cdecl bink_setup_rasterizer(long)

void __cdecl bink_notify_load_masking_complete()
{
	INVOKE(0x006168C0, bink_notify_load_masking_complete);
}

bool __cdecl bink_playback_active()
{
	return INVOKE(0x00616910, bink_playback_active);
}

void __cdecl bink_playback_check_for_terminate_no_lock()
{
	INVOKE(0x00616960, bink_playback_check_for_terminate_no_lock);
}

void __cdecl bink_playback_grab_memory()
{
	INVOKE(0x00616A30, bink_playback_grab_memory);
}

bool __cdecl bink_playback_in_progress()
{
	return INVOKE(0x00616AA0, bink_playback_in_progress);
}

void __cdecl bink_playback_memory_dispose()
{
	INVOKE(0x00616BA0, bink_playback_memory_dispose);
}

void __cdecl bink_playback_memory_initialize(e_map_memory_configuration configuration)
{
	INVOKE(0x00616C00, bink_playback_memory_initialize, configuration);
}

void __cdecl bink_playback_render()
{
	INVOKE(0x00616CC0, bink_playback_render);
}

bool __cdecl bink_playback_should_be_playing_deterministic_bink()
{
	return INVOKE(0x00616D50, bink_playback_should_be_playing_deterministic_bink);
}

void __cdecl bink_playback_start_or_queue(char const* bink_full_path, long tag_index, dword_flags flags)
{
	INVOKE(0x00616D60, bink_playback_start_or_queue, bink_full_path, tag_index, flags);
}

void __cdecl bink_playback_stop()
{
	INVOKE(0x00617040, bink_playback_stop);
}

void __cdecl bink_playback_stop_due_to_input_device_change()
{
	INVOKE(0x00617080, bink_playback_stop_due_to_input_device_change);
}

bool __cdecl bink_playback_ui_rendering_inhibited()
{
	return INVOKE(0x00617380, bink_playback_ui_rendering_inhibited);
}

void __cdecl bink_playback_update()
{
	INVOKE(0x006173A0, bink_playback_update);
}

bool __cdecl bink_playback_using_io_during_map_load_masking()
{
	return INVOKE(0x00617680, bink_playback_using_io_during_map_load_masking);
}

