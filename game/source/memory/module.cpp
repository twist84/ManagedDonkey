#include "memory/module.hpp"

#include <windows.h>
#include <detours.h>

static module_address global_module = { .pointer = GetModuleHandle(nullptr) };

uns32 global_address_get(uns32 rva)
{
	if (!global_module.address)
		global_module.pointer = GetModuleHandle(nullptr);
	ASSERT(global_module.pointer);

	return global_module.address + rva;
}

static void* donkey_module;
void set_donkey_module(void* _module)
{
	donkey_module = _module;
}

void* get_donkey_module()
{
	return donkey_module;
}

const int32 k_maximum_individual_modification_count = 4096;

int32 g_call_hook_count = 0;
c_hook_call* call_hooks[k_maximum_individual_modification_count];

int32 g_detour_hook_count = 0;
c_hook* detour_hooks[k_maximum_individual_modification_count];

int32 g_data_patch_count = 0;
c_data_patch* data_patches[k_maximum_individual_modification_count];

int32 g_data_patch_array_count = 0;
c_data_patch_array* data_patch_arrays[k_maximum_individual_modification_count];

void apply_all_hooks(bool revert)
{
	c_hook_call* call_hook = nullptr;
	c_hook* detour_hook = nullptr;

	for (int32 call_index = 0; call_index < g_call_hook_count; call_index++)
	{
		if (call_hook = call_hooks[call_index]; call_hook)
		{
			if (!call_hook->apply(revert) && !revert)
				printf("call hook didn't apply: 0x%08X, %s\n", call_hook->get_address(), call_hook->get_name());
		}
	}

	for (int32 detour_index = 0; detour_index < g_detour_hook_count; detour_index++)
	{
		if (detour_hook = detour_hooks[detour_index]; detour_hook)
		{
			if (!detour_hook->apply(revert) && !revert)
				printf("detour hook didn't apply: 0x%08X, %s\n", detour_hook->get_address(), detour_hook->get_name());
		}
	}
}

void apply_all_patches(bool revert)
{
	c_data_patch* data_patch = nullptr;
	c_data_patch_array* data_patch_array = nullptr;

	for (int32 data_patch_index = 0; data_patch_index < g_data_patch_count; data_patch_index++)
	{
		if (data_patch = data_patches[data_patch_index]; data_patch)
		{
			if (!data_patch->apply(revert) && !revert)
				printf("data patch didn't apply: 0x%08X, %s\n", data_patch->get_address(), data_patch->get_name());
		}
	}

	for (int32 data_patch_array_index = 0; data_patch_array_index < g_data_patch_array_count; data_patch_array_index++)
	{
		if (data_patch_array = data_patch_arrays[data_patch_array_index]; data_patch_array)
		{
			if (!data_patch_array->apply(revert) && !revert)
				printf("data patch array didn't apply: %s\n", data_patch_array->get_name());
		}
	}
}

c_hook::c_hook(const char* name, uns32 address, module_address const function, bool remove_base) :
	m_name(name),
	m_addr({ .address = global_address_get(remove_base ? function.address - 0x00400000 : function.address) }),
	m_orig({ .address = global_address_get(remove_base ? address - 0x00400000 : address) })
{
	ASSERT(VALID_COUNT(g_detour_hook_count,  k_maximum_individual_modification_count));
	detour_hooks[g_detour_hook_count++] = this;
}

bool c_hook::apply(bool revert)
{
	if (m_addr.pointer == nullptr || m_orig.pointer == nullptr)
		return false;
	
	if (m_addr.address == 0xFEFEFEFE || m_orig.address == 0xFEFEFEFE)
		return false;

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

c_hook_call::c_hook_call(const char* name, uns32 address, module_address const function, bool remove_base) :
	m_name(name),
	m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
	m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) }),
	m_call_original()
{
	ASSERT(VALID_COUNT(g_call_hook_count, k_maximum_individual_modification_count));
	call_hooks[g_call_hook_count++] = this;
}

bool c_hook_call::apply(bool revert)
{
	if (m_addr.pointer == nullptr)
		return false;

	if (m_addr.address == 0xFEFEFEFE)
		return false;

	if (!revert)
		csmemcpy(&m_call_original, m_addr.pointer, sizeof(call_instruction));

	uns32 protect;
	if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
		return false;

	csmemcpy(m_addr.pointer, revert ? &m_call_original : &m_call, sizeof(call_instruction));

	if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
		return false;

	return true;
}

c_data_patch::c_data_patch(const char* name, uns32 address, int32 patch_size, byte const(&patch)[], bool remove_base) :
	m_name(name),
	m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
	m_byte_count(patch_size),
	m_bytes(patch),
	m_bytes_original(new byte[m_byte_count]{})
{
	ASSERT(VALID_COUNT(g_data_patch_count, k_maximum_individual_modification_count));
	data_patches[g_data_patch_count++] = this;
}

bool c_data_patch::apply(bool revert)
{
	if (m_addr.pointer == nullptr)
		return false;

	if (m_addr.address == 0xFEFEFEFE)
		return false;

	if (!revert)
		csmemcpy(m_bytes_original, m_addr.pointer, m_byte_count);

	uns32 protect;
	if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
		return false;

	csmemcpy(m_addr.pointer, revert ? m_bytes_original : m_bytes, m_byte_count);

	if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
		return false;

	return true;
}

c_data_patch_array::c_data_patch_array(const char* name, int32 address_count, uns32 const(&addresses)[], int32 patch_size, void* patch, bool remove_base) :
	m_name(name),
	m_address_count(address_count),
	m_addresses(addresses),
	m_byte_count(patch_size),
	m_bytes(patch),
	m_bytes_original(new byte* [patch_size] {})
{
	ASSERT(VALID_COUNT(g_data_patch_array_count, k_maximum_individual_modification_count));
	data_patch_arrays[g_data_patch_array_count++] = this;
}

c_data_patch_array::~c_data_patch_array()
{
	if (m_bytes_original)
	{
		for (int32 i = 0; i < m_address_count; i++)
		{
			if (m_bytes_original[i])
			{
				delete[] m_bytes_original[i];
				m_bytes_original[i] = nullptr;
			}
		}

		delete[] m_bytes_original;
		m_bytes_original = nullptr;
	}
}

bool c_data_patch_array::apply(bool revert)
{
	module_address address{};
	for (int32 i = 0; i < m_address_count; i++)
	{
		address.address = m_addresses[i];

		if (!revert)
			m_bytes_original[i] = (byte*)csmemcpy(new byte[m_byte_count]{}, address.pointer, m_byte_count);

		uns32 protect;
		if (!VirtualProtect(address.pointer, m_byte_count, PAGE_READWRITE, &protect))
			continue;

		csmemcpy(address.pointer, revert ? m_bytes_original[i] : m_bytes, m_byte_count);

		if (!VirtualProtect(address.pointer, m_byte_count, protect, &protect))
			continue;
	}

	return true;
}

void buffer_as_byte_string(byte* buffer, uns32 buffer_size, char* out_string, int32 out_string_size)
{
	csmemset(out_string, 0, out_string_size);

	for (uns32 i = 0; i < buffer_size; i++)
		csnzprintf(&out_string[3 * i], out_string_size, "%02X ", buffer[i]);
}

bool patch_pointer(module_address address, const void* pointer)
{
	uns32 protect;
	if (!VirtualProtect(address.pointer, sizeof(void*), PAGE_READWRITE, &protect))
		return false;

	csmemcpy(address.pointer, &pointer, sizeof(void*));

	if (!VirtualProtect(address.pointer, sizeof(void*), protect, &protect))
		return false;

	return true;
}

