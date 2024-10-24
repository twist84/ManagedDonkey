#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_global_material_flags
{
	_global_material_flammable_bit,
	_global_material_biomass_bit,
	_global_material_rad_xfer_interior_bit,

	k_global_material_flags
};

struct s_material_physics_drag_properties
{
	// drag= k*power(q*velocity, e)
	// 
	// defaults to k= .5, q= 1.0, e= 1.0
	real drag_k;
	real drag_q;
	real drag_e;

	// objects of this flotation category have this density relative to being in this material (1.0f==neutral)
	byte _default[0x4]; // pad
	real super_floater;
	real floater;
	real neutral;
	real sinker;
	real super_sinker;
	byte none[0x4]; // pad
};
static_assert(sizeof(s_material_physics_drag_properties) == 0x28);

struct s_material_physics_properties_definition
{
	dword_flags flags;
	real friction;
	real_fraction restitution;
	real density;
	c_typed_tag_block<s_material_physics_drag_properties> drag_block;
};
static_assert(sizeof(s_material_physics_properties_definition) == 0x1C);

enum e_materials_sweeteners_inheritance_flags
{
	_materials_sweeteners_inheritance_sound_small_bit,
	_materials_sweeteners_inheritance_sound_medium_bit,
	_materials_sweeteners_inheritance_sound_large_bit,
	_materials_sweeteners_inheritance_sound_rolling_bit,
	_materials_sweeteners_inheritance_sound_grinding_bit,
	_materials_sweeteners_inheritance_sound_melee_small_bit,
	_materials_sweeteners_inheritance_sound_melee_bit,
	_materials_sweeteners_inheritance_sound_melee_large_bit,
	_materials_sweeteners_inheritance_effect_small_bit,
	_materials_sweeteners_inheritance_effect_medium_bit,
	_materials_sweeteners_inheritance_effect_large_bit,
	_materials_sweeteners_inheritance_effect_rolling_bit,
	_materials_sweeteners_inheritance_effect_grinding_bit,
	_materials_sweeteners_inheritance_effect_melee_bit,
	_materials_sweeteners_inheritance_water_ripple_small_bit,
	_materials_sweeteners_inheritance_water_ripple_medium_bit,
	_materials_sweeteners_inheritance_water_ripple_large_bit,

	k_materials_sweeteners_inheritance_flags
};

struct s_global_material_sweeteners_definition
{
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_small;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_medium;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_large;
	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> sound_sweetener_rolling;
	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> sound_sweetener_grinding;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_melee_small;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_melee;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sound_sweetener_melee_large;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_small;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_medium;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_large;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_rolling;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_grinding;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> effect_sweetener_melee;
	c_typed_tag_reference<RENDER_WATER_RIPPLE_TAG, INVALID_TAG> water_ripple_small;
	c_typed_tag_reference<RENDER_WATER_RIPPLE_TAG, INVALID_TAG> water_ripple_medium;
	c_typed_tag_reference<RENDER_WATER_RIPPLE_TAG, INVALID_TAG> water_ripple_large;

	// sweetener inheritance flags
	// 
	// when a sweetener inheritance flag is set the sound\effect is not inherited from the parent material.  If you leave the sweetener blank and set the flag than no effect\sound will play

	c_flags<e_materials_sweeteners_inheritance_flags, dword, k_materials_sweeteners_inheritance_flags> sweetener_inheritance_flags;

	void update_reference_names();
};
static_assert(sizeof(s_global_material_sweeteners_definition) == 0x114);

struct s_global_material_underwater_proxy
{
	// when this material is encounted under a material of this type ...
	c_string_id underwater_material;

	// ... we report it as this material
	c_string_id proxy_material;

	short underwater_material_type;
	short proxy_material_type;
};
static_assert(sizeof(s_global_material_underwater_proxy) == 0xC);

struct s_global_material_definition
{
	c_string_id name;
	c_string_id parent_name;
	short runtime_material_index;
	c_flags<e_global_material_flags, word, k_global_material_flags> flags;
	c_string_id general_armor;
	c_string_id specific_armor;
	s_material_physics_properties_definition physics_properties;
	c_typed_tag_reference<BREAKABLE_SURFACE_TAG, INVALID_TAG> breakable_surface;
	s_global_material_sweeteners_definition sweeteners;
	c_typed_tag_reference<MATERIAL_EFFECTS_TAG, INVALID_TAG> material_effects;
	c_typed_tag_block<s_global_material_underwater_proxy> underwater_proxies_block;
	c_string_id footsteps_in_rain_material_name; // When it's raining, use this as the footstep material
	short runtime_rain_material_index;
	byte ASDF[0x2]; // pad

	void update_reference_names();
};
static_assert(sizeof(s_global_material_definition) == 0x178);

