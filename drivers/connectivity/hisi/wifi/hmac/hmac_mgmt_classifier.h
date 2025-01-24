

#ifndef __HMAC_MGMT_CLASSIFIER_H__
#define __HMAC_MGMT_CLASSIFIER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "frw_ext_if.h"
#include "dmac_ext_if.h"
#include "hmac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_CLASSIFIER_H
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
extern oal_uint32  hmac_rx_process_mgmt_event(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  hmac_mgmt_rx_delba_event(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  hmac_mgmt_tx_action(
                hmac_vap_stru              *pst_hmac_vap,
                hmac_user_stru             *pst_hmac_user,
                mac_action_mgmt_args_stru  *pst_action_args);
extern oal_uint32  hmac_mgmt_tx_priv_req(
                hmac_vap_stru              *pst_hmac_vap,
                hmac_user_stru             *pst_hmac_user,
                mac_priv_req_args_stru     *pst_priv_req);
extern oal_uint32  hmac_mgmt_tbtt_event(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 hmac_mgmt_send_disasoc_deauth_event(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  hmac_proc_disasoc_misc_event(frw_event_mem_stru *pst_event_mem);

#ifdef _PRE_WLAN_FEATURE_ROAM
extern oal_uint32  hmac_proc_roam_trigger_event(frw_event_mem_stru *pst_event_mem);
#endif  //_PRE_WLAN_FEATURE_ROAM

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_mgmt_classifier.h */
