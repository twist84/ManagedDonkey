#include "networking/logic/logic_qos_reply_manager.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_time.hpp"
#include "networking/online/online.hpp"
#include "networking/session/network_session.hpp"

REFERENCE_DECLARE(0x018639A0, s_logic_qos_reply_manager_globals, g_logic_qos_reply_manager_globals);

// $TODO remove this when we have full control over `logic_qos_reply_manager_globals`
void __cdecl dynamic_initializer_for_logic_qos_reply_manager_globals()
{
	static s_logic_qos_reply_manager_globals temp;
	csmemcpy(&g_logic_qos_reply_manager_globals, &temp, sizeof(s_logic_qos_reply_manager_globals));
}
HOOK_DECLARE(0x01594CD0, dynamic_initializer_for_logic_qos_reply_manager_globals);

c_group_session_qos_reply_manager::c_group_session_qos_reply_manager() :
	c_session_qos_reply_manager()
{
	//DECLFUNC(0x0049AD90, void, __thiscall, c_group_session_qos_reply_manager*)(this);
}

c_live_service_qos_manager::c_live_service_qos_manager() :
	m_flags()
{
	//DECLFUNC(0x0049ADB0, void, __thiscall, c_live_service_qos_manager*)(this);
}

c_session_qos_reply_manager::c_session_qos_reply_manager() :
	m_initialized(),
	m_qos_listener_update_timestamp(),
	saved_qos_status()
{
	//DECLFUNC(0x0049ADC0, void, __thiscall, c_session_qos_reply_manager*)(this);
}

c_squad_session_qos_reply_manager::c_squad_session_qos_reply_manager() :
	c_session_qos_reply_manager()
{
	//DECLFUNC(0x0049ADE0, void, __thiscall, c_squad_session_qos_reply_manager*)(this);
}

//.text:0049AE00 ; public: s_logic_qos_reply_manager_globals::s_logic_qos_reply_manager_globals()
//.text:0049AE40 ; 

bool __cdecl c_live_service_qos_manager::connected_to_live()
{
	//INVOKE(0x0049AE50, c_live_service_qos_manager::connected_to_live);

	return online_is_connected_to_live();
}

bool c_group_session_qos_reply_manager::desires_qos_reply_block_set()
{
	//return INVOKE_CLASS_MEMBER(0x0049AE60, c_group_session_qos_reply_manager, desires_qos_reply_block_set);

	c_network_session* group_session = nullptr;
	if (!network_life_cycle_in_group_session(&group_session) || !group_session->established() || !group_session->is_host())
	{
		return false;
	}
	return network_life_cycle_desires_qos_reply_block_for_group();
}

bool c_squad_session_qos_reply_manager::desires_qos_reply_block_set()
{
	//return INVOKE_CLASS_MEMBER(0x0049AEB0, c_squad_session_qos_reply_manager, desires_qos_reply_block_set);

	c_network_session* squad_session = nullptr;
	if (!network_life_cycle_in_squad_session(&squad_session) || !squad_session->established() || !squad_session->is_host())
	{
		return false;
	}
	return true;
}

void c_live_service_qos_manager::dispose()
{
	//INVOKE_CLASS_MEMBER(0x0049AF00, c_live_service_qos_manager, dispose);

	if (!m_flags.test(_live_service_qos_manager_qos_in_progress_bit))
	{
		return;
	}
	transport_qos_delete(m_live_service_qos_index);
	m_live_service_qos_index = NONE;
	m_flags.set(_live_service_qos_manager_qos_in_progress_bit, true);
}

void c_session_qos_reply_manager::dispose()
{
	//INVOKE_CLASS_MEMBER(0x0049AF30, c_session_qos_reply_manager, dispose);

	ASSERT(m_initialized);
	m_initialized = false;
}

//.text:0049AF40 ; 

bool c_live_service_qos_manager::get_live_service_qos(s_transport_qos_result* live_service_qos_out) const
{
	//INVOKE_CLASS_MEMBER(0x0049AF50, c_live_service_qos_manager, get_live_service_qos, live_service_qos_out);

	if (!m_flags.test(_live_service_qos_manager_qos_completed_bit))
	{
		return false;
	}
	*live_service_qos_out = m_live_service_qos;
	return true;
}

int32 c_group_session_qos_reply_manager::get_qos_reply_bandwidth_limit()
{
	return INVOKE_CLASS_MEMBER(0x0049AF90, c_group_session_qos_reply_manager, get_qos_reply_bandwidth_limit);
}

int32 c_squad_session_qos_reply_manager::get_qos_reply_bandwidth_limit()
{
	return INVOKE_CLASS_MEMBER(0x0049AFA0, c_squad_session_qos_reply_manager, get_qos_reply_bandwidth_limit);
}

int32 c_session_qos_reply_manager::get_qos_reply_block(const s_network_squad_status_data* qos_status, int32 max_block_size, byte* block_data)
{
	return INVOKE_CLASS_MEMBER(0x0049AFC0, c_session_qos_reply_manager, get_qos_reply_block, qos_status, max_block_size, block_data);
}

