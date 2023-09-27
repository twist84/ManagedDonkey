#include "hs/hs_function.hpp"

#define MAKE_HS_FUNCTION_TABLE_ENTRY(_return_type, _name, _flags, _parser, _evaluate, _description, _usage, _parameter_count, ...) new hs_function_definition_debug \
{ \
	.return_type = (_return_type), \
	.name = #_name, \
	.flags = (_flags), \
	.parser = (_parser), \
	.evaluate = (_evaluate), \
	.description = (_description), \
	.usage = (_usage), \
	.parameter_count = (_parameter_count), \
	.parameters = { __VA_ARGS__ } \
}
#define MAKE_HS_FUNCTION_TABLE_ENTRY2(_return_type, _name, _flags, _description, _usage, _parameter_count, ...) MAKE_HS_FUNCTION_TABLE_ENTRY(_return_type, _name, _flags, hs_##_name##_parser, hs_##_name##_evaluate, _description, _usage, _parameter_count, __VA_ARGS__)

REFERENCE_DECLARE_ARRAY(0x018ED378, hs_function_definition const*, hs_function_table, hs_function_table_count);

// index (comment)
// name (string)
// description (comment)
// usage (comment)

char const* const hs_function_table_names[hs_function_table_count]
{
	// 0
	"begin",
	// returns the last expression in a sequence after evaluating the sequence in order.\r\nNETWORK SAFE: Yes
	// <expression(s)>

	// 1
	"begin_random",
	// evaluates the sequence of expressions in random order and returns the last value evaluated.\r\nNETWORK SAFE: Yes
	// <expression(s)>

	// 2
	"if",
	// returns one of two values based on the value of a condition.\r\nNETWORK SAFE: Yes
	// <boolean> <then> [<else>]

	// 3
	"cond",
	// returns the value associated with the first true condition.\r\nNETWORK SAFE: Yes
	// (<boolean1> <result1>) [(<boolean2> <result2>) [...]]

	// 4
	"set",
	// set the value of a global variable.\r\nNETWORK SAFE: Yes (depending on result)
	// <variable name> <expression>

	// 5
	"and",
	// returns true if all specified expressions are true.\r\nNETWORK SAFE: Yes
	// <boolean(s)>

	// 6
	"or",
	// returns true if any specified expressions are true.\r\nNETWORK SAFE: Yes
	// <boolean(s)>

	// 7
	"+",
	// returns the sum of all specified expressions.\r\nNETWORK SAFE: Yes
	// <number(s)>

	// 8
	"-",
	// returns the difference of two expressions.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 9
	"*",
	// returns the product of all specified expressions.\r\nNETWORK SAFE: Yes
	// <number(s)>

	// 10
	"/",
	// returns the quotient of two expressions.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 11
	"unknown00B",
	// <number> <number>

	// 12
	"min",
	// returns the minimum of all specified expressions.\r\nNETWORK SAFE: Yes
	// <number(s)>

	// 13
	"max",
	// returns the maximum of all specified expressions.\r\nNETWORK SAFE: Yes
	// <number(s)>

	// 14
	"=",
	// returns true if two expressions are equal\r\nNETWORK SAFE: Yes
	// <expression> <expression>

	// 15
	"!=",
	// returns true if two expressions are not equal\r\nNETWORK SAFE: Yes
	// <expression> <expression>

	// 16
	">",
	// returns true if the first number is larger than the second.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 17
	"<",
	// returns true if the first number is smaller than the second.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 18
	">=",
	// returns true if the first number is larger than or equal to the second.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 19
	"<=",
	// returns true if the first number is smaller than or equal to the second.\r\nNETWORK SAFE: Yes
	// <number> <number>

	// 20
	"sleep",
	// pauses execution of this script (or, optionally, another script) for the specified number of ticks.\r\nNETWORK SAFE: Yes
	// <short> [<script>]

	// 21
	"sleep_forever",
	// pauses execution of this script (or, optionally, another script) forever.\r\nNETWORK SAFE: Yes
	// [<script>]

	// 22
	"sleep_until",
	// pauses execution of this script until the specified condition is true, checking once per second unless a different number of ticks is specified.\r\nNETWORK SAFE: Yes
	// <boolean> [<short>]

	// 23
	"wake",
	// wakes a sleeping script in the next update.\r\nNETWORK SAFE: Yes
	// <script name>

	// 24
	"inspect",
	// prints the value of an expression to the screen for debugging purposes.\r\nNETWORK SAFE: Yes
	// <expression>

	// 25
	"unit",

	// 26
	"evaluate",

	// 27
	"not",

	// 28
	"pin",

	// 29
	"print",

	// 30
	"log_print",

	// 31
	"debug_scripting_show_thread",

	// 32
	"debug_script_thread",

	// 33
	"debug_scripting",

	// 34
	"debug_scripting_globals",

	// 35
	"debug_scripting_variable",

	// 36
	"debug_scripting_variable_all",

	// 37
	"breakpoint",

	// 38
	"kill_active_scripts",

	// 39
	"get_executing_running_thread",

	// 40
	"kill_thread",

	// 41
	"script_started",

	// 42
	"script_finished",

	// 43
	"players",

	// 44
	"player_get",

	// 45
	"kill_volume_enable",

	// 46
	"kill_volume_disable",

	// 47
	"volume_teleport_players_not_inside",

	// 48
	"volume_test_object",

	// 49
	"volume_test_objects",

	// 50
	"volume_test_objects_all",

	// 51
	"volume_test_players",

	// 52
	"volume_test_players_all",

	// 53
	"volume_return_objects",

	// 54
	"volume_return_objects_by_type",

	// 55
	"zone_set_trigger_volume_enable",

	// 56
	"list_get",

	// 57
	"list_count",

	// 58
	"list_count_not_dead",

	// 59
	"effect_new",

	// 60
	"effect_new_random",

	// 61
	"effect_new_at_ai_point",

	// 62
	"effect_new_on_object_marker",

	// 63
	"effect_new_on_object",

	// 64
	"damage_new",

	// 65
	"damage_object_effect",

	// 66
	"damage_objects_effect",

	// 67
	"damage_object",

	// 68
	"damage_objects",

	// 69
	"damage_players",

	// 70
	"soft_ceiling_enable",

	// 71
	"object_create",

	// 72
	"object_create_clone",

	// 73
	"object_create_anew",

	// 74
	"object_create_if_necessary",

	// 75
	"object_create_containing",

	// 76
	"object_create_clone_containing",

	// 77
	"object_create_anew_containing",

	// 78
	"object_create_folder",

	// 79
	"object_create_folder_anew",

	// 80
	"object_destroy",

	// 81
	"object_destroy_containing",

	// 82
	"object_destroy_all",

	// 83
	"object_destroy_type_mask",

	// 84
	"objects_delete_by_definition",

	// 85
	"object_destroy_folder",

	// 86
	"object_hide",

	// 87
	"object_set_shadowless",

	// 88
	"object_buckling_magnitude_get",

	// 89
	"object_function_set",

	// 90
	"object_set_function_variable",

	// 91
	"object_clear_function_variable",

	// 92
	"object_clear_all_function_variables",

	// 93
	"object_dynamic_simulation_disable",

	// 94
	"object_set_phantom_power",

	// 95
	"object_wake_physics",

	// 96
	"object_set_ranged_attack_inhibited",

	// 97
	"object_set_melee_attack_inhibited",

	// 98
	"objects_dump_memory",

	// 99
	"object_get_health",

	// 100
	"object_get_shield",

	// 101
	"object_set_shield_effect",

	// 102
	"object_set_physics",

	// 103
	"object_get_parent",

	// 104
	"objects_attach",

	// 105
	"object_at_marker",

	// 106
	"objects_detach",

	// 107
	"object_set_scale",

	// 108
	"object_set_velocity",
	// Sets the (object-relative) forward velocity of the given object\r\nNETWORK SAFE: Yes

	// 109
	"unknown06D", // object_set_inertia_tensor_scale?

	// 110
	"object_set_collision_damage_armor_scale",
	// ==default of 1.0, otherwise it is scale on how this objects takes collisoin damage, >1 == more protection, <1 == less\r\nNETWORK SAFE: Yes

	// 111
	"object_set_velocity",
	// Sets the (object-relative) velocity of the given object\r\nNETWORK SAFE: Yes

	// 112
	"object_set_deleted_when_deactivated",
	// when this object deactivates it will be deleted\r\nNETWORK SAFE: Yes

	// 113
	"object_model_target_destroyed",

	// 114
	"object_model_targets_destroyed",

	// 115
	"object_damage_damage_section",

	// 116
	"object_cannot_die",

	// 117
	"object_vitality_pinned",

	// 118
	"garbage_collect_now",

	// 119
	"garbage_collect_unsafe",

	// 120
	"garbage_collect_multiplayer",

	// 121
	"object_cannot_take_damage",

	// 122
	"object_can_take_damage",

	// 123
	"object_cinematic_lod",

	// 124
	"object_cinematic_collision",

	// 125
	"object_cinematic_visibility",

	// 126
	"objects_predict",

	// 127
	"objects_predict_high",

	// 128
	"objects_predict_low",

	// 129
	"object_type_predict_high",

	// 130
	"object_type_predict_low",

	// 131
	"object_type_predict",

	// 132
	"object_teleport",

	// 133
	"object_teleport_to_ai_point",

	// 134
	"object_set_facing",

	// 135
	"object_set_shield",

	// 136
	"object_set_shield_normalized",

	// 137
	"object_set_shield_stun",

	// 138
	"object_set_shield_stun_infinite",

	// 139
	"object_set_permutation",

	// 140
	"object_set_variant",

	// 141
	"object_set_region_state",

	// 142
	"objects_can_see_object",

	// 143
	"objects_can_see_flag",

	// 144
	"objects_distance_to_object",

	// 145
	"objects_distance_to_flag",

	// 146
	"map_info",

	// 147
	"position_predict",

	// 148
	"shader_predict",

	// 149
	"bitmap_predict",

	// 150
	"script_recompile",

	// 151
	"script_doc",

	// 152
	"help",

	// 153
	"game_engine_objects",

	// 154
	"random_range",

	// 155
	"real_random_range",

	// 156
	"physics_constants_reset",

	// 157
	"physics_set_gravity",

	// 158
	"physics_set_velocity_frame",

	// 159
	"physics_disable_character_ground_adhesion_forces",

	// 160
	"havok_debug_start",

	// 161
	"havok_dump_world",

	// 162
	"havok_dump_world_close_movie",

	// 163
	"havok_profile_start",

	// 164
	"havok_profile_end",

	// 165
	"havok_profile_range",

	// 166
	"havok_reset_allocated_state",

	// 167
	"breakable_surfaces_enable",

	// 168
	"breakable_surfaces_reset",

	// 169
	"recording_play",

	// 170
	"recording_play_and_delete",

	// 171
	"recording_play_and_hover",

	// 172
	"recording_kill",

	// 173
	"recording_time",

	// 174
	"render_lights",

	// 175
	"print_light_state",

	// 176
	"render_lights_enable_cinematic_shadow",

	// 177
	"texture_camera_set_object_marker",

	// 178
	"texture_camera_set_position",

	// 179
	"texture_camera_set_target",

	// 180
	"texture_camera_attach_to_object",

	// 181
	"texture_camera_target_object",

	// 182
	"texture_camera_position_world_offset",

	// 183
	"texture_camera_on",

	// 184
	"texture_camera_off",

	// 185
	"texture_camera_set_aspect_ratio",

	// 186
	"texture_camera_set_resolution",

	// 187
	"texture_camera_render_mode",

	// 188
	"texture_camera_set_fov",

	// 189
	"texture_camera_set_fov_frame_target",

	// 190
	"unknown0BE",

	// 191
	"texture_camera_enable_dynamic_lights",

	// 192
	"hud_camera_on",

	// 193
	"hud_camera_off",

	// 194
	"hud_camera_set_position",

	// 195
	"hud_camera_set_target",

	// 196
	"hud_camera_attach_to_object",

	// 197
	"hud_camera_target_object",

	// 198
	"hud_camera_structure",

	// 199
	"hud_camera_highlight_object",

	// 200
	"hud_camera_clear_objects",

	// 201
	"hud_camera_spin_around",

	// 202
	"hud_camera_from_player_view",

	// 203
	"hud_camera_window",

	// 204
	"render_debug_texture_camera",

	// 205
	"debug_structure_cluster",

	// 206
	"render_debug_structure_cluster_fog",

	// 207
	"render_debug_structure_fog_plane",

	// 208
	"render_debug_structure_fog_plane_infinite_extent",

	// 209
	"render_debug_structure_fog_zone",

	// 210
	"render_debug_structure_fog_zone_floodfill",

	// 211
	"render_debug_structure_all_fog_planes",

	// 212
	"render_debug_structure_all_cluster_errors",

	// 213
	"render_debug_structure_line_opacity",

	// 214
	"render_debug_structure_text_opacity",

	// 215
	"render_debug_structure_opacity",

	// 216
	"render_debug_structure_non_occluded_fog_planes",

	// 217
	"render_debug_structure_lightmaps_use_pervertex",

	// 218
	"render_debug_structure_lightmaps_use_reset",

	// 219
	"render_debug_structure_lightmaps_sample_enable",

	// 220
	"render_debug_structure_lightmaps_sample_disable",

	// 221
	"render_debug_query_object_bitmaps",

	// 222
	"render_debug_query_bsp_resources",

	// 223
	"render_debug_query_all_object_resources",

	// 224
	"render_debug_query_d3d_resources",

	// 225
	"render_debug_text_using_simple_font",

	// 226
	"render_postprocess_color_tweaking_reset",

	// 227
	"scenery_animation_start",

	// 228
	"scenery_animation_start_loop",

	// 229
	"scenery_animation_start_relative",

	// 230
	"scenery_animation_start_relative_loop",

	// 231
	"scenery_animation_start_at_frame",

	// 232
	"scenery_animation_start_relative_at_frame",

	// 233
	"scenery_animation_idle",

	// 234
	"scenery_get_animation_time",

	// 235
	"unit_can_blink",

	// 236
	"unit_set_active_camo",

	// 237
	"unit_open",

	// 238
	"unit_close",

	// 239
	"unit_kill",

	// 240
	"unit_kill_silent",

	// 241
	"unit_is_emitting",

	// 242
	"unit_get_custom_animation_time",

	// 243
	"unit_stop_custom_animation",

	// 244
	"custom_animation",

	// 245
	"custom_animation_loop",

	// 246
	"custom_animation_relative",

	// 247
	"custom_animation_relative_loop",

	// 248
	"custom_animation_list",

	// 249
	"unit_custom_animation_at_frame",

	// 250
	"unit_custom_animation_relative_at_frame",

	// 251
	"unit_is_playing_custom_animation",

	// 252
	"object_set_custom_animations_hold_on_last_frame",

	// 253
	"object_set_custom_animations_prevent_lipsync_head_movement",

	// 254
	"preferred_animation_list_add",

	// 255
	"preferred_animation_list_clear",

	// 256
	"unit_set_actively_controlled",

	// 257
	"unit_get_team_index",

	// 258
	"unit_aim_without_turning",

	// 259
	"unit_set_enterable_by_player",

	// 260
	"unit_set_seat_enterable_by_player",

	// 261
	"unit_get_enterable_by_player",

	// 262
	"unit_only_takes_damage_from_players_team",

	// 263
	"unit_enter_vehicle",

	// 264
	"unit_falling_damage_disable",

	// 265
	"unit_in_vehicle_type",

	// 266
	"object_get_turret_count",

	// 267
	"object_get_turret",

	// 268
	"unit_board_vehicle",

	// 269
	"unit_set_emotion_by_?",

	// 270
	"unit_set_emotion_by_name",

	// 271
	"unit_enable_eye_tracking",

	// 272
	"unit_set_integrated_flashlight",

	// 273
	"unit_set_voice",

	// 274
	"unit_enable_vision_mode",

	// 275
	"unit_in_vehicle",

	// 276
	"vehicle_test_seat_list",

	// 277
	"vehicle_test_seat_unit",

	// 278
	"vehicle_test_seat",

	// 279
	"unit_set_prefer_tight_camera_track",

	// 280
	"unit_exit_vehicle",

	// 281
	"unit_exit_vehicle",

	// 282
	"unit_set_maximum_vitality",

	// 283
	"units_set_maximum_vitality",

	// 284
	"unit_set_current_vitality",

	// 285
	"units_set_current_vitality",

	// 286
	"vehicle_load_magic",

	// 287
	"vehicle_unload",

	// 288
	"unit_set_animation_mode",

	// 289
	"magic_melee_attack",

	// 290
	"vehicle_riders",

	// 291
	"vehicle_driver",

	// 292
	"vehicle_gunner",

	// 293
	"unit_get_health",

	// 294
	"unit_get_shield",

	// 295
	"unit_get_total_grenade_count",

	// 296
	"unit_has_weapon",

	// 297
	"unit_has_weapon_readied",

	// 298
	"unit_has_any_equipment",

	// 299
	"unit_has_equipment",

	// 300
	"unit_lower_weapon",

	// 301
	"unit_raise_weapon",

	// 302
	"unit_drop_support_weapon",

	// 303
	"unit_spew_action",

	// 304
	"unit_force_reload",

	// 305
	"animation_stats_dump",

	// 306
	"unit_animation_forced_seat",

	// 307
	"unit_doesnt_drop_items",

	// 308
	"unit_impervious",

	// 309
	"unit_suspended",

	// 310
	"unit_add_equipment",

	// 311
	"weapon_hold_trigger",

	// 312
	"weapon_enable_warthog_chaingun_light",

	// 313
	"device_set_never_appears_locked",

	// 314
	"device_set_power",

	// 315
	"device_get_power",

	// 316
	"device_set_position",

	// 317
	"device_get_position",

	// 318
	"device_set_position_immediate",

	// 319
	"device_group_get",

	// 320
	"device_group_set",

	// 321
	"device_group_set_immediate",

	// 322
	"device_one_sided_set",

	// 323
	"device_ignore_player_set",

	// 324
	"device_operates_automatically_set",

	// 325
	"device_closes_automatically_set",

	// 326
	"device_group_change_only_once_more_set",

	// 327
	"device_set_position_track",

	// 328
	"device_set_overlay_track",

	// 329
	"device_animate_position",

	// 330
	"device_animate_overlay",

	// 331
	"cheat_all_powerups",

	// 332
	"cheat_all_weapons",

	// 333
	"cheat_all_vehicles",

	// 334
	"cheat_teleport_to_camera",

	// 335
	"cheat_active_camouflage",

	// 336
	"cheat_active_camouflage_by_player",

	// 337
	"cheats_load",

	// 338
	"drop_safe",

	// 339
	"drop",

	// 340
	"drop_variant",

	// 341
	"ai_enable",

	// 342
	"ai_enabled",

	// 343
	"ai_grenades",

	// 344
	"ai_dialogue_enable",

	// 345
	"ai_player_dialogue_enable",

	// 346
	"ai_infection_suppress",

	// 347
	"ai_fast_and_dumb",

	// 348
	"ai_dialogue_log_reset",

	// 349
	"ai_dialogue_log_dump",

	// 350
	"ai_get_object",

	// 351
	"ai_get_unit",

	// 352
	"ai_get_squad",

	// 353
	"ai_get_turret_ai",

	// 354
	"ai_random_smart_point",

	// 355
	"ai_nearest_point",

	// 356
	"ai_get_point_count",

	// 357
	"ai_point_set_get_point",

	// 358
	"ai_place",

	// 359
	"ai_place",

	// 360
	"ai_place_in_vehicle",

	// 361
	"ai_cannot_die",

	// 362
	"ai_vitality_pinned",

	// 363
	"ai_index_from_spawn_formation",

	// 364
	"ai_resurrect",

	// 365
	"ai_kill",

	// 366
	"ai_kill_silent",

	// 367
	"ai_erase",

	// 368
	"ai_erase_all",

	// 369
	"ai_disposable",

	// 370
	"ai_select",

	// 371
	"ai_deselect",

	// 372
	"ai_set_deaf",

	// 373
	"ai_set_blind",

	// 374
	"ai_set_weapon_up",

	// 375
	"ai_flood_disperse",

	// 376
	"ai_magically_see",

	// 377
	"ai_magically_see_object",

	// 378
	"ai_set_active_camo",

	// 379
	"ai_suppress_combat",

	// 380
	"ai_engineer_explode",

	// 381
	"ai_grunt_kamikaze",

	// 382
	"ai_migrate",

	// 383
	"ai_allegiance",

	// 384
	"ai_allegiance_remove",

	// 385
	"ai_allegiance_break",

	// 386
	"ai_braindead",

	// 387
	"ai_braindead_by_unit",

	// 388
	"ai_disregard",

	// 389
	"ai_prefer_target",

	// 390
	"ai_prefer_target_team",

	// 391
	"ai_prefer_target_ai",

	// 392
	"ai_set_targeting_group",

	// 393
	"ai_set_targeting_group",

	// 394
	"ai_teleport_to_starting_location_if_outside_bsp",

	// 395
	"ai_teleport_to_starting_location_if_outside_bsp?",

	// 396
	"ai_teleport",

	// 397
	"ai_bring_forward",

	// 398
	"ai_migrate_form",

	// 399
	"ai_morph",

	// 400
	"biped_morph",

	// 401
	"ai_renew",

	// 402
	"ai_force_active",

	// 403
	"ai_force_active_by_unit",

	// 404
	"ai_playfight",

	// 405
	"ai_reconnect",

	// 406
	"ai_berserk",

	// 407
	"ai_set_team",

	// 408
	"ai_allow_dormant",

	// 409
	"ai_is_attacking",

	// 410
	"ai_fighting_count",

	// 411
	"ai_living_count",

	// 412
	"ai_living_fraction",

	// 413
	"ai_in_vehicle_count",

	// 414
	"ai_body_count",

	// 415
	"ai_strength",

	// 416
	"ai_swarm_count",

	// 417
	"ai_nonswarm_count",

	// 418
	"ai_actors",

	// 419
	"ai_allegiance_broken",

	// 420
	"ai_spawn_count",

	// 421
	"object_get_ai",

	// 422
	"ai_rotate_scenario",

	// 423
	"ai_translate_scenario",

	// 424
	"scenario_duplicate_bsp_objects",

	// 425
	"scenario_duplicate_bsp_objects",

	// 426
	"scenario_rotate",

	// 427
	"scenario_reflect",

	// 428
	"scenario_translate",

	// 429
	"scenario_rotate",

	// 430
	"scenario_reflect",

	// 431
	"scenario_translate",

	// 432
	"ai_set_task",

	// 433
	"ai_set_objective",

	// 434
	"ai_task_status",

	// 435
	"ai_set_task_condition",

	// 436
	"ai_leadership",

	// 437
	"ai_leadership_all",

	// 438
	"ai_task_count",

	// 439
	"ai_reset_objective",

	// 440
	"ai_squad_patrol_objective_disallow",

	// 441
	"generate_pathfinding",

	// 442
	"ai_render_paths_all",

	// 443
	"ai_activity_set",

	// 444
	"ai_activity_abort",

	// 445
	"ai_vehicle_get",

	// 446
	"ai_vehicle_get_from_starting_location",

	// 447
	"ai_vehicle_get_from_spawn_point",

	// 448
	"ai_vehicle_get_from_starting_location",

	// 449
	"ai_vehicle_get_from_squad",

	// 450
	"ai_vehicle_reserve_seat",

	// 451
	"ai_vehicle_reserve",

	// 452
	"ai_player_get_vehicle_squad",

	// 453
	"ai_vehicle_count",

	// 454
	"ai_carrying_player",

	// 455
	"player_in_vehicle",

	// 456
	"player_in_vehicle?",

	// 457
	"ai_player_any_needs_vehicle",

	// 458
	"ai_vehicle_enter",

	// 459
	"ai_vehicle_enter",

	// 460
	"ai_vehicle_enter_immediate",

	// 461
	"ai_vehicle_enter_immediate",

	// 462
	"ai_enter_squad_vehicles",

	// 463
	"ai_vehicle_exit",

	// 464
	"ai_vehicle_exit",

	// 465
	"vehicle_overturned",

	// 466
	"vehicle_flip",

	// 467
	"ai_combat_status",

	// 468
	"flock_start",

	// 469
	"flock_stop",

	// 470
	"flock_create",

	// 471
	"flock_delete",

	// 472
	"flock_definition_set",

	// 473
	"flock_unperch",

	// 474
	"drop_ai",

	// 475
	"ai_verify_tags",

	// 476
	"ai_wall_lean",

	// 477
	"ai_play_line",

	// 478
	"ai_play_line_at_player",

	// 479
	"ai_play_line_on_object",

	// 480
	"ai_play_line_on_object",

	// 481
	"ai_play_line_on_point_set",

	// 482
	"ai_play_line_on_point_set",

	// 483
	"campaign_metagame_time_pause",

	// 484
	"campaign_metagame_award_points",

	// 485
	"campaign_metagame_award_primary_skull",

	// 486
	"campaign_metagame_award_secondary_skull",

	// 487
	"campaign_metagame_award_event",

	// 488
	"campaign_metagame_enabled",

	// 489
	"campaign_survival_enabled",

	// 490
	"campaign_is_finished_easy",

	// 491
	"campaign_is_finished_normal",

	// 492
	"campaign_is_finished_heroic",

	// 493
	"campaign_is_finished_legendary",

	// 494
	"cs_run_command_script",

	// 495
	"cs_queue_command_script",

	// 496
	"cs_stack_command_script",

	// 497
	"vs_reserve",

	// 498
	"vs_reserve",

	// 499
	"vs_cast",

	// 500
	"vs_cast",

	// 501
	"vs_cast",

	// 502
	"vs_cast",

	// 503
	"vs_cast",

	// 504
	"vs_cast",

	// 505
	"vs_cast",

	// 506
	"vs_role",

	// 507
	"vs_abort_on_alert",

	// 508
	"vs_abort_on_damage",

	// 509
	"vs_abort_on_combat_status",

	// 510
	"vs_abort_on_vehicle_exit",

	// 511
	"cs_abort_on_alert",

	// 512
	"cs_abort_on_damage",

	// 513
	"cs_abort_on_combat_status",

	// 514
	"cs_abort_on_vehicle_exit",

	// 515
	"cs_abort_on_alert",

	// 516
	"vs_abort_on_alert",

	// 517
	"cs_abort_on_damage",

	// 518
	"vs_abort_on_damage",

	// 519
	"cs_abort_on_combat_status",

	// 520
	"vs_abort_on_combat_status",

	// 521
	"cs_abort_on_vehicle_exit",

	// 522
	"vs_abort_on_vehicle_exit",

	// 523
	"vs_set_cleanup_script",

	// 524
	"vs_release",

	// 525
	"vs_release_all",

	// 526
	"cs_command_script_running",

	// 527
	"cs_command_script_queued",

	// 528
	"cs_number_queued",

	// 529
	"unknown211",

	// 530
	"unknown212",

	// 531
	"vs_running_atom",

	// 532
	"vs_running_atom_movement",

	// 533
	"vs_running_atom_?",

	// 534
	"vs_running_atom_dialogue",

	// 535
	"cs_fly_to",

	// 536
	"vs_fly_to",

	// 537
	"cs_fly_to",

	// 538
	"vs_fly_to",

	// 539
	"cs_fly_to_and_face",

	// 540
	"vs_fly_to_and_face",

	// 541
	"cs_fly_to_and_face",

	// 542
	"vs_fly_to_and_face",

	// 543
	"cs_fly_by",

	// 544
	"vs_fly_by",

	// 545
	"cs_fly_by",

	// 546
	"vs_fly_by",

	// 547
	"cs_go_to",

	// 548
	"vs_go_to",

	// 549
	"cs_go_to",

	// 550
	"vs_go_to",

	// 551
	"cs_go_by",

	// 552
	"vs_go_by",

	// 553
	"cs_go_by",

	// 554
	"vs_go_by",

	// 555
	"cs_go_to_and_face",

	// 556
	"vs_go_to_and_face",

	// 557
	"cs_go_to_and_posture",

	// 558
	"vs_go_to_and_posture",

	// 559
	"cs_go_to_nearest",

	// 560
	"vs_go_to_nearest",

	// 561
	"cs_move_in_direction",

	// 562
	"vs_move_in_direction",

	// 563
	"cs_move_towards?",

	// 564
	"vs_move_towards?",

	// 565
	"cs_move_towards",

	// 566
	"vs_move_towards",

	// 567
	"cs_move_towards_point",

	// 568
	"vs_move_towards_point",

	// 569
	"cs_swarm_to",

	// 570
	"vs_swarm_to",

	// 571
	"cs_swarm_from",

	// 572
	"vs_swarm_from",

	// 573
	"cs_pause",

	// 574
	"vs_pause",

	// 575
	"cs_grenade",

	// 576
	"vs_grenade",

	// 577
	"cs_equipment",

	// 578
	"vs_equipment",

	// 579
	"cs_jump",

	// 580
	"vs_jump",

	// 581
	"cs_jump_to_point",

	// 582
	"vs_jump_to_point",

	// 583
	"cs_vocalize",

	// 584
	"vs_vocalize",

	// 585
	"cs_play_sound",

	// 586
	"vs_play_sound",

	// 587
	"cs_play_sound",

	// 588
	"vs_play_sound",

	// 589
	"cs_play_sound",

	// 590
	"vs_play_sound",

	// 591
	"cs_action",

	// 592
	"vs_action",

	// 593
	"cs_action_at_object",

	// 594
	"vs_action_at_object",

	// 595
	"cs_action_at_player",

	// 596
	"vs_action_at_player",

	// 597
	"cs_custom_animation",

	// 598
	"vs_custom_animation",

	// 599
	"cs_custom_animation",

	// 600
	"vs_custom_animation",

	// 601
	"cs_custom_animation_death",

	// 602
	"vs_custom_animation_death",

	// 603
	"cs_custom_animation_death",

	// 604
	"vs_custom_animation_death",

	// 605
	"cs_custom_animation_loop",

	// 606
	"vs_custom_animation_loop",

	// 607
	"cs_custom_animation_loop",

	// 608
	"vs_custom_animation_loop",

	// 609
	"cs_play_line",

	// 610
	"vs_play_line",

	// 611
	"unknown263",

	// 612
	"unknown264",

	// 613
	"cs_deploy_turret",

	// 614
	"vs_deploy_turret",

	// 615
	"cs_approach",

	// 616
	"vs_approach",

	// 617
	"cs_approach_player",

	// 618
	"vs_approach_player",

	// 619
	"cs_go_to_vehicle",

	// 620
	"vs_go_to_vehicle",

	// 621
	"cs_go_to_vehicle",

	// 622
	"vs_go_to_vehicle",

	// 623
	"cs_set_style",

	// 624
	"vs_set_style",

	// 625
	"cs_force_combat_status",

	// 626
	"vs_force_combat_status",

	// 627
	"cs_enable_targeting",

	// 628
	"vs_enable_targeting",

	// 629
	"cs_enable_looking",

	// 630
	"vs_enable_looking",

	// 631
	"cs_enable_moving",

	// 632
	"vs_enable_moving",

	// 633
	"cs_enable_dialogue",

	// 634
	"vs_enable_dialogue",

	// 635
	"cs_suppress_activity_termination",

	// 636
	"vs_suppress_activity_termination",

	// 637
	"cs_suppress_dialogue_global?",

	// 638
	"vs_suppress_dialogue_global?",

	// 639
	"cs_look",

	// 640
	"vs_look",

	// 641
	"cs_look_player",

	// 642
	"vs_look_player",

	// 643
	"cs_look_object",

	// 644
	"vs_look_object",

	// 645
	"cs_aim",

	// 646
	"vs_aim",

	// 647
	"cs_aim_player",

	// 648
	"vs_aim_player",

	// 649
	"cs_aim_object",

	// 650
	"vs_aim_object",

	// 651
	"cs_face",

	// 652
	"vs_face",

	// 653
	"cs_face_player",

	// 654
	"vs_face_player",

	// 655
	"cs_face_object",

	// 656
	"vs_face_object",

	// 657
	"cs_shoot",

	// 658
	"vs_shoot",

	// 659
	"cs_shoot",

	// 660
	"vs_shoot",

	// 661
	"cs_shoot_point",

	// 662
	"vs_shoot_point",

	// 663
	"cs_shoot_secondary_trigger",

	// 664
	"vs_shoot_secondary_trigger",

	// 665
	"cs_lower_weapon",

	// 666
	"vs_lower_weapon",

	// 667
	"cs_vehicle_speed",

	// 668
	"vs_vehicle_speed",

	// 669
	"cs_vehicle_speed_instantaneous",

	// 670
	"vs_vehicle_speed_instantaneous",

	// 671
	"cs_vehicle_boost",

	// 672
	"vs_vehicle_boost",

	// 673
	"cs_turn_sharpness?",

	// 674
	"vs_turn_sharpness?",

	// 675
	"cs_enable_pathfinding_failsafe",

	// 676
	"vs_enable_pathfinding_failsafe",

	// 677
	"unknown2A5",

	// 678
	"unknown2A6",

	// 679
	"cs_ignore_obstacles",

	// 680
	"vs_ignore_obstacles",

	// 681
	"cs_approach_stop",

	// 682
	"vs_approach_stop",

	// 683
	"cs_movement_mode",

	// 684
	"vs_movement_mode",

	// 685
	"cs_crouch",

	// 686
	"vs_crouch",

	// 687
	"cs_crouch",

	// 688
	"vs_crouch",

	// 689
	"cs_walk",

	// 690
	"vs_walk",

	// 691
	"cs_posture_set",

	// 692
	"vs_posture_set",

	// 693
	"cs_posture_exit",

	// 694
	"vs_posture_exit",

	// 695
	"cs_stow",

	// 696
	"vs_stow",

	// 697
	"cs_draw",

	// 698
	"vs_draw",

	// 699
	"cs_teleport",

	// 700
	"vs_teleport",

	// 701
	"cs_stop_custom_animation",

	// 702
	"vs_stop_custom_animation",

	// 703
	"cs_stop_sound",

	// 704
	"vs_stop_sound",

	// 705
	"unknown2C1",

	// 706
	"vs_player_melee",

	// 707
	"unknown2C3",

	// 708
	"vs_melee_direction",

	// 709
	"unknown2C5",

	// 710
	"vs_smash_direction",

	// 711
	"camera_control",

	// 712
	"camera_set",

	// 713
	"camera_set_relative",

	// 714
	"camera_set_animation",

	// 715
	"camera_set_animation_relative",

	// 716
	"camera_set_animation_with_speed",

	// 717
	"camera_set_animation_relative_with_speed",

	// 718
	"camera_set_animation_relative_with_speed_?boolean",

	// 719
	"camera_set_animation_relative_with_speed_?boolean_real",

	// 720
	"camera_predict_resources_at_frame",

	// 721
	"camera_predict_resources_at_point",

	// 722
	"camera_set_first_person",

	// 723
	"camera_set_cinematic",

	// 724
	"camera_set_cinematic_scene",

	// 725
	"camera_place_relative",

	// 726
	"camera_place_worldspace",

	// 727
	"camera_time",

	// 728
	"camera_set_field_of_view",

	// 729
	"cinematic_camera_set_easing_in",

	// 730
	"cinematic_camera_set_easing_out",

	// 731
	"cinematic_print",

	// 732
	"camera_set_pan",

	// 733
	"camera_pan",

	// 734
	"debug_camera_save",

	// 735
	"debug_camera_load",

	// 736
	"debug_camera_save_name",

	// 737
	"debug_camera_load_name",

	// 738
	"director_debug_camera",

	// 739
	"game_difficulty_get",

	// 740
	"game_difficulty_get_real",

	// 741
	"game_insertion_point_get",

	// 742
	"game_insertion_point_set",

	// 743
	"pvs_set_object",

	// 744
	"pvs_set_camera",

	// 745
	"pvs_clear",

	// 746
	"pvs_reset",

	// 747
	"players_unzoom_all",

	// 748
	"player_enable_input",

	// 749
	"player_disable_movement",

	// 750
	"unknown2EE",

	// 751
	"unknown2EF",

	// 752
	"unknown2F0",

	// 753
	"player_flashlight_on",

	// 754
	"player_active_camouflage_on",

	// 755
	"player_camera_control",

	// 756
	"player_action_test_reset",

	// 757
	"player_action_test_jump",

	// 758
	"player_action_test_primary_trigger",

	// 759
	"player_action_test_grenade_trigger",

	// 760
	"player_action_test_vision_trigger",

	// 761
	"player_action_test_rotate_weapons",

	// 762
	"player_action_test_rotate_grenades",

	// 763
	"player_action_test_melee",

	// 764
	"player_action_test_action*",

	// 765
	"player_action_test_accept",

	// 766
	"player_action_test_cancel",

	// 767
	"player_action_test_look_relative_up",

	// 768
	"player_action_test_look_relative_down",

	// 769
	"player_action_test_look_relative_left",

	// 770
	"player_action_test_look_relative_right",

	// 771
	"player_action_test_look_relative_all_directions",

	// 772
	"player_action_test_move_relative_all_directions",

	// 773
	"player_action_test_back",

	// 774
	"player_action_test_dpad_left",

	// 775
	"player_action_test_dpad_right",

	// 776
	"player_action_test_dpad_up",

	// 777
	"player_action_test_dpad_down",

	// 778
	"player_action_test_x",

	// 779
	"player_action_test_y",

	// 780
	"player_action_test_left_shoulder",

	// 781
	"player_action_test_right_shoulder",

	// 782
	"player_action_test_waypoint_activate",

	// 783
	"unit_action_test_reset",

	// 784
	"unit_action_test_primary_trigger",

	// 785
	"unit_action_test_vision_trigger",

	// 786
	"unit_action_test_rotate_weapons",

	// 787
	"unit_action_test_rotate_grenades",

	// 788
	"unit_action_test_melee",

	// 789
	"unit_action_test_action",

	// 790
	"unit_action_test_accept",

	// 791
	"unit_action_test_cancel",

	// 792
	"unit_action_test_look_relative_up",

	// 793
	"unit_action_test_look_relative_down",

	// 794
	"unit_action_test_look_relative_left",

	// 795
	"unit_action_test_look_relative_right",

	// 796
	"unit_action_test_look_relative_all_directions",

	// 797
	"unit_action_test_move_relative_all_directions",

	// 798
	"unit_action_test_back",

	// 799
	"unit_action_test_dpad_left",

	// 800
	"unit_action_test_dpad_right",

	// 801
	"unit_action_test_dpad_up",

	// 802
	"unit_action_test_dpad_down",

	// 803
	"unit_action_test_x",

	// 804
	"unit_action_test_y",

	// 805
	"unit_action_test_left_shoulder",

	// 806
	"unit_action_test_right_shoulder",

	// 807
	"player0_looking_up",

	// 808
	"player0_looking_down",

	// 809
	"player0_set_pitch",

	// 810
	"player1_set_pitch",

	// 811
	"player2_set_pitch",

	// 812
	"player3_set_pitch",

	// 813
	"player_action_test_lookstick_forward",

	// 814
	"player_action_test_lookstick_backward",

	// 815
	"debug_teleport_player",

	// 816
	"map_reset",

	// 817
	"map_reset_random",

	// 818
	"switch_bsp",

	// 819
	"switch_zone_set",

	// 820
	"current_zone_set",

	// 821
	"current_zone_set_fully_active",

	// 822
	"switch_map_and_zone_set",

	// 823
	"crash",

	// 824
	"version",

	// 825
	"status",

	// 826
	"record_movie",

	// 827
	"record_movie_distributed",

	// 828
	"screenshot",

	// 829
	"screenshot_debug",

	// 830
	"screenshot_big",

	// 831
	"screenshot_big_jittered",

	// 832
	"screenshot_size",

	// 833
	"screenshot_simple",

	// 834
	"screenshot_cubemap",

	// 835
	"screenshot_webmap",

	// 836
	"unknown344",

	// 837
	"unknown345",

	// 838
	"screenshot_paths_set",

	// 839
	"unknown347",

	// 840
	"main_menu",

	// 841
	"main_halt",

	// 842
	"map_name",

	// 843
	"game_multiplayer",

	// 844
	"game_set_variant",

	// 845
	"game_splitscreen",

	// 846
	"game_difficulty",

	// 847
	"game_active_primary_skulls",

	// 848
	"game_active_secondary_skulls",

	// 849
	"game_coop_players",

	// 850
	"game_initial_zone_set",

	// 851
	"game_tick_rate",

	// 852
	"game_start",

	// 853
	"game_start_when_ready",

	// 854
	"game_start_when_joined",

	// 855
	"game_rate",

	// 856
	"texture_cache_flush",

	// 857
	"geometry_cache_flush",

	// 858
	"sound_cache_flush",

	// 859
	"debug_memory",

	// 860
	"debug_memory_by_file",

	// 861
	"debug_memory_for_file",

	// 862
	"debug_tags",

	// 863
	"tags_verify_all",

	// 864
	"debug_memory_by_file",

	// 865
	"debug_memory_for_file",

	// 866
	"debug_tags",

	// 867
	"tags_verify_all",

	// 868
	"profiler_enable",

	// 869
	"profiler_set_thread",

	// 870
	"profiler_set_sort_method",

	// 871
	"profiler_set_range",

	// 872
	"profiler_set_attribute",

	// 873
	"trace_next_frame",

	// 874
	"trace_next_frame_to_file",

	// 875
	"trace_tick",

	// 876
	"collision_log_enable",

	// 877
	"damage_control_get",

	// 878
	"damage_control_set",

	// 879
	"ai_lines",

	// 880
	"ai_dialogue_break_on_vocalization",

	// 881
	"fade_in",

	// 882
	"fade_out",

	// 883
	"cinematic_start",

	// 884
	"cinematic_stop",

	// 885
	"cinematic_skip_start_internal",

	// 886
	"cinematic_skip_stop_internal",

	// 887
	"cinematic_show_letterbox",

	// 888
	"cinematic_show_letterbox_immediate",

	// 889
	"cinematic_set_title",

	// 890
	"cinematic_set_title_delayed",

	// 891
	"cinematic_suppress_bsp_object_creation",

	// 892
	"cinematic_subtitle",

	// 893
	"cinematic_set",

	// 894
	"cinematic_set_shot",

	// 895
	"cinematic_get_shot",

	// 896
	"cinematic_set_early_exit",

	// 897
	"cinematic_get_early_exit",

	// 898
	"cinematic_set_active_camera",

	// 899
	"cinematic_object_create",

	// 900
	"cinematic_object_create_cinematic_anchor",

	// 901
	"cinematic_object_destroy",

	// 902
	"cinematic_destroy",

	// 903
	"cinematic_clips_initialize_for_shot",

	// 904
	"cinematic_clips_destroy",

	// 905
	"cinematic_lights_initialize_for_shot",

	// 906
	"cinematic_lights_destroy",

	// 907
	"cinematic_lights_destroy_shot",

	// 908
	"cinematic_light_object",

	// 909
	"cinematic_light_object_off",

	// 910
	"cinematic_lighting_rebuild_all",

	// 911
	"cinematic_lighting_update_dynamic_light_direction",

	// 912
	"cinematic_lighting_update_sh_light",

	// 913
	"cinematic_object_get",

	// 914
	"cinematic_reset",

	// 915
	"camera_set_briefing",

	// 916
	"cinematic_tag_reference_get_cinematic",

	// 917
	"cinematic_tag_reference_get_scene",

	// 918
	"cinematic_tag_reference_get_effect",

	// 919
	"cinematic_tag_reference_get_sound",

	// 920
	"cinematic_tag_reference_get_sound2",

	// 921
	"cinematic_tag_reference_get_looping_sound",

	// 922
	"cinematic_tag_reference_get_animation",

	// 923
	"cinematic_tag_reference_get_?",

	// 924
	"cinematic_scripting_fade_out",

	// 925
	"cinematic_scripting_create_object",

	// 926
	"cinematic_scripting_create_cinematic_object",

	// 927
	"cinematic_scripting_create_object?",

	// 928
	"cinematic_scripting_destroy_object",

	// 929
	"cinematic_scripting_start_effect",

	// 930
	"cinematic_scripting_start_music",

	// 931
	"cinematic_scripting_start_dialogue",

	// 932
	"cinematic_scripting_stop_music",

	// 933
	"cinematic_scripting_create_and_animate_object",

	// 934
	"cinematic_scripting_create_and_animate_cinematic_object",

	// 935
	"cinematic_scripting_create_and_animate_object_no_animation",

	// 936
	"cinematic_scripting_create_and_animate_cinematic_object_no_animation",

	// 937
	"cinematic_scripting_play_cortana_effect",

	// 938
	"attract_mode_start",

	// 939
	"attract_mode_set_seconds",

	// 940
	"game_level_advance",

	// 941
	"game_won",

	// 942
	"game_lost",

	// 943
	"game_revert",

	// 944
	"game_is_cooperative",

	// 945
	"game_is_playtest",

	// 946
	"game_can_use_flashlights",

	// 947
	"game_save_and_quit",

	// 948
	"game_save_unsafe",

	// 949
	"game_insertion_point_unlock",

	// 950
	"game_insertion_point_lock?",

	// 951
	"saved_games_enumerate",

	// 952
	"saved_games_delete_campaign_save",

	// 953
	"saved_games_save_last_film",

	// 954
	"saved_games_autosave_free_up_space",

	// 955
	"content_catalogue_display_status",

	// 956
	"achievement_grant_to_player",

	// 957
	"achievement_grant_to_all_players",

	// 958
	"achievements_enable",

	// 959
	"achievements_skip_validation_checks",

	// 960
	"debug_spawning_influencers",

	// 961
	"debug_spawning_respawn_zones",

	// 962
	"debug_spawning_proximity_forbid",

	// 963
	"debug_spawning_moving_vehicle",

	// 964
	"debug_spawning_weapon_influences",

	// 965
	"debug_spawning_dangerous_projectiles",

	// 966
	"debug_spawning_deployed_equipment",

	// 967
	"debug_spawning_proximity_enemy",

	// 968
	"debug_spawning_teammates",

	// 969
	"debug_spawning_random_influence",

	// 970
	"debug_spawning_nominal_weight",

	// 971
	"debug_spawning_natural_weight",

	// 972
	"debug_spawning_target",

	// 973
	"debug_spawning_use_history",

	// 974
	"debug_initial_spawn_point_objects",

	// 975
	"debug_respawn_point_objects",

	// 976
	"game_export_variant_settings",

	// 977
	"game_engine_event_test_general",

	// 978
	"game_engine_event_test_flavor",

	// 979
	"game_engine_event_test_slayer",

	// 980
	"game_engine_event_test_ctf",

	// 981
	"game_engine_event_test_oddball",

	// 982
	"game_engine_event_test_king",

	// 983
	"game_engine_event_test_vip",

	// 984
	"game_engine_event_test_juggernaut",

	// 985
	"game_engine_event_test_territories",

	// 986
	"game_engine_event_test_assault",

	// 987
	"game_engine_event_test_infection",

	// 988
	"core_load",

	// 989
	"core_load_name",

	// 990
	"core_save",

	// 991
	"core_save_name",

	// 992
	"core_load_game",

	// 993
	"core_load_game_name",

	// 994
	"core_regular_upload_to_debug_server",

	// 995
	"core_set_upload_option",

	// 996
	"force_debugger_not_present",

	// 997
	"force_debugger_always_present",

	// 998
	"game_safe_to_save",

	// 999
	"game_safe_to_speak",

	// 1000
	"game_all_quiet",

	// 1001
	"game_save",

	// 1002
	"game_save_cancel",

	// 1003
	"game_save_no_timeout",

	// 1004
	"game_save_immediate",

	// 1005
	"game_saving",

	// 1006
	"game_reverted",

	// 1007
	"sound_ui_screen_init",

	// 1008
	"sound_ui_screen_term",

	// 1009
	"sound_set_tag_parameter_unsafe",

	// 1010
	"sound_impulse_predict",

	// 1011
	"sound_impulse_trigger",

	// 1012
	"sound_impulse_start",

	// 1013
	"sound_impulse_start_cinematic",

	// 1014
	"sound_impulse_start_effect",

	// 1015
	"sound_impulse_start_with_subtitle",

	// 1016
	"sound_impulse_language_time",

	// 1017
	"sound_impulse_stop",

	// 1018
	"sound_impulse_start_3d",

	// 1019
	"sound_impulse_mark_as_outro",

	// 1020
	"sound_impulse_start_naked",

	// 1021
	"sound_looping_predict",

	// 1022
	"sound_looping_start",

	// 1023
	"sound_looping_stop",

	// 1024
	"sound_looping_stop_immediately",

	// 1025
	"sound_looping_set_scale",

	// 1026
	"sound_looping_set_alternate",

	// 1027
	"sound_loop_spam",

	// 1028
	"sound_class_show_channel",

	// 1029
	"sound_class_debug_sound_start",

	// 1030
	"debug_sounds_enable",

	// 1031
	"sound_class_set_gain",

	// 1032
	"sound_class_set_gain_db",

	// 1033
	"sound_class_enable_ducker",

	// 1034
	"debug_sound_environment_parameter",

	// 1035
	"sound_set_global_effect",

	// 1036
	"sound_set_global_effect_scale",

	// 1037
	"vehicle_auto_turret",

	// 1038
	"vehicle_hover",

	// 1039
	"vehicle_count_bipeds_killed",

	// 1040
	"biped_ragdoll",

	// 1041
	"water_float_reset",

	// 1042
	"hud_show_training_text",

	// 1043
	"hud_set_training_text",

	// 1044
	"hud_enable_training",

	// 1045
	"player_training_activate_flashlight",

	// 1046
	"player_training_activate_crouch",

	// 1047
	"player_training_activate_stealth",

	// 1048
	"player_training_activate_?",

	// 1049
	"player_training_activate_jump",

	// 1050
	"player_training_reset",

	// 1051
	"hud_activate_team_nav_point_flag",

	// 1052
	"hud_deactivate_team_nav_point_flag",

	// 1053
	"chud_cortana_suck",

	// 1054
	"chud_texture_cam",

	// 1055
	"chud_cortana_set_range_multiplier",

	// 1056
	"play_cortana_effect",

	// 1057
	"chud_show",

	// 1058
	"chud_show_weapon_stats",

	// 1059
	"chud_show_crosshair",

	// 1060
	"chud_show_shield",

	// 1061
	"chud_show_grenades",

	// 1062
	"chud_show_messages",

	// 1063
	"chud_show_motion_sensor",

	// 1064
	"chud_show_spike_grenades",

	// 1065
	"chud_show_fire_grenades",

	// 1066
	"chud_show_compass",

	// 1067
	"chud_show_stamina",

	// 1068
	"chud_show_energy_meters",

	// 1069
	"chud_show_consumables",

	// 1070
	"chud_cinematic_fade",

	// 1071
	"chud_bonus_round_show_timer",

	// 1072
	"chud_bonus_round_start_timer",

	// 1073
	"chud_bonus_round_set_timer",

	// 1074
	"cls",

	// 1075
	"error_overflow_suppression",

	// 1076
	"error_geometry_show",

	// 1077
	"error_geometry_hide",

	// 1078
	"error_geometry_show_all",

	// 1079
	"error_geometry_hide_all",

	// 1080
	"error_geometry_list",

	// 1081
	"player_effect_set_max_translation",

	// 1082
	"player_effect_set_max_rotation",

	// 1083
	"player_effect_set_max_rumble",

	// 1084
	"player_effect_start",

	// 1085
	"player_effect_stop",

	// 1086
	"player_effect_set_max_translation_for_player",

	// 1087
	"player_effect_set_max_rotation_for_player",

	// 1088
	"player_effect_set_max_rumble_for_player",

	// 1089
	"player_effect_start_for_player",

	// 1090
	"player_effect_stop_for_player",

	// 1091
	"time_code_show",

	// 1092
	"time_code_start",

	// 1093
	"time_code_reset",

	// 1094
	"script_screen_effect_set_value",

	// 1095
	"cinematic_screen_effect_start",

	// 1096
	"cinematic_screen_effect_set_crossfade",

	// 1097
	"cinematic_screen_effect_set_crossfade",

	// 1098
	"cinematic_screen_effect_stop",

	// 1099
	"cinematic_set_near_clip_distance",

	// 1100
	"cinematic_set_far_clip_distance",

	// 1101
	"render_atmosphere_fog",

	// 1102
	"atmosphere_fog_override_fade",

	// 1103
	"motion_blur",

	// 1104
	"render_weather",

	// 1105
	"render_patchy_fog",

	// 1106
	"cinematic_set_environment_map_attenuation",

	// 1107
	"cinematic_set_environment_map_bitmap",

	// 1108
	"cinematic_reset_environment_map_bitmap",

	// 1109
	"cinematic_set_environment_map_tint",

	// 1110
	"cinematic_reset_environment_map_tint",

	// 1111
	"cinematic_layer",

	// 1112
	"player_has_skills",

	// 1113
	"player_has_mad_secret_skills",

	// 1114
	"controller_invert_look",

	// 1115
	"controller_look_speed",

	// 1116
	"controller_set_look_invert",

	// 1117
	"controller_get_look_invert",

	// 1118
	"controller_unlock_solo_levels",

	// 1119
	"unknown45F",

	// 1120
	"unknown460",

	// 1121
	"unknown461",

	// 1122
	"unknown462",

	// 1123
	"unknown463",

	// 1124
	"unknown464",

	// 1125
	"unknown465",

	// 1126
	"unknown466",

	// 1127
	"unknown467",

	// 1128
	"unknown468",

	// 1129
	"unknown469",

	// 1130
	"unknown46A",

	// 1131
	"unknown46B",

	// 1132
	"unknown46C",

	// 1133
	"unknown46D",

	// 1134
	"unknown46E",

	// 1135
	"unknown46F",

	// 1136
	"unknown470",

	// 1137
	"unknown471",

	// 1138
	"unknown472",

	// 1139
	"unknown473",

	// 1140
	"unknown474",

	// 1141
	"unknown475",

	// 1142
	"unknown476",

	// 1143
	"unknown477",

	// 1144
	"unknown478",

	// 1145
	"unknown479",

	// 1146
	"unknown47A",

	// 1147
	"unknown47B",

	// 1148
	"unknown47C",

	// 1149
	"unknown47D",

	// 1150
	"unknown47E",

	// 1151
	"unknown47F",

	// 1152
	"unknown480",

	// 1153
	"unknown481",

	// 1154
	"unknown482",

	// 1155
	"unknown483",

	// 1156
	"unknown484",

	// 1157
	"unknown485",

	// 1158
	"unknown486",

	// 1159
	"unknown487",

	// 1160
	"unknown488",

	// 1161
	"unknown489",

	// 1162
	"unknown48A",

	// 1163
	"unknown48B",

	// 1164
	"unknown48C",

	// 1165
	"unknown48D",

	// 1166
	"unknown48E",

	// 1167
	"unknown48F",

	// 1168
	"unknown490",

	// 1169
	"unknown491",

	// 1170
	"unknown492",

	// 1171
	"unknown493",

	// 1172
	"unknown494",

	// 1173
	"unknown495",

	// 1174
	"unknown496",

	// 1175
	"unknown497",

	// 1176
	"unknown498",

	// 1177
	"unknown499",

	// 1178
	"unknown49A",

	// 1179
	"unknown49B",

	// 1180
	"unknown49C",

	// 1181
	"unknown49D",

	// 1182
	"unknown49E",

	// 1183
	"unknown49F",

	// 1184
	"unknown4A0",

	// 1185
	"unknown4A1",

	// 1186
	"unknown4A2",

	// 1187
	"unknown4A3",

	// 1188
	"unknown4A4",

	// 1189
	"unknown4A5",

	// 1190
	"unknown4A6",

	// 1191
	"unknown4A7",

	// 1192
	"unknown4A8",

	// 1193
	"cc_enable",

	// 1194
	"cc_test",

	// 1195
	"objectives_clear",

	// 1196
	"objectives_show_up_to",

	// 1197
	"objectives_finish_up_to",

	// 1198
	"objectives_show",

	// 1199
	"objectives_finish",

	// 1200
	"objectives_unavailable",

	// 1201
	"objectives_secondary_show",

	// 1202
	"objectives_secondary_finish",

	// 1203
	"objectives_secondary_unavailable",

	// 1204
	"input_suppress_rumble",

	// 1205
	"unknown4B5",

	// 1206
	"unknown4B6",

	// 1207
	"unknown4B7",

	// 1208
	"unknown4B8",

	// 1209
	"unknown4B9",

	// 1210
	"unknown4BA",

	// 1211
	"unknown4BB",

	// 1212
	"unknown4BC",

	// 1213
	"unknown4BD",

	// 1214
	"unknown4BE",

	// 1215
	"unknown4BF",

	// 1216
	"unknown4C0",

	// 1217
	"unknown4C1",

	// 1218
	"unknown4C2",

	// 1219
	"unknown4C3",

	// 1220
	"unknown4C4",

	// 1221
	"unknown4C5",

	// 1222
	"unknown4C6",

	// 1223
	"unknown4C7",

	// 1224
	"unknown4C8",

	// 1225
	"unknown4C9",

	// 1226
	"unknown4CA",

	// 1227
	"unknown4CB",

	// 1228
	"unknown4CC",

	// 1229
	"unknown4CD",

	// 1230
	"unknown4CE",

	// 1231
	"unknown4CF",

	// 1232
	"unknown4D0",

	// 1233
	"net_delegate_?",

	// 1234
	"net_map_name",

	// 1235
	"unknown4D3",

	// 1236
	"net_campaign_difficulty",

	// 1237
	"unknown4D5",

	// 1238
	"unknown4D6",

	// 1239
	"unknown4D7",

	// 1240
	"unknown4D8",

	// 1241
	"unknown4D9",

	// 1242
	"unknown4DA",

	// 1243
	"unknown4DB",

	// 1244
	"unknown4DC",

	// 1245
	"unknown4DD",

	// 1246
	"unknown4DE",

	// 1247
	"unknown4DF",

	// 1248
	"unknown4E0",

	// 1249
	"unknown4E1",

	// 1250
	"unknown4E2",

	// 1251
	"unknown4E3",

	// 1252
	"unknown4E4",

	// 1253
	"unknown4E5",

	// 1254
	"unknown4E6",

	// 1255
	"unknown4E7",

	// 1256
	"unknown4E8",

	// 1257
	"unknown4E9",

	// 1258
	"unknown4EA",

	// 1259
	"unknown4EB",

	// 1260
	"data_mine_set_mission_segment",

	// 1261
	"data_mine_display_mission_segment",

	// 1262
	"data_mine_insert",

	// 1263
	"unknown4EF",

	// 1264
	"data_mine_upload",

	// 1265
	"data_mine_playback",

	// 1266
	"data_mine_enable",

	// 1267
	"unknown4F3",

	// 1268
	"unknown4F4",

	// 1269
	"unknown4F5",

	// 1270
	"unknown4F6",

	// 1271
	"unknown4F7",

	// 1272
	"unknown4F8",

	// 1273
	"unknown4F9",

	// 1274
	"unknown4FA",

	// 1275
	"unknown4FB",

	// 1276
	"unknown4FC",

	// 1277
	"unknown4FD",

	// 1278
	"unknown4FE",

	// 1279
	"unknown4FF",

	// 1280
	"unknown500",

	// 1281
	"unknown501",

	// 1282
	"unknown502",

	// 1283
	"unknown503",

	// 1284
	"unknown504",

	// 1285
	"unknown505",

	// 1286
	"unknown506",

	// 1287
	"unknown507",

	// 1288
	"unknown508",

	// 1289
	"unknown509",

	// 1290
	"unknown50A",

	// 1291
	"unknown50B",

	// 1292
	"unknown50C",

	// 1293
	"unknown50D",

	// 1294
	"unknown50E",

	// 1295
	"unknown50F",

	// 1296
	"unknown510",

	// 1297
	"unknown511",

	// 1298
	"unknown512",

	// 1299
	"unknown513",

	// 1300
	"unknown514",

	// 1301
	"unknown515",

	// 1302
	"unknown516",

	// 1303
	"unknown517",

	// 1304
	"unknown518",

	// 1305
	"unknown519",

	// 1306
	"unknown51A",

	// 1307
	"unknown51B",

	// 1308
	"unknown51C",

	// 1309
	"unknown51D",

	// 1310
	"unknown51E",

	// 1311
	"unknown51F",

	// 1312
	"unknown520",

	// 1313
	"object_list_children",

	// 1314
	"voice_set_outgoing_channel_count",

	// 1315
	"voice_set_voice_repeater_peer_index",

	// 1316
	"unknown524",

	// 1317
	"unknown525",

	// 1318
	"unknown526",

	// 1319
	"unknown527",

	// 1320
	"unknown528",

	// 1321
	"unknown529",

	// 1322
	"interpolator_start",

	// 1323
	"interpolator_start_smooth",

	// 1324
	"interpolator_stop",

	// 1325
	"interpolator_restart",

	// 1326
	"interpolator_is_active",

	// 1327
	"interpolator_is_finished",

	// 1328
	"interpolator_set_current_value",

	// 1329
	"interpolator_get_current_value",

	// 1330
	"interpolator_get_start_value",

	// 1331
	"interpolator_get_final_value",

	// 1332
	"interpolator_get_current_phase",

	// 1333
	"interpolator_get_current_time_fraction",

	// 1334
	"interpolator_get_start_time",

	// 1335
	"interpolator_get_final_time",

	// 1336
	"interpolator_evaluate_at",

	// 1337
	"interpolator_evaluate_at_time_fraction",

	// 1338
	"interpolator_evaluate_at_time",

	// 1339
	"interpolator_evaluate_at_time_delta",

	// 1340
	"interpolator_stop_all",

	// 1341
	"interpolator_restart_all",

	// 1342
	"interpolator_flip",

	// 1343
	"unknown53F",

	// 1344
	"animation_cache_stats_reset",

	// 1345
	"cinematic_clone_players_weapon",

	// 1346
	"cinematic_move_attached_objects",

	// 1347
	"vehicle_enable_ghost_effects",

	// 1348
	"set_global_sound_environment",

	// 1349
	"reset_dsp_image",

	// 1350
	"game_save_cinematic_skip",

	// 1351
	"cinematic_outro_start",

	// 1352
	"cinematic_enable_ambience_details",

	// 1353
	"unknown549",

	// 1354
	"unknown54A",

	// 1355
	"unknown54B",

	// 1356
	"unknown54C",

	// 1357
	"unknown54D",

	// 1358
	"unknown54E",

	// 1359
	"unknown54F",

	// 1360
	"unknown550",

	// 1361
	"unknown551",

	// 1362
	"unknown552",

	// 1363
	"unknown553",

	// 1364
	"unknown554",

	// 1365
	"cache_block_for_one_frame",

	// 1366
	"sound_suppress_ambience_update_on_revert",

	// 1367
	"render_autoexposure_enable",

	// 1368
	"render_exposure_full",

	// 1369
	"render_exposure_fade_in",

	// 1370
	"render_exposure_fade_out",

	// 1371
	"render_exposure",

	// 1372
	"render_autoexposure_instant",

	// 1373
	"render_exposure_set_environment_darken",

	// 1374
	"render_depth_of_field_enable",

	// 1375
	"render_depth_of_field",

	// 1376
	"render_dof_focus_depth",

	// 1377
	"render_dof_blur_animate",

	// 1378
	"render_debug_video_mode",

	// 1379
	"cinematic_lightmap_shadow_disable?",

	// 1380
	"cinematic_lightmap_shadow_enable?",

	// 1381
	"predict_animation",

	// 1382
	"game_team_get_players?",

	// 1383
	"game_team_get_player_count?",

	// 1384
	"deterministic_end_game",

	// 1385
	"mp_game_won",

	// 1386
	"mp_respawn_override_timers",

	// 1387
	"mp_ai_allegiance",

	// 1388
	"mp_allegiance",

	// 1389
	"mp_round_started",

	// 1390
	"give_medal",

	// 1391
	"mp_scripts_reset",

	// 1392
	"mp_ai_place",

	// 1393
	"mp_ai_place",

	// 1394
	"mp_ai_kill",

	// 1395
	"mp_ai_kill_silent",

	// 1396
	"mp_object_create",

	// 1397
	"mp_object_create_clone",

	// 1398
	"mp_object_create_anew",

	// 1399
	"mp_object_destroy",

	// 1400
	"tag_file_set_backend",

	// 1401
	"mp_object_belongs_to_team",

	// 1402
	"mp_weapon_belongs_to_team",

	// 1403
	"mp_debug_goal_object_boundary_geometry",

	// 1404
	"mp_dump_candy_monitor_state",

	// 1405
	"set_camera_third_person",

	// 1406
	"get_camera_third_person",

	// 1407
	"determinism_debug_manager_enable_logging",

	// 1408
	"determinism_debug_manager_set_trace_flags",

	// 1409
	"determinism_debug_manager_enable_game_state_checksum",

	// 1410
	"determinism_debug_manager_enable_trace",

	// 1411
	"determinism_debug_manager_set_consumer_sample_level",

	// 1412
	"unknown584",

	// 1413
	"saved_film_play",

	// 1414
	"saved_film_play_last",

	// 1415
	"unknown587",

	// 1416
	"unknown588",

	// 1417
	"unknown589",

	// 1418
	"unknown58A",

	// 1419
	"unknown58B",

	// 1420
	"mover_set_program",

	// 1421
	"determinism_log_compare_log_files",

	// 1422
	"floating_point_exceptions_enable",

	// 1423
	"determinism_debug_manager_enable_log_file_comparision_on_oos",

	// 1424
	"event_logs_snapshot",

	// 1425
	"tag_reload_force",

	// 1426
	"tag_unload_force",

	// 1427
	"tag_load_force",

	// 1428
	"predict_bink_movie",

	// 1429
	"predict_bink_movie_from_tag",

	// 1430
	"profiler_dump_history",

	// 1431
	"camera_set_mode",

	// 1432
	"camera_set_flying_cam_at_point",

	// 1433
	"camera_set_target",

	// 1434
	"game_coop_player_count",

	// 1435
	"player_force_mode",

	// 1436
	"profiler_output_pulse",

	// 1437
	"string_id_name",

	// 1438
	"find",

	// 1439
	"add_recycling_volume",

	// 1440
	"tag_resources_set_per_frame_publish",

	// 1441
	"object_recycling_clear_history",

	// 1442
	"dump_cinematics_script",

	// 1443
	"global_preferences_clear",

	// 1444
	"network_storage_set_storage_subdirectory",

	// 1445
	"network_storage_set_storage_user",

	// 1446
	"status_line_dump",

	// 1447
	"game_tick_get",

	// 1448
	"loop_it",

	// 1449
	"loop_clear",

	// 1450
	"status_lines_enable",

	// 1451
	"status_lines_disable",

	// 1452
	"on_target_platform",

	// 1453
	"profile_activate",

	// 1454
	"profile_deactivate",

	// 1455
	"net_game_set_player_standing",

	// 1456
	"unknown5B0",

	// 1457
	"unknown5B1",

	// 1458
	"unknown5B2",

	// 1459
	"net_test_matchmaking_hopper_set_game",

	// 1460
	"unknown5B4",

	// 1461
	"unknown5B5",

	// 1462
	"noguchis_mystery_tour",

	// 1463
	"designer_zone_sync",

	// 1464
	"dump_designer_zone",

	// 1465
	"designer_zone_activate",

	// 1466
	"designer_zone_deactivate",

	// 1467
	"object_set_always_active",

	// 1468
	"saved_film_seek_to_film_tick",

	// 1469
	"tag_is_active",

	// 1470
	"tag_resources_enable_optional_caching",

	// 1471
	"dump_active_resources",

	// 1472
	"object_set_persistent",

	// 1473
	"display_zone_size_estimates",

	// 1474
	"report_zone_size_estimates",

	// 1475
	"net_test_disconnect_squad",

	// 1476
	"net_test_disconnect_group",

	// 1477
	"net_test_clear_squad_session_parameter",

	// 1478
	"net_test_clear_group_session_parameter",

	// 1479
	"net_test_life_cycle_pause",

	// 1480
	"net_test_life_cycle_display_states",

	// 1481
	"overlapped_display_task_descriptions",

	// 1482
	"overlapped_task_inject_error",

	// 1483
	"net_leaderboard_clear_hopper_all_users",

	// 1484
	"net_leaderboard_clear_global_arbitrated_all_users",

	// 1485
	"net_leaderboard_clear_global_unarbitrated_all_users",

	// 1486
	"test_sapien_crash",

	// 1487
	"output_window_add_category",

	// 1488
	"output_window_remove_category",

	// 1489
	"output_window_list_categories",

	// 1490
	"decorators_split",

	// 1491
	"bandwidth_profiler_enable",

	// 1492
	"bandwidth_profiler_set_context",

	// 1493
	"overlapped_task_pause",

	// 1494
	"net_banhammer_set_controller_cheat_flags",

	// 1495
	"net_banhammer_set_controller_ban_flags",

	// 1496
	"net_banhammer_dump_strings",

	// 1497
	"net_banhammer_dump_repeated_play_list",

	// 1498
	"net_leaderboard_set_user_stats",

	// 1499
	"net_leaderboard_set_user_game_stats",

	// 1500
	"net_build_map_variant",

	// 1501
	"net_verify_map_variant",

	// 1502
	"async_set_work_delay_milliseconds",

	// 1503
	"sound_looping_start_with_effect",

	// 1504
	"tag_resources_set_demand_throttle_to_io",

	// 1505
	"set_performance_throttle",

	// 1506
	"global_preferences_quality_set_all",

	// 1507
	"get_performance_throttle",

	// 1508
	"unknown5E4",

	// 1509
	"cinematic_zone_activate",

	// 1510
	"cinematic_zone_deactivate",

	// 1511
	"cinematic_zone_activate_from_editor",

	// 1512
	"cinematic_zone_deactivate_from_editor",

	// 1513
	"tiling_current",

	// 1514
	"unit_limit_lipsync_to_mouth_only",

	// 1515
	"dump_active_zone_tags",

	// 1516
	"calculate_tag_prediction",

	// 1517
	"tag_resources_enable_fast_prediction",

	// 1518
	"unit_start_first_person_custom_animation",

	// 1519
	"unit_is_playing_custom_first_person_animation",

	// 1520
	"unit_stop_first_person_custom_animation",

	// 1521
	"prepare_to_switch_to_zone_set",

	// 1522
	"cinematic_zone_activate_for_debugging",

	// 1523
	"unit_play_random_ping",

	// 1524
	"player_control_fade_out_all_input",

	// 1525
	"player_control_fade_in_all_input",

	// 1526
	"unit_control_fade_out_all_input",

	// 1527
	"unit_control_fade_in_all_input",

	// 1528
	"player_control_lock_gaze",

	// 1529
	"player_control_unlock_gaze",

	// 1530
	"player_control_scale_all_input",

	// 1531
	"run_like_dvd",

	// 1532
	"profiler_auto_core_save",

	// 1533
	"run_no_hdd",

	// 1534
	"cinematic_tag_reference_get_bink",

	// 1535
	"voice_set_force_match_configurations",

	// 1536
	"voice_set_force_hud",

	// 1537
	"object_set_custom_animation_speed",

	// 1538
	"scenery_animation_start_at_frame_loop",

	// 1539
	"film_manager_set_reproduction_mode",

	// 1540
	"cortana_tag_reference_get_scene",

	// 1541
	"net_banhammer_force_download",

	// 1542
	"font_set_emergency",

	// 1543
	"biped_force_ground_fitting_on",

	// 1544
	"cinematic_set_chud_objective",

	// 1545
	"chud_show_cinematic_title",

	// 1546
	"terminal_is_being_read",

	// 1547
	"terminal_was_accessed",

	// 1548
	"terminal_was_completed",

	// 1549
	"unit_get_primary_weapon",

	// 1550
	"dump_cortana_script",

	// 1551
	"budget_resource_get_animation_graph",

	// 1552
	"game_award_level_complete_achievements",

	// 1553
	"budget_resource_get_looping_sound",

	// 1554
	"game_safe_to_respawn",

	// 1555
	"cortana_effect_kill",

	// 1556
	"test_create_content_item_slayer",

	// 1557
	"test_create_content_item_screenshot",

	// 1558
	"cinematic_scripting_destroy_cortana_effect_cinematic",

	// 1559
	"ai_migrate_infanty",

	// 1560
	"render_cinematic_motion_blur",

	// 1561
	"ai_dont_do_avoidance",

	// 1562
	"cinematic_scripting_clean_up",

	// 1563
	"ai_erase_inactive",

	// 1564
	"ai_survival_cleanup",

	// 1565
	"stop_bink_movie",

	// 1566
	"play_credits_unskippable",

	// 1567
	"budget_resource_get_sound",

	// 1568
	"controller_set_single_player_level_unlocked",

	// 1569
	"physical_memory_dump",

	// 1570
	"tag_resources_validate_all_pages",

	// 1571
	"cinematic_set_debug_mode",

	// 1572
	"cinematic_scripting_get_object",

	// 1573
	"dummy_function",

	// 1574
	"gp_integer_get",

	// 1575
	"gp_integer_set",

	// 1576
	"gp_boolean_get",

	// 1577
	"gp_boolean_set",

	// 1578
	"gp_dump",

	// 1579
	"gp_dump_debug",

	// 1580
	"gp_startup",

	// 1581
	"gp_reset",

	// 1582
	"gp_commit_options",

	// 1583
	"cinematic_scripting_start_screen_effect",

	// 1584
	"cinematic_scripting_stop_screen_effect",

	// 1585
	"game_level_prepare",

	// 1586
	"prepare_game_level",

	// 1587
	"game_start_with_network_session",

	// 1588
	"levels_add_campaign_map_with_id",

	// 1589
	"levels_add_campaign_map",

	// 1590
	"sound_impulse_start_editor",

	// 1591
	"sound_impulse_start_effect_editor",

	// 1592
	"sound_impulse_start_3d_editor",

	// 1593
	"sound_looping_start_editor",

	// 1594
	"debug_sound_channels_log_start",

	// 1595
	"debug_sound_channels_log_start_named",

	// 1596
	"debug_sound_channels_log_stop",

	// 1597
	"cinematic_scripting_set_user_input_constraints",

	// 1598
	"is_skull_primary_enable",

	// 1599
	"is_skull_secondary_enable",

	// 1600
	"controller_set_popup_message_index",

	// 1601
	"controller_set_vidmaster_seen",

	// 1602
	"gui_enter_lobby",

	// 1603
	"survival_mode_respawn_dead_players",

	// 1604
	"survival_mode_lives_get",

	// 1605
	"survival_mode_lives_set",

	// 1606
	"survival_mode_set_get",

	// 1607
	"survival_mode_set_set",

	// 1608
	"survival_mode_round_get",

	// 1609
	"survival_mode_round_set",

	// 1610
	"survival_mode_wave_get",

	// 1611
	"survival_mode_wave_set",

	// 1612
	"survival_mode_set_multiplier_get",

	// 1613
	"survival_mode_set_multiplier_set",

	// 1614
	"survival_mode_round_multiplier_get?",

	// 1615
	"survival_mode_round_multiplier_set?",

	// 1616
	"unknown650",

	// 1617
	"survival_mode_set_rounds_per_set",

	// 1618
	"survival_mode_set_waves_per_round",

	// 1619
	"render_model_marker_name_filter",

	// 1620
	"survival_mode_event_new",

	// 1621
	"survival_mode_begin_new_set",

	// 1622
	"survival_mode_begin_new_round",

	// 1623
	"survival_mode_begin_new_wave",

	// 1624
	"survival_mode_award_hero_medal",

	// 1625
	"campaign_metagame_get_player_score",

	// 1626
	"collision_debug_lightmaps_print",

	// 1627
	"debug_coop_campaign_save",

	// 1628
	"player_set_look_training_hack",

	// 1629
	"device_arg_has_been_touched_by_unit",

	// 1630
	"gui_hide_all_screens",

	// 1631
	"debug_lightmap_inspect",

	// 1632
	"achievements_display_debug_names",

	// 1633
	"achievement_grant_to_controller_by_debug_name",

	// 1634
	"achievements_grant_all_to_controller",

	// 1635
	"achievements_award_all_vidmaster_achievements",

	// 1636
	"chud_fade_for_first_person_cinematic",

	// 1637
	"achievement_post_chud_progression",

	// 1638
	"object_set_vision_mode_render_default",

	// 1639
	"chud_show_navpoint",

	// 1640
	"unknown668",

	// 1641
	"unknown669",

	// 1642
	"unknown66A",

	// 1643
	"unit_confirm_message",

	// 1644
	"unit_confirm_cancel_message",

	// 1645
	"unit_confirm_y_button",

	// 1646
	"player_get_kills_by_type",

	// 1647
	"unit_flashlight_on",

	// 1648
	"clear_command_buffer_cache_from_script",

	// 1649
	"sound_looping_resume",

	// 1650
	"chud_bonus_round_set_target_score",

	// 1651
	"unknown673",

	// 1652
	"unknown674",

	// 1653
	"unknown675",

	// 1654
	"unknown676",

	// 1655
	"unknown677",

	// 1656
	"unknown678",

	// 1657
	"unknown679",

	// 1658
	"unknown67A",

	// 1659
	"unknown67B",

	// 1660
	"unknown67C",

	// 1661
	"ui_get_player_model_id",

	// 1662
	"ui_get_music_id",

	// 1663
	"unknown67F",

	// 1664
	"unit_set_armor",

	// 1665
	"player_set_armor",

	// 1666
	"change_network_privacy_to_system_link",

	// 1667
	"ssao_enable",

	// 1668
	"ssao_radius",

	// 1669
	"ssao_intensity",

	// 1670
	"ssao_sampleZThreshold",

	// 1671
	"fxaa_enable",

	// 1672
	"color_grading_enable",

	// 1673
	"lightshafts_enable",

	// 1674
	"unknown68A",

	// 1675
	"high_quality_postprocessing_enable",

	// 1676
	"unknown68C",

	// 1677
	"unknown68D",

	// 1678
	"unknown68E",

	// 1679
	"unknown68F",

	// 1680
	"debug_clear_z_color_stencil",

	// 1681
	"unknown691",

	// 1682
	"end_match_wait_time_increment",

	// 1683
	"unknown693",

	// 1684
	"background_set",

	// 1685
	"local_player_earn_wp_event",

	// 1686
	"unknown696",

	// 1687
	"unknown697",

	// 1688
	"unknown698",

	// 1689
	"mouse_inversion_get",

	// 1690
	"mouse_inversion_set",

	// 1691
	"unknown69B",

	// 1692
	"unknown69C",

	// 1693
	"unknown69D",

	// 1694
	"unknown69E",

	// 1695
	"unknown69F",

	// 1696
	"unknown6A0",
};

hs_function_definition_debug const* const hs_function_table_debug[]
{
	MAKE_HS_FUNCTION_TABLE_ENTRY(_hs_passthrough, begin, FLAG(1), NULL, hs_function_table[0]->evaluate, "returns the last expression in a sequence after evaluating the sequence in order.\r\nNETWORK SAFE: Yes", "<expression(s)>", 0, _hs_unparsed),
	//MAKE_HS_FUNCTION_TABLE_ENTRY2(_hs_passthrough, begin, FLAG(1), "returns the last expression in a sequence after evaluating the sequence in order.\r\nNETWORK SAFE: Yes", "<expression(s)>", 0, _hs_unparsed)
};
long const hs_function_table_debug_count = NUMBEROF(hs_function_table_debug);

#undef MAKE_HS_FUNCTION_TABLE_ENTRY2
#undef MAKE_HS_FUNCTION_TABLE_ENTRY

