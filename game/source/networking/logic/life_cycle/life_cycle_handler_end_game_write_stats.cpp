#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"

#include "cseries/cseries_events.hpp"
#include "data_mining/data_mine_management.hpp"
#include "main/main_game.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/network_banhammer.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_session.hpp"

bool g_end_game_write_stats_bypass_override = false;

const char* k_stats_write_desire_strings[k_end_game_stats_write_desire_count]
{
	"no-reason",
	"not-live",
	"game-options-not-valid",
	"game-is-campaign",
	"game-is-film",
	"results-not-finalized",
	"can't-get-final-stats",
	"stats-uninitialized",
	"stats-unreliable",
	"simulation-aborted",
	"not-enough-machines",
	"write-desired",
};

c_life_cycle_state_handler_end_game_write_stats::c_life_cycle_state_handler_end_game_write_stats() :
	c_life_cycle_state_handler(),
	m_end_game_write_stats_start_time(),
	m_flags()
{
	//DECLFUNC(0x00453ED0, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

const char* c_life_cycle_state_handler_end_game_write_stats::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546D0, c_life_cycle_state_handler_end_game_write_stats, get_state_string);

	return "end-game-write-stats";
}

void c_life_cycle_state_handler_end_game_write_stats::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00492B40, c_life_cycle_state_handler_end_game_write_stats, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_end_game_write_stats::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x00492B50, c_life_cycle_state_handler_end_game_write_stats, enter, from, entry_data_size, entry_data);

	ASSERT(entry_data_size == 0);
	ASSERT(entry_data == NULL);

	c_life_cycle_state_handler::enter(from, 0, NULL);

	m_flags.clear();
	e_life_cycle_state from_state = from->get_state();
	if (from_state != _life_cycle_state_in_game)
	{
		m_flags.set(_end_game_write_stats_bypassed_bit, true);
	}

	m_end_game_write_stats_start_time = network_time_get();

	ASSERT(!network_session_interface_test_peer_status_flag(_network_session_peer_properties_status_game_stats_written_bit));

	network_squad_session_set_closed_status(_network_session_closed_not_closed);
	network_join_set_join_queue_mode(_network_join_open_to_join_squad);
	network_banhammer_upload_network_statistics();
}

void c_life_cycle_state_handler_end_game_write_stats::exit(c_life_cycle_state_handler* to)
{
	//INVOKE_CLASS_MEMBER(0x00492C10, c_life_cycle_state_handler_end_game_write_stats, exit, to);

	c_life_cycle_state_handler::exit(to);
	if (!main_game_change_in_progress())
	{
		main_menu_launch();
	}
}

//.text:00492C40 ; 

e_end_game_stats_write_desire c_life_cycle_state_handler_end_game_write_stats::game_get_stats_write_desire()
{
	return INVOKE_CLASS_MEMBER(0x00492C50, c_life_cycle_state_handler_end_game_write_stats, game_get_stats_write_desire);
}

void c_life_cycle_state_handler_end_game_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00492D10, c_life_cycle_state_handler_end_game_write_stats, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_game_write_stats, &parent_flags, 0x0ULL, 0x0ULL);
}

//.text:00492D40 ; 
//.text:00492D50 ; 
//.text:00492D70 ; 
//.text:00492D80 ; 
//.text:00492DA0 ; 

void c_life_cycle_state_handler_end_game_write_stats::stats_write_mark_complete()
{
	//INVOKE_CLASS_MEMBER(0x00492DD0, c_life_cycle_state_handler_end_game_write_stats, stats_write_mark_complete);

	ASSERT(!m_flags.test(_end_game_write_stats_completed_bit));

	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_game_stats_written_bit, true);
	m_flags.set(_end_game_write_stats_completed_bit, true);
}

//.text:00492DF0 ; 

