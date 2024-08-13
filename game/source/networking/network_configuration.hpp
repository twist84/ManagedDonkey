#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_event_configuration
{
	real __unknown0;
	dword __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
};
static_assert(sizeof(s_simulation_event_configuration) == 0x14);

struct s_simulation_entity_creation_configuration
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
};
static_assert(sizeof(s_simulation_entity_creation_configuration) == 0x10);

struct s_simulation_entity_update_configuration
{
	dword __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	dword __unknown10;
	dword __unknown14;
	real __unknown18;
	real __unknown1C;
	dword __unknown20;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	dword __unknown30;
	dword __unknown34;
	dword __unknown38;
};
static_assert(sizeof(s_simulation_entity_update_configuration) == 0x3C);

struct s_simulation_entity_configuration
{
	s_simulation_entity_creation_configuration creation_configuration;
	s_simulation_entity_update_configuration update_configuration;
};
static_assert(sizeof(s_simulation_entity_configuration) == 0x4C);

struct s_simulation_configuration
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
	real __unknown14;
	real __unknown18;
	dword __unknown1C;
	dword __unknown20;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	real __unknown30;
	dword __unknown34;
	dword __unknown38;
	dword __unknown3C;
	dword __unknown40;
	dword __unknown44;
	dword __unknown48;
	dword __unknown4C;
	dword __unknown50;
	dword __unknown54;
	dword __unknown58;
	dword __unknown5C;
	real __unknown60;
	real __unknown64;
	real __unknown68;
	dword __unknown6C;
	dword __unknown70;
	dword __unknown74;
	dword __unknown78;
	dword __unknown7C;
	real __unknown80;
	real __unknown84;
	dword __unknown88;
	dword __unknown8C;
	dword __unknown90;
	dword __unknown94;
	real __unknown98;
	real __unknown9C;
	c_static_array<s_simulation_event_configuration, 39> simulation_event_configurations;
	c_static_array<s_simulation_entity_configuration, 22> simulation_entity_configurations;
	real __unknownA34;
	real __unknownA38;
	real __unknownA3C;
	real __unknownA40;
	real __unknownA44;
	real __unknownA48;
	real __unknownA4C;
	real __unknownA50;
	real __unknownA54;
	real __unknownA58;
	dword __unknownA5C;
};
static_assert(sizeof(s_simulation_configuration) == 0xA60);

