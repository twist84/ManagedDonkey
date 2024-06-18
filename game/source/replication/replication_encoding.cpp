#include "replication/replication_encoding.hpp"

#include "memory/bitstream.hpp"

bool __cdecl player_motion_compare(s_player_motion const* player_motion_a, s_player_motion* player_motion_b)
{
    return INVOKE(0x004A9F50, player_motion_compare, player_motion_a, player_motion_b);
}

bool __cdecl player_motion_decode(c_bitstream* packet, s_player_motion* player_motion)
{
    return INVOKE(0x004AA0A0, player_motion_decode, packet, player_motion);
}

void __cdecl player_motion_encode(c_bitstream* packet, s_player_motion const* player_motion)
{
    INVOKE(0x004AA260, player_motion_encode, packet, player_motion);
}

long __cdecl player_motion_minimum_required_bits()
{
    return INVOKE(0x004AA750, player_motion_minimum_required_bits);
}

bool __cdecl player_motion_valid(s_player_motion const* player_motion)
{
    return INVOKE(0x004AA760, player_motion_valid, player_motion);
}

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

void __cdecl predicted_aim_assist_clear(s_player_predicted_aim_assist* aim_assist)
{
    INVOKE(0x004AAE10, predicted_aim_assist_clear, aim_assist);
}

bool __cdecl predicted_aim_assist_compare(s_player_predicted_aim_assist const* aim_assist_a, s_player_predicted_aim_assist* aim_assist_b)
{
    return INVOKE(0x004AAE40, predicted_aim_assist_compare, aim_assist_a, aim_assist_b);
}

//bool __cdecl predicted_aim_assist_decode(c_bitstream* packet, s_player_predicted_aim_assist* aim_assist, e_aim_assist_encoding_precision encoding_precision)
bool __cdecl predicted_aim_assist_decode(c_bitstream* packet, s_player_predicted_aim_assist* aim_assist, long encoding_precision)
{
    return INVOKE(0x004AAEE0, predicted_aim_assist_decode, packet, aim_assist, encoding_precision);
}

//void __cdecl predicted_aim_assist_encode(c_bitstream* packet, s_player_predicted_aim_assist const* aim_assist, bool a3, e_aim_assist_encoding_precision encoding_precision)
void __cdecl predicted_aim_assist_encode(c_bitstream* packet, s_player_predicted_aim_assist const* aim_assist, bool a3, long encoding_precision)
{
    INVOKE(0x004AAFE0, predicted_aim_assist_encode, packet, aim_assist, a3, encoding_precision);
}

