#include "networking/http/http_server.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "networking/network_time.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"
#include "tag_files/files.hpp"

static s_http_server g_http_server{};

s_http_server* http_server_get()
{
	return &g_http_server;
}

// HTTP timeout constants
const uns32 k_http_client_timeout_ms = 30'000; // 30 seconds
const uns32 k_http_request_timeout_ms = 10'000; // 10 seconds

// Client connection management
static void http_client_init(s_http_client* client);
static void http_client_reset(s_http_client* client);
static void http_client_close(s_http_client* client);
static bool http_client_is_active(const s_http_client* client);
static bool http_client_has_timed_out(const s_http_client* client);

// Request/Response handling
static bool http_client_read_request(s_http_client* client);
static bool http_client_process_request(s_http_server* server, s_http_client* client);
static bool http_client_write_response(s_http_client* client);
static void http_client_handle_route(s_http_server* server, s_http_client* client);

// Default route handlers
static void http_route_handler_404(s_http_client* client, const s_http_request* request, s_http_response* response);
static void http_route_handler_500(s_http_client* client, const s_http_request* request, s_http_response* response);

// ============================================================================
// HTTP Server Implementation
// ============================================================================

bool http_server_initialize(uns16 port)
{
	s_http_server* server = http_server_get();
	ASSERT(server != NULL);

	csmemset(server, 0, sizeof(s_http_server));
	server->port = port;
	server->running = false;
	server->route_count = 0;

	for (int32 client_index = 0; client_index < s_http_server::k_max_clients; client_index++)
	{
		s_http_client* client = &server->clients[client_index];

		http_client_init(client);
	}

	bool success = false;

	server->server_endpoint = transport_endpoint_create(_transport_type_tcp);
	if (server->server_endpoint)
	{
		transport_address server_address{};
		transport_get_listen_address(&server_address, port);

		if (transport_endpoint_bind(server->server_endpoint, &server_address))
		{
			transport_endpoint_set_option_value(server->server_endpoint, _transport_endpoint_option_reuse_address, 1);

			if (transport_endpoint_listen(server->server_endpoint))
			{
				transport_endpoint_set_blocking(server->server_endpoint, false);
				
				switch (server_address.address_length)
				{
				case IPV4_ADDRESS_LENGTH:
				{
					event(_event_message, "networking:http_server: initialized on http://127.0.0.1:%d/", port);
					server->running = true;
				}
				break;
				case IPV6_ADDRESS_LENGTH:
				{
					event(_event_message, "networking:http_server: initialized on http://[::1]:%d/", port);
					server->running = true;
				}
				break;
				}
				success = server->running;
			}
			else
			{
				event(_event_error, "networking:http_server: failed to listen on port %d", port);
				transport_endpoint_delete(server->server_endpoint);
				server->server_endpoint = NULL;
			}
		}
		else
		{
			event(_event_error, "networking:http_server: failed to bind to port %d", port);
			transport_endpoint_delete(server->server_endpoint);
			server->server_endpoint = NULL;
		}
	}
	else
	{
		event(_event_error, "networking:http_server: failed to create server endpoint");
	}

	return success;
}

void http_server_dispose()
{
	s_http_server* server = http_server_get();
	ASSERT(server != NULL);

	if (server->server_endpoint)
	{
		event(_event_message, "networking:http_server: shutting down (port %d)", server->port);
		event(_event_message, "networking:http_server: stats - requests: %d, sent: %d bytes, received: %d bytes",
			server->total_requests,
			server->total_bytes_sent,
			server->total_bytes_received);

		for (int32 client_index = 0; client_index < s_http_server::k_max_clients; client_index++)
		{
			s_http_client* client = &server->clients[client_index];

			if (http_client_is_active(client))
			{
				http_client_close(client);
			}
		}

		transport_endpoint_disconnect(server->server_endpoint);
		transport_endpoint_delete(server->server_endpoint);
		server->server_endpoint = NULL;
	}

	server->route_count = 0;
	server->running = false;
}

