#include "networking/messages/network_messages_session_protocol.hpp"

#include "networking/messages/network_message_type_collection.hpp"

char const* k_join_refuse_reason_strings[k_network_join_refuse_reason_count]
{
	"no-reason-given",
	"tried-to-join-self",
	"could-not-connect",
	"join-timed-out",
	"not-found",
	"privacy-mode",
	"not-joinable",
	"session-full",
	"alpha-split-screen",
	"session-disbanded",
	"session-booted",
	"address-invalid",
	"address-failed",
	"too-many-observers",
	"aborted",
	"abort-ignored",
	"wrong-payload-type",
	"no-reservation",
	"in-matchmaking",
	"player-count-zero",
	"player-not-online-enabled",
	"player-add-pending",
	"player-add-failed",
	"host-time-out",
	"rejected-by-host",
	"peer-version-too-low",
	"host-version-too-low",
	"holding-in-queue",
	"film-in-progress",
	"campaign-in-progress",
	"user-content-not-allowed",
	"survival-in-progress",
	"executable-type-mismatch",
};

char const* network_message_join_refuse_get_reason_string(long reason)
{
	if (VALID_COUNT(reason, k_network_join_refuse_reason_count))
		return k_join_refuse_reason_strings[reason];

	return "<unknown>";
}

bool __cdecl c_network_message_time_synchronize::compare(long a1, void* a2, void* a3)
{
	return INVOKE(0x004DC8F0, compare, a1, a2, a3);
}

bool __cdecl c_network_message_host_decline::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DC920, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_join_abort::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DC980, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_join_refuse::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DC9C0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_join_request::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCA00, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_leave_acknowledge::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCB70, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_leave_session::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCBA0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_peer_connect::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCBD0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_peer_establish::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCC20, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_session_boot::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCC50, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_session_disband::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCC90, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_time_synchronize::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DCCC0, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_time_synchronize::dispose(long a1, void* a2)
{
	INVOKE(0x004DCD70, dispose, a1, a2);
}

void __cdecl c_network_message_host_decline::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DCD80, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_join_abort::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DCE80, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_join_refuse::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DCEB0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_join_request::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DCF10, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_leave_acknowledge::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD300, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_leave_session::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD320, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_peer_connect::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD340, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_peer_establish::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD3B0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_session_boot::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD3D0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_session_disband::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD430, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_time_synchronize::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DD450, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_session_protocol(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004DD5D0, network_message_types_register_session_protocol, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_join_request,
		"join-request",
		0,
		sizeof(s_network_message_join_request),
		sizeof(s_network_message_join_request),
		c_network_message_join_request::encode,
		c_network_message_join_request::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_peer_connect,
		"peer-connect",
		0,
		sizeof(s_network_message_peer_connect),
		sizeof(s_network_message_peer_connect),
		c_network_message_peer_connect::encode,
		c_network_message_peer_connect::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_join_abort,
		"join-abort",
		0,
		sizeof(s_network_message_join_abort),
		sizeof(s_network_message_join_abort),
		c_network_message_join_abort::encode,
		c_network_message_join_abort::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_join_refuse,
		"join-refuse",
		0,
		sizeof(s_network_message_join_refuse),
		sizeof(s_network_message_join_refuse),
		c_network_message_join_refuse::encode,
		c_network_message_join_refuse::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_leave_session,
		"leave-session",
		0,
		sizeof(s_network_message_leave_session),
		sizeof(s_network_message_leave_session),
		c_network_message_leave_session::encode,
		c_network_message_leave_session::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_leave_acknowledge,
		"leave-acknowledge",
		0,
		sizeof(s_network_message_leave_acknowledge),
		sizeof(s_network_message_leave_acknowledge),
		c_network_message_leave_acknowledge::encode,
		c_network_message_leave_acknowledge::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_session_disband,
		"session-disband",
		0,
		sizeof(s_network_message_session_disband),
		sizeof(s_network_message_session_disband),
		c_network_message_session_disband::encode,
		c_network_message_session_disband::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_session_boot,
		"session-boot",
		0,
		sizeof(s_network_message_session_boot),
		sizeof(s_network_message_session_boot),
		c_network_message_session_boot::encode,
		c_network_message_session_boot::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_host_decline,
		"host-decline",
		0,
		sizeof(s_network_message_host_decline),
		sizeof(s_network_message_host_decline),
		c_network_message_host_decline::encode,
		c_network_message_host_decline::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_peer_establish,
		"peer-establish",
		0,
		sizeof(s_network_message_peer_establish),
		sizeof(s_network_message_peer_establish),
		c_network_message_peer_establish::encode,
		c_network_message_peer_establish::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_time_synchronize,
		"time-synchronize",
		0,
		sizeof(s_network_message_time_synchronize),
		sizeof(s_network_message_time_synchronize),
		c_network_message_time_synchronize::encode,
		c_network_message_time_synchronize::decode,
		c_network_message_time_synchronize::compare,
		c_network_message_time_synchronize::dispose);
}

