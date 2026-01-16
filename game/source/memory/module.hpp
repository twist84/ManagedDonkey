#pragma once

#include "cseries/cseries.hpp"

#define HOOK_DECLARE_CALL_WITH_ADDRESS(ADDR, ADDR2, NAME) inline static c_hook_call CONCAT(NAME##_hook,__LINE__)(#NAME, ADDR, { .address = ADDR2 })
#define HOOK_DECLARE_CALL(ADDR, NAME) inline static c_hook_call CONCAT(NAME##_hook,__LINE__)(#NAME, ADDR, { .pointer = NAME })
#define HOOK_DECLARE(ADDR, NAME) inline static c_hook NAME##_hook(#NAME, ADDR, { .pointer = NAME })
#define HOOK_DECLARE_CLASS(ADDR, CLASS, NAME) inline static c_hook CLASS##_##NAME##_hook(#NAME, ADDR, { .pointer = CLASS::NAME })
#define HOOK_DECLARE_CLASS_MEMBER(ADDR, CLASS, NAME) inline static c_hook CLASS##_##NAME##_hook(#NAME, ADDR, { .pointer = member_to_static_function(&CLASS##::##NAME) })

#define HOOK_INVOKE(RESULT, NAME, ...) \
{ \
	NAME##_hook.apply(true); \
	RESULT reinterpret_cast<decltype(&NAME)>(NAME##_hook.get_original())(__VA_ARGS__); \
	NAME##_hook.apply(false); \
}

#define HOOK_INVOKE_CLASS(RESULT, CLASS, NAME, TYPE, ...) \
{ \
	CLASS##_##NAME##_hook.apply(true); \
	RESULT reinterpret_cast<TYPE>(CLASS##_##NAME##_hook.get_original())(__VA_ARGS__); \
	CLASS##_##NAME##_hook.apply(false); \
}

#define HOOK_INVOKE_CLASS_MEMBER(RESULT, CLASS, NAME, ...) \
{ \
    CLASS##_##NAME##_hook.apply(true); \
    RESULT (this->*static_to_member_t<decltype(&CLASS##::##NAME)>{ .address = CLASS##_##NAME##_hook.get_original() }.function)(__VA_ARGS__); \
    CLASS##_##NAME##_hook.apply(false); \
}

#define DATA_PATCH_DECLARE(ADDR, NAME, ...) static c_data_patch CONCAT(NAME##_patch,__LINE__)(#NAME, ADDR, NUMBEROF(__VA_ARGS__), __VA_ARGS__)
#define DATA_PATCH_DECLARE2(ADDR, NAME, SIZE, ...) static c_data_patch CONCAT(NAME##_patch,__LINE__)(#NAME, ADDR, SIZE, __VA_ARGS__)

#define DATA_PATCH_ARRAY_DECLARE(ADDRS, NAME, ...) static c_data_patch_array CONCAT(NAME##_patch,__LINE__)(#NAME, NUMBEROF(ADDRS), ADDRS, NUMBEROF(__VA_ARGS__), __VA_ARGS__)
#define DATA_PATCH_ARRAY_DECLARE2(ADDRS, NAME, ...) static c_data_patch_array NAME##_patch(#NAME, NUMBEROF(ADDRS), ADDRS, NUMBEROF(__VA_ARGS__), __VA_ARGS__)

#define ASM_ADDR(ADDR, NAME) static void* NAME = (void*)ADDR

union module_address
{
	uns32 address;
	byte* data;
	void* pointer;
};

extern module_address global_module;
extern uns32 global_address_get(uns32 rva);

extern void set_donkey_module(void* _module);
extern void* get_donkey_module();

extern void apply_all_hooks(bool revert);
extern void apply_all_patches(bool revert);

class c_hook
{
public:
	c_hook(const char* name, uns32 address, module_address const function, bool remove_base = true);

	bool apply(bool revert);

	const char* get_name()
	{
		return m_name.get_string();
	}

	uns32 get_address()
	{
		return m_addr.address;
	}

	uns32 get_original()
	{
		return m_orig.address;
	}

private:
	c_static_string<128> m_name;
	module_address m_addr;
	module_address m_orig;
};

class c_hook_call
{
#pragma pack(push, 1)
	struct call_instruction
	{
		byte opcode;
		uns32 offset;
	};
	COMPILE_ASSERT(sizeof(call_instruction) == 0x5);
#pragma pack(pop)

public:
	c_hook_call(const char* name, uns32 address, module_address const function, bool remove_base = true);

	bool apply(bool revert);

	const char* get_name()
	{
		return m_name.get_string();
	}

	uns32 get_address()
	{
		return m_addr.address;
	}

private:
	c_static_string<128> m_name;
	module_address m_addr;
	call_instruction const m_call;
	call_instruction m_call_original;
};

class c_data_patch
{
public:
	c_data_patch(const char* name, uns32 address, int32 patch_size, byte const(&patch)[], bool remove_base = true);

	bool apply(bool revert);

	const char* get_name()
	{
		return m_name.get_string();
	}

	uns32 get_address()
	{
		return m_addr.address;
	}

private:
	c_static_string<128> m_name;
	module_address m_addr;
	const byte* m_bytes;
	byte* m_bytes_original;
	int32 m_byte_count;
};

class c_data_patch_array
{
public:
	c_data_patch_array(const char* name, int32 address_count, uns32 const(&addresses)[], int32 patch_size, void* patch, bool remove_base = true);

	~c_data_patch_array();

	bool apply(bool revert);

	const char* get_name()
	{
		return m_name.get_string();
	}

private:
	c_static_string<128> m_name;
	int32 m_address_count;
	const uns32* m_addresses;
	int32 m_byte_count;
	void* m_bytes;
	byte** m_bytes_original;
};

extern void buffer_as_byte_string(byte* buffer, uns32 buffer_size, char* out_string, int32 out_string_size);

template<typename t_type, int32 k_string_size>
void type_as_byte_string(t_type* type, char(&out_string)[k_string_size])
{
	buffer_as_byte_string((byte*)type, sizeof(t_type), out_string, k_string_size);
}

extern bool patch_pointer(module_address address, const void* pointer);