void http_server_update()
{
	s_http_server* server = http_server_get();
	ASSERT(server != NULL);

	if (server->running && server->server_endpoint)
	{
		uns32 current_time = network_time_get();

		if (transport_endpoint_readable(server->server_endpoint))
		{
			transport_endpoint* client_endpoint = transport_endpoint_accept(server->server_endpoint);
			if (client_endpoint)
			{
				s_http_client* client = NULL;
				for (int32 client_index = 0; client_index < s_http_server::k_max_clients; client_index++)
				{
					if (!http_client_is_active(&server->clients[client_index]))
					{
						client = &server->clients[client_index];
						break;
					}
				}

				if (client)
				{
					http_client_reset(client);
					client->endpoint = client_endpoint;
					client->state = _http_client_state_reading_request;
					client->connection_time = current_time;
					client->last_activity_time = current_time;

					transport_get_endpoint_address(client_endpoint, &client->address);
					transport_endpoint_set_blocking(client_endpoint, false);

					event(_event_message, "networking:http_server: accepted connection from %s",
						transport_address_get_string(&client->address));
				}
				else
				{
					event(_event_warning, "networking:http_server: no available client slots, rejecting connection");
					transport_endpoint_disconnect(client_endpoint);
					transport_endpoint_delete(client_endpoint);
				}
			}
		}

		for (int32 client_index = 0; client_index < s_http_server::k_max_clients; client_index++)
		{
			s_http_client* client = &server->clients[client_index];

			if (!http_client_is_active(client))
			{
				continue;
			}

			if (http_client_has_timed_out(client))
			{
				event(_event_warning, "networking:http_server: client %s timed out",
					transport_address_get_string(&client->address));
				http_client_close(client);
				continue;
			}

			switch (client->state)
			{
			case _http_client_state_reading_request:
			{
				if (http_client_read_request(client))
				{
					client->state = _http_client_state_processing_request;
					client->last_activity_time = current_time;
				}
			}
			break;
			case _http_client_state_processing_request:
			{
				if (http_client_process_request(server, client))
				{
					client->state = _http_client_state_writing_response;
					client->last_activity_time = current_time;
					server->total_requests++;
				}
				else
				{
					client->state = _http_client_state_closing;
				}
			}
			break;
			case _http_client_state_writing_response:
			{
				if (http_client_write_response(client))
				{
					client->last_activity_time = current_time;

					if (client->request.keep_alive)
					{
						http_client_reset(client);
						client->state = _http_client_state_reading_request;
						client->last_activity_time = current_time;
					}
					else
					{
						client->state = _http_client_state_closing;
					}
				}
			}
			break;
			case _http_client_state_closing:
			{
				http_client_close(client);
			}
			break;
			}
		}
	}
}

bool http_server_register_route(const char* path, e_http_method method, void (*handler)(s_http_client*, const s_http_request*, s_http_response*))
{
	s_http_server* server = http_server_get();
	ASSERT(server != NULL);
	ASSERT(path != NULL);
	ASSERT(handler != NULL);

	bool success = false;

	if (server->route_count < s_http_server::k_max_routes)
	{
		s_http_route* route = &server->routes[server->route_count++];
		route->path = path;
		route->method = method;
		route->handler = handler;

		event(_event_message, "networking:http_server: registered route %s %s",
			http_method_to_string(method),
			path);

		success = true;
	}
	else
	{
		event(_event_error, "networking:http_server: cannot register route '%s', max routes reached",
			path);
	}

	return success;
}

// ============================================================================
// Client Connection Management
// ============================================================================

static void http_client_init(s_http_client* client)
{
	ASSERT(client != NULL);
	csmemset(client, 0, sizeof(s_http_client));
	client->state = _http_client_state_idle;
}

static void http_client_reset(s_http_client* client)
{
	ASSERT(client != NULL);

	transport_endpoint* saved_endpoint = client->endpoint;
	transport_address saved_address = client->address;
	uns32 saved_connection_time = client->connection_time;

	if (client->request.body)
	{
		system_free(client->request.body);
		client->request.body = NULL;
	}
	if (client->response.body)
	{
		system_free(client->response.body);
		client->response.body = NULL;
	}
	if (client->response.response_buffer)
	{
		system_free(client->response.response_buffer);
		client->response.response_buffer = NULL;
	}

	csmemset(client, 0, sizeof(s_http_client));

	client->endpoint = saved_endpoint;
	client->address = saved_address;
	client->connection_time = saved_connection_time;
	client->state = _http_client_state_idle;
}

