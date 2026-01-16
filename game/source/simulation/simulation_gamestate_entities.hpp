#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_gamestate_entity :
	s_datum_header
{
	int32 simulation_entity_index;
	int32 object_index;
	bool marked_for_deletion;
};
COMPILE_ASSERT(sizeof(s_simulation_gamestate_entity) == 0x10);

class c_bitstream;

extern void __cdecl simulation_gamestate_entities_dispose();
extern void __cdecl simulation_gamestate_entities_dispose_from_old_map();
extern void __cdecl simulation_gamestate_entities_initialize();
extern void __cdecl simulation_gamestate_entities_initialize_for_new_map();
extern void __cdecl simulation_object_glue_delete(int32 object_index);
extern int32 __cdecl simulation_object_glue_get_simulation_entity_index(int32 object_index);
extern void __cdecl simulation_gamestate_entity_set_object_index(int32 simulation_object_glue_index, int32 object_index);
extern void __cdecl simulation_object_glue_index_decode(c_bitstream* bitstream, int32* gamestate_index_out);
extern void __cdecl simulation_object_glue_index_encode(c_bitstream* bitstream, int32 gamestate_index);
extern bool __cdecl simulation_gamestate_index_valid(int32 gamestate_index);
extern int32 __cdecl simulation_entity_create(e_simulation_entity_type entity_type, int32 object_index, int32 simulation_object_glue_index);
extern void __cdecl simulation_entity_delete(int32 simulation_entity_index, int32 object_index, int32 simulation_object_glue_index);
extern void __cdecl simulation_entity_force_update(int32 simulation_entity_index, int32 object_index, c_flags<int32, uns64, 64>& flags);
extern void __cdecl simulation_entity_update(int32 simulation_entity_index, int32 object_index, c_flags<int32, uns64, 64>& flags);

