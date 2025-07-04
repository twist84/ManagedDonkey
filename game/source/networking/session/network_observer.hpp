#pragma once

#include "cseries/cseries.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/transport/transport_qos.hpp"
#include "networking/network_statistics.hpp"

int32 const k_network_maximum_observers = 34;

class c_network_session;
class c_network_link;
class c_network_message_gateway;
class c_network_message_handler;
class c_network_message_type_collection;
struct s_observer_configuration;
struct s_network_message_connect_request;

struct s_channel_observer_owner
{
	c_network_session* owner;
	int32 managed_session_index;
};
static_assert(sizeof(s_channel_observer_owner) == 0x8);

class c_network_observer
{
public:
	struct s_stream_parameters
	{
		bool active;
		bool is_load_bearing;
		bool is_simulation;
		bool is_simulation_authority;
		bool is_simulation_client;
		bool is_synchronous;
		bool is_synchronous_joining;
		bool saturate_stream;
		int32 current_bandwidth_bps;
		real32 current_packet_rate;
		bool current_packet_rate_limiting;
		bool period_bandwidth_restricted;
		int32 period_transmission_bytes;
		int32 period_throughput_bytes;
		int32 period_minimum_average_rtt_msec;
		int32 period_maximum_average_rtt_msec;
		int32 period_minimum_lost_packet_rate;
		int32 period_maximum_lost_packet_rate;
		int32 period_start_average_rtt_msec;
		int32 mini_period_initial_rtt_msec;
		int32 mini_period_initial_lost_packet_rate;
		int32 mini_period_delivered_packet_count;
		int32 mini_period_lost_packet_count;
		int32 mini_period_maximum_average_rtt;
		int32 mini_period_maximum_average_rtt_fast_deviation;
		int32 mini_period_last_maximum_average_rtt;
		int32 mini_period_bits_transmitted_game;
		int32 mini_period_bits_transmitted_fill;
		int32 mini_period_bits_transmitted_voice;
		int32 packet_data_game_ratio;
		int32 packet_data_fill_ratio;
		int32 packet_data_voice_ratio;
		int32 average_lost_packet_rate_unshifted;
		int32 average_lost_packet_rate;
		int32 average_lost_packet_rate_deviation;
		bool recent_period_bandwidth_restricted;
		int32 recent_period_transmission_bps;
		int32 recent_period_throughput_bps;
		int32 probe_maximum_rtt_msec;
		int32 related_probe_maximum_rtt_msec;
		int32 congest_at_rtt_msec;
		int32 congest_at_lost_packet_rate;
		bool congest_at_rtt_locked;
		int32 last_known_good_lost_packet_rate;
		int32 last_known_good_lost_packet_rate_deviation;
		int32 last_known_good_lost_packet_rate_bandwidth_bps;
		bool has_completed_initial_probe;
		bool has_set_initial_bandwidth;
		int32 overprobe_bps;
		int32 allocated_bandwidth_bps;
		int32 maximum_bandwidth_bps;
		int32 maximum_bandwidth_find_duplicate_count;
		uns32 time_to_grow_bandwidth_base;
		int32 time_to_grow_bandwidth_delta;
		int32 packet_loss_spike_count;
		int32 upstream_bandwidth_failures;
		int32 downstream_bandwidth_failures;
		int32 latency_failures;
		int32 packet_loss_failures;
		int32 packet_loss_spike_failures;
		int32 consecutive_probe_failures;
		int32 consecutive_probe_successes;
		int32 consecutive_congested_bandwidth_throttles;
		int32 probe_previous_bandwidth;
		bool stream_requires_more_bandwidth;
	};
	static_assert(sizeof(s_stream_parameters) == 0xE4);

