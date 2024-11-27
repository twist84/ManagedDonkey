#include "cseries/cseries.hpp"

#include "game/materials.hpp"

struct s_character_physics_definition;
struct c_animation_manager;
struct hkShape;
struct s_character_physics_update_input_datum
{
	struct s_flying
	{
		real bank;
	};

	struct s_sentinel
	{
		real_point3d position;
		real_point3d animation_position;
		vector3d desired_velocity;
		c_animation_id animation_id;
	};

	struct s_ground
	{
		long last_support_havok_component_index;
		long last_support_rigid_body_index;
		real_matrix4x3 last_support_havok_component_matrix;
		vector3d ground_plane;
		real stun_fraction;
		vector3d movement_forward;
		real adhesion_velocity_scale;
		bool effected_by_phantom_this_tick;
		bool __unknown5D;
	};

	struct s_melee
	{
		bool has_target;
		real physics_radius;
		real target_physics_radius;
		real_point3d target_position;
		vector3d target_velocity;
		real target_distance;
	};

	struct s_dead
	{
		vector3d resting_surface_normal;
		bool update_size;
		bool in_relaxation_state;
	};

	long mode; // c_character_physics_component::e_mode

	bool sub_initialized;
	s_character_physics_definition const* character_physics_definition;
	dword_flags flags;
	long early_mover_object_index;
	long accepted_early_mover_object_index;
	long havok_component_index;
	c_animation_manager const* animation_manager;
	real movement_scale;
	long havok_group;
	s_flying flying;
	s_sentinel sentinel;
	s_ground ground;
	s_dead dead;
	s_melee melee;
	vector3d throttle;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d desired_facing_vector;
	vector3d aiming;
	vector3d velocity;
	vector3d early_mover_localized_velocity;
	real gravity;
	real boost_power;
	vector3d desired_movement_speed;
	real acceleration_maximum;
	real airborne_acceleration_maximum;
	hkShape const* shape_switch_request;
};
static_assert(sizeof(s_character_physics_update_input_datum) == 0x16C);

struct s_havok_contact_point_reference;
struct s_character_physics_move_output_datum;
struct s_character_physics_move_input_datum;
struct s_character_physics_update_output_datum;

struct c_character_physics_mode_ground_datum
{
	vector3d const* get_ground_plane() const;
	real get_ground_plane_contact_offset() const;
	real get_landing_velocity() const;
	long get_support_havok_component_index() const;
	real_matrix4x3 const* get_support_havok_component_matrix() const;
	long get_support_object_index() const;
	long get_support_rigid_body_index() const;
	void initialize();
	void initialize_internal(bool initialize_support_data);
	void move(s_character_physics_move_output_datum* physics_output, s_character_physics_move_input_datum const* physics_input);
	void notify_in_slip_surface_volume();
	void set_contact(long havok_component_index, s_havok_contact_point_reference const* primary_contact_point_reference, vector3d const* ground_plane);
	void set_ground_plane(vector3d const* ground_plane);
	void update(s_character_physics_update_output_datum* physics_output, s_character_physics_update_input_datum const* physics_input);
	void update_internal(s_character_physics_update_output_datum* physics_output, s_character_physics_update_input_datum const* physics_input, bool localize_airborne, vector3d const* localized_velocity);
	bool update_localized_physics_space_linear_velocity(s_character_physics_update_input_datum const* physics_input, vector3d* linear_velocity, vector3d* space_acceleration, matrix3x3* rotation);

	bool __thiscall update_ground_velocity(s_character_physics_update_input_datum const* physics_input, vector3d* linear_velocity, matrix3x3* rotation);

	vector3d m_last_localized_physics_velocity;
	long m_last_time_animation_velocity;
	vector3d m_last_animation_velocity;
	long m_support_havok_component_index;
	long m_support_havok_component_rigid_body_index;
	real_matrix4x3 m_support_havok_component_matrix;
	long m_surface_index;
	long m_support_object_index;
	c_global_material_type m_ground_material_type;
	byte m_ground_physics_update_ticks;
	byte_flags m_flags;
	vector3d m_ground_plane;
	real m_landing_velocity;
};
static_assert(sizeof(c_character_physics_mode_ground_datum) == 0x74);

struct s_havok_contact_point_reference
{
	short rigid_body_index;
	short contact_point_index;
};
static_assert(sizeof(s_havok_contact_point_reference) == 0x4);

