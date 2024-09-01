#pragma once

#include "cseries/cseries.hpp"

struct s_game_cluster_bit_vectors;
struct c_atmosphere_fog_interface
{
public:
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
	static void __cdecl change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3);
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(dword new_structure_bsp_mask);

	byte __data[0x14];
};
static_assert(sizeof(c_atmosphere_fog_interface) == 0x14);

