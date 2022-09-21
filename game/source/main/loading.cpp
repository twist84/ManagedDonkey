#include "main/loading.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0052F180, main_load_map);

void __cdecl loading_basic_progress_complete()
{
    INVOKE(0x0052EDF0, loading_basic_progress_complete);
}

void __cdecl loading_basic_progress_disable()
{
    INVOKE(0x0052EE20, loading_basic_progress_disable);
}

void __cdecl loading_basic_progress_enable(char const* scenario_path, long insertion_point)
{
    INVOKE(0x0052EE40, loading_basic_progress_enable, scenario_path, insertion_point);
}

bool __cdecl loading_basic_progress_enabled()
{
    return INVOKE(0x0052EEA0, loading_basic_progress_enabled);
}

real __cdecl loading_basic_progress_get()
{
    return INVOKE(0x0052EEB0, loading_basic_progress_get);
}

void __cdecl loading_basic_progress_phase_begin(long phase, dword update_size)
{
    INVOKE(0x0052EEC0, loading_basic_progress_phase_begin, phase, update_size);
}

void __cdecl loading_basic_progress_phase_end()
{
    INVOKE(0x0052EEF0, loading_basic_progress_phase_end);
}

void __cdecl loading_basic_progress_update_fraction(real update_fraction)
{
    INVOKE(0x0052EF10, loading_basic_progress_update_fraction, update_fraction);
}

void __cdecl loading_basic_progress_update_phase_sizes()
{
    INVOKE(0x0052EF70, loading_basic_progress_update_phase_sizes);
}

void __cdecl loading_basic_progress_update_size(dword update_size)
{
    INVOKE(0x0052F010, loading_basic_progress_update_size, update_size);
}

bool __cdecl main_blocking_load_in_progress(real* out_progress)
{
    return INVOKE(0x0052F130, main_blocking_load_in_progress, out_progress);
}

bool force_load_map_failed = false;

//bool __cdecl main_load_map(char const *,enum e_map_load_type)
bool __cdecl main_load_map(char* scenario_path, long map_load_type)
{
    if (force_load_map_failed)
        return false;

    bool result = false;
    HOOK_INVOKE(result =, main_load_map, scenario_path, map_load_type);
    return result;
}

// a2 is possibly insertion_point
real __cdecl main_load_map_loading_progress(long scenario_type, short a2, char const* scenario_path)
{
    return INVOKE(0x0052F1A0, main_load_map_loading_progress, scenario_type, a2, scenario_path);
}

//enum e_map_load_status __cdecl main_load_map_status(char const*)
long __cdecl main_load_map_status(char const* scenario_path)
{
    return INVOKE(0x0052F250, main_load_map_status, scenario_path);
}

//enum e_map_load_status __cdecl main_load_map_status_with_insertion_point(short,char const *)
long __cdecl main_load_map_status_with_insertion_point(short insertion_point, char const* scenario_path)
{
    return INVOKE(0x0052F270, main_load_map_status_with_insertion_point, insertion_point, scenario_path);
}

//bool __cdecl main_load_map_with_insertion_point(short, char const*, enum e_map_load_type)
bool __cdecl main_load_map_with_insertion_point(short insertion_point, char const* scenario_path, long map_load_type)
{
    return INVOKE(0x0052F2E0, main_load_map_with_insertion_point, insertion_point, scenario_path, map_load_type);
}

bool __cdecl main_load_next_map_loading()
{
    return INVOKE(0x0052F480, main_load_next_map_loading);
}

void __cdecl main_loading_dispose()
{
    INVOKE(0x0052F4E0, main_loading_dispose);
}

void __cdecl main_loading_enable_spinner(bool enable_spinner)
{
    INVOKE(0x0052F4F0, main_loading_enable_spinner, enable_spinner);
}

bool __cdecl main_loading_get_action(struct s_main_loading_action* out_loading_action)
{
    return INVOKE(0x0052F500, main_loading_get_action, out_loading_action);
}

//enum e_gui_game_mode __cdecl main_loading_get_gui_game_mode(void)
long __cdecl main_loading_get_gui_game_mode()
{
    return INVOKE(0x0052F8F0, main_loading_get_gui_game_mode);
}

//enum e_main_pregame_frame __cdecl main_loading_get_loading_status(class c_static_wchar_string<12288> *)
long __cdecl main_loading_get_loading_status(wchar_t* status_message)
{
    return INVOKE(0x0052F930, main_loading_get_loading_status, status_message);
}

void __cdecl main_loading_idle_with_blocking_load(bool blocking_load)
{
    INVOKE(0x0052FA00, main_loading_idle_with_blocking_load, blocking_load);
}

void __cdecl main_loading_initialize()
{
    INVOKE(0x0052FAD0, main_loading_initialize);
}

bool __cdecl main_loading_is_idle()
{
    return INVOKE(0x0052FB40, main_loading_is_idle);
}

void __cdecl main_loading_progress_done(char const* scenario_path, void* a2)
{
    INVOKE(0x0052FB60, main_loading_progress_done, scenario_path, a2);
}

void __cdecl main_loading_progress_new(char const* scenario_path, void* a2)
{
    INVOKE(0x0052FB70, main_loading_progress_done, scenario_path, a2);
}

void __cdecl main_loading_progress_update(char const* scenario_path, char const* a2, long progress, void* a4)
{
    INVOKE(0x0052FB80, main_loading_progress_update, scenario_path, a2, progress, a4);
}