#pragma once

#include "cseries/cseries.hpp"

struct c_network_session;
struct s_network_session_parameter_update;
struct c_network_session_parameter_base
{
protected:
	virtual void notify_set_update_required(void);
	virtual void notify_set_change_request(void);
	virtual void notify_update_payload_sent_to_peer(long);
	virtual void notify_change_request_payload_sent(void);
	virtual bool can_set_peer_no_longer_requires_update(long);
	virtual bool can_clear_change_request_desired(void);
	virtual bool parameter_data_ready_for_consumption(void) const;

private:
	virtual void get_update_payload(long, void*) const;
	virtual long get_update_payload_size(void) const;
	virtual bool handle_update_payload(void const*);
	virtual void get_change_request_payload(long, void*) const;
	virtual long get_change_request_payload_size(void) const;
	virtual bool handle_change_request_payload(void const*);

protected:
	virtual void notify_reset_peer_state(long);
	virtual void notify_clear(void);
	virtual void notify_transition_state_to_become_host(void);

	long m_type;
	char const* m_name;
	c_network_session* m_session;
	long m_update_priority;
	long m_change_request_access;

	// c_static_array<bool, 17> m_transmitted_peer_updates;
	bool m_transmitted_peer_updates[17];

	dword_flags m_state_flags;

	char const* get_session_description() const;
	void set_desires_change_request();
	void reset_peer_state(long peer_index);
	void notify_update_sent_to_peer(long peer_index);
	void set_update_required();
	void transition_state_to_become_host();
	void notify_change_request_sent();
	bool handle_change_request(void const* change_request, long change_request_size);
	long get_change_request_size() const;
	bool handle_update(s_network_session_parameter_update const* update, long update_size);
	long get_update_size() const;

private:
	void set_change_request_no_longer_desired();
	void set_peer_no_longer_requires_update(long peer_index);

public:
	bool change_request_allowed() const;
	bool get_allowed() const;
	char const* get_change_request_denied_reason() const;
	char const* get_set_denied_reason() const;
	bool set_allowed() const;
};
static_assert(sizeof(c_network_session_parameter_base) == 0x30);

const long k_network_session_parameters_update_payload_size = 0x20000;
const long k_network_session_parameters_request_payload_size = 0x20000;

struct s_network_session_parameter_update
{
	byte payload[k_network_session_parameters_update_payload_size];
};
static_assert(sizeof(s_network_session_parameter_update) == 0x20000);