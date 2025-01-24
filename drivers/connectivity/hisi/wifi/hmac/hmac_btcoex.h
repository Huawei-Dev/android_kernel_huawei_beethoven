

#ifndef __HMAC_BTCOEX_H__
#define __HMAC_BTCOEX_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif

#ifdef _PRE_WLAN_FEATURE_BTCOEX

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "frw_ext_if.h"
#include "oal_ext_if.h"
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define MAX_BTCOEX_BSS_IN_BL 16

/*****************************************************************************
  3 ????????
*****************************************************************************/


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
    oal_uint16 us_last_baw_start;       /* ????????????ADDBA REQ????baw_start?? */
    oal_uint16 us_last_seq_num;         /* ????????????ADDBA REQ????seq_num?? */
    oal_uint8 uc_delba_enable;          /* ????????????????BA */
    oal_uint8 auc_resv[3];
} hmac_btcoex_addba_req_stru;

typedef struct
{
    oal_uint8 auc_user_mac_addr[WLAN_MAC_ADDR_LEN];     /* ??????MAC???? */
    oal_uint8 uc_type;                                  /* ???????????????? */
    oal_uint8 uc_used;                                  /* ??????????????????MAC???? */
} hmac_btcoex_delba_exception_stru;

typedef struct
{
    frw_timeout_stru st_delba_opt_timer;    /* ????ARP REQ???????????? */
    oal_atomic ul_rx_unicast_pkt_to_lan;    /* ?????????????????? */
} hmac_btcoex_arp_req_process_stru;

typedef struct
{
    hmac_btcoex_delba_exception_stru ast_hmac_btcoex_delba_exception[MAX_BTCOEX_BSS_IN_BL];
    oal_uint8 uc_exception_bss_index;       /* ??????MAC?????????????? */
    oal_uint8 auc_resv[3];
} hmac_device_btcoex_stru;

typedef struct
{
    hmac_btcoex_arp_req_process_stru st_hmac_btcoex_arp_req_process;
    hmac_btcoex_addba_req_stru st_hmac_btcoex_addba_req;
    oal_uint16 us_ba_size;
    oal_uint8 uc_rx_no_pkt_count;           /* ?????????????????????????? */
    oal_uint8 auc_resv[1];
} hmac_user_btcoex_stru;
/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/


#endif /* #ifdef _PRE_WLAN_FEATURE_COEXIST_BT */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of __HMAC_BTCOEX_H__ */

