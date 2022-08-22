#include "memory/module.hpp"

#include <windows.h>
#include <stdio.h>

static module_address global_module = { .pointer = GetModuleHandle(NULL) };

template<dword address>
inline c_hook_call<address>::c_hook_call(module_address const function, bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) })
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
        return;

    memcpy(m_addr.pointer, &m_call, sizeof(call_instruction));
    VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect);
}
