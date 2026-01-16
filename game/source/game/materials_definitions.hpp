#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_water_density_type
{
	_water_density_type_default = 0,
	_water_density_type_super_floater,
	_water_density_type_floater,
	_water_density_type_neutral,
	_water_density_type_sinker,
	_water_density_type_super_sinker,
	_water_density_type_none,

	k_water_density_count
};

struct s_material_physics_drag_properties
{
	// drag= k*power(q*velocity, e)
	// 
	// defaults to k= .5, q= 1.0, e= 1.0
	real32 drag_k;
	real32 drag_q;
	real32 drag_e;

	// objects of this flotation category have this density relative to being in this material (1.0f==neutral)
	real32 water_densities[k_water_density_count];
};
COMPILE_ASSERT(sizeof(s_material_physics_drag_properties) == 0x28);

enum e_global_material_bit
{
	_global_material_flammable_bit = 0,
	_global_material_biomass_bit,
	_global_material_radiance_transfer_interior_bit,

	k_number_of_global_material_bits
};

struct s_material_physics_properties
{
	uns32 flags;
	real32 friction;
	real32 restitution;
	real32 density;
	c_typed_tag_block<s_material_physics_drag_properties> drag_block;
};
COMPILE_ASSERT(sizeof(s_material_physics_properties) == 0x1C);

enum e_global_material_sweetener_types
{
	_global_material_sweeteners_types_first = 0,
	_global_material_sweeteners_sound_small = _global_material_sweeteners_types_first,
	_global_material_sweeteners_sound_medium,
	_global_material_sweeteners_sound_large,
	_global_material_sweeteners_sound_rolling,
	_global_material_sweeteners_sound_grinding,
	_global_material_sweeteners_sound_melee_small,
	_global_material_sweeteners_sound_melee_medium,
	_global_material_sweeteners_sound_melee_large,
	_global_material_sweeteners_effect_small,
	_global_material_sweeteners_effect_medium,
	_global_material_sweeteners_effect_large,
	_global_material_sweeteners_effect_rolling,
	_global_material_sweeteners_effect_grinding,
	_global_material_sweeteners_effect_melee,
	_global_material_sweeteners_water_ripple_small,
	_global_material_sweeteners_water_ripple_medium,
	_global_material_sweeteners_water_ripple_large,

	k_global_material_sweeteners_inheritance_flags_count
};

struct s_global_material_sweeteners_definition
{
	union
	{
		struct
		{
			c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound[3];
			c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> rolling_sound;
			c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> grinding_sound;
			c_typed_tag_reference<SOUND_TAG, INVALID_TAG> melee_sounds[3];
			c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect[3];
			c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> rolling_effect;
			c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> grinding_effect;
			c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> melee_effect;
			c_typed_tag_reference<RENDER_WATER_RIPPLE_TAG, INVALID_TAG> water_ripple[3];
		};

		c_typed_tag_reference<> sweetener_tag_references[k_global_material_sweeteners_inheritance_flags_count];
	};

	// sweetener inheritance flags
	// 
	// when a sweetener inheritance flag is set the sound\effect is not inherited from the parent material.  If you leave the sweetener blank and set the flag than no effect\sound will play

	c_flags<e_global_material_sweetener_types, int32, k_global_material_sweeteners_inheritance_flags_count> inheritance_flags;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_global_material_sweeteners_definition) == 0x114);

struct s_global_material_underwater_proxy
{
	// when this material is encounted under a material of this type ...
	c_string_id underwater_material;

	// ... we report it as this material
	c_string_id proxy_material;

	int16 underwater_material_type;
	int16 proxy_material_type;
};
COMPILE_ASSERT(sizeof(s_global_material_underwater_proxy) == 0xC);

struct s_global_material_definition
{
	c_string_id name;
	c_string_id parent_name;
	int16 runtime_material_index;
	c_flags<e_global_material_bit, uns16, k_number_of_global_material_bits> flags;
	c_string_id general_armor;
	c_string_id specific_armor;
	s_material_physics_properties physics_properties;
	c_typed_tag_reference<BREAKABLE_SURFACE_TAG, INVALID_TAG> breakable_surface;
	s_global_material_sweeteners_definition sweeteners;
	c_typed_tag_reference<MATERIAL_EFFECTS_TAG, INVALID_TAG> material_effects;
	c_typed_tag_block<s_global_material_underwater_proxy> underwater_proxies_block;
	c_string_id footsteps_in_rain_material_name; // When it's raining, use this as the footstep material
	int16 runtime_rain_material_index;
	byte ASDF[0x2]; // pad

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_global_material_definition) == 0x178);

