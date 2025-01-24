

#ifndef __MAC_DEVICE_H__
#define __MAC_DEVICE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "hal_ext_if.h"
#include "mac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_DEVICE_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define MAC_NET_DEVICE_NAME_LENGTH          16

#define MAC_DATARATES_PHY_80211G_NUM        12

#define DMAC_BA_LUT_IDX_BMAP_LEN            ((HAL_MAX_BA_LUT_SIZE + 7) >> 3)
#define DMAC_TX_BA_LUT_BMAP_LEN             ((HAL_MAX_AMPDU_LUT_SIZE + 7) >> 3)

/* ???????????????? */
#define MAC_EXCEPTION_TIME_OUT              10000

/* DMAC SCANNER ???????? */
#define MAC_SCAN_FUNC_MEAS           0x1
#define MAC_SCAN_FUNC_STATS          0x2
#define MAC_SCAN_FUNC_RADAR          0x4
#define MAC_SCAN_FUNC_BSS            0x8
#define MAC_SCAN_FUNC_P2P_LISTEN     0x10
#define MAC_SCAN_FUNC_ALL            (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS | MAC_SCAN_FUNC_RADAR | MAC_SCAN_FUNC_BSS)

#define MAC_ERR_LOG(_uc_vap_id, _puc_string)
#define MAC_ERR_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define MAC_ERR_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define MAC_ERR_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define MAC_ERR_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define MAC_ERR_VAR(_uc_vap_id, _c_fmt, ...)

#define MAC_WARNING_LOG(_uc_vap_id, _puc_string)
#define MAC_WARNING_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define MAC_WARNING_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define MAC_WARNING_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define MAC_WARNING_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define MAC_WARNING_VAR(_uc_vap_id, _c_fmt, ...)

#define MAC_INFO_LOG(_uc_vap_id, _puc_string)
#define MAC_INFO_LOG1(_uc_vap_id, _puc_string, _l_para1)
#define MAC_INFO_LOG2(_uc_vap_id, _puc_string, _l_para1, _l_para2)
#define MAC_INFO_LOG3(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3)
#define MAC_INFO_LOG4(_uc_vap_id, _puc_string, _l_para1, _l_para2, _l_para3, _l_para4)
#define MAC_INFO_VAR(_uc_vap_id, _c_fmt, ...)

#ifdef _PRE_WLAN_DFT_EVENT
#define MAC_EVENT_STATE_CHANGE(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string)    oam_event_report(_puc_macaddr, _uc_vap_id, OAM_MODULE_ID_MAC, en_event_type, _puc_string)
#else
#define MAC_EVENT_STATE_CHANGE(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string)    ((void)_puc_string)
#endif

/* ???????????????????????? */
#define MAC_DEV_ALG_PRIV(_pst_dev)                  ((_pst_dev)->p_alg_priv)

/*????????*/
#define MAC_DEV_RESET_IN_PROGRESS(_pst_device,uc_value)    ((_pst_device)->uc_device_reset_in_progress = uc_value)
#define MAC_DEV_IS_RESET_IN_PROGRESS(_pst_device)          ((_pst_device)->uc_device_reset_in_progress)

#define MAC_DFS_RADAR_WAIT_TIME_MS    60000

#define MAC_DEV_MAX_40M_INTOL_USER_BITMAP_LEN 4

#ifdef _PRE_WLAN_FEATURE_HILINK
#define HMAC_FBT_MAX_USER_NUM   32

#define FBT_DEFAULT_SCAN_CHANNEL            (0)     /* ????????FBT scan channel??home????*/
#define FBT_DEFAULT_SCAN_INTERVAL           (200)   /* ????????FBT scan interval 200ms */
#define FBT_DEFAULT_SCAN_REPORT_PERIOD      (1000)  /* ??????????????????????????????1000ms -- ??????????200ms */
#endif

#define MAC_SCAN_CHANNEL_INTERVAL_DEFAULT               6           /* ????6???????????????????????????????? */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_DEFAULT           100         /* ?????????????????????????????????? */
#define MAC_SCAN_CHANNEL_INTERVAL_PERFORMANCE           2           /* ????2???????????????????????????????? */
#define MAC_WORK_TIME_ON_HOME_CHANNEL_PERFORMANCE       60          /* WLAN????????P2P???????????????????????????? */

#define MAC_FCS_DBAC_IGNORE           0   /* ????DBAC???? */
#define MAC_FCS_DBAC_NEED_CLOSE       1   /* DBAC???????? */
#define MAC_FCS_DBAC_NEED_OPEN        2   /* DBAC???????? */


#ifdef _PRE_WLAN_FEATURE_IP_FILTER
#define MAC_MAX_IP_FILTER_BTABLE_SIZE 512 /* rx ip?????????????????????????? */
#endif //_PRE_WLAN_FEATURE_IP_FILTER

/*****************************************************************************
  3 ????????
*****************************************************************************/
/* SDT???????????? */
typedef enum
{
    MAC_SDT_MODE_WRITE = 0,
    MAC_SDT_MODE_READ,

    MAC_SDT_MODE_BUTT
}mac_sdt_rw_mode_enum;
typedef oal_uint8 mac_sdt_rw_mode_enum_uint8;

typedef enum
{
    MAC_CH_TYPE_NONE      = 0,
    MAC_CH_TYPE_PRIMARY   = 1,
    MAC_CH_TYPE_SECONDARY = 2,
}mac_ch_type_enum;
typedef oal_uint8 mac_ch_type_enum_uint8;

typedef enum
{
    MAC_SCAN_OP_INIT_SCAN,
    MAC_SCAN_OP_FG_SCAN_ONLY,
    MAC_SCAN_OP_BG_SCAN_ONLY,

    MAC_SCAN_OP_BUTT
}mac_scan_op_enum;
typedef oal_uint8 mac_scan_op_enum_uint8;

typedef enum
{
    MAC_CHAN_NOT_SUPPORT = 0,        /* ?????????????????? */
    MAC_CHAN_AVAILABLE_ALWAYS,       /* ???????????????? */
    MAC_CHAN_AVAILABLE_TO_OPERATE,   /* ????????(CAC, etc...)?????????????????? */
    MAC_CHAN_DFS_REQUIRED,           /* ?????????????????????? */
    MAC_CHAN_BLOCK_DUE_TO_RADAR,     /* ?????????????????????????????????? */

    MAC_CHAN_STATUS_BUTT
}mac_chan_status_enum;
typedef oal_uint8 mac_chan_status_enum_uint8;

#ifdef _PRE_WLAN_DFT_STAT
typedef enum
{
    MAC_DEV_MGMT_STAT_TYPE_TX = 0,
    MAC_DEV_MGMT_STAT_TYPE_RX,
    MAC_DEV_MGMT_STAT_TYPE_TX_COMPLETE,

    MAC_DEV_MGMT_STAT_TYPE_BUTT
}mac_dev_mgmt_stat_type_enum;
typedef oal_uint8 mac_dev_mgmt_stat_type_enum_uint8;
#endif

/* device reset?????????????? */
typedef enum
{
    MAC_RESET_SWITCH_SET_TYPE,
    MAC_RESET_SWITCH_GET_TYPE,
    MAC_RESET_STATUS_GET_TYPE,
    MAC_RESET_STATUS_SET_TYPE,
    MAC_RESET_SWITCH_SYS_TYPE = MAC_RESET_SWITCH_SET_TYPE,
    MAC_RESET_STATUS_SYS_TYPE = MAC_RESET_STATUS_SET_TYPE,

    MAC_RESET_SYS_TYPE_BUTT
}mac_reset_sys_type_enum;
typedef oal_uint8 mac_reset_sys_type_enum_uint8;

typedef enum
{
    MAC_TRY_INIT_SCAN_VAP_UP,
    MAC_TRY_INIT_SCAN_SET_CHANNEL,
    MAC_TRY_INIT_SCAN_START_DBAC,
    MAC_TRY_INIT_SCAN_RESCAN,

    MAC_TRY_INIT_SCAN_BUTT
}mac_try_init_scan_type;
typedef oal_uint8 mac_try_init_scan_type_enum_uint8;

typedef enum
{
    MAC_INIT_SCAN_NOT_NEED,
    MAC_INIT_SCAN_NEED,
    MAC_INIT_SCAN_IN_SCAN,

}mac_need_init_scan_res;
typedef oal_uint8 mac_need_init_scan_res_enum_uint8;

/* ?????????????????????????????????????????????????????????????????????????????????? */
typedef enum
{
    MAC_SCAN_STATE_IDLE,
    MAC_SCAN_STATE_RUNNING,

    MAC_SCAN_STATE_BUTT
}mac_scan_state_enum;
typedef oal_uint8 mac_scan_state_enum_uint8;

#define     MAC_FCS_MAX_CHL_NUM    2
#define     MAC_FCS_TIMEOUT_JIFFY  2
#define     MAC_FCS_DEFAULT_PROTECT_TIME_OUT    5120    /* us */
#define     MAC_FCS_DEFAULT_PROTECT_TIME_OUT2   1024    /* us */
#define     MAC_FCS_DEFAULT_PROTECT_TIME_OUT3   15000   /* us */

#define     MAC_ONE_PACKET_TIME_OUT             1000
#define     MAC_ONE_PACKET_TIME_OUT3            2000
#define     MAC_FCS_CTS_MAX_DURATION            32767   /* us */

/*
 self CTS
+-------+-----------+----------------+
|frame  | duration  |      RA        |     len=10
|control|           |                |
+-------+-----------+----------------+

null data
+-------+-----------+---+---+---+--------+
|frame  | duration  |A1 |A2 |A3 |Seq Ctl | len=24
|control|           |   |   |   |        |
+-------+-----------+---+---+---+--------+

*/

