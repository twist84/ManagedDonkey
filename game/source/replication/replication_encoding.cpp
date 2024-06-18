#include "replication/replication_encoding.hpp"

#include "memory/bitstream.hpp"

bool __cdecl player_prediction_compare_identical(s_player_prediction const* player_prediction_a, s_player_prediction const* player_prediction_b)
{
    return INVOKE(0x004AA8D0, player_prediction_compare_identical, player_prediction_a, player_prediction_b);
}

bool __cdecl player_prediction_decode(c_bitstream* packet, s_player_prediction* player_prediction, bool replicated)
{
    return INVOKE(0x004AA9E0, player_prediction_decode, packet, player_prediction, replicated);
}

void __cdecl player_prediction_encode(c_bitstream* packet, s_player_prediction const* player_prediction, bool replicated)
{
    INVOKE(0x004AAAD0, player_prediction_encode, packet, player_prediction, replicated);
}

long __cdecl player_prediction_minimum_required_bits()
{
    return INVOKE(0x004AAD30, player_prediction_minimum_required_bits);
}

bool __cdecl player_prediction_valid(s_player_prediction const* player_prediction)
{
    return INVOKE(0x004AAD40, player_prediction_valid, player_prediction);
}