bool c_group_session_qos_reply_manager::get_qos_status(s_network_squad_status_data* qos_status_out)
{
	return INVOKE_CLASS_MEMBER(0x0049B060, c_group_session_qos_reply_manager, get_qos_status, qos_status_out);
}

bool c_squad_session_qos_reply_manager::get_qos_status(s_network_squad_status_data* qos_status_out)
{
	return INVOKE_CLASS_MEMBER(0x0049B0E0, c_squad_session_qos_reply_manager, get_qos_status, qos_status_out);
}

bool c_group_session_qos_reply_manager::get_session_id(s_transport_secure_identifier* session_id_out)
{
	return INVOKE_CLASS_MEMBER(0x0049B100, c_group_session_qos_reply_manager, get_session_id, session_id_out);
}

bool c_squad_session_qos_reply_manager::get_session_id(s_transport_secure_identifier* session_id_out)
{
	return INVOKE_CLASS_MEMBER(0x0049B170, c_squad_session_qos_reply_manager, get_session_id, session_id_out);
}

void c_live_service_qos_manager::initialize()
{
	//INVOKE_CLASS_MEMBER(0x0049B1E0, c_live_service_qos_manager, initialize);

	m_flags.clear();
	m_live_service_qos_index = NONE;
	m_last_attempt_time = 0;
}

void c_session_qos_reply_manager::initialize()
{
	//INVOKE_CLASS_MEMBER(0x0049B200, c_session_qos_reply_manager, initialize);

	ASSERT(!m_initialized);
	m_initialized = true;
}

void __cdecl logic_qos_reply_manager_dispose()
{
	//INVOKE(0x0049B210, logic_qos_reply_manager_dispose);

	ASSERT(g_logic_qos_reply_manager_globals.initialized);
	g_logic_qos_reply_manager_globals.live_service_qos_manager.dispose();
	g_logic_qos_reply_manager_globals.squad_qos_reply_manager.dispose();
	g_logic_qos_reply_manager_globals.group_qos_reply_manager.dispose();
	g_logic_qos_reply_manager_globals.initialized = false;
}

void __cdecl logic_qos_reply_manager_initialize()
{
	//INVOKE(0x0049B250, logic_qos_reply_manager_initialize);

	ASSERT(!g_logic_qos_reply_manager_globals.initialized);
	g_logic_qos_reply_manager_globals.live_service_qos_manager.initialize();
	g_logic_qos_reply_manager_globals.squad_qos_reply_manager.initialize();
	g_logic_qos_reply_manager_globals.group_qos_reply_manager.initialize();
	g_logic_qos_reply_manager_globals.initialized = true;
}

void __cdecl logic_qos_reply_manager_update()
{
	//INVOKE(0x0049B290, logic_qos_reply_manager_update);

	ASSERT(g_logic_qos_reply_manager_globals.initialized);
	if (main_is_in_main_loop_pregame())
	{
		return;
	}
	g_logic_qos_reply_manager_globals.live_service_qos_manager.update();
	g_logic_qos_reply_manager_globals.squad_qos_reply_manager.update();
	g_logic_qos_reply_manager_globals.group_qos_reply_manager.update();
}

bool __cdecl network_qos_get_live_service_qos(s_transport_qos_result* live_service_qos_out)
{
	//return INVOKE(0x0049B2C0, network_qos_get_live_service_qos, live_service_qos_out);

	return g_logic_qos_reply_manager_globals.live_service_qos_manager.get_live_service_qos(live_service_qos_out);
}

bool __cdecl network_qos_original_qos_complete()
{
	//return INVOKE(0x0049B310, network_qos_original_qos_complete);

	return g_logic_qos_reply_manager_globals.live_service_qos_manager.original_qos_complete();
}

//.text:0049B320 ; network_time_difference_msec
//.text:0049B330 ; network_time_get
//.text:0049B350 ; network_time_get_exact
//.text:0049B360 ; network_time_since

void c_group_session_qos_reply_manager::notify_qos_reply_block_set()
{
	//INVOKE_CLASS_MEMBER(0x0049B380, c_group_session_qos_reply_manager, notify_qos_reply_block_set);

	network_life_cycle_notify_qos_reply_block_set_for_group();
}

void c_squad_session_qos_reply_manager::notify_qos_reply_block_set()
{
	//INVOKE_CLASS_MEMBER(0x0049B390, c_squad_session_qos_reply_manager, notify_qos_reply_block_set);
}

bool c_live_service_qos_manager::original_qos_complete() const
{
	//return INVOKE_CLASS_MEMBER(0x0049B3A0, c_live_service_qos_manager, original_qos_complete);

	return m_flags.test(_live_service_qos_manager_qos_completed_bit) || m_flags.test(_live_service_qos_manager_qos_failed_bit);
}

//.text:0049B3B0 ; public: void c_flags_no_init<e_live_service_qos_manager_flags, k_live_service_qos_manager_flags_count>::set(e_live_service_qos_manager_flags, bool)
//.text:0049B3D0 ; public: bool c_flags_no_init<e_live_service_qos_manager_flags, k_live_service_qos_manager_flags_count>::test(e_live_service_qos_manager_flags) const