	struct s_channel_observer
	{
		c_network_channel channel;
		int32 state;
		uns32 state_entry_timestamp;
		uns8 flags;
		uns8 owner_flags;
		int16 connection_attempts;
		uns32 owner_connection_identifier;
		s_transport_secure_address remote_secure_address;
		uns32 secure_connection_discarded_owner_mask;
		int32 secure_connection_owner;
		transport_address secure_connection_address;
		uns32 last_acknowledgement_timestamp;
		int32 average_rtt_msec;
		int32 average_rtt_fast_deviation_msec;
		int32 average_rtt_deviation_msec;
		int32 average_rtt_interval_msec;
		int32 current_timeout_msec;
		bool is_bad_client;
		int32 remote_machine_upstream_bandwidth_bps;
		int32 remote_machine_downstream_bandwidth_bps;
		int32 qos_attempt_index;
		s_transport_qos_result qos_result;
		uns32 last_inactive_timestamp;
		uns32 undesired_connection_timestamp;
		uns32 undesired_connection_empty_timestamp;
		c_network_time_statistics received_bytes_statistics;
		c_network_time_statistics transmitted_bytes_statistics;
		c_network_time_statistics acknowledged_bytes_statistics;
		c_network_window_statistics packet_loss_statistics;
		c_network_window_statistics packet_warning_statistics;
		int32 bandwidth_monitor_interval_count;
		int32 bandwidth_monitor_period_count;
		int32 bandwidth_monitor_throughput_minimum_bps;
		int32 bandwidth_monitor_throughput_maximum_bps;
		int32 bandwidth_monitor_previous_maximum_bps;
		int32 bandwidth_event_counters[5];
		uns32 last_packet_slot_timestamp;
		c_network_observer::s_stream_parameters stream;
		bool badness_reported_connectivity;
		bool badness_reported_squad_host;
		bool badness_reported_squad_client;
		bool badness_reported_group_host;
		bool badness_reported_group_client;
		bool badness_was_ever_game_squad_host;
		bool badness_was_ever_game_squad_client;
		bool badness_was_ever_game_group_host;
		bool badness_was_ever_game_group_client;
		uns64 message_backlog_mask;
	};
	static_assert(sizeof(s_channel_observer) == 0x10D8);

	const s_channel_observer* find_observer_by_channel(const c_network_channel* channel) const;
	void destroy_observer();
	void handle_connect_request(const transport_address* incoming_address, const s_network_message_connect_request* connect_request);
	bool initialize_observer(c_network_link* link, c_network_message_type_collection* message_types, c_network_message_gateway* message_gateway, c_network_message_handler* message_handler, const s_observer_configuration* configuration);
	void monitor();
	int32 observer_channel_find_by_network_channel(int32 owner_type, c_network_channel* channel) const;
	void observer_channel_send_message(int32 owner_type, int32 observer_channel_index, bool out_of_band, e_network_message_type message_type, int32 message_size, const void* message_payload);
	void observer_prioritize_upload_bandwidth(bool prioritize);
	void quality_statistics_get_ratings(int32* out_connectivity_badness_rating, int32* out_host_badness_rating, int32* out_client_badness_rating);
	void set_online_network_environment(bool online_network_environment);

