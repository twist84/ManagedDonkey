#include "networking/messages/network_messages_session_membership.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/network_game_definitions.hpp"

HOOK_DECLARE(0x004DFE60, network_message_types_register_session_membership);

bool __cdecl c_network_message_boot_machine::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DD770, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_delegate_leadership::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DD7C0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_membership_update::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DD800, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_peer_properties::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DDE80, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_player_add::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DE0B0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_player_properties::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	//return INVOKE(0x004DE140, decode, packet, message_storage_size, message_storage);

	s_network_message_player_properties* message = static_cast<s_network_message_player_properties*>(message_storage);

	packet->read_raw_data("session-id", &message->session_id, 128);
	message->player_update_number = packet->read_integer("player-update-number", 32);
	message->controller_index = packet->read_integer("controller-index", 2);
	bool success = player_configuration_client_decode(packet, &message->player_data.client, 0);

	{
		wchar_t service_tag[5]{};
		packet->read_string_wchar("service-tag", service_tag, 5);
		message->player_data.host_partial.service_tag.set(service_tag);

		message->player_data.host_partial.bungienet_user = packet->read_integer("bungienet-user-flags", 8);

		for (long color_index = 0; color_index < k_color_type_count; color_index++)
			message->player_data.host_partial.colors[color_index].value = packet->read_integer("color", 32);

		for (long armor_index = 0; armor_index < k_armor_type_count; armor_index++)
			message->player_data.host_partial.armors[armor_index] = static_cast<byte>(packet->read_integer("armor", 8));

		for (long consumable_index = 0; consumable_index < 4; consumable_index++)
			message->player_data.host_partial.consumables[consumable_index] = static_cast<char>(packet->read_integer("consumable", 8));
	}

	message->player_voice = packet->read_integer("player-voice", 32);

	return success && !packet->error_occurred() && VALID_INDEX(message->controller_index, k_number_of_controllers);
}

bool __cdecl c_network_message_player_refuse::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DE1C0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_player_remove::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DE210, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_boot_machine::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DE240, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_delegate_leadership::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DE2C0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_membership_update::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DE2F0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_peer_properties::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DF5D0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_player_add::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DFBB0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_player_properties::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	//INVOKE(0x004DFCB0, encode, packet, message_storage_size, message_storage);

	s_network_message_player_properties const* message = static_cast<s_network_message_player_properties const*>(message_storage);

	packet->write_raw_data("session-id", &message->session_id, 128);
	packet->write_integer("player-update-number", message->player_update_number, 32);
	packet->write_integer("controller-index", message->controller_index, 2);
	player_configuration_client_encode(packet, &message->player_data.client, 0);

	{
		packet->write_string_wchar("service-tag", message->player_data.host_partial.service_tag.get_string(), 5);

		packet->write_integer("bungienet-user-flags", message->player_data.host_partial.bungienet_user, 8);

		for (long color_index = 0; color_index < k_color_type_count; color_index++)
			packet->write_integer("color", message->player_data.host_partial.colors[color_index].value, 32);

		for (long armor_index = 0; armor_index < k_armor_type_count; armor_index++)
			packet->write_integer("armor", message->player_data.host_partial.armors[armor_index], 8);

		for (long consumable_index = 0; consumable_index < 4; consumable_index++)
			packet->write_integer("consumable", message->player_data.host_partial.consumables[consumable_index], 8);
	}

	packet->write_integer("player-voice", message->player_voice, 32);
}

void __cdecl c_network_message_player_refuse::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DFDA0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_player_remove::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DFE10, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_session_membership(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004DFE60, network_message_types_register_session_membership, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_membership_update,
		"membership-update",
		0,
		sizeof(s_network_message_membership_update),
		sizeof(s_network_message_membership_update),
		c_network_message_membership_update::encode,
		c_network_message_membership_update::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_peer_properties,
		"peer-properties",
		0,
		sizeof(s_network_message_peer_properties),
		sizeof(s_network_message_peer_properties),
		c_network_message_peer_properties::encode,
		c_network_message_peer_properties::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_delegate_leadership,
		"delegate-leadership",
		0,
		sizeof(s_network_message_delegate_leadership),
		sizeof(s_network_message_delegate_leadership),
		c_network_message_delegate_leadership::encode,
		c_network_message_delegate_leadership::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_boot_machine,
		"boot-machine",
		0,
		sizeof(s_network_message_boot_machine),
		sizeof(s_network_message_boot_machine),
		c_network_message_boot_machine::encode,
		c_network_message_boot_machine::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_add,
		"player-add",
		0,
		sizeof(s_network_message_player_add),
		sizeof(s_network_message_player_add),
		c_network_message_player_add::encode,
		c_network_message_player_add::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_refuse,
		"player-refuse",
		0,
		sizeof(s_network_message_player_refuse),
		sizeof(s_network_message_player_refuse),
		c_network_message_player_refuse::encode,
		c_network_message_player_refuse::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_remove,
		"player-remove",
		0,
		sizeof(s_network_message_player_remove),
		sizeof(s_network_message_player_remove),
		c_network_message_player_remove::encode,
		c_network_message_player_remove::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_properties,
		"player-properties",
		0,
		sizeof(s_network_message_player_properties),
		sizeof(s_network_message_player_properties),
		c_network_message_player_properties::encode,
		c_network_message_player_properties::decode,
		nullptr,
		nullptr);
}