static void http_client_close(s_http_client* client)
{
	ASSERT(client != NULL);

	if (client->endpoint)
	{
		transport_endpoint_disconnect(client->endpoint);
		transport_endpoint_delete(client->endpoint);
		client->endpoint = NULL;
	}

	http_client_reset(client);
}

static bool http_client_is_active(const s_http_client* client)
{
	return client && client->endpoint && client->state != _http_client_state_idle;
}

static bool http_client_has_timed_out(const s_http_client* client)
{
	uns32 current_time = network_time_get();
	uns32 idle_time = current_time - client->last_activity_time;

	if (client->state == _http_client_state_reading_request)
	{
		return idle_time > k_http_request_timeout_ms;
	}

	return idle_time > k_http_client_timeout_ms;
}

// ============================================================================
// Request Handling
// ============================================================================

static bool http_client_read_request(s_http_client* client)
{
	ASSERT(client != NULL);
	ASSERT(client->endpoint != NULL);

	bool success = false;

	if (client->request_complete)
	{
		success = true;
	}
	else
	{
		int16 available_space = sizeof(client->request_buffer) - client->request_bytes_received - 1;
		if (available_space > 0)
		{
			int16 bytes_read = transport_endpoint_read(
				client->endpoint,
				client->request_buffer + client->request_bytes_received,
				available_space);

			if (bytes_read > 0)
			{
				client->request_bytes_received += bytes_read;
				client->request_buffer[client->request_bytes_received] = '\0';
				client->last_activity_time = network_time_get();

				if (csstrstr(client->request_buffer, "\r\n\r\n"))
				{
					client->request_complete = true;
					success = true;
				}
			}
			else if (bytes_read == _transport_readwrite_error_would_block)
			{
				// No data available yet, continue waiting
			}
			else
			{
				// Error or connection closed
				event(_event_warning, "networking:http_server: error reading request");
			}
		}
		else
		{
			event(_event_warning, "networking:http_server: request buffer overflow");
		}
	}

	return success;
}

static bool http_client_process_request(s_http_server* server, s_http_client* client)
{
	ASSERT(server != NULL);
	ASSERT(client != NULL);

	bool success = false;

	if (http_request_parse(client->request_buffer, client->request_bytes_received, &client->request))
	{
		event(_event_message, "networking:http_server: %s %s from %s",
			http_method_to_string(client->request.method),
			client->request.uri,
			transport_address_get_string(&client->address));

		http_response_initialize(&client->response);

		http_client_handle_route(server, client);
		success = true;
	}
	else
	{
		event(_event_warning, "networking:http_server: failed to parse HTTP request");

		http_response_initialize(&client->response);
		http_response_set_status(&client->response, _http_status_bad_request);
		http_response_set_body(&client->response, "400 Bad Request", 15);
		http_response_build(&client->response);
		success = true;
	}
	return success;
}

static void http_client_handle_route(s_http_server* server, s_http_client* client)
{
	bool has_route = false;
	for (int32 route_index = 0; route_index < server->route_count; route_index++)
	{
		const s_http_route* route = &server->routes[route_index];

		if (route->method == client->request.method || route->method == _http_method_unknown)
		{
			if (csstrcmp(route->path, client->request.uri) == 0 || (csstrcmp(route->path, "*") == 0))
			{
				route->handler(client, &client->request, &client->response);
				http_response_build(&client->response);
				has_route = true;
				break;
			}
		}
	}

	if (!has_route)
	{
		http_route_handler_404(client, &client->request, &client->response);
		http_response_build(&client->response);
	}
}

