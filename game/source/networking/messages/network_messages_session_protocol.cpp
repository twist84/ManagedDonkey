#include "networking/messages/network_messages_session_protocol.hpp"

char const* k_join_refuse_reason_strings[k_network_join_refuse_reason_count]
{
	"no-reason-given",
	"tried-to-join-self",
	"could-not-connect",
	"join-timed-out",
	"not-found",
	"privacy-mode",
	"not-joinable",
	"session-full",
	"alpha-split-screen",
	"session-disbanded",
	"session-booted",
	"address-invalid",
	"address-failed",
	"too-many-observers",
	"aborted",
	"abort-ignored",
	"wrong-payload-type",
	"no-reservation",
	"in-matchmaking",
	"player-count-zero",
	"player-not-online-enabled",
	"player-add-pending",
	"player-add-failed",
	"host-time-out",
	"rejected-by-host",
	"peer-version-too-low",
	"host-version-too-low",
	"holding-in-queue",
	"film-in-progress",
	"campaign-in-progress",
	"user-content-not-allowed",
	"survival-in-progress",
	"executable-type-mismatch",
};

char const* network_message_join_refuse_get_reason_string(long reason)
{
	if (VALID_COUNT(reason, k_network_join_refuse_reason_count))
		return k_join_refuse_reason_strings[reason];

    return "<unknown>";
}