	c_network_link* m_link;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	c_network_message_type_collection* m_message_types;
	s_observer_configuration* m_configuration;
	s_channel_observer_owner m_owners[4];
	c_network_session* m_session;
	s_channel_observer m_channel_observers[k_network_maximum_observers];
	bool m_quality_statistics_are_set;
	s_network_quality_statistics m_quality_statistics;
	int32 m_expansion_state;
	int32 m_probe_state;
	bool m_prioritize_upload_bandwidth;
	bool m_online_network_environment;
	bool m_bandwidth_estimate_reliable;
	bool m_allow_bandwidth_estimate_override;
	int32 m_bandwidth_estimated_bps;
	int32 m_bandwidth_maximum_throughput_bps;
	int32 m_bandwidth_congestion_bps;
	uns32 m_bandwidth_satiation_timestamp;
	bool m_bandwidth_satiation_setting;
	int32 m_bandwidth_satiation_count[2];
	int32 m_maximum_upstream_bandwidth_bps;
	bool m_has_simulation_stream;
	uns32 m_recently_probed_streams;
	uns32 m_time_of_expansion;
	uns32 m_time_of_probe;
	uns32 m_time_of_probe_delay;
	uns32 m_time_of_end_of_last_probe;
	int32 m_time_to_delay_next_probe;
	int32 m_bandwidth_known_good_kbps;
	int32 m_bandwidth_known_bad_kbps;
	int32 m_bandwidth_target_kbps;
	c_network_time_statistics m_bandwidth_statistics;
	int32 m_allocated_observer_count;
	int32 m_allocated_bandwidth_bps;
	real32 m_allocated_rate_authority;
	real32 m_allocated_rate_client;
	int32 m_allocated_upstream_bandwidth;
	int32 m_allocated_upload_bandwidth;
	uns32 m_stream_period_start_timestamp;
	uns32 m_stream_mini_period_start_timestamp;
	bool m_stream_needs_rebalancing;
	bool m_stream_abort_growth;
	bool m_stream_period_restart_flag;
	bool m_stream_period_restart_abort_expansion_flag;
	bool m_stream_period_restart_record_congestion_flag;
	int32 m_stream_period_growth_initiated_count;
	int32 m_stream_period_growth_maximum_count;
	uns32 m_stream_reset_timestamp;
	bool m_stream_all_streams_are_stable;
	uns32 m_streams_that_are_still_growing;
};
static_assert(sizeof(c_network_observer) == 0x23F20);
static_assert(0x00014 == OFFSETOF(c_network_observer, m_owners));
static_assert(0x00034 == OFFSETOF(c_network_observer, m_session));
static_assert(0x00038 == OFFSETOF(c_network_observer, m_channel_observers));
static_assert(0x23CE8 == OFFSETOF(c_network_observer, m_quality_statistics_are_set));
static_assert(0x23CF0 == OFFSETOF(c_network_observer, m_quality_statistics));
static_assert(0x23DB0 == OFFSETOF(c_network_observer, m_expansion_state));
static_assert(0x23DB4 == OFFSETOF(c_network_observer, m_probe_state));
static_assert(0x23DB8 == OFFSETOF(c_network_observer, m_prioritize_upload_bandwidth));
static_assert(0x23DB9 == OFFSETOF(c_network_observer, m_online_network_environment));
static_assert(0x23DBA == OFFSETOF(c_network_observer, m_bandwidth_estimate_reliable));
static_assert(0x23DBB == OFFSETOF(c_network_observer, m_allow_bandwidth_estimate_override));
static_assert(0x23DBC == OFFSETOF(c_network_observer, m_bandwidth_estimated_bps));
static_assert(0x23DC0 == OFFSETOF(c_network_observer, m_bandwidth_maximum_throughput_bps));
static_assert(0x23DC4 == OFFSETOF(c_network_observer, m_bandwidth_congestion_bps));
static_assert(0x23DC8 == OFFSETOF(c_network_observer, m_bandwidth_satiation_timestamp));
static_assert(0x23DCC == OFFSETOF(c_network_observer, m_bandwidth_satiation_setting));
static_assert(0x23DD0 == OFFSETOF(c_network_observer, m_bandwidth_satiation_count));
static_assert(0x23DD8 == OFFSETOF(c_network_observer, m_maximum_upstream_bandwidth_bps));
static_assert(0x23DDC == OFFSETOF(c_network_observer, m_has_simulation_stream));
static_assert(0x23DE0 == OFFSETOF(c_network_observer, m_recently_probed_streams));
static_assert(0x23DE4 == OFFSETOF(c_network_observer, m_time_of_expansion));
static_assert(0x23DE8 == OFFSETOF(c_network_observer, m_time_of_probe));
static_assert(0x23DEC == OFFSETOF(c_network_observer, m_time_of_probe_delay));
static_assert(0x23DF0 == OFFSETOF(c_network_observer, m_time_of_end_of_last_probe));
static_assert(0x23DF4 == OFFSETOF(c_network_observer, m_time_to_delay_next_probe));
static_assert(0x23DF8 == OFFSETOF(c_network_observer, m_bandwidth_known_good_kbps));
static_assert(0x23DFC == OFFSETOF(c_network_observer, m_bandwidth_known_bad_kbps));
static_assert(0x23E00 == OFFSETOF(c_network_observer, m_bandwidth_target_kbps));
static_assert(0x23E08 == OFFSETOF(c_network_observer, m_bandwidth_statistics));
static_assert(0x23EE0 == OFFSETOF(c_network_observer, m_allocated_observer_count));
static_assert(0x23EE4 == OFFSETOF(c_network_observer, m_allocated_bandwidth_bps));
static_assert(0x23EE8 == OFFSETOF(c_network_observer, m_allocated_rate_authority));
static_assert(0x23EEC == OFFSETOF(c_network_observer, m_allocated_rate_client));
static_assert(0x23EF0 == OFFSETOF(c_network_observer, m_allocated_upstream_bandwidth));
static_assert(0x23EF4 == OFFSETOF(c_network_observer, m_allocated_upload_bandwidth));
static_assert(0x23EF8 == OFFSETOF(c_network_observer, m_stream_period_start_timestamp));
static_assert(0x23EFC == OFFSETOF(c_network_observer, m_stream_mini_period_start_timestamp));
static_assert(0x23F00 == OFFSETOF(c_network_observer, m_stream_needs_rebalancing));
static_assert(0x23F01 == OFFSETOF(c_network_observer, m_stream_abort_growth));
static_assert(0x23F02 == OFFSETOF(c_network_observer, m_stream_period_restart_flag));
static_assert(0x23F03 == OFFSETOF(c_network_observer, m_stream_period_restart_abort_expansion_flag));
static_assert(0x23F04 == OFFSETOF(c_network_observer, m_stream_period_restart_record_congestion_flag));
static_assert(0x23F08 == OFFSETOF(c_network_observer, m_stream_period_growth_initiated_count));
static_assert(0x23F0C == OFFSETOF(c_network_observer, m_stream_period_growth_maximum_count));
static_assert(0x23F10 == OFFSETOF(c_network_observer, m_stream_reset_timestamp));
static_assert(0x23F14 == OFFSETOF(c_network_observer, m_stream_all_streams_are_stable));
static_assert(0x23F18 == OFFSETOF(c_network_observer, m_streams_that_are_still_growing));