typedef enum
{
    MAC_FCS_NOTIFY_TYPE_SWITCH_AWAY    = 0,
    MAC_FCS_NOTIFY_TYPE_SWITCH_BACK,

    MAC_FCS_NOTIFY_TYPE_BUTT
}mac_fcs_notify_type_enum;
typedef oal_uint8 mac_fcs_notify_type_enum_uint8;

typedef struct
{
    mac_channel_stru                st_dst_chl;
    mac_channel_stru                st_src_chl;
    hal_one_packet_cfg_stru         st_one_packet_cfg;
    oal_uint8                       uc_src_fake_q_id;   /* ??????????????????id ???????? 0 1 */
    oal_uint8                       uc_dst_fake_q_id;   /* ????????????????????id ???????? 0 1 */
    oal_uint16                      us_hot_cnt;

    mac_channel_stru                st_src_chl2;
    hal_one_packet_cfg_stru         st_one_packet_cfg2;
}mac_fcs_cfg_stru;

typedef enum
{
    MAC_FCS_HOOK_ID_DBAC,
    MAC_FCS_HOOK_ID_ACS,

    MAC_FCS_HOOK_ID_BUTT
}mac_fcs_hook_id_enum;
typedef oal_uint8   mac_fcs_hook_id_enum_uint8;

typedef struct
{
    mac_fcs_notify_type_enum_uint8  uc_notify_type;
    oal_uint8                       uc_chip_id;
    oal_uint8                       uc_device_id;
    oal_uint8                       uc_resv[1];
    mac_fcs_cfg_stru                st_fcs_cfg;
}mac_fcs_event_stru;

typedef void (* mac_fcs_notify_func)(const mac_fcs_event_stru*);

typedef struct
{
    mac_fcs_notify_func    p_func;
}mac_fcs_notify_node_stru;

typedef struct
{
    mac_fcs_notify_node_stru   ast_notify_nodes[MAC_FCS_HOOK_ID_BUTT];
}mac_fcs_notify_chain_stru;

typedef enum
{
    MAC_FCS_STATE_STANDBY        = 0,  // free to use
    MAC_FCS_STATE_REQUESTED,           // requested by other module, but not in switching
    MAC_FCS_STATE_IN_PROGESS,          // in switching

    MAC_FCS_STATE_BUTT
}mac_fcs_state_enum;
typedef oal_uint8 mac_fcs_state_enum_uint8;

typedef enum
{
    MAC_FCS_SUCCESS = 0,
    MAC_FCS_ERR_NULL_PTR,
    MAC_FCS_ERR_INVALID_CFG,
    MAC_FCS_ERR_BUSY,
    MAC_FCS_ERR_UNKNOWN_ERR,
}mac_fcs_err_enum;
typedef oal_uint8   mac_fcs_err_enum_uint8;

typedef struct
{
    oal_uint32  ul_offset_addr;
    oal_uint32  ul_value[MAC_FCS_MAX_CHL_NUM];
}mac_fcs_reg_record_stru;

typedef struct tag_mac_fcs_mgr_stru
{
    volatile oal_bool_enum_uint8    en_fcs_done;
    oal_uint8                       uc_chip_id;
    oal_uint8                       uc_device_id;
    oal_uint8                       uc_fcs_cnt;
    oal_spin_lock_stru              st_lock;
    mac_fcs_state_enum_uint8        en_fcs_state;
    hal_fcs_service_type_enum_uint8 en_fcs_service_type;
    oal_uint8                       uc_resv[2];
    mac_fcs_cfg_stru               *pst_fcs_cfg;
    mac_fcs_notify_chain_stru       ast_notify_chain[MAC_FCS_NOTIFY_TYPE_BUTT];
}mac_fcs_mgr_stru;

#define MAC_FCS_VERIFY_MAX_ITEMS    1
typedef enum
{
    // isr
    MAC_FCS_STAGE_INTR_START,
    MAC_FCS_STAGE_INTR_POST_EVENT,
    MAC_FCS_STAGE_INTR_DONE,

    // event
    MAC_FCS_STAGE_EVENT_START,
    MAC_FCS_STAGE_PAUSE_VAP,
    MAC_FCS_STAGE_ONE_PKT_START,
    MAC_FCS_STAGE_ONE_PKT_INTR,
    MAC_FCS_STAGE_ONE_PKT_DONE,
    MAC_FCS_STAGE_RESET_HW_START,
    MAC_FCS_STAGE_RESET_HW_DONE,
    MAC_FCS_STAGE_RESUME_VAP,
    MAC_FCS_STAGE_EVENT_DONE,

    MAC_FCS_STAGE_COUNT
}mac_fcs_stage_enum;
typedef mac_fcs_stage_enum mac_fcs_stage_enum_uint8;

typedef struct
{
    oal_bool_enum_uint8         en_enable;
    oal_uint8                   auc_resv[3];
    oal_uint32                  ul_item_cnt;
    oal_uint32                  aul_timestamp[MAC_FCS_VERIFY_MAX_ITEMS][MAC_FCS_STAGE_COUNT];
}mac_fcs_verify_stat_stru;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
/* ??????????????flags??????????????????????1?????????????????? */
typedef enum
{
	MAC_REPORT_INFO_FLAGS_HARDWARE_INFO          = BIT(0),
	MAC_REPORT_INFO_FLAGS_QUEUE_INFO			 = BIT(1),
	MAC_REPORT_INFO_FLAGS_MEMORY_INFO	         = BIT(2),
	MAC_REPORT_INFO_FLAGS_EVENT_INFO             = BIT(3),
	MAC_REPORT_INFO_FLAGS_VAP_INFO	             = BIT(4),
	MAC_REPORT_INFO_FLAGS_USER_INFO	             = BIT(5),
	MAC_REPORT_INFO_FLAGS_TXRX_PACKET_STATISTICS = BIT(6),
	MAC_REPORT_INFO_FLAGS_BUTT			         = BIT(7),
}mac_report_info_flags;
#endif

#ifdef _PRE_WLAN_FEATURE_HILINK
/* ????????FBT SCAN??????????:?????????????????? */
typedef enum
{
    HMAC_FBT_SCAN_CLOSE     = 0,
    HMAC_FBT_SCAN_OPEN      = 1,

    HMAC_FBT_SCAN_BUTT
}hmac_fbt_scan_enum;
typedef oal_uint8 hmac_fbt_scan_enum_uint8;

/* ????????????????????*/
typedef enum
{
    HMAC_FBT_SCAN_USER_NOT_USED = 0, /*0??????????*/
    HMAC_FBT_SCAN_USER_IS_USED  = 1, /*1????????????????MAC????*/

    HMAC_FBT_SCAN_USER_BUTT
}hmac_fbt_scan_user_used_state;
#endif

#ifdef _PRE_WLAN_FEATURE_IP_FILTER

/* rx ip???????????????????? */
typedef enum
{
    MAC_IP_FILTER_ENABLE         = 0, /* ??/??ip?????????????? */
    MAC_IP_FILTER_UPDATE_BTABLE  = 1, /* ?????????? */
    MAC_IP_FILTER_CLEAR          = 2, /* ?????????? */

    MAC_IP_FILTER_BUTT
}mac_ip_filter_cmd_enum;
typedef oal_uint8 mac_ip_filter_cmd_enum_uint8;

/* ?????????????? */
typedef struct
{
    oal_uint16                 us_port;          /* ???????????????????????????????? */
    oal_uint8                  uc_protocol;
    oal_uint8                  uc_resv;
    //oal_uint32                  ul_filter_cnt; /* ??????????"??????????????"?????????????????????? */
}mac_ip_filter_item_stru;

/* ???????????? */
typedef struct
{
    oal_uint8                       uc_item_count;
    oal_bool_enum_uint8             en_enable;      /* ???????????????? */
    mac_ip_filter_cmd_enum_uint8    en_cmd;
    oal_uint8                       uc_resv;
    mac_ip_filter_item_stru         ast_filter_items_items[1];
}mac_ip_filter_cmd_stru;

#endif //_PRE_WLAN_FEATURE_IP_FILTER
/*****************************************************************************
  4 ????????????
*****************************************************************************/


/*****************************************************************************
  5 ??????????
*****************************************************************************/


/*****************************************************************************
  6 ????????
*****************************************************************************/


/*****************************************************************************
  7 STRUCT????
*****************************************************************************/
/* device reset?????????????? */
typedef struct
{
    mac_reset_sys_type_enum_uint8  en_reset_sys_type;  /* ???????????? */
    oal_uint8                      uc_value;           /* ?????????? */
    oal_uint8                      uc_resv[2];
}mac_reset_sys_stru;

typedef void (*mac_scan_cb_fn)(void *p_scan_record);

typedef struct
{
    oal_uint16                    us_num_networks;
    mac_ch_type_enum_uint8        en_ch_type;
#ifdef _PRE_WLAN_FEATURE_DFS
    mac_chan_status_enum_uint8    en_ch_status;
#else
    oal_uint8                     auc_resv[1];
#endif
}mac_ap_ch_info_stru;

typedef struct
{
    oal_uint16    us_num_networks;    /* ??????????????????????BSS???? */
    oal_uint8     auc_resv[2];
    oal_uint8     auc_bssid_array[WLAN_MAX_SCAN_BSS_PER_CH][WLAN_MAC_ADDR_LEN];  /* ??????????????????????????BSSID */
}mac_bss_id_list_stru;

#define MAX_PNO_SSID_COUNT          16
#define MAX_PNO_REPEAT_TIMES        4
#define PNO_SCHED_SCAN_INTERVAL     (60 * 1000)

/* PNO?????????????? */
typedef struct
{
    oal_uint8           auc_ssid[WLAN_SSID_MAX_LEN];
    oal_bool_enum_uint8 en_scan_ssid;
    oal_uint8           auc_resv[2];
}pno_match_ssid_stru;

