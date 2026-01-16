#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/string_ids.hpp"
#include "tag_files/tag_group_definitions.hpp"

#define INVALID_TAG NONE
#define ACHIEVEMENTS_TAG 'achi'
#define AI_DIALOGUE_GLOBALS_TAG 'adlg'
#define AI_GLOBALS_TAG 'aigl'
#define AI_MISSION_DIALOGUE_TAG 'mdlg'
#define ANTENNA_TAG 'ant!'
#define AREA_SCREEN_EFFECT_TAG 'sefc'
#define ARMOR_SOUNDS_TAG 'arms'
#define BEAM_SYSTEM_TAG 'beam'
#define BINK_TAG 'bink'
#define BIPED_TAG 'bipd'
#define BITMAP_TAG 'bitm'
#define BREAKABLE_SURFACE_TAG 'bsdt'
#define CACHE_FILE_GLOBAL_TAGS_TAG 'cfgt'
#define CACHE_FILE_RESOURCE_GESTALT_TAG 'zone'
#define CACHE_FILE_SOUND_TAG '$#! '
#define CAMERA_FX_SETTINGS_TAG 'cfxs'
#define CAMERA_TRACK_TAG 'trak'
#define CAMO_TAG 'cmoe'
#define CELLULAR_AUTOMATA2D_TAG 'whip'
#define CELLULAR_AUTOMATA_TAG 'devo'
#define CHARACTER_TAG 'char'
#define CHOCOLATE_MOUNTAIN_NEW_TAG 'chmt'
#define CHUD_ANIMATION_DEFINITION_TAG 'chad'
#define CHUD_DEFINITION_TAG 'chdt'
#define CHUD_GLOBALS_DEFINITION_TAG 'chgd'
#define CHUD_WIDGET_PARALLAX_DATA_TAG 'cprl'
#define CINEMATIC_SCENE_TAG 'cisc'
#define CINEMATIC_TAG 'cine'
#define CLOTH_TAG 'clwd'
#define COLLISION_DAMAGE_TAG 'cddf'
#define COLLISION_MODEL_TAG 'coll'
#define COLOR_TABLE_TAG 'colo'
#define CONTRAIL_SYSTEM_TAG 'cntl'
#define CORTANA_EFFECT_DEFINITION_TAG 'crte'
#define CRATE_TAG 'bloc'
#define CREATURE_TAG 'crea'
#define DAMAGE_EFFECT_TAG 'jpt!'
#define DAMAGE_RESPONSE_DEFINITION_TAG 'drdf'
#define DECAL_SYSTEM_TAG 'decs'
#define DECORATOR_SET_TAG 'dctr'
#define DETAIL_OBJECT_COLLECTION_TAG 'dobc'
#define DEVICE_ARG_DEVICE_TAG 'argd'
#define DEVICE_CONTROL_TAG 'ctrl'
#define DEVICE_MACHINE_TAG 'mach'
#define DEVICE_TAG 'devi'
#define DEVICE_TERMINAL_TAG 'term'
#define DIALOGUE_TAG 'udlg'
#define EFFECT_GLOBALS_TAG 'effg'
#define EFFECT_SCENERY_TAG 'efsc'
#define EFFECT_TAG 'effe'
#define EQUIPMENT_TAG 'eqip'
#define FLOCK_TAG 'flck'
#define FLUID_DYNAMICS_TAG 'fldy'
#define FORMATION_TAG 'form'
#define FRAGMENT_TAG 'frag'
#define GAME_ENGINE_SETTINGS_DEFINITION_TAG 'wezr'
#define GAME_PROGRESSION_TAG 'gpdt'
#define GFX_TEXTURES_LIST_TAG 'gfxt'
#define GIANT_TAG 'gint'
#define GLOBALS_TAG 'matg'
#define GLOBAL_PIXEL_SHADER_TAG 'glps'
#define GLOBAL_VERTEX_SHADER_TAG 'glvs'
#define GUI_BITMAP_WIDGET_DEFINITION_TAG 'bmp3'
#define GUI_BUTTON_KEY_DEFINITION_TAG 'bkey'
#define GUI_DATASOURCE_DEFINITION_TAG 'dsrc'
#define GUI_GROUP_WIDGET_DEFINITION_TAG 'grup'
#define GUI_LIST_WIDGET_DEFINITION_TAG 'lst3'
#define GUI_MODEL_WIDGET_DEFINITION_TAG 'mdl3'
#define GUI_SCREEN_WIDGET_DEFINITION_TAG 'scn3'
#define GUI_SKIN_DEFINITION_TAG 'skn3'
#define GUI_TEXT_WIDGET_DEFINITION_TAG 'txt3'
#define GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG 'wacd'
#define GUI_WIDGET_ANIMATION_DEFINITION_TAG 'wgan'
#define GUI_WIDGET_COLOR_ANIMATION_DEFINITION_TAG 'wclr'
#define GUI_WIDGET_FONT_ANIMATION_DEFINITION_TAG 'wfon'
#define GUI_WIDGET_POSITION_ANIMATION_DEFINITION_TAG 'wpos'
#define GUI_WIDGET_ROTATION_ANIMATION_DEFINITION_TAG 'wrot'
#define GUI_WIDGET_SCALE_ANIMATION_DEFINITION_TAG 'wscl'
#define GUI_WIDGET_SPRITE_ANIMATION_DEFINITION_TAG 'wspr'
#define GUI_WIDGET_TEXTURE_COORDINATE_ANIMATION_DEFINITION_TAG 'wtuv'
#define HLSL_INCLUDE_TAG 'hlsl'
#define INPUT_GLOBALS_TAG 'inpg'
#define ITEM_COLLECTION_TAG 'itmc'
#define ITEM_TAG 'item'
#define LEAF_SYSTEM_TAG 'lswd'
#define LENS_FLARE_TAG 'lens'
#define LIGHT_TAG 'ligh'
#define LIGHT_VOLUME_SYSTEM_TAG 'ltvl'
#define MATERIAL_EFFECTS_TAG 'foot'
#define METER_TAG 'metr'
#define MODEL_ANIMATION_GRAPH_TAG 'jmad'
#define MODEL_TAG 'hlmt'
#define MUFFIN_TAG 'mffn'
#define MULTILINGUAL_UNICODE_STRING_LIST_TAG 'unic'
#define MULTIPLAYER_GLOBALS_TAG 'mulg'
#define MULTIPLAYER_SCENARIO_DESCRIPTION_TAG 'mply'
#define MULTIPLAYER_VARIANT_SETTINGS_INTERFACE_DEFINITION_TAG 'goof'
#define NEW_CINEMATIC_LIGHTING_TAG 'nclt'
#define OBJECT_TAG 'obje'
#define PARTICLE_EMITTER_CUSTOM_POINTS_TAG 'pecp'
#define PARTICLE_MODEL_TAG 'pmdf'
#define PARTICLE_PHYSICS_TAG 'pmov'
#define PARTICLE_TAG 'prt3'
#define PATCHY_FOG_TAG 'fpch'
#define PERFORMANCE_THROTTLES_TAG 'perf'
#define PHYSICS_MODEL_TAG 'phmo'
#define PIXEL_SHADER_TAG 'pixl'
#define PLANAR_FOG_TAG 'fog '
#define PODIUM_SETTINGS_TAG 'pdm!'
#define POINT_PHYSICS_TAG 'pphy'
#define PROJECTILE_TAG 'proj'
#define RASTERIZER_CACHE_FILE_GLOBALS_TAG 'draw'
#define RASTERIZER_GLOBALS_TAG 'rasg'
#define RENDER_METHOD_DEFINITION_TAG 'rmdf'
#define RENDER_METHOD_OPTION_TAG 'rmop'
#define RENDER_METHOD_TAG 'rm  '
#define RENDER_METHOD_TEMPLATE_TAG 'rmt2'
#define RENDER_MODEL_TAG 'mode'
#define RENDER_WATER_RIPPLE_TAG 'rwrd'
#define SANDBOX_TEXT_VALUE_PAIR_DEFINITION_TAG 'jmrq'
#define SCENARIO_AI_RESOURCE_TAG 'ai**'
#define SCENARIO_BIPEDS_RESOURCE_TAG '*ipd'
#define SCENARIO_CINEMATICS_RESOURCE_TAG 'cin*'
#define SCENARIO_CLUSTER_DATA_RESOURCE_TAG 'clu*'
#define SCENARIO_COMMENTS_RESOURCE_TAG '/**/'
#define SCENARIO_CREATURE_RESOURCE_TAG '*rea'
#define SCENARIO_CUBEMAP_RESOURCE_TAG 'cub*'
#define SCENARIO_DECALS_RESOURCE_TAG 'dec*'
#define SCENARIO_DECORATORS_RESOURCE_TAG 'dc*s'
#define SCENARIO_DEVICES_RESOURCE_TAG 'dgr*'
#define SCENARIO_EFFECT_SCENERY_RESOURCE_TAG '*fsc'
#define SCENARIO_EQUIPMENT_RESOURCE_TAG '*qip'
#define SCENARIO_FAUX_DATA_TAG 'sFdT'
#define SCENARIO_HS_SOURCE_FILE_TAG 'hsc*'
#define SCENARIO_LIGHTMAP_BSP_DATA_TAG 'Lbsp'
#define SCENARIO_LIGHTMAP_TAG 'sLdT'
#define SCENARIO_LIGHTS_RESOURCE_TAG '*igh'
#define SCENARIO_PDA_TAG 'spda'
#define SCENARIO_SCENERY_RESOURCE_TAG '*cen'
#define SCENARIO_SKY_REFERENCES_RESOURCE_TAG 'sky*'
#define SCENARIO_SOUND_SCENERY_RESOURCE_TAG '*sce'
#define SCENARIO_STRUCTURE_BSP_TAG 'sbsp'
#define SCENARIO_STRUCTURE_LIGHTING_INFO_TAG 'stli'
#define SCENARIO_STRUCTURE_LIGHTING_RESOURCE_TAG 'sslt'
#define SCENARIO_TAG 'scnr'
#define SCENARIO_TRIGGER_VOLUMES_RESOURCE_TAG 'trg*'
#define SCENARIO_VEHICLES_RESOURCE_TAG '*ehi'
#define SCENARIO_WEAPONS_RESOURCE_TAG '*eap'
#define SCENERY_TAG 'scen'
#define SCREEN_EFFECT_TAG 'egor'
#define SHADER_BEAM_TAG 'rmb '
#define SHADER_BLACK_TAG 'rmbk'
#define SHADER_CONTRAIL_TAG 'rmc\0'
#define SHADER_CORTANA_TAG 'rmct'
#define SHADER_CUSTOM_TAG 'rmcs'
#define SHADER_DECAL_TAG 'rmd '
#define SHADER_FOLIAGE_TAG 'rmfl'
#define SHADER_HALOGRAM_TAG 'rmhg'
#define SHADER_LIGHT_VOLUME_TAG 'rmlv'
#define SHADER_PARTICLE_TAG 'rmp\0'
#define SHADER_SCREEN_TAG 'rmss'
#define SHADER_SKIN_TAG 'rmsk'
#define SHADER_TAG 'rmsh'
#define SHADER_TERRAIN_TAG 'rmtr'
#define SHADER_WATER_TAG 'rmw '
#define SHADER_ZONLY_TAG 'rmzo'
#define SHIELD_IMPACT_TAG 'shit'
#define SKY_ATM_PARAMETERS_TAG 'skya'
#define SOUND_CACHE_FILE_GESTALT_TAG 'ugh!'
#define SOUND_CLASSES_TAG 'sncl'
#define SOUND_DIALOGUE_CONSTANTS_TAG 'spk!'
#define SOUND_EFFECT_COLLECTION_TAG 'sfx+'
#define SOUND_EFFECT_TEMPLATE_TAG '<fx>'
#define SOUND_ENVIRONMENT_TAG 'snde'
#define SOUND_GLOBAL_PROPAGATION_TAG 'sgp!'
#define SOUND_LOOPING_TAG 'lsnd'
#define SOUND_MIX_TAG 'snmx'
#define SOUND_SCENERY_TAG 'ssce'
#define SOUND_TAG 'snd!'
#define SOUND_UI_SOUNDS_TAG 'sus!'
#define SQUAD_TEMPLATE_TAG 'sqtm'
#define STEREO_SYSTEM_TAG 'BooM'
#define STRUCTURE_DESIGN_TAG 'sddt'
#define STRUCTURE_SEAMS_TAG 'stse'
#define STYLE_TAG 'styl'
#define SURVIVAL_MODE_GLOBALS_TAG 'smdt'
#define TAG_TEMPLATE_UNIT_TEST_TAG 'uttt'
#define TEST_TAG_TAG 'ttag'
#define TEXTURE_RENDER_LIST_TAG 'trdf'
#define TEXT_VALUE_PAIR_DEFINITION_TAG 'sily'
#define UNIT_TAG 'unit'
#define USER_INTERFACE_FOURTH_WALL_TIMING_DEFINITION_TAG 'fwtg'
#define USER_INTERFACE_GLOBALS_DEFINITION_TAG 'wgtz'
#define USER_INTERFACE_SHARED_GLOBALS_DEFINITION_TAG 'wigl'
#define USER_INTERFACE_SOUNDS_DEFINITION_TAG 'uise'
#define VEHICLE_COLLECTION_TAG 'vehc'
#define VEHICLE_TAG 'vehi'
#define VERTEX_SHADER_TAG 'vtsh'
#define VFILES_LIST_TAG 'vfsl'
#define VISION_MODE_TAG 'vmdx'
#define WEAPON_TAG 'weap'
#define WIND_TAG 'wind'

