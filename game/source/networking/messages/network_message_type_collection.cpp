#include "networking/messages/network_message_type_collection.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"


REFERENCE_DECLARE(0x0224A498, c_network_message_type_collection*, g_network_message_types);

HOOK_DECLARE_CLASS(0x0047FF50, c_network_message_type_collection, _clear_message_types);
void __fastcall c_network_message_type_collection::_clear_message_types(c_network_message_type_collection* _this, void* unused)
{
	_this->clear_message_types();
	//HOOK_INVOKE_CLASS(, c_network_message_type_collection, _clear_message_types, void(__thiscall*)(c_network_message_type_collection*), _this);
}

HOOK_DECLARE_CLASS(0x0047FF70, c_network_message_type_collection, _decode_message);
bool __fastcall c_network_message_type_collection::_decode_message(c_network_message_type_collection* _this, void* unused, c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage)
{
	bool result = _this->decode_message(packet, message_type, message_storage_size, message_storage);

	HOOK_INVOKE_CLASS(result =, c_network_message_type_collection, _decode_message, bool(__thiscall*)(c_network_message_type_collection*, c_bitstream*, e_network_message_type*, long*, void*), _this, packet, message_type, message_storage_size, message_storage);
	return result;
}

HOOK_DECLARE_CLASS(0x0047FFE0, c_network_message_type_collection, _decode_message_header);
bool __fastcall c_network_message_type_collection::_decode_message_header(c_network_message_type_collection* _this, void* unused, c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size)
{
	bool result = _this->decode_message_header(packet, message_type, message_storage_size);

	HOOK_INVOKE_CLASS(result =, c_network_message_type_collection, _decode_message_header, bool(__thiscall*)(c_network_message_type_collection*, c_bitstream*, e_network_message_type*, long*), _this, packet, message_type, message_storage_size);
	return result;
}

HOOK_DECLARE_CLASS(0x00480070, c_network_message_type_collection, _dispose_message);
void __fastcall c_network_message_type_collection::_dispose_message(c_network_message_type_collection* _this, void* unused, e_network_message_type message_type, long message_storage_size, void* message_storage)
{
	_this->dispose_message(message_type, message_storage_size, message_storage);
	HOOK_INVOKE_CLASS(, c_network_message_type_collection, _dispose_message, void(__thiscall*)(c_network_message_type_collection*, e_network_message_type, long, void*), _this, message_type, message_storage_size, message_storage);
}

HOOK_DECLARE_CLASS(0x00480090, c_network_message_type_collection, _encode_message);
void __fastcall c_network_message_type_collection::_encode_message(c_network_message_type_collection* _this, void* unused, c_bitstream* packet, e_network_message_type message_type, long message_storage_size, void* message_storage)
{
	_this->encode_message(packet, message_type, message_storage_size, message_storage);
	HOOK_INVOKE_CLASS(, c_network_message_type_collection, _encode_message, void(__thiscall*)(c_network_message_type_collection*, c_bitstream*, e_network_message_type, long, void*), _this, packet, message_type, message_storage_size, message_storage);
}

HOOK_DECLARE_CLASS(0x004800D0, c_network_message_type_collection, _encode_message_header);
void __fastcall c_network_message_type_collection::_encode_message_header(c_network_message_type_collection* _this, void* unused, c_bitstream* packet, e_network_message_type message_type, long message_storage_size)
{
	_this->encode_message_header(packet, message_type, message_storage_size);
	HOOK_INVOKE_CLASS(, c_network_message_type_collection, _encode_message_header, void(__thiscall*)(c_network_message_type_collection*, c_bitstream*, e_network_message_type, long), _this, packet, message_type, message_storage_size);
}

