

#ifndef __DMAC_WMMAC_H__
#define __DMAC_WMMAC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "oal_ext_if.h"
#include "wlan_spec.h"
#include "wlan_mib.h"
#include "frw_ext_if.h"
#include "hal_ext_if.h"
#include "mac_regdomain.h"
#include "mac_ie.h"
#include "mac_frame.h"
#include "dmac_tx_bss_comm.h"
#include "dmac_mgmt_bss_comm.h"
#include "dmac_mgmt_sta.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_WMMAC_H
/*****************************************************************************
  2 ??????
*****************************************************************************/


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
extern oal_uint32 dmac_mgmt_tx_addts_req(dmac_vap_stru *pst_dmac_vap, dmac_ctx_action_event_stru *pst_ctx_action_event, oal_netbuf_stru *pst_net_buff);
extern oal_uint32 dmac_mgmt_tx_addts_rsp(dmac_vap_stru *pst_dmac_vap, dmac_ctx_action_event_stru *pst_ctx_action_event, oal_netbuf_stru *pst_net_buff);
extern oal_uint32 dmac_mgmt_tx_delts(dmac_vap_stru *pst_dmac_vap, dmac_ctx_action_event_stru *pst_ctx_action_event, oal_netbuf_stru *pst_net_buff);
extern oal_uint32 dmac_mgmt_rx_addts_rsp(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, oal_uint8 *puc_payload);
extern oal_uint32 dmac_mgmt_rx_delts(dmac_vap_stru *pst_dmac_vap,dmac_user_stru *pst_dmac_user,oal_uint8 *puc_payload);

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_wmmac.h */
