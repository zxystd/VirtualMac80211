//
//  apple80211_ioctl.h
//  VirtualMac80211
//
//  Created by qcwap on 2020/6/28.
//  Copyright © 2020 zxystd. All rights reserved.
//

#ifndef apple80211_ioctl_h
#define apple80211_ioctl_h

#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include "apple80211_var.h"

#define IOCTL_GET 3224398281LL
#define IOCTL_SET 2150656456LL

#define IFNAMSIZ 16

struct apple80211req
{
    char        req_if_name[IFNAMSIZ];
    uint32_t            req_type;
    uint32_t            req_val;
    uint64_t    req_len;
    void       *req_data;
    uint32_t req_flag;
};

#define APPLE80211_VERSION    1

struct apple80211_ssid_data
{
    u_int32_t    version;
    u_int32_t    ssid_len;
    u_int8_t    ssid_bytes[APPLE80211_MAX_SSID_LEN];
} __packed;

struct apple80211_channel_data
{
    u_int32_t                    version;
    struct apple80211_channel    channel;
} __packed;

struct apple80211_bssid_data
{
    u_int32_t            version;
    struct ether_addr    bssid;
} __packed;

struct apple80211_cipher_key
{
    u_int32_t version;
    uint8_t type;
    uint8_t flags;
} __packed;

struct apple80211_capability_data
{
    u_int32_t    version;
    u_int8_t    capabilities[7];
} __packed;

struct apple80211_state_data
{
    u_int32_t    version;
    u_int32_t    state;
} __packed;

struct apple80211_rssi_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    rssi_unit;
    int32_t        rssi[APPLE80211_MAX_RADIO];        // control channel
    int32_t        aggregate_rssi;                    // aggregate control channel rssi
    int32_t        rssi_ext[APPLE80211_MAX_RADIO];    // extension channel rssi
    int32_t        aggregate_rssi_ext;                // aggregate extension channel rssi
} __packed;

struct apple80211_power_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    power_state[APPLE80211_MAX_RADIO];
} __packed;

struct apple80211_assoc_result_data
{
    u_int32_t    version;
    u_int32_t    result;
} __packed;

struct apple80211_assoc_status_data
{
    u_int32_t    version;
    u_int32_t    status;
} __packed;

struct apple80211_rate_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    rate[APPLE80211_MAX_RADIO];
} __packed;

struct apple80211_status_dev_data
{
    u_int32_t    version;
    u_int8_t    dev_name[MAXPATHLEN];
} __packed;

struct apple80211_powersave_data
{
    u_int32_t    version;
    u_int32_t    powersave_level;
} __packed;

struct apple80211_protmode_data
{
    u_int32_t    version;
    u_int32_t    protmode;
    u_int32_t    threshold;        // bytes
} __packed;

struct apple80211_txpower_data
{
    u_int32_t    version;
    u_int32_t    txpower_unit;
    int32_t        txpower;
} __packed;

struct apple80211_phymode_data
{
    u_int32_t    version;
    u_int32_t    phy_mode;            // vector of supported phy modes
    u_int32_t    active_phy_mode;    // current active phy mode
} __packed;

struct apple80211_opmode_data
{
    u_int32_t    version;
    u_int32_t    op_mode;
} __packed;

struct apple80211_noise_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    noise_unit;
    int32_t        noise[APPLE80211_MAX_RADIO];        // control channel
    int32_t        aggregate_noise;                    // aggregate control channel noise
    int32_t        noise_ext[APPLE80211_MAX_RADIO];    // extension channel noise
    int32_t        aggregate_noise_ext;                // aggregate extension channel noise
} __packed;

struct apple80211_intmit_data
{
    u_int32_t    version;
    u_int32_t    int_mit;
} __packed;

struct apple80211_authtype_data
{
    u_int32_t    version;
    u_int32_t    authtype_lower;    //    apple80211_authtype_lower
    u_int32_t    authtype_upper;    //    apple80211_authtype_upper
} __packed;

struct apple80211_sup_channel_data
{
    u_int32_t                    version;
    u_int32_t                    num_channels;
    struct apple80211_channel    supported_channels[APPLE80211_MAX_CHANNELS];
} __packed;


