#pragma once

#include "cseries/cseries.hpp"

class c_network_session;
struct s_network_session_parameter_update;
class c_network_session_parameter_base
{
public:
	bool get_allowed(void) const;
	bool set_allowed(void) const;
	const char* get_set_denied_reason(void) const;
	bool change_request_allowed(void) const;
	const char* get_change_request_denied_reason(void) const;
	//c_network_session_parameter_base(c_network_session_parameter_base& const);

protected:
	//c_network_session_parameter_base(void);
	void initialize(e_network_session_parameter_type parameter_type, const char* parameter_type_description, e_parameter_update_priority update_priority, e_parameter_change_request_access change_request_access, c_network_session* session);
	void build_update(int32 peer_index, s_network_session_parameter_update* update, int32 update_size) const;
	bool handle_update(const s_network_session_parameter_update* update, int32 update_size);
	int32 get_update_size(void) const;
	void build_change_request(int32 peer_index, void* payload, int32 payload_size) const;
	bool handle_change_request(const void* change_request, int32 change_request_size);
	int32 get_change_request_size(void) const;
	bool peer_requires_update(int32 peer_index) const;
	e_parameter_update_priority get_update_priority(void) const;
	e_parameter_change_request_access get_change_request_access(void) const;
	bool desires_change_request(void) const;
	const char* get_session_description(void) const;
	void clear(void);
	void set_update_required(void);
	void notify_update_sent_to_peer(int32 peer_index);
	void set_desires_change_request(void);
	void notify_change_request_sent(void);
	void reset_peer_state(int32 peer_index);
	void transition_state_to_become_host(void);

	//virtual void validate_buffer(int32 size, const void* original, void* temporary, void* buffer);
	virtual void notify_set_update_required(void);
	virtual void notify_set_change_request(void);
	virtual void notify_update_payload_sent_to_peer(int32 peer_index);
	virtual void notify_change_request_payload_sent(void);
	virtual bool can_set_peer_no_longer_requires_update(int32 peer_index);
	virtual bool can_clear_change_request_desired(void);
	virtual bool parameter_data_ready_for_consumption(void) const;
	virtual void get_update_payload(int32 peer_index, void* payload_out) const;// = 0;
	virtual int32 get_update_payload_size(void) const;// = 0;
	virtual bool handle_update_payload(const void* payload);// = 0;
	virtual void get_change_request_payload(int32 peer_index, void* payload_out) const;// = 0;
	virtual int32 get_change_request_payload_size(void) const;// = 0;
	virtual bool handle_change_request_payload(const void* payload);// = 0;
	virtual void notify_reset_peer_state(int32 peer_index);// = 0;
	virtual void notify_clear(void);// = 0;
	virtual void notify_transition_state_to_become_host(void);// = 0;

private:
	void set_peer_no_longer_requires_update(int32 peer_index);
	void set_change_request_no_longer_desired(void);

public:
	//c_network_session_parameter_base& operator=(c_network_session_parameter_base&);

protected:
	e_network_session_parameter_type m_parameter_type;
	const char* m_parameter_type_description;
	c_network_session* m_session;

private:
	e_parameter_update_priority m_update_priority;
	e_parameter_change_request_access m_change_request_access;
	c_static_array<bool, 17> m_transmitted_peer_updates;
	uns32 m_flags;
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_base) == 0x30);

struct s_network_session_parameter_update
{
	byte* payload;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_update) == sizeof(byte*));

