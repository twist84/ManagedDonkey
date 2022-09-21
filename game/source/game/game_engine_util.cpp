#include "game/game_engine_util.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_slayer.hpp"
#include "game/game_engine_oddball.hpp"
#include "game/game_engine_king.hpp"
#include "game/game_engine_sandbox.hpp"
#include "game/game_engine_vip.hpp"
#include "game/game_engine_juggernaut.hpp"
#include "game/game_engine_territories.hpp"
#include "game/game_engine_assault.hpp"
#include "game/game_engine_infection.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE(0x005CE150, current_game_engine);

c_game_engine*(&game_engines)[11] = *reinterpret_cast<c_game_engine*(*)[11]>(0x0471A920);

c_game_engine*& ctf_engine = *reinterpret_cast<c_game_engine**>(0x0190B890);
c_ctf_engine& internal_ctf_engine = *reinterpret_cast<c_ctf_engine*>(0x0190B8D0);

c_game_engine*& slayer_engine = *reinterpret_cast<c_game_engine**>(0x0190E1E8);
c_slayer_engine& internal_slayer_engine = *reinterpret_cast<c_slayer_engine*>(0x0190E218);

c_game_engine*& oddball_engine = *reinterpret_cast<c_game_engine**>(0x0190E23C);
c_oddball_engine& internal_oddball_engine = *reinterpret_cast<c_oddball_engine*>(0x0190E26C);

c_game_engine*& king_engine = *reinterpret_cast<c_game_engine**>(0x0190E290);
c_king_engine& internal_king_engine = *reinterpret_cast<c_king_engine*>(0x0190E2C0);

c_game_engine*& sandbox_engine = *reinterpret_cast<c_game_engine**>(0x018A1564);
c_sandbox_engine& internal_sandbox_engine = *reinterpret_cast<c_sandbox_engine*>(0x018A15BC);

c_game_engine*& vip_engine = *reinterpret_cast<c_game_engine**>(0x0190E3E8);
c_vip_engine& internal_vip_engine = *reinterpret_cast<c_vip_engine*>(0x0190E418);

c_game_engine*& juggernaut_engine = *reinterpret_cast<c_game_engine**>(0x0190E2E0);
c_juggernaut_engine& internal_juggernaut_engine = *reinterpret_cast<c_juggernaut_engine*>(0x0190E310);

c_game_engine*& territories_engine = *reinterpret_cast<c_game_engine**>(0x0190E338);
c_territories_engine& internal_territories_engine = *reinterpret_cast<c_territories_engine*>(0x0190E368);

c_game_engine*& assault_engine = *reinterpret_cast<c_game_engine**>(0x018EB5A8);
c_assault_engine& internal_assault_engine = *reinterpret_cast<c_assault_engine*>(0x018EB5E8);

c_game_engine*& infection_engine = *reinterpret_cast<c_game_engine**>(0x0190E390);
c_infection_engine& internal_infection_engine = *reinterpret_cast<c_infection_engine*>(0x0190E3C0);

c_game_engine const* __cdecl current_game_engine()
{
	FUNCTION_BEGIN(false)

	c_game_engine const* result = nullptr;
	HOOK_INVOKE(result =, current_game_engine);

	game_engine_globals* game_engine = get_tls()->game_engine_globals;
	if (game_engine && (game_engine->GameType > 0 && game_engine->GameType < 11))
		return game_engines[game_engine->GameType];

	return result;
}
