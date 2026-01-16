#include "animations/animation_id.hpp"
#include "game/materials.hpp"

struct s_character_physics_definition;
class c_animation_manager;
struct hkShape;
struct s_character_physics_update_input_datum
{
	struct s_flying
	{
		real32 bank;
	};

	struct s_sentinel
	{
		real_point3d position;
		real_point3d animation_position;
		real_vector3d desired_velocity;
		c_animation_id animation_id;
	};

	struct s_ground
	{
		int32 last_support_havok_component_index;
		int32 last_support_rigid_body_index;
		real_matrix4x3 last_support_havok_component_matrix;
		real_vector3d ground_plane;
		real32 stun_fraction;
		real_vector3d movement_forward;
		real32 adhesion_velocity_scale;
		bool effected_by_phantom_this_tick;
		bool __unknown5D;
	};

	struct s_melee
	{
		bool has_target;
		real32 physics_radius;
		real32 target_physics_radius;
		real_point3d target_position;
		real_vector3d target_velocity;
		real32 target_distance;
	};

	struct s_dead
	{
		real_vector3d resting_surface_normal;
		bool __unknownC;
		bool update_size;
		bool in_relaxation_state;
	};

	int32 mode; // c_character_physics_component::e_mode

	bool sub_initialized;
	const s_character_physics_definition* character_physics_definition;
	uns32 flags;
	int32 early_mover_object_index;
	int32 accepted_early_mover_object_index;
	int32 havok_component_index;
	const c_animation_manager* animation_manager;
	real32 movement_scale;
	int32 havok_group;
	s_flying flying;
	s_sentinel sentinel;
	s_ground ground;
	s_dead dead;
	s_melee melee;
	real_vector3d throttle;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d desired_facing_vector;
	real_vector3d aiming;
	real_vector3d velocity;
	real_vector3d early_mover_localized_velocity;
	real32 gravity;
	real32 boost_power;
	real_vector3d desired_movement_speed;
	real32 acceleration_maximum;
	real32 airborne_acceleration_maximum;
	const hkShape* shape_switch_request;
};
COMPILE_ASSERT(sizeof(s_character_physics_update_input_datum) == 0x16C);

struct s_havok_contact_point_reference;
struct s_character_physics_move_output_datum;
struct s_character_physics_move_input_datum;
struct s_character_physics_update_output_datum;

class c_character_physics_mode_ground_datum
{
public:
	const real_vector3d* get_ground_plane() const;
	real32 get_ground_plane_contact_offset() const;
	real32 get_landing_velocity() const;
	int32 get_support_havok_component_index() const;
	const real_matrix4x3* get_support_havok_component_matrix() const;
	int32 get_support_object_index() const;
	int32 get_support_rigid_body_index() const;
	void initialize();
	void initialize_internal(bool initialize_support_data);
	void move(s_character_physics_move_output_datum* physics_output, const s_character_physics_move_input_datum* physics_input);
	void notify_in_slip_surface_volume();
	void set_contact(int32 havok_component_index, const s_havok_contact_point_reference* primary_contact_point_reference, const real_vector3d* ground_plane);
	void set_ground_plane(const real_vector3d* ground_plane);
	void update(s_character_physics_update_output_datum* physics_output, const s_character_physics_update_input_datum* physics_input);
	void update_internal(s_character_physics_update_output_datum* physics_output, const s_character_physics_update_input_datum* physics_input, bool localize_airborne, const real_vector3d* localized_velocity);
	bool update_localized_physics_space_linear_velocity(const s_character_physics_update_input_datum* physics_input, real_vector3d* linear_velocity, real_vector3d* space_acceleration, real_matrix3x3* rotation);

	bool __thiscall update_ground_velocity(const s_character_physics_update_input_datum* physics_input, real_vector3d* linear_velocity, real_matrix3x3* rotation);

	real_vector3d m_last_localized_physics_velocity;
	int32 m_last_time_animation_velocity;
	real_vector3d m_last_animation_velocity;
	int32 m_support_havok_component_index;
	int32 m_support_havok_component_rigid_body_index;
	real_matrix4x3 m_support_havok_component_matrix;
	int32 m_surface_index;
	int32 m_support_object_index;
	c_global_material_type m_ground_material_type;
	uns8 m_ground_physics_update_ticks;
	uns8 m_flags;
	real_vector3d m_ground_plane;
	real32 m_landing_velocity;
};
COMPILE_ASSERT(sizeof(c_character_physics_mode_ground_datum) == 0x74);

struct s_havok_contact_point_reference
{
	int16 rigid_body_index;
	int16 contact_point_index;
};
COMPILE_ASSERT(sizeof(s_havok_contact_point_reference) == 0x4);

