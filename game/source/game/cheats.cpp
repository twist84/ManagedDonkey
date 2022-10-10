#include "game/cheats.hpp"

#include "camera/observer.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "objects/objects.hpp"

#include <assert.h>

void cheat_teleport_to_camera()
{
    FUNCTION_BEGIN(true);

    long output_user = player_mapping_first_active_output_user();
    if (output_user == -1)
        return;

    s_observer_result const* result = observer_get_camera(output_user);
    assert(result);

    if (result->location.cluster_reference.bsp_index == 0xFF)
    {
        c_console::write_line("Camera is outside BSP... cannot initiate teleportation...");
        return;
    }

    long unit_index = player_mapping_get_unit_by_output_user(output_user);
    long parent_object = object_get_ultimate_parent(unit_index);
    real_point3d position = result->focus_point;

    object_debug_teleport(parent_object, &position);
}