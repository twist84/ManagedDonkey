#pragma once

static module_address global_module = { .pointer = GetModuleHandle(NULL) };

template<dword address>
c_hook_call<address>::c_hook_call(module_address const function, bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) })
{
    memcpy(&m_call_original, m_addr.pointer, sizeof(call_instruction));
    apply();
}

template<dword address>
bool c_hook_call<address>::apply()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, &m_call, sizeof(call_instruction));

    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
        return false;

    return true;
}

template<dword address>
bool c_hook_call<address>::revert()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, &m_call_original, sizeof(call_instruction));

    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
        return false;

    return true;
}

template<dword address>
template<long k_patch_size>
c_data_patch<address>::c_data_patch(byte const(&bytes)[k_patch_size], bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(bytes),
    m_bytes_original(new byte[m_byte_count]{})
{
    memcpy(m_bytes_original, m_addr.pointer, m_byte_count);
    apply();
}

template<dword address>
template<typename t_type, long k_patch_size>
c_data_patch<address>::c_data_patch(const t_type type, bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(reinterpret_cast<byte const*>(&type)),
    m_bytes_original(new byte[m_byte_count]{})
{
    memcpy(m_bytes_original, m_addr.pointer, m_byte_count);
    apply();
}

template<dword address>
bool c_data_patch<address>::apply()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, m_bytes, m_byte_count);

    if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
        return false;

    return true;
}

template<dword address>
bool c_data_patch<address>::revert()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, m_bytes_original, m_byte_count);

    if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
        return false;

    return true;
}
