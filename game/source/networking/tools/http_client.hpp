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

struct c_http_client
{
public:
	c_http_client();

	bool do_work(
		bool* upload_complete,
		char* out_response_content_buffer,
		long* out_response_content_buffer_count,
		long* out_http_response_code
	);

	dword get_ip_address();
	static void get_ip_address_string(long ipv4_address, c_static_string<16>* out_string);
	long get_upload_length();
	long get_upload_position();
	long get_upstream_quota();
	bool is_connected();
	bool is_started();

private:
	bool parse_http_response(
		char const* buffer,
		long buffer_length,
		bool* out_completed_successfully,
		long* http_header_size,
		long* http_response_code,
		long* content_length
	);
	bool receive_data(
		bool* out_completed_successfully,
		char* out_response_content_buffer,
		long* out_response_content_buffer_count,
		long* out_http_response_code
	);

	bool send_data();
	void set_upstream_quota(long upstream_quota);

public:
	bool start(c_http_stream* stream, long ip_address, word port, char const* url, bool endpoint_is_alpha);

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
	long m_response_buffer_count;
	long m_bytes_remaining;
	long m_socket_count;
	bool m_started;
	qword __time1048;
	qword __time1050;
	long m_upstream_quota;
	e_upload_state m_current_state;
};
static_assert(sizeof(c_http_client) == 0x1060);