typedef struct
{
    pno_match_ssid_stru   ast_match_ssid_set[MAX_PNO_SSID_COUNT];
    oal_int32             l_ssid_count;                           /* ????????????????ssid???????? */
    oal_int32             l_rssi_thold;                           /* ????????rssi???? */
    oal_uint32            ul_pno_scan_interval;                   /* pno???????? */
    oal_uint8             auc_sour_mac_addr[WLAN_MAC_ADDR_LEN];   /* probe req???????????????????? */
    oal_uint8             uc_pno_scan_repeat;                     /* pno???????????? */
    oal_bool_enum_uint8   en_is_random_mac_addr_scan;             /* ????????mac */

    mac_scan_cb_fn        p_fn_cb;                                /* ???????????????????????????????????? */
}mac_pno_scan_stru;

/* PNO?????????????????? */
typedef struct
{
    mac_pno_scan_stru       st_pno_sched_scan_params;             /* pno?????????????????? */
    //frw_timeout_stru        st_pno_sched_scan_timer;              /* pno?????????????? */
    oal_void               *p_pno_sched_scan_timer;               /* pno????????rtc??????????????????????????????????????????device */
    oal_uint8               uc_curr_pno_sched_scan_times;         /* ????pno???????????? */
    oal_bool_enum_uint8     en_is_found_match_ssid;               /* ??????????????????ssid */
    oal_uint8               auc_resv[2];
}mac_pno_sched_scan_mgmt_stru;


typedef struct
{
    oal_uint8  auc_ssid[WLAN_SSID_MAX_LEN];
    oal_uint8  auc_resv[3];
}mac_ssid_stru;

/* ?????????????? */
typedef struct
{
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type;            /* ????????bss???? */
    wlan_scan_type_enum_uint8           en_scan_type;           /* ????/???? */
    oal_uint8                           uc_bssid_num;           /* ??????????bssid???? */
    oal_uint8                           uc_ssid_num;            /* ??????????ssid???? */

    oal_uint8                           auc_sour_mac_addr[WLAN_MAC_ADDR_LEN];       /* probe req???????????????????? */
    oal_uint8                           uc_p2p0_listen_channel;
    oal_bool_enum_uint8                 en_working_in_home_chan;

    oal_uint8                           auc_bssid[WLAN_SCAN_REQ_MAX_BSSID][WLAN_MAC_ADDR_LEN];  /* ??????bssid */
    mac_ssid_stru                       ast_mac_ssid_set[WLAN_SCAN_REQ_MAX_SSID];               /* ??????ssid */

    oal_uint8                           uc_max_scan_count_per_channel;                          /* ?????????????????? */
    oal_uint8                           uc_max_send_probe_req_count_per_channel;                /* ????????????????????????????????????1 */
    oal_uint8                           auc_resv[2];

    oal_bool_enum_uint8                 en_need_switch_back_home_channel;       /* ???????????????????????????????????????????????????????? */
    oal_uint8                           uc_scan_channel_interval;               /* ????n???????????????????????????????? */
    oal_uint16                          us_work_time_on_home_channel;           /* ?????????????????????????????????? */

    oal_uint8                           uc_last_channel_band;
    oal_uint8                           bit_is_p2p0_scan : 1;   /* ??????p2p0 ???????? */
    oal_uint8                           bit_rsv          : 7;   /* ?????? */
    oal_bool_enum_uint8                 en_is_random_mac_addr_scan;                             /* ??????????mac addr???? */
    oal_bool_enum_uint8                 en_abort_scan_flag;     /* ???????? */

    mac_channel_stru                    ast_channel_list[WLAN_MAX_CHANNEL_NUM];

    oal_uint8                           uc_channel_nums;        /* ???????????????????? */
    oal_uint8                           uc_probe_delay;         /* ??????????probe request???????????????? */
    oal_uint16                          us_scan_time;           /* ????????????????????????????????????, ms????????10???????? */

    wlan_scan_mode_enum_uint8           en_scan_mode;                   /* ????????:???????? or ???????? */
    oal_uint8                           uc_curr_channel_scan_count;     /* ??????????????????????????????????????ssid??probe req????????????????ssid??probe req?? */
    oal_uint8                           uc_scan_func;                   /* DMAC SCANNER ???????? */
    oal_uint8                           uc_vap_id;                      /* ??????????????vap id */
    oal_uint64                          ull_cookie;             /* P2P ??????????cookie ?? */


    /* ????:????????????:???????????????????????????????????? */
    mac_scan_cb_fn                      p_fn_cb;
}mac_scan_req_stru;

/* ??????????????rssi?????????????????????????? */
typedef struct
{
    oal_uint32     ul_rssi_debug_switch;        /* ??????????????rssi?????????????? */
    oal_uint32     ul_rx_comp_isr_interval;     /* ??????????????????????????????rssi???? */
    oal_uint32     ul_curr_rx_comp_isr_count;   /* ?????????????????????????????????? */
}mac_rssi_debug_switch_stru;

/* ACS ?????????????? */
typedef struct
{
    oal_uint8  uc_cmd;
    oal_uint8  uc_chip_id;
    oal_uint8  uc_device_id;
    oal_uint8  uc_resv;

    oal_uint32 ul_len;      /* ??????????????????4?????? */
    oal_uint32 ul_cmd_cnt;  /* ?????????? */
}mac_acs_response_hdr_stru;

typedef struct
{
    oal_uint8   uc_cmd;
    oal_uint8   auc_arg[3];
    oal_uint32  ul_cmd_cnt;  /* ?????????? */
    oal_uint32  ul_cmd_len;  /* ????????????auc_data?????????????? */
    oal_uint8   auc_data[4];
}mac_acs_cmd_stru;

typedef mac_acs_cmd_stru    mac_init_scan_req_stru;

typedef enum
{
    MAC_ACS_RSN_INIT,
    MAC_ACS_RSN_LONG_TX_BUF,
    MAC_ACS_RSN_LARGE_PER,
    MAC_ACS_RSN_MWO_DECT,
    MAC_ACS_RSN_RADAR_DECT,

    MAC_ACS_RSN_BUTT
}mac_acs_rsn_enum;
typedef oal_uint8 mac_acs_rsn_enum_uint8;

typedef enum
{
    MAC_ACS_SW_NONE = 0x0,
    MAC_ACS_SW_INIT = 0x1,
    MAC_ACS_SW_DYNA = 0x2,
    MAC_ACS_SW_BOTH = 0x3,

    MAC_ACS_SW_BUTT
}en_mac_acs_sw_enum;
typedef oal_uint8 en_mac_acs_sw_enum_uint8;

typedef enum
{
    MAC_ACS_SET_CH_DNYA = 0x0,
    MAC_ACS_SET_CH_INIT = 0x1,

    MAC_ACS_SET_CH_BUTT
}en_mac_acs_set_ch_enum;
typedef oal_uint8 en_mac_acs_set_ch_enum_uint8;

typedef struct
{
    oal_bool_enum_uint8               en_sw_when_connected_enable : 1;
    oal_bool_enum_uint8               en_drop_dfs_channel_enable  : 1;
    oal_bool_enum_uint8               en_lte_coex_enable          : 1;
    en_mac_acs_sw_enum_uint8          en_acs_switch               : 5;
}mac_acs_switch_stru;

/* DMAC SCAN ????????BSS???????????? */
typedef struct
{

    oal_int8                            c_rssi;             /* bss?????????? */
    oal_uint8                           uc_channel_number;  /* ?????? */
    oal_uint8                           auc_bssid[WLAN_MAC_ADDR_LEN];

    /* 11n, 11ac???? */
    oal_bool_enum_uint8                 en_ht_capable;             /* ????????ht */
    oal_bool_enum_uint8                 en_vht_capable;            /* ????????vht */
    wlan_bw_cap_enum_uint8              en_bw_cap;                 /* ?????????? 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8   en_channel_bandwidth;      /* ???????????? */
}mac_scan_bss_stats_stru;

/* DMAC SCAN ?????????????????????? */
typedef struct
{
    oal_uint8   uc_channel_number;      /* ?????? */
    oal_uint8   uc_stats_valid;
    oal_uint8   uc_stats_cnt;           /* ?????????????????? */
    oal_uint8   uc_free_power_cnt;      /* ???????????? */

    oal_uint8   uc_bandwidth_mode;
    oal_uint8   auc_resv[1];
    oal_int16   s_free_power_stats_20M;
    oal_int16   s_free_power_stats_40M;
    oal_int16   s_free_power_stats_80M;

    oal_uint32  ul_total_stats_time_us;
    oal_uint32  ul_total_free_time_20M_us;
    oal_uint32  ul_total_free_time_40M_us;
    oal_uint32  ul_total_free_time_80M_us;
    oal_uint32  ul_total_send_time_us;
    oal_uint32  ul_total_recv_time_us;

    oal_uint8   uc_radar_detected;  // FIXME: feed value
    oal_uint8   uc_radar_bw;
    oal_uint8   uc_radar_type;
    oal_uint8   uc_radar_freq_offset;
}mac_scan_chan_stats_stru;

typedef struct
{
    oal_int8                            c_rssi;             /* bss?????????? */
    oal_uint8                           uc_channel_number;  /* ?????? */

    oal_bool_enum_uint8                 en_ht_capable   : 1;             /* ????????ht */
    oal_bool_enum_uint8                 en_vht_capable  : 1;            /* ????????vht */
    wlan_bw_cap_enum_uint8              en_bw_cap       : 3;            /* ?????????? 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8   en_channel_bandwidth : 3;      /* ???????????? */
}mac_scan_bss_stats_simple_stru;

