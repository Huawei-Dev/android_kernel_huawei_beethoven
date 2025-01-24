

#ifndef __HMAC_ROAM_ALG_H__
#define __HMAC_ROAM_ALG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_ROAM

/*****************************************************************************
  1 ??????????????
*****************************************************************************/


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_ALG_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define ROAM_LIST_MAX                     (4)          /* ???????? */
#define ROAM_BLACKLIST_NORMAL_AP_TIME_OUT (30000)      /* ?????????????????? */
#define ROAM_BLACKLIST_REJECT_AP_TIME_OUT (30000)      /* ?????????????????????? */
#define ROAM_BLACKLIST_COUNT_LIMIT        (1)           /* ?????????????????? */

#define ROAM_HISTORY_BSS_TIME_OUT         (20000)       /* ???????????????????? */
#define ROAM_HISTORY_COUNT_LIMIT          (1)           /* ???????????????????? */
#define ROAM_RSSI_LEVEL                   (3)
#define ROAM_CONCURRENT_USER_NUMBER       (10)
#define ROAM_THROUGHPUT_THRESHOLD         (1000)

#define ROAM_RSSI_NE80_DB                 (-80)
#define ROAM_RSSI_NE75_DB                 (-75)
#define ROAM_RSSI_NE70_DB                 (-70)

#define ROAM_RSSI_DIFF_4_DB               (4)
#define ROAM_RSSI_DIFF_6_DB               (6)
#define ROAM_RSSI_DIFF_8_DB               (8)
#define ROAM_RSSI_DIFF_10_DB              (10)

#define ROAM_RSSI_CMD_TYPE                (-128)
#define ROAM_RSSI_LINKLOSS_TYPE           (-121)
#define ROAM_RSSI_MAX_TYPE                (-126)


/*****************************************************************************
  3 ????????
*****************************************************************************/
/* ?????????????? */
typedef enum
{
    ROAM_BLACKLIST_TYPE_NORMAL_AP       = 0,
    ROAM_BLACKLIST_TYPE_REJECT_AP       = 1,
    ROAM_BLACKLIST_TYPE_BUTT
}roam_blacklist_type_enum;
typedef oal_uint8  roam_blacklist_type_enum_uint8;

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
typedef struct
{
    oal_uint32                 ul_time_stamp;           /* ???????????????? */
    oal_uint32                 ul_timeout;              /* ?????????????????? */
    oal_uint16                 us_count_limit;          /* ?????????????????????????????? */
    oal_uint16                 us_count;                /* ????????????????????????Bssid?????? */
    oal_uint8                  auc_bssid[WLAN_MAC_ADDR_LEN];
}hmac_roam_bss_info_stru;

/* ????blacklist?????? */
typedef struct
{
    hmac_roam_bss_info_stru     ast_bss[ROAM_LIST_MAX];
}hmac_roam_bss_list_stru;
/* ?????????????? */
typedef struct
{
    hmac_roam_bss_list_stru      st_blacklist;          /* ??????????AP???????? */
    hmac_roam_bss_list_stru      st_history;            /* ????????AP???????? */
    oal_uint32                   ul_max_capacity;       /* ???? scan ?????????? capacity */
    mac_bss_dscr_stru           *pst_max_capacity_bss;  /* ???? scan ?????????? capacity ?? bss*/
    oal_int8                     c_current_rssi;        /* ???? dmac ???? rssi */
    oal_int8                     c_max_rssi;            /* ???? scan ?????????? rssi */
    oal_uint8                    uc_another_bss_scaned; /* ???????????????????????? bss */
    oal_uint8                    uc_invalid_scan_cnt;   /* ?????????????????????????????? bss ?????? */
    mac_bss_dscr_stru           *pst_max_rssi_bss;      /* ???? scan ?????????? rssi ?? bss */
}hmac_roam_alg_stru;

/* ????connect?????? */
typedef struct
{
    roam_connect_state_enum_uint8   en_state;
    oal_uint8                       uc_auth_num;
    oal_uint8                       uc_assoc_num;
    oal_uint8                       uc_ft_num;
    frw_timeout_stru                st_timer;           /* ????connect???????????? */
    mac_bss_dscr_stru              *pst_bss_dscr;
}hmac_roam_connect_stru;
typedef struct
{
    oal_int8                       c_rssi;
    oal_uint32                     ul_capacity_kbps;
} hmac_roam_rssi_capacity_stru;

