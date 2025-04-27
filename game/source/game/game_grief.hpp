#pragma once

struct s_game_grief_user
{
	long time_of_last_betrayal_ms;
	long time_of_last_ejection_ms;
	long last_update_time_ms;
};
static_assert(sizeof(s_game_grief_user) == 0xC);

struct s_game_grief_globals
{
	s_game_grief_user grief_user;
	long time_of_last_forgiveness_ms;
	bool betrayal_boot_ui_active_for_user[4];
};
static_assert(sizeof(s_game_grief_globals) == 0x14);

extern s_game_grief_globals& game_grief_globals;

extern void __cdecl game_grief_record_reprieve();
extern bool __cdecl game_grief_can_eject(long player_index);
extern void __cdecl game_grief_dispose();
extern void __cdecl game_grief_dispose_from_old_map();
extern bool __cdecl game_grief_get_ui_active_for_local_user(e_controller_index controller_index);
extern void __cdecl game_grief_initialize();
extern void __cdecl game_grief_initialize_for_new_map();
extern void __cdecl game_grief_record_betrayal(long player_index);
extern void __cdecl game_grief_record_ejection(long player_index);
extern void __cdecl game_grief_set_ui_active_for_local_user(e_controller_index controller_index, bool ui_active_for_local_user);
extern void __cdecl game_grief_update();