extern bool const print_reference_updates;

#define UPDATE_REFERENCE_NAME(_reference) { _reference.get_name(); if (_reference.name) { if (print_reference_updates) c_console::write_line("\t%s: '%s.%s'", #_reference, _reference.name, _reference.get_group_name()); } }
#define UPDATE_ARRAY_REFERENCE_NAMES(_array) { for (int32 i = 0; i < NUMBEROF(_array); i++) { if (print_reference_updates) c_console::write_line("\t%s[%d]: '%s.%s'", #_array, i, _array[i].name, _array[i].get_group_name()); } }
#define UPDATE_BLOCK_REFERENCE_NAMES(_block) { for (int32 i = 0; i < _block.count; i++) { if (print_reference_updates) c_console::write_line("\t%s[%d]:", #_block, i); auto& _element = _block[i]; _element.update_reference_names(); } }
#define UPDATE_STRUCT_REFERENCE_NAMES(_struct) { if (print_reference_updates) c_console::write_line("\t%s: ", #_struct); _struct.update_reference_names(); }
#define UPDATE_STRUCT_POINTER_REFERENCE_NAMES(_struct) { if (print_reference_updates) c_console::write_line("\t%s: ", #_struct); _struct->update_reference_names(); }
#define UPDATE_STATIC_RUNTIME_DATA(_struct) { if (print_reference_updates) c_console::write_line("\t%s: ", #_struct); _struct->update_static_runtime_data(); }

