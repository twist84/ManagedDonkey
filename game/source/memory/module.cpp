#include "memory/module.hpp"

#include <windows.h>
#include <assert.h>
#include <detours.h>

static module_address global_module = { .pointer = GetModuleHandle(NULL) };

dword global_address_get(dword rva)
{
	if (!global_module.address)
		global_module.pointer = GetModuleHandle(NULL);
	assert(global_module.pointer);

	return global_module.address + rva;
}

const long k_maximum_individual_modification_count = 1024;

long g_call_hook_count = 0;
c_hook_call* call_hooks[k_maximum_individual_modification_count];

long g_detour_hook_count = 0;
c_hook* detour_hooks[k_maximum_individual_modification_count];

long g_data_patch_count = 0;
c_data_patch* data_patches[k_maximum_individual_modification_count];

long g_data_patch_array_count = 0;
c_data_patch_array* data_patch_arrays[k_maximum_individual_modification_count];

void apply_all_hooks(bool revert)
{
	c_hook_call* call_hook = nullptr;
	c_hook* detour_hook = nullptr;

	for (long call_index = 0; call_index < g_call_hook_count; call_index++)
	{
		if (call_hook = call_hooks[call_index]; call_hook)
			call_hook->apply(revert);
	}

	for (long detour_index = 0; detour_index < g_detour_hook_count; detour_index++)
	{
		if (detour_hook = detour_hooks[detour_index]; detour_hook)
			detour_hook->apply(revert);
	}
}

void apply_all_patches(bool revert)
{
	c_data_patch* data_patch = nullptr;
	c_data_patch_array* data_patch_array = nullptr;

	for (long data_patch_index = 0; data_patch_index < g_data_patch_count; data_patch_index++)
	{
		if (data_patch = data_patches[data_patch_index]; data_patch)
			data_patch->apply(revert);
	}

	for (long data_patch_array_index = 0; data_patch_array_index < g_data_patch_array_count; data_patch_array_index++)
	{
		if (data_patch_array = data_patch_arrays[data_patch_array_index]; data_patch_array)
			data_patch_array->apply(revert);
	}
}

c_hook::c_hook(dword address, module_address const function, bool remove_base) :
	m_addr({ .address = global_address_get(remove_base ? function.address - 0x00400000 : function.address) }),
	m_orig({ .address = global_address_get(remove_base ? address - 0x00400000 : address) })
{
	if (g_detour_hook_count < k_maximum_individual_modification_count)
		detour_hooks[g_detour_hook_count++] = this;
}

bool c_hook::apply(bool revert)
{
	if (NO_ERROR != DetourTransactionBegin())
		return false;

	if (NO_ERROR != DetourUpdateThread(GetCurrentThread()))
		return false;

	if (NO_ERROR != (revert ? DetourDetach : DetourAttach)(&m_orig.pointer, m_addr.pointer))
		return false;

	if (NO_ERROR != DetourTransactionCommit())
		return false;

	return true;
}

c_hook_call::c_hook_call(dword address, module_address const function, bool remove_base) :
	m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
	m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) })
{
	if (g_call_hook_count < k_maximum_individual_modification_count)
		call_hooks[g_call_hook_count++] = this;
}

bool c_hook_call::apply(bool revert)
{
	if (!revert)
		memcpy(&m_call_original, m_addr.pointer, sizeof(call_instruction));

	dword protect;
	if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
		return false;

	memcpy(m_addr.pointer, revert ? &m_call_original : &m_call, sizeof(call_instruction));

	if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
		return false;

	return true;
}

template<long k_patch_size>
c_data_patch::c_data_patch(dword address, byte const(&bytes)[k_patch_size], bool remove_base) :
	m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
	m_byte_count(k_patch_size),
	m_bytes(bytes),
	m_bytes_original(new byte[m_byte_count]{})
{
	if (g_data_patch_count < k_maximum_individual_modification_count)
		data_patches[g_data_patch_count++] = this;
}

template<typename t_type, long k_patch_size>
c_data_patch::c_data_patch(dword address, const t_type type, bool remove_base) :
	m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
	m_byte_count(k_patch_size),
	m_bytes(reinterpret_cast<byte const*>(&type)),
	m_bytes_original(new byte[m_byte_count]{})
{
	if (g_data_patch_count < k_maximum_individual_modification_count)
		data_patches[g_data_patch_count++] = this;
}

bool c_data_patch::apply(bool revert)
{
	if (!revert)
		memcpy(m_bytes_original, m_addr.pointer, m_byte_count);

	dword protect;
	if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
		return false;

	memcpy(m_addr.pointer, revert ? m_bytes_original : m_bytes, m_byte_count);

	if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
		return false;

	return true;
}

template<long k_address_count, long k_patch_size>
c_data_patch_array::c_data_patch_array(dword const(&_addresses)[k_address_count], byte const(&patch)[k_patch_size]) :
	address_count(k_address_count),
	byte_count(k_patch_size),
	addresses(_addresses),
	bytes(patch),
	bytes_original(new byte* [k_patch_size] {})
{
	if (g_data_patch_array_count < k_maximum_individual_modification_count)
		data_patch_arrays[g_data_patch_array_count++] = this;
}

template<long k_patch_size>
c_data_patch_array::c_data_patch_array(dword address, byte const(&patch)[k_patch_size]) :
	address_count(1),
	byte_count(k_patch_size),
	addresses(&address),
	bytes(patch),
	bytes_original(new byte* [k_patch_size] {})
{
	if (g_data_patch_array_count < k_maximum_individual_modification_count)
		data_patch_arrays[g_data_patch_array_count++] = this;
}

c_data_patch_array::~c_data_patch_array()
{
	if (bytes_original)
	{
		for (long i = 0; i < address_count; i++)
		{
			if (bytes_original[i])
			{
				delete[] bytes_original[i];
				bytes_original[i] = nullptr;
			}
		}

		delete[] bytes_original;
		bytes_original = nullptr;
	}
}

bool c_data_patch_array::apply(bool revert)
{
	module_address address{};
	for (long i = 0; i < address_count; i++)
	{
		address.address = addresses[i];

		if (!revert)
			bytes_original[i] = (byte*)memcpy(new byte[byte_count]{}, address.pointer, byte_count);

		dword protect;
		if (!VirtualProtect(address.pointer, byte_count, PAGE_READWRITE, &protect))
			continue;

		memcpy(address.pointer, revert ? bytes_original[i] : bytes, byte_count);

		if (!VirtualProtect(address.pointer, byte_count, protect, &protect))
			continue;
	}

	return true;
}

void buffer_as_byte_string(byte* buffer, dword buffer_size, char** out_string)
{
	char* temp = new char[3 * buffer_size + 1] {};
	for (dword i = 0; i < buffer_size; i++)
		csnzprintf(&temp[3 * i], 3 * buffer_size + 1, "%02X ", buffer[i]);

	if (out_string)
	{
		*out_string = temp;
		return;
	}

	delete[] temp;
}
