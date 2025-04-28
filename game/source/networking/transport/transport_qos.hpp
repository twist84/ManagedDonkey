#pragma once

#include "cseries/cseries.hpp"

struct s_transport_qos_listener_stats;

struct s_transport_qos_result
{
	int32 probes_sent;
	int32 probes_received;
	int32 ping_msec_minimum;
	int32 ping_msec_median;
	int32 bandwidth_upstream_bps;
	int32 bandwidth_downstream_bps;
	int32 data_block_size;
	void* data_block;
};
static_assert(sizeof(s_transport_qos_result) == 0x20);

struct s_transport_qos_target;

struct s_transport_secure_identifier;
extern void __cdecl transport_qos_delete(int32 index);
extern void __cdecl transport_qos_dispose();
extern bool __cdecl transport_qos_get_result(int32 index, int32 target_index, s_transport_qos_result* result);
extern int32 __cdecl transport_qos_get_target_count(int32 index);
extern e_transport_qos_target_status __cdecl transport_qos_get_target_status(int32 index, int32 target_index);
extern void __cdecl transport_qos_initialize();
extern bool __cdecl transport_qos_is_complete(int32 index);
extern bool __cdecl transport_qos_listener_get_stats(s_transport_secure_identifier const* secure_identifier, s_transport_qos_listener_stats* stats);
extern bool __cdecl transport_qos_listener_initiate(s_transport_secure_identifier const* secure_identifier);
extern void __cdecl transport_qos_listener_set_bandwidth_limit(s_transport_secure_identifier const* secure_identifier, bool a2, int32 bandwidth_limit);
extern void __cdecl transport_qos_listener_set_reply_block(s_transport_secure_identifier const* secure_identifier, bool a2, int32 reply_block_size, void const* reply_block);
extern void __cdecl transport_qos_listener_terminate(s_transport_secure_identifier const* secure_identifier);
extern int32 __cdecl transport_qos_service_new();
extern void __cdecl transport_qos_shutdown();
extern void __cdecl transport_qos_startup();
extern int32 __cdecl transport_qos_target_new(e_transport_qos_type type, int32 target_count, s_transport_qos_target const* target, int32 a4);

