#pragma once

#include "cseries/cseries.hpp"

#define HOOK_DECLARE(ADDR, NAME) c_hook NAME##_hook(ADDR, { .pointer = NAME })
#define HOOK_INVOKE(RESULT, NAME, ...) { NAME##_hook.apply(true); RESULT reinterpret_cast<decltype(NAME)*>(NAME##_hook.get_original())(__VA_ARGS__); NAME##_hook.apply(false); }

union module_address
{
    dword address;
    byte* data;
    void* pointer;
};

extern module_address global_module;
extern dword global_address_get(dword rva);

class c_hook
{
public:
    c_hook(dword original, module_address const function, bool remove_base = true);

    bool apply(bool revert);

    dword get_address()
    {
        return m_addr.address;
    }

    dword get_original()
    {
        return m_orig.address;
    }

private:
    module_address m_addr;
    module_address m_orig;
};

class c_hook_call
{
#pragma pack(push, 1)
    struct call_instruction
    {
        byte opcode;
        dword offset;
    };
    static_assert(sizeof(call_instruction) == 0x5);
#pragma pack(pop)

public:
    c_hook_call(dword original, module_address const function, bool remove_base = true);

    bool apply(bool revert);

    dword get_address()
    {
        return m_addr.address;
    }

private:
    module_address m_addr;
    call_instruction const m_call;
    call_instruction m_call_original;
};

template<dword address>
class c_data_patch
{
public:
    template<long k_patch_size>
    c_data_patch(byte const(&patch)[k_patch_size], bool remove_base = true);

    template<typename t_type, long k_patch_size = sizeof(t_type)>
    c_data_patch(t_type const patch, bool remove_base = true);

    bool apply(bool revert);

private:
    module_address m_addr;
    byte const* m_bytes;
    byte* m_bytes_original;
    long m_byte_count;
};

class c_data_patch_array
{
public:
	template<long k_address_count, long k_patch_size>
	c_data_patch_array(dword const(&_addresses)[k_address_count], byte const(&patch)[k_patch_size]);

	template<long k_patch_size>
	c_data_patch_array(dword address, byte const(&patch)[k_patch_size]);

    ~c_data_patch_array();

	void apply(bool revert);

private:
	long address_count;
	long byte_count;

	const dword* addresses;

	const byte* bytes;
	byte** bytes_original;
};

extern void buffer_as_byte_string(byte* buffer, dword buffer_size, char** out_string);

template<typename t_type>
void type_as_byte_string(t_type* type, char** out_string)
{
    buffer_as_byte_string((byte*)type, sizeof(t_type), out_string);

