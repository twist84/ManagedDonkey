#include "networking/transport/transport_qos.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x00434010, transport_qos_get_result);
HOOK_DECLARE(0x00434030, transport_qos_get_target_status);
HOOK_DECLARE(0x00434050, transport_qos_is_complete);
HOOK_DECLARE(0x00434070, transport_qos_listener_initiate);
HOOK_DECLARE(0x004340B0, transport_qos_service_new);
HOOK_DECLARE(0x004340E0, transport_qos_target_new);

void __cdecl transport_qos_delete(int32 index)
{
	INVOKE(0x00433FF0, transport_qos_delete, index);
}

void __cdecl transport_qos_dispose()
{
	INVOKE(0x00434000, transport_qos_dispose);
}

bool __cdecl transport_qos_get_result(int32 index, int32 target_index, s_transport_qos_result* result)
{
	//return INVOKE(0x00434010, transport_qos_get_result, index, target_index, result);

	result->probes_sent = 1;
	result->probes_received = 1;
	result->ping_msec_minimum = 70;
	result->ping_msec_median = 30;
	result->bandwidth_upstream_bps = 20 * 1024;
	result->bandwidth_downstream_bps = 20 * 1024;
	result->data_block_size = 0;
	result->data_block = nullptr;

	return true;
}

int32 __cdecl transport_qos_get_target_count(int32 index)
{
	return INVOKE(0x00434020, transport_qos_get_target_count, index);
}

e_transport_qos_target_status __cdecl transport_qos_get_target_status(int32 index, int32 target_index)
{
	//return INVOKE(0x00434030, transport_qos_get_target_status, index, target_index);

	return _transport_qos_target_status_succeeded;
}

void __cdecl transport_qos_initialize()
{
	return INVOKE(0x00434040, transport_qos_initialize);
}

bool __cdecl transport_qos_is_complete(int32 index)
{
	//return INVOKE(0x00434050, transport_qos_is_complete, index);

	return true;
}

bool __cdecl transport_qos_listener_get_stats(const s_transport_secure_identifier* secure_id, s_transport_qos_listener_stats* transport_qos_listener_stats_out)
{
	return INVOKE(0x00434060, transport_qos_listener_get_stats, secure_id, transport_qos_listener_stats_out);
}

bool __cdecl transport_qos_listener_initiate(const s_transport_secure_identifier* secure_id)
{
	//return INVOKE(0x00434070, transport_qos_listener_initiate, secure_id);

	return true;
}

void __cdecl transport_qos_listener_set_bandwidth_limit(const s_transport_secure_identifier* secure_id, bool enabled, int32 outgoing_bps)
{
	INVOKE(0x00434080, transport_qos_listener_set_bandwidth_limit, secure_id, enabled, outgoing_bps);
}

void __cdecl transport_qos_listener_set_reply_block(const s_transport_secure_identifier* secure_id, bool enabled, int32 reply_block_size, const void* reply_block)
{
	INVOKE(0x00434090, transport_qos_listener_set_reply_block, secure_id, enabled, reply_block_size, reply_block);
}

void __cdecl transport_qos_listener_terminate(const s_transport_secure_identifier* secure_id)
{
	INVOKE(0x004340A0, transport_qos_listener_terminate, secure_id);
}

int32 __cdecl transport_qos_service_new()
{
	//return INVOKE(0x004340B0, transport_qos_service_new);

	return true;
}

void __cdecl transport_qos_shutdown()
{
	INVOKE(0x004340C0, transport_qos_shutdown);
}

void __cdecl transport_qos_startup()
{
	INVOKE(0x004340D0, transport_qos_startup);
}

int32 __cdecl transport_qos_target_new(e_transport_qos_type qos_type, int32 target_count, const s_transport_qos_target* targets, int32 qos_bps)
{
	//return INVOKE(0x004340E0, transport_qos_target_new, qos_type, target_count, targets, qos_bps);

	return 1;
}

