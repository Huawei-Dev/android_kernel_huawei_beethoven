

#ifndef __HMAC_EDCA_OPT_H__
#define __HMAC_EDCA_OPT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "dmac_ext_if.h"
#include "oam_ext_if.h"
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define HMAC_EDCA_OPT_MIN_PKT_LEN   256                                 /* ????????????ip??????????????????chariot????????*/

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
#define HMAC_EDCA_OPT_TIME_MS       1000                                /* edca?????????????????? */
#else
#define HMAC_EDCA_OPT_TIME_MS       30000                               /* edca?????????????????? */
#endif

#define HMAC_EDCA_OPT_PKT_NUM       ((HMAC_EDCA_OPT_TIME_MS) >> 3)      /* ?????????????????? */

#define WLAN_EDCA_OPT_MAX_WEIGHT_STA    (3)
#define WLAN_EDCA_OPT_WEIGHT_STA        (2)
/*****************************************************************************
  3 ????????
*****************************************************************************/
typedef enum
{
    WLAN_TX_TCP_DATA = 0,    /* ????TCP data */
    WLAN_RX_TCP_DATA = 1,    /* ????TCP data */
    WLAN_TX_UDP_DATA = 2,    /* ????UDP data */
    WLAN_RX_UDP_DATA = 3,    /* ????UDP data */

    WLAN_TXRX_DATA_BUTT = 4,
}wlan_txrx_data_type_enum;
typedef oal_uint8 wlan_txrx_data_enum_uint8;


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


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_void     hmac_edca_opt_rx_pkts_stat(oal_uint16 us_assoc_id, oal_uint8 uc_tidno, mac_ip_header_stru *pst_ip);
extern oal_void     hmac_edca_opt_tx_pkts_stat(mac_tx_ctl_stru  *pst_tx_ctl, oal_uint8 uc_tidno, mac_ip_header_stru *pst_ip);
extern oal_uint32   hmac_edca_opt_timeout_fn(oal_void *p_arg);
#if 0//????????????????????????-wanran
extern oal_void     hmac_edca_opt_adj_param_sta(oal_void *pst_void);
#endif

#endif   /* end of _PRE_WLAN_FEATURE_EDCA_OPT_AP */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_edca_opt.h */
