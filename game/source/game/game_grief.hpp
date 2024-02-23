#pragma once

struct s_game_grief_globals
{
	unsigned long betrayal_time;
	unsigned long ejection_time;
	unsigned long update_time;
	unsigned long reprieve_time;
	bool ui_active[4];
};
static_assert(sizeof(s_game_grief_globals) == 0x14);

extern s_game_grief_globals& game_grief_globals;

enum e_controller_index;

extern void __cdecl game_grief_record_reprieve();
extern bool __cdecl game_grief_can_eject(long player_index);
extern void __cdecl game_grief_dispose();
extern void __cdecl game_grief_dispose_from_old_map();
extern bool __cdecl game_grief_get_ui_active_for_local_user(e_controller_index controller_index);
extern void __cdecl game_grief_initialize();
extern void __cdecl game_grief_initialize_for_new_map();
extern void __cdecl game_grief_record_betrayal(long player_index);
extern void __cdecl game_grief_record_ejection(long player_index);
extern void __cdecl sub_6790A0(e_controller_index controller_index, bool a2);
extern void __cdecl game_grief_update();

