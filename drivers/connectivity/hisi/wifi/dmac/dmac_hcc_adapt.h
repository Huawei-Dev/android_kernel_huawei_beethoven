

#ifndef __DMAC_HCC_ADAPT_H__
#define __DMAC_HCC_ADAPT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oam_ext_if.h"
#include "dmac_ext_if.h"
#include "frw_ext_if.h"
#include "frw_event_main.h"



#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_HCC_ADAPT_H

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

/*Rx????????*/
extern frw_event_mem_stru* dmac_process_rx_data_event_adapt_default(frw_event_mem_stru * pst_hcc_event_mem);
extern frw_event_mem_stru* dmac_hcc_rx_convert_netbuf_to_event_default(frw_event_mem_stru * pst_hcc_event_mem);
extern frw_event_mem_stru* dmac_event_config_syn_alg_rx_adapt(frw_event_mem_stru * pst_hcc_event_mem);
extern frw_event_mem_stru* dmac_scan_proc_scan_req_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem);
#ifdef _PRE_WLAN_RF_110X_CALI_DPD
extern  frw_event_mem_stru* dmac_dpd_data_processed_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem);
#endif
extern frw_event_mem_stru*   dmac_cali_hmac2dmac_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem);
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
extern frw_event_mem_stru* dmac_config_update_ip_filter_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem);
#endif //_PRE_WLAN_FEATURE_IP_FILTER
extern frw_event_mem_stru* dmac_scan_proc_sched_scan_req_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem);

/*Tx????????*/
extern oal_uint32 dmac_proc_wlan_drx_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_tkip_mic_fail_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_crx_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_scan_report_scanned_bss_tx_adapt(frw_event_mem_stru *pst_event_mem);
#ifdef _PRE_WLAN_RF_110X_CALI_DPD
extern oal_uint32 dmac_dpd_data_cali_tx_adapt(frw_event_mem_stru *pst_event_mem);
#endif
extern oal_uint32 dmac_scan_proc_scan_comp_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_chan_result_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_init_event_process_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_proc_event_del_ba_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_hcc_tx_convert_event_to_netbuf_uint16(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_event_config_syn_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_alg_ct_result_tx_adapt(frw_event_mem_stru *pst_event_mem);
//extern oal_uint32 dmac_proc_event_log_syn_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_mgmt_rx_delba_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_rx_send_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_switch_to_new_chan_complete_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_dbac_status_notify_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_proc_disasoc_misc_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32 dmac_cali2hmac_misc_event_tx_adapt(frw_event_mem_stru *pst_event_mem);

#ifdef _PRE_WLAN_FEATURE_ROAM
extern oal_uint32 dmac_proc_roam_trigger_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
#endif //_PRE_WLAN_FEATURE_ROAM

extern oal_uint32  dmac_alg_syn_info_adapt(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_alg_voice_aggr_adapt(frw_event_mem_stru *pst_event_mem);

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
extern oal_uint32 dmac_scan_proc_obss_scan_comp_event_tx_adapt(frw_event_mem_stru *pst_event_mem);
#endif

#ifdef _PRE_WLAN_FEATURE_FLOWCTL
extern oal_uint32  dmac_alg_flow_tx_adapt_tx_adapt(frw_event_mem_stru * pst_event_mem);
#endif

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_main */


