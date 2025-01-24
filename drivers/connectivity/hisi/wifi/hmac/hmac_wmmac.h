

#ifndef __HMAC_WMMAC_H__
#define __HMAC_WMMAC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC
/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "hmac_device.h"
#include "mac_resource.h"
#include "hmac_resource.h"
#include "mac_vap.h"
#include "mac_ie.h"
#include "dmac_ext_if.h"
#include "hmac_ext_if.h"
#include "hmac_fsm.h"
#include "hmac_main.h"
#include "hmac_vap.h"


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WMMAC_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define WLAN_ADDTS_TIMEOUT                      500


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


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32 hmac_mgmt_tx_addts_rsp(hmac_vap_stru *pst_hmac_vap,hmac_user_stru *pst_hmac_user, hmac_ba_rx_stru *pst_ba_rx_info, oal_uint8 uc_tid, oal_uint8 uc_status);
extern oal_uint32 hmac_mgmt_rx_addts_rsp(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
                                         oal_uint8 *puc_payload, oal_uint32 frame_body_len);
extern oal_uint32 hmac_mgmt_rx_delts(hmac_vap_stru *pst_hmac_vap,hmac_user_stru *pst_hmac_user,
                                     oal_uint8 *puc_payload, oal_uint32 frame_body_len);
extern oal_uint32 hmac_mgmt_rx_addts_req_frame(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
extern oal_uint32  hmac_config_addts_req(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
extern oal_uint32  hmac_config_delts(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
extern oal_uint32  hmac_config_reassoc_req(mac_vap_stru *pst_mac_vap);
extern oal_uint32  hmac_config_wmmac_switch(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);

#endif



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_wmmac.h */
