#include "networking/logic/logic_matchmaking_composition.hpp"

#include "game/multiplayer_game_hopper.hpp"
#include "memory/module.hpp"
#include "networking/session/network_session.hpp"

//REFERENCE_DECLARE_ARRAY(0x0189C1E8, const char*, k_network_session_matchmaking_composition_build_results, k_network_session_matchmaking_composition_build_result_count);

HOOK_DECLARE(0x004D2840, network_session_build_matchmaking_composition);

//.text:004D27E0 ; public: bool c_hopper_configuration::are_guests_allowed() const
//.text:004D27F0 ; public: int32 c_hopper_configuration::get_big_squad_size_threshold() const
//.text:004D2810 ; public: int32 c_hopper_configuration::get_maximum_big_squad_imbalance() const
//.text:004D2820 ; public: bool c_hopper_configuration::is_big_squad_restriction_enabled() const

e_network_session_matchmaking_composition_build_result __cdecl network_session_build_matchmaking_composition(const c_network_session* session, s_network_session_matchmaking_composition* composition_out)
{
	//return INVOKE(0x004D2840, network_session_build_matchmaking_composition, session, composition_out);

	ASSERT(session);
	ASSERT(composition_out);

	csmemset(composition_out, 0, sizeof(s_network_session_matchmaking_composition));

	e_network_session_matchmaking_composition_build_result result = _network_session_matchmaking_composition_build_result_none;

	if (!network_session_matchmaking_composition_session_state_valid(session, &result))
	{
		if (result != _network_session_matchmaking_composition_build_result_success)
		{
			return result;
		}
	}

	const c_network_session_membership* session_membership = session->get_session_membership();
	const c_hopper_configuration* hopper_configuration = multiplayer_game_hoppers_get_current_hopper_configuration();
	if (!network_session_matchmaking_composition_session_state_ready(session_membership, hopper_configuration, &result))
	{
		if (result != _network_session_matchmaking_composition_build_result_success)
		{
			return result;
		}
	}

	//composition_out->session_properties.hopper_identifier = hopper_configuration->get_hopper_identifier();
	composition_out->session_properties.hopper_identifier = hopper_configuration->m_universal.hopper_identifier;

	//composition_out->session_properties.hopper_ranked = hopper_configuration->is_ranked();
	e_hopper_type hopper_type = hopper_configuration->m_universal.hopper_type;
	composition_out->session_properties.hopper_ranked = hopper_type == _hopper_type_ranked_ffa || hopper_type == _hopper_type_ranked_teams;

	result = network_session_matchmaking_composition_build_players_and_parties(session_membership, hopper_configuration, composition_out);
	if (result != _network_session_matchmaking_composition_build_result_success)
	{
		return result;
	}

	result = network_session_matchmaking_composition_build_party_masks(session_membership, hopper_configuration, composition_out);
	if (result != _network_session_matchmaking_composition_build_result_success)
	{
		return result;
	}

	result = network_session_matchmaking_composition_build_session_statistics(session_membership, composition_out);
	if (result != _network_session_matchmaking_composition_build_result_success)
	{
		return result;
	}

	result = network_session_matchmaking_composition_build_local_properties(session_membership, composition_out);
	if (result != _network_session_matchmaking_composition_build_result_success)
	{
		return result;
	}

	composition_out->session_properties.flags.set(_network_session_matchmaking_composition_valid_bit, true);

	return _network_session_matchmaking_composition_build_result_success;
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_flags(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D2930, network_session_matchmaking_composition_build_flags, session_membership, hopper_configuration, composition_out);
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_guest_statistics(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D2970, network_session_matchmaking_composition_build_guest_statistics, session_membership, hopper_configuration, composition_out);
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_local_properties(const c_network_session_membership* session_membership, s_network_session_matchmaking_composition* composition_out)
{
	//return INVOKE(0x004D2A00, network_session_matchmaking_composition_build_local_properties, session_membership, composition_out);
	
	// $IMPLEMENT

	composition_out->session_properties.language = session_membership->m_shared_network_membership.peers[session_membership->m_local_peer_index].properties.language;
	composition_out->session_properties.nat_type = session_membership->m_shared_network_membership.peers[session_membership->m_local_peer_index].properties.peer_nat_type;
	
	//composition_out->session_properties.gamer_region = ;
	//composition_out->session_properties.gamer_zone = ;

	return _network_session_matchmaking_composition_build_result_success;
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_party_masks(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D2A40, network_session_matchmaking_composition_build_party_masks, session_membership, hopper_configuration, composition_out);
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_player_party_indices(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D2F30, network_session_matchmaking_composition_build_player_party_indices, session_membership, hopper_configuration, composition_out);
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_player_statistics(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D30C0, network_session_matchmaking_composition_build_player_statistics, session_membership, hopper_configuration, composition_out);
}

e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_players_and_parties(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D33B0, network_session_matchmaking_composition_build_players_and_parties, session_membership, hopper_configuration, composition_out);
}
e_network_session_matchmaking_composition_build_result __cdecl network_session_matchmaking_composition_build_session_statistics(const c_network_session_membership* session_membership, s_network_session_matchmaking_composition* composition_out)
{
	return INVOKE(0x004D3460, network_session_matchmaking_composition_build_session_statistics, session_membership, composition_out);
}

const char* __cdecl network_session_matchmaking_composition_get_build_result_string(e_network_session_matchmaking_composition_build_result result)
{
	return INVOKE(0x004D34E0, network_session_matchmaking_composition_get_build_result_string, result);
}

bool __cdecl network_session_matchmaking_composition_session_state_ready(const c_network_session_membership* session_membership, const c_hopper_configuration* hopper_configuration, e_network_session_matchmaking_composition_build_result* failure_build_result_out)
{
	return INVOKE(0x004D34F0, network_session_matchmaking_composition_session_state_ready, session_membership, hopper_configuration, failure_build_result_out);
}

bool __cdecl network_session_matchmaking_composition_session_state_valid(const c_network_session* session, e_network_session_matchmaking_composition_build_result* result_out)
{
	return INVOKE(0x004D35C0, network_session_matchmaking_composition_session_state_valid, session, result_out);
}

const char* k_network_session_matchmaking_composition_build_results[k_network_session_matchmaking_composition_build_result_count] //.data:0189C1E8 ; 
{
	"none",
	"session-state-not-ready",
	"no-hopper-configuration",
	"hopper-mismatch",
	"no-game-set",
	"session-not-ready-no-stats",
	"session-not-ready-banned",
	"session-not-ready-no-controller",
	"invalid-player-count",
	"invalid-party-composition",
	"guests-not-allowed",
	"can't-pick-teams",
	"can't-even-out-teams",
	"big-party-no-rabble-yet",
	"big-party-teams-exceed-imbalance",
	"big-party-count-exceeds-teams",
	"no-signed-in-local-players",
	"invalid-skill-range",
	"success"
};

