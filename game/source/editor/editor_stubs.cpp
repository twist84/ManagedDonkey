#include "editor/editor_stubs.hpp"

void __cdecl editor_change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3)
{
	INVOKE(0x0042DF80, editor_change_pvs, a1, a2, a3);
}

void __cdecl editor_dispose()
{
	INVOKE(0x0042DF90, editor_dispose);
}

void __cdecl editor_dispose_from_old_map()
{
	INVOKE(0x0042DFA0, editor_dispose_from_old_map);
}

void __cdecl editor_dispose_from_old_structure_bsp(dword a1)
{
	INVOKE(0x0042DFB0, editor_dispose_from_old_structure_bsp, a1);
}

void __cdecl editor_initialize()
{
	INVOKE(0x0042E010, editor_initialize);
}

void __cdecl editor_initialize_for_new_map()
{
	INVOKE(0x0042E020, editor_initialize_for_new_map);
}

void __cdecl editor_initialize_for_new_structure_bsp(dword a1)
{
	INVOKE(0x0042E030, editor_initialize_for_new_structure_bsp, a1);
}

bool __cdecl editor_input_inhibited()
{
	return INVOKE(0x0042E040, editor_input_inhibited);
}

void __cdecl editor_orphan_object(e_object_type a1, long a2)
{
	INVOKE(0x0042E060, editor_orphan_object, a1, a2);
}

void __cdecl editor_register_new_object(e_object_type a1, long a2, long a3)
{
	INVOKE(0x0042E080, editor_register_new_object, a1, a2, a3);
}

void __cdecl editor_save_progress()
{
	INVOKE(0x0042E0C0, editor_save_progress);
}

bool __cdecl editor_should_render_transparent_geometry()
{
	return INVOKE(0x0042E0E0, editor_should_render_transparent_geometry);
}

bool __cdecl editor_switch_zone_set(long a1)
{
	return INVOKE(0x0042E110, editor_switch_zone_set, a1);
}

void __cdecl editor_update()
{
	INVOKE(0x0042E120, editor_update);
}

bool __cdecl game_in_editor()
{
	return INVOKE(0x0042E130, game_in_editor);
}
