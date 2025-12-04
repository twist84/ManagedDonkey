#pragma once

enum e_equipment_type
{
	_equipment_type_none = NONE,
	_equipment_type_super_shield,
	_equipment_type_multiplayer_powerup,
	_equipment_type_spawner,
	_equipment_type_proximity_mine,
	_equipment_type_motion_tracker_noise,
	_equipment_type_showme,
	_equipment_type_invisibility_mode,
	_equipment_type_invincibility,
	_equipment_type_treeoflife,

	k_equipment_type_count,

	// ODST
	_equipment_type_health_pack = k_equipment_type_count,

	// Halo Online
	_equipment_type_forced_reload,
	_equipment_type_concussive_blast,
	_equipment_type_tank_mode,
	_equipment_type_mag_pulse,
	_equipment_type_hologram,
	_equipment_type_reactive_armor,
	_equipment_type_bomb_run,
	_equipment_type_armor_lock,
	_equipment_type_adrenaline,
	_equipment_type_lightning_strike,
	_equipment_type_scrambler,
	_equipment_type_weapon_jammer,
	_equipment_type_ammo_pack,
	_equipment_type_vision,

	k_equipment_type_count_halo_online,
};

class c_equipment_type
{
public:
	virtual void equipment_new(int32 equipment_index) const {}
	virtual void create_children(int32 equipment_index) const {}
	virtual bool can_activate(int32 equipment_index) const { return true; }
	virtual bool compute_function_value(int32 object_index, int32 function, int32 function_owner_definition_index, real32* value, bool* active, bool* deterministic) const { return false; }
	virtual void activate_local_predicted(int32 equipment_index) const {}
	virtual void deactivate_local_predicted(int32 equipment_index) const {}
	virtual void activate_local_authoritative(int32 equipment_index) const {}
	virtual void deactivate_local_authoritative(int32 equipment_index) const {}
	virtual void handle_activation_request(int32 equipment_index) const {}
	virtual void handle_deactivation_request(int32 equipment_index) const {}
	virtual void apply_activation_message_remote(int32 equipment_index) const {}
	virtual void apply_deactivation_message_remote(int32 equipment_index) const {}
	virtual void apply_activation_message_local(int32 equipment_index) const {}
	virtual void apply_deactivation_message_local(int32 equipment_index) const {}
	virtual bool update(int32 equipment_index) const { return false; }
	virtual void notify_child_took_shield_damage(int32 equipment_index) const {}
	virtual bool can_be_thrown(int32 equipment_index) const { return false; }
	virtual void handle_client_key_frame_request(int32 equipment_index) const {}
};

extern c_equipment_type* g_equipment_types[k_equipment_type_count];

