#pragma once

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

extern void __cdecl collision_log_end_frame();
extern void __cdecl collision_log_begin_frame();

extern void collision_log_render();

