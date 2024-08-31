#include "sky_atm/atmosphere.hpp"

void __cdecl c_atmosphere_fog_interface::dispose_from_old_map()
{
	INVOKE(0x00530170, dispose_from_old_map);
}

void __cdecl c_atmosphere_fog_interface::dispose()
{
	INVOKE(0x00530180, dispose);
}

void __cdecl c_atmosphere_fog_interface::dispose_from_old_structure_bsp(dword structure_bsp_mask)
{
	INVOKE(0x00530190, dispose_from_old_structure_bsp, structure_bsp_mask);
}

void __cdecl c_atmosphere_fog_interface::change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3)
{
	INVOKE(0x00672230, change_pvs, a1, a2, a3);
}

void __cdecl c_atmosphere_fog_interface::initialize()
{
	INVOKE(0x00673820, initialize);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_map()
{
	INVOKE(0x00673880, initialize_for_new_map);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_structure_bsp(dword structure_bsp_mask)
{
	INVOKE(0x006738F0, initialize_for_new_structure_bsp, structure_bsp_mask);
}