struct apple80211_roam_threshold_data
{
    u_int32_t threshold;
    u_int32_t count;
} __packed;

struct apple80211_locale_data
{
    u_int32_t    version;
    u_int32_t    locale;
} __packed;

struct apple80211_scan_data
{
    u_int32_t                    version;
    u_int32_t                    bss_type;                            // apple80211_apmode
    struct ether_addr            bssid;                                // target BSSID
    u_int32_t                    ssid_len;                            // length of the SSID
    u_int8_t                    ssid[APPLE80211_MAX_SSID_LEN];
    u_int32_t                    scan_type;                            // apple80211_scan_type
    u_int32_t                    phy_mode;                            // apple80211_phymode vector
    u_int16_t                    dwell_time;                            // time to spend on each channel (ms)
    u_int32_t                    rest_time;                            // time between scanning each channel (ms)
    u_int32_t                    num_channels;                        // 0 if not passing in channels
    struct apple80211_channel    channels[APPLE80211_MAX_CHANNELS];    // channel list
} __packed;

struct apple80211_scan_multiple_data
{
    uint32_t version;
    uint32_t three;
    uint32_t ssid_count;
    apple80211_ssid_data ssids[16];
    uint32_t bssid_count;
    ether_addr bssids[16];
    uint32_t scan_type;
    uint32_t phy_mode;
    uint32_t dwell_time;
    uint32_t rest_time;
    uint32_t num_channels;
    struct apple80211_channel channels[128];
    uint16_t unk_2;
} __packed;

struct apple80211_apmode_data
{
    u_int32_t    version;
    u_int32_t    apmode;
} __packed;

struct apple80211_assoc_data
{
    u_int32_t                version;
    u_int16_t                ad_mode;        // apple80211_apmode
    u_int16_t                ad_auth_lower;    // apple80211_authtype_lower
    u_int16_t                ad_auth_upper;    // apple80211_authtype_upper
    u_int32_t                ad_ssid_len;
    u_int8_t                ad_ssid[ APPLE80211_MAX_SSID_LEN ];
    struct ether_addr        ad_bssid;        // prefer over ssid if not zeroed
    struct apple80211_key    ad_key;
    u_int16_t                ad_rsn_ie_len;
    u_int8_t                ad_rsn_ie[ APPLE80211_MAX_RSN_IE_LEN ];
    u_int32_t                ad_flags;        // apple80211_assoc_flags
} __packed;

struct apple80211_deauth_data
{
    u_int32_t            version;
    u_int32_t            deauth_reason;    // reason code
    struct ether_addr    deauth_ea;        // BSSID of AP
} __packed;

struct apple80211_countermeasures_data
{
    u_int32_t    version;
    u_int32_t    enabled;
} __packed;

struct apple80211_frag_threshold_data
{
    u_int32_t    version;
    u_int32_t    threshold;    // bytes
} __packed;

struct apple80211_rate_set_data
{
    u_int32_t                version;
    u_int16_t                num_rates;
    struct apple80211_rate    rates[APPLE80211_MAX_RATES];
} __packed;

struct apple80211_short_slot_data
{
    u_int32_t    version;
    u_int8_t    mode;
} __packed;

struct apple80211_retry_limit_data
{
    u_int32_t    version;
    u_int32_t    limit;
} __packed;

struct apple80211_antenna_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    int32_t        antenna_index[APPLE80211_MAX_RADIO];
} __packed;

struct apple80211_dtim_int_data
{
    u_int32_t    version;
    u_int32_t    interval;
} __packed;

struct apple80211_sta_data
{
    u_int32_t                    version;
    u_int32_t                    num_stations;
    struct apple80211_station    station_list[APPLE80211_MAX_STATIONS];
} __packed;

struct apple80211_version_data
{
    u_int32_t    version;
    u_int16_t    string_len;
    char        string[APPLE80211_MAX_VERSION_LEN];
} __packed;

