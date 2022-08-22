#pragma once

#include "cseries/cseries.hpp"

union module_address
{
    dword address;
    char* data;
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

private:
    module_address m_addr;
    call_instruction m_call;
};