typedef struct
{
    oal_uint32  us_total_stats_time_ms  : 9;  // max 512 ms
    oal_uint32  uc_bandwidth_mode       : 3;
    oal_uint32  uc_radar_detected       : 1;  // FIXME: feed
    oal_uint32  uc_dfs_check_needed     : 1;
    oal_uint32  uc_radar_bw             : 3;
    oal_uint32  uc_radar_type           : 4;
    oal_uint32  uc_radar_freq_offset    : 3;
    oal_uint8   uc_channel_number;      /* ?????? */

    oal_int8    s_free_power_20M;           // dBm
    oal_int8    s_free_power_40M;
    oal_int8    s_free_power_80M;
    oal_uint8   uc_free_time_20M_rate;      // percent, 255 scaled
    oal_uint8   uc_free_time_40M_rate;
    oal_uint8   uc_free_time_80M_rate;
    oal_uint8   uc_total_send_time_rate;    // percent, 255 scaled
    oal_uint8   uc_total_recv_time_rate;
}mac_scan_chan_stats_simple_stru;

/* DMAC SCAN ?????????????? */
typedef struct
{
    oal_uint8                   uc_nchans;      /* ????????       */
    oal_uint8                   uc_nbss;        /* BSS???? */
    oal_uint8                   uc_scan_func;   /* ?????????????? */

    oal_uint8                   uc_need_rank    : 1; // kernel write, app read
    oal_uint8                   uc_obss_on      : 1;
    oal_uint8                   uc_dfs_on       : 1;
    oal_uint8                   uc_dbac_on      : 1;
    oal_uint8                   uc_chip_id      : 2;
    oal_uint8                   uc_device_id    : 2;

}mac_scan_event_stru;


#ifdef _PRE_WLAN_FEATURE_DFS
typedef struct
{
    oal_bool_enum_uint8    en_dfs_switch;                               /* DFS???????? bit0:dfs????,bit1:????AP????DFS??????????????wait_start */
    oal_bool_enum_uint8    en_cac_switch;
    oal_bool_enum_uint8    en_offchan_cac_switch;
    oal_uint8              uc_debug_level;                              /* bit0:????????????????bit1:?????????????? */
    oal_uint8              uc_offchan_flag;                             /* bit0:0????homechan,1????offchan; bit1:0????????,1????offchancac */
    oal_uint8              uc_offchan_num;
    oal_uint8              uc_timer_cnt;
    oal_uint8              uc_timer_end_cnt;
    oal_uint8              uc_cts_duration;
    oal_uint8              uc_dmac_channel_flag;                        /* dmac????????????????off or home */
    oal_uint8              auc_res[2];
    oal_uint32             ul_dfs_cac_outof_5600_to_5650_time_ms;       /* CAC??????????5600MHz ~ 5650MHz????????????60?? */
    oal_uint32             ul_dfs_cac_in_5600_to_5650_time_ms;          /* CAC??????????5600MHz ~ 5650MHz????????????10???? */
    oal_uint32             ul_off_chan_cac_outof_5600_to_5650_time_ms;  /* Off-Channel CAC??????????5600MHz ~ 5650MHz????????????6???? */
    oal_uint32             ul_off_chan_cac_in_5600_to_5650_time_ms;     /* Off-Channel CAC??????????5600MHz ~ 5650MHz????????????60???? */
    oal_uint16             us_dfs_off_chan_cac_opern_chan_dwell_time;   /* Off-channel CAC???????????????????? */
    oal_uint16             us_dfs_off_chan_cac_off_chan_dwell_time;     /* Off-channel CAC??Off-Channel?????????????? */
    oal_uint32             ul_radar_detected_timestamp;
}mac_dfs_info_stru;

typedef struct
{
    oal_uint8             uc_chan_idx;        /* ???????? */
    oal_uint8             uc_device_id;       /* device id */
    oal_uint8             auc_resv[2];
    frw_timeout_stru      st_dfs_nol_timer;   /* NOL?????????? */
    oal_dlist_head_stru   st_entry;           /* NOL???? */
}mac_dfs_nol_node_stru;

typedef struct
{
    frw_timeout_stru      st_dfs_cac_timer;                   /* CAC?????? */
    frw_timeout_stru      st_dfs_off_chan_cac_timer;          /* Off-Channel CAC?????? */
    frw_timeout_stru      st_dfs_chan_dwell_timer;            /* ?????????????????????????????????????? */
    frw_timeout_stru      st_dfs_radar_timer;
    mac_dfs_info_stru     st_dfs_info;
    oal_dlist_head_stru   st_dfs_nol;
}mac_dfs_core_stru;
#endif

/* bss?????????????????? */
typedef struct
{
    oal_uint8                    uc_bss_80211i_mode;                                      /*????????AP????????????WPA??WPA2??BIT0: WPA; BIT1:WPA2 */
    oal_uint8                    uc_rsn_grp_policy;                                       /*????????WPA2????????AP??????????????????*/
    oal_uint8                    auc_rsn_pairwise_policy[MAC_PAIRWISE_CIPHER_SUITES_NUM]; /*????????WPA2????????AP??????????????????*/
    oal_uint8                    auc_rsn_auth_policy[MAC_AUTHENTICATION_SUITE_NUM];        /*????????WPA2????????AP??????????????*/
    oal_uint8                    auc_rsn_cap[2];                                          /*????????RSN????????????????????????copy????*/
    oal_uint8                    auc_wpa_pairwise_policy[MAC_PAIRWISE_CIPHER_SUITES_NUM]; /*????????WPA????????AP??????????????????*/
    oal_uint8                    auc_wpa_auth_policy[MAC_AUTHENTICATION_SUITE_NUM];        /*????????WPA????????AP??????????????*/
    oal_uint8                    uc_wpa_grp_policy;                                       /*????????WPA????????AP??????????????????*/
    oal_uint8                    uc_grp_policy_match;                                     /*??????????????????????*/
    oal_uint8                    uc_pairwise_policy_match;                                /*??????????????????????*/
    oal_uint8                    uc_auth_policy_match;                                    /*??????????????????????*/
}mac_bss_80211i_info_stru;


/* ?????????????????????? */
typedef enum
{
    MAC_SCAN_SUCCESS = 0,       /* ???????? */
    MAC_SCAN_PNO     = 1,       /* pno???????? */
    MAC_SCAN_TIMEOUT = 2,       /* ???????? */
    MAC_SCAN_REFUSED = 3,       /* ?????????? */
    MAC_SCAN_ABORT   = 4,       /* ???????? */
    MAC_SCAN_STATUS_BUTT,       /* ???????????????????????????????? */
}mac_scan_status_enum;
typedef oal_uint8   mac_scan_status_enum_uint8;

/* ???????? */
typedef struct
{
    mac_scan_status_enum_uint8  en_scan_rsp_status;
    oal_uint8                   auc_resv[3];
    oal_uint64                  ull_cookie;
}mac_scan_rsp_stru;


/* ????????BSS?????????? */
typedef struct
{
    /* ???????? */
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type;                        /* bss???????? */
    oal_uint8                           uc_dtim_period;                     /* dtime???? */
    oal_uint8                           uc_dtim_cnt;                        /* dtime cnt */
    oal_bool_enum_uint8                 en_11ntxbf;                         /* 11n txbf */
    oal_bool_enum_uint8                 en_new_scan_bss;                    /* ????????????????BSS */
    oal_uint8                           auc_resv1[1];
    oal_int8                            c_rssi;                             /* bss?????????? */
    oal_int8                            ac_ssid[WLAN_SSID_MAX_LEN];         /* ????ssid */
    oal_uint16                          us_beacon_period;                   /* beacon???? */
    oal_uint16                          us_cap_info;                        /* ???????????? */
    oal_uint8                           auc_mac_addr[WLAN_MAC_ADDR_LEN];    /* ?????????? mac??????bssid???? */
    oal_uint8                           auc_bssid[WLAN_MAC_ADDR_LEN];       /* ????bssid */
    mac_channel_stru                    st_channel;                         /* bss?????????? */
    oal_uint8                           uc_wmm_cap;                         /* ????????wmm */
    oal_uint8                           uc_uapsd_cap;                       /* ????????uapsd */
    oal_bool_enum_uint8                 en_desired;                         /* ??????????bss???????????? */
    oal_uint8                           uc_num_supp_rates;                  /* ???????????????? */
    oal_uint8                           auc_supp_rates[WLAN_MAX_SUPP_RATES];/* ???????????? */

#ifdef _PRE_WLAN_FEATURE_11D
    oal_int8                            ac_country[WLAN_COUNTRY_STR_LEN];   /* ?????????? */
    oal_uint8                           auc_resv2[1];
#endif

    /* ?????????????? */
    mac_bss_80211i_info_stru            st_bss_sec_info;                    /*????????STA????????????????AP????????????*/

    /* 11n 11ac???? */
    oal_bool_enum_uint8                 en_ht_capable;                      /* ????????ht */
    oal_bool_enum_uint8                 en_vht_capable;                     /* ????????vht */
    wlan_bw_cap_enum_uint8              en_bw_cap;                          /* ?????????? 0-20M 1-40M */
    wlan_channel_bandwidth_enum_uint8   en_channel_bandwidth;               /* ???????? */
    oal_uint8                           uc_coex_mgmt_supp;                  /* ???????????????? */
    oal_bool_enum_uint8                 en_ht_ldpc;                         /* ????????ldpc */
    oal_bool_enum_uint8                 en_ht_stbc;                         /* ????????stbc */
    oal_uint8                           uc_wapi;
    oal_uint32                          ul_timestamp;                       /* ??????bss???????? */

    /* ?????????? */
    oal_uint32                          ul_mgmt_len;                        /* ???????????? */
    oal_uint8                           auc_mgmt_buff[4];                   /* ????beacon????probe rsp?? */
    //oal_uint8                          *puc_mgmt_buff;                      /* ????beacon????probe rsp?? */
}mac_bss_dscr_stru;