struct s_observer_configuration
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
	dword __unknown10;
	dword __unknown14;
	dword __unknown18;
	dword __unknown1C;
	dword __unknown20;
	dword __unknown24;
	dword __unknown28;
	dword __unknown2C;
	dword __unknown30;
	dword __unknown34;
	dword __unknown38;
	dword __unknown3C;
	dword __unknown40;
	dword __unknown44;
	dword __unknown48;
	dword __unknown4C;
	dword __unknown50;
	dword __unknown54;
	dword __unknown58;
	dword __unknown5C;
	dword __unknown60;
	dword __unknown64;
	dword __unknown68;

	dword connection_initiation_retry_count;

	dword __unknown70;
	dword __unknown74;
	dword __unknown78;
	dword __unknown7C;
	dword __unknown80;
	dword __unknown84;
	dword __unknown88;
	dword __unknown8C;
	dword __unknown90;
	dword __unknown94;
	dword __unknown98;
	dword __unknown9C;
	dword __unknownA0;
	dword __unknownA4;
	dword __unknownA8;
	dword __unknownAC;
	dword __unknownB0;
	dword __unknownB4;
	dword __unknownB8;
	real __unknownBC;
	real __unknownC0;
	real __unknownC4;
	dword __unknownC8;
	real __unknownCC;
	real __unknownD0;
	real __unknownD4;
	real __unknownD8;
	real __unknownDC;
	real __unknownE0;
	real __unknownE4;
	dword __unknownE8;
	dword __unknownEC;
	dword __unknownF0;
	dword __unknownF4;
	dword __unknownF8;
	dword __unknownFC;
	dword __unknown100;
	dword __unknown104;
	dword __unknown108;
	dword __unknown10C;
	real __unknown110;
	real __unknown114;
	real __unknown118;
	real __unknown11C;
	dword __unknown120;

	dword period_duration_msec;

	dword __unknown128;
	dword __unknown12C;
	real __unknown130;
	dword __unknown134;
	dword __unknown138;
	real __unknown13C;
	dword __unknown140;
	dword __unknown144;
	real __unknown148;
	dword __unknown14C;
	dword __unknown150;
	dword __unknown154;
	dword __unknown158;
	dword __unknown15C;
	dword __unknown160;
	dword __unknown164;
	dword __unknown168;
	dword __unknown16C;
	byte __unknown170;
	dword __unknown174;
	dword __unknown178;
	dword __unknown17C;
	dword __unknown180;
	dword __unknown184;
	dword __unknown188;
	dword __unknown18C;
	dword __unknown190;
	dword __unknown194;
	dword __unknown198;
	dword __unknown19C;
	dword __unknown1A0;
	dword __unknown1A4;
	dword __unknown1A8;
	dword __unknown1AC;
	dword __unknown1B0;
	dword __unknown1B4;
	dword __unknown1B8;
	dword __unknown1BC;
	dword __unknown1C0;
	dword __unknown1C4;
	dword __unknown1C8;
	dword __unknown1CC;
	dword __unknown1D0;
	dword __unknown1D4;
	real __unknown1D8;
	dword __unknown1DC;
	real __unknown1E0;
	dword __unknown1E4;
	dword __unknown1E8;
	dword __unknown1EC;
	real __unknown1F0;
	dword __unknown1F4;
	real __unknown1F8;
	dword __unknown1FC;
	dword __unknown200;
	dword __unknown204;
	dword __unknown208;
	real __unknown20C;
	dword __unknown210;
	dword __unknown214;
	dword __unknown218;
	dword __unknown21C;
	dword __unknown220;
	real __unknown224;
	real __unknown228;
	dword __unknown22C;
	real __unknown230;
	dword __unknown234;
	dword __unknown238;
	dword __unknown23C;
	dword __unknown240;
	dword __unknown244;
	dword __unknown248;
	dword __unknown24C;
	dword __unknown250;
	dword __unknown254;
	dword __unknown258;
	real __unknown25C;
	dword __unknown260;
	dword __unknown264;
	dword __unknown268;

	bool disable_bad_client_anticheating;
	bool disable_bad_connectivity_anticheating;
	bool disable_bad_bandwidth_anticheating;

	dword __unknown270;
	dword __unknown274;
	dword __unknown278;
	dword __unknown27C;
	dword __unknown280;
	dword __unknown284;
	dword __unknown288;
	dword __unknown28C;
	dword __unknown290;
	dword __unknown294;
	dword __unknown298;
	dword __unknown29C;
	dword __unknown2A0;
	dword __unknown2A4;
	dword __unknown2A8;
	dword __unknown2AC;
	dword __unknown2B0;
	dword __unknown2B4;
	dword __unknown2B8;
	dword __unknown2BC;
	dword __unknown2C0;
	dword __unknown2C4;
	dword __unknown2C8;
	dword __unknown2CC;
	dword __unknown2D0;
	dword __unknown2D4;
	dword __unknown2D8;
	dword __unknown2DC;
	dword __unknown2E0;
	dword __unknown2E4;
	dword __unknown2E8;
	dword __unknown2EC;
	dword __unknown2F0;
	dword __unknown2F4;
	dword __unknown2F8;
	dword __unknown2FC;
	dword __unknown300;
	dword __unknown304;
	dword __unknown308;
	dword __unknown30C;
	dword __unknown310;
	dword __unknown314;
	dword __unknown318;
	dword __unknown31C;
	dword __unknown320;
	dword __unknown324;
	dword __unknown328;
	dword __unknown32C;
	dword __unknown330;
	dword __unknown334;
	dword __unknown338;
	dword __unknown33C;
	dword __unknown340;
	dword __unknown344;
	dword __unknown348;
	dword __unknown34C;
	dword __unknown350;
	dword __unknown354;
	dword __unknown358;
	dword __unknown35C;
	dword __unknown360;
	dword __unknown364;
	dword __unknown368;
	dword __unknown36C;
	real __unknown370;
	dword __unknown374;
	dword __unknown378;
	dword __unknown37C;
	dword __unknown380;
	dword __unknown384;
	dword __unknown388;
	dword __unknown38C;
	dword __unknown390;
	dword __unknown394;
	dword __unknown398;
	dword __unknown39C;
	real __unknown3A0;
	dword __unknown3A4;
	dword __unknown3A8;
	dword __unknown3AC;
	dword __unknown3B0;
	dword __unknown3B4;
	dword __unknown3B8;
	dword __unknown3BC;
	dword __unknown3C0;
	dword __unknown3C4;
	dword __unknown3C8;
	dword __unknown3CC;
	real __unknown3D0;
	dword __unknown3D4;
	dword __unknown3D8;
	dword __unknown3DC;
	dword __unknown3E0;
	dword __unknown3E4;
	dword __unknown3E8;
	dword __unknown3EC;
	dword __unknown3F0;
	dword __unknown3F4;
	dword __unknown3F8;
	dword __unknown3FC;
	real __unknown400;
	dword __unknown404;
	dword __unknown408;
	dword __unknown40C;
	dword __unknown410;
	dword __unknown414;
};
static_assert(sizeof(s_observer_configuration) == 0x418);

