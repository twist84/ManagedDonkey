#pragma once

#include "cseries/cseries.hpp"

struct s_weighted_atmosphere_parameters
{
	bool is_scaled;
	real_rgb_color color;
	real_point3d point;
	real distance_bias;
	real max_fog_thickness;
	vector3d beta_m;
	vector3d beta_p;
	vector3d beta_m_theta_prefix;
	vector3d beta_p_theta_prefix;
	real sun_phase_function;
	real sea_level;
	real mie_height_scale;
	real rayleign_height_scale;
	real sheet_density;
	real full_intensity_height;
	real half_intensity_height;
	real_point3d wind_direction;
};
static_assert(sizeof(s_weighted_atmosphere_parameters) == 0x7C);

struct s_game_cluster_bit_vectors;
struct c_atmosphere_fog_interface
{
public:
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
	static void __cdecl change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3);
	static void __cdecl compute_cluster_weights(s_cluster_reference cluster_reference, real_point3d const* point);
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(dword new_structure_bsp_mask);
	static void __cdecl populate_atmosphere_parameters(s_cluster_reference cluster_reference, s_weighted_atmosphere_parameters* parameters);
	static void __cdecl sub_A397D0();
	static void __cdecl restore_atmosphere_constants();
	static void __cdecl set_default_atmosphere_constants(s_weighted_atmosphere_parameters* parameters);

	static long& long_18BE9DC;
	static s_weighted_atmosphere_parameters& m_default_parameters;

	byte __data[0x14];
};
static_assert(sizeof(c_atmosphere_fog_interface) == 0x14);

