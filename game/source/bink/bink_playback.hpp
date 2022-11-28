#pragma once

#include "cseries/cseries.hpp"

struct s_game_non_bsp_zone_set;
enum e_map_memory_configuration;

extern void __cdecl bink_notify_load_masking_complete();
extern bool __cdecl bink_playback_active();
extern void __cdecl bink_playback_check_for_terminate();
extern void __cdecl bink_playback_grab_memory();
extern bool __cdecl bink_playback_in_progress();
extern void __cdecl bink_playback_memory_dispose();
extern void __cdecl bink_playback_memory_initialize(e_map_memory_configuration configuration);
extern void __cdecl bink_playback_render();
extern bool __cdecl bink_playback_should_be_playing_deterministic_bink();
extern void __cdecl bink_playback_start_or_queue(char const* bink_full_path, long tag_index, dword_flags flags);
extern void __cdecl bink_playback_stop();
extern void __cdecl bink_playback_stop_due_to_input_device_change();
extern bool __cdecl bink_playback_ui_rendering_inhibited();
extern void __cdecl bink_playback_update();
extern bool __cdecl bink_playback_using_io_during_map_load_masking();
