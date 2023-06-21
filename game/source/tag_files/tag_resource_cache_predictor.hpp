#pragma once

#include "cseries/cseries.hpp"

struct c_tag_index_hash_table
{
	// c_simple_hash_table<long, 8192, 3307, 3, 337>
	// c_static_hash_table<c_static_hash_table_data<long, 8192>, 3307, 3, 337>
	long m_total_count;

	struct
	{
		long __unknown0;
		long __unknown4;
		long __unknown8;
		long __unknownC;
	} __unknown4[8192];

	short __unknown20004[8192];
	short __unknown24004[8192];
};
static_assert(sizeof(c_tag_index_hash_table) == 0x28004);

struct s_tag_resource_prediction_quantum
{
	dword internal_resource_handle;
};
static_assert(sizeof(s_tag_resource_prediction_quantum) == 0x4);

struct s_tag_resource_prediction_atom
{
	word identifier;
	word predicted_resource_count;
	dword first_prediction_index;
};
static_assert(sizeof(s_tag_resource_prediction_atom) == 0x8);

struct s_tag_resource_prediction_molecule
{
	word predicted_atom_count;
	word first_predicted_atom_index;
	word predicted_resource_count;
	word first_predicted_resource_index;
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
	c_wrapped_array<s_tag_resource_prediction_quantum const> m_prediction_quanta;
	c_wrapped_array<s_tag_resource_prediction_atom const> m_prediction_atoms;
	c_wrapped_array<long const> m_prediction_molecule_atoms;
	c_wrapped_array<s_tag_resource_prediction_molecule const> m_prediction_molecules;
};
static_assert(sizeof(c_tag_resource_cache_prediction_table) == 0x20);

struct c_tag_resource_cache_precompiled_predictor
{
	// c_simple_hash_table<long, 8192, 3307, 3, 337>
	// c_static_hash_table<c_static_hash_table_data<long, 8192>, 3307, 3, 337>
	struct
	{
		long m_total_count;

		struct
		{
			long __unknown0;
			long __unknown4;
			long __unknown8;
			long __unknownC;
		} __unknown4[8192];

		short __unknown20004[8192];
		short __unknown24004[8192];

	} m_molecule_index_table;

	c_tag_resource_cache_prediction_table m_prediction_table;
};
static_assert(sizeof(c_tag_resource_cache_precompiled_predictor) == 0x28024);

struct c_tag_resource_cache_dynamic_predictor
{
	c_tag_resource_cache_precompiled_predictor m_precompiled_predictor;
	c_tag_index_hash_table m_index_hash_table;

	// struct?
	void* __unknown50028; // vftable
	word __unknown5002C;
	word __unknown5002E;

	c_static_sized_dynamic_array<s_tag_resource_prediction_quantum, 16384> m_prediction_quanta;
	c_static_sized_dynamic_array<s_tag_resource_prediction_atom, 63488> m_prediction_atoms;
	c_static_sized_dynamic_array<long, 63488> m_prediction_molecule_atoms;
	c_static_sized_dynamic_array<s_tag_resource_prediction_molecule, 8192> m_prediction_molecules;

	bool m_prediction_quanta_bool0;
	bool m_prediction_atoms_bool0;
	bool m_prediction_molecule_atoms_bool0;
	bool m_prediction_molecules_bool0;

	bool m_prediction_quanta_bool1;
	bool m_prediction_atoms_bool1;
	bool m_prediction_molecule_atoms_bool1;
	bool m_prediction_molecules_bool1;

	dword m_idle_time;
};
static_assert(sizeof(c_tag_resource_cache_dynamic_predictor) == 0x12A04C);

