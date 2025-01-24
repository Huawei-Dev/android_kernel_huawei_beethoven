/******************************************************************************

                  ???????????? (C), 2001-2011, ????????????????????????

 ******************************************************************************
  ??? ??? ???   : dmac_chan_mgmt.h
  ??? ??? ???   : ??????
  ???    ???   : mayuan
  ????????????   : 2014???2???22???
  ????????????   :
  ????????????   : dmac_chan_mgmt.c ????????????
  ????????????   :
  ????????????   :
  1.???    ???   : 2014???2???22???
    ???    ???   : mayuan
    ????????????   : ????????????

******************************************************************************/

#ifndef __DMAC_CHAN_MGMT_H__
#define __DMAC_CHAN_MGMT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ?????????????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "dmac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_CHAN_MGMT_H
/*****************************************************************************
  2 ?????????
*****************************************************************************/


/*****************************************************************************
  3 ????????????
*****************************************************************************/


/*****************************************************************************
  4 ??????????????????
*****************************************************************************/


/*****************************************************************************
  5 ???????????????
*****************************************************************************/


/*****************************************************************************
  6 ????????????
*****************************************************************************/


/*****************************************************************************
  7 STRUCT??????
*****************************************************************************/


/*****************************************************************************
  8 UNION??????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS??????
*****************************************************************************/


/*****************************************************************************
  10 ????????????
*****************************************************************************/
extern oal_uint32  dmac_chan_initiate_switch_to_new_channel(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_chan_sync(frw_event_mem_stru *pst_event_mem);
extern oal_void  dmac_chan_attempt_new_chan(dmac_vap_stru *pst_dmac_vap, oal_uint8 uc_channel, wlan_channel_bandwidth_enum_uint8 en_bandwidth);
extern oal_void dmac_chan_select_real_channel(mac_device_stru  *pst_mac_device, mac_channel_stru *pst_channel);
extern oal_void  dmac_chan_select_channel_mac(mac_vap_stru *pst_mac_vap, oal_uint8 uc_channel, wlan_channel_bandwidth_enum_uint8 en_bandwidth);
extern oal_uint32  dmac_chan_disable_machw_tx_event_process(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_chan_enable_machw_tx_event_process(frw_event_mem_stru *pst_event_mem);
extern oal_void  dmac_chan_tx_complete_2040_coexist(mac_device_stru *pst_mac_device, hal_to_dmac_device_stru *pst_hal_device, oal_netbuf_stru *pst_netbuf);
extern oal_uint32  dmac_chan_restart_network_after_switch_event(frw_event_mem_stru *pst_event_mem);
extern oal_void  dmac_chan_tx_complete_suspend_tx(mac_device_stru      *pst_mac_device,
                                             mac_vap_stru              *pst_mac_vap,
                                             hal_to_dmac_device_stru   *pst_hal_device,
                                             oal_netbuf_stru           *pst_netbuf);
extern oal_void  dmac_chan_disable_machw_tx(mac_vap_stru *pst_mac_vap);
extern oal_void  dmac_chan_enable_machw_tx(mac_vap_stru *pst_mac_vap);
extern oal_uint32 dmac_dfs_radar_detect_event(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_dfs_test(frw_event_mem_stru* pst_event_mem);
extern oal_void  dmac_switch_complete_notify(mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_check_cac);
#ifdef _PRE_WLAN_FEATURE_OFFCHAN_CAC
extern oal_uint32  dmac_dfs_switch_to_offchan_event_process(frw_event_mem_stru* pst_event_mem);
extern oal_uint32  dmac_dfs_switch_back_event_process(frw_event_mem_stru* pst_event_mem);
#endif
extern oal_void  dmac_chan_update_user_bandwidth(mac_vap_stru *pst_mac_vap);

extern oal_uint32 dmac_sta_up_update_vht_params(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_payload, oal_uint16 us_frame_len, mac_user_stru *pst_mac_user);

extern oal_uint32  dmac_ie_proc_wide_bandwidth_ie(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_payload);

extern oal_uint32  dmac_ie_proc_ch_switch_ie(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_payload, mac_eid_enum_uint8 en_eid_type);
extern oal_void    dmac_chan_update_csw_info(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_payload, oal_uint16 us_frame_len);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_chan_mgmt.h */