int32 const k_tag_file_name_length = 256;

struct s_tag_block
{
	int32 count;
	union
	{
		void* address;
		uns8* base;
	};
	int32 : 32; // uns8* definition;
};
COMPILE_ASSERT(sizeof(s_tag_block) == 0xC);

struct s_tag_reference
{
	tag group_tag = _tag_none;
	const char* name;
	int32 name_length;
	int32 index = NONE;

	void* get_definition();

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(get_definition());
	}

	const char* get_name();
	const char* get_group_name();
};
COMPILE_ASSERT(sizeof(s_tag_reference) == 0x10);

struct s_tag_data
{
	int32 size;
	uns32 internal_flags;
	uns32 file_offset;

	union
	{
		void* address;
		uns8* base;
	};

	int32 : 32; // uns8* definition;
};
COMPILE_ASSERT(sizeof(s_tag_data) == 0x14);

template<typename t_element_type, uns32 ...t_extra>
//using c_typed_tag_block = s_tag_block;
class c_typed_tag_block :
	public s_tag_block
{
public:

	t_element_type* begin()
	{
		return static_cast<t_element_type*>(address);
	}

	const t_element_type* begin() const
	{
		return static_cast<t_element_type*>(address);
	}

	t_element_type* end()
	{
		return static_cast<t_element_type*>(address) + s_tag_block::count;
	}

	const t_element_type* end() const
	{
		return static_cast<const t_element_type*>(address) + s_tag_block::count;
	}

	t_element_type& operator[](int32 index)
	{
		ASSERT(VALID_INDEX(index, s_tag_block::count));

		return static_cast<t_element_type*>(address)[index];
	}

	t_element_type& operator[](int32 index) const
	{
		ASSERT(VALID_INDEX(index, s_tag_block::count));

		return static_cast<t_element_type*>(address)[index];
	}

	void clear()
	{
		csmemset(address, 0, sizeof(t_element_type) * s_tag_block::count);
		s_tag_block::count = 0;
	}
};