static_assert(0x0000 == OFFSETOF(c_network_observer::s_channel_observer, channel));
static_assert(0x0A74 == OFFSETOF(c_network_observer::s_channel_observer, state));
static_assert(0x0A78 == OFFSETOF(c_network_observer::s_channel_observer, state_entry_timestamp));
static_assert(0x0A7C == OFFSETOF(c_network_observer::s_channel_observer, flags));
static_assert(0x0A7D == OFFSETOF(c_network_observer::s_channel_observer, owner_flags));
static_assert(0x0A7E == OFFSETOF(c_network_observer::s_channel_observer, connection_attempts));
static_assert(0x0A80 == OFFSETOF(c_network_observer::s_channel_observer, owner_connection_identifier));
static_assert(0x0A84 == OFFSETOF(c_network_observer::s_channel_observer, remote_secure_address));
static_assert(0x0A94 == OFFSETOF(c_network_observer::s_channel_observer, secure_connection_discarded_owner_mask));
static_assert(0x0A98 == OFFSETOF(c_network_observer::s_channel_observer, secure_connection_owner));
static_assert(0x0A9C == OFFSETOF(c_network_observer::s_channel_observer, secure_connection_address));
static_assert(0x0AB0 == OFFSETOF(c_network_observer::s_channel_observer, last_acknowledgement_timestamp));
static_assert(0x0AB4 == OFFSETOF(c_network_observer::s_channel_observer, average_rtt_msec));
static_assert(0x0AB8 == OFFSETOF(c_network_observer::s_channel_observer, average_rtt_fast_deviation_msec));
static_assert(0x0ABC == OFFSETOF(c_network_observer::s_channel_observer, average_rtt_deviation_msec));
static_assert(0x0AC0 == OFFSETOF(c_network_observer::s_channel_observer, average_rtt_interval_msec));
static_assert(0x0AC4 == OFFSETOF(c_network_observer::s_channel_observer, current_timeout_msec));
static_assert(0x0AC8 == OFFSETOF(c_network_observer::s_channel_observer, is_bad_client));
static_assert(0x0ACC == OFFSETOF(c_network_observer::s_channel_observer, remote_machine_upstream_bandwidth_bps));
static_assert(0x0AD0 == OFFSETOF(c_network_observer::s_channel_observer, remote_machine_downstream_bandwidth_bps));
static_assert(0x0AD4 == OFFSETOF(c_network_observer::s_channel_observer, qos_attempt_index));
static_assert(0x0AD8 == OFFSETOF(c_network_observer::s_channel_observer, qos_result));
static_assert(0x0AF8 == OFFSETOF(c_network_observer::s_channel_observer, last_inactive_timestamp));
static_assert(0x0AFC == OFFSETOF(c_network_observer::s_channel_observer, undesired_connection_timestamp));
static_assert(0x0B00 == OFFSETOF(c_network_observer::s_channel_observer, undesired_connection_empty_timestamp));
static_assert(0x0B08 == OFFSETOF(c_network_observer::s_channel_observer, received_bytes_statistics));
static_assert(0x0BE0 == OFFSETOF(c_network_observer::s_channel_observer, transmitted_bytes_statistics));
static_assert(0x0CB8 == OFFSETOF(c_network_observer::s_channel_observer, acknowledged_bytes_statistics));
static_assert(0x0D90 == OFFSETOF(c_network_observer::s_channel_observer, packet_loss_statistics));
static_assert(0x0EA0 == OFFSETOF(c_network_observer::s_channel_observer, packet_warning_statistics));
static_assert(0x0FB0 == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_monitor_interval_count));
static_assert(0x0FB4 == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_monitor_period_count));
static_assert(0x0FB8 == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_monitor_throughput_minimum_bps));
static_assert(0x0FBC == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_monitor_throughput_maximum_bps));
static_assert(0x0FC0 == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_monitor_previous_maximum_bps));
static_assert(0x0FC4 == OFFSETOF(c_network_observer::s_channel_observer, bandwidth_event_counters));
static_assert(0x0FD8 == OFFSETOF(c_network_observer::s_channel_observer, last_packet_slot_timestamp));
static_assert(0x0FDC == OFFSETOF(c_network_observer::s_channel_observer, stream));
static_assert(0x10C0 == OFFSETOF(c_network_observer::s_channel_observer, badness_reported_connectivity));
static_assert(0x10C1 == OFFSETOF(c_network_observer::s_channel_observer, badness_reported_squad_host));
static_assert(0x10C2 == OFFSETOF(c_network_observer::s_channel_observer, badness_reported_squad_client));
static_assert(0x10C3 == OFFSETOF(c_network_observer::s_channel_observer, badness_reported_group_host));
static_assert(0x10C4 == OFFSETOF(c_network_observer::s_channel_observer, badness_reported_group_client));
static_assert(0x10C5 == OFFSETOF(c_network_observer::s_channel_observer, badness_was_ever_game_squad_host));
static_assert(0x10C6 == OFFSETOF(c_network_observer::s_channel_observer, badness_was_ever_game_squad_client));
static_assert(0x10C7 == OFFSETOF(c_network_observer::s_channel_observer, badness_was_ever_game_group_host));
static_assert(0x10C8 == OFFSETOF(c_network_observer::s_channel_observer, badness_was_ever_game_group_client));
static_assert(0x10D0 == OFFSETOF(c_network_observer::s_channel_observer, message_backlog_mask));

