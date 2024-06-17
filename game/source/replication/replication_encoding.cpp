#include "replication/replication_encoding.hpp"

#include "memory/bitstream.hpp"

bool __cdecl player_prediction_decode(c_bitstream* packet, s_player_prediction* player_prediction, bool replicated)
{
    return INVOKE(0x004AA9E0, player_prediction_decode, packet, player_prediction, replicated);
}

void __cdecl player_prediction_encode(c_bitstream* packet, s_player_prediction const* player_prediction, bool replicated)
{
    INVOKE(0x004AAAD0, player_prediction_encode, packet, player_prediction, replicated);
}

