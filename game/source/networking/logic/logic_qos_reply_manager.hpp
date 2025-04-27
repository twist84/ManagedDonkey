#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/transport/transport_qos.hpp"

struct c_live_service_qos_manager
{
	s_transport_qos_result m_qos_result;
	uint8 m_flags;
	int32 m_live_service_qos_index;
	int32 __unknown28;
};
static_assert(sizeof(c_live_service_qos_manager) == 0x2C);

struct c_session_qos_reply_manager
{
protected:
	virtual bool get_session_id(s_transport_secure_identifier* session_id);
	virtual bool desires_qos_reply_block_set();
	virtual bool get_qos_status(s_network_session_status_data* status);
	virtual int32 get_qos_reply_bandwidth_limit();
	virtual void notify_qos_reply_block_set();

	uint8 __data4[0x4];
	bool m_initialized;
	uint32 m_time;
	s_network_session_status_data game_status;
};
static_assert(sizeof(c_session_qos_reply_manager) == 0x164D0);

struct c_squad_session_qos_reply_manager :
	c_session_qos_reply_manager
{
};
static_assert(sizeof(c_squad_session_qos_reply_manager) == sizeof(c_session_qos_reply_manager));

struct c_group_session_qos_reply_manager :
	c_session_qos_reply_manager
{
};
static_assert(sizeof(c_group_session_qos_reply_manager) == sizeof(c_session_qos_reply_manager));

struct s_logic_qos_reply_manager_globals
{
	bool initialized;
	c_live_service_qos_manager live_service;
	c_squad_session_qos_reply_manager squad_session;
	c_group_session_qos_reply_manager group_session;
};
static_assert(sizeof(s_logic_qos_reply_manager_globals) == 0x2C9D0);

extern s_logic_qos_reply_manager_globals& logic_qos_reply_manager_globals;