static bool http_client_write_response(s_http_client* client)
{
	ASSERT(client != NULL);
	ASSERT(client->endpoint != NULL);

	bool success = false;

	if (client->response.response_buffer)
	{
		int16 bytes_to_send = (int16)(client->response.response_length - client->response.bytes_sent);
		if (bytes_to_send <= 0)
		{
			// Response fully sent
			success = true;
		}
		else
		{
			int16 bytes_written = transport_endpoint_write(
				client->endpoint,
				client->response.response_buffer + client->response.bytes_sent,
				bytes_to_send);

			if (bytes_written > 0)
			{
				client->response.bytes_sent += bytes_written;
				client->last_activity_time = network_time_get();

				if (client->response.bytes_sent >= client->response.response_length)
				{
					event(_event_message, "networking:http_server: sent %d bytes to %s",
						client->response.response_length,
						transport_address_get_string(&client->address));
					success = true;
				}
			}
			else if (bytes_written == _transport_readwrite_error_would_block)
			{
				// Socket buffer full, try again later
			}
			else
			{
				// Error
				event(_event_warning, "networking:http_server: error writing response");
			}
		}
	}
	else
	{
		event(_event_error, "networking:http_server: no response buffer to send");
	}

	return success;
}

// ============================================================================
// Default Route Handlers
// ============================================================================

static void http_route_handler_404(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	http_response_set_status(response, _http_status_not_found);
	http_response_set_content_type(response, "text/html");

	c_static_string<512> body;
	body.print(
		"<html>\r\n"
		"<head><title>404 Not Found</title></head>\r\n"
		"<body>\r\n"
		"<h1>404 - Not Found</h1>\r\n"
		"<p>The requested resource '%s' was not found on this server.</p>\r\n"
		"</body>\r\n"
		"</html>\r\n",
		request->uri);

	http_response_set_body(response, body.get_string(), body.length());
}

static void http_route_handler_500(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	http_response_set_status(response, _http_status_internal_server_error);
	http_response_set_content_type(response, "text/html");

	c_static_string<512> body;
	body.print(
		"<html>\r\n"
		"<body>\r\n"
		"<h1>500 Internal Server Error</h1>\r\n"
		"</body>\r\n"
		"</html>\r\n",
		request->uri);

	http_response_set_body(response, body.get_string(), body.length());
}

// ============================================================================
// HTTP Response Helpers
// ============================================================================

void http_response_initialize(s_http_response* response)
{
	ASSERT(response != NULL);
	csmemset(response, 0, sizeof(s_http_response));
	response->status_code = _http_status_ok;
	csstrnzcpy(response->content_type, "text/html", sizeof(response->content_type));
}

void http_response_dispose(s_http_response* response)
{
	ASSERT(response != NULL);

	if (response->body)
	{
		system_free(response->body);
		response->body = NULL;
	}

	if (response->response_buffer)
	{
		system_free(response->response_buffer);
		response->response_buffer = NULL;
	}
}

void http_response_set_status(s_http_response* response, e_http_status status)
{
	ASSERT(response != NULL);
	response->status_code = status;
}

void http_response_set_content_type(s_http_response* response, const char* content_type)
{
	ASSERT(response != NULL);
	ASSERT(content_type != NULL);
	csstrnzcpy(response->content_type, content_type, sizeof(response->content_type));
}

void http_response_set_body(s_http_response* response, const char* body, int32 length)
{
	ASSERT(response != NULL);
	ASSERT(body != NULL);

	if (response->body)
	{
		system_free(response->body);
		response->body = NULL;
	}

	response->body = (char*)system_malloc(length + 1);
	if (response->body)
	{
		csmemcpy(response->body, body, length);
		response->body[length] = '\0';
		response->body_length = length;
		response->body_capacity = length + 1;
	}
}

void http_response_append_body(s_http_response* response, const char* data, int32 length)
{
	ASSERT(response != NULL);
	ASSERT(data != NULL);

	bool has_new_body = false;

	int32 new_length = response->body_length + length;
	if (new_length >= response->body_capacity)
	{
		int32 new_capacity = new_length * 2;
		char* new_body = (char*)system_malloc(new_capacity);

		if (new_body)
		{
			if (response->body)
			{
				csmemcpy(new_body, response->body, response->body_length);
				system_free(response->body);
				response->body = NULL;
			}

			response->body = new_body;
			response->body_capacity = new_capacity;

			has_new_body = true;
		}
	}

	if (has_new_body)
	{
		csmemcpy(response->body + response->body_length, data, length);
		response->body_length = new_length;
		response->body[response->body_length] = '\0';
	}
}