void c_life_cycle_state_handler_end_game_write_stats::update()
{
	//INVOKE_CLASS_MEMBER(0x00492E10, c_life_cycle_state_handler_end_game_write_stats, update);

	c_life_cycle_state_manager* manager = get_manager();
	c_network_session* squad_session = manager->get_active_squad_session();

	if (!m_flags.test(_end_game_write_stats_initiated_bit))
	{
		bool suppress_stat_write = false;
		if (m_flags.test(_end_game_write_stats_bypassed_bit))
		{
			event(_event_message, "networking:logic:life_cycle:end_game_write_stats: not writing stats, bypassing");
		}
		else
		{
			e_end_game_stats_write_desire stats_write_desire = game_get_stats_write_desire();
			if (stats_write_desire != _end_game_stats_write_desired)
			{
				event(_event_message, "networking:logic:life_cycle:end_game_write_stats: should not write stats for this game, not writing stats [reason %s]",
					k_stats_write_desire_strings[stats_write_desire]);

				suppress_stat_write = true;
			}
		}

		if (g_end_game_write_stats_bypass_override)
		{
			c_life_cycle_state_handler_end_game_write_stats::stats_write_mark_complete();
		}
		else
		{
			m_flags.set(_end_game_write_stats_suppressed_bit, suppress_stat_write);
		}
		m_flags.set(_end_game_write_stats_initiated_bit, true);
	}

	if (!m_flags.test(_end_game_write_stats_completed_bit))
	{
		if (m_flags.test(_end_game_write_stats_session_start_completed_bit))
		{
			if (m_flags.test(_end_game_write_stats_suppressed_bit) || m_flags.test(_end_game_write_stats_write_completed_bit))
			{
				if (m_flags.test(_end_game_write_stats_session_end_completed_bit))
				{
					if (m_flags.test(_end_game_write_stats_webstats_submitted_bit) || !squad_session->is_host())
					{
						if (m_flags.test(_end_game_write_stats_data_mine_upload_completed_bit))
						{
							c_life_cycle_state_handler_end_game_write_stats::stats_write_mark_complete();
						}
						else
						{
							c_life_cycle_state_handler_end_game_write_stats::update_data_mine_upload();
						}
					}
					else
					{
						c_life_cycle_state_handler_end_game_write_stats::update_submit_webstats();
					}
				}
				else
				{
					c_life_cycle_state_handler_end_game_write_stats::update_session_end();
				}
			}
			else
			{
				c_life_cycle_state_handler_end_game_write_stats::update_write_stats();
			}
		}
		else
		{
			c_life_cycle_state_handler_end_game_write_stats::update_session_start();
		}
	}

	if (squad_session->is_host())
	{
		if (c_life_cycle_state_handler::all_peers_have_main_menu_ready(squad_session))
		{
			if (m_flags.test(_end_game_write_stats_completed_bit))
			{
				const c_network_session_membership* squad_session_membership = squad_session->get_session_membership();
				if (squad_session_membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_game_stats_written_bit))
				{
					c_network_session_parameters* squad_session_parameters = squad_session->get_session_parameters();
					squad_session_parameters->m_parameters.session_mode.set(_network_session_mode_idle);
				}
			}
		}
	}

	if (!g_end_game_write_stats_bypass_override && !data_mine_enabled())
	{
		if (network_time_get() > (uns32)(1000 * g_network_configuration.life_cycle.matchmaking.end_match_write_stats_boot_threshold_seconds))
		{
			if (!m_flags.test(_end_game_write_stats_completed_bit))
			{
				event(
					_event_warning,
					"networking:logic:life-cycle:end_match_write_stats: we exceeded the grief timer, and had not written stats, force completing");

				stats_write_mark_complete();
			}

			if (squad_session->is_host() && c_life_cycle_state_handler::all_peers_have_main_menu_ready(squad_session))
			{
				const c_network_session_membership* squad_session_membership = squad_session->get_session_membership();
				const s_network_session_peer* local_peer = squad_session_membership->get_local_peer();
				if (local_peer->properties.flags.test(_network_session_peer_properties_status_match_stats_written_bit))
				{
					uns32 peer_kick_mask = 0;

					for (int32 peer_index = squad_session_membership->get_first_peer(); peer_index != NONE; peer_index = squad_session_membership->get_next_peer(peer_index))
					{
						const s_network_session_peer* peer = squad_session_membership->get_peer(peer_index);
						if (!peer->properties.flags.test(_network_session_peer_properties_status_match_stats_written_bit))
						{
							SET_BIT(peer_kick_mask, peer_index, true);
						}
					}

					if (peer_kick_mask != 0)
					{
						for (int32 peer_index = 0; peer_index < 17; peer_index++)
						{
							if (TEST_BIT(peer_kick_mask, peer_index))
							{
								event(
									_event_warning,
									"networking:logic:life-cycle:end_game_write_stats: peer %d is taking too long to write stats, kicking!",
									peer_index);

								squad_session->host_boot_machine(peer_index, _network_session_boot_blocking_stats_write);
							}
						}
					}
				}
				else
				{
					event(
						_event_warning,
						"networking:logic:life-cycle:end_game_write_stats: host is taking too long to write stats, bailing!");

					squad_session->host_boot_machine(squad_session_membership->local_peer_index(), _network_session_boot_blocking_stats_write);

					c_network_session_parameters* squad_session_parameters = squad_session->get_session_parameters();
					squad_session_parameters->m_parameters.session_mode.set(_network_session_mode_idle);
				}
			}
		}
	}
}

