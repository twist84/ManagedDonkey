#pragma once

#include "cseries/cseries.hpp"
#include "networking/tools/http_stream.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

enum e_upload_state
{
	_upload_state_none = 0,
	_upload_state_connecting,
	_upload_state_sending,
	_upload_state_receiving_header,
	_upload_state_receiving_content
};

class c_http_client
{
public:
	c_http_client();

	bool do_work(
		bool* upload_complete,
		char* out_response_content_buffer,
		int32* out_response_content_buffer_count,
		int32* out_http_response_code
	);

	uns32 get_ip_address();
	static void get_ip_address_string(int32 ipv4_address, c_static_string<16>* out_string);
	int32 get_upload_length();
	int32 get_upload_position();
	int32 get_upstream_quota();
	bool is_connected();
	bool is_started();

//private:
	bool parse_http_response(
		const char* buffer,
		int32 buffer_length,
		bool* out_completed_successfully,
		int32* http_header_size,
		int32* http_response_code,
		int32* content_length
	);
	bool __thiscall receive_data(
		bool* out_completed_successfully,
		char* out_response_content_buffer,
		int32* out_response_content_buffer_count,
		int32* out_http_response_code
	);

	bool send_data();
	void set_upstream_quota(int32 upstream_quota);

public:
	bool start(c_http_stream* stream, int32 ip_address, uns16 port, const char* url, bool endpoint_is_alpha);

private:
	bool start_connect();

public:
	bool stop();

private:
	static void transport_shutdown(void* client);

protected:
	transport_address m_address;
	c_static_string<16> m_ip_address_string;
	transport_endpoint m_endpoint;
	transport_endpoint* m_endpoint_ptr;
	c_http_stream* m_http_stream;
	char m_response_buffer[4096];
	int32 m_response_buffer_count;
	int32 m_bytes_remaining;
	int32 m_socket_count;
	bool m_started;
	uns64 m_start_time;
	uns64 m_previous_time;
	int32 m_upstream_quota;
	e_upload_state m_current_state;
};
COMPILE_ASSERT(sizeof(c_http_client) == 0x1060);

