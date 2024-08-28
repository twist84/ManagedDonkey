#pragma once

#define NUMBER_OF_COLLISION_TIME_PERIODS 7
#define NUMBER_OF_COLLISION_FUNCTION_TYPES 12
#define NUMBER_OF_COLLISION_USER_TYPES 28

#define MAXIMUM_COLLISION_PERIOD_STACK_DEPTH 8
#define MAXIMUM_COLLISION_USER_STACK_DEPTH 32

extern bool collision_log_render_enable;
extern bool collision_log_detailed;
extern bool collision_log_extended;
extern bool collision_log_totals_only;
extern bool collision_log_time;
extern bool global_collision_log_enable;
extern bool global_collision_log_switch_pending;
extern bool global_collision_log_switch_pending_value;
extern short global_current_collision_user_depth;
extern short global_collision_period_depth;

extern short global_current_collision_users[MAXIMUM_COLLISION_USER_STACK_DEPTH];
extern char const* const global_collision_function_names[NUMBER_OF_COLLISION_FUNCTION_TYPES];
extern char const* const global_collision_user_names[NUMBER_OF_COLLISION_USER_TYPES];

extern void __cdecl collision_log_initialize();
extern void __cdecl collision_log_dispose();
extern void __cdecl collision_log_end_frame();
extern void __cdecl collision_log_begin_frame();

extern void collision_log_render();

