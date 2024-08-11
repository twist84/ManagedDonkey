#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_simulation_gamestate_entity :
	s_datum_header
{
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(s_simulation_gamestate_entity) == 0x10);

enum e_simulation_entity_type;

extern void __cdecl simulation_gamestate_entities_dispose();
extern void __cdecl simulation_gamestate_entities_dispose_from_old_map();
extern void __cdecl simulation_gamestate_entities_initialize();
extern void __cdecl simulation_gamestate_entities_initialize_for_new_map();
extern void __cdecl simulation_object_glue_delete(long object_index);
extern long __cdecl simulation_object_glue_get_simulation_entity_index(long object_index);
extern void __cdecl simulation_gamestate_entity_set_object_index(long simulation_object_glue_index, long object_index);
extern long __cdecl simulation_entity_create(e_simulation_entity_type entity_type, long object_index, long simulation_object_glue_index);
extern void __cdecl simulation_entity_delete(long simulation_entity_index, long object_index, long simulation_object_glue_index);
extern void __cdecl simulation_entity_force_update(long simulation_entity_index, long object_index, c_flags<long, qword, 64>& flags);
extern void __cdecl simulation_entity_update(long simulation_entity_index, long object_index, c_flags<long, qword, 64>& flags);

