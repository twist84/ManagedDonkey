#pragma once

#include "cseries/cseries.hpp"

struct s_transport_qos_listener_stats
{
	int32 data_size;
	int32 data_requests_received;
	int32 probe_requests_received;
	int32 discarded_count;
	int32 data_replies_sent;
	int32 data_replies_bytes_sent;
	int32 probe_replies_sent;
};
COMPILE_ASSERT(sizeof(s_transport_qos_listener_stats) == 0x1C);

struct s_qos_listener_data
{
	bool logged_initial;
	uns32 update_time;
	s_transport_qos_listener_stats qos_listener_stats;
};
COMPILE_ASSERT(sizeof(s_qos_listener_data) == 0x24);

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
COMPILE_ASSERT(sizeof(s_transport_qos_result) == 0x20);

struct s_transport_qos_target;

struct s_transport_secure_identifier;
extern void __cdecl transport_qos_delete(int32 index);
extern void __cdecl transport_qos_dispose();
extern bool __cdecl transport_qos_get_result(int32 index, int32 target_index, s_transport_qos_result* result);
extern int32 __cdecl transport_qos_get_target_count(int32 index);
extern e_transport_qos_target_status __cdecl transport_qos_get_target_status(int32 index, int32 target_index);
extern void __cdecl transport_qos_initialize();
extern bool __cdecl transport_qos_is_complete(int32 index);
extern bool __cdecl transport_qos_listener_get_stats(const s_transport_secure_identifier* secure_id, s_transport_qos_listener_stats* transport_qos_listener_stats_out);
extern bool __cdecl transport_qos_listener_initiate(const s_transport_secure_identifier* secure_id);
extern void __cdecl transport_qos_listener_set_bandwidth_limit(const s_transport_secure_identifier* secure_id, bool enabled, int32 outgoing_bps);
extern void __cdecl transport_qos_listener_set_reply_block(const s_transport_secure_identifier* secure_id, bool enabled, int32 reply_block_size, const void* reply_block);
extern void __cdecl transport_qos_listener_terminate(const s_transport_secure_identifier* secure_id);
extern int32 __cdecl transport_qos_service_new();
extern void __cdecl transport_qos_shutdown();
extern void __cdecl transport_qos_startup();
extern int32 __cdecl transport_qos_target_new(e_transport_qos_type qos_type, int32 target_count, const s_transport_qos_target* targets, int32 qos_bps);