#ifdef _PRE_WLAN_DFT_STAT
/* ?????????????? */
typedef struct
{
    /* ?????????????? */
    oal_uint32          aul_rx_mgmt[WLAN_MGMT_SUBTYPE_BUTT];

    /* ???????????????????????? */
    oal_uint32          aul_tx_mgmt_soft[WLAN_MGMT_SUBTYPE_BUTT];

    /* ???????????????????? */
    oal_uint32          aul_tx_mgmt_hardware[WLAN_MGMT_SUBTYPE_BUTT];
}mac_device_mgmt_statistic_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
typedef struct
{
    oal_bool_enum_uint8             en_psta_enable;
    oal_uint8                       uc_proxysta_cnt;                                        /* ????proxy sta??????(??????main ProxySTA) */
    oal_uint8                       auc_resv[2];
}mac_device_psta_stru;

#define mac_dev_xsta_num(dev) ((dev)->st_psta.uc_proxysta_cnt)
#define mac_is_proxysta_enabled(pst_dev) ((pst_dev)->st_psta.en_psta_enable)
#endif


#ifdef _PRE_WLAN_DFT_STAT
/* ???????????????????????????????? */
typedef struct
{
    oal_uint32                          ul_non_directed_frame_num;             /* ???????????????????? */
    oal_uint8                           uc_collect_period_cnt;                 /* ????????????????????100?????????????????????????????? */
    oal_bool_enum_uint8                 en_non_directed_frame_stat_flg;        /* ?????????????????????????????? */
    oal_int16                           s_ant_power;                           /* ?????????? */
    frw_timeout_stru                    st_collect_period_timer;               /* ?????????????? */
}mac_device_dbb_env_param_ctx_stru;
#endif

typedef enum
{
    MAC_DFR_TIMER_STEP_1 = 0,
    MAC_DFR_TIMER_STEP_2 = 1,

}mac_dfr_timer_step_enum;
typedef oal_uint8 mac_dfr_timer_step_enum_uint8;

typedef struct
{
    oal_uint32                         ul_tx_seqnum;                           /* ????????tx??????SN?? */
    oal_uint16                         us_seqnum_used_times;                   /* ??????????ul_tx_seqnum?????? */
    oal_uint16                         us_incr_constant;                       /* ??????Qos ???????????????? */
}mac_tx_seqnum_struc;

typedef struct
{
    oal_uint8                           uc_p2p_device_num;                      /* ????device????P2P_DEVICE???? */
    oal_uint8                           uc_p2p_goclient_num;                    /* ????device????P2P_CL/P2P_GO???? */
    oal_uint8                           uc_p2p0_vap_idx;                        /* P2P ????????????P2P_DEV(p2p0) ???? */
    mac_vap_state_enum_uint8            en_last_vap_state;                      /* P2P0/P2P_CL ????VAP ????????????????????VAP ???????????????? */
    oal_uint8                           uc_resv[2];                             /* ???? */
    oal_uint8                           en_roc_need_switch;                     /* remain on channel????????????????*/
    oal_uint8                           en_p2p_ps_pause;                        /* P2P ????????????pause????*/
    oal_net_device_stru                *pst_p2p_net_device;                     /* P2P ????????????net_device(p2p0) ???? */
    oal_uint64                          ull_send_action_id;                     /* P2P action id/cookie */
    oal_uint64                          ull_last_roc_id;
    oal_ieee80211_channel_stru          st_listen_channel;
    oal_nl80211_channel_type            en_listen_channel_type;
    oal_net_device_stru                *pst_primary_net_device;                 /* P2P ????????????net_device(wlan0) ???? */
}mac_p2p_info_stru;

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
typedef struct
{
    oal_bool_enum_uint8                 en_brk_limit_aggr_enable;
    oal_uint8                           uc_resv[3];
    oal_uint32                          ul_tx_dataflow_brk_cnt;
    oal_uint32                          ul_last_tx_complete_isr_cnt;
}mac_tx_dataflow_brk_bypass_struc;
#endif

/* ???????? */
typedef enum
{
    MAC_SCREEN_OFF = 0,
    MAC_SCREEN_ON  = 1,
}mac_screen_state_enum;

#ifdef _PRE_WLAN_FEATURE_HILINK
/* ?????????????????????????? */
typedef struct
{
    oal_uint8   uc_is_used;                 /* ???????????????? */
    oal_uint8   uc_user_channel;            /* ?????????????????? */
    oal_uint8   auc_user_bssid[6];          /* ??????????????AP??bssid */
    oal_uint8   auc_user_mac_addr[6];       /* ??????????MAC???? */
    oal_int8    c_rssi;                     /* ?????????????????? */
    oal_uint8   uc_is_found;                /* 0-????????????????1-?????????? */
    oal_uint32  ul_rssi_timestamp;          /* ?????????????????? */
    oal_uint32  ul_total_pkt_cnt;           /* ????????STA?????????? */
}mac_fbt_scan_result_stru;


/* ?????????????????????????????? */
typedef struct
{
    oal_uint8  uc_fbt_scan_enable;              /* ?????????????????????????? */
    oal_uint8  uc_scan_channel;                 /* ????????????????????0?????????????? */
    oal_uint32 ul_scan_interval;                /* ???????????????????????????????????????????? */
    oal_uint32 ul_scan_report_period;           /* ???????????????? */
    mac_fbt_scan_result_stru ast_fbt_scan_user_list[HMAC_FBT_MAX_USER_NUM]; /* ????32???????????????????? */
    frw_timeout_stru st_timer;                  /* ?????????????????????????? */
}mac_fbt_scan_mgmt_stru;

/* ???????????????????? fbt_monitor_specified_sta????????????mac????*/
typedef struct
{
    oal_uint8               auc_mac_addr[WLAN_MAC_ADDR_LEN];    /* MAC???? */
    oal_uint8               auc_resv[2];
}mac_fbt_scan_sta_addr_stru;

typedef struct
{
    unsigned char   mac[WLAN_MAC_ADDR_LEN];                     /* ??????????sta??mac???? */
    oal_uint32      ul_channel;                                 /* ???????????????? */
    oal_uint32      ul_interval;                                /* ???????????? */
    oal_uint8       en_is_on;                                   /* ???????????? */
}mac_cfg_fbt_scan_params_stru;

