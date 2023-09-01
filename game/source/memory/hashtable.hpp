#pragma once

#include "cseries/cseries.hpp"

struct c_hash
{
	static byte const k_hash_polynomials[];
	static long const k_hash_polynomial_count;

public:
	c_hash();
	~c_hash();

	dword __cdecl add_byte(byte byte_to_add);
	dword __cdecl add_data_range(void const* data, long data_size);
	dword __cdecl get_hash() const;

protected:
	dword m_hash;
	long m_polynomial_index;
};
static_assert(sizeof(c_hash) == 0x8);

using hash_table_hash_function_t = dword __cdecl(void const*);
using hash_table_compare_function_t = bool __cdecl(void const*, void const*);

struct s_hash_table_bucket
{
	void const* key;
	dword hash;
	s_hash_table_bucket* next;
};
static_assert(sizeof(s_hash_table_bucket) == 0xC);

// long target_bucket_index = hash - hash / table->bucket_count * table->bucket_count;
// long target_bucket_index = hash % table->bucket_count;
// s_hash_table_bucket* target_bucket = table.data[target_bucket_index];

struct s_hash_table
{
	c_static_string<32> name;
	long bucket_count;
	long maximum_elements;
	unsigned int user_data_size;
	hash_table_hash_function_t* hash_function;
	hash_table_compare_function_t* compare_function;
	c_allocation_base* allocation;
	void* base_address;
	s_hash_table_bucket* free_list;

#pragma warning(push)
#pragma warning(disable : 4200)
	s_hash_table_bucket* data[];
#pragma warning(pop)
};
static_assert(sizeof(s_hash_table) == 0x40);

extern bool __cdecl hash_table_add(s_hash_table* table, void const* key, void const* user_data);
extern long __cdecl sub_967FA0(unsigned int user_data_size, long bucket_count, long maximum_elements);
extern void __cdecl hash_table_dispose(s_hash_table* table);
extern void const* __cdecl hash_table_find(s_hash_table* table, void const* key, void* user_data);
extern s_hash_table_bucket* __cdecl hash_table_find_internal(s_hash_table* table, void const* key);
extern s_hash_table* __cdecl hash_table_new(char const* name, unsigned int user_data_size, long bucket_count, long maximum_elements, hash_table_hash_function_t* const hash_function, hash_table_compare_function_t* const compare_function, c_allocation_base* allocation);
extern void __cdecl hash_table_rebase(s_hash_table* table);
extern void __cdecl sub_968190(s_hash_table* table, long a2, dword* a3);
extern bool __cdecl hash_table_remove(s_hash_table* table, void const* key);
extern void __cdecl hash_table_reset(s_hash_table* table);
extern bool __cdecl hash_table_set_data(s_hash_table* table, void const* key, void const* user_data);
extern void __cdecl hash_table_set_functions(s_hash_table* table, hash_table_hash_function_t* const hash_function, hash_table_compare_function_t* const compare_function);
extern void __cdecl hash_table_verify(s_hash_table* table);
extern bool __cdecl string_hash_table_compare_function(void const* string_a, void const* string_b);
extern dword __cdecl string_hash_table_hash_function(void const* string);

template<typename t_key_type, typename t_user_data_type>
struct c_hash_table
{
	c_hash_table() :
		m_hash_table(NULL)
	{
	}

	~c_hash_table()
	{
		if (m_hash_table)
			hash_table_dispose(m_hash_table);
	}

	bool __cdecl create(char const* name, long bucket_count, long maximum_elements, hash_table_hash_function_t* hash_function, hash_table_compare_function_t* compare_function, c_allocation_base* allocation)
	{
		ASSERT(m_hash_table == NULL);
		m_hash_table = hash_table_new(name, sizeof(t_user_data_type), bucket_count, maximum_elements, hash_function, compare_function, allocation);

		return created();
	}

	void __cdecl reset()
	{
		hash_table_reset(m_hash_table);
	}

	bool __cdecl add(t_key_type const* key, t_user_data_type const* user_data)
	{
		return hash_table_add(m_hash_table, key, user_data);
	}

	t_user_data_type const* __cdecl find(t_key_type const* key, t_user_data_type* user_data)
	{
		return hash_table_find(m_hash_table, key, user_data);
	}

	bool __cdecl created()
	{
		return m_hash_table != NULL;
	}

	s_hash_table* m_hash_table;
};
static_assert(sizeof(c_hash_table<long, char>) == sizeof(s_hash_table*));

