#include "networking/messages/network_message_type_collection.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "networking/messages/network_messages_text_chat.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0047FF50, c_network_message_type_collection, clear_message_types_);
HOOK_DECLARE_CLASS_MEMBER(0x0047FF70, c_network_message_type_collection, decode_message_);
HOOK_DECLARE_CLASS_MEMBER(0x0047FFE0, c_network_message_type_collection, decode_message_header_);
HOOK_DECLARE_CLASS_MEMBER(0x00480070, c_network_message_type_collection, dispose_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00480090, c_network_message_type_collection, encode_message_);
HOOK_DECLARE_CLASS_MEMBER(0x004800D0, c_network_message_type_collection, encode_message_header_);
HOOK_DECLARE_CLASS_MEMBER(0x00480180, c_network_message_type_collection, get_message_type_name_);
HOOK_DECLARE_CLASS_MEMBER(0x004801B0, c_network_message_type_collection, register_message_type_);

void __thiscall c_network_message_type_collection::clear_message_types_()
{
	clear_message_types();
}

bool __thiscall c_network_message_type_collection::decode_message_(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size, void* message_storage)
{
	return decode_message(packet, message_type, message_storage_size, message_storage);
}

bool __thiscall c_network_message_type_collection::decode_message_header_(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size)
{
	return decode_message_header(packet, message_type, message_storage_size);
}

void __thiscall c_network_message_type_collection::dispose_message_(e_network_message_type message_type, int32 message_storage_size, void* message_storage)
{
	dispose_message(message_type, message_storage_size, message_storage);
}

void __thiscall c_network_message_type_collection::encode_message_(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size, void* message_storage)
{
	encode_message(packet, message_type, message_storage_size, message_storage);
}

void __thiscall c_network_message_type_collection::encode_message_header_(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size)
{
	encode_message_header(packet, message_type, message_storage_size);
}

const char* __thiscall c_network_message_type_collection::get_message_type_name_(e_network_message_type message_type)
{
	return get_message_type_name(message_type);
}

void __thiscall c_network_message_type_collection::register_message_type_(
	e_network_message_type message_type,
	const char* message_type_name,
	int32 flags,
	int32 message_size,
	int32 message_size_maximum,
	encode_t* encode_function,
	decode_t* decode_function,
	compare_t* compare_function,
	dispose_t* dispose_function
)
{
	register_message_type(
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
}

void __cdecl c_network_message_type_collection::check_message_types() const
{
	for (int32 message_type_index = 0; message_type_index < k_network_message_type_count; message_type_index++)
		ASSERT(m_message_types[message_type_index].initialized);
}

void __cdecl c_network_message_type_collection::clear_message_types()
{
	csmemset(m_message_types, 0, sizeof(m_message_types));
}

bool __cdecl c_network_message_type_collection::decode_message(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size, void* message_storage) const
{
	ASSERT(packet);
	ASSERT(message_type);
	ASSERT(message_storage_size);
	ASSERT(message_storage);

	if (decode_message_header(packet, message_type, message_storage_size))
	{
		const s_network_message_type* type_definition = &m_message_types[*message_type];

		ASSERT(*message_type >= 0 && *message_type < k_network_message_type_count);
		ASSERT(type_definition->initialized);
		ASSERT(*message_storage_size >= 0 && *message_storage_size <= k_network_message_maximum_size);
		ASSERT(*message_storage_size >= type_definition->message_size);
		ASSERT(*message_storage_size <= type_definition->message_size_maximum);

		csmemset(message_storage, 0, *message_storage_size);

		if (TEST_BIT(type_definition->flags, 1))
			return true;

		ASSERT(type_definition->decode_function);
		return type_definition->decode_function(packet, *message_storage_size, message_storage);
	}

	return false;
}

bool c_network_message_type_collection::decode_message_header(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size) const
{
	ASSERT(packet);
	ASSERT(message_type);
	ASSERT(message_storage_size);

	*message_type = packet->read_enum<e_network_message_type, 8>("type");
	*message_storage_size = packet->read_integer("size", 18);
	if (!packet->error_occurred() && *message_type >= 0 && *message_type < k_network_message_type_count)
	{
		const s_network_message_type* type_definition = &m_message_types[*message_type];
		if (type_definition->initialized)
		{
			if (*message_storage_size >= type_definition->message_size && *message_storage_size <= type_definition->message_size_maximum)
			{
				packet->read_identifier(type_definition->message_type_name);
				return !packet->error_occurred();
			}
		}
	}

	return false;
}

void __cdecl c_network_message_type_collection::dispose_message(e_network_message_type message_type, int32 message_storage_size, void* message_storage) const
{
	const s_network_message_type* type_definition = &m_message_types[message_type];

	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);
	ASSERT(message_storage_size);
	ASSERT(message_storage);

	if (type_definition->dispose_function)
		type_definition->dispose_function(message_storage_size, message_storage);
}

void __cdecl c_network_message_type_collection::encode_message(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size, const void* message_storage) const
{
	ASSERT(packet);
	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);

	const s_network_message_type* type_definition = &m_message_types[message_type];
	ASSERT(type_definition->initialized);

	ASSERT(message_storage_size >= type_definition->message_size);
	ASSERT(message_storage_size <= type_definition->message_size_maximum);

	encode_message_header(packet, message_type, message_storage_size);

	if (!TEST_BIT(type_definition->flags, 1))
	{
		ASSERT(type_definition->encode_function);
		type_definition->encode_function(packet, message_storage_size, message_storage);
	}
}

void c_network_message_type_collection::encode_message_header(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size) const
{
	const s_network_message_type* type_definition = &m_message_types[message_type];

	ASSERT(packet);
	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);

	packet->write_integer("type", message_type, 8);
	packet->write_integer("size", message_storage_size, 18);
	packet->write_identifier(type_definition->message_type_name);
}

const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type) const
{
	if (message_type >= 0 && message_type < k_network_message_type_count && m_message_types[message_type].initialized)
		return m_message_types[message_type].message_type_name;

	return "<unknown>";
}

void __cdecl c_network_message_type_collection::register_message_type(
	e_network_message_type message_type,
	const char* message_type_name,
	int32 flags,
	int32 message_size,
	int32 message_size_maximum,
	encode_t* encode_function,
	decode_t* decode_function,
	compare_t* compare_function,
	dispose_t* dispose_function)
{
	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);
	ASSERT(message_type_name);
	if (!TEST_BIT(flags, 1))
	{
		ASSERT(message_size > 0 && message_size <= k_network_message_maximum_size);
		ASSERT(message_size_maximum > 0 && message_size_maximum <= k_network_message_maximum_size);
		ASSERT(encode_function);
		ASSERT(decode_function);
	}

	if (TEST_BIT(flags, 0))
		ASSERT(message_size_maximum > message_size);
	//else
	//	ASSERT(message_size_maximum == message_size); // "synchronous-gamestate", 0x10 == 0x40000, -_-

	s_network_message_type* type = &m_message_types[message_type];
	ASSERT(!type->initialized);

	type->message_type_name = message_type_name;
	type->flags = flags;
	type->message_size = message_size;
	type->message_size_maximum = message_size_maximum;
	type->encode_function = encode_function;
	type->decode_function = decode_function;
	type->compare_function = compare_function;
	type->dispose_function = dispose_function;
	type->initialized = true;
}