/* ??bss?????????? */
typedef struct
{
    oal_uint8                       auc_bssid[WLAN_MAC_ADDR_LEN];
    oal_uint16                      us_sta_aid;
#ifdef _PRE_WLAN_FEATURE_TXBF
    mac_vap_txbf_add_stru           st_txbf_add_cap;
#endif
    mac_cap_flag_stru               st_cap_flag;
    mac_channel_stru                st_channel;
    wlan_mib_ieee802dot11_stru      st_mib_info;
    mac_user_cap_info_stru          st_cap_info;
    mac_key_mgmt_stru               st_key_info;
    mac_user_tx_param_stru          st_user_tx_info;    /* TX???????? */
    mac_rate_stru                   st_op_rates;
    mac_user_ht_hdl_stru            st_ht_hdl;
    mac_vht_hdl_stru                st_vht_hdl;
    wlan_bw_cap_enum_uint8          en_bandwidth_cap;
    wlan_bw_cap_enum_uint8          en_avail_bandwidth;
    wlan_bw_cap_enum_uint8          en_cur_bandwidth;
    wlan_protocol_enum_uint8        en_protocol_mode;
    wlan_protocol_enum_uint8        en_avail_protocol_mode;
    wlan_protocol_enum_uint8        en_cur_protocol_mode;
    oal_uint8                       uc_num_spatial_stream;
    oal_uint8                       uc_avail_num_spatial_stream;
    oal_uint8                       uc_cur_num_spatial_stream;
    oal_uint8                       uc_avail_bf_num_spatial_stream;
    oal_uint16                      us_cap_info;        /* ????bss???????????? */
}hmac_roam_old_bss_stru;

#if 0
/* ?????????????? */
typedef struct
{
    struct sk_buff_head         st_data_queue;
    struct workqueue_struct    *pst_wq;
    struct work_struct          st_work;
    struct hcc_handler         *pst_hcc;
}hmac_roam_buf_stru;
#endif
/* ???????????? */
typedef struct
{
    oal_uint8                       uc_enable;          /* ???????????? */
    roam_main_state_enum_uint8      en_main_state;      /* ?????????? */
    oal_uint8                       uc_rssi_ignore;     /* ????rssi???????? */
    oal_uint8                       uc_invalid_scan_cnt;/* ???????????? */
    hmac_vap_stru                  *pst_hmac_vap;       /* ??????????vap */
    hmac_user_stru                 *pst_hmac_user;      /* ??????????BSS user */
    hmac_roam_old_bss_stru          st_old_bss;         /* ??????????????bss???????? */
    mac_scan_req_stru               st_scan_params;     /* ???????????? */
    hmac_roam_config_stru           st_config;          /* ???????????????? */
    hmac_roam_connect_stru          st_connect;         /* ????connect???? */
    hmac_roam_alg_stru              st_alg;             /* ???????????? */
    hmac_roam_static_stru           st_static;          /* ???????????? */
    frw_timeout_stru                st_timer;           /* ???????????????? */
    wpas_connect_state_enum_uint32  ul_connected_state; /* ?????????????????? */
    oal_uint32                      ul_ip_addr_obtained;/* IP???????????? */
#if 0
    hmac_roam_buf_stru              st_buf;             /* ???????????? */
#endif
}hmac_roam_info_stru;
typedef oal_uint32  (*hmac_roam_fsm_func)(hmac_roam_info_stru *pst_roam_info, oal_void *p_param);


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
oal_uint32 hmac_roam_alg_add_blacklist(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid, roam_blacklist_type_enum_uint8 list_type);
oal_uint32 hmac_roam_alg_add_history(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid);
oal_uint32 hmac_roam_alg_bss_check(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr);
oal_uint32 hmac_roam_alg_scan_channel_init(hmac_roam_info_stru *pst_roam_info, mac_scan_req_stru *pst_scan_params);
oal_void hmac_roam_alg_init(hmac_roam_info_stru *pst_roam_info, oal_int8 c_current_rssi);
mac_bss_dscr_stru *hmac_roam_alg_select_bss(hmac_roam_info_stru *pst_roam_info);
oal_bool_enum_uint8 hmac_roam_alg_find_in_blacklist(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid);
oal_bool_enum_uint8 hmac_roam_alg_find_in_history(hmac_roam_info_stru *pst_roam_info, oal_uint8 *puc_bssid);
oal_bool_enum_uint8 hmac_roam_alg_need_to_stop_roam_trigger(hmac_roam_info_stru *pst_roam_info);
oal_uint32 hmac_roam_alg_bss_in_ess(hmac_roam_info_stru *pst_roam_info, mac_bss_dscr_stru *pst_bss_dscr);
#endif //_PRE_WLAN_FEATURE_ROAM

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_roam_alg.h */
