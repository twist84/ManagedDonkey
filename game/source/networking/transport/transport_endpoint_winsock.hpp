#pragma once

enum
{
	_transport_readwrite_error_endpoint_closed = 0,
	_transport_write_error_unreachable = -1,
	_transport_readwrite_error_would_block = -2,
	_transport_readwrite_error_unknown = -3,
};

enum e_transport_type
{
	_transport_type_udp = 0,
	_transport_type_vdp,
	_transport_type_tcp,
	_transport_type_unix,

	k_transport_type_count
};

enum e_transport_endpoint_option
{
	// SO_REUSEADDR
	_transport_endpoint_option_reuse_address,

	// SO_LINGER
	_transport_endpoint_option_linger,

	// SO_BROADCAST
	_transport_endpoint_option_broadcast,

	// SO_SNDBUF
	_transport_endpoint_option_send_buffer_size,

	// SO_RCVBUF
	_transport_endpoint_option_recv_buffer_size,

	// 0x4001
	_transport_endpoint_option_alpha,

	k_transport_endpoint_option_count
};

enum e_transport_endpoint_connected_flag
{
	_transport_endpoint_connected_bit = 0,
	_transport_endpoint_listening_bit,
	_transport_endpoint_readable_bit,
	_transport_endpoint_in_set_bit, 
	_transport_endpoint_blocking_bit,
	_transport_endpoint_clientside_bit,
	_transport_endpoint_alpha_bit,

	k_number_of_transport_endpoint_flags,

	k_maximum_outstanding_connect_requests = 16
};

struct transport_endpoint
{
	uns32 socket;
	e_transport_type type;
	int32 flags;
};
static_assert(sizeof(transport_endpoint) == 0xC);

struct transport_address;

extern int32 __cdecl get_platform_socket_option(e_transport_endpoint_option option);
extern transport_endpoint* __cdecl transport_endpoint_accept(transport_endpoint* listening_endpoint);
extern bool __cdecl transport_endpoint_async_connect(transport_endpoint* endpoint, const transport_address* address);
extern bool __cdecl transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected);
extern bool __cdecl transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address);
extern bool __cdecl transport_endpoint_blocking(transport_endpoint* endpoint);
extern bool __cdecl transport_endpoint_connect(transport_endpoint* endpoint, const transport_address* address);
extern bool __cdecl transport_endpoint_connected(transport_endpoint* endpoint);
extern transport_endpoint* __cdecl transport_endpoint_create(e_transport_type type);
extern bool __cdecl transport_endpoint_create_socket(transport_endpoint* endpoint, const transport_address* address);
extern void __cdecl transport_endpoint_delete(transport_endpoint* endpoint);
extern void __cdecl transport_endpoint_disconnect(transport_endpoint* endpoint);
extern int32 __cdecl transport_endpoint_get_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option);
extern bool __cdecl transport_endpoint_get_socket_address(const transport_address* address, int32* socket_address_size, uns8* const socket_address);
extern bool __cdecl transport_endpoint_get_transport_address(int32 socket_address_length, const uns8* socket_address, transport_address* address);
extern e_transport_type __cdecl transport_endpoint_get_type(transport_endpoint* endpoint);
extern bool __cdecl transport_endpoint_listen(transport_endpoint* endpoint);
extern bool __cdecl transport_endpoint_listening(transport_endpoint* endpoint);
extern int16 __cdecl transport_endpoint_read(transport_endpoint* endpoint, void* buffer, int16 length);
extern int16 __cdecl transport_endpoint_read_from(transport_endpoint* endpoint, void* buffer, int16 length, transport_address* source);
extern bool __cdecl transport_endpoint_readable(transport_endpoint* endpoint);
extern bool __cdecl transport_endpoint_reject(transport_endpoint* listening_endpoint);
extern bool __cdecl transport_endpoint_set_blocking(transport_endpoint* endpoint, bool blocking);
extern bool __cdecl transport_endpoint_set_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option, int32 value);
extern void __cdecl transport_endpoint_setup(transport_endpoint* endpoint, e_transport_type type);
extern bool __cdecl transport_endpoint_test(transport_endpoint* endpoint, const transport_address* address);
extern int16 __cdecl transport_endpoint_write(transport_endpoint* endpoint, const void* buffer, int16 length);
extern int16 __cdecl transport_endpoint_write_to(transport_endpoint* endpoint, const void* buffer, int16 length, const transport_address* destination);
extern bool __cdecl transport_endpoint_writeable(transport_endpoint* endpoint);
extern bool __cdecl transport_get_endpoint_address(transport_endpoint* endpoint, transport_address* address);

