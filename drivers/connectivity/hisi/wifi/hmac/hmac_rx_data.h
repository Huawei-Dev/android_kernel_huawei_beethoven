

#ifndef __HMAC_RX_DATA_H__
#define __HMAC_RX_DATA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "mac_frame.h"
#include "dmac_ext_if.h"
#include "hmac_user.h"
#include "oal_net.h"
#include "hmac_tcp_opt.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_resource.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DATA_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define HMAC_RX_DATA_ETHER_OFFSET_LENGTH      6   /* ????????lan????????????????????????????????????6 */
#define HMAC_RX_SMALL_PKT_MIPS_LIMIT        30000L /* rx????MIPS????????????30us */

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
/* HMAC??????????????????????????????????????netbuf??CB????????????????48???? */
typedef struct
{
    mac_rx_ctl_stru    st_rx_info;
}hmac_rx_ctl_stru;

/* HMAC ???? TKIP MIC FAILE ?????????????????????? */
typedef struct
{
    oal_uint8            uc_vap_id;     /* ?????????????? vap_id */
    oal_uint8            uc_rsv[3];
    frw_timeout_stru    *pst_timeout;   /* MIC faile ?????????????? */
}hmac_drx_mic_faile_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32  hmac_rx_process_data_ap(frw_event_mem_stru *pst_event_mem);
extern oal_void  hmac_rx_process_data_ap_tcp_ack_opt(hmac_vap_stru *pst_vap,oal_netbuf_head_stru* pst_netbuf_header);
extern oal_uint32  hmac_rx_process_data_sta_tcp_ack_opt(hmac_vap_stru *pst_vap,oal_netbuf_head_stru* pst_netbuf_header);
extern oal_uint32  hmac_rx_process_data_sta(frw_event_mem_stru *pst_event_mem);
#ifdef _PRE_WLAN_DFT_DUMP_FRAME
extern oal_void  hmac_rx_report_eth_frame(mac_vap_stru   *pst_mac_vap,
                                                      oal_netbuf_stru *pst_netbuf);
#endif
extern oal_void  hmac_rx_free_netbuf(oal_netbuf_stru *pst_netbuf, oal_uint16 us_nums);
extern oal_void  hmac_rx_free_netbuf_list(
                oal_netbuf_head_stru       *pst_netbuf_hdr,
                oal_uint16                  us_nums_buf);

extern oal_uint32 hmac_rx_lan_frame(oal_netbuf_head_stru *pst_netbuf_header);
#ifdef _PRE_WLAN_FEATURE_PKT_MEM_OPT
extern oal_void hmac_pkt_mem_opt_init(hmac_device_stru *pst_hmac_device);
extern oal_void hmac_pkt_mem_opt_exit(hmac_device_stru *pst_hmac_device);
extern oal_void hmac_pkt_mem_opt_cfg(oal_uint32 ul_cfg_tpye, oal_uint32 ul_cfg_value);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_rx_data.h */
