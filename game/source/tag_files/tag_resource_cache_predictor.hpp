#pragma once

#include "cseries/cseries.hpp"

struct s_tag_resource_prediction_quantum
{
	int32 internal_resource_handle;
};
COMPILE_ASSERT(sizeof(s_tag_resource_prediction_quantum) == 0x4);

struct s_tag_resource_prediction_atom
{
	int16 identifier;
	uns16 predicted_resource_count;
	int32 first_prediction_index;
};
COMPILE_ASSERT(sizeof(s_tag_resource_prediction_atom) == 0x8);

struct s_tag_resource_prediction_molecule
{
	uns16 predicted_atom_count;
	int16 first_predicted_atom_index;
	uns16 predicted_resource_count;
	int16 first_predicted_resource_index;
};
COMPILE_ASSERT(sizeof(s_tag_resource_prediction_molecule) == 0x8);

struct s_tag_resource_prediction_molecule_key
{
	int32 index_a;
	int32 index_b;
	int32 index_c;
};
COMPILE_ASSERT(sizeof(s_tag_resource_prediction_molecule_key) == 0xC);

class c_tag_resource_cache_prediction_table
{
public:
	c_wrapped_array<const s_tag_resource_prediction_quantum*> m_prediction_quanta;
	c_wrapped_array<const s_tag_resource_prediction_atom*> m_prediction_atoms;
	c_wrapped_array<const int32*> m_prediction_molecule_atoms;
	c_wrapped_array<const s_tag_resource_prediction_molecule*> m_prediction_molecules;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_prediction_table) == 0x20);

class c_tag_resource_prediction_table_base
{
public:
	enum
	{
		k_maximum_mapped_molecules = 8192,
		k_a_hash_scalar = 3307,
		k_b_hash_scalar = 3,
		k_c_hash_scalar = 337,
	};
};

class c_tag_resource_cache_precompiled_predictor :
	public c_tag_resource_prediction_table_base
{
public:
	c_simple_hash_table<int32, k_maximum_mapped_molecules, k_a_hash_scalar, k_b_hash_scalar, k_c_hash_scalar> m_molecule_index_table;
	c_tag_resource_cache_prediction_table m_precomputed_prediction_table;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_precompiled_predictor) == 0x28024);

class c_tag_index_hash_table
{
public:
	c_simple_hash_table<int32, 8192, 3307, 3, 337> m_hash_table;
};
COMPILE_ASSERT(sizeof(c_tag_index_hash_table) == 0x28004);

// $TODO find an actual home
template<typename t_salt_type>
class c_negative_salt_generator
{
public:
	t_salt_type m_salt;
};
COMPILE_ASSERT(sizeof(c_negative_salt_generator<int16>) == sizeof(int16));

class c_tag_resource_prediction_atom_generator;
class c_tag_resource_cache_dynamic_predictor :
	public c_tag_resource_cache_precompiled_predictor
{
public:
	enum
	{
		k_maximum_prediction_atoms = 63488,
		k_maximum_prediction_quanta = 16384,
		k_maximum_prediction_molecule_atoms = 63488,
	};

	c_tag_index_hash_table m_index_hash_table;
	c_tag_resource_prediction_atom_generator* m_tag_atom_generator;
	c_negative_salt_generator<int16> m_prediction_atom_salt_generator;
	c_static_sized_dynamic_array<s_tag_resource_prediction_quantum, k_maximum_prediction_quanta> m_prediction_quanta;
	c_static_sized_dynamic_array<s_tag_resource_prediction_atom, k_maximum_prediction_atoms> m_prediction_atoms;
	c_static_sized_dynamic_array<int32, k_maximum_prediction_molecule_atoms> m_prediction_molecule_atoms;
	c_static_sized_dynamic_array<s_tag_resource_prediction_molecule, k_maximum_mapped_molecules> m_prediction_molecules;

	bool m_ever_ran_out_of_quanta;
	bool m_ever_ran_out_of_atoms;
	bool m_ever_ran_out_of_molecule_atoms;
	bool m_ever_ran_out_of_molecules;

	bool m_ran_out_of_quanta;
	bool m_ran_out_of_atoms;
	bool m_ran_out_of_molecule_atoms;
	bool m_ran_out_of_molecules;

	uns32 m_last_reset_time;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_dynamic_predictor) == 0x12A04C);

