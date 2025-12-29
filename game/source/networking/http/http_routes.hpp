#pragma once

struct s_http_client;
struct s_http_request;
struct s_http_response;

// Route handler declarations
void http_route_index(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_session_info(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_ping(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_players(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_map(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_game_state(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_events(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_kick_player(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_api_change_map(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_dashboard(s_http_client* client, const s_http_request* request, s_http_response* response);
void http_route_fallback(s_http_client* client, const s_http_request* request, s_http_response* response);

