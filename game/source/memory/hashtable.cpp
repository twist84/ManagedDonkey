#include "memory/hashtable.hpp"

byte const c_hash::k_hash_polynomials[]{ 3, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 54, 59 };
long const c_hash::k_hash_polynomial_count = NUMBEROF(c_hash::k_hash_polynomials);

c_hash::c_hash() :
	m_hash(0),
	m_polynomial_index(0)
{
	DECLFUNC(0x00967E80, c_hash*, __thiscall, c_hash*)(this);
}

c_hash::~c_hash()
{
	DECLFUNC(0x00967E90, c_hash*, __thiscall, c_hash*)(this);
}

dword __cdecl c_hash::add_byte(byte byte_to_add)
{
	return DECLFUNC(0x00967EA0, dword, __thiscall, c_hash*, byte)(this, byte_to_add);

	//ASSERT((m_polynomial_index >= 0) && (m_polynomial_index < k_hash_polynomial_count));
	//
	//byte hash_polynomial = k_hash_polynomials[m_polynomial_index++];
	//if (m_polynomial_index == k_hash_polynomial_count)
	//	m_polynomial_index = 0;
	//
	//m_hash += byte_to_add * hash_polynomial;
	//
	//return m_hash;
}

dword __cdecl c_hash::add_data_range(void const* data, long data_size)
{
	return DECLFUNC(0x00967EE0, dword, __thiscall, c_hash*, void const*, long)(this, data, data_size);

	//for (long i = 0; i < data_size; i++)
	//	add_byte(static_cast<byte const*>(data)[i]);
	//
	//return m_hash;
}

dword __cdecl c_hash::get_hash() const
{
	return DECLFUNC(0x00967F30, dword, __thiscall, c_hash const*)(this);

	//return m_hash;
}

bool __cdecl hash_table_add(s_hash_table* table, void const* key, void const* user_data)
{
	return INVOKE(0x00967F40, hash_table_add, table, key, user_data);
}

long __cdecl sub_967FA0(unsigned int user_data_size, long bucket_count, long maximum_elements)
{
	return INVOKE(0x00967FA0, sub_967FA0, user_data_size, bucket_count, maximum_elements);

	// calculate_allocation_size?
	// maximum_elements * (user_data_size + 0xC) + sizeof(void*) * (bucket_count + 0x10);
}

void __cdecl hash_table_dispose(s_hash_table* table)
{
	return INVOKE(0x00967FC0, hash_table_dispose, table);

	//if (table)
	//{
	//	hash_table_verify(table);
	//	table->allocation->deallocate(table);
	//}
}

void const* __cdecl hash_table_find(s_hash_table* table, void const* key, void* user_data)
{
	return INVOKE(0x00967FE0, hash_table_find, table, key, user_data);
}

s_hash_table_bucket* __cdecl hash_table_find_internal(s_hash_table* table, void const* key)
{
	return INVOKE(0x00968020, hash_table_find_internal, table, key);
}

s_hash_table* __cdecl hash_table_new(char const* name, unsigned int user_data_size, long bucket_count, long maximum_elements, hash_table_hash_function_t* const hash_function, hash_table_compare_function_t* const compare_function, c_allocation_base* allocation)
{
	return INVOKE(0x00968070, hash_table_new, name, user_data_size, bucket_count, maximum_elements, hash_function, compare_function, allocation);
}

void __cdecl hash_table_rebase(s_hash_table* table)
{
	return INVOKE(0x009680E0, hash_table_rebase, table);
}

void __cdecl sub_968190(s_hash_table* table, long a2, dword* a3)
{
	return INVOKE(0x00968190, sub_968190, table, a2, a3);

	//if (*a3)
	//	*a3 = a2 + (table->user_data_size + sizeof(s_hash_table_bucket)) * ((*a3 - reinterpret_cast<dword>(table->base_address)) / (table->user_data_size + sizeof(s_hash_table_bucket)));
}

bool __cdecl hash_table_remove(s_hash_table* table, void const* key)
{
	return INVOKE(0x009681C0, hash_table_remove, table, key);
}

void __cdecl hash_table_reset(s_hash_table* table)
{
	return INVOKE(0x00968250, hash_table_reset, table);

	//long bucket_size = sizeof(void*) * table->bucket_count;
	//csmemset(table + 1, 0, bucket_size + table->maximum_elements * (table->user_data_size + sizeof(s_hash_table_bucket)));
	//
	//void* next_element_offset = offset_pointer(table, bucket_size + sizeof(s_hash_table));
	//long element_size = table->user_data_size + sizeof(s_hash_table_bucket);
	//for (long element_index = 0; element_index < table->maximum_elements; ++element_index)
	//{
	//	s_hash_table_bucket* next_element_to_add = static_cast<s_hash_table_bucket*>(offset_pointer(next_element_offset, element_index * element_size));
	//	//ASSERT(hash_table_valid_bucket_pointer(table, next_element_to_add))
	//
	//	next_element_to_add->next = static_cast<s_hash_table_bucket*>(table->base_address);
	//	table->base_address = next_element_to_add;
	//}
	//
	//hash_table_verify(table);
}

bool __cdecl hash_table_set_data(s_hash_table* table, void const* key, void const* user_data)
{
	return INVOKE(0x009682C0, hash_table_set_data, table, key, user_data);

	//s_hash_table_bucket* current_bucket = hash_table_find_internal(table, key);
	//if (!current_bucket)
	//	return false;
	//
	//csmemcpy(current_bucket + 1, user_data, table->user_data_size);
	//return true;
}

void __cdecl hash_table_set_functions(s_hash_table* table, hash_table_hash_function_t* const hash_function, hash_table_compare_function_t* const compare_function)
{
	return INVOKE(0x00968300, hash_table_set_functions, table, hash_function, compare_function);

	//table->hash_function = hash_function;
	//table->compare_function = compare_function;
}

void __cdecl hash_table_verify(s_hash_table* table)
{
	//for (long bucket_index = 0; bucket_index < table->bucket_count; bucket_index++)
	//{
	//	for (s_hash_table_bucket* current_bucket = reinterpret_cast<s_hash_table_bucket*>(table) + bucket_index; current_bucket; current_bucket = current_bucket->next)
	//	{
	//		//ASSERT(hash_table_valid_bucket_pointer(table, current_bucket));
	//	}
	//}
	//
	//for (s_hash_table_bucket* current_bucket = static_cast<s_hash_table_bucket*>(table->base_address); current_bucket; current_bucket = current_bucket->next)
	//{
	//	//ASSERT(hash_table_valid_bucket_pointer(table, current_bucket));
	//}
}

bool __cdecl string_hash_table_compare_function(void const* string_a, void const* string_b)
{
	return INVOKE(0x00968330, string_hash_table_compare_function, string_a, string_b);

	//return csstrcmp(static_cast<char const*>(string_a), static_cast<char const*>(string_b)) == 0;
}

dword __cdecl string_hash_table_hash_function(void const* string)
{
	return INVOKE(0x00968380, string_hash_table_hash_function, string);

	//c_hash hash;
	//for (byte const* pos = static_cast<byte const*>(string); *pos; ++pos)
	//	hash.add_byte(*pos);
	//
	//return hash.get_hash();
}