template<tag ...k_group_tags>
using c_typed_tag_reference = s_tag_reference;

template<typename t_data_type, uns32 ...t_extra>
//using c_typed_tag_data = s_tag_data;
class c_typed_tag_data :
	public s_tag_data
{
public:
	t_data_type* get()
	{
		return reinterpret_cast<t_data_type*>(base);
	}
};

struct s_cache_file_tag_resource_data;
struct s_tag_resource
{
	union
	{
		uns32 resource_handle;
		s_cache_file_tag_resource_data* resource_data;
	};

	uns32 definition_address;
};
COMPILE_ASSERT(sizeof(s_tag_resource) == 0x8);

template<typename t_resource_type, uns32 ...t_extra>
class c_typed_tag_resource :
	public s_tag_resource
{
public:
	t_resource_type* get()
	{
		return reinterpret_cast<t_resource_type*>(resource_handle);
	}
};
COMPILE_ASSERT(sizeof(c_typed_tag_resource<void>) == sizeof(s_tag_resource));

struct s_cache_file_tag_group
{
	tag group_tag;
	tag parent_group_tags[2];
	c_string_id name;

	bool is_group(tag group_tag_)
	{
		return group_tag_ == group_tag || group_tag_ == parent_group_tags[0] || group_tag_ == parent_group_tags[1];
	}

	bool operator==(tag group_tag_)
	{
		return is_group(group_tag_);
	}

	bool operator!=(tag group_tag_)
	{
		return !is_group(group_tag_);
	}
};
COMPILE_ASSERT(sizeof(s_cache_file_tag_group) == 0x10);

extern void* __cdecl tag_block_get_element_with_size(const s_tag_block* block, int32 index, int32 size);
extern void* __cdecl tag_data_get_pointer(const s_tag_data* data, int32 offset, int32 size);
extern void __cdecl tag_load_missing_tags_report();
extern const char* __cdecl tag_name_strip_path(const char* path);
extern const wchar_t* __cdecl tag_name_strip_path(const wchar_t* path);
extern tag group_name_to_group_tag(const char* group_name);
extern void tag_reference_set(s_tag_reference* reference, tag group_tag, const char* name);
extern void tag_block_set_elements(s_tag_block* block, void* elements);
extern void tag_block_set_element_count(s_tag_block* block, int32 count);