#endif
/* device?????? */
typedef struct
{
    oal_uint32                          ul_core_id;
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    oal_uint8                           auc_vap_id[WLAN_REPEATER_SERVICE_VAP_MAX_NUM_PER_DEVICE];   /* device????????vap????????????VAP ID */
#else
    oal_uint8                           auc_vap_id[WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE];   /* device????????vap????????????VAP ID */
#endif
    oal_uint8                           uc_cfg_vap_id;                          /* ????vap ID */
    oal_uint8                           uc_device_id;                           /* ????ID */
    oal_uint8                           uc_chip_id;                             /* ????ID */
    oal_uint8                           uc_device_reset_in_progress;            /* ??????????*/

    oal_bool_enum_uint8                 en_device_state;                        /* ????????????????????(OAL_TRUE????????????OAL_FALSE???????? ) */
    oal_uint8                           uc_vap_num;                             /* ????device????????VAP????(AP+STA) */
    oal_uint8                           uc_sta_num;                             /* ????device????STA???? */
/* begin: P2P */
    mac_p2p_info_stru                   st_p2p_info;                            /* P2P ???????? */
/* end: P2P */

    oal_uint8                           auc_hw_addr[WLAN_MAC_ADDR_LEN];         /* ??eeprom??flash??????mac??????ko??????????hal???????? */
    /* device???????? */
    oal_uint8                           uc_max_channel;                         /* ??????VAP????????????????VAP????????????????????????????DBAC?????? */
    wlan_channel_band_enum_uint8        en_max_band;                            /* ??????VAP??????????????VAP????????????????????????????DBAC?????? */

    wlan_channel_bandwidth_enum_uint8   en_max_bandwidth;                       /* ??????VAP????????????????????VAP????????????????????????????DBAC?????? */
    oal_uint8                           uc_tx_chain;                            /* ???????? */
    oal_uint8                           uc_rx_chain;                            /* ???????? */
    wlan_nss_enum_uint8                 en_nss_num;                             /* Nss ?????????? */

    oal_bool_enum_uint8                 en_wmm;                                 /* wmm???????? */
    wlan_tidno_enum_uint8               en_tid;
    oal_uint8                           en_reset_switch;                        /* ????????????????*/
    oal_uint8                           uc_csa_vap_cnt;                        /* ????running AP????????CSA????????????1????????????AP?????????????????????????????????? */

    hal_to_dmac_device_stru            *pst_device_stru;                        /* ??mac??????????HAL????????????????????device?????? */

    oal_uint32                          ul_beacon_interval;                     /*device????beacon interval,device??????VAP????????????*/
    oal_uint32                          ul_duty_ratio;                          /* ?????????? */
    oal_uint32                          ul_duty_ratio_lp;                       /*??????????????????????*/
    oal_uint32                          ul_rx_nondir_duty_lp;                   /*????????????????non-direct??????????*/
    oal_uint32                          ul_rx_dir_duty_lp;                      /*????????????????direct??????????*/

    oal_int16                           s_upc_amend;                            /* UPC?????? */

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    oal_bool_enum_uint8                 en_is_wavetest;                         /* wavetest???????? 1:??  0:????    */
    oal_uint8                           uc_lock_channel;                        /* AGC???????? 0:????????0  1:????????1   2:??????  */
    oal_uint8                           auc_rev[2];
#endif

    /* device???? */
    wlan_protocol_cap_enum_uint8        en_protocol_cap;                        /* ???????? */
    wlan_band_cap_enum_uint8            en_band_cap;                            /* ???????? */
    wlan_bw_cap_enum_uint8              en_bandwidth_cap;                       /* ???????? */
    oal_uint8                           bit_ldpc_coding : 1,                    /* ????????????LDPC???????? */
                                        bit_tx_stbc     : 1,                    /* ????????????2x1 STBC???? */
                                        bit_rx_stbc     : 3,                    /* ????????stbc???? */
                                        bit_su_bfmer    : 1,                    /* ??????????????beamformer */
                                        bit_su_bfmee    : 1,                    /* ??????????????beamformee */
                                        bit_mu_bfmee    : 1;                    /* ??????????????beamformee */


    oal_uint16                          us_device_reset_num;                    /* ??????????????*/
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    mac_device_psta_stru                st_psta;
#endif

    mac_data_rate_stru                  st_mac_rates_11g[MAC_DATARATES_PHY_80211G_NUM];  /* 11g???? */

    mac_pno_sched_scan_mgmt_stru       *pst_pno_sched_scan_mgmt;                    /* pno?????????????????????????????????????????????????? */
    mac_scan_req_stru                   st_scan_params;                             /* ?????????????????????? */
    frw_timeout_stru                    st_scan_timer;                              /* ?????????????????????? */
    frw_timeout_stru                    st_obss_scan_timer;                         /* obss?????????????????????? */
    mac_channel_stru                    st_p2p_vap_channel;                     /* p2p listen??????p2p????????????p2p listen?????????? */

    oal_uint8                           uc_active_user_cnt;                     /* ?????????? */
    oal_uint8                           uc_asoc_user_cnt;                       /* ?????????? */
    oal_bool_enum_uint8                 en_2040bss_switch;                      /* 20/40 bss???????? */
    oal_uint8                           uc_in_suspend;
#ifdef _PRE_WLAN_FEATURE_11K
    //frw_timeout_stru                    st_backoff_meas_timer;
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS
    oal_uint8                           auc_resv18[2];
    oal_bool_enum_uint8                 en_smps;                                /* Device SMPS???????? */
    oal_uint8                           uc_dev_smps_mode;                       /* Device SMPS???? */
#endif


#ifdef _PRE_SUPPORT_ACS
    /* DMAC ACS???? */
    oal_void                           *pst_acs;
    mac_acs_switch_stru                 st_acs_switch;
#endif

    /* linux????????device???????? */
    oal_wiphy_stru                     *pst_wiphy;                             /* ??????????VAP??????wiphy????????????AP/STA????????????????????????VAP????????wiphy */

    oal_uint8                           uc_mac_vap_id;                         /* ??vap????????????????????mac vap id */
    mac_bss_id_list_stru                st_bss_id_list;                        /* ???????????????????? */

    oal_bool_enum_uint8                 en_dbac_enabled;
    oal_bool_enum_uint8                 en_dbac_running;                       /* DBAC?????????? */
    oal_bool_enum_uint8                 en_dbac_has_vip_frame;                 /* ????DBAC?????????????????? */
    oal_uint8                           uc_arpoffload_switch;
    oal_uint8                           uc_wapi;
    oal_uint8                           uc_reserve;
    oal_bool_enum_uint8                 en_is_random_mac_addr_scan;            /* ????mac????????,??hmac???? */
    oal_uint8                           auc_mac_oui[WLAN_RANDOM_MAC_OUI_LEN];  /* ????mac????OUI,?????????? */
    oal_uint8                           auc_rsv[2];

#ifdef _PRE_WLAN_FEATURE_DFS
    mac_dfs_core_stru                   st_dfs;
#endif

#ifdef _PRE_WLAN_FEATURE_HILINK
    mac_fbt_scan_mgmt_stru             st_fbt_scan_mgmt;                        /* ?????????????????????????????? */
#endif
    /*????Device????????????????dmac_device*/
#if IS_DEVICE
    oal_uint8                           auc_resv12[2];
    oal_uint16                          us_total_mpdu_num;                      /* device??????TID????????mpdu_num???? */
    oal_uint16                          aus_ac_mpdu_num[WLAN_WME_AC_BUTT];      /* device??????AC??????mpdu_num?? */
    oal_uint16                          aus_vap_mpdu_num[WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];      /* ????????vap??????mpdu_num?? */

    oal_void                           *p_alg_priv;                             /* ?????????????? */

    oal_uint16                          us_dfs_timeout;
    oal_uint32                          ul_rx_buf_too_small_show_counter;
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    oal_uint32                          ul_beacon_miss_show_counter;
    oal_uint32                          ul_rx_fsm_st_timeout_show_counter;
    oal_uint32                          ul_tx_fsm_st_timeout_show_counter;
    oal_uint16                          us_user_nums_max;
    oal_uint8                           auc_resv26[2];
    oal_uint32                          ul_pcie_reg110_timeout_counter;
    oal_uint32                          ul_pcie_read_counter;

    mac_tx_dataflow_brk_bypass_struc    st_dataflow_brk_bypass;
#endif
    oal_uint32                          ul_first_timestamp;                         /*????????????????????????*/

    oal_uint8                           auc_tx_ba_index_table[DMAC_TX_BA_LUT_BMAP_LEN];      /* ??????LUT?? */

    /* ???????????????? */
    oal_uint32                          ul_scan_timestamp;                      /* ?????????????????????????? */
    oal_uint8                           uc_scan_chan_idx;                       /* ???????????????? */
    mac_scan_state_enum_uint8           en_curr_scan_state;                     /* ????????????????????????????obss??????host???????????????????????????????????????? */

    oal_uint8                           uc_resume_qempty_flag;                  /* ????????qempty????, ?????????? */
    oal_uint8                           uc_scan_count;

    mac_channel_stru                    st_home_channel;                        /* ???????????? ???????????? */
    mac_fcs_cfg_stru                    st_fcs_cfg;                             /* ???????????????? */

    mac_scan_chan_stats_stru            st_chan_result;                         /* dmac?????? ?????????????????????? */

    oal_uint8                           auc_original_mac_addr[WLAN_MAC_ADDR_LEN]; /* ????????????????????MAC???? */
    oal_uint8                           uc_scan_ap_num_in_2p4;
    oal_bool_enum_uint8                 en_scan_curr_chan_find_bss_flag;        /* ????????????????????BSS */

#ifdef _PRE_WLAN_DFT_REG
    frw_timeout_stru                    st_reg_prd_timer;                       /* ???????????????????? */
#endif

    /* ???????????????? */
    frw_timeout_stru                    st_active_user_timer;                   /* ?????????????? */

    oal_uint8                           auc_ra_lut_index_table[WLAN_ACTIVE_USER_IDX_BMAP_LEN];  /* lut?????? */

#ifdef _PRE_WLAN_DFT_STAT
    /* ?????????????? */
    mac_device_mgmt_statistic_stru      st_mgmt_stat;
    mac_device_dbb_env_param_ctx_stru   st_dbb_env_param_ctx;                  /* ???????????????????????????????? */
#endif
    mac_fcs_mgr_stru                    st_fcs_mgr;

    oal_uint8                           uc_csa_cnt;                            /* ????AP????????CSA????????????1??AP???????????????????????? */

    oal_bool_enum_uint8                 en_txop_enable;                        /* ????????????????TXOP???? */
    oal_uint8                           uc_tx_ba_num;                  /* ????????BA???????? */
    oal_uint8                           auc_resv[1];


    frw_timeout_stru                    st_keepalive_timer;                     /* keepalive?????? */

#ifdef _PRE_DEBUG_MODE
    frw_timeout_stru                    st_exception_report_timer;
#endif
    oal_uint32                          aul_mac_err_cnt[HAL_MAC_ERROR_TYPE_BUTT];   /*mac ??????????*/

#ifdef _PRE_WLAN_FEATURE_PM
    oal_void*                           pst_pm_arbiter;                        /*device????????????????????????*/
    oal_bool_enum_uint8                 en_pm_enable;                          /*PM????????????????????*/
    oal_uint8                           auc_resv7[3];
#endif

#ifdef _PRE_WLAN_FEATURE_GREEN_AP
    oal_void                            *pst_green_ap_mgr;                     /*device????????green ap??????????*/
#endif

#ifdef _PRE_WLAN_REALTIME_CALI
    frw_timeout_stru                    st_realtime_cali_timer;                    /* ??????????????????*/
#endif
#endif /* IS_DEVICE */

    /*????Host????????????????hmac_device*/
#if IS_HOST
#ifndef _PRE_WLAN_FEATURE_AMPDU_VAP
    oal_uint8                           uc_rx_ba_session_num;                   /* ??device????rx BA?????????? */
    oal_uint8                           uc_tx_ba_session_num;                   /* ??device????tx BA?????????? */
    oal_uint8                           auc_resv11[2];
#endif
    oal_bool_enum_uint8                 en_vap_classify;                        /* ????????????vap?????????? */
    oal_uint8                           auc_resv14[3];

    oal_uint8                           auc_rx_ba_lut_idx_table[DMAC_BA_LUT_IDX_BMAP_LEN];   /* ??????LUT?? */

    frw_timeout_stru                    st_obss_aging_timer;                    /* OBSS?????????????? */

#ifdef _PRE_WLAN_FEATURE_SMPS
    oal_uint8                           uc_no_smps_user_cnt;                    /* ??????SMPS????????  */
    oal_uint8                           auc_resv17[3];
#endif
    mac_ap_ch_info_stru                 st_ap_channel_list[MAC_MAX_SUPP_CHANNEL];
    oal_uint8                           uc_ap_chan_idx;                        /* ???????????????? */
    oal_uint8                           auc_resv21[3];

    oal_bool_enum_uint8                 en_40MHz_intol_bit_recd;
    oal_uint8                           auc_resv4[3];
#endif /* IS_HOST */
#ifdef _PRE_WLAN_FEATURE_STA_PM
    hal_mac_key_statis_info_stru      st_mac_key_statis_info;                   /* mac???????????? */
#endif

#ifdef _PRE_WLAN_RF_CALI
     oal_work_stru                       auto_cali_work;
#endif
}mac_device_stru;

