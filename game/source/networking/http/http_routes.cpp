#include "networking/http/http_routes.hpp"

#include "config/version.hpp"
#include "game/game_engine_team.hpp"
#include "memory/thread_local.hpp"
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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

	c_json_static_string<4096> json;

	json.build_root_object();
	{
		if (game_engine_running())
		{
			s_game_engine_score_list new_list{};
			game_engine_scoring_build_score_list(&new_list, _game_engine_scoring_type_in_round, false);
			if (new_list.player_count > 0)
			{
				ASSERT(VALID_COUNT(new_list.player_count, NUMBEROF(new_list.player_indices)));

				json.begin_array("players");
				for (int32 player_num = 0; player_num < new_list.player_count; player_num++)
				{
					int32 player_index = new_list.player_indices[player_num];
					if (player_index == NONE)
					{
						continue;
					}

					const player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);

					json.begin_object();
					{
						json.add_integer("index", player_index);

						c_static_string<32> player_name;
						wchar_string_to_ascii_string(player->configuration.host.name, player_name.get_buffer(), player_name.element_count, nullptr);
						json.add_string("name", player_name.get_string());

						json.add_integer("score", game_engine_get_player_score(player_index));
						json.add_integer("kills", game_engine_get_player_kills(player_index));
						json.add_integer("deaths", game_engine_get_player_deaths(player_index));

						// Simulate realistic ping (20-150 ms)
						int32 simulated_ping = 20 + (player_index * 27) % 130;
						if (simulated_ping < 30)
						{
							 simulated_ping += 10;
						}
						json.add_integer("ping", simulated_ping);
					}
					json.end_object();
				}
				json.end_array();
			}
		}
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_api_map(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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
			char map[32] = "unknown";
			char mode[32] = "unknown";
			char state[32] = "unknown";
			int32 time_elapsed = 0;
			int32 time_remaining = 0;

			if (game_engine_running())
			{
				const c_map_variant& map_variant = game_engine_globals->runtime_map_variant;
				wchar_string_to_ascii_string(map_variant.m_metadata.display_name, map, NUMBEROF(map), nullptr);

				const c_game_variant* game_variant = user_interface_game_settings_get_game_variant();
				const c_game_engine_base_variant* active_variant = game_variant->get_active_variant();
				const s_saved_game_item_metadata* metadata = active_variant->get_metadata();
				wchar_string_to_ascii_string(metadata->display_name, mode, NUMBEROF(mode), nullptr);

				// state

				time_elapsed = game_engine_round_time_get();
				time_remaining = game_engine_globals->round_timer_in_seconds;
			}

			json.add_string("map", map);
			json.add_string("mode", mode);
			json.add_string("state", state);
			json.add_integer("time_elapsed", time_elapsed);
			json.add_integer("time_remaining", time_remaining);
		}
		json.end_object();

		if (game_engine_running())
		{
			s_game_engine_score_list new_list{};
			game_engine_scoring_build_score_list(&new_list, _game_engine_scoring_type_in_round, false);
			//if (new_list.player_count > 0)
			//{
			//	bool has_teams = game_engine_has_teams();
			//	ASSERT(VALID_COUNT(new_list.player_count, NUMBEROF(new_list.player_indices)));
			//
			//	json.begin_object();
			//	for (int32 player_num = 0; player_num < new_list.player_count; player_num++)
			//	{
			//		int32 player_index = new_list.player_indices[player_num];
			//		if (player_index != NONE)
			//		{
			//			const player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);
			//	
			//			c_static_string<32> player_name;
			//			wchar_string_to_ascii_string(player->configuration.host.name.get_string(), player_name.get_buffer(), player_name.element_count, nullptr);
			//		}
			//	}
			//	json.end_object();
			//}

			if (new_list.team_count > 0)
			{
				constexpr const char* k_team_names[k_multiplayer_team_count]
				{
					"Red",
					"Blue",
					"Green",
					"Yellow",
					"Purple",
					"Orange",
					"Brown",
					"Pink",
				};

				json.begin_array("teams");
				for (int32 team_num = 0; team_num < new_list.team_count; team_num++)
				{
					e_game_team team_index = new_list.team_indices[team_num];
					int32 team_score = game_engine_get_team_score(team_index);

					json.begin_object();
					{
						json.add_string("name", k_team_names[team_index]);
						json.add_integer("score", team_score);
						json.add_integer("players", 4);
					}
					json.end_object();
				}
				json.end_array();
			}
		}
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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

	static c_html_static_string<16384> html;  // Increased buffer for safety

	html.begin_html();
	{
		html.head_begin();
		{
			html.meta_charset("UTF-8");
			html.meta_viewport();
			html.title("Donkey Server Dashboard");

			html.style_begin();
			{
				html.raw("body { font-family: Arial, sans-serif; margin: 0; background: #1a1a1a; color: #fff; }");
				html.raw(".header { background: #2a2a2a; padding: 20px; border-bottom: 2px solid #00ff00; }");
				html.raw(".container { max-width: 1200px; margin: 0 auto; padding: 20px; }");
				html.raw(".stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 20px; margin: 20px 0; }");
				html.raw(".stat-card { background: #2a2a2a; padding: 20px; border-radius: 8px; border-left: 5px solid #00ff00; }");
				html.raw(".stat-value { font-size: 42px; font-weight: bold; color: #00ff00; margin: 10px 0; }");
				html.raw(".stat-label { font-size: 16px; color: #aaa; }");
				html.raw(".players { margin-top: 40px; }");
				html.raw("h2 { color: #00ccff; }");
				html.raw("table { width: 100%; border-collapse: collapse; background: #2a2a2a; border-radius: 8px; overflow: hidden; }");
				html.raw("th { background: #3a3a3a; padding: 15px; text-align: left; }");
				html.raw("td { padding: 12px 15px; border-top: 1px solid #444; }");
				html.raw("tr:nth-child(even) { background: #333; }");
				html.raw(".team-red { color: #ff6666; }");
				html.raw(".team-blue { color: #6688ff; }");
				html.raw(".refresh { background: #00ff00; color: #000; border: none; padding: 12px 24px; border-radius: 6px; cursor: pointer; font-size: 16px; margin-left: 20px; }");
				html.raw(".refresh:hover { background: #00cc00; }");
				html.raw(".loading { color: #aaa; font-style: italic; }");
				html.raw(".ping-good { color: #00ff00; font-weight: bold; }");
				html.raw(".ping-ok   { color: #ffff00; }");
				html.raw(".ping-bad  { color: #ff4444; font-weight: bold; }");
			}
			html.style_end();

			html.script_begin();
			{
				html.raw(R"(
					function formatTime(seconds) {
						const mins = Math.floor(seconds / 60);
						const secs = seconds % 60;
						return mins + ':' + (secs < 10 ? '0' : '') + secs;
					}

					function getPingClass(ping) {
						if (ping < 50) return 'ping-good';      // Green
						if (ping < 100) return 'ping-ok';       // Yellow
						return 'ping-bad';                      // Red
					}

					function updateDashboard() {
						fetch('/api/game-state')
							.then(r => r.json())
							.then(data => {
								document.getElementById('server-name').textContent = data.server.name;
								document.getElementById('uptime').textContent = Math.floor(data.server.uptime / 1000) + 's';

								document.getElementById('current-map').textContent = data.game.map;
								document.getElementById('game-mode').textContent = data.game.mode;
								document.getElementById('time-remaining').textContent = formatTime(data.game.time_remaining || 0);

								const teams = data.teams || [];
								teams.forEach((team, i) => {
									const scoreEl = document.getElementById('team-' + i + '-score');
									if (scoreEl) scoreEl.textContent = team.score;
									const nameEl = document.getElementById('team-' + i + '-name');
									if (nameEl) nameEl.textContent = team.name;
								});

								// Update players with ping
								fetch('/api/players')
									.then(r => r.json())
									.then(playersData => {
										const tbody = document.querySelector('#players-table tbody');
										tbody.innerHTML = '';
										const players = playersData.players || [];

										if (players.length === 0) {
											const row = document.createElement('tr');
											row.innerHTML = '<td colspan="6" class="loading">No players connected</td>';
											tbody.appendChild(row);
										} else {
											players.forEach(player => {
												const row = document.createElement('tr');
												const teamClass = player.index % 2 === 0 ? 'team-red' : 'team-blue';
												const teamName = player.index % 2 === 0 ? 'Red' : 'Blue';

												let kdText;
												if (player.deaths === 0) {
													kdText = player.kills > 0 ? 'Perfect' : '0.00';
												} else {
													kdText = (player.kills / player.deaths).toFixed(2);
												}

												const ping = player.ping || 999;
												const pingClass = getPingClass(ping);

												row.innerHTML = `
													<td>${player.name || 'Player' + player.index}</td>
													<td class="${teamClass}">${teamName}</td>
													<td>${player.score || 0}</td>
													<td>${player.kills || 0}</td>
													<td>${kdText}</td>
													<td class="${pingClass}">${ping} ms</td>
												`;
												tbody.appendChild(row);
											});
										}
									});
							})
							.catch(err => {
								console.error('Failed to fetch data:', err);
								document.getElementById('status').textContent = 'Error loading data';
							});
					}

					document.addEventListener('DOMContentLoaded', updateDashboard);
					setInterval(updateDashboard, 5000);
				)");
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

					html.span_begin();
					{
						html.attr("id", "status");
						html.text("Live");
					}
					html.span_end();

					html.button_begin();
					{
						html.css_class("refresh");
						html.attr("onclick", "updateDashboard()");
						html.text("Refresh Now");
					}
					html.button_end();
				}
				html.div_end();
			}
			html.div_end();

			html.div_begin();
			{
				html.css_class("container");

				// Stats Grid
				html.div_begin();
				{
					html.css_class("stats");

					// Server Name & Uptime
					html.div_begin();
					{
						html.css_class("stat-card");
						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "server-name");
							html.text("Loading...");
						}
						html.div_end();
						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Server Name");
						}
						html.div_end();

						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "uptime");
							html.text("--");
						}
						html.div_end();
						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Uptime");
						}
						html.div_end();
					}
					html.div_end();

					// Current Map
					html.div_begin();
					{
						html.css_class("stat-card");
						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "current-map");
							html.text("Loading...");
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

					// Game Mode
					html.div_begin();
					{
						html.css_class("stat-card");
						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "game-mode");
							html.text("Loading...");
						}
						html.div_end();
						html.div_begin();
						{
							html.css_class("stat-label");
							html.text("Game Mode");
						}
						html.div_end();
					}
					html.div_end();

					// Time Remaining
					html.div_begin();
					{
						html.css_class("stat-card");
						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "time-remaining");
							html.text("--:--");
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

					// Team Scores
					html.div_begin();
					{
						html.css_class("stat-card");
						html.div_begin();
						{
							html.css_class("stat-value");
							html.attr("id", "team-0-score");
							html.text("--");
						}
						html.div_end();
						html.div_begin();
						{
							html.css_class("stat-label");
							html.attr("id", "team-0-name");
							html.text("Red Team");
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
							html.attr("id", "team-1-score");
							html.text("--");
						}
						html.div_end();
						html.div_begin();
						{
							html.css_class("stat-label");
							html.attr("id", "team-1-name");
							html.text("Blue Team");
						}
						html.div_end();
					}
					html.div_end();
				}
				html.div_end();

				// Players Table
				html.div_begin();
				{
					html.css_class("players");
					html.h2("Active Players");

					html.table_begin();
					{
						html.attr("id", "players-table");
						html.thead_begin();
						{
							html.tr_begin();
							{
								html.th("Name");
								html.th("Team");
								html.th("Score");
								html.th("Kills");
								html.th("K/D");
								html.th("Ping");
							}
							html.tr_end();
						}
						html.thead_end();

						html.tbody_begin();
						{
							html.tr_begin();
							{
								html.td_begin();
								html.attr("colspan", "6");
								html.css_class("loading");
								html.text("Loading players...");
								html.td_end();
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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

	s_http_server* server = http_server_get();
	ASSERT(server != nullptr);

	c_json_static_string<2048> json;

	json.build_root_object();
	{
		const char* version = version_get_full_string();

		json.add_string("server", "Donkey Dedicated Server");
		json.add_string("version", version);
		json.add_integer("game_port", g_broadcast_port);
		json.add_integer("http_port", server->port);
		json.add_unsigned("uptime_ms", network_time_get());
		json.add_boolean("initialized", network_globals.initialized);

		json.add_boolean("has_session_manager", g_network_session_manager != nullptr);
		if (g_network_session_manager != nullptr)
		{
			json.begin_object("session");
			{
				int32 session_index = 0;
				c_network_session* session = g_network_session_manager->get_session(session_index);

				json.add_string("session_state", session->get_state_string());
				json.add_string("session_mode", session->get_mode_string());

				// Add more session-specific info here if needed
			}
			json.end_object();
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
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

	c_json_static_string<512> json;

	json.build_root_object();
	{
		json.add_string("status", "ok");
		json.add_unsigned("timestamp", network_time_get());
		json.add_string("client_address", transport_address_get_string(&client->address));
	}
	json.finalize_root_object();

	http_response_set_json(response, json.get_string());
}

void http_route_fallback(s_http_client* client, const s_http_request* request, s_http_response* response)
{
	ASSERT(client != nullptr);
	ASSERT(request != nullptr);
	ASSERT(response != nullptr);

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

