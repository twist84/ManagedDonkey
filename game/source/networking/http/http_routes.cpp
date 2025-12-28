#include "networking/http/http_routes.hpp"

#include "networking/http/http_server.hpp"
#include "networking/http/http_static_strings.hpp"
#include "networking/network_globals.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "tag_files/files.hpp"

// ============================================================================
// HTTP Route Handlers
// ============================================================================
void http_route_index(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	// Try to serve index.html from bin directory
	s_file_reference index_file{};
	if (file_reference_create_from_path(&index_file, "bin/index.html", false) && file_exists(&index_file))
	{
		http_response_set_file(response, "bin/index.html");
	}
	else
	{
		// Build fallback page using HTML builder
		c_html_static_string<4096> html;

		html.begin_html();
		{
			html.head_begin();
			{
				html.meta_charset("UTF-8");
				html.meta_viewport();
				html.title("Donkey Server");

				html.style_begin();
				{
					html.raw("body { font-family: Arial, sans-serif; margin: 40px; background: #1a1a1a; color: #fff; }");
					html.raw(".container { max-width: 800px; margin: 0 auto; }");
					html.raw("h1 { color: #00ff00; }");
					html.raw(".endpoint { background: #2a2a2a; padding: 20px; margin: 10px 0; border-radius: 5px; }");
					html.raw(".endpoint h3 { margin-top: 0; color: #00ccff; }");
					html.raw("a { color: #00ccff; text-decoration: none; }");
					html.raw("a:hover { text-decoration: underline; }");
				}
				html.style_end();
			}
			html.head_end();

			html.body_begin();
			{
				html.div_begin();
				{
					html.css_class("container");

					html.h1("Donkey Dedicated Server");
					html.p("Server is running and accepting connections.");

					html.div_begin();
					{
						html.css_class("endpoint");
						html.h3("GET /session");
						html.p("Returns current session information as JSON");
						html.a("/session", "View session info");
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("endpoint");
						html.h3("GET /api/ping");
						html.p("Simple ping endpoint for testing");
						html.a("/api/ping", "Ping server");
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("endpoint");
						html.h3("GET /dashboard");
						html.p("Server statistics and monitoring dashboard");
						html.a("/dashboard", "Open dashboard");
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("endpoint");
						html.h3("GET /api/players");
						html.p("List of active players");
						html.a("/api/players", "View players");
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("endpoint");
						html.h3("GET /api/game-state");
						html.p("Complete game state information");
						html.a("/api/game-state", "View game state");
					}
					html.div_end();
				}
				html.div_end();
			}
			html.body_end();
		}
		html.end_html();

		http_response_set_content_type(response, "text/html");
		http_response_set_body(response, html.get_string(), html.length());
	}
}
void http_route_api_players(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	c_json_static_string<4096> json;

	json.build_root_object();
	{
		json.begin_array("players");

		// Iterate through active players
		for (int32 player_index = 0; player_index < 4; player_index++)
		{
			// Get player data (pseudo-code, adjust for your actual API)
			// const s_player_data* player = get_player_data(player_index);
			// if (!player || !player->active) continue;

			json.begin_object();
			{
				json.add_integer("index", player_index);

				c_static_string<64> player_name;
				player_name.print("Player%d", player_index);
				json.add_string("name", player_name.get_string());

				json.add_integer("score", player_index * 100);
				json.add_integer("kills", player_index * 5);
				json.add_integer("deaths", player_index * 3);
			}
			json.end_object();
		}

		json.end_array();
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}
void http_route_api_map(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	c_json_static_string<512> json;

	json.build_root_object();
	{
		json.add_string("current_map", "sandtrap");
		json.add_string("game_mode", "slayer");
		json.add_integer("time_remaining", 600);
		json.add_integer("score_limit", 50);
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}
void http_route_api_game_state(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	c_json_static_string<4096> json;

	json.build_root_object();
	{
		json.begin_object("server");
		{
			json.add_string("name", "My Donkey Server");
			json.add_string("version", "1.0");
			json.add_unsigned("uptime", network_time_get());
		}
		json.end_object();

		json.begin_object("game");
		{
			json.add_string("map", "sandtrap");
			json.add_string("mode", "slayer");
			json.add_string("state", "in_progress");
			json.add_integer("time_elapsed", 300);
			json.add_integer("time_remaining", 300);
		}
		json.end_object();

		json.begin_array("teams");
		{
			json.begin_object();
			{
				json.add_string("name", "Red");
				json.add_integer("score", 25);
				json.add_integer("players", 4);
			}
			json.end_object();

			json.begin_object();
			{
				json.add_string("name", "Blue");
				json.add_integer("score", 30);
				json.add_integer("players", 4);
			}
			json.end_object();
		}
		json.end_array();
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_api_events(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	c_json_static_string<2048> json;

	json.build_root_object();
	{
		json.begin_array("events");
		{
			// Add recent events
			json.begin_object();
			{
				json.add_string("type", "player_join");
				json.add_string("player", "Player1");
				json.add_unsigned("timestamp", network_time_get() - 5000);
			}
			json.end_object();

			json.begin_object();
			{
				json.add_string("type", "kill");
				json.add_string("killer", "Player1");
				json.add_string("victim", "Player2");
				json.add_unsigned("timestamp", network_time_get() - 1000);
			}
			json.end_object();
		}

		json.end_array();
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_api_kick_player(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	if (request->method != _http_method_post)
	{
		c_json_static_string<256> json;

		json.build_root_object();
		{
			json.add_string("error", "Method not allowed");
		}
		json.finalize_root_object();

		http_response_set_status(response, _http_status_method_not_allowed);
		http_response_set_json(response, json.get_string());
	}
	else
	{
		// Parse request body (JSON) - simplified example
		// In real implementation, you'd parse the JSON body properly

		c_json_static_string<256> json;

		json.build_root_object();
		{
			json.add_string("status", "success");
			json.add_string("message", "Player kicked");
		}
		json.finalize_root_object();

		http_response_set_json(response, json.get_string());
	}
}

void http_route_api_change_map(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	if (request->method != _http_method_post)
	{
		c_json_static_string<256> json;

		json.build_root_object();
		{
			json.add_string("error", "Method not allowed");
		}
		json.finalize_root_object();

		http_response_set_status(response, _http_status_method_not_allowed);
		http_response_set_json(response, json.get_string());
	}
	else
	{
		// Example: Change map
		// network_test_set_map_name("levels\\multi\\sandtrap\\sandtrap");

		c_json_static_string<256> json;

		json.build_root_object();
		{
			json.add_string("status", "success");
			json.add_string("new_map", "sandtrap");
		}
		json.finalize_root_object();

		http_response_set_json(response, json.get_string());
	}
}

void http_route_dashboard(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	c_html_static_string<8192> html;

	html.begin_html();
	{
		html.head_begin();
		{
			html.meta_charset("UTF-8");
			html.meta_viewport();
			html.title("Server Dashboard");

			html.style_begin();
			{
				html.raw("body { font-family: Arial, sans-serif; margin: 0; background: #1a1a1a; color: #fff; }");
				html.raw(".header { background: #2a2a2a; padding: 20px; border-bottom: 2px solid #00ff00; }");
				html.raw(".container { max-width: 1200px; margin: 0 auto; padding: 20px; }");
				html.raw(".stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; margin: 20px 0; }");
				html.raw(".stat-card { background: #2a2a2a; padding: 20px; border-radius: 5px; border-left: 4px solid #00ff00; }");
				html.raw(".stat-value { font-size: 36px; font-weight: bold; color: #00ff00; }");
				html.raw(".stat-label { font-size: 14px; color: #aaa; margin-top: 5px; }");
				html.raw(".players { margin-top: 30px; }");
				html.raw("table { width: 100%; background: #2a2a2a; border-radius: 5px; overflow: hidden; }");
				html.raw("th { background: #3a3a3a; padding: 12px; text-align: left; }");
				html.raw("td { padding: 12px; border-top: 1px solid #3a3a3a; }");
				html.raw(".refresh { background: #00ff00; color: #000; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }");
			}
			html.style_end();

			html.script_begin();
			{
				html.raw("function refreshStats() {");
				html.raw("  fetch('/api/game-state')");
				html.raw("    .then(r => r.json())");
				html.raw("    .then(data => { console.log(data); });");
				html.raw("}");
				html.raw("setInterval(refreshStats, 5000);");
			}
			html.script_end();
		}
		html.head_end();

		html.body_begin();
		{
			html.div_begin();
			{
				html.css_class("header");

				html.div_begin();
				{
					html.css_class("container");
					html.h1("Donkey Server Dashboard");

					html.button_begin();
					{
						html.css_class("refresh");
						html.attr("onclick", "refreshStats()");
						html.text("Refresh");
					}
					html.button_end();
				}
				html.div_end();
			}
			html.div_end();

			html.div_begin();
			{
				html.css_class("container");

				html.div_begin();
				{
					html.css_class("stats");

					html.div_begin();
					{
						html.css_class("stat-card");

						html.div_begin();
						{
							html.css_class("stat-value");
							html.text("8");
						}
						html.div_end();

						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Active Players");
						}
						html.div_end();
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("stat-card");

						html.div_begin();
						{
							html.css_class("stat-value");
							html.text("sandtrap");
						}
						html.div_end();

						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Current Map");
						}
						html.div_end();
					}
					html.div_end();

					html.div_begin();
					{
						html.css_class("stat-card");

						html.div_begin();
						{
							html.css_class("stat-value");
							html.text("5:00");
						}
						html.div_end();

						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Time Remaining");
						}
						html.div_end();
					}
					html.div_end();
				}
				html.div_end();

				html.div_begin();
				{
					html.css_class("players");
					html.h2("Active Players");

					html.table_begin();
					{
						html.thead_begin();
						{
							html.tr_begin();
							{
								html.th("Name");
								html.th("Team");
								html.th("Score");
								html.th("K/D");
							}
							html.tr_end();
						}
						html.thead_end();

						html.tbody_begin();
						{
							html.attr("id", "players");

							html.tr_begin();
							{
								html.td("Player1");
								html.td("Red");
								html.td("1500");
								html.td("15/3");
							}
							html.tr_end();

							html.tr_begin();
							{
								html.td("Player2");
								html.td("Blue");
								html.td("1200");
								html.td("12/5");
							}
							html.tr_end();
						}
						html.tbody_end();
					}
					html.table_end();
				}
				html.div_end();
			}
			html.div_end();
		}
		html.body_end();
	}
	html.end_html();

	http_response_set_content_type(response, "text/html");
	http_response_set_body(response, html.get_string(), html.length());
}

void http_route_session_info(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	s_http_server* server = http_server_get();
	ASSERT(server != NULL);

	c_json_static_string<2048> json;
	json.clear();

	json.build_root_object();
	{
		json.add_string("server", "Donkey Dedicated Server");
		json.add_string("version", "donkey cache debug pc 1.106708 cert_ms23 " __DATE__ " " __TIME__);
		json.add_integer("game_port", g_broadcast_port);
		json.add_integer("http_port", server->port);
		json.add_unsigned("uptime_ms", network_time_get());
		json.add_boolean("initialized", network_globals.initialized);

		if (g_network_session_manager)
		{
			json.add_boolean("has_session_manager", true);
			// Add more session-specific info here if needed
		}
		else
		{
			json.add_boolean("has_session_manager", false);
		}

		json.begin_object("stats");
		{
			json.add_unsigned("total_requests", server->total_requests);
			json.add_unsigned("bytes_sent", server->total_bytes_sent);
			json.add_unsigned("bytes_received", server->total_bytes_received);
		}
		json.end_object();
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_api_ping(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	c_json_static_string<512> json;
	json.clear();

	json.build_root_object();
	{
		json.add_string("status", "ok");
		json.add_unsigned("timestamp", network_time_get());
		json.add_string("client_address", transport_address_get_string(&client->address));
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_static_files(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != NULL);
	ASSERT(request != NULL);
	ASSERT(response != NULL);

	// Serve static files from bin/ directory
	// Security: prevent directory traversal
	if (strstr(request->uri, "..") || strstr(request->uri, "\\"))
	{
		http_response_set_status(response, _http_status_forbidden);
		http_response_set_content_type(response, "text/html");

		c_html_static_string<512> html;
		{
			html.begin_html();
			{
				html.head_begin();
				{
					html.meta_charset("UTF-8");
					html.title("403 Forbidden");
				}
				html.head_end();

				html.body_begin();
				{
					html.h1("403 - Forbidden");
					html.p("Access to this resource is forbidden.");
				}
				html.body_end();
			}
			html.end_html();
		}

		http_response_set_body(response, html.get_string(), html.length());
	}
	else
	{
		c_static_string<512> filepath;
		filepath.print("bin%s", request->uri);

		s_file_reference file{};
		if (file_reference_create_from_path(&file, filepath.get_string(), false) && file_exists(&file))
		{
			http_response_set_file(response, filepath.get_string());
		}
		else
		{
			// File not found
			http_response_set_status(response, _http_status_not_found);
			http_response_set_content_type(response, "text/html");

			c_html_static_string<1024> html;
			{
				html.begin_html();
				html.head_begin();
				{
					html.meta_charset("UTF-8");
					html.title("404 Not Found");
					html.style_begin();
					html.raw("body { font-family: Arial, sans-serif; margin: 40px; background: #1a1a1a; color: #fff; }");
					html.raw("h1 { color: #ff0000; }");
					html.style_end();
				}
				html.head_end();

				html.body_begin();
				{
					html.h1("404 - Not Found");
					html.p_begin();
					html.text("The requested resource '");
					html.strong(request->uri);
					html.text("' was not found.");
					html.p_end();
					html.a("/", "Return to home");
				}
				html.body_end();
				html.end_html();
			}

			http_response_set_body(response, html.get_string(), html.length());
		}
	}
}