struct apple80211_rom_data
{
    u_int32_t    version;
    u_int32_t    rom_len;
    u_int8_t    rom[1];    // variable length
} __packed;

struct apple80211_rand_data
{
    u_int32_t    version;
    u_int32_t    rand;
} __packed;

struct apple80211_rsn_ie_data
{
    u_int32_t    version;
    u_int16_t    len;
    u_int8_t    ie[ APPLE80211_MAX_RSN_IE_LEN ];
} __packed;

struct apple80211_ap_ie_data
{
    u_int32_t    version;
    u_int32_t    len;
    u_int8_t    *ie_data;
} __packed;

struct apple80211_stats_data
{
    u_int32_t    version;
    u_int32_t    tx_frame_count;
    u_int32_t    tx_errors;
    u_int32_t    rx_frame_count;
    u_int32_t    rx_errors;
} __packed;

struct apple80211_country_code_data
{
    u_int32_t    version;
    u_int8_t    cc[APPLE80211_MAX_CC_LEN];
} __packed;

struct apple80211_last_rx_pkt_data
{
    u_int32_t    version;
    u_int32_t    rate;
    int32_t        rssi;
    u_int32_t    num_streams;    // number of spatial streams
    struct ether_addr sa;        // source address
} __packed;

struct apple80211_radio_info_data
{
    u_int32_t    version;
    u_int32_t    count;        // number of rf chains
} __packed;

struct apple80211_guard_interval_data
{
    u_int32_t    version;
    u_int32_t    interval;    // apple80211_guard_interval
} __packed;

struct apple80211_mcs_data
{
    u_int32_t    version;
    u_int32_t    index;        // 0 to APPLE80211_MAX_MCS_INDEX
} __packed;

struct apple80211_rifs_data
{
    u_int32_t    version;
    u_int32_t    enabled;
} __packed;

struct apple80211_ldpc_data
{
    u_int32_t    version;
    u_int32_t    enabled;
} __packed;

struct apple80211_msdu_data
{
    u_int32_t    version;
    u_int32_t    max_length;        // 3839 or 7935 bytes
} __packed;

struct apple80211_mpdu_data
{
    u_int32_t    version;
    u_int32_t    max_factor;        // 0 - APPLE80211_MAX_MPDU_FACTOR
    u_int32_t    max_density;    // 0 - APPLE80211_MAX_MPDU_DENSITY
} __packed;

struct apple80211_block_ack_data
{
    u_int32_t    version;
    u_int8_t    ba_enabled;                // block ack enabled
    u_int8_t    immediate_ba_enabled;    // immediate block ack enabled
    u_int8_t    cbba_enabled;            // compressed bitmap block ack enabled
    u_int8_t    implicit_ba_enabled;    // implicit block ack enabled
} __packed;

struct apple80211_pls_data
{
    u_int32_t    version;
    u_int32_t    enabled;    // phy level spoofing enabled
} __packed;

struct apple80211_psmp_data
{
    u_int32_t    version;
    u_int32_t    enabled;
} __packed;

struct apple80211_physubmode_data
{
    u_int32_t    version;
    u_int32_t    phy_mode;        // one apple80211_phymode
    u_int32_t    phy_submode;    // one apple80211_physubmode
    u_int32_t    flags;            // apple80211_channel_flag vector
} __packed;

struct apple80211_mcs_index_set_data
{
    u_int32_t    version;
    u_int8_t    mcs_set_map[APPLE80211_MAP_SIZE( APPLE80211_MAX_MCS_INDEX + 1 )];
} __packed;

struct apple80211_wow_parameter_data
{
    u_int32_t    version;
    u_int8_t    wake_cond_map[APPLE80211_MAP_SIZE( APPLE80211_MAX_WAKE_COND + 1 )];
    u_int32_t    beacon_loss_time;
    u_int32_t    pattern_count;
    struct apple80211_wow_pattern patterns[APPLE80211_MAX_WOW_PATTERNS];
} __packed;

struct apple80211_40mhz_intolerant_data
{
    u_int32_t    version;
    u_int32_t    enabled;    // bit enabled or not
} __packed;

#endif /* apple80211_ioctl_h */
