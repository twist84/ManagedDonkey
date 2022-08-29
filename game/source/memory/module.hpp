#pragma once

#include "cseries/cseries.hpp"

union module_address
{
    dword address;
    byte* data;
    void* pointer;
};

extern module_address global_module;

template<dword address>
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
    c_hook_call(module_address const function, bool remove_base = true);

    bool apply();
    bool revert();

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

    bool apply();
    bool revert();

private:
    module_address m_addr;
    byte const* m_bytes;
    byte* m_bytes_original;
    long m_byte_count;
};
