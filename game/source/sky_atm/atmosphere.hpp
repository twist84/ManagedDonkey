#pragma once

#include "cseries/cseries.hpp"

struct s_weighted_atmosphere_parameters
{
	bool is_scaled;
	real_rgb_color color;
	real_point3d point;
	real32 distance_bias;
	real32 max_fog_thickness;
	real_vector3d beta_m;
	real_vector3d beta_p;
	real_vector3d beta_m_theta_prefix;
	real_vector3d beta_p_theta_prefix;
	real32 sun_phase_function;
	real32 sea_level;
	real32 mie_height_scale;
	real32 rayleign_height_scale;
	real32 sheet_density;
	real32 full_intensity_height;
	real32 half_intensity_height;
	real_point3d wind_direction;
};
static_assert(sizeof(s_weighted_atmosphere_parameters) == 0x7C);

struct s_game_cluster_bit_vectors;
struct c_atmosphere_fog_interface
{
public:
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask);
	static void __cdecl change_pvs(s_game_cluster_bit_vectors const* old_cluster_activation, s_game_cluster_bit_vectors const* new_cluster_activation, bool local);
	static void __cdecl compute_cluster_weights(s_cluster_reference cluster_reference, real_point3d const* point);
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask);
	static void __cdecl populate_atmosphere_parameters(s_cluster_reference cluster_reference, s_weighted_atmosphere_parameters* parameters);
	static void __cdecl invalidate_atmosphere_constants();
	static void __cdecl restore_atmosphere_constants();
	static void __cdecl set_default_atmosphere_constants(s_weighted_atmosphere_parameters* parameters);

	static int32& m_last_custom_index;
	static bool& m_use_local_pvs;
	static s_weighted_atmosphere_parameters& m_default_parameters;

	uint8 __data[0x14];
};
static_assert(sizeof(c_atmosphere_fog_interface) == 0x14);

