#pragma once

#include "cseries/cseries.hpp"

enum e_stimulus
{
	_stimulus_surprise,
	_stimulus_surprise_combat_started,
	_stimulus_friendly_body,
	_stimulus_shield_depleted,
	_stimulus_leader_dead,
	_stimulus_peer_dead,
	_stimulus_last_man,
	_stimulus_stuck_with_grenade,
	_stimulus_super_detonation,
	_stimulus_null,
	_stimulus_destrucible_obstacle,
	_stimulus_cover_friend,
	_stimulus_activity,
	_stimulus_changed_target,
	_stimulus_cover_failed,
	_stimulus_broken,
	_stimulus_leader_abandoned,
	_stimulus_flanked,
	_stimulus_enemy_vehicle,
	_stimulus_turtle,
	_stimulus_flood_disperse,

	k_stimulus_count
};

extern char const* const g_stimulus_names[k_stimulus_count];

extern void stimuli_debug();

