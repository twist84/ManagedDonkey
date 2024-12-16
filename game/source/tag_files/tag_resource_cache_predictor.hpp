#pragma once

#include "cseries/cseries.hpp"

struct s_tag_resource_prediction_quantum
{
	long internal_resource_handle;
};
static_assert(sizeof(s_tag_resource_prediction_quantum) == 0x4);

struct s_tag_resource_prediction_atom
{
	short identifier;
	word predicted_resource_count;
	long first_prediction_index;
};
static_assert(sizeof(s_tag_resource_prediction_atom) == 0x8);

struct s_tag_resource_prediction_molecule
{
	word predicted_atom_count;
	short first_predicted_atom_index;
	word predicted_resource_count;
	short first_predicted_resource_index;
};
static_assert(sizeof(s_tag_resource_prediction_molecule) == 0x8);

struct s_tag_resource_prediction_molecule_key
{
	long index_a;
	long index_b;
	long index_c;
};
static_assert(sizeof(s_tag_resource_prediction_molecule_key) == 0xC);

struct c_tag_resource_cache_prediction_table
{
	c_wrapped_array<s_tag_resource_prediction_quantum const*> m_prediction_quanta;
	c_wrapped_array<s_tag_resource_prediction_atom const*> m_prediction_atoms;
	c_wrapped_array<long const*> m_prediction_molecule_atoms;
	c_wrapped_array<s_tag_resource_prediction_molecule const*> m_prediction_molecules;
};
static_assert(sizeof(c_tag_resource_cache_prediction_table) == 0x20);

struct c_tag_resource_prediction_table_base
{
	enum
	{
		k_maximum_mapped_molecules = 8192,
		k_a_hash_scalar = 3307,
		k_b_hash_scalar = 3,
		k_c_hash_scalar = 337,
	};
};

struct c_tag_resource_cache_precompiled_predictor :
	public c_tag_resource_prediction_table_base
{
	c_simple_hash_table<long, k_maximum_mapped_molecules, k_a_hash_scalar, k_b_hash_scalar, k_c_hash_scalar> m_molecule_index_table;
	c_tag_resource_cache_prediction_table m_precomputed_prediction_table;
};
static_assert(sizeof(c_tag_resource_cache_precompiled_predictor) == 0x28024);

struct c_tag_index_hash_table
{
	c_simple_hash_table<long, 8192, 3307, 3, 337> m_hash_table;
};
static_assert(sizeof(c_tag_index_hash_table) == 0x28004);

// #TODO: find an actual home
template<typename t_salt_type>
struct c_negative_salt_generator
{
	t_salt_type m_salt;
};
static_assert(sizeof(c_negative_salt_generator<short>) == sizeof(short));

struct c_tag_resource_prediction_atom_generator;
struct c_tag_resource_cache_dynamic_predictor :
	public c_tag_resource_cache_precompiled_predictor
{
	enum
	{
		k_maximum_prediction_atoms = 63488,
		k_maximum_prediction_quanta = 16384,
		k_maximum_prediction_molecule_atoms = 63488,
	};

	c_tag_index_hash_table m_index_hash_table;
	c_tag_resource_prediction_atom_generator* m_tag_atom_generator;
	c_negative_salt_generator<short> m_prediction_atom_salt_generator;
	c_static_sized_dynamic_array<s_tag_resource_prediction_quantum, k_maximum_prediction_quanta> m_prediction_quanta;
	c_static_sized_dynamic_array<s_tag_resource_prediction_atom, k_maximum_prediction_atoms> m_prediction_atoms;
	c_static_sized_dynamic_array<long, k_maximum_prediction_molecule_atoms> m_prediction_molecule_atoms;
	c_static_sized_dynamic_array<s_tag_resource_prediction_molecule, k_maximum_mapped_molecules> m_prediction_molecules;

	bool m_ever_ran_out_of_quanta;
	bool m_ever_ran_out_of_atoms;
	bool m_ever_ran_out_of_molecule_atoms;
	bool m_ever_ran_out_of_molecules;

	bool m_ran_out_of_quanta;
	bool m_ran_out_of_atoms;
	bool m_ran_out_of_molecule_atoms;
	bool m_ran_out_of_molecules;

	dword m_last_reset_time;
};
static_assert(sizeof(c_tag_resource_cache_dynamic_predictor) == 0x12A04C);