struct s_channel_configuration
{
	long __unknown0; // c_network_channel::check_to_send_connect_packet
	long __unknown4; // c_network_channel::check_to_send_connect_packet
	long __unknown8; // c_network_channel::check_to_send_connect_packet
	long __unknownC; // c_network_channel::check_establishment_timeout
};
static_assert(sizeof(s_channel_configuration) == 0x10);

struct s_lsp_service_configuration
{
	long lsp_port;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __time10;
};
static_assert(sizeof(s_lsp_service_configuration) == 0x14);

struct s_map_information
{
	long map_id;
	long __unknown4;
};
static_assert(sizeof(s_map_information) == 0x8);

struct s_bandwidth_configuration
{
	real __unknown0;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	real __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;
};
static_assert(sizeof(s_bandwidth_configuration) == 0x24);

struct s_network_configuration
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;

	s_bandwidth_configuration bandwidth_configuration;

	// used in `c_network_session::idle`
	dword __unknown30;
	dword __unknown34;
	dword __unknown38;
	dword __unknown3C;

	dword __unknown40;
	dword __unknown44;
	dword __unknown48;
	dword __unknown4C;
	dword __unknown50;
	dword __unknown54;
	dword __unknown58;
	dword __unknown5C;
	dword __unknown60;
	dword __unknown64;
	dword __unknown68;
	dword __unknown6C;
	dword __unknown70;
	dword __unknown74;
	dword __unknown78;
	dword __unknown7C;
	dword __unknown80;
	dword __unknown84;
	dword __unknown88;
	dword __unknown8C;
	dword __unknown90;
	dword __unknown94;
	dword __unknown98;
	dword __unknown9C;
	dword __unknownA0;
	dword __unknownA4;
	dword __unknownA8;
	dword __unknownAC;
	dword __unknownB0;
	dword __unknownB4;
	dword __unknownB8;
	dword __unknownBC;
	dword __unknownC0;
	dword __unknownC4;
	dword __unknownC8;
	dword __unknownCC;
	dword __unknownD0;
	dword __unknownD4;
	dword __unknownD8;
	dword __unknownDC;
	dword __unknownE0;
	dword __unknownE4;
	dword __unknownE8;
	dword __unknownEC;
	dword __unknownF0;
	dword __unknownF4;
	dword __unknownF8;
	dword __unknownFC;
	dword __unknown100;
	dword __unknown104;
	dword __unknown108;
	dword __unknown10C;
	dword __unknown110;
	dword __unknown114;
	dword __unknown118;
	dword __unknown11C;
	dword __unknown120;
	dword __unknown124;
	dword __unknown128;
	dword __unknown12C;
	dword __unknown130;
	dword __unknown134;
	dword __unknown138;
	dword __unknown13C;

	// used in `c_network_session::idle`
	dword __unknown140;

	dword __unknown144;

	// used in `matchmaking_calculate_best_possible_host`
	struct
	{
		dword __unknown148;
		dword __unknown14C;
		dword __unknown150;
		dword __unknown154;
		dword __unknown158;
		dword __unknown15C;
		dword __unknown160;
		dword __unknown164;
		dword __unknown168;
		dword __unknown16C;
		dword __unknown170;
		dword __unknown174;
		dword __unknown178;
		dword __unknown17C;
		dword __unknown180;
		dword __unknown184;
		dword __unknown188;
		dword __unknown18C;
	} __unknown148[3];

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown220;

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown224;

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown228;

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown22C;

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown230;

	// used in `matchmaking_calculate_best_possible_host`
	dword __unknown234;

	dword __unknown238;
	dword __unknown23C;

	// used in `c_network_observer::stream_balance_all_stream_bandwidth`
	dword minimum_bandwidth;

	// used in `network_join_update`
	dword __unknown244;

	// used in `network_join_update`
	dword disband_timeout;

	// used in `c_network_session::process_pending_joins`
	dword __unknown24C;

	dword __unknown250;
	dword __unknown254;
	dword __unknown258;

	// used in `c_network_session::process_pending_joins`
	dword __unknown25C;

	dword __unknown260;

	// used in `matchmaking_calculate_best_possible_host`
	byte __unknown264;

	// used in `initialize_fake_hopper`
	real __unknown268;

	// used in `c_life_cycle_state_handler_matchmaking_start::update`
	dword __unknown26C;

	// used in `c_life_cycle_state_handler_matchmaking_find_match::update_joining`
	dword __unknown270;

	// used in `c_life_cycle_state_handler_matchmaking_find_match::update_joining`
	dword __unknown274;

	// used in `c_life_cycle_state_handler_matchmaking_find_match::update_joining`
	// used in `c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_joining`
	dword __unknown278;

	// used in `c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_joining`
	dword __unknown27C;

	// used in `c_life_cycle_state_handler_matchmaking_find_match::update_leaving`
	// used in `c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_leaving`
	dword __unknown280;

	// used in `c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update`
	dword __unknown284;

	byte __unknown288;

	// used in `c_life_cycle_state_handler_matchmaking_prepare_map::update_veto`
	dword __unknown28C;

	// used in `c_life_cycle_state_handler_matchmaking_prepare_map::update_loading`
	dword __unknown290;

	// used in `c_life_cycle_state_handler_matchmaking_prepare_map::update_countdown`
	dword __unknown294;

	// used in `c_life_cycle_state_handler_matchmaking_prepare_map::update_countdown`
	dword __unknown298;

	// used in `c_life_cycle_state_handler_matchmaking_prepare_map::update_countdown`
	dword __unknown29C;

	// used in `c_life_cycle_state_handler_end_game_write_stats::update`
	// used in `c_life_cycle_state_handler_end_match_write_stats::update`
	dword grief_timer_timeout;

	// used in `c_life_cycle_state_handler_matchmaking_arbitration::exit`
	// used in `c_life_cycle_state_handler_matchmaking_arbitration::update`
	long arbitration_establishment_and_conectivity_giveup_timer;

	// used in `c_life_cycle_state_handler_matchmaking_arbitration::exit`
	// used in `c_life_cycle_state_handler_matchmaking_arbitration::update`
	long arbitration_completion_giveup_timer;

	// used in `c_life_cycle_state_handler_post_match::update_for_state_transition`
	// used in `c_life_cycle_state_handler_post_match::update_squad_host`
	bool should_disband_to_pre_game;

	dword __unknown2B0;
	dword __unknown2B4;
	dword simulation_abort_as_host_timeout;
	dword simulation_abort_as_peer_timeout;
	long logic_unsuitable_session_count;
	dword __unknown2C4;
	dword __unknown2C8;
	dword __unknown2CC;
	dword __unknown2D0;
	long logic_qos_attempt_count;
	dword __unknown2D8;
	dword __unknown2DC;
	dword __unknown2E0;
	dword __unknown2E4;
	dword __unknown2E8;
	dword __unknown2EC;
	dword __unknown2F0;
	dword __unknown2F4;
	dword __unknown2F8;
	dword __unknown2FC;
	real __unknown300;
	dword __unknown304;
	dword __unknown308;
	dword __unknown30C;
	dword __unknown310;
	dword __unknown314;
	dword __unknown318;
	dword __unknown31C;
	dword __unknown320;
	dword __unknown324;
	dword __unknown328;
	dword __unknown32C;
	dword __unknown330;
	dword __unknown334;
	dword __unknown338;
	dword __unknown33C;
	dword __unknown340;
	real __unknown344;
	real __unknown348;
	dword __unknown34C;
	dword __unknown350;
	dword __unknown354;
	dword __unknown358;
	dword __unknown35C;
	dword __unknown360;
	dword __unknown364;
	dword __unknown368;
	dword __unknown36C;
	dword __unknown370;
	real __unknown374;
	dword __unknown378;
	dword __unknown37C;
	dword __unknown380;
	dword __unknown384;
	dword __unknown388;
	dword __unknown38C;
	dword peer_properties_update_timeout;
	dword __unknown394;
	dword __unknown398;
	dword __unknown39C;
	dword local_specific_parameters_update_timeout;
	dword __unknown3A4;
	dword __unknown3A8;
	dword __unknown3AC;
	dword __unknown3B0;
	dword __unknown3B4;
	dword __unknown3B8;
	dword __unknown3BC;
	dword __unknown3C0;
	dword lobby_coop_max_player_count;
	dword lobby_mapeditor_max_player_count;
	dword __unknown3CC;
	dword machine_file_refresh_timeout;
	dword __unknown3D4;
	dword __unknown3D8;
	dword __unknown3DC;
	dword __unknown3E0;

	s_simulation_configuration simulation_configuration;

	real __unknownE44;
	dword __unknownE48;
	real __unknownE4C;
	real __unknownE50;
	real __unknownE54;
	real __unknownE58;
	real __unknownE5C;
	dword __unknownE60;
	dword managed_session_process_pending_timeout;
	dword __unknownE68;
	dword idle_peer_joining_timeout;
	dword peer_join_timed_out;
	dword join_queue_max_wait_time;
	dword __unknownE78;
	dword __unknownE7C;
	dword idle_peer_join_abort_timeout;
	dword __unknownE84;
	dword idle_peer_leaving_timeout;
	dword __unknownE8C;
	dword __unknownE90;
	dword __unknownE94;
	dword __unknownE98;
	dword __unknownE9C;
	dword __unknownEA0;
	dword __unknownEA4;
	dword __unknownEA8;
	dword __unknownEAC;
	dword __unknownEB0;
	dword __unknownEB4;
	dword __unknownEB8;
	dword __unknownEBC;
	dword __unknownEC0;
	dword __unknownEC4;

	s_observer_configuration observer_configuration;
	s_channel_configuration channel_configuration;

	dword __unknown12F0;
	dword __unknown12F4;
	dword __unknown12F8;
	dword __unknown12FC;
	dword __unknown1300;
	dword __unknown1304;
	dword __unknown1308;
	dword __unknown130C;
	dword __unknown1310;
	dword __unknown1314;
	dword __unknown1318;
	dword __unknown131C;
	dword __unknown1320;
	dword __unknown1324;
	dword __unknown1328;
	dword __unknown132C;
	dword __unknown1330;
	dword __unknown1334;
	dword __unknown1338;
	dword __unknown133C;
	real __unknown1340;
	dword __unknown1344;
	dword __unknown1348;
	dword __unknown134C;
	dword __unknown1350;
	dword __unknown1354;
	dword __unknown1358;
	byte __unknown135C;
	dword __unknown1360;
	dword __unknown1364;
	real __unknown1368;
	real __unknown136C;
	byte __unknown1370;
	dword __unknown1374;
	dword __unknown1378;
	dword __unknown137C;
	byte __unknown1380;
	long griefer_betrayal_timeout;
	long griefer_ejection_timeout;
	real maximum_griefer_betrayal_count;
	long maximum_griefer_eviction_count;
	dword __unknown1394;
	dword __unknown1398;
	dword __unknown139C;
	dword __unknown13A0;
	dword lobby_film_max_player_count;
	dword __unknown13A8;
	dword __unknown13AC;
	dword __unknown13B0;
	dword __unknown13B4;
	dword __unknown13B8;
	dword __unknown13BC;
	dword __unknown13C0;
	dword __unknown13C4;
	dword __unknown13C8;
	dword __unknown13CC;
	dword __unknown13D0;
	dword __unknown13D4;
	dword __unknown13D8;
	dword __unknown13DC;
	dword __unknown13E0;
	dword __unknown13E4;
	dword __unknown13E8;
	dword __unknown13EC;
	dword __unknown13F0;
	dword __unknown13F4;
	dword __unknown13F8;
	dword __unknown13FC;
	dword __unknown1400;
	dword __unknown1404;
	dword __unknown1408;
	dword __unknown140C;
	dword __unknown1410;
	dword __unknown1414;
	dword __unknown1418;
	dword __unknown141C;
	dword __unknown1420;
	dword __unknown1424;
	dword __unknown1428;
	dword __unknown142C;
	dword __unknown1430;
	dword __unknown1434;
	dword __unknown1438;
	dword __unknown143C;
	dword __unknown1440;
	dword __unknown1444;
	dword __unknown1448;
	dword __unknown144C;
	dword __unknown1450;
	dword __unknown1454;
	dword __unknown1458;
	dword __unknown145C;
	dword __unknown1460;
	dword __unknown1464;
	dword __unknown1468;
	dword __unknown146C;
	dword __unknown1470;
	dword __unknown1474;
	dword __unknown1478;
	dword __unknown147C;
	dword __unknown1480;
	dword __unknown1484;
	dword __unknown1488;
	dword __unknown148C;
	dword __unknown1490;
	dword __unknown1494;
	dword __unknown1498;
	dword __unknown149C;
	dword __unknown14A0;
	dword __unknown14A4;
	dword __unknown14A8;
	dword __unknown14AC;
	dword __unknown14B0;
	dword __unknown14B4;
	dword __unknown14B8;
	dword __unknown14BC;
	dword __unknown14C0;
	dword __unknown14C4;
	dword __unknown14C8;
	dword __unknown14CC;
	dword __unknown14D0;
	dword __unknown14D4;
	dword __unknown14D8;
	dword __unknown14DC;
	dword __unknown14E0;
	dword __unknown14E4;
	dword __unknown14E8;
	dword __unknown14EC;
	dword __unknown14F0;
	dword __unknown14F4;
	dword __unknown14F8;
	dword __unknown14FC;
	dword __unknown1500;
	dword __unknown1504;
	dword __unknown1508;
	dword __unknown150C;
	dword __unknown1510;
	dword __unknown1514;
	dword __unknown1518;
	dword __unknown151C;
	dword __unknown1520;
	dword __unknown1524;
	dword __unknown1528;
	dword __unknown152C;
	dword __unknown1530;
	dword __unknown1534;
	dword __unknown1538;
	dword __unknown153C;
	dword __unknown1540;
	dword __unknown1544;
	dword __unknown1548;
	dword __unknown154C;
	dword __unknown1550;
	dword __unknown1554;
	dword __unknown1558;
	dword __unknown155C;
	dword __unknown1560;
	dword __unknown1564;
	dword __unknown1568;
	dword __unknown156C;
	dword __unknown1570;
	dword __unknown1574;
	dword __unknown1578;
	dword __unknown157C;
	dword __unknown1580;
	dword __unknown1584;
	dword __unknown1588;
	dword __unknown158C;
	byte __unknown1590;
	bool custom_games_enabled_override;
	byte __unknown1592;
	byte __unknown1593;
	dword alpha_locked_down_state;
	dword maximum_multiplayer_split_screen;
	byte __unknown159C;
	dword __unknown15A0;
	s_lsp_service_configuration lsp_service_configuration;
	s_map_information map_infos[32];
	dword __unknown16B8;
	long determinism_version;
	long determinism_compatible_version;
};
static_assert(sizeof(s_network_configuration) == 0x16C4);

