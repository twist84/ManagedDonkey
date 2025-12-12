#pragma once

struct transport_endpoint;
struct transport_endpoint_set;

extern int16 __cdecl get_next_available_set_array_index(transport_endpoint_set* set);
extern int __cdecl poll_endpoint_array_compare_proc(const void* a, const void* b);
extern bool __cdecl transport_endpoint_set_add_endpoint(transport_endpoint_set* set, transport_endpoint* endpoint);
extern int16 __cdecl transport_endpoint_set_count_endpoints(transport_endpoint_set* set);
extern transport_endpoint_set* __cdecl transport_endpoint_set_create(int16 maximum_endpoints);
extern void __cdecl transport_endpoint_set_delete(transport_endpoint_set* set);
extern transport_endpoint* __cdecl transport_endpoint_set_get_next_endpoint(transport_endpoint_set* set);
extern bool __cdecl transport_endpoint_set_poll(transport_endpoint_set* set, uns16 milliseconds_timeout);
extern bool __cdecl transport_endpoint_set_remove_endpoint(transport_endpoint_set* set, transport_endpoint* endpoint);
extern void __cdecl transport_endpoint_set_rewind(transport_endpoint_set* set);
extern const char* __cdecl winsock_error_to_string(int err);
extern const char* __cdecl winsock_error_to_string_no_default(int err);

