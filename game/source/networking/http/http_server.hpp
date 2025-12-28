#pragma once

#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

enum e_http_method
{
	_http_method_get = 0,
	_http_method_post,
	_http_method_put,
	_http_method_delete,
	_http_method_head,
	_http_method_options,

	k_http_method_count,
	_http_method_unknown = k_http_method_count
};

enum e_http_status
{
	_http_status_ok = 200,
	_http_status_created = 201,
	_http_status_no_content = 204,
	_http_status_moved_permanently = 301,
	_http_status_found = 302,
	_http_status_not_modified = 304,
	_http_status_bad_request = 400,
	_http_status_unauthorized = 401,
	_http_status_forbidden = 403,
	_http_status_not_found = 404,
	_http_status_method_not_allowed = 405,
	_http_status_request_timeout = 408,
	_http_status_payload_too_large = 413,
	_http_status_uri_too_long = 414,
	_http_status_internal_server_error = 500,
	_http_status_not_implemented = 501,
	_http_status_service_unavailable = 503
};

enum e_http_client_state
{
	_http_client_state_idle = 0,
	_http_client_state_reading_request,
	_http_client_state_processing_request,
	_http_client_state_writing_response,
	_http_client_state_closing
};

struct s_http_request
{
	e_http_method method;
	char uri[512];
	char version[16];

	// Common headers
	char host[128];
	char user_agent[128];
	char content_type[64];
	int32 content_length;
	bool keep_alive;

	// Request body (for POST/PUT)
	char* body;
	int32 body_length;
	int32 body_capacity;
};

struct s_http_response
{
	e_http_status status_code;
	char content_type[64];

	char* body;
	int32 body_length;
	int32 body_capacity;

	// Full response buffer (headers + body)
	char* response_buffer;
	int32 response_length;
	int32 bytes_sent;
};

struct s_http_client
{
	transport_endpoint* endpoint;
	transport_address address;

	e_http_client_state state;
	uns32 connection_time;
	uns32 last_activity_time;

	// Request buffer
	char request_buffer[8192];
	int16 request_bytes_received;
	bool request_complete;

	// Parsed request
	s_http_request request;

	// Response
	s_http_response response;
};

struct s_http_route
{
	const char* path;
	e_http_method method;
	void (*handler)(s_http_client* client, const s_http_request* request, s_http_response* response);
};

struct s_http_server
{
	transport_endpoint* server_endpoint;
	uns16 port;
	bool running;

	// Client connection pool
	static const int32 k_max_clients = 32;
	s_http_client clients[k_max_clients];

	// Route handlers
	static const int32 k_max_routes = 64;
	s_http_route routes[k_max_routes];
	int32 route_count;

	// Statistics
	uns32 total_requests;
	uns32 total_bytes_sent;
	uns32 total_bytes_received;
};

// HTTP Server API
s_http_server* http_server_get();
extern bool http_server_initialize(uns16 port);
extern void http_server_dispose();
extern void http_server_update();
extern bool http_server_register_route(const char* path, e_http_method method, void (*handler)(s_http_client*, const s_http_request*, s_http_response*));

// HTTP Response Helpers
extern void http_response_initialize(s_http_response* response);
extern void http_response_dispose(s_http_response* response);
extern void http_response_set_status(s_http_response* response, e_http_status status);
extern void http_response_set_content_type(s_http_response* response, const char* content_type);
extern void http_response_set_body(s_http_response* response, const char* body, int32 length);
extern void http_response_append_body(s_http_response* response, const char* data, int32 length);
extern void http_response_set_json(s_http_response* response, const char* json);
extern void http_response_set_file(s_http_response* response, const char* filepath);
extern bool http_response_build(s_http_response* response);

// HTTP Request Helpers
extern const char* http_request_get_header(const s_http_request* request, const char* header_name);
extern bool http_request_parse(const char* buffer, int32 length, s_http_request* request);

// Utility functions
extern const char* http_method_to_string(e_http_method method);
extern e_http_method http_string_to_method(const char* method_str);
extern const char* http_status_to_string(e_http_status status);
extern const char* http_get_content_type_for_extension(const char* extension);
extern const char* http_get_mime_type(const char* filepath);