//HOOK_DECLARE_CLASS(0x00480180, c_network_message_type_collection, _get_message_type_name);
//char const* __fastcall _get_message_type_name(c_network_message_type_collection* _this, void* unused, e_network_message_type message_type)
//{
//	char const* result = _this->get_message_type_name(message_type);
//
//	HOOK_INVOKE_CLASS(result =, c_network_message_type_collection, _get_message_type_name, char const*(__thiscall*)(c_network_message_type_collection*, e_network_message_type), _this, message_type);
//	return result;
//}

HOOK_DECLARE_CLASS(0x004801B0, c_network_message_type_collection, _register_message_type);
void __fastcall c_network_message_type_collection::_register_message_type(
	c_network_message_type_collection* _this,
	void* unused,
	e_network_message_type message_type,
	char const* message_type_name,
	long flags,
	long message_size,
	long message_size_maximum,
	encode_t* encode_function,
	decode_t* decode_function,
	compare_t* compare_function,
	dispose_t* dispose_function
)
{
	_this->register_message_type(
		message_type,
		message_type_name,
		flags,
		message_size,
		message_size_maximum,
		encode_function,
		decode_function,
		compare_function,
		dispose_function
	);
	
	//HOOK_INVOKE_CLASS(, c_network_message_type_collection, _register_message_type, 
	//	void(__thiscall*)(c_network_message_type_collection*, e_network_message_type, char const*, long, long, long, encode_t*, decode_t*, compare_t*, dispose_t*),
	//	_this,
	//	message_type,
	//	message_type_name,
	//	flags,
	//	message_size,
	//	message_size_maximum,
	//	encode_function,
	//	decode_function,
	//	compare_function,
	//	dispose_function
	//);
}

void __cdecl c_network_message_type_collection::clear_message_types()
{
	csmemset(m_message_types, 0, sizeof(m_message_types));
}

bool __cdecl c_network_message_type_collection::decode_message(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage) const
{
	return false;
}

bool c_network_message_type_collection::decode_message_header(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size) const
{
	return false;
}

void __cdecl c_network_message_type_collection::dispose_message(e_network_message_type message_type, long message_storage_size, void* message_storage) const
{

}

void __cdecl c_network_message_type_collection::encode_message(c_bitstream* packet, e_network_message_type message_type, long message_storage_size, void const* message_storage) const
{

}

void c_network_message_type_collection::encode_message_header(c_bitstream* packet, e_network_message_type message_type, long message_storage_size) const
{

}

char const* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type) const
{
	return nullptr;
}

void __cdecl c_network_message_type_collection::register_message_type(
	e_network_message_type message_type,
	char const* message_type_name,
	long flags,
	long message_size,
	long message_size_maximum,
	encode_t* encode_function,
	decode_t* decode_function,
	compare_t* compare_function,
	dispose_t* dispose_function)
{
	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);
	ASSERT(message_type_name);
	//ASSERT(message_size > 0 && message_size <= k_network_message_maximum_size);
	//ASSERT(message_size_maximum > 0 && message_size_maximum <= k_network_message_maximum_size);
	//ASSERT(encode_function);
	//ASSERT(decode_function);
	ASSERT(message_size_maximum >= message_size);

	s_network_message_type& type = m_message_types[message_type];
	ASSERT(!type.initialized);

	type.message_type_name = message_type_name;
	type.flags = flags;
	type.message_size = message_size;
	type.message_size_maximum = message_size_maximum;
	type.encode_function = encode_function;
	type.decode_function = decode_function;
	type.compare_function = compare_function;
	type.dispose_function = dispose_function;
	type.initialized = true;

	//DECLFUNC(0x004801B0,
	//	void, __thiscall, c_network_message_type_collection const*,
	//	e_network_message_type,
	//	char const*,
	//	long,
	//	long,
	//	long,
	//	encode_t*,
	//	decode_t*,
	//	compare_t*,
	//	dispose_t*
	//)(
	//	this,
	//	message_type,
	//	message_type_name,
	//	flags,
	//	message_size,
	//	message_size_maximum,
	//	encode_function,
	//	decode_function,
	//	compare_function,
	//	dispose_function
	//);
}