void c_life_cycle_state_handler_end_game_write_stats::update_data_mine_upload()
{
	INVOKE_CLASS_MEMBER(0x004930E0, c_life_cycle_state_handler_end_game_write_stats, update_data_mine_upload);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_game_write_stats::update_for_state_transition()
{
	//return INVOKE_CLASS_MEMBER(0x00493170, c_life_cycle_state_handler_end_game_write_stats, update_for_state_transition);

	e_life_cycle_state_transition_type result = _life_cycle_state_transition_unchanged;
	bool state_work_completed = m_flags.test(_end_game_write_stats_completed_bit);
	if (state_work_completed)
	{
		c_life_cycle_state_manager* manager = get_manager();
		const c_network_session* squad_session = manager->get_active_squad_session();

		if (squad_session->session_mode() != _network_session_mode_end_game)
		{
			manager->request_state_change(_life_cycle_state_pre_game, 0, NULL);
			result = _life_cycle_state_transition_change_requested;
		}
	}
	return result;
}

void c_life_cycle_state_handler_end_game_write_stats::update_session_end()
{
	INVOKE_CLASS_MEMBER(0x004931B0, c_life_cycle_state_handler_end_game_write_stats, update_session_end);
}

void c_life_cycle_state_handler_end_game_write_stats::update_session_start()
{
	//INVOKE_CLASS_MEMBER(0x00493240, c_life_cycle_state_handler_end_game_write_stats, update_session_start);

	c_life_cycle_state_manager* manager = c_life_cycle_state_handler::get_manager();
	c_network_session* squad_session = manager->get_active_squad_session();
	int32 managed_session_index = squad_session->m_managed_session_index;

	if (!m_flags.test(_end_game_write_stats_session_start_initiated_bit))
	{
		ASSERT(!m_flags.test(_end_game_write_stats_session_start_completed_bit));

		event(_event_message, "networking:logic:life_cycle:end_game_write_stats: starting session 0x%08X",
			managed_session_index);

		managed_session_game_start(managed_session_index);
		m_flags.set(_end_game_write_stats_session_start_initiated_bit, true);
	}

	if (!m_flags.test(_end_game_write_stats_session_start_completed_bit))
	{
		c_managed_session_status_flags session_status_flags = managed_session_get_status(managed_session_index);
		if (!session_status_flags.test(_managed_session_game_start_in_progress_bit))
		{
			if (!session_status_flags.test(_managed_session_game_started_bit))
			{
				if (session_status_flags.test(_managed_session_game_start_failed_bit))
				{
					event(_event_warning, "networking:logic:life_cycle:end_game_write_stats: failed to start session");
				}
			}

			m_flags.set(_end_game_write_stats_session_start_completed_bit, true);
		}
	}
}

void c_life_cycle_state_handler_end_game_write_stats::update_submit_webstats()
{
	INVOKE_CLASS_MEMBER(0x004932C0, c_life_cycle_state_handler_end_game_write_stats, update_submit_webstats);
}

void c_life_cycle_state_handler_end_game_write_stats::update_write_stats()
{
	INVOKE_CLASS_MEMBER(0x00493310, c_life_cycle_state_handler_end_game_write_stats, update_write_stats);
}