#pragma pack(push,1)
/* ??????????????????????????????????host??????????netbuf?????? */
typedef struct
{
    oal_int32                           l_rssi;                     /* ???????? */
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type;                /* ????????bss???? */
    oal_uint8                           auc_resv[3];                /* ???????? */
}mac_scanned_result_extend_info_stru;
#pragma pack(pop)

/* chip?????? */
typedef struct
{
    oal_uint8                   auc_device_id[WLAN_DEVICE_MAX_NUM_PER_CHIP];    /* CHIP??????DEV??????????????ID?????? */
    oal_uint8                   uc_device_nums;                                 /* chip??device?????? */
    oal_uint8                   uc_chip_id;                                     /* ????ID */
    oal_bool_enum_uint8         en_chip_state;                                  /* ??????????????????OAL_TRUE????????????OAL_FALSE???????? */
    oal_uint32                  ul_chip_ver;                                    /* ???????? */
    hal_to_dmac_chip_stru       *pst_chip_stru;                                 /* ??mac??????????HAL????????????????????chip?????? */

}mac_chip_stru;

#ifdef _PRE_WLAN_FEATURE_IP_FILTER
typedef enum
{
    MAC_RX_IP_FILTER_STOPED  = 0, //??????????????????????????????????????????????
    MAC_RX_IP_FILTER_WORKING = 1, //??????????????????????????
    MAC_RX_IP_FILTER_BUTT
}mac_ip_filter_state_enum;
typedef oal_uint8 mac_ip_filter_state_enum_uint8;

typedef struct
{
    mac_ip_filter_state_enum_uint8 en_state;     //????????????????????????
    oal_uint8                  uc_btable_items_num; //??????????????????items????
    oal_uint8                  uc_btable_size;      //??????????????????????????items????
    oal_uint8                  uc_resv;
    mac_ip_filter_item_stru   *pst_filter_btable;   //??????????
}mac_rx_ip_filter_struc;
#endif //_PRE_WLAN_FEATURE_IP_FILTER


/* board?????? */
typedef struct
{
    mac_chip_stru               ast_chip[WLAN_CHIP_MAX_NUM_PER_BOARD];              /* board?????????? */
    oal_uint8                   uc_chip_id_bitmap;                                  /* ????chip???????????????? */
    oal_uint8                   auc_resv[3];                                        /* ???????? */
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
    mac_rx_ip_filter_struc      st_rx_ip_filter;                      /* rx ip???????????????????? */
#endif //_PRE_WLAN_FEATURE_IP_FILTER
}mac_board_stru;

typedef struct
{
    mac_device_stru                    *pst_mac_device;
}mac_wiphy_priv_stru;


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/
    /* ?????????????????? */
#ifdef _PRE_WLAN_FEATURE_DBAC
#define MAC_DBAC_ENABLE(_pst_device) (_pst_device->en_dbac_enabled == OAL_TRUE)
#else
#define MAC_DBAC_ENABLE(_pst_device) (OAL_FALSE)
#endif
#ifdef _PRE_WLAN_FEATURE_WMMAC
extern oal_bool_enum_uint8 g_en_wmmac_switch;
#endif


OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_is_dbac_enabled(mac_device_stru *pst_device)
{
    return  pst_device->en_dbac_enabled;
}


OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_is_dbac_running(mac_device_stru *pst_device)
{
    if (OAL_FALSE == pst_device->en_dbac_enabled)
    {
        return OAL_FALSE;
    }

    return  pst_device->en_dbac_running;
}

#ifdef _PRE_WLAN_FEATURE_DBAC

OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_need_enqueue_tid_for_dbac(mac_device_stru *pst_device, mac_vap_stru *pst_vap)
{
    return  (OAL_TRUE == pst_device->en_dbac_enabled) && (MAC_VAP_STATE_PAUSE == pst_vap->en_vap_state) ? OAL_TRUE : OAL_FALSE;
}
#endif
#ifdef _PRE_SUPPORT_ACS

OAL_STATIC  OAL_INLINE  en_mac_acs_sw_enum_uint8 mac_get_acs_switch(mac_device_stru *pst_mac_device)
{
    if (pst_mac_device->pst_acs == OAL_PTR_NULL)
    {
        return MAC_ACS_SW_NONE;
    }

    return pst_mac_device->st_acs_switch.en_acs_switch;
}

OAL_STATIC  OAL_INLINE  oal_void mac_set_acs_switch(mac_device_stru *pst_mac_device, en_mac_acs_sw_enum_uint8 en_switch)
{
    if (pst_mac_device->pst_acs == OAL_PTR_NULL)
    {
        return;
    }

    pst_mac_device->st_acs_switch.en_acs_switch = en_switch;
}
#endif

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_get_2040bss_switch(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->en_2040bss_switch;
}
OAL_STATIC  OAL_INLINE  oal_void mac_set_2040bss_switch(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_switch)
{
    pst_mac_device->en_2040bss_switch = en_switch;
}
#endif

#if IS_DEVICE
OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_device_is_scaning(mac_device_stru *pst_mac_device)
{
    return (pst_mac_device->en_curr_scan_state == MAC_SCAN_STATE_RUNNING)?OAL_TRUE:OAL_FALSE;
}

OAL_STATIC  OAL_INLINE  oal_bool_enum_uint8 mac_device_is_listening(mac_device_stru *pst_mac_device)
{
    return ((pst_mac_device->en_curr_scan_state == MAC_SCAN_STATE_RUNNING)
            && (pst_mac_device->st_scan_params.uc_scan_func & MAC_SCAN_FUNC_P2P_LISTEN))?OAL_TRUE:OAL_FALSE;
}
#endif /* IS_DEVICE */

/*****************************************************************************
  10 ????????
*****************************************************************************/
/*****************************************************************************
  10.1 ??????????????????????
*****************************************************************************/
extern oal_uint32  mac_device_init(mac_device_stru *pst_mac_device, oal_uint32 ul_chip_ver, oal_uint8 chip_id, oal_uint8 uc_device_id);
extern oal_uint32  mac_chip_init(mac_chip_stru *pst_chip, oal_uint8 uc_chip_id);
extern oal_uint32  mac_board_init(mac_board_stru *pst_board);

extern oal_uint32  mac_device_exit(mac_device_stru *pst_device);
extern oal_uint32  mac_chip_exit(mac_board_stru *pst_board, mac_chip_stru *pst_chip);
extern oal_uint32  mac_board_exit(mac_board_stru *pst_board);


/*****************************************************************************
  10.2 ????????????????
*****************************************************************************/
extern oal_void mac_device_set_vap_id(mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap,  oal_uint8 uc_vap_idx, wlan_vap_mode_enum_uint8 en_vap_mode, wlan_p2p_mode_enum_uint8 en_p2p_mode, oal_uint8 is_add_vap);
extern oal_void mac_device_set_dfr_reset(mac_device_stru *pst_mac_device, oal_uint8 uc_device_reset_in_progress);
extern oal_void mac_device_set_state(mac_device_stru *pst_mac_device, oal_uint8 en_device_state);

extern oal_void mac_device_set_channel(mac_device_stru *pst_mac_device, mac_cfg_channel_param_stru * pst_channel_param);
extern oal_void mac_device_get_channel(mac_device_stru *pst_mac_device, mac_cfg_channel_param_stru * pst_channel_param);

extern oal_void mac_device_set_txchain(mac_device_stru *pst_mac_device, oal_uint8 uc_tx_chain);
extern oal_void mac_device_set_rxchain(mac_device_stru *pst_mac_device, oal_uint8 uc_rx_chain);
extern oal_void mac_device_set_beacon_interval(mac_device_stru *pst_mac_device, oal_uint32 ul_beacon_interval);
extern oal_void mac_device_inc_active_user(mac_device_stru *pst_mac_device);

extern oal_void mac_device_dec_active_user(mac_device_stru *pst_mac_device);
#if 0
extern oal_void mac_device_inc_assoc_user(mac_device_stru *pst_mac_device);
extern oal_void mac_device_dec_assoc_user(mac_device_stru *pst_mac_device);
extern oal_void mac_device_set_dfs(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_dfs_switch, oal_uint8 uc_debug_level);
#endif
extern oal_void * mac_device_get_all_rates(mac_device_stru *pst_dev);
#ifdef _PRE_WLAN_FEATURE_HILINK
extern oal_uint32  mac_device_clear_fbt_scan_list(mac_device_stru *pst_mac_dev, oal_uint8 *puc_param);
extern oal_uint32  mac_device_set_fbt_scan_sta(mac_device_stru *pst_mac_dev, mac_fbt_scan_sta_addr_stru *pst_fbt_scan_sta);
extern oal_uint32  mac_device_set_fbt_scan_interval(mac_device_stru *pst_mac_dev, oal_uint32 ul_scan_interval);
extern oal_uint32  mac_device_set_fbt_scan_channel(mac_device_stru *pst_mac_dev, oal_uint8 uc_fbt_scan_channel);
extern oal_uint32  mac_device_set_fbt_scan_report_period(mac_device_stru *pst_mac_dev, oal_uint32 ul_fbt_scan_report_period);
extern oal_uint32  mac_device_set_fbt_scan_enable(mac_device_stru *pst_mac_device, oal_uint8 uc_cfg_fbt_scan_enable);
#endif


/*****************************************************************************
  10.3 ????????????
*****************************************************************************/
#if 0
#ifdef _PRE_WLAN_FEATURE_DBAC
extern oal_uint32  mac_dbac_update_chl_config(mac_device_stru *pst_mac_device, mac_channel_stru *pst_chl);
#endif
#endif