struct c_network_configuration_globals
{
	bool __unknown0;
	bool __unknown1;
	long __unknown4;
	long __unknown8;
	long __unknownC;
};
static_assert(sizeof(c_network_configuration_globals) == 0x10);

extern bool& g_network_configuration_initialized;
extern s_network_configuration& g_network_configuration;
extern c_network_configuration_globals& g_network_configuration_globals;

extern long __cdecl network_configuration_calculate_next_retry_interval(long a1, long a2);
extern bool __cdecl network_configuration_custom_games_enabled();
extern bool __cdecl network_configuration_disable_bad_bandwidth_anticheating();
extern bool __cdecl network_configuration_disable_bad_client_anticheating();
extern bool __cdecl network_configuration_disable_bad_connectivity_anticheating();
extern void __cdecl network_configuration_dispose();
extern long __cdecl network_configuration_get_map_index_from_map_id(long map_id);
extern long __cdecl network_configuration_get_state();
extern void __cdecl network_configuration_initialize(bool a1);
extern void __cdecl network_configuration_initialize_observer_constants();
extern void __cdecl network_configuration_initialize_private();
extern void __cdecl network_configuration_initialize_simulation_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_creation_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_update_constants();
extern void __cdecl network_configuration_initialize_simulation_event_constants();
extern long __cdecl network_configuration_maximum_multiplayer_split_screen();
extern void __cdecl network_configuration_update();

extern void network_build_network_configuration();

