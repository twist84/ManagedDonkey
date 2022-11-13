#include "game/multiplayer_definitions.hpp"

#include "cache/cache_files.hpp"
#include "game/game_globals.hpp"
#include "scenario/scenario.hpp"

s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data()
{
    return INVOKE(0x0069D310, scenario_multiplayer_globals_try_and_get_runtime_data);
}

s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data()
{
    s_game_globals* game_globals = scenario_try_and_get_game_globals();

    s_multiplayer_globals_definition* multiplayer_globals = nullptr;
    if (game_globals)
        multiplayer_globals = static_cast<s_multiplayer_globals_definition*>(tag_get('mulg', game_globals->multiplayer_globals.index));

    s_multiplayer_universal_globals_definition* universal_globals = nullptr;
    if (multiplayer_globals)
        universal_globals = multiplayer_globals_try_and_get_universal_data(multiplayer_globals);

    return universal_globals;

    return INVOKE(0x0069D340, scenario_multiplayer_globals_try_and_get_universal_data);
}

s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals)
{
    if (multiplayer_globals && multiplayer_globals->universal.count > 0)
        return &multiplayer_globals->universal.elements[0];

    return nullptr;
}

