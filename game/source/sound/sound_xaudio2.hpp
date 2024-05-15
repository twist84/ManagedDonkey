#pragma once

extern void __cdecl platform_sound_source_queue_sound(long source_index, void const* sound);
extern void __cdecl platform_sound_dispose();
extern bool __cdecl platform_sound_initialize(void const* preferences);