void http_response_set_json(s_http_response* response, const char* json)
{
	http_response_set_content_type(response, "application/json");
	http_response_set_body(response, json, strlen_debug(json));
}

void http_response_set_file(s_http_response* response, const char* filepath)
{
	ASSERT(response != NULL);
	ASSERT(filepath != NULL);

	s_file_reference file{};
	if (file_reference_create_from_path(&file, filepath, false) && file_exists(&file))
	{
		uns32 file_size = 0;
		char* file_content = (char*)file_read_into_memory_with_null_padding(&file, &file_size, 16);
		if (file_content)
		{
			const char* mime_type = http_get_mime_type(filepath);
			http_response_set_content_type(response, mime_type);
			http_response_set_body(response, file_content, file_size);

			system_free(file_content);
			file_content = NULL;
		}
		else
		{
			http_response_set_status(response, _http_status_internal_server_error);
			http_response_set_body(response, "Failed to read file", 19);
		}
	}
	else
	{
		http_response_set_status(response, _http_status_not_found);
		http_response_set_body(response, "File not found", 14);
	}
}

bool http_response_build(s_http_response* response)
{
	ASSERT(response != NULL);

	bool success = false;

	if (response->response_buffer)
	{
		system_free(response->response_buffer);
		response->response_buffer = NULL;
	}

	c_static_string<512> headers;
	headers.print("HTTP/1.1 %d %s\r\n", response->status_code, http_status_to_string(response->status_code));
	headers.append_print("Content-Type: %s\r\n", response->content_type);
	headers.append_print("Content-Length: %d\r\n", response->body_length);
	headers.append_print("Connection: close\r\n");
	headers.append_print("Server: DonkeyHTTP/1.0\r\n");
	headers.append_print("\r\n");

	int32 total_length = headers.length() + response->body_length;
	response->response_buffer = (char*)system_malloc(total_length);

	if (response->response_buffer)
	{
		csmemcpy(response->response_buffer, headers.get_string(), headers.length());
		if (response->body && response->body_length > 0)
		{
			csmemcpy(response->response_buffer + headers.length(), response->body, response->body_length);
		}

		response->response_length = total_length;
		response->bytes_sent = 0;

		success = true;
	}
	return success;
}

// ============================================================================
// HTTP Request Helpers
// ============================================================================

bool http_request_parse(const char* buffer, int32 length, s_http_request* request)
{
	ASSERT(buffer != NULL);
	ASSERT(request != NULL);

	bool success = false;

	csmemset(request, 0, sizeof(s_http_request));
	request->content_length = NONE;

	// Parse request line
	char method_str[16];
	if (sscanf_s(buffer, "%15s %511s %15s",
		method_str, NUMBEROF(method_str),
		request->uri, NUMBEROF(request->uri),
		request->version, NUMBEROF(request->version)) == 3)
	{
		request->method = http_string_to_method(method_str);

		// Parse headers
		const char* line = buffer;
		while (line)
		{
			line = csstrstr(line, "\r\n");
			if (!line) break;
			line += 2;

			if (*line == '\r' || *line == '\n')
			{
				break; // End of headers
			}

			if (csstrnicmp(line, "Host:", 5) == 0)
			{
				sscanf_s(line + 5, " %127s", request->host, 128);
			}
			else if (csstrnicmp(line, "User-Agent:", 11) == 0)
			{
				sscanf_s(line + 11, " %127[^\r\n]", request->user_agent, 128);
			}
			else if (csstrnicmp(line, "Content-Type:", 13) == 0)
			{
				sscanf_s(line + 13, " %63[^\r\n]", request->content_type, 64);
			}
			else if (csstrnicmp(line, "Content-Length:", 15) == 0)
			{
				sscanf_s(line + 15, " %d", &request->content_length);
			}
			else if (csstrnicmp(line, "Connection:", 11) == 0)
			{
				char connection[32];
				sscanf_s(line + 11, " %31s", connection, NUMBEROF(connection));
				request->keep_alive = string_hash(connection) == "keep-alive"_hash;
			}
		}

		success = true;
	}
	return success;
}