static_assert(0x00 == OFFSETOF(c_network_observer::s_stream_parameters, active));
static_assert(0x01 == OFFSETOF(c_network_observer::s_stream_parameters, is_load_bearing));
static_assert(0x02 == OFFSETOF(c_network_observer::s_stream_parameters, is_simulation));
static_assert(0x03 == OFFSETOF(c_network_observer::s_stream_parameters, is_simulation_authority));
static_assert(0x04 == OFFSETOF(c_network_observer::s_stream_parameters, is_simulation_client));
static_assert(0x05 == OFFSETOF(c_network_observer::s_stream_parameters, is_synchronous));
static_assert(0x06 == OFFSETOF(c_network_observer::s_stream_parameters, is_synchronous_joining));
static_assert(0x07 == OFFSETOF(c_network_observer::s_stream_parameters, saturate_stream));
static_assert(0x08 == OFFSETOF(c_network_observer::s_stream_parameters, current_bandwidth_bps));
static_assert(0x0C == OFFSETOF(c_network_observer::s_stream_parameters, current_packet_rate));
static_assert(0x10 == OFFSETOF(c_network_observer::s_stream_parameters, current_packet_rate_limiting));
static_assert(0x11 == OFFSETOF(c_network_observer::s_stream_parameters, period_bandwidth_restricted));
static_assert(0x14 == OFFSETOF(c_network_observer::s_stream_parameters, period_transmission_bytes));
static_assert(0x18 == OFFSETOF(c_network_observer::s_stream_parameters, period_throughput_bytes));
static_assert(0x1C == OFFSETOF(c_network_observer::s_stream_parameters, period_minimum_average_rtt_msec));
static_assert(0x20 == OFFSETOF(c_network_observer::s_stream_parameters, period_maximum_average_rtt_msec));
static_assert(0x24 == OFFSETOF(c_network_observer::s_stream_parameters, period_minimum_lost_packet_rate));
static_assert(0x28 == OFFSETOF(c_network_observer::s_stream_parameters, period_maximum_lost_packet_rate));
static_assert(0x2C == OFFSETOF(c_network_observer::s_stream_parameters, period_start_average_rtt_msec));
static_assert(0x30 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_initial_rtt_msec));
static_assert(0x34 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_initial_lost_packet_rate));
static_assert(0x38 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_delivered_packet_count));
static_assert(0x3C == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_lost_packet_count));
static_assert(0x40 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_maximum_average_rtt));
static_assert(0x44 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_maximum_average_rtt_fast_deviation));
static_assert(0x48 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_last_maximum_average_rtt));
static_assert(0x4C == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_bits_transmitted_game));
static_assert(0x50 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_bits_transmitted_fill));
static_assert(0x54 == OFFSETOF(c_network_observer::s_stream_parameters, mini_period_bits_transmitted_voice));
static_assert(0x58 == OFFSETOF(c_network_observer::s_stream_parameters, packet_data_game_ratio));
static_assert(0x5C == OFFSETOF(c_network_observer::s_stream_parameters, packet_data_fill_ratio));
static_assert(0x60 == OFFSETOF(c_network_observer::s_stream_parameters, packet_data_voice_ratio));
static_assert(0x64 == OFFSETOF(c_network_observer::s_stream_parameters, average_lost_packet_rate_unshifted));
static_assert(0x68 == OFFSETOF(c_network_observer::s_stream_parameters, average_lost_packet_rate));
static_assert(0x6C == OFFSETOF(c_network_observer::s_stream_parameters, average_lost_packet_rate_deviation));
static_assert(0x70 == OFFSETOF(c_network_observer::s_stream_parameters, recent_period_bandwidth_restricted));
static_assert(0x74 == OFFSETOF(c_network_observer::s_stream_parameters, recent_period_transmission_bps));
static_assert(0x78 == OFFSETOF(c_network_observer::s_stream_parameters, recent_period_throughput_bps));
static_assert(0x7C == OFFSETOF(c_network_observer::s_stream_parameters, probe_maximum_rtt_msec));
static_assert(0x80 == OFFSETOF(c_network_observer::s_stream_parameters, related_probe_maximum_rtt_msec));
static_assert(0x84 == OFFSETOF(c_network_observer::s_stream_parameters, congest_at_rtt_msec));
static_assert(0x88 == OFFSETOF(c_network_observer::s_stream_parameters, congest_at_lost_packet_rate));
static_assert(0x8C == OFFSETOF(c_network_observer::s_stream_parameters, congest_at_rtt_locked));
static_assert(0x90 == OFFSETOF(c_network_observer::s_stream_parameters, last_known_good_lost_packet_rate));
static_assert(0x94 == OFFSETOF(c_network_observer::s_stream_parameters, last_known_good_lost_packet_rate_deviation));
static_assert(0x98 == OFFSETOF(c_network_observer::s_stream_parameters, last_known_good_lost_packet_rate_bandwidth_bps));
static_assert(0x9C == OFFSETOF(c_network_observer::s_stream_parameters, has_completed_initial_probe));
static_assert(0x9D == OFFSETOF(c_network_observer::s_stream_parameters, has_set_initial_bandwidth));
static_assert(0xA0 == OFFSETOF(c_network_observer::s_stream_parameters, overprobe_bps));
static_assert(0xA4 == OFFSETOF(c_network_observer::s_stream_parameters, allocated_bandwidth_bps));
static_assert(0xA8 == OFFSETOF(c_network_observer::s_stream_parameters, maximum_bandwidth_bps));
static_assert(0xAC == OFFSETOF(c_network_observer::s_stream_parameters, maximum_bandwidth_find_duplicate_count));
static_assert(0xB0 == OFFSETOF(c_network_observer::s_stream_parameters, time_to_grow_bandwidth_base));
static_assert(0xB4 == OFFSETOF(c_network_observer::s_stream_parameters, time_to_grow_bandwidth_delta));
static_assert(0xB8 == OFFSETOF(c_network_observer::s_stream_parameters, packet_loss_spike_count));
static_assert(0xBC == OFFSETOF(c_network_observer::s_stream_parameters, upstream_bandwidth_failures));
static_assert(0xC0 == OFFSETOF(c_network_observer::s_stream_parameters, downstream_bandwidth_failures));
static_assert(0xC4 == OFFSETOF(c_network_observer::s_stream_parameters, latency_failures));
static_assert(0xC8 == OFFSETOF(c_network_observer::s_stream_parameters, packet_loss_failures));
static_assert(0xCC == OFFSETOF(c_network_observer::s_stream_parameters, packet_loss_spike_failures));
static_assert(0xD0 == OFFSETOF(c_network_observer::s_stream_parameters, consecutive_probe_failures));
static_assert(0xD4 == OFFSETOF(c_network_observer::s_stream_parameters, consecutive_probe_successes));
static_assert(0xD8 == OFFSETOF(c_network_observer::s_stream_parameters, consecutive_congested_bandwidth_throttles));
static_assert(0xDC == OFFSETOF(c_network_observer::s_stream_parameters, probe_previous_bandwidth));
static_assert(0xE0 == OFFSETOF(c_network_observer::s_stream_parameters, stream_requires_more_bandwidth));

