#include "editor/editor_stubs.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0042DF80, editor_change_pvs);
HOOK_DECLARE(0x0042DF90, editor_dispose);
HOOK_DECLARE(0x0042DFA0, editor_dispose_from_old_map);
HOOK_DECLARE(0x0042DFB0, editor_dispose_from_old_structure_bsp);
HOOK_DECLARE(0x0042E010, editor_initialize);
HOOK_DECLARE(0x0042E020, editor_initialize_for_new_map);
HOOK_DECLARE(0x0042E030, editor_initialize_for_new_structure_bsp);
HOOK_DECLARE(0x0042E040, editor_input_inhibited);
HOOK_DECLARE(0x0042E060, editor_orphan_object);
HOOK_DECLARE(0x0042E080, editor_register_new_object);
HOOK_DECLARE(0x0042E0C0, editor_save_progress);
HOOK_DECLARE(0x0042E0D0, editor_should_render_object);
HOOK_DECLARE(0x0042E0E0, editor_should_render_transparent_geometry);
HOOK_DECLARE(0x0042E110, editor_switch_zone_set);
HOOK_DECLARE(0x0042E120, editor_update);
HOOK_DECLARE(0x0042E130, game_in_editor);

void __cdecl editor_change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3)
{
	HOOK_INVOKE(, editor_change_pvs, a1, a2, a3);
}

void __cdecl editor_dispose()
{
	HOOK_INVOKE(, editor_dispose);
}

void __cdecl editor_dispose_from_old_map()
{
	HOOK_INVOKE(, editor_dispose_from_old_map);
}

void __cdecl editor_dispose_from_old_structure_bsp(dword a1)
{
	HOOK_INVOKE(, editor_dispose_from_old_structure_bsp, a1);
}

void __cdecl editor_initialize()
{
	HOOK_INVOKE(, editor_initialize);
}

void __cdecl editor_initialize_for_new_map()
{
	HOOK_INVOKE(, editor_initialize_for_new_map);
}

void __cdecl editor_initialize_for_new_structure_bsp(dword a1)
{
	HOOK_INVOKE(, editor_initialize_for_new_structure_bsp, a1);
}

bool __cdecl editor_input_inhibited()
{
	bool result = false;
	HOOK_INVOKE(result =, editor_input_inhibited);
	return result;
}

void __cdecl editor_orphan_object(e_object_type a1, long a2)
{
	HOOK_INVOKE(, editor_orphan_object, a1, a2);
}

void __cdecl editor_register_new_object(e_object_type a1, long a2, long a3)
{
	HOOK_INVOKE(, editor_register_new_object, a1, a2, a3);
}

void __cdecl editor_save_progress()
{
	HOOK_INVOKE(, editor_save_progress);
}

bool __cdecl editor_should_render_object(long object_index)
{
	bool result = false;
	HOOK_INVOKE(result =, editor_should_render_object, object_index);
	return result;
}

bool __cdecl editor_should_render_transparent_geometry()
{
	bool result = false;
	HOOK_INVOKE(result =, editor_should_render_transparent_geometry);
	return result;
}

bool __cdecl editor_switch_zone_set(long a1)
{
	bool result = false;
	HOOK_INVOKE(result =, editor_switch_zone_set, a1);
	return result;
}

void __cdecl editor_update()
{
	HOOK_INVOKE(, editor_update);
}

bool __cdecl game_in_editor()
{
	bool result = false;
	HOOK_INVOKE(result =, game_in_editor);
	return result;
}
