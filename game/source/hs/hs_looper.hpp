#pragma once

#include "cseries/cseries.hpp"

class c_hs_looper
{
public:
    c_hs_looper();

    void clear();

private:
    int32 create_suspended_loop_thread();

public:
    void game_tick();
    void handle_thread_delete(int32 thread_index);
    void reboot_loop();
    void reinitialize();
    void restore_from_saved_game();
    void set_loop_expression(const char* new_loop_expression);

private:
    c_static_string<4096> m_loop_expression;
    int32 m_loop_thread_index;
    bool m_loop_enabled;
};
static_assert(sizeof(c_hs_looper) == 0x1008);

extern void hs_looper_clear();
extern void hs_looper_game_tick();
extern void hs_looper_handle_thread_delete(int32 thread_index);
extern void hs_looper_reboot();
extern void hs_looper_reinitialize();
extern void hs_looper_restore_from_saved_game();
extern void hs_looper_setup(const char*expression);

