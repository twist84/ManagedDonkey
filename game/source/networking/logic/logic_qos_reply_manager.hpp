#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/transport/transport_qos.hpp"

enum e_live_service_qos_manager_flags
{
	_live_service_qos_manager_qos_in_progress_bit = 0,
	_live_service_qos_manager_qos_completed_bit,
	_live_service_qos_manager_qos_failed_bit,

	k_live_service_qos_manager_flags_count,
};
typedef c_flags<e_live_service_qos_manager_flags, uns8, k_live_service_qos_manager_flags_count> c_live_service_qos_manager_flags;

class c_live_service_qos_manager
{
public:
	c_live_service_qos_manager();

private:
	static bool __cdecl connected_to_live();

public:
	void dispose();
	bool get_live_service_qos(s_transport_qos_result* live_service_qos_out) const;
	void initialize();
	bool original_qos_complete() const;
	void update();

private:
	s_transport_qos_result m_live_service_qos;
	c_live_service_qos_manager_flags m_flags;
	int32 m_live_service_qos_index;
	uns32 m_last_attempt_time;
};
COMPILE_ASSERT(sizeof(c_live_service_qos_manager) == 0x2C);

class c_session_qos_reply_manager
{
protected:
	virtual bool get_session_id(s_transport_secure_identifier* session_id_out) = 0;
	virtual bool desires_qos_reply_block_set() = 0;
	virtual bool get_qos_status(s_network_squad_status_data* qos_status_out) = 0;
	virtual int32 get_qos_reply_bandwidth_limit() = 0;
	virtual void notify_qos_reply_block_set() = 0;

public:
	c_session_qos_reply_manager();
	void dispose();
	int32 get_qos_reply_block(const s_network_squad_status_data* qos_status, int32 max_block_size, byte* block_data);
	void initialize();
	void update();

private:
	byte __data4[0x4];
	bool m_initialized;
	uns32 m_qos_listener_update_timestamp;
	s_network_squad_status_data saved_qos_status;
};
COMPILE_ASSERT(sizeof(c_session_qos_reply_manager) == 0x164D0);

class c_squad_session_qos_reply_manager :
	public c_session_qos_reply_manager
{
protected:
	virtual bool get_session_id(s_transport_secure_identifier* session_id_out) override;
	virtual bool desires_qos_reply_block_set() override;
	virtual bool get_qos_status(s_network_squad_status_data* qos_status_out) override;
	virtual int32 get_qos_reply_bandwidth_limit() override;
	virtual void notify_qos_reply_block_set() override;

public:
	c_squad_session_qos_reply_manager();
};
COMPILE_ASSERT(sizeof(c_squad_session_qos_reply_manager) == sizeof(c_session_qos_reply_manager));

class c_group_session_qos_reply_manager :
	public c_session_qos_reply_manager
{
protected:
	virtual bool get_session_id(s_transport_secure_identifier* session_id_out) override;
	virtual bool desires_qos_reply_block_set() override;
	virtual bool get_qos_status(s_network_squad_status_data* qos_status_out) override;
	virtual int32 get_qos_reply_bandwidth_limit() override;
	virtual void notify_qos_reply_block_set() override;

public:
	c_group_session_qos_reply_manager();
};
COMPILE_ASSERT(sizeof(c_group_session_qos_reply_manager) == sizeof(c_session_qos_reply_manager));

struct s_logic_qos_reply_manager_globals
{
	bool initialized;
	c_live_service_qos_manager live_service_qos_manager;
	c_squad_session_qos_reply_manager squad_qos_reply_manager;
	c_group_session_qos_reply_manager group_qos_reply_manager;
};
COMPILE_ASSERT(sizeof(s_logic_qos_reply_manager_globals) == 0x2C9D0);

extern s_logic_qos_reply_manager_globals& g_logic_qos_reply_manager_globals;

extern void __cdecl logic_qos_reply_manager_dispose();
extern void __cdecl logic_qos_reply_manager_initialize();
extern void __cdecl logic_qos_reply_manager_update();
extern bool __cdecl network_qos_get_live_service_qos(s_transport_qos_result* live_service_qos_out);
extern bool __cdecl network_qos_original_qos_complete();

