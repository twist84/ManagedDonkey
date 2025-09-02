#include "render/render_lightmap_shadows.hpp"

#include "render/views/render_view.hpp"

REFERENCE_DECLARE(0x01918160, bool, c_lightmap_shadows_view::g_debug_shadow_screenspace);
REFERENCE_DECLARE(0x05115B3E, bool, c_lightmap_shadows_view::g_debug_shadow_bounds);            // unused
REFERENCE_DECLARE(0x05115B3F, bool, c_lightmap_shadows_view::g_debug_shadow_bounds_solid);
REFERENCE_DECLARE(0x05115B40, bool, c_lightmap_shadows_view::g_debug_shadow_opaque);            // unused
REFERENCE_DECLARE(0x05115B41, bool, c_lightmap_shadows_view::g_debug_shadow_histencil);
REFERENCE_DECLARE(0x05115B42, bool, c_lightmap_shadows_view::g_debug_shadow_force_hi_res);      // unused
REFERENCE_DECLARE(0x05115B43, bool, c_lightmap_shadows_view::g_debug_objectspace_stencil_clip);
REFERENCE_DECLARE(0x05115B44, bool, c_lightmap_shadows_view::g_debug_force_fancy_shadows);      // unused
REFERENCE_DECLARE(0x05115B45, bool, c_lightmap_shadows_view::g_debug_force_old_shadows);        // unused