static s_cache_file_tag_group global_tag_groups[] =
{
	{
		.group_tag = ACHIEVEMENTS_TAG,
		.name = STRING_ID(global, achievements)
	},
	{
		.group_tag = AI_DIALOGUE_GLOBALS_TAG,
		.name = STRING_ID(global, ai_dialogue_globals)
	},
	{
		.group_tag = AI_GLOBALS_TAG,
		.name = STRING_ID(global, ai_globals)
	},
	{
		.group_tag = AI_MISSION_DIALOGUE_TAG,
		.name = STRING_ID(global, ai_mission_dialogue)
	},
	{
		.group_tag = ANTENNA_TAG,
		.name = STRING_ID(global, antenna)
	},
	{
		.group_tag = AREA_SCREEN_EFFECT_TAG,
		.name = STRING_ID(global, area_screen_effect)
	},
	{
		.group_tag = ARMOR_SOUNDS_TAG,
		.name = STRING_ID(global, armor_sounds)
	},
	{
		.group_tag = BEAM_SYSTEM_TAG,
		.name = STRING_ID(global, beam_system)
	},
	{
		.group_tag = BINK_TAG,
		.name = STRING_ID(global, bink)
	},
	{
		.group_tag = BIPED_TAG,
		.parent_group_tags = { UNIT_TAG, OBJECT_TAG },
		.name = STRING_ID(global, biped)
	},
	{
		.group_tag = BITMAP_TAG,
		.name = STRING_ID(global, bitmap)
	},
	{
		.group_tag = BREAKABLE_SURFACE_TAG,
		.name = STRING_ID(global, breakable_surface)
	},
	{
		.group_tag = CACHE_FILE_GLOBAL_TAGS_TAG,
		.name = STRING_ID(global, cache_file_global_tags)
	},
	{
		.group_tag = CACHE_FILE_RESOURCE_GESTALT_TAG,
		.name = STRING_ID(global, cache_file_resource_gestalt)
	},
	{
		.group_tag = CACHE_FILE_SOUND_TAG,
		.name = STRING_ID(global, cache_file_sound)
	},
	{
		.group_tag = CAMERA_FX_SETTINGS_TAG,
		.name = STRING_ID(global, camera_fx_settings)
	},
	{
		.group_tag = CAMERA_TRACK_TAG,
		.name = STRING_ID(global, camera_track)
	},
	{
		.group_tag = CAMO_TAG,
		.name = STRING_ID(global, camo)
	},
	{
		.group_tag = CELLULAR_AUTOMATA2D_TAG,
		.name = STRING_ID(global, cellular_automata2d)
	},
	{
		.group_tag = CELLULAR_AUTOMATA_TAG,
		.name = STRING_ID(global, cellular_automata)
	},
	{
		.group_tag = CHARACTER_TAG,
		.name = STRING_ID(global, character)
	},
	{
		.group_tag = CHOCOLATE_MOUNTAIN_NEW_TAG,
		.name = STRING_ID(global, chocolate_mountain_new)
	},
	{
		.group_tag = CHUD_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, chud_animation_definition)
	},
	{
		.group_tag = CHUD_DEFINITION_TAG,
		.name = STRING_ID(global, chud_definition)
	},
	{
		.group_tag = CHUD_GLOBALS_DEFINITION_TAG,
		.name = STRING_ID(global, chud_globals_definition)
	},
	{
		.group_tag = CHUD_WIDGET_PARALLAX_DATA_TAG,
		.name = STRING_ID(global, chud_widget_parallax_data)
	},
	{
		.group_tag = CINEMATIC_SCENE_TAG,
		.name = STRING_ID(global, cinematic_scene)
	},
	{
		.group_tag = CINEMATIC_TAG,
		.name = STRING_ID(global, cinematic)
	},
	{
		.group_tag = CLOTH_TAG,
		.name = STRING_ID(global, cloth)
	},
	{
		.group_tag = COLLISION_DAMAGE_TAG,
		.name = STRING_ID(global, collision_damage)
	},
	{
		.group_tag = COLLISION_MODEL_TAG,
		.name = STRING_ID(global, collision_model)
	},
	{
		.group_tag = COLOR_TABLE_TAG,
		.name = STRING_ID(global, color_table)
	},
	{
		.group_tag = CONTRAIL_SYSTEM_TAG,
		.name = STRING_ID(global, contrail_system)
	},
	{
		.group_tag = CORTANA_EFFECT_DEFINITION_TAG,
		.name = STRING_ID(global, cortana_effect_definition)
	},
	{
		.group_tag = CRATE_TAG,
		.name = STRING_ID(global, crate)
	},
	{
		.group_tag = CREATURE_TAG,
		.name = STRING_ID(global, creature)
	},
	{
		.group_tag = DAMAGE_EFFECT_TAG,
		.name = STRING_ID(global, damage_effect)
	},
	{
		.group_tag = DAMAGE_RESPONSE_DEFINITION_TAG,
		.name = STRING_ID(global, damage_response_definition)
	},
	{
		.group_tag = DECAL_SYSTEM_TAG,
		.name = STRING_ID(global, decal_system)
	},
	{
		.group_tag = DECORATOR_SET_TAG,
		.name = STRING_ID(global, decorator_set)
	},
	{
		.group_tag = DETAIL_OBJECT_COLLECTION_TAG,
		.name = STRING_ID(global, detail_object_collection)
	},
	{
		.group_tag = DEVICE_ARG_DEVICE_TAG,
		.name = STRING_ID(global, device_arg_device)
	},
	{
		.group_tag = DEVICE_CONTROL_TAG,
		.name = STRING_ID(global, device_control)
	},
	{
		.group_tag = DEVICE_MACHINE_TAG,
		.name = STRING_ID(global, device_machine)
	},
	{
		.group_tag = DEVICE_TAG,
		.name = STRING_ID(global, device)
	},
	{
		.group_tag = DEVICE_TERMINAL_TAG,
		.name = STRING_ID(global, device_terminal)
	},
	{
		.group_tag = DIALOGUE_TAG,
		.name = STRING_ID(global, dialogue)
	},
	{
		.group_tag = EFFECT_GLOBALS_TAG,
		.name = STRING_ID(global, effect_globals)
	},
	{
		.group_tag = EFFECT_SCENERY_TAG,
		.name = STRING_ID(global, effect_scenery)
	},
	{
		.group_tag = EFFECT_TAG,
		.name = STRING_ID(global, effect)
	},
	{
		.group_tag = EQUIPMENT_TAG,
		.name = STRING_ID(global, equipment)
	},
	{
		.group_tag = FLOCK_TAG,
		.name = STRING_ID(global, flock)
	},
	{
		.group_tag = FLUID_DYNAMICS_TAG,
		.name = STRING_ID(global, fluid_dynamics)
	},
	{
		.group_tag = FORMATION_TAG,
		.name = STRING_ID(global, formation)
	},
	{
		.group_tag = FRAGMENT_TAG,
		.name = STRING_ID(global, fragment)
	},
	{
		.group_tag = GAME_ENGINE_SETTINGS_DEFINITION_TAG,
		.name = STRING_ID(global, game_engine_settings_definition)
	},
	{
		.group_tag = GAME_PROGRESSION_TAG,
		.name = STRING_ID(global, game_progression)
	},
	{
		.group_tag = GFX_TEXTURES_LIST_TAG,
		.name = STRING_ID(global, gfx_textures_list)
	},
	{
		.group_tag = GIANT_TAG,
		.name = STRING_ID(global, giant)
	},
	{
		.group_tag = GLOBALS_TAG,
		.name = STRING_ID(global, globals)
	},
	{
		.group_tag = GLOBAL_PIXEL_SHADER_TAG,
		.name = STRING_ID(global, global_pixel_shader)
	},
	{
		.group_tag = GLOBAL_VERTEX_SHADER_TAG,
		.name = STRING_ID(global, global_vertex_shader)
	},
	{
		.group_tag = GUI_BITMAP_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_bitmap_widget_definition)
	},
	{
		.group_tag = GUI_BUTTON_KEY_DEFINITION_TAG,
		.name = STRING_ID(global, gui_button_key_definition)
	},
	{
		.group_tag = GUI_DATASOURCE_DEFINITION_TAG,
		.name = STRING_ID(global, gui_datasource_definition)
	},
	{
		.group_tag = GUI_GROUP_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_group_widget_definition)
	},
	{
		.group_tag = GUI_LIST_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_list_widget_definition)
	},
	{
		.group_tag = GUI_MODEL_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_model_widget_definition)
	},
	{
		.group_tag = GUI_SCREEN_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_screen_widget_definition)
	},
	{
		.group_tag = GUI_SKIN_DEFINITION_TAG,
		.name = STRING_ID(global, gui_skin_definition)
	},
	{
		.group_tag = GUI_TEXT_WIDGET_DEFINITION_TAG,
		.name = STRING_ID(global, gui_text_widget_definition)
	},
	{
		.group_tag = GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_animation_collection_definition)
	},
	{
		.group_tag = GUI_WIDGET_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_COLOR_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_color_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_FONT_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_font_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_POSITION_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_position_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_ROTATION_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_rotation_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_SCALE_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_scale_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_SPRITE_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_sprite_animation_definition)
	},
	{
		.group_tag = GUI_WIDGET_TEXTURE_COORDINATE_ANIMATION_DEFINITION_TAG,
		.name = STRING_ID(global, gui_widget_texture_coordinate_animation_definition)
	},
	{
		.group_tag = HLSL_INCLUDE_TAG,
		.name = STRING_ID(global, hlsl_include)
	},
	{
		.group_tag = INPUT_GLOBALS_TAG,
		.name = STRING_ID(global, input_globals)
	},
	{
		.group_tag = ITEM_COLLECTION_TAG,
		.name = STRING_ID(global, item_collection)
	},
	{
		.group_tag = ITEM_TAG,
		.parent_group_tags = { OBJECT_TAG },
		.name = STRING_ID(global, item)
	},
	{
		.group_tag = LEAF_SYSTEM_TAG,
		.name = STRING_ID(global, leaf_system)
	},
	{
		.group_tag = LENS_FLARE_TAG,
		.name = STRING_ID(global, lens_flare)
	},
	{
		.group_tag = LIGHT_TAG,
		.name = STRING_ID(global, light)
	},
	{
		.group_tag = LIGHT_VOLUME_SYSTEM_TAG,
		.name = STRING_ID(global, light_volume_system)
	},
	{
		.group_tag = MATERIAL_EFFECTS_TAG,
		.name = STRING_ID(global, material_effects)
	},
	{
		.group_tag = METER_TAG,
		.name = STRING_ID(global, meter)
	},
	{
		.group_tag = MODEL_ANIMATION_GRAPH_TAG,
		.name = STRING_ID(global, model_animation_graph)
	},
	{
		.group_tag = MODEL_TAG,
		.name = STRING_ID(global, model)
	},
	{
		.group_tag = MUFFIN_TAG,
		.name = STRING_ID(global, muffin)
	},
	{
		.group_tag = MULTILINGUAL_UNICODE_STRING_LIST_TAG,
		.name = STRING_ID(global, multilingual_unicode_string_list)
	},
	{
		.group_tag = MULTIPLAYER_GLOBALS_TAG,
		.name = STRING_ID(global, multiplayer_globals)
	},
	{
		.group_tag = MULTIPLAYER_SCENARIO_DESCRIPTION_TAG,
		.name = STRING_ID(global, multiplayer_scenario_description)
	},
	{
		.group_tag = MULTIPLAYER_VARIANT_SETTINGS_INTERFACE_DEFINITION_TAG,
		.name = STRING_ID(global, multiplayer_variant_settings_interface_definition)
	},
	{
		.group_tag = NEW_CINEMATIC_LIGHTING_TAG,
		.name = STRING_ID(global, new_cinematic_lighting)
	},
	{
		.group_tag = OBJECT_TAG,
		.name = STRING_ID(global, object)
	},
	{
		.group_tag = PARTICLE_EMITTER_CUSTOM_POINTS_TAG,
		.name = STRING_ID(global, particle_emitter_custom_points)
	},
	{
		.group_tag = PARTICLE_MODEL_TAG,
		.name = STRING_ID(global, particle_model)
	},
	{
		.group_tag = PARTICLE_PHYSICS_TAG,
		.name = STRING_ID(global, particle_physics)
	},
	{
		.group_tag = PARTICLE_TAG,
		.name = STRING_ID(global, particle)
	},
	{
		.group_tag = PATCHY_FOG_TAG,
		.name = STRING_ID(global, patchy_fog)
	},
	{
		.group_tag = PERFORMANCE_THROTTLES_TAG,
		.name = STRING_ID(global, performance_throttles)
	},
	{
		.group_tag = PHYSICS_MODEL_TAG,
		.name = STRING_ID(global, physics_model)
	},
	{
		.group_tag = PIXEL_SHADER_TAG,
		.name = STRING_ID(global, pixel_shader)
	},
	{
		.group_tag = PLANAR_FOG_TAG,
		.name = STRING_ID(global, planar_fog)
	},
	{
		.group_tag = PODIUM_SETTINGS_TAG,
		.name = STRING_ID(global, podium_settings)
	},
	{
		.group_tag = POINT_PHYSICS_TAG,
		.name = STRING_ID(global, point_physics)
	},
	{
		.group_tag = PROJECTILE_TAG,
		.name = STRING_ID(global, projectile)
	},
	{
		.group_tag = RASTERIZER_CACHE_FILE_GLOBALS_TAG,
		.name = STRING_ID(global, rasterizer_cache_file_globals)
	},
	{
		.group_tag = RASTERIZER_GLOBALS_TAG,
		.name = STRING_ID(global, rasterizer_globals)
	},
	{
		.group_tag = RENDER_METHOD_DEFINITION_TAG,
		.name = STRING_ID(global, render_method_definition)
	},
	{
		.group_tag = RENDER_METHOD_OPTION_TAG,
		.name = STRING_ID(global, render_method_option)
	},
	{
		.group_tag = RENDER_METHOD_TAG,
		.name = STRING_ID(global, render_method)
	},
	{
		.group_tag = RENDER_METHOD_TEMPLATE_TAG,
		.name = STRING_ID(global, render_method_template)
	},
	{
		.group_tag = RENDER_MODEL_TAG,
		.name = STRING_ID(global, render_model)
	},
	{
		.group_tag = RENDER_WATER_RIPPLE_TAG,
		.name = STRING_ID(global, render_water_ripple)
	},
	{
		.group_tag = SANDBOX_TEXT_VALUE_PAIR_DEFINITION_TAG,
		.name = STRING_ID(global, sandbox_text_value_pair_definition)
	},
	{
		.group_tag = SCENARIO_AI_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_ai_resource)
	},
	{
		.group_tag = SCENARIO_BIPEDS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_bipeds_resource)
	},
	{
		.group_tag = SCENARIO_CINEMATICS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_cinematics_resource)
	},
	{
		.group_tag = SCENARIO_CLUSTER_DATA_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_cluster_data_resource)
	},
	{
		.group_tag = SCENARIO_COMMENTS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_comments_resource)
	},
	{
		.group_tag = SCENARIO_CREATURE_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_creature_resource)
	},
	{
		.group_tag = SCENARIO_CUBEMAP_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_cubemap_resource)
	},
	{
		.group_tag = SCENARIO_DECALS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_decals_resource)
	},
	{
		.group_tag = SCENARIO_DECORATORS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_decorators_resource)
	},
	{
		.group_tag = SCENARIO_DEVICES_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_devices_resource)
	},
	{
		.group_tag = SCENARIO_EFFECT_SCENERY_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_effect_scenery_resource)
	},
	{
		.group_tag = SCENARIO_EQUIPMENT_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_equipment_resource)
	},
	{
		.group_tag = SCENARIO_FAUX_DATA_TAG,
		.name = STRING_ID(global, scenario_faux_data)
	},
	{
		.group_tag = SCENARIO_HS_SOURCE_FILE_TAG,
		.name = STRING_ID(global, scenario_hs_source_file)
	},
	{
		.group_tag = SCENARIO_LIGHTMAP_BSP_DATA_TAG,
		.name = STRING_ID(global, scenario_lightmap_bsp_data)
	},
	{
		.group_tag = SCENARIO_LIGHTMAP_TAG,
		.name = STRING_ID(global, scenario_lightmap)
	},
	{
		.group_tag = SCENARIO_LIGHTS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_lights_resource)
	},
	{
		.group_tag = SCENARIO_PDA_TAG,
		.name = STRING_ID(global, scenario_pda)
	},
	{
		.group_tag = SCENARIO_SCENERY_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_scenery_resource)
	},
	{
		.group_tag = SCENARIO_SKY_REFERENCES_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_sky_references_resource)
	},
	{
		.group_tag = SCENARIO_SOUND_SCENERY_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_sound_scenery_resource)
	},
	{
		.group_tag = SCENARIO_STRUCTURE_BSP_TAG,
		.name = STRING_ID(global, scenario_structure_bsp)
	},
	{
		.group_tag = SCENARIO_STRUCTURE_LIGHTING_INFO_TAG,
		.name = STRING_ID(global, scenario_structure_lighting_info)
	},
	{
		.group_tag = SCENARIO_STRUCTURE_LIGHTING_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_structure_lighting_resource)
	},
	{
		.group_tag = SCENARIO_TAG,
		.name = STRING_ID(global, scenario)
	},
	{
		.group_tag = SCENARIO_TRIGGER_VOLUMES_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_trigger_volumes_resource)
	},
	{
		.group_tag = SCENARIO_VEHICLES_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_vehicles_resource)
	},
	{
		.group_tag = SCENARIO_WEAPONS_RESOURCE_TAG,
		.name = STRING_ID(global, scenario_weapons_resource)
	},
	{
		.group_tag = SCENERY_TAG,
		.name = STRING_ID(global, scenery)
	},
	{
		.group_tag = SCREEN_EFFECT_TAG,
		.name = STRING_ID(global, screen_effect)
	},
	{
		.group_tag = SHADER_BEAM_TAG,
		.name = STRING_ID(global, shader_beam)
	},
	{
		.group_tag = SHADER_BLACK_TAG,
		.name = STRING_ID(global, shader_black)
	},
	{
		.group_tag = SHADER_CONTRAIL_TAG,
		.name = STRING_ID(global, shader_contrail)
	},
	{
		.group_tag = SHADER_CORTANA_TAG,
		.name = STRING_ID(global, shader_cortana)
	},
	{
		.group_tag = SHADER_CUSTOM_TAG,
		.name = STRING_ID(global, shader_custom)
	},
	{
		.group_tag = SHADER_DECAL_TAG,
		.name = STRING_ID(global, shader_decal)
	},
	{
		.group_tag = SHADER_FOLIAGE_TAG,
		.name = STRING_ID(global, shader_foliage)
	},
	{
		.group_tag = SHADER_HALOGRAM_TAG,
		.name = STRING_ID(global, shader_halogram)
	},
	{
		.group_tag = SHADER_LIGHT_VOLUME_TAG,
		.name = STRING_ID(global, shader_light_volume)
	},
	{
		.group_tag = SHADER_PARTICLE_TAG,
		.name = STRING_ID(global, shader_particle)
	},
	{
		.group_tag = SHADER_SCREEN_TAG,
		.name = STRING_ID(global, shader_screen)
	},
	{
		.group_tag = SHADER_SKIN_TAG,
		.name = STRING_ID(global, shader_skin)
	},
	{
		.group_tag = SHADER_TAG,
		.name = STRING_ID(global, shader)
	},
	{
		.group_tag = SHADER_TERRAIN_TAG,
		.name = STRING_ID(global, shader_terrain)
	},
	{
		.group_tag = SHADER_WATER_TAG,
		.name = STRING_ID(global, shader_water)
	},
	{
		.group_tag = SHADER_ZONLY_TAG,
		.name = STRING_ID(global, shader_zonly)
	},
	{
		.group_tag = SHIELD_IMPACT_TAG,
		.name = STRING_ID(global, shield_impact)
	},
	{
		.group_tag = SKY_ATM_PARAMETERS_TAG,
		.name = STRING_ID(global, sky_atm_parameters)
	},
	{
		.group_tag = SOUND_CACHE_FILE_GESTALT_TAG,
		.name = STRING_ID(global, sound_cache_file_gestalt)
	},
	{
		.group_tag = SOUND_CLASSES_TAG,
		.name = STRING_ID(global, sound_classes)
	},
	{
		.group_tag = SOUND_DIALOGUE_CONSTANTS_TAG,
		.name = STRING_ID(global, sound_dialogue_constants)
	},
	{
		.group_tag = SOUND_EFFECT_COLLECTION_TAG,
		.name = STRING_ID(global, sound_effect_collection)
	},
	{
		.group_tag = SOUND_EFFECT_TEMPLATE_TAG,
		.name = STRING_ID(global, sound_effect_template)
	},
	{
		.group_tag = SOUND_ENVIRONMENT_TAG,
		.name = STRING_ID(global, sound_environment)
	},
	{
		.group_tag = SOUND_GLOBAL_PROPAGATION_TAG,
		.name = STRING_ID(global, sound_global_propagation)
	},
	{
		.group_tag = SOUND_LOOPING_TAG,
		.name = STRING_ID(global, sound_looping)
	},
	{
		.group_tag = SOUND_MIX_TAG,
		.name = STRING_ID(global, sound_mix)
	},
	{
		.group_tag = SOUND_SCENERY_TAG,
		.name = STRING_ID(global, sound_scenery)
	},
	{
		.group_tag = SOUND_TAG,
		.name = STRING_ID(global, sound)
	},
	{
		.group_tag = SOUND_UI_SOUNDS_TAG,
		.name = STRING_ID(global, sound_ui_sounds)
	},
	{
		.group_tag = SQUAD_TEMPLATE_TAG,
		.name = STRING_ID(global, squad_template)
	},
	{
		.group_tag = STEREO_SYSTEM_TAG,
		.name = STRING_ID(global, stereo_system)
	},
	{
		.group_tag = STRUCTURE_DESIGN_TAG,
		.name = STRING_ID(global, structure_design)
	},
	{
		.group_tag = STRUCTURE_SEAMS_TAG,
		.name = STRING_ID(global, structure_seams)
	},
	{
		.group_tag = STYLE_TAG,
		.name = STRING_ID(global, style)
	},
	{
		.group_tag = SURVIVAL_MODE_GLOBALS_TAG,
		.name = STRING_ID(global, survival_mode_globals)
	},
	{
		.group_tag = TAG_TEMPLATE_UNIT_TEST_TAG,
		.name = STRING_ID(global, tag_template_unit_test)
	},
	{
		.group_tag = TEST_TAG_TAG,
		.name = STRING_ID(global, test_tag)
	},
	{
		.group_tag = TEXTURE_RENDER_LIST_TAG,
		.name = STRING_ID(global, texture_render_list)
	},
	{
		.group_tag = TEXT_VALUE_PAIR_DEFINITION_TAG,
		.name = STRING_ID(global, text_value_pair_definition)
	},
	{
		.group_tag = UNIT_TAG,
		.parent_group_tags = { OBJECT_TAG },
		.name = STRING_ID(global, unit)
	},
	{
		.group_tag = USER_INTERFACE_FOURTH_WALL_TIMING_DEFINITION_TAG,
		.name = STRING_ID(global, user_interface_fourth_wall_timing_definition)
	},
	{
		.group_tag = USER_INTERFACE_GLOBALS_DEFINITION_TAG,
		.name = STRING_ID(global, user_interface_globals_definition)
	},
	{
		.group_tag = USER_INTERFACE_SHARED_GLOBALS_DEFINITION_TAG,
		.name = STRING_ID(global, user_interface_shared_globals_definition)
	},
	{
		.group_tag = USER_INTERFACE_SOUNDS_DEFINITION_TAG,
		.name = STRING_ID(global, user_interface_sounds_definition)
	},
	{
		.group_tag = VEHICLE_COLLECTION_TAG,
		.name = STRING_ID(global, vehicle_collection)
	},
	{
		.group_tag = VEHICLE_TAG,
		.parent_group_tags = { UNIT_TAG, OBJECT_TAG },
		.name = STRING_ID(global, vehicle)
	},
	{
		.group_tag = VERTEX_SHADER_TAG,
		.name = STRING_ID(global, vertex_shader)
	},
	{
		.group_tag = VFILES_LIST_TAG,
		.name = STRING_ID(global, vfiles_list)
	},
	{
		.group_tag = VISION_MODE_TAG,
		.name = STRING_ID(global, vision_mode)
	},
	{
		.group_tag = WEAPON_TAG,
		.parent_group_tags = { ITEM_TAG, OBJECT_TAG },
		.name = STRING_ID(global, weapon)
	},
	{
		.group_tag = WIND_TAG,
		.name = STRING_ID(global, wind)
	},
};
static int32 const global_tag_group_count = NUMBEROF(global_tag_groups);

