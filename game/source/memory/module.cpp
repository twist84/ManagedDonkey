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

c_hook::c_hook(dword original, module_address const function, bool remove_base) :
    m_addr({ .address = global_address_get(remove_base ? function.address - 0x00400000 : function.address) }),
    m_orig({ .address = global_address_get(remove_base ? original - 0x00400000 : original) })
{
    apply(false);
}

bool c_hook::apply(bool revert)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    (revert ? DetourDetach : DetourAttach)(&m_orig.pointer, m_addr.pointer);
    DetourTransactionCommit();

    return true;
}

c_hook_call::c_hook_call(dword original, module_address const function, bool remove_base) :
    m_addr({ .address = global_address_get(remove_base ? original - 0x00400000 : original) }),
    m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) })
{
    apply(false);
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

template<dword address>
template<long k_patch_size>
c_data_patch<address>::c_data_patch(byte const(&bytes)[k_patch_size], bool remove_base) :
    m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(bytes),
    m_bytes_original(new byte[m_byte_count]{})
{
    apply(false);
}

template<dword address>
template<typename t_type, long k_patch_size>
c_data_patch<address>::c_data_patch(const t_type type, bool remove_base) :
    m_addr({ .address = global_address_get(remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(reinterpret_cast<byte const*>(&type)),
    m_bytes_original(new byte[m_byte_count]{})
{
    apply(false);
}

template<dword address>
bool c_data_patch<address>::apply(bool revert)
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
    apply(false);
}

template<long k_patch_size>
c_data_patch_array::c_data_patch_array(dword address, byte const(&patch)[k_patch_size]) :
    address_count(1),
    byte_count(k_patch_size),
    addresses(&address),
    bytes(patch),
    bytes_original(new byte* [k_patch_size] {})
{
    apply(false);
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

void c_data_patch_array::apply(bool revert)
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