extern oal_uint32  mac_device_find_up_vap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
extern mac_vap_stru * mac_device_find_another_up_vap(mac_device_stru *pst_mac_device, oal_uint8 uc_vap_id_self);
extern oal_uint32  mac_device_find_up_ap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
extern oal_uint32  mac_device_calc_up_vap_num(mac_device_stru *pst_mac_device);
extern oal_uint32 mac_device_calc_work_vap_num(mac_device_stru *pst_mac_device);
extern oal_uint32  mac_device_find_up_p2p_go(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);
extern oal_uint32  mac_device_find_2up_vap(
                mac_device_stru *pst_mac_device,
                mac_vap_stru   **ppst_mac_vap1,
                mac_vap_stru   **ppst_mac_vap2);
extern oal_uint32  mac_fcs_dbac_state_check(mac_device_stru *pst_mac_device);
extern oal_uint32  mac_device_find_up_sta(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap);

extern oal_uint32  mac_device_is_p2p_connected(mac_device_stru *pst_mac_device);
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
extern mac_vap_stru *mac_find_main_proxysta(mac_device_stru *pst_mac_device);
#endif
/*****************************************************************************
  10.4 ??????
*****************************************************************************/

/*****************************************************************************
  11 inline????????
*****************************************************************************/

OAL_STATIC OAL_INLINE oal_bool_enum_uint8  mac_is_hide_ssid(oal_uint8 *puc_ssid_ie, oal_uint8 uc_ssid_len)
{
    return ((OAL_PTR_NULL == puc_ssid_ie) || (0 == uc_ssid_len) || ('\0' == puc_ssid_ie[0]));
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8  mac_device_is_auto_chan_sel_enabled(mac_device_stru *pst_mac_device)
{
    /* BSS?????????????????????????????????????????????????????? */
    if (0 == pst_mac_device->uc_max_channel)
    {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


#ifdef _PRE_WLAN_FEATURE_DFS

OAL_STATIC OAL_INLINE oal_void  mac_dfs_set_cac_enable(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_val)
{
    pst_mac_device->st_dfs.st_dfs_info.en_cac_switch = en_val;
}


OAL_STATIC OAL_INLINE oal_void  mac_dfs_set_offchan_cac_enable(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_val)
{
    pst_mac_device->st_dfs.st_dfs_info.en_offchan_cac_switch = en_val;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8  mac_dfs_get_offchan_cac_enable(mac_device_stru *pst_mac_device)
{
    mac_regdomain_info_stru   *pst_rd_info;

    mac_get_regdomain_info(&pst_rd_info);
    if(MAC_DFS_DOMAIN_ETSI == pst_rd_info->en_dfs_domain)
    {
        return pst_mac_device->st_dfs.st_dfs_info.en_offchan_cac_switch;
    }

    return OAL_FALSE;
}



OAL_STATIC OAL_INLINE oal_void  mac_dfs_set_offchan_number(mac_device_stru *pst_mac_device, oal_uint32 ul_val)
{
    pst_mac_device->st_dfs.st_dfs_info.uc_offchan_num = ul_val;
}



OAL_STATIC OAL_INLINE oal_bool_enum_uint8  mac_dfs_get_cac_enable(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->st_dfs.st_dfs_info.en_cac_switch;
}


OAL_STATIC OAL_INLINE oal_void  mac_dfs_set_dfs_enable(mac_device_stru *pst_mac_device, oal_bool_enum_uint8 en_val)
{
    pst_mac_device->st_dfs.st_dfs_info.en_dfs_switch = en_val;

    /* ???? ???????????????? ????????????CAC???? */
    if (OAL_FALSE == en_val)
    {
        pst_mac_device->st_dfs.st_dfs_info.en_cac_switch = OAL_FALSE;
    }
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8  mac_dfs_get_dfs_enable(mac_device_stru *pst_mac_device)
{
    if (WLAN_BAND_5G == pst_mac_device->en_max_band)
    {
        return pst_mac_device->st_dfs.st_dfs_info.en_dfs_switch;
    }

    return OAL_FALSE;
}


OAL_STATIC OAL_INLINE oal_void  mac_dfs_set_debug_level(mac_device_stru *pst_mac_device, oal_uint8 uc_debug_lev)
{
    pst_mac_device->st_dfs.st_dfs_info.uc_debug_level = uc_debug_lev;
}


OAL_STATIC OAL_INLINE oal_uint8  mac_dfs_get_debug_level(mac_device_stru *pst_mac_device)
{
    return pst_mac_device->st_dfs.st_dfs_info.uc_debug_level;
}
#endif

extern  oal_uint32  mac_fcs_init(mac_fcs_mgr_stru *pst_fcs_mgr,
                                  oal_uint8        uc_chip_id,
                                  oal_uint8        uc_device_id);

extern  mac_fcs_err_enum_uint8  mac_fcs_request(mac_fcs_mgr_stru           *pst_fcs_mgr,
                                                mac_fcs_state_enum_uint8   *puc_state,
                                                mac_fcs_cfg_stru           *pst_fcs_cfg);

extern  void    mac_fcs_release(mac_fcs_mgr_stru *pst_fcs_mgr);

extern  mac_fcs_err_enum_uint8    mac_fcs_start(mac_fcs_mgr_stru *pst_fcs_mgr,
                                                mac_fcs_cfg_stru *pst_fcs_cfg,
                                                hal_one_packet_status_stru *pst_status,
                                                oal_uint8 uc_fake_tx_q_id);

extern mac_fcs_err_enum_uint8    mac_fcs_start_same_channel(
                mac_fcs_mgr_stru            *pst_fcs_mgr,
                mac_fcs_cfg_stru            *pst_fcs_cfg,
                hal_one_packet_status_stru  *pst_status,
                oal_uint8                    uc_fake_tx_q_id);
extern mac_fcs_err_enum_uint8    mac_fcs_start_enhanced_same_channel(
                mac_fcs_mgr_stru            *pst_fcs_mgr,
                mac_fcs_cfg_stru            *pst_fcs_cfg);

extern mac_fcs_err_enum_uint8    mac_fcs_start_enhanced(
                mac_fcs_mgr_stru            *pst_fcs_mgr,
                mac_fcs_cfg_stru            *pst_fcs_cfg);
extern oal_void mac_fcs_send_one_packet_start(mac_fcs_mgr_stru *pst_fcs_mgr,
                                       hal_one_packet_cfg_stru *pst_one_packet_cfg,
                                       hal_to_dmac_device_stru *pst_device,
                                    hal_one_packet_status_stru *pst_status,
                                           oal_bool_enum_uint8  en_ps);
extern  oal_uint32 mac_fcs_notify_chain_register (mac_fcs_mgr_stru              *pst_fcs_mgr,
                                                  mac_fcs_notify_type_enum_uint8 uc_notify_type,
                                                  mac_fcs_hook_id_enum_uint8     en_hook_id,
                                                  mac_fcs_notify_func            p_func);

extern  oal_uint32  mac_fcs_notify(mac_fcs_mgr_stru               *pst_fcs_mgr,
                                   mac_fcs_notify_type_enum_uint8  uc_notify_type);

extern  oal_uint32 mac_fcs_notify_chain_unregister(mac_fcs_mgr_stru              *pst_fcs_mgr,
                                                   mac_fcs_notify_type_enum_uint8 uc_notify_type,
                                                   mac_fcs_hook_id_enum_uint8     en_hook_id);

extern  oal_uint32  mac_fcs_notify_chain_destroy(mac_fcs_mgr_stru *pst_fcs_mgr);

extern oal_uint32  mac_fcs_get_prot_mode(mac_vap_stru *pst_src_vap);
extern oal_uint32  mac_fcs_get_prot_datarate(mac_vap_stru *pst_src_vap);
extern oal_void  mac_fcs_prepare_one_packet_cfg(
                mac_vap_stru                *pst_mac_vap,
                hal_one_packet_cfg_stru     *pst_one_packet_cfg,
                oal_uint16                   us_protect_time);


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_fcs_is_same_channel(mac_channel_stru *pst_channel_dst,
                                                                  mac_channel_stru *pst_channel_src)
{
    return  pst_channel_dst->uc_chan_number == pst_channel_src->uc_chan_number ? OAL_TRUE : OAL_FALSE;
}


OAL_STATIC  OAL_INLINE  hal_fcs_protect_type_enum_uint8 mac_fcs_get_protect_type(
                        mac_vap_stru *pst_mac_vap)
{
    hal_fcs_protect_type_enum_uint8 en_protect_type;

    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        en_protect_type = HAL_FCS_PROTECT_TYPE_SELF_CTS;
    }
    else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NULL_DATA;
    }
    else
    {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NONE;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    if (WLAN_P2P_GO_MODE == pst_mac_vap->en_p2p_mode)
    {
        en_protect_type = HAL_FCS_PROTECT_TYPE_NONE;
    }
#endif

    return en_protect_type;
}


OAL_STATIC  OAL_INLINE oal_uint8  mac_fcs_get_protect_cnt(mac_vap_stru *pst_mac_vap)
{
    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        return HAL_FCS_PROTECT_CNT_1;
    }

    return HAL_FCS_PROTECT_CNT_3;
}

#if (_PRE_TEST_MODE_BOARD_ST == _PRE_TEST_MODE)
extern  oal_void mac_fcs_verify_init(oal_void);
extern  oal_void mac_fcs_verify_start(oal_void);
extern  oal_void mac_fcs_verify_timestamp(mac_fcs_stage_enum_uint8 en_stage);
extern  oal_void mac_fcs_verify_stop(oal_void);

#else
#define          mac_fcs_verify_init()
#define          mac_fcs_verify_start()
#define          mac_fcs_verify_timestamp(a)
#define          mac_fcs_verify_stop()
#endif  // _PRE_DEBUG_MODE

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of mac_device.h */
