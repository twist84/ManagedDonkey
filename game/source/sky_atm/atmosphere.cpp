#include "sky_atm/atmosphere.hpp"

REFERENCE_DECLARE(0x018BE9DC, long, c_atmosphere_fog_interface::long_18BE9DC);
REFERENCE_DECLARE(0x02497E08, s_weighted_atmosphere_parameters, c_atmosphere_fog_interface::m_default_parameters);

void __cdecl c_atmosphere_fog_interface::dispose_from_old_map()
{
	INVOKE(0x00530170, c_atmosphere_fog_interface::dispose_from_old_map);
}

void __cdecl c_atmosphere_fog_interface::dispose()
{
	INVOKE(0x00530180, c_atmosphere_fog_interface::dispose);
}

void __cdecl c_atmosphere_fog_interface::dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x00530190, c_atmosphere_fog_interface::dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

void __cdecl c_atmosphere_fog_interface::change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3)
{
	INVOKE(0x00672230, c_atmosphere_fog_interface::change_pvs, a1, a2, a3);
}

void __cdecl c_atmosphere_fog_interface::compute_cluster_weights(s_cluster_reference cluster_reference, real_point3d const* point)
{
	INVOKE(0x006724B0, c_atmosphere_fog_interface::compute_cluster_weights, cluster_reference, point);
}

void __cdecl c_atmosphere_fog_interface::initialize()
{
	INVOKE(0x00673820, c_atmosphere_fog_interface::initialize);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_map()
{
	INVOKE(0x00673880, c_atmosphere_fog_interface::initialize_for_new_map);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x006738F0, c_atmosphere_fog_interface::initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

void __cdecl c_atmosphere_fog_interface::populate_atmosphere_parameters(s_cluster_reference cluster_reference, s_weighted_atmosphere_parameters* parameters)
{
	INVOKE(0x006739A0, c_atmosphere_fog_interface::populate_atmosphere_parameters, cluster_reference, parameters);
}

void __cdecl c_atmosphere_fog_interface::sub_A397D0()
{
	INVOKE(0x00A397D0, sub_A397D0);
}

void __cdecl c_atmosphere_fog_interface::restore_atmosphere_constants()
{
	INVOKE(0x00A3B5A0, c_atmosphere_fog_interface::restore_atmosphere_constants);
}

void __cdecl c_atmosphere_fog_interface::set_default_atmosphere_constants(s_weighted_atmosphere_parameters* parameters)
{
	INVOKE(0x00A3B720, c_atmosphere_fog_interface::set_default_atmosphere_constants, parameters);
}

