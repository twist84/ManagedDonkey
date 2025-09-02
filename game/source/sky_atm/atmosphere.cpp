#include "sky_atm/atmosphere.hpp"

REFERENCE_DECLARE(0x018BE9DC, int32, c_atmosphere_fog_interface::m_last_custom_index);
REFERENCE_DECLARE(0x018BE9E0, bool, c_atmosphere_fog_interface::m_use_local_pvs);
REFERENCE_DECLARE(0x02497E08, s_weighted_atmosphere_parameters, c_atmosphere_fog_interface::m_default_parameters);

void __cdecl c_atmosphere_fog_interface::dispose_from_old_map()
{
	INVOKE(0x00530170, c_atmosphere_fog_interface::dispose_from_old_map);
}

void __cdecl c_atmosphere_fog_interface::dispose()
{
	INVOKE(0x00530180, c_atmosphere_fog_interface::dispose);
}

void __cdecl c_atmosphere_fog_interface::dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00530190, c_atmosphere_fog_interface::dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:00671D30 ; 
//.text:00671D50 ; 
//.text:00671D60 ; 
//.text:00671D70 ; 
//.text:00671D90 ; private: static void __cdecl c_atmosphere_fog_interface::accumulate_atmosphere_settings(const c_atmosphere_setting*, s_weighted_atmosphere_parameters*, real32)
//.text:00672020 ; 
//.text:00672060 ; 

void __cdecl c_atmosphere_fog_interface::change_pvs(const s_game_cluster_bit_vectors* old_cluster_activation, const s_game_cluster_bit_vectors* new_cluster_activation, bool local)
{
	INVOKE(0x00672230, c_atmosphere_fog_interface::change_pvs, old_cluster_activation, new_cluster_activation, local);
}

void __cdecl c_atmosphere_fog_interface::compute_cluster_weights(s_cluster_reference cluster_reference, const real_point3d* point)
{
	INVOKE(0x006724B0, c_atmosphere_fog_interface::compute_cluster_weights, cluster_reference, point);
}

//.text:00672870 ; public: static void __cdecl c_atmosphere_fog_interface::compute_scattering(const s_weighted_atmosphere_parameters*, const real_point3d&, const real_point3d&, real32, real_linear_rgb_color*, real_linear_rgb_color*)
//.text:00672DD0 ; 
//.text:00672E40 ; 
//.text:00672EF0 ; 
//.text:00672FC0 ; 
//.text:00673070 ; 
//.text:006730D0 ; 
//.text:006731A0 ; 
//.text:00673210 ; 
//.text:00673240 ; public: static void __cdecl c_atmosphere_fog_interface::disable_atmosphere()
//.text:006732A0 ; 
//.text:006732D0 ; 
//.text:006733A0 ; private: static c_atmosphere_setting* __cdecl c_atmosphere_fog_interface::get_atmosphere_setting(s_cluster_reference, const char**)
//.text:00673480 ; public: static const c_sky_atm_parameters* __cdecl c_atmosphere_fog_interface::get_global_atmosphere_parameters()
//.text:006734C5 ; 
//.text:006734D0 ; 
//.text:00673600 ; private: static void __cdecl c_atmosphere_fog_interface::get_sun_parameters(const c_atmosphere_setting*, real_linear_rgb_color*, real_vector3d*)
//.text:00673780 ; 

void __cdecl c_atmosphere_fog_interface::initialize()
{
	INVOKE(0x00673820, c_atmosphere_fog_interface::initialize);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_map()
{
	INVOKE(0x00673880, c_atmosphere_fog_interface::initialize_for_new_map);
}

void __cdecl c_atmosphere_fog_interface::initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x006738F0, c_atmosphere_fog_interface::initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:00673960 ; 

void __cdecl c_atmosphere_fog_interface::populate_atmosphere_parameters(s_cluster_reference cluster_reference, s_weighted_atmosphere_parameters* parameters)
{
	INVOKE(0x006739A0, c_atmosphere_fog_interface::populate_atmosphere_parameters, cluster_reference, parameters);
}

//.text:00673A30 ; 
//.text:00674060 ; 
//.text:00674080 ; 
//.text:006740C0 ; public: static void __cdecl c_atmosphere_fog_interface::set_atmosphere_constants_by_index(int32)
//.text:00674170 ; public: static void __cdecl c_atmosphere_fog_interface::set_constant(const s_weighted_atmosphere_parameters*, bool, bool)

void __cdecl c_atmosphere_fog_interface::invalidate_atmosphere_constants()
{
	INVOKE(0x00A397D0, invalidate_atmosphere_constants);
}

void __cdecl c_atmosphere_fog_interface::restore_atmosphere_constants()
{
	INVOKE(0x00A3B5A0, c_atmosphere_fog_interface::restore_atmosphere_constants);
}

void __cdecl c_atmosphere_fog_interface::set_default_atmosphere_constants(s_weighted_atmosphere_parameters* parameters)
{
	INVOKE(0x00A3B720, c_atmosphere_fog_interface::set_default_atmosphere_constants, parameters);
}

