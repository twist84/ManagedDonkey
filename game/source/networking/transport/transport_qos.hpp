#pragma once

#include "cseries/cseries.hpp"

enum e_transport_qos_target_status;
struct s_transport_qos_listener_stats;

struct s_transport_qos_result
{
	long probes_sent;
	long probes_received;
	long ping_msec_minimum;
	long ping_msec_median;
	long bandwidth_upstream_bps;
	long bandwidth_downstream_bps;
	long data_block_size;
	void* data_block;
};
static_assert(sizeof(s_transport_qos_result) == 0x20);

struct s_transport_qos_target;

struct s_transport_secure_identifier;
extern void __cdecl transport_qos_delete(long index);
extern void __cdecl transport_qos_dispose();
extern bool __cdecl transport_qos_get_result(long index, long target_index, s_transport_qos_result* result);
extern long __cdecl transport_qos_get_target_count(long index);
extern e_transport_qos_target_status __cdecl transport_qos_get_target_status(long index, long target_index);
extern void __cdecl transport_qos_initialize();
extern bool __cdecl transport_qos_is_complete(long index);
extern bool __cdecl transport_qos_listener_get_stats(s_transport_secure_identifier const* secure_identifier, s_transport_qos_listener_stats* stats);
extern bool __cdecl transport_qos_listener_initiate(s_transport_secure_identifier const* secure_identifier);
extern void __cdecl transport_qos_listener_set_bandwidth_limit(s_transport_secure_identifier const* secure_identifier, bool a2, long bandwidth_limit);
extern void __cdecl transport_qos_listener_set_reply_block(s_transport_secure_identifier const* secure_identifier, bool a2, long reply_block_size, void const* reply_block);
extern void __cdecl transport_qos_listener_terminate(s_transport_secure_identifier const* secure_identifier);
extern long __cdecl transport_qos_service_new();
extern void __cdecl transport_qos_shutdown();
extern void __cdecl transport_qos_startup();
extern long __cdecl transport_qos_target_new(e_transport_qos_type type, long target_count, s_transport_qos_target const* target, long a4);