void c_live_service_qos_manager::update()
{
	INVOKE_CLASS_MEMBER(0x0049B3F0, c_live_service_qos_manager, update);

	//if (!m_flags.test(_live_service_qos_manager_qos_in_progress_bit) &&
	//	!m_flags.test(_live_service_qos_manager_qos_completed_bit) &&
	//	c_live_service_qos_manager::connected_to_live())
	//{
	//	ASSERT(m_live_service_qos_index == NONE);
	//
	//	m_live_service_qos_index = transport_qos_service_new();
	//	m_last_attempt_time = network_time_get();
	//	if (m_live_service_qos_index == NONE)
	//	{
	//		event(_event_warning, "networking:logic:qos:live: failed to initiate qos to live");
	//		m_flags.set(_live_service_qos_manager_qos_failed_bit, true);
	//	}
	//	else
	//	{
	//		m_flags.set(_live_service_qos_manager_qos_in_progress_bit, true);
	//	}
	//}
	//
	//if (!m_flags.test(_live_service_qos_manager_qos_in_progress_bit))
	//{
	//	return;
	//}
	//
	//ASSERT(!m_flags.test(_live_service_qos_manager_qos_completed_bit));
	//ASSERT(m_live_service_qos_index != NONE);
	//
	//if (!transport_qos_is_complete(m_live_service_qos_index))
	//{
	//	return;
	//}
	//
	//if (!transport_qos_get_result(m_live_service_qos_index, 0, &m_live_service_qos))
	//{
	//	m_flags.set(_live_service_qos_manager_qos_failed_bit, true);
	//}
	//else if (g_network_configuration.transport.qos_upstream_cap_enabled)
	//{
	//	event(_event_message, "networking:logic:qos:live: probes %d/%d ping min/med %d/%d bw dn/up %.1f/%.1f NAT type %d",
	//		m_live_service_qos.probes_sent,
	//		m_live_service_qos.probes_received,
	//		m_live_service_qos.ping_msec_minimum,
	//		m_live_service_qos.ping_msec_median,
	//		(real32)m_live_service_qos.bandwidth_downstream_bps * 0.0009765625f, // 1 / 1024
	//		(real32)m_live_service_qos.bandwidth_upstream_bps * 0.0009765625f,   // 1 / 1024
	//		online_get_nat_type());
	//	user_interface_network_update_live_service_qos(&m_live_service_qos);
	//	m_flags.clear();
	//	m_flags.set(_live_service_qos_manager_qos_completed_bit, true);
	//}
	//
	//c_live_service_qos_manager::dispose();
}

void c_session_qos_reply_manager::update()
{
	INVOKE_CLASS_MEMBER(0x0049B570, c_session_qos_reply_manager, update);

	//ASSERT(m_initialized);
	//
	//s_transport_secure_identifier session_id{};
	//if (!get_session_id(&session_id))
	//{
	//	return;
	//}
	//
	//if (m_qos_listener_update_timestamp && network_time_since(m_qos_listener_update_timestamp) < g_network_configuration.logic.base_qos_reply_block.qos_listener_update_interval)
	//{
	//	return;
	//}
	//
	//bool clear_qos_status = true;
	//bool clear_reply_block = true;
	//if (c_session_qos_reply_manager::desires_qos_reply_block_set())
	//{
	//	s_network_squad_status_data qos_status{};
	//	csmemset(&qos_status, 0, sizeof(s_network_squad_status_data));
	//	if (!c_session_qos_reply_manager::get_qos_status(&qos_status))
	//	{
	//		event(_event_warning, "networking:logic:qos: failed to get qos status");
	//	}
	//	else
	//	{
	//		byte reply_block[0x800]{};
	//		int32 qos_reply_block = c_session_qos_reply_manager::get_qos_reply_block(&qos_status, sizeof(reply_block), reply_block);
	//		if (qos_reply_block > 0)
	//		{
	//			transport_qos_listener_set_reply_block(&session_id, true, qos_reply_block, reply_block);
	//			transport_qos_listener_set_bandwidth_limit(&session_id, true, get_qos_reply_bandwidth_limit());
	//			notify_qos_reply_block_set();
	//			clear_qos_status = false;
	//		}
	//		csmemcpy(&saved_qos_status, &qos_status, sizeof(s_network_squad_status_data));
	//		clear_reply_block = false;
	//	}
	//}
	//
	//if (clear_reply_block)
	//{
	//	transport_qos_listener_set_reply_block(&session_id, false, 0, nullptr);
	//	transport_qos_listener_set_bandwidth_limit(&session_id, false, g_network_configuration.logic.base_qos_reply_block.disabled_qos_reply_bandwidth_bps);
	//}
	//
	//if (clear_qos_status)
	//{
	//	csmemset(&saved_qos_status, 0, sizeof(s_network_squad_status_data));
	//}
	//
	//m_qos_listener_update_timestamp = network_time_get();
}