// ============================================================================
// Utility Functions
// ============================================================================

const char* http_method_to_string(e_http_method method)
{
	const char* string = "UNKNOWN";
	switch (method)
	{
	case _http_method_get:     string = "GET";     break;
	case _http_method_post:    string = "POST";    break;
	case _http_method_put:     string = "PUT";     break;
	case _http_method_delete:  string = "DELETE";  break;
	case _http_method_head:    string = "HEAD";    break;
	case _http_method_options: string = "OPTIONS"; break;
	}
	return string;
}

e_http_method http_string_to_method(const char* method_str)
{
	e_http_method method = _http_method_unknown;
	switch (string_hash(method_str))
	{
	case "GET"_hash:     method = _http_method_get;     break;
	case "POST"_hash:    method = _http_method_post;    break;
	case "PUT"_hash:     method = _http_method_put;     break;
	case "DELETE"_hash:  method = _http_method_delete;  break;
	case "HEAD"_hash:    method = _http_method_head;    break;
	case "OPTIONS"_hash: method = _http_method_options; break;
	}
	return method;
}

const char* http_status_to_string(e_http_status status)
{
	const char* string = "Unknown";
	switch (status)
	{
	case _http_status_ok:                    string = "OK";                    break;
	case _http_status_created:               string = "Created";               break;
	case _http_status_no_content:            string = "No Content";            break;
	case _http_status_moved_permanently:     string = "Moved Permanently";     break;
	case _http_status_found:                 string = "Found";                 break;
	case _http_status_not_modified:          string = "Not Modified";          break;
	case _http_status_bad_request:           string = "Bad Request";           break;
	case _http_status_unauthorized:          string = "Unauthorized";          break;
	case _http_status_forbidden:             string = "Forbidden";             break;
	case _http_status_not_found:             string = "Not Found";             break;
	case _http_status_method_not_allowed:    string = "Method Not Allowed";    break;
	case _http_status_request_timeout:       string = "Request Timeout";       break;
	case _http_status_payload_too_large:     string = "Payload Too Large";     break;
	case _http_status_uri_too_long:          string = "URI Too Long";          break;
	case _http_status_internal_server_error: string = "Internal Server Error"; break;
	case _http_status_not_implemented:       string = "Not Implemented";       break;
	case _http_status_service_unavailable:   string = "Service Unavailable";   break;
	}
	return string;
}

const char* http_get_content_type_for_extension(const char* extension)
{
	const char* content_type = "application/octet-stream";
	if (extension)
	{
		switch (string_hash(extension))
		{
		case ".html"_hash: content_type = "text/html";              break;
		case ".htm"_hash:  content_type = "text/html";              break;
		case ".css"_hash:  content_type = "text/css";               break;
		case ".js"_hash:   content_type = "application/javascript"; break;
		case ".json"_hash: content_type = "application/json";       break;
		case ".xml"_hash:  content_type = "application/xml";        break;
		case ".txt"_hash:  content_type = "text/plain";             break;
		case ".png"_hash:  content_type = "image/png";              break;
		case ".jpg"_hash:  content_type = "image/jpeg";             break;
		case ".jpeg"_hash: content_type = "image/jpeg";             break;
		case ".gif"_hash:  content_type = "image/gif";              break;
		case ".svg"_hash:  content_type = "image/svg+xml";          break;
		case ".ico"_hash:  content_type = "image/x-icon";           break;
		case ".pdf"_hash:  content_type = "application/pdf";        break;
		case ".zip"_hash:  content_type = "application/zip";        break;
		}
	}
	return content_type;
}

const char* http_get_mime_type(const char* filepath)
{
	const char* extension = strrchr(filepath, '.');
	return http_get_content_type_for_extension(extension);
}

