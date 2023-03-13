#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"
#include "saved_games/saved_game_files.hpp"

enum e_game_engine_symmetric_placement
{
	_game_engine_symmetric_placement_ignore = 0,
	_game_engine_symmetric_placement_symmetric,
	_game_engine_symmetric_placement_asymmetric,

	k_game_engine_symmetric_placement_count
};

enum e_scenario_game_engine
{
	_scenario_game_engine_ctf = 0,
	_scenario_game_engine_slayer,
	_scenario_game_engine_oddball,
	_scenario_game_engine_king,
	_scenario_game_engine_juggernaut,
	_scenario_game_engine_territories,
	_scenario_game_engine_assault,
	_scenario_game_engine_vip,
	_scenario_game_engine_infection,
	_scenario_game_engine_target_training,

	k_scenario_game_engine_count
};

enum e_multiplayer_team_designator
{
	_multiplayer_team_designator_defender = 0,
	_multiplayer_team_designator_attacker,
	_multiplayer_team_designator_third_party,
	_multiplayer_team_designator_fourth_party,
	_multiplayer_team_designator_fifth_party,
	_multiplayer_team_designator_sixth_party,
	_multiplayer_team_designator_seventh_party,
	_multiplayer_team_designator_eigth_party,
	_multiplayer_team_designator_neutral,

	k_multiplayer_team_designator_count
};

enum e_teleporter_channel
{
	_teleporter_channel_alpha,
	_teleporter_channel_bravo,
	_teleporter_channel_charlie,
	_teleporter_channel_delta,
	_teleporter_channel_echo,
	_teleporter_channel_foxtrot,
	_teleporter_channel_golf,
	_teleporter_channel_hotel,
	_teleporter_channel_india,
	_teleporter_channel_juliet,
	_teleporter_channel_kilo,
	_teleporter_channel_lima,
	_teleporter_channel_mike,
	_teleporter_channel_november,
	_teleporter_channel_oscar,
	_teleporter_channel_papa,
	_teleporter_channel_quebec,
	_teleporter_channel_romeo,
	_teleporter_channel_sierra,
	_teleporter_channel_tango,
	_teleporter_channel_uniform,
	_teleporter_channel_victor,
	_teleporter_channel_whiskey,
	_teleporter_channel_xray,
	_teleporter_channel_yankee,
	_teleporter_channel_zulu,

	k_teleporter_channel_count
};

enum e_multiplayer_object_boundary_shape
{
	_multiplayer_object_boundary_shape_unused = 0,
	_multiplayer_object_boundary_shape_sphere,
	_multiplayer_object_boundary_shape_cylinder,
	_multiplayer_object_boundary_shape_box,

	k_multiplayer_object_boundary_shape_count
};

struct s_variant_multiplayer_object_properties_definition
{
public:
	s_variant_multiplayer_object_properties_definition() :
		symmetry_placement_flags(),
		game_engine_flags()
	{
		initialize_to_default();
	}

	void initialize_to_default()
	{
		symmetry_placement_flags = _game_engine_symmetric_placement_ignore;
		game_engine_flags = 12;
		owner_team = 9;
		shared_storage = { 0 };
		spawn_rate = 0;
		teleporter_channel = _teleporter_channel_alpha;
		boundary_shape = _multiplayer_object_boundary_shape_unused;
		boundary_radius = 0.0f;
		boundary_box_length = 0.0f;
		boundary_positive_height = 0.0f;
		boundary_negative_height = 0.0f;
	}

protected:
	c_flags<e_game_engine_symmetric_placement, word, k_game_engine_symmetric_placement_count> symmetry_placement_flags;
	c_flags<e_scenario_game_engine, byte, k_scenario_game_engine_count> game_engine_flags;
	c_enum<e_multiplayer_team_designator, byte, k_multiplayer_team_designator_count> owner_team;

	union
	{
		char spare_clips;
		char teleporter_channel;
		char spawn_order;
	} shared_storage;

	// seconds
	char spawn_rate;

	c_enum<e_teleporter_channel, byte, k_teleporter_channel_count> teleporter_channel;

	c_enum<e_multiplayer_object_boundary_shape, char, k_multiplayer_object_boundary_shape_count> boundary_shape;

	union
	{
		real boundary_radius;
		real boundary_width;
	};
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
};
static_assert(sizeof(s_variant_multiplayer_object_properties_definition) == 0x18);

enum e_variant_object_placement_flags
{
	_map_variant_occupied_slot = 0,
	_map_variant_scenario_object_bit = 3,
	_map_variant_scenario_object_removed = 5,

	k_variant_object_placement_flag_count
};

struct s_variant_object_datum
{
public:
	s_variant_object_datum() :
		flags(),
		object_index(-1),
		editor_object_index(-1),
		object_definition_index(-1),
		position(),
		forward(),
		up(),
		parent_object_identifier(),
		multiplayer_game_object_properties()
	{
	}

protected:
	c_flags<e_variant_object_placement_flags, word, k_variant_object_placement_flag_count> flags;
	short : 16;

	long object_index;
	long editor_object_index;
	long object_definition_index;

	// axis
	real_point3d position;
	vector3d forward;
	vector3d up;

	c_object_identifier parent_object_identifier;
	s_variant_multiplayer_object_properties_definition multiplayer_game_object_properties;
};
static_assert(sizeof(s_variant_object_datum) == 0x54);

struct s_variant_quota
{
public:
	s_variant_quota() :
		object_definition_index(-1),
		minimum_count(0),
		maximum_count(0),
		placed_on_map(0),
		maximum_allowed(-1),
		price_per_item(0.0f)
	{
	}

protected:
	long object_definition_index;
	byte minimum_count;
	byte maximum_count;
	byte placed_on_map;
	byte maximum_allowed;
	real price_per_item;
};
static_assert(sizeof(s_variant_quota) == 0xC);

struct c_map_variant
{
public:
	c_map_variant();
	void __cdecl create_default(long map_id);

private:
	s_saved_game_item_metadata m_metadata;

	short m_map_variant_version;

	short m_number_of_scenario_objects;
	short m_number_of_variant_objects;
	short m_number_of_placeable_object_quotas;

	long m_map_id;

	real_rectangle3d m_world_bounds;

	long m_game_engine_subtype;

	real m_maximum_budget;
	real m_spent_budget;

	bool m_helpers_enabled;
	bool m_built_in;
	byte __pad12A[2];

	dword m_map_variant_checksum;

	c_static_array<s_variant_object_datum, 640> m_variant_objects;
	c_static_array<short, k_object_type_count> m_object_type_start_index;
	c_static_array<s_variant_quota, 256> m_quotas;

	byte unused[0x140];
};
static_assert(sizeof(c_map_variant) == 0xE090);