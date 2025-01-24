
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oal_net.h"
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#include "oal_schedule.h"
#endif
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "hal_ext_if.h"
#include "mac_regdomain.h"
#include "mac_resource.h"
#include "mac_device.h"
#include "mac_ie.h"
#include "dmac_scan.h"
#include "dmac_main.h"
#include "dmac_acs.h"
#include "dmac_tx_bss_comm.h"
#include "dmac_ext_if.h"
#include "dmac_device.h"
#include "dmac_mgmt_sta.h"
#include "dmac_alg.h"
#if defined(_PRE_WLAN_CHIP_TEST) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "dmac_scan_test.h"
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "dmac_sta_pm.h"
#include "pm_extern.h"
#endif
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#include "dmac_dft.h"
#endif
#include "hal_device.h"
#include "dmac_config.h"
#include "dmac_chan_mgmt.h"
#include "dmac_mgmt_classifier.h"
#ifdef _PRE_WLAN_FEATURE_11K
#include "dmac_11k.h"
#endif
#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_SCAN_C


/*****************************************************************************
  2 ????????????
*****************************************************************************/
/* ???????????? */

OAL_STATIC oal_uint32 dmac_scan_send_bcast_probe(mac_device_stru *pst_mac_device, oal_uint8 uc_band, oal_uint8  uc_index);

#if 0
#ifdef _PRE_WLAN_FEATURE_DBAC
OAL_STATIC oal_uint32  dmac_scan_register_scan_req_to_dbac(mac_device_stru *pst_mac_device);
#endif
#endif
OAL_STATIC oal_uint32  dmac_scan_report_channel_statistics_result(mac_device_stru *pst_mac_device, oal_uint8 uc_scan_idx);

OAL_STATIC oal_uint32  dmac_scan_switch_home_channel_work_timeout(void *p_arg);

OAL_STATIC oal_uint32  dmac_scan_start_pno_sched_scan_timer(void *p_arg);


/*****************************************************************************
  3 ????????
*****************************************************************************/
#if 0
OAL_STATIC oal_void dmac_scan_print_time_stamp()
{
    oal_uint32                  ul_timestamp;

    ul_timestamp = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_print_time_stamp:: time_stamp:%d.}", ul_timestamp);

    return;
}
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST

oal_void dmac_detect_2040_te_a_b(dmac_vap_stru *pst_dmac_vap, oal_uint8 *puc_frame_body, oal_uint16 us_frame_len, oal_uint16 us_offset,oal_uint8 uc_curr_chan)
{
    oal_uint8            chan_index     = 0;
    oal_bool_enum_uint8  ht_cap         = OAL_FALSE;
    oal_uint8            uc_real_chan   = uc_curr_chan;
    oal_uint8           *puc_ie         = OAL_PTR_NULL;
    mac_device_stru     *pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device)) {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_detect_2040_te_a_b::mac_res_get_dev return null.}");
        return;
    }

    if (us_frame_len <= us_offset) {
        mac_get_channel_idx_from_num((pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]).en_band,
                                      uc_real_chan, &chan_index);
        /* Detect Trigger Event - A */
        pst_dmac_vap->st_vap_base_info.st_ch_switch_info.ul_chan_report_for_te_a |= (1 << chan_index);

        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_detect_2040_te_a_b::framebody_len[%d]}", us_frame_len);
        return;
    }

    us_frame_len   -= us_offset;
    puc_frame_body += us_offset;

    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_frame_body, us_frame_len);
    if (OAL_PTR_NULL != puc_ie) {
        ht_cap = OAL_TRUE;

        /* Check for the Forty MHz Intolerant bit in HT-Capabilities */
        if((puc_ie[3] & BIT6) != 0)
        {
            //OAM_INFO_LOG0(0, OAM_SF_SCAN, "dmac_detect_2040_te_a_b::40 intolerant in ht cap");
            /* Register Trigger Event - B */
            pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_te_b = OAL_TRUE;
        }
    }

    puc_ie = mac_find_ie(MAC_EID_2040_COEXT, puc_frame_body, us_frame_len);
    if (OAL_PTR_NULL != puc_ie) {
        /* Check for the Forty MHz Intolerant bit in Coex-Mgmt IE */
        if((puc_ie[2] & BIT1) != 0)
        {
            //OAM_INFO_LOG0(0, OAM_SF_SCAN, "dmac_detect_2040_te_a_b::40 intolerant in co");
            /* Register Trigger Event - B */
            pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_te_b = OAL_TRUE;
        }

    }

    /* ??????HT??????False?????????????????????????????????????????? */
    if (OAL_FALSE == ht_cap) {
        puc_ie = mac_find_ie(MAC_EID_DSPARMS, puc_frame_body, us_frame_len);
        if (OAL_PTR_NULL != puc_ie)
        {
            uc_real_chan = puc_ie[2];
        }

        mac_get_channel_idx_from_num((pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]).en_band,
                                      uc_real_chan, &chan_index);
        pst_dmac_vap->st_vap_base_info.st_ch_switch_info.ul_chan_report_for_te_a |= (1 << chan_index);
    }

    return;
}
#endif

oal_void  dmac_scan_proc_obss_scan_complete_event(dmac_vap_stru *pst_dmac_vap)
{
#if 0
    OAM_INFO_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                 "{dmac_scan_proc_obss_scan_complete_event::te_a:%d,te_b:%d}",
                 pst_dmac_vap->st_vap_base_info.st_ch_switch_info.ul_chan_report_for_te_a,
                 pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_te_b);
#endif

    if(!pst_dmac_vap->st_vap_base_info.st_ch_switch_info.ul_chan_report_for_te_a
       && (OAL_FALSE == pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_te_b)) {
        return;
    }

    dmac_send_2040_coext_mgmt_frame_sta(&(pst_dmac_vap->st_vap_base_info));

    return;
}
#endif


OAL_STATIC oal_void  dmac_scan_set_vap_mac_addr_by_scan_state(mac_device_stru  *pst_mac_device,
                                                                           oal_bool_enum_uint8 en_is_scan_start)
{
    dmac_vap_stru               *pst_dmac_vap;
    mac_vap_stru                *pst_mac_vap;

    /* ???????????? */
    mac_scan_req_stru *pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ??????mac addr???????????????????????????????????? */
    if (OAL_TRUE != pst_scan_params->en_is_random_mac_addr_scan) {
        //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_set_vap_mac_addr_by_scan_state:: don't need modified mac addr.}");
        return;
    }

    /* p2p??????????????mac addr */
    if (OAL_TRUE == pst_scan_params->bit_is_p2p0_scan) {
        //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_set_vap_mac_addr_by_scan_state:: p2p scan, don't need modified mac addr.}");
        return;
    }

    /* ????dmac vap */
    pst_dmac_vap = mac_res_get_dmac_vap(pst_scan_params->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_set_vap_mac_addr_by_scan_state:: pst_dmac_vap is null.}");
        return;
    }

    pst_mac_vap = &(pst_dmac_vap->st_vap_base_info);

    /* ??????????P2P??????????????MAC ADDR?????? */
    if (!IS_LEGACY_VAP(pst_mac_vap))
    {
        return;
    }

    /* ???????????????????????????????? */
    if (OAL_TRUE == en_is_scan_start)
    {
        /* ??????????mac addr */
        oal_set_mac_addr(pst_mac_device->auc_original_mac_addr, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

        /* ????mib????????macaddr??????mac addr */
        oal_set_mac_addr(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID,
                         pst_scan_params->auc_sour_mac_addr);
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_scan_params->auc_sour_mac_addr);
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
        OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_set_vap_mac_addr_by_scan_state::original mac addr[%02X:XX:XX:%02X:%02X:%02X], set random mac addr[%02X:XX:XX:%02X:%02X:%02X]}", 8,
                                 pst_mac_device->auc_original_mac_addr[0], pst_mac_device->auc_original_mac_addr[3],
                                 pst_mac_device->auc_original_mac_addr[4], pst_mac_device->auc_original_mac_addr[5],
                                 pst_scan_params->auc_sour_mac_addr[0], pst_scan_params->auc_sour_mac_addr[3],
                                 pst_scan_params->auc_sour_mac_addr[4], pst_scan_params->auc_sour_mac_addr[5]);
#endif
    }
    else
    {
        /* ??????????????????mib??????????????mac addr */
        oal_set_mac_addr(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID,
                         pst_mac_device->auc_original_mac_addr);
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_mac_device->auc_original_mac_addr);

        OAM_WARNING_LOG4(0, OAM_SF_SCAN, "{dmac_scan_set_vap_mac_addr_by_scan_state:: resume original mac addr, mac_addr:%02X:XX:XX:%02X:%02X:%02X.}",
                         pst_mac_device->auc_original_mac_addr[0], pst_mac_device->auc_original_mac_addr[3], pst_mac_device->auc_original_mac_addr[4], pst_mac_device->auc_original_mac_addr[5]);
    }

    return;
}


OAL_STATIC oal_uint32  dmac_scan_report_scanned_bss(dmac_vap_stru *pst_dmac_vap, oal_void *p_param)
{
    frw_event_mem_stru                    *pst_event_mem;
    frw_event_stru                        *pst_event;
    dmac_tx_event_stru                    *pst_dtx_event;
    oal_netbuf_stru                       *pst_netbuf;
    dmac_rx_ctl_stru                      *pst_rx_ctrl;
    oal_uint8                             *puc_frame_body_tail;            /* ?????????????? */
    mac_scan_req_stru                     *pst_scan_params;
    mac_scanned_result_extend_info_stru   *pst_scanned_result_extend_info;
    oal_uint16                             us_frame_len;
    oal_uint16                             us_remain_netbuf_len;

    mac_device_stru *pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_report_scanned_bss::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_netbuf = (oal_netbuf_stru *)p_param;

    /* ?????????? */
    us_frame_len = (oal_uint16)oal_netbuf_get_len(pst_netbuf);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    us_remain_netbuf_len = oal_netbuf_get_payload_len(pst_netbuf) - (us_frame_len - MAC_80211_FRAME_LEN);
#else
    us_remain_netbuf_len = (oal_uint16)oal_netbuf_tailroom(pst_netbuf);
#endif

    if(us_remain_netbuf_len < OAL_SIZEOF(mac_scanned_result_extend_info_stru))
    {
        /*to cut one netbuf mem.*/
        /*tailroom not enough*/
        OAM_ERROR_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_report_scanned_bss::scan netbuf tailroom not enough,requet[%u],actual[%u] }",
                         us_remain_netbuf_len,
                         OAL_SIZEOF(mac_scanned_result_extend_info_stru));
        return OAL_FAIL;
    }

    /* ??????buffer?????????? */
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ???????????? */
    pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ????????????????????????????????????HMAC, ???????????? */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_tx_event_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_report_scanned_bss::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CRX,
                       DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT, OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1, pst_dmac_vap->st_vap_base_info.uc_chip_id,
                       pst_dmac_vap->st_vap_base_info.uc_device_id, pst_dmac_vap->st_vap_base_info.uc_vap_id);

    /***********************************************************************************************/
    /*            netbuf data??????????????????????????????                                        */
    /* ------------------------------------------------------------------------------------------  */
    /* beacon/probe rsp body  |     ????????????????(mac_scanned_result_extend_info_stru)          */
    /* -----------------------------------------------------------------------------------------   */
    /* ??????beacon/rsp??body | rssi(4????) | channel num(1????)| band(1????)|bss_tye(1????)|????  */
    /* ------------------------------------------------------------------------------------------  */
    /*                                                                                             */
    /***********************************************************************************************/

    /* ??????????????????????????????????host?? */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    puc_frame_body_tail = (oal_uint8 *)MAC_GET_RX_PAYLOAD_ADDR(&(pst_rx_ctrl->st_rx_info), pst_netbuf) + us_frame_len - MAC_80211_FRAME_LEN;
#else
    puc_frame_body_tail = (oal_uint8 *)mac_get_rx_cb_mac_hdr(&(pst_rx_ctrl->st_rx_info)) + us_frame_len;
#endif

    /* ??????????????????netbuf???????????????????????????????? */
    pst_scanned_result_extend_info = (mac_scanned_result_extend_info_stru *)puc_frame_body_tail;

    /* ???????????????????????????????????????? */
    OAL_MEMZERO(pst_scanned_result_extend_info, OAL_SIZEOF(mac_scanned_result_extend_info_stru));
    pst_scanned_result_extend_info->l_rssi = (oal_int32)pst_rx_ctrl->st_rx_statistic.c_rssi_dbm;
    pst_scanned_result_extend_info->en_bss_type = pst_scan_params->en_bss_type;

    /* ???????????????????????????????? */
    us_frame_len += OAL_SIZEOF(mac_scanned_result_extend_info_stru);

    /* ???????????? */
    pst_dtx_event               = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_dtx_event->pst_netbuf   = pst_netbuf;
    pst_dtx_event->us_frame_len = us_frame_len;

#if 0
    /* ??????????????????????????????????????????netbuf???? */
    OAM_ERROR_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                   "{dmac_scan_report_scanned_bss::rssi[%d], cb_rssi[%d], channel_num[%d], buf_len[%d].}",
                   pst_scanned_result_extend_info->l_rssi,
                   pst_rx_ctrl->st_rx_statistic.c_rssi_dbm,
                   pst_rx_ctrl->st_rx_info.uc_channel_number,
                   us_frame_len);
#endif

    /* ???????? */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_scan_check_bss_in_pno_scan(oal_uint8 *puc_frame_body, oal_int32 l_frame_body_len,
                                                       mac_pno_scan_stru *pst_pno_scan_info, oal_int32 l_rssi)
{
    oal_uint8       *puc_ssid;
    oal_int32        l_loop;
    oal_int8         ac_ssid[WLAN_SSID_MAX_LEN];
    oal_uint8        uc_ssid_len = 0;

    /* ??????????????????pno???????????????????????????????????????? */
    if (l_rssi < pst_pno_scan_info->l_rssi_thold)
    {
        return OAL_FAIL;
    }

    OAL_MEMZERO(ac_ssid, OAL_SIZEOF(ac_ssid));

    /* ??????????????ssid IE???? */
    puc_ssid = mac_get_ssid(puc_frame_body, l_frame_body_len, &uc_ssid_len);
    if ((OAL_PTR_NULL != puc_ssid) && (0 != uc_ssid_len))
    {
        oal_memcopy(ac_ssid, puc_ssid, uc_ssid_len);
        ac_ssid[uc_ssid_len] = '\0';
    }

    /* ??pno????????????ssid?????????????????????????????? */
    for (l_loop = 0; l_loop < pst_pno_scan_info->l_ssid_count; l_loop++)
    {
        /* ????ssid?????????????? */
        if (0 == oal_memcmp(ac_ssid, pst_pno_scan_info->ast_match_ssid_set[l_loop].auc_ssid, (uc_ssid_len + 1)))
        {
            OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_check_bss_in_pno_scan::ssid match success.}");
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}


OAL_STATIC oal_uint32  dmac_scan_check_bss_type(oal_uint8 *puc_frame_body, mac_scan_req_stru *pst_scan_params)
{
    mac_cap_info_stru         *pst_cap_info;

    /*************************************************************************/
    /*                       Beacon Frame - Frame Body                       */
    /* ----------------------------------------------------------------------*/
    /* |Timestamp|BcnInt|CapInfo|SSID|SupRates|DSParamSet|TIM  |CountryElem |*/
    /* ----------------------------------------------------------------------*/
    /* |8        |2     |2      |2-34|3-10    |3         |6-256|8-256       |*/
    /* ----------------------------------------------------------------------*/
    pst_cap_info = (mac_cap_info_stru *)(puc_frame_body + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN);

    if ((WLAN_MIB_DESIRED_BSSTYPE_INFRA == pst_scan_params->en_bss_type) && (1 != pst_cap_info->bit_ess))
    {
        //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_check_bss_type::expect infra bss, but it's not infra bss.}\r\n");
        return OAL_FAIL;
    }

    if ((WLAN_MIB_DESIRED_BSSTYPE_INDEPENDENT == pst_scan_params->en_bss_type) && (1 != pst_cap_info->bit_ibss))
    {
        //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_check_bss_type::expect ibss, but it's not ibss.}\r\n");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

#define DMAC_CSA_RSP_TIMEOUT 120000
frw_timeout_stru g_csa_stop_timer = {0};
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST

oal_void  dmac_scan_check_assoc_ap_channel(dmac_vap_stru *pst_dmac_vap, mac_device_stru *pst_mac_device, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    mac_ieee80211_frame_stru *pst_frame_hdr;
    oal_uint8 *puc_frame_body = MAC_GET_RX_PAYLOAD_ADDR(&(pst_rx_ctrl->st_rx_info), pst_netbuf);
    oal_uint16 us_frame_len = (oal_uint16)oal_netbuf_get_len(pst_netbuf);

    oal_uint16                              us_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    mac_cfg_ssid_param_stru                 st_mib_ssid = {0};
    oal_uint8                               uc_mib_ssid_len = 0;
    oal_uint8                               uc_frame_channel;
    oal_uint8                               uc_ssid_len = 0;
    oal_uint8                               *puc_ssid;
    oal_uint32                              ul_ret;
    oal_uint8                               uc_idx;
    mac_scan_req_stru                       *pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ?????????? */
    pst_frame_hdr  = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr(&(pst_rx_ctrl->st_rx_info));

    if (!OAL_MEMCMP(pst_frame_hdr->auc_address3, pst_dmac_vap->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN))
    {
        puc_ssid = mac_get_ssid(puc_frame_body, (oal_int32)(us_frame_len - MAC_80211_FRAME_LEN), &uc_ssid_len);
        mac_mib_get_ssid(&pst_dmac_vap->st_vap_base_info, &uc_mib_ssid_len, (oal_uint8 *)&st_mib_ssid);
        if ((OAL_PTR_NULL != puc_ssid) && (0 != uc_ssid_len) && (st_mib_ssid.uc_ssid_len == uc_ssid_len))
        {
            if (!OAL_MEMCMP(st_mib_ssid.ac_ssid, puc_ssid, uc_ssid_len))
            {
                /* ?????????????????? */
                uc_frame_channel = mac_ie_get_chan_num(puc_frame_body, (us_frame_len - MAC_80211_FRAME_LEN),
                                   us_offset, pst_rx_ctrl->st_rx_info.uc_channel_number);

                if (((pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number != uc_frame_channel &&
                    pst_scan_params->en_scan_mode != WLAN_SCAN_MODE_BACKGROUND_CSA) ||
                    (g_st_old_channel.uc_chan_number == pst_rx_ctrl->st_rx_info.uc_channel_number &&
                    pst_scan_params->en_scan_mode == WLAN_SCAN_MODE_BACKGROUND_CSA)) && (0 != uc_frame_channel))
                {
                    if (pst_scan_params->en_scan_mode == WLAN_SCAN_MODE_BACKGROUND_CSA) {
                        // ??????????AP??????????????????????????????????CSA????
                        pst_dmac_vap->st_vap_base_info.st_ch_switch_info.uc_switch_fail = OAL_TRUE;
                        if (!g_csa_stop_timer.en_is_registerd) {
                            FRW_TIMER_CREATE_TIMER(&g_csa_stop_timer, dmac_sta_csa_stop_timeout_fn,
                                DMAC_CSA_RSP_TIMEOUT, (void *)pst_dmac_vap, OAL_FALSE,
                                OAM_MODULE_ID_DMAC, pst_dmac_vap->st_vap_base_info.ul_core_id);
                        }
                        uc_frame_channel = pst_rx_ctrl->st_rx_info.uc_channel_number;
                    }
                    /* AP??????????????????,?????????? */
                    ul_ret = mac_get_channel_idx_from_num(pst_dmac_vap->st_vap_base_info.st_channel.en_band, uc_frame_channel, &uc_idx);
                    if (OAL_SUCC != ul_ret)
                    {
                        OAM_ERROR_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{dmac_scan_check_assoc_ap_channel::channel[%d],mac_get_channel_idx_from_num failed[%d].}", uc_frame_channel,ul_ret);
                        return;
                    }
                    /* ????????,???????????? */
                    pst_dmac_vap->st_vap_base_info.st_ch_switch_info.uc_new_channel    = uc_frame_channel;
                    pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_new_bandwidth = pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth;

                    /* ????2??????????????????????????????????????????,????CSA IE??????????uc_new_ch_swt_cnt??????0?? */
                    pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_waiting_to_shift_channel = OAL_TRUE;
                    pst_dmac_vap->st_vap_base_info.st_ch_switch_info.uc_new_ch_swt_cnt = 0;
                    OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{dmac_scan_check_assoc_ap_channel::AP is changed to new channel[%d].}", uc_frame_channel);

                }
            }
        }
    }
}
#endif


oal_uint32  dmac_scan_mgmt_filter(dmac_vap_stru *pst_dmac_vap, oal_void *p_param, oal_bool_enum_uint8 *pen_report_bss, oal_uint8 *pen_go_on)
{
    /* !!! ????:dmac_rx_filter_mgmt ??????pen_report_bss ????????????????netbuf,????pen_go_on???????????????????? */
    /* ????pen_report_bss??????OAL_TRUE????????????????????,dmac_rx_filter_mgmt??????????netbuf,????????pen_go_on??????????????????netbuf */

    oal_netbuf_stru            *pst_netbuf;
    dmac_rx_ctl_stru           *pst_rx_ctrl;
    mac_ieee80211_frame_stru   *pst_frame_hdr;
    oal_uint8                  *puc_frame_body;
    mac_device_stru            *pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    mac_scan_req_stru          *pst_scan_params;
    oal_uint32                  ul_ret;
    oal_uint16                  us_frame_len;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
    oal_uint16                  us_offset =  MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
#endif
#endif

    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_mgmt_filter::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_scan_params = &(pst_mac_device->st_scan_params);

    pst_netbuf = (oal_netbuf_stru *)p_param;

    /* ??????buffer?????????? */
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* ?????????? */
    pst_frame_hdr  = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr(&(pst_rx_ctrl->st_rx_info));
    puc_frame_body = MAC_GET_RX_PAYLOAD_ADDR(&(pst_rx_ctrl->st_rx_info), pst_netbuf);

    us_frame_len   = (oal_uint16)oal_netbuf_get_len(pst_netbuf);

    if ((WLAN_BEACON == pst_frame_hdr->st_frame_control.bit_sub_type) ||
        (WLAN_PROBE_RSP == pst_frame_hdr->st_frame_control.bit_sub_type))
    {
        /* ??????????bss?????? */
        if (OAL_SUCC != dmac_scan_check_bss_type(puc_frame_body, pst_scan_params))
        {
            //OAM_INFO_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_mgmt_filter::scanned bss isn't the desired one.}\r\n");
            return OAL_SUCC;
        }

        /* ??????obss????????????????????????????????????????beacon??????probe rsp????????????host????20/40???????????? */
        if (WLAN_SCAN_MODE_BACKGROUND_OBSS == pst_scan_params->en_scan_mode)
        {
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
            dmac_detect_2040_te_a_b(pst_dmac_vap, puc_frame_body, us_frame_len, us_offset, pst_rx_ctrl->st_rx_info.uc_channel_number);
#endif
#endif
            /* OBSS??????????????????????????????????????host */
            *pen_go_on = OAL_FALSE;
            return OAL_SUCC;
        }
        else
        {
            /* ????????????????????????bss ????????????????????????????????????????????????BSS */
            if ((pst_mac_device->st_scan_params.en_working_in_home_chan == OAL_FALSE)
                && (pst_mac_device->pst_device_stru->uc_current_chan_number
                        == mac_ie_get_chan_num(puc_frame_body, (us_frame_len - MAC_80211_FRAME_LEN),
                                                                MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN,
                                                                pst_rx_ctrl->st_rx_info.uc_channel_number)))
            {
                pst_mac_device->en_scan_curr_chan_find_bss_flag = OAL_TRUE;
            }
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
            if ((WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
                 && (MAC_VAP_STATE_PAUSE == pst_dmac_vap->st_vap_base_info.en_vap_state))
            {
                /* STA??????linkloss????????1/4????????AP?????????????????? */
                if (pst_dmac_vap->st_linkloss_info.us_link_loss >= pst_dmac_vap->st_linkloss_info.us_linkloss_threshold / 4)
                {
                    dmac_scan_check_assoc_ap_channel(pst_dmac_vap, pst_mac_device, pst_netbuf);
                }

                if (WLAN_SCAN_MODE_BACKGROUND_CSA == pst_scan_params->en_scan_mode)
                {
                    dmac_scan_check_assoc_ap_channel(pst_dmac_vap, pst_mac_device, pst_netbuf);
                    /* linkloss CSA??????????????????????????????host */
                    *pen_go_on = OAL_FALSE;
                    return OAL_SUCC;
                }
            }
#endif
            /* ??????pno????????????????????rssi??ssid?????? */
            if (WLAN_SCAN_MODE_BACKGROUND_PNO == pst_scan_params->en_scan_mode)
            {
                /* ??????bss????????????????pno???????????? */
                ul_ret = dmac_scan_check_bss_in_pno_scan(puc_frame_body,
                                                         (oal_int32)(us_frame_len - MAC_80211_FRAME_LEN),
                                                         &(pst_mac_device->pst_pno_sched_scan_mgmt->st_pno_sched_scan_params),
                                                         (oal_int32)pst_rx_ctrl->st_rx_statistic.c_rssi_dbm);
                if (OAL_SUCC != ul_ret)
                {
                    //OAM_INFO_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_mgmt_filter::this bss info can't report host.}");

                    /* PNO????,????????????????????AP,??????????????????????????host */
                    *pen_go_on = OAL_FALSE;

                    return OAL_SUCC;
                }

                /* ??????????????????????????ssid??????????????????ssid?????????? */
                if (OAL_TRUE != pst_mac_device->pst_pno_sched_scan_mgmt->en_is_found_match_ssid)
                {
                    pst_mac_device->pst_pno_sched_scan_mgmt->en_is_found_match_ssid = OAL_TRUE;

                    /* ????pno?????????????? */
                    dmac_scan_stop_pno_sched_scan_timer(pst_mac_device->pst_pno_sched_scan_mgmt);
                }
            }
#ifdef _PRE_WLAN_FEATURE_11K
            else if (WLAN_SCAN_MODE_RRM_BEACON_REQ == pst_scan_params->en_scan_mode)
            {
                /* WLAN_SCAN_MODE_RRM_BEACON_REQ????,??????????????????????????host */
                *pen_go_on = OAL_FALSE;

                dmac_rrm_get_bcn_info_from_rx(pst_dmac_vap, pst_netbuf);
                return OAL_SUCC;
            }
#endif

            /* ?????????????????????????????????? */
            ul_ret = dmac_scan_report_scanned_bss(pst_dmac_vap, p_param);
            if (OAL_SUCC != ul_ret)
            {
                *pen_report_bss = OAL_FALSE;
                OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_mgmt_filter::report scan result failed.}");
                return OAL_SUCC;
            }
            else
            {
                *pen_report_bss = OAL_TRUE;
            }
        }
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint16  dmac_scan_encap_probe_req_frame(dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_mgmt_buf, oal_uint8 *puc_bssid, oal_int8 *pc_ssid)
{
    oal_uint8        uc_ie_len;
    oal_uint8       *puc_mac_header          = oal_netbuf_header(pst_mgmt_buf);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_uint8       *puc_payload_addr        = oal_netbuf_data(pst_mgmt_buf);
#else
    oal_uint8       *puc_payload_addr        = puc_mac_header + MAC_80211_FRAME_LEN;
#endif
    oal_uint8       *puc_payload_addr_origin = puc_payload_addr;
    mac_device_stru *pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    oal_uint16       us_app_ie_len;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_scan_encap_probe_req_frame::pst_mac_device[%d] null!}", pst_dmac_vap->st_vap_base_info.uc_device_id);
        return 0;
    }
    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/

    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* ??????????????0??????type??subtype */
    mac_hdr_set_frame_control(puc_mac_header, WLAN_PROTOCOL_VERSION| WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_PROBE_REQ);

    /* ??????????????0 */
    mac_hdr_set_fragment_number(puc_mac_header, 0);

    /* ????????1?????????? */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR1_OFFSET, BROADCAST_MACADDR);

    /* ????????2??MAC????(p2p??????p2p????????????????????????????????mac addr??????????????????????) */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR2_OFFSET, pst_mac_device->st_scan_params.auc_sour_mac_addr);

    /* ????3?????????? */
    oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR3_OFFSET, puc_bssid);

    /*************************************************************************/
    /*                       Probe Request Frame - Frame Body                */
    /* --------------------------------------------------------------------- */
    /* |SSID |Supported Rates |Extended supp rates| HT cap|Extended cap      */
    /* --------------------------------------------------------------------- */
    /* |2-34 |   3-10         | 2-257             |  28   | 3-8              */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/

    /* ????SSID */
    /***************************************************************************
                    ----------------------------
                    |Element ID | Length | SSID|
                    ----------------------------
           Octets:  |1          | 1      | 0~32|
                    ----------------------------
    ***************************************************************************/
    if ('\0' == pc_ssid[0])    /* ????SSID */
    {
        puc_payload_addr[0] = MAC_EID_SSID;
        puc_payload_addr[1] = 0;
        puc_payload_addr   += MAC_IE_HDR_LEN;    /* ????buffer??????????ie */
    }
    else
    {
        puc_payload_addr[0] = MAC_EID_SSID;
        puc_payload_addr[1] = (oal_uint8)OAL_STRLEN(pc_ssid);
        oal_memcopy(&(puc_payload_addr[2]), pc_ssid, puc_payload_addr[1]);
        puc_payload_addr += MAC_IE_HDR_LEN + puc_payload_addr[1];  /* ????buffer??????????ie */
    }

    /* ???????????????? */
    mac_set_supported_rates_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* ????dsss?????? */
    mac_set_dsss_params(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* ????extended supported rates???? */
    mac_set_exsup_rates_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* PNO????,probe request??????????????????????????????,???????????????? */
    if((MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state)
       && (WLAN_SCAN_MODE_BACKGROUND_PNO == pst_mac_device->st_scan_params.en_scan_mode))
    {
        return (oal_uint16)(puc_payload_addr - puc_payload_addr_origin + MAC_80211_FRAME_LEN);
    }

    /* ????HT Capabilities???? */
    mac_set_ht_capabilities_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* ????Extended Capabilities???? */
    mac_set_ext_capabilities_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* ????vht capabilities???? */
    mac_set_vht_capabilities_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
    puc_payload_addr += uc_ie_len;

    /* ????WPS???? */
    mac_add_app_ie((oal_void *)&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &us_app_ie_len, OAL_APP_PROBE_REQ_IE);
    puc_payload_addr += us_app_ie_len;
#ifdef _PRE_WLAN_FEATURE_HILINK
    /* ????okc ie???? */
    mac_add_app_ie((oal_void *)&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &us_app_ie_len, OAL_APP_OKC_PROBE_IE);
    puc_payload_addr += us_app_ie_len;
#endif

#ifdef _PRE_WLAN_FEATURE_11K
    if (OAL_TRUE == pst_dmac_vap->bit_11k_enable)
    {
        mac_set_wfa_tpc_report_ie(&(pst_dmac_vap->st_vap_base_info), puc_payload_addr, &uc_ie_len);
        puc_payload_addr += uc_ie_len;
    }
#endif //_PRE_WLAN_FEATURE_11K

    return (oal_uint16)(puc_payload_addr - puc_payload_addr_origin + MAC_80211_FRAME_LEN);
}


oal_uint32  dmac_scan_send_probe_req_frame(dmac_vap_stru *pst_dmac_vap, oal_uint8 *puc_bssid, oal_int8 *pc_ssid)
{
    oal_netbuf_stru        *pst_mgmt_buf;
    mac_tx_ctl_stru        *pst_tx_ctl;
    oal_uint32              ul_ret;
    oal_uint16              us_mgmt_len;
    oal_uint8              *puc_mac_header = OAL_PTR_NULL;
    oal_uint8              *puc_saddr;

    /* ?????????????? */
    pst_mgmt_buf = OAL_MEM_NETBUF_ALLOC(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_MID);
    if (OAL_PTR_NULL == pst_mgmt_buf)
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_send_probe_req_frame::alloc netbuf failed.}");
        OAL_MEM_INFO_PRINT(OAL_MEM_POOL_ID_NETBUF);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_set_netbuf_prev(pst_mgmt_buf, OAL_PTR_NULL);
    oal_set_netbuf_next(pst_mgmt_buf, OAL_PTR_NULL);

    OAL_MEM_NETBUF_TRACE(pst_mgmt_buf, OAL_TRUE);

    /* ????probe request?? */
    us_mgmt_len = dmac_scan_encap_probe_req_frame(pst_dmac_vap, pst_mgmt_buf, puc_bssid, pc_ssid);

    /* ????netbuf??cb???????????????????????????????????? */
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_mgmt_buf);

    OAL_MEMZERO(pst_tx_ctl, sizeof(mac_tx_ctl_stru));
    mac_set_cb_ac(pst_tx_ctl, WLAN_WME_AC_MGMT);

    if (!ETHER_IS_MULTICAST(puc_bssid))
    {
        /* ?????????????? */
        puc_mac_header = oal_netbuf_header(pst_mgmt_buf);
        puc_saddr = mac_vap_get_mac_addr(&(pst_dmac_vap->st_vap_base_info));
        oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR1_OFFSET, puc_bssid);
        oal_set_mac_addr(puc_mac_header + WLAN_HDR_ADDR2_OFFSET, puc_saddr);
        MAC_GET_CB_IS_MCAST(pst_tx_ctl) = OAL_FALSE;
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) =(oal_uint8)pst_dmac_vap->st_vap_base_info.uc_assoc_vap_id;
    }
    else
    {
        /* ?????????????? */
        MAC_GET_CB_IS_MCAST(pst_tx_ctl) = OAL_TRUE;
        MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) =(oal_uint8)pst_dmac_vap->st_vap_base_info.us_multi_user_idx; /* probe request?????????? */
    }

    /* ?????????????????? */
    ul_ret = dmac_tx_mgmt(pst_dmac_vap, pst_mgmt_buf, us_mgmt_len);
    if (OAL_SUCC != ul_ret)
    {

        oal_netbuf_free(pst_mgmt_buf);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_uint32  dmac_scan_proc_scan_complete_event(dmac_vap_stru *pst_dmac_vap,
                                               mac_scan_status_enum_uint8 en_scan_rsp_status)
{
    frw_event_mem_stru         *pst_event_mem;
    frw_event_stru             *pst_event;
    mac_device_stru            *pst_mac_device;
    mac_scan_rsp_stru          *pst_scan_rsp_info;

    oal_uint8 uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;

    //OAM_INFO_LOG0(uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_complete_event:: start proc scan complete event.}");
    //dmac_scan_print_time_stamp();

    /* ????device?????? */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_complete_event::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????????????????DMAC, ???????????? */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(mac_scan_rsp_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_complete_event::alloc memory failed.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CRX,
                       DMAC_WLAN_CRX_EVENT_SUB_TYPE_SCAN_COMP,
                       OAL_SIZEOF(mac_scan_rsp_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_dmac_vap->st_vap_base_info.uc_chip_id,
                       pst_dmac_vap->st_vap_base_info.uc_device_id,
                       pst_dmac_vap->st_vap_base_info.uc_vap_id);

    pst_scan_rsp_info = (mac_scan_rsp_stru *)(pst_event->auc_event_data);

    /* ?????????????????????????????????????????? */
    if(OAL_TRUE == pst_mac_device->st_scan_params.en_abort_scan_flag)
    {
        pst_scan_rsp_info->en_scan_rsp_status = MAC_SCAN_ABORT;
    }
    else
    {
        pst_scan_rsp_info->en_scan_rsp_status = en_scan_rsp_status;
    }
    pst_scan_rsp_info->ull_cookie         = pst_mac_device->st_scan_params.ull_cookie;

    OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_complete_event::status:%d, vap channel:%d, cookie[%x], scan_mode[%d]}",
                        en_scan_rsp_status,
                        pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number,
                        pst_scan_rsp_info->ull_cookie,
                        pst_mac_device->st_scan_params.en_scan_mode);

    /* ???????? */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}

#if 0

OAL_STATIC oal_uint32  dmac_scan_is_too_busy(mac_device_stru *pst_mac_device, mac_scan_req_stru *pst_scan_req_params)
{
    oal_uint32       ul_ret;
    oal_uint32       ul_timestamp;
    oal_uint32       ul_deltatime;

    ul_ret = mac_device_is_p2p_connected(pst_mac_device);
    if (OAL_SUCC == ul_ret)
    {
        ul_timestamp = (oal_uint32)OAL_TIME_GET_STAMP_MS();
        ul_deltatime = (ul_timestamp > pst_mac_device->ul_scan_timestamp)? \
                       (ul_timestamp - pst_mac_device->ul_scan_timestamp):(0xffffffff - pst_mac_device->ul_scan_timestamp + ul_timestamp);

        if (MAC_SCAN_FUNC_P2P_LISTEN != pst_scan_req_params->uc_scan_func)
        {
            if (ul_deltatime < DMAC_SCAN_DBAC_SCAN_DELTA_TIME)
            {
                OAM_WARNING_LOG2(0, OAM_SF_DBAC, "has connected p2p. scan deltatime:%d<%d, refused", ul_deltatime, DMAC_SCAN_DBAC_SCAN_DELTA_TIME);
                return OAL_TRUE;
            }
        }
        else
        {
            if (ul_deltatime > 500 || pst_scan_req_params->us_scan_time >= DMAC_SCAN_GO_MAX_SCAN_TIME)
            {
                OAM_WARNING_LOG2(0, OAM_SF_DBAC, "has connected p2p. p2p listen deltatime:%d, scan_time:%d, refused", ul_deltatime, pst_scan_req_params->us_scan_time);
                return OAL_TRUE;
            }
        }
    }

    return OAL_FALSE;
}
#endif


oal_uint32  dmac_scan_update_channel_list(mac_device_stru    *pst_mac_device,
                                        dmac_vap_stru      *pst_dmac_vap)
{
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (mac_is_proxysta_enabled(pst_mac_device))
    {
        /* proxysta ?????????????? */
        if (mac_vap_is_vsta(&pst_dmac_vap->st_vap_base_info))
        {
            oal_uint8 uc_idx;
            oal_int32 l_found_idx = -1;

            mac_vap_stru *pst_msta = mac_find_main_proxysta(pst_mac_device);

            if (!pst_msta)
            {
                return OAL_FAIL;
            }

            for (uc_idx = 0; uc_idx < pst_mac_device->st_scan_params.uc_channel_nums; uc_idx++)
            {
                if (pst_mac_device->st_scan_params.ast_channel_list[uc_idx].uc_chan_number == pst_msta->st_channel.uc_chan_number)
                {
                    l_found_idx = (oal_int32)uc_idx;
                    break;
                }
            }

            if (l_found_idx >= 0)
            {
                pst_mac_device->st_scan_params.uc_channel_nums = 1;
                pst_mac_device->st_scan_params.ast_channel_list[0] = pst_mac_device->st_scan_params.ast_channel_list[l_found_idx];
            }
        }
    }
#endif
    return OAL_SUCC;
}


oal_uint32  dmac_scan_handle_scan_req_entry(mac_device_stru    *pst_mac_device,
                                            dmac_vap_stru      *pst_dmac_vap,
                                            mac_scan_req_stru  *pst_scan_req_params)
{
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_to_dmac_device_stru *pst_hal_device = OAL_PTR_NULL;
#endif
    /* ???????????????????????????? */
    /* ???????????????????????????????? */
    if((MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state)
       || ((OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag) && (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)))
    {
    #if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
        OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_handle_scan_req_entry:: device scan is running or always tx is running, cann't start scan. scan_vap_id[%d], scan_func[0x%02x], curr_scan_mode[%d], req_scan_mode[%d], scan_cookie[%x], al_tx_flag[%d].}",
                         6,
                         pst_mac_device->st_scan_params.uc_vap_id,
                         pst_mac_device->st_scan_params.uc_scan_func,
                         pst_mac_device->st_scan_params.en_scan_mode,
                         pst_scan_req_params->en_scan_mode,
                         pst_scan_req_params->ull_cookie,
                         pst_dmac_vap->st_vap_base_info.bit_al_tx_flag);
    #else
        OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_handle_scan_req_entry:: device scan is running or always tx is running, cann't start scan. scan_vap_id[%d], scan_func[0x%02x], scan_cookie[%x], al_tx_flag[%d].}",
                         pst_mac_device->st_scan_params.uc_vap_id,
                         pst_mac_device->st_scan_params.uc_scan_func,
                         pst_scan_req_params->ull_cookie,
                         pst_dmac_vap->st_vap_base_info.bit_al_tx_flag);
    #endif

        /* ????????????????????????????????????????????; OBSS?????????????????????????????????????????????? */
        if (pst_scan_req_params->en_scan_mode < WLAN_SCAN_MODE_BACKGROUND_OBSS)
        {
            /* ??????????????cookie ?? */
            pst_mac_device->st_scan_params.ull_cookie = pst_scan_req_params->ull_cookie;

            /* ?????????????????????????????? */
            return dmac_scan_proc_scan_complete_event(pst_dmac_vap, MAC_SCAN_REFUSED);
        }
#ifdef _PRE_WLAN_FEATURE_11K
        else if (WLAN_SCAN_MODE_RRM_BEACON_REQ == pst_scan_req_params->en_scan_mode)
        {
            OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_handle_scan_req_entry:: RRM BEACON REQ SCAN FAIL");
            return  OAL_FAIL;
        }
#endif
        else
        {
            return OAL_SUCC;
        }
    }

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_mac_device->uc_scan_ap_num_in_2p4                 = 0;

    if (WLAN_SCAN_MODE_BACKGROUND_OBSS > pst_scan_req_params->en_scan_mode)
    {
        pst_mac_device->uc_scan_count++;
    }
#endif

    /* ????device?????????????????????? */
    if (WLAN_SCAN_MODE_BACKGROUND_CCA == pst_scan_req_params->en_scan_mode)
    {
        /* ??????????????CCA ??????????????en_curr_scan_state ??????????????????host ?????????????? */
        pst_mac_device->en_curr_scan_state = MAC_SCAN_STATE_IDLE;
    }
    else
    {
        pst_mac_device->en_curr_scan_state = MAC_SCAN_STATE_RUNNING;

        /* ??????????????BT */
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        hal_set_btcoex_soc_gpreg1(OAL_TRUE, BIT1, 1);   // ????????????
        hal_coex_sw_irq_set(HAL_COEX_SW_IRQ_BT);
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
        /* ?????????? */
        if (WLAN_SCAN_MODE_BACKGROUND_AP >= pst_scan_req_params->en_scan_mode)
        {
            PM_WLAN_DumpSleepCnt();
        }
#endif
    }
    pst_mac_device->ul_scan_timestamp  = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    /* ??????????????????????????????????mac deivce?????????????????????????????????????????????????? */
    oal_memcopy(&(pst_mac_device->st_scan_params), pst_scan_req_params, OAL_SIZEOF(mac_scan_req_stru));
    pst_mac_device->en_scan_curr_chan_find_bss_flag = OAL_FALSE;
    pst_mac_device->st_scan_params.uc_curr_channel_scan_count = 0;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_mac_device->st_scan_params.uc_last_channel_band   = WLAN_BAND_BUTT;
#endif

#ifdef _PRE_WLAN_FEATURE_P2P
    /* P2P0 ??????????P2P listen channel */
    if (OAL_TRUE == pst_scan_req_params->bit_is_p2p0_scan)
    {
        pst_dmac_vap->st_vap_base_info.uc_p2p_listen_channel = pst_scan_req_params->uc_p2p0_listen_channel;
    }
#endif
    /* ?????????????????? */
    pst_mac_device->uc_scan_chan_idx = 0;
    dmac_scan_update_channel_list(pst_mac_device, pst_dmac_vap);

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (mac_is_proxysta_enabled(pst_mac_device)
      && pst_mac_device->st_scan_params.uc_channel_nums == 1
      && pst_mac_device->st_scan_params.ast_channel_list[0].uc_chan_number == pst_mac_device->pst_device_stru->uc_current_chan_number)
    {
        OAM_INFO_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,"{dmac_scan_handle_scan_req_entry:proxysta scan working channel %d, skip switching}",
                pst_mac_device->st_scan_params.ast_channel_list[0].uc_chan_number);
    }
    else
#endif
    {
        dmac_scan_switch_channel_off(pst_mac_device);
    }

    /* ????????????????????????????????????????vap??mac addr */
    dmac_scan_set_vap_mac_addr_by_scan_state(pst_mac_device, OAL_TRUE);

    /* ???????????????? */
    OAL_MEMZERO(&(pst_mac_device->st_chan_result), OAL_SIZEOF(mac_scan_chan_stats_stru));
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "dmac_btcoex_scan_begin:pst_hal_device is null");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if(HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
    {
        GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_BEGIN;
        OAM_WARNING_LOG0(0, OAM_SF_COEX, "{dmac_scan_handle_scan_req_entry:: normal scan begin delay by btcoex!}");
    }
    else
#endif
    {
        dmac_scan_begin(pst_mac_device);
    }
    return OAL_SUCC;
}


OAL_STATIC oal_void dmac_scan_prepare_pno_scan_params(mac_scan_req_stru  *pst_scan_params,
                                                                  dmac_vap_stru    *pst_dmac_vap)
{
    oal_uint8           uc_chan_idx;
    oal_uint8           uc_2g_chan_num = 0;
    oal_uint8           uc_5g_chan_num = 0;
    oal_uint8           uc_chan_number;
    mac_device_stru    *pst_mac_device;

    /* ???????????????? */
    OAL_MEMZERO(pst_scan_params, OAL_SIZEOF(mac_scan_req_stru));

    /* ??????????????vap id */
    pst_scan_params->uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id,OAM_SF_SCAN, "{dmac_scan_prepare_pno_scan_params::pst_mac_device null.}");
        return;
    }

    /* ?????????????????????? */
    pst_scan_params->en_bss_type    = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type   = WLAN_SCAN_TYPE_ACTIVE;
    pst_scan_params->en_scan_mode   = WLAN_SCAN_MODE_BACKGROUND_PNO;
    pst_scan_params->us_scan_time   = WLAN_DEFAULT_ACTIVE_SCAN_TIME * 2; /* ????PNO????SSID????,????????????????????40ms */
    pst_scan_params->uc_probe_delay = 0;
    pst_scan_params->uc_scan_func   = MAC_SCAN_FUNC_BSS;   /* ????????bss */
    pst_scan_params->uc_max_scan_count_per_channel           = 1;
    pst_scan_params->uc_max_send_probe_req_count_per_channel = WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL;

	/* ????????????pro req??src mac????*/
    oal_set_mac_addr(pst_scan_params->auc_sour_mac_addr, pst_mac_device->pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.auc_sour_mac_addr);
    pst_scan_params->en_is_random_mac_addr_scan = pst_mac_device->pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.en_is_random_mac_addr_scan;

    /* ??????????????ssid???? */
    pst_scan_params->ast_mac_ssid_set[0].auc_ssid[0] = '\0';   /* ????ssid */
    pst_scan_params->uc_ssid_num = 1;

    /* ??????????????????1??bssid???????????? */
    oal_set_mac_addr(pst_scan_params->auc_bssid[0], BROADCAST_MACADDR);
    pst_scan_params->uc_bssid_num = 1;

    /* 2G????????????, ?????????? */
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_chan_idx++)
    {
        /* ???????????????????????? */
        if (OAL_SUCC == mac_is_channel_idx_valid(WLAN_BAND_2G, uc_chan_idx))
        {
            mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_chan_idx, &uc_chan_number);

            pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_chan_number = uc_chan_number;
            pst_scan_params->ast_channel_list[uc_2g_chan_num].en_band        = WLAN_BAND_2G;
            pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_idx         = uc_chan_idx;
            pst_scan_params->uc_channel_nums++;
            uc_2g_chan_num++;
        }
    }
    //OAM_INFO_LOG1(0, OAM_SF_SCAN, "{dmac_scan_prepare_pno_scan_params::after regdomain filter, the 2g total channel num is %d", uc_2g_chan_num);
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (OAL_TRUE != g_st_customize.st_cali.uc_band_5g_enable)
    {
        return;
    }
#endif

    /* 5G????????????, ?????????? */
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_chan_idx++)
    {
        /* ???????????????????????? */
        if (OAL_SUCC == mac_is_channel_idx_valid(WLAN_BAND_5G, uc_chan_idx))
        {
            mac_get_channel_num_from_idx(WLAN_BAND_5G, uc_chan_idx, &uc_chan_number);

            pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].uc_chan_number = uc_chan_number;
            pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].en_band        = WLAN_BAND_5G;
            pst_scan_params->ast_channel_list[uc_2g_chan_num + uc_5g_chan_num].uc_idx         = uc_chan_idx;
            pst_scan_params->uc_channel_nums++;
            uc_5g_chan_num++;
        }
    }
    //OAM_INFO_LOG1(0, OAM_SF_SCAN, "{dmac_scan_prepare_pno_scan_params::after regdomain filter, the 5g total channel num is %d", uc_5g_chan_num);

    return;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

/*lint -e528*/
OAL_STATIC oal_void  dmac_scan_pno_scan_timeout_fn(void *p_ptr, void *p_arg)
{
    dmac_vap_stru                       *pst_dmac_vap;
    mac_device_stru                     *pst_mac_device;
    mac_scan_req_stru                    st_scan_req_params;


    pst_dmac_vap = (dmac_vap_stru *)p_arg;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_pno_scan_timeout_fn::pst_dmac_vap null.}");
        return;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_pno_scan_timeout_fn::pst_mac_device null.}");
        return;
    }

#ifdef _PRE_WLAN_FEATURE_STA_PM
    PM_WLAN_PsmHandle(PM_STA0_ID, PM_WLAN_WORK_PROCESS);
#endif


#if 0
    /* ????pno?????????????? */
    pst_mac_device->pst_pno_sched_scan_mgmt->uc_curr_pno_sched_scan_times++;

    /* pno???????????????????????????????????????????? */
    if (pst_mac_device->pst_pno_sched_scan_mgmt->uc_curr_pno_sched_scan_times >= pst_mac_device->pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.uc_pno_scan_repeat)
    {
        /* ????PNO?????????????????? */
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_pno_scan_timeout_fn:: reached max pno scan repeat times, stop pno sched scan.}");

#ifdef _PRE_WLAN_FEATURE_STA_PM
        dmac_pm_sta_post_event(pst_dmac_vap, STA_PWR_EVENT_PNO_SCHED_SCAN_COMP, 0, OAL_PTR_NULL);
#endif
        /* ????PNO?????????????? */
        OAL_MEM_FREE(pst_mac_device->pst_pno_sched_scan_mgmt, OAL_TRUE);
        pst_mac_device->pst_pno_sched_scan_mgmt = OAL_PTR_NULL;
        return OAL_SUCC;
    }
#endif

    /* ????????????: ??????????????bss */
    pst_mac_device->pst_pno_sched_scan_mgmt->en_is_found_match_ssid = OAL_FALSE;

    /* ????PNO?????????????????????? */
    dmac_scan_prepare_pno_scan_params(&st_scan_req_params, pst_dmac_vap);

    /* ???????????????? */
    hal_set_rx_filter(pst_mac_device->pst_device_stru, 0x37B9CEEA);

    /* ???????????? */
    dmac_scan_handle_scan_req_entry(pst_mac_device, pst_dmac_vap, &st_scan_req_params);

    /* ????????PNO?????????????? */
    dmac_scan_start_pno_sched_scan_timer((void *)pst_dmac_vap);

    return;
}
#endif



OAL_STATIC oal_uint32  dmac_scan_start_pno_sched_scan_timer(void *p_arg)
{
#if ((_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_TEST_MODE != _PRE_TEST_MODE_UT))
    dmac_vap_stru                           *pst_dmac_vap = (dmac_vap_stru *)p_arg;
    mac_device_stru                         *pst_mac_device;
    mac_pno_sched_scan_mgmt_stru            *pst_pno_mgmt;
    oal_int32                                l_ret = OAL_SUCC;

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_start_pno_sched_scan_timer:: pst_mac_device is null.}");
        return OAL_FAIL;
    }

    /* ????pno?????????? */
    pst_pno_mgmt = pst_mac_device->pst_pno_sched_scan_mgmt;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* ????pno??????????rtc?????????????????????????????????????????????????????????????????? */
    if (OAL_PTR_NULL == pst_pno_mgmt->p_pno_sched_scan_timer)
    {
        pst_pno_mgmt->p_pno_sched_scan_timer = (oal_void *)oal_rtctimer_create(dmac_scan_pno_scan_timeout_fn, p_arg);
        if (OAL_PTR_NULL == pst_pno_mgmt->p_pno_sched_scan_timer)
        {
            OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                           "{dmac_scan_start_pno_sched_scan_timer:: create pno timer faild.}");
            return OAL_FAIL;
        }
    }

#endif

    /* ??????????????????????????????pno?????????????? */
    if (0 == pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval / 100)
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_start_pno_sched_scan_timer:: pno scan interval[%d] time too short.}",
                         pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval);
        return OAL_FAIL;
    }

    /* ????????????????????????????????????????????????????100??????????????????????100 */
    l_ret = oal_rtctimer_start((STIMER_STRU *)pst_pno_mgmt->p_pno_sched_scan_timer, pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval / 100);
    if (OAL_SUCC != l_ret)
    {
        OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_start_pno_sched_scan_timer:: start pno timer faild[%d].}",l_ret);
        return OAL_FAIL;
    }

    OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                     "{dmac_scan_start_pno_sched_scan_timer:: start pno timer succ, timeout[%d].}",
                     pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval / 100);

    /* ????PNO??????????????????????60s(PNO_SCHED_SCAN_INTERVAL),??????????????????????60??,??????????300s */
    pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval += (60 * 1000);
    if(pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval > (300 * 1000))
    {
        pst_pno_mgmt->st_pno_sched_scan_params.ul_pno_scan_interval = (300 * 1000);
    }

#else
    /* 1151????????????????????????do nothing???????????????????????????????????? */
#endif

    return OAL_SUCC;
}


oal_uint32  dmac_scan_stop_pno_sched_scan_timer(void *p_arg)
{
#if ((_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_TEST_MODE != _PRE_TEST_MODE_UT))
    mac_pno_sched_scan_mgmt_stru    *pst_pno_mgmt;

    pst_pno_mgmt = (mac_pno_sched_scan_mgmt_stru *)p_arg;

    /* ?????????????????????????? */
    if (OAL_PTR_NULL == pst_pno_mgmt->p_pno_sched_scan_timer)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN,
                         "{dmac_scan_stop_pno_sched_scan_timer:: pno sched timer not create yet.}");
        return OAL_SUCC;
    }
    /* ?????????? */
    oal_rtctimer_delete((STIMER_STRU *)pst_pno_mgmt->p_pno_sched_scan_timer);
    pst_pno_mgmt->p_pno_sched_scan_timer = OAL_PTR_NULL;
#else
    /* 1151????????????????????????do nothing???????????????????????????????????? */
#endif

    return OAL_SUCC;
}


oal_uint32  dmac_scan_proc_sched_scan_req_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru             *pst_event;
    frw_event_hdr_stru         *pst_event_hdr;
    dmac_vap_stru              *pst_dmac_vap;
    mac_device_stru            *pst_mac_device;
    mac_scan_req_stru           st_scan_req_params;

    OAL_IO_PRINT("dmac_scan_proc_sched_scan_req_event::enter here.\r\n");

    /* ?????????????? */
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_proc_sched_scan_req_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????? */
    pst_event        = (frw_event_stru *)(pst_event_mem->puc_data);
    pst_event_hdr    = &(pst_event->st_event_hdr);

    /* ????dmac vap */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_event_hdr->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_sched_scan_req_event::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_event_hdr->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_sched_scan_req_event::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????pno???????????? */
    dmac_scan_prepare_pno_scan_params(&st_scan_req_params, pst_dmac_vap);

    /* ????PNO????????????????0 */
    pst_mac_device->pst_pno_sched_scan_mgmt->uc_curr_pno_sched_scan_times = 0;
    pst_mac_device->pst_pno_sched_scan_mgmt->en_is_found_match_ssid = OAL_FALSE;

    /* ???????????????? */
    hal_set_rx_filter(pst_mac_device->pst_device_stru, 0x37B9CEEA);

#ifdef _PRE_WLAN_FEATURE_STA_PM
	/* pno????????????????????????work??,?????????????????????????????????????? */
    PM_WLAN_PsmHandle(PM_STA0_ID, PM_WLAN_WORK_PROCESS);
#endif

    /* ?????????????????????? */
    dmac_scan_handle_scan_req_entry(pst_mac_device, pst_dmac_vap, &st_scan_req_params);

    /* ????pno??????????rtc????????????????????????device */
    dmac_scan_start_pno_sched_scan_timer((void *)pst_dmac_vap);

    return OAL_SUCC;
}


oal_uint32  dmac_scan_proc_scan_req_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru             *pst_event;
    frw_event_hdr_stru         *pst_event_hdr;
    dmac_vap_stru              *pst_dmac_vap;
    mac_device_stru            *pst_mac_device;
    mac_scan_req_stru          *pst_h2d_scan_req_params;

    /* ?????????????? */
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_proc_scan_req_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????? */
    pst_event        = (frw_event_stru *)(pst_event_mem->puc_data);
    pst_event_hdr    = &(pst_event->st_event_hdr);

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_event_hdr->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_req_event::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_event_hdr->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_SCAN, "{dmac_scan_proc_scan_req_event::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????? */
    pst_h2d_scan_req_params = (mac_scan_req_stru *)frw_get_event_payload(pst_event_mem);

    /* ????????mac??????????????dmac?????????????? */
    pst_mac_device->en_is_random_mac_addr_scan = pst_h2d_scan_req_params->en_is_random_mac_addr_scan;

    /* host?????????????????????? */
    return dmac_scan_handle_scan_req_entry(pst_mac_device, pst_dmac_vap, pst_h2d_scan_req_params);
}

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST


OAL_STATIC oal_uint32 dmac_scan_prepare_obss_scan_params(mac_scan_req_stru  *pst_scan_params,
                                                         dmac_vap_stru      *pst_dmac_vap)
{
    mac_device_stru *pst_mac_device;
    oal_uint8        uc_2g_chan_num      = 0;
    oal_uint8        uc_channel_idx      = 0;
    oal_uint8        uc_low_channel_idx  = 0;
    oal_uint8        uc_high_channel_idx = 0;
    oal_uint8        uc_channel_num      = 0;
    oal_uint8        uc_curr_channel_num = pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number;
    oal_uint8        uc_curr_band        = pst_dmac_vap->st_vap_base_info.st_channel.en_band;

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_prepare_obss_scan_params::mac_res_get_dev failed.}");
        return OAL_FAIL;
    }

    OAL_MEMZERO(pst_scan_params, OAL_SIZEOF(mac_scan_req_stru));

    /* 1.??????????????vap id */
    pst_scan_params->uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;

    /* 2.?????????????????????? */
    pst_scan_params->en_bss_type         = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type        = WLAN_SCAN_TYPE_ACTIVE;
    pst_scan_params->uc_probe_delay      = 0;
    pst_scan_params->uc_scan_func        = MAC_SCAN_FUNC_BSS;               /* ????????bss */
    pst_scan_params->uc_max_send_probe_req_count_per_channel = WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL;
    pst_scan_params->uc_max_scan_count_per_channel           = 1;

    /* ???????????????????????????????? */
    if (WLAN_SCAN_TYPE_ACTIVE == pst_scan_params->en_scan_type)
    {
        pst_scan_params->us_scan_time = WLAN_DEFAULT_ACTIVE_SCAN_TIME;
    }
    else
    {
        pst_scan_params->us_scan_time = WLAN_DEFAULT_PASSIVE_SCAN_TIME;
    }

    /* OBSS????????ssid */
    pst_scan_params->ast_mac_ssid_set[0].auc_ssid[0] = '\0';
    pst_scan_params->uc_ssid_num = 1;
    /* OBSS????????Source MAC ADDRESS */
    if((pst_mac_device->en_is_random_mac_addr_scan)
       && ((pst_mac_device->auc_mac_oui[0] != 0) || (pst_mac_device->auc_mac_oui[1] != 0) || (pst_mac_device->auc_mac_oui[2] != 0)))
    {
        pst_scan_params->auc_sour_mac_addr[0] = (pst_mac_device->auc_mac_oui[0] & 0xfe);  /*??????????mac*/
        pst_scan_params->auc_sour_mac_addr[1] = pst_mac_device->auc_mac_oui[1];
        pst_scan_params->auc_sour_mac_addr[2] = pst_mac_device->auc_mac_oui[2];
        pst_scan_params->auc_sour_mac_addr[3] = oal_gen_random((oal_uint32)OAL_TIME_GET_STAMP_MS(), 1);
        pst_scan_params->auc_sour_mac_addr[4] = oal_gen_random((oal_uint32)OAL_TIME_GET_STAMP_MS(), 1);
        pst_scan_params->auc_sour_mac_addr[5] = oal_gen_random((oal_uint32)OAL_TIME_GET_STAMP_MS(), 1);
        pst_scan_params->en_is_random_mac_addr_scan = OAL_TRUE;
    }
    else
    {
        oal_set_mac_addr(pst_scan_params->auc_sour_mac_addr, pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    }

    /* OBSS??????????????1??bssid???????????? */
    oal_set_mac_addr(pst_scan_params->auc_bssid[0], BROADCAST_MACADDR);
    pst_scan_params->uc_bssid_num = 1;

    /* ??????????????OBSS???? */
    pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_BACKGROUND_OBSS;

    /* ????OBSS?????????? */
    if (WLAN_BAND_2G == uc_curr_band)
    {

        /* ??????????????????5????????????OBSS????????
           1) ????????idx????????5??????0????????idx+5,
           2) ????5????8????????idx-5??idx+5,
           3) ????8????????idx-5??13 */
        if (uc_curr_channel_num <= 5)
        {
            uc_low_channel_idx = 0;
            uc_high_channel_idx = uc_curr_channel_num + 5;
        }
        else if (5 < uc_curr_channel_num && uc_curr_channel_num <= 8)
        {
            uc_low_channel_idx  = uc_curr_channel_num - 5;
            uc_high_channel_idx = uc_curr_channel_num + 5;
        }
        else if (8 < uc_curr_channel_num && uc_curr_channel_num <= 13)
        {
            uc_low_channel_idx = uc_curr_channel_num - 5;
            uc_high_channel_idx = 13;
        }
        else
        {
            uc_low_channel_idx  = 0;
            uc_high_channel_idx = 0;
            OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_update_obss_scan_params::2040M,Current channel index is %d.}",
                           uc_curr_channel_num);
        }

        /* ????2.4G??OBSS???????? */
        for(uc_channel_idx = uc_low_channel_idx; uc_channel_idx <= uc_high_channel_idx; uc_channel_idx++)
        {
            /* ???????????????????????? */
            if (OAL_SUCC == mac_is_channel_idx_valid(WLAN_BAND_2G, uc_channel_idx))
            {
                mac_get_channel_num_from_idx(WLAN_BAND_2G, uc_channel_idx, &uc_channel_num);

                pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_chan_number = uc_channel_num;
                pst_scan_params->ast_channel_list[uc_2g_chan_num].en_band        = WLAN_BAND_2G;
                pst_scan_params->ast_channel_list[uc_2g_chan_num].uc_idx         = uc_channel_idx;
                pst_scan_params->uc_channel_nums++;
                uc_2g_chan_num++;
            }
        }

        /* ?????????????????????? */
        pst_scan_params->uc_channel_nums = uc_2g_chan_num;
    }
#if 0
    else if (WLAN_BAND_5G == uc_curr_band)
    {
        /* ??????????5G????obss???? */
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_update_obss_scan_params::5G don't do obss scan.}");
    }
    else
    {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_update_obss_scan_params::error band[%d].}", uc_curr_band);
    }
#endif
    /* ????????????????????????0?????????????????????????? */
    if (0 == pst_scan_params->uc_channel_nums)
    {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_update_obss_scan_params::scan total channel num is 0, band[%d]!}", uc_curr_band);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_scan_obss_timeout_fn(void *p_arg)
{
    dmac_vap_stru          *pst_dmac_vap;
    mac_device_stru        *pst_mac_device;
    mac_scan_req_stru       st_scan_req_params;
    oal_uint32              ul_ret;
    oal_uint8               uc_scan_now = OAL_FALSE;

    //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_obss_timeout_fn::obss timer time out.}");

    pst_dmac_vap = (dmac_vap_stru *)p_arg;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_obss_timeout_fn::pst_dmac_vap null.}");
        return OAL_FAIL;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_obss_timeout_fn::pst_mac_device null.}");
        return OAL_FAIL;
    }

    /* ??????obss scan timer??????????????????sta????????sta??????obss??????
     * ??????obss scan timer
     * ??????????????obss??????????!!!
     */
    if (OAL_FALSE == dmac_mgmt_need_obss_scan(&pst_dmac_vap->st_vap_base_info))
    {
        pst_dmac_vap->ul_obss_scan_timer_remain  = 0;
        pst_dmac_vap->uc_obss_scan_timer_started = OAL_FALSE;
        //OAM_INFO_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
        //            "{dmac_scan_obss_timeout_fn::stop obss scan timer}");
        return OAL_SUCC;
    }

    if (0 == pst_dmac_vap->ul_obss_scan_timer_remain)
    {
        uc_scan_now = OAL_TRUE;
    }

    dmac_scan_start_obss_timer(&pst_dmac_vap->st_vap_base_info);

    /* ??????????????????obss???? */
    if (OAL_TRUE == uc_scan_now)
    {
        /* ????OBSS?????????????????????? */
        ul_ret = dmac_scan_prepare_obss_scan_params(&st_scan_req_params, pst_dmac_vap);
        if (OAL_SUCC != ul_ret)
        {
            //OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{dmac_scan_obss_timeout_fn::update scan params error[%d].}", ul_ret);
            return ul_ret;
        }

        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_obss_timeout_fn:: start scan}");
        return dmac_scan_handle_scan_req_entry(pst_mac_device, pst_dmac_vap, &st_scan_req_params);
    }

    return OAL_SUCC;
}


oal_void dmac_scan_start_obss_timer(mac_vap_stru *pst_mac_vap)
{
    dmac_vap_stru                 *pst_dmac_vap;
    mac_device_stru               *pst_mac_device;
    oal_uint32                    ul_new_timer;

    /* ??????????????vap id????dmac vap */
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_start_obss_timer:: pst_dmac_vap is NULL.}");
        return;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                       "{dmac_scan_start_obss_timer::pst_mac_device null.}");
        return;
    }

    /* ?????????????? */
    if (0 == pst_dmac_vap->ul_obss_scan_timer_remain)
    {
        pst_dmac_vap->ul_obss_scan_timer_remain = 1000*mac_mib_get_BSSWidthTriggerScanInterval(&pst_dmac_vap->st_vap_base_info);
    }

    ul_new_timer = pst_dmac_vap->ul_obss_scan_timer_remain > DMAC_SCAN_MAX_TIMER?
                        DMAC_SCAN_MAX_TIMER:pst_dmac_vap->ul_obss_scan_timer_remain;
    pst_dmac_vap->ul_obss_scan_timer_remain -= ul_new_timer;
    OAM_INFO_LOG2(0, OAM_SF_SCAN, "{dmac_scan_start_obss_timer::remain=%d new_timer=%d}",
                pst_dmac_vap->ul_obss_scan_timer_remain, ul_new_timer);

    FRW_TIMER_CREATE_TIMER(&(pst_mac_device->st_obss_scan_timer),
                           dmac_scan_obss_timeout_fn,
                           ul_new_timer,
                           (void *)pst_dmac_vap,
                           OAL_FALSE,
                           OAM_MODULE_ID_DMAC,
                           pst_mac_device->ul_core_id);
    pst_dmac_vap->uc_obss_scan_timer_started = OAL_TRUE;

    return;
}
oal_uint8 g_csa_scan_flag = OAL_FALSE;

oal_uint32 dmac_trigger_csa_scan(mac_scan_req_stru  *pst_scan_params,
                                       mac_vap_stru      *pst_mac_vap,
                                       mac_channel_stru  *pst_old_channel)
{
    oal_uint8               uc_chan_num      = 0;
    oal_uint8               uc_channel_idx      = 0;
    dmac_vap_stru          *pst_dmac_vap;
    mac_device_stru        *pst_mac_device;

    if (pst_old_channel->uc_chan_number == pst_mac_vap->st_channel.uc_chan_number)
    {
        return OAL_SUCC;
    }

    OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_trigger_csa_scan::update csa scan params.}");

    OAL_MEMZERO(pst_scan_params, OAL_SIZEOF(mac_scan_req_stru));

    /* 1.??????????????vap id */
    pst_scan_params->uc_vap_id = pst_mac_vap->uc_vap_id;

    /* 2.?????????????????????? */
    pst_scan_params->en_bss_type         = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    pst_scan_params->en_scan_type        = WLAN_SCAN_TYPE_ACTIVE;
    pst_scan_params->uc_probe_delay      = 0;
    pst_scan_params->uc_scan_func        = MAC_SCAN_FUNC_BSS;               /* ????????bss */
    pst_scan_params->uc_max_send_probe_req_count_per_channel = WLAN_DEFAULT_SEND_PROBE_REQ_COUNT_PER_CHANNEL;
    pst_scan_params->uc_max_scan_count_per_channel           = 2;

    /* ???????????????????????????????? */
    if (WLAN_SCAN_TYPE_ACTIVE == pst_scan_params->en_scan_type)
    {
        pst_scan_params->us_scan_time = WLAN_DEFAULT_ACTIVE_SCAN_TIME;
    }

    /* CSA????????ssid */
    pst_scan_params->ast_mac_ssid_set[0].auc_ssid[0] = '\0';
    pst_scan_params->uc_ssid_num = 1;
    /* CSA????????Source MAC ADDRESS */
    oal_set_mac_addr(pst_scan_params->auc_sour_mac_addr, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

    /* CSA??????????????1??bssid???????????? */
    oal_set_mac_addr(pst_scan_params->auc_bssid[0], BROADCAST_MACADDR);
    pst_scan_params->uc_bssid_num = 1;

    /* ??????????????CSA???? */
    pst_scan_params->en_scan_mode = WLAN_SCAN_MODE_BACKGROUND_CSA;

    /* ?????????????? */
    /* ???????????????????????? */

    if (OAL_SUCC == mac_is_channel_num_valid(pst_old_channel->en_band, pst_old_channel->uc_chan_number))
    {
        mac_get_channel_idx_from_num(pst_old_channel->en_band, pst_old_channel->uc_chan_number, &uc_channel_idx);
        pst_scan_params->ast_channel_list[uc_chan_num].uc_chan_number = pst_old_channel->uc_chan_number;
        pst_scan_params->ast_channel_list[uc_chan_num].en_band        = pst_old_channel->en_band;
        pst_scan_params->ast_channel_list[uc_chan_num].uc_idx         = uc_channel_idx;
        pst_scan_params->uc_channel_nums++;
        uc_chan_num++;
    }

    if (OAL_SUCC == mac_is_channel_num_valid(pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.uc_chan_number))
    {
        mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.uc_chan_number, &uc_channel_idx);
        pst_scan_params->ast_channel_list[uc_chan_num].uc_chan_number = pst_mac_vap->st_channel.uc_chan_number;
        pst_scan_params->ast_channel_list[uc_chan_num].en_band        = pst_mac_vap->st_channel.en_band;
        pst_scan_params->ast_channel_list[uc_chan_num].uc_idx         = uc_channel_idx;
        pst_scan_params->uc_channel_nums++;
        uc_chan_num++;
    }


    /* ?????????????????????? */
    pst_scan_params->uc_channel_nums = uc_chan_num;

    /* ????????????????????????0?????????????????????????? */
    if (0 == pst_scan_params->uc_channel_nums)
    {
       OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_trigger_csa_scan::scan total channel num is 0.}");
       return OAL_FAIL;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_trigger_csa_scan::pst_dmac_vap null.}");
        return OAL_FAIL;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{dmac_trigger_csa_scan::pst_mac_device null.}");
        return OAL_FAIL;
    }

    g_csa_scan_flag = OAL_TRUE;
    dmac_scan_handle_scan_req_entry(pst_mac_device, pst_dmac_vap, pst_scan_params);

    return OAL_SUCC;
}

#endif


OAL_STATIC oal_void  dmac_scan_switch_channel_notify_alg(dmac_vap_stru     *pst_dmac_vap,
                                                         mac_channel_stru  *pst_channel)
{
    mac_channel_stru                st_channel_tmp;

    /* ?????????????? */
    if ((OAL_PTR_NULL == pst_dmac_vap) || (OAL_PTR_NULL == pst_channel))
    {
        OAM_ERROR_LOG2(0, OAM_SF_SCAN, "{dmac_scan_switch_channel_notify_alg::pst_dmac_vap[%p], pst_channel[%p].}",
                       pst_dmac_vap, pst_channel);
        return;
    }

    /* ????????vap???????????? */
    st_channel_tmp = pst_dmac_vap->st_vap_base_info.st_channel;

    /* ???????????????????????????????? */
    pst_dmac_vap->st_vap_base_info.st_channel.en_band        = pst_channel->en_band;
    pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number = pst_channel->uc_chan_number;
    pst_dmac_vap->st_vap_base_info.st_channel.uc_idx         = pst_channel->uc_idx;
    pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;

    /* ???????? */
    dmac_alg_cfg_channel_notify(&pst_dmac_vap->st_vap_base_info, CH_BW_CHG_TYPE_SCAN);
    dmac_alg_cfg_bandwidth_notify(&pst_dmac_vap->st_vap_base_info, CH_BW_CHG_TYPE_SCAN);

    /* ????????????????vap???????????? */
    pst_dmac_vap->st_vap_base_info.st_channel = st_channel_tmp;

    return;
}


oal_uint32  dmac_switch_channel_off(
                mac_device_stru     *pst_mac_device,
                mac_vap_stru        *pst_mac_vap,
                mac_channel_stru    *pst_dst_chl,
                oal_uint16           us_protect_time)
{
    mac_fcs_mgr_stru               *pst_fcs_mgr;
    mac_fcs_cfg_stru               *pst_fcs_cfg;

    /* ?????????????????????????????? */
    pst_mac_device->st_home_channel = pst_mac_vap->st_channel;

    pst_fcs_mgr = dmac_fcs_get_mgr_stru(pst_mac_device);
    pst_fcs_cfg = &(pst_mac_device->st_fcs_cfg);

    OAL_MEMZERO(pst_fcs_cfg, OAL_SIZEOF(mac_fcs_cfg_stru));

    pst_fcs_cfg->st_src_chl = pst_mac_vap->st_channel;
    pst_fcs_cfg->st_dst_chl = *pst_dst_chl;

    mac_fcs_prepare_one_packet_cfg(pst_mac_vap, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

    /* ????FCS?????????? ?????????????????????????????????? */
    mac_fcs_start(pst_fcs_mgr, pst_fcs_cfg, 0, HAL_TX_FAKE_QUEUE_BGSCAN_ID);
    mac_fcs_release(pst_fcs_mgr);

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_switch_channel_off::switch src channel[%d] to dst channel[%d].}",
                  pst_mac_vap->st_channel.uc_chan_number, pst_fcs_cfg->st_dst_chl.uc_chan_number);

    return OAL_SUCC;
}


oal_uint32  dmac_switch_channel_off_enhanced(
                mac_device_stru     *pst_mac_device,
                mac_vap_stru        *pst_mac_vap1,
                mac_vap_stru        *pst_mac_vap2,
                mac_channel_stru    *pst_dst_chl,
                oal_uint16           us_protect_time)
{
    mac_fcs_mgr_stru               *pst_fcs_mgr;
    mac_fcs_cfg_stru               *pst_fcs_cfg;
    mac_vap_stru                   *pst_vap_sta;
#ifdef _PRE_WLAN_FEATURE_DBAC
    mac_vap_stru                   *pst_current_chan_vap;   /* ????????????VAP */
    oal_uint8                       uc_fake_q_id = 0;
#endif

    /* ???????????????????????????????????????????????? */
    if (pst_mac_vap1->st_channel.en_bandwidth >= pst_mac_vap2->st_channel.en_bandwidth)
    {
        pst_mac_device->st_home_channel = pst_mac_vap1->st_channel;
    }
    else
    {
        pst_mac_device->st_home_channel = pst_mac_vap2->st_channel;
    }

    /* ????????VAP?????? */
    dmac_vap_pause_tx(pst_mac_vap1);
    dmac_vap_pause_tx(pst_mac_vap2);

    pst_fcs_mgr = dmac_fcs_get_mgr_stru(pst_mac_device);
    pst_fcs_cfg = &(pst_mac_device->st_fcs_cfg);
    OAL_MEMZERO(pst_fcs_cfg, OAL_SIZEOF(mac_fcs_cfg_stru));

    pst_fcs_cfg->st_dst_chl = *pst_dst_chl;

    OAM_WARNING_LOG2(0, OAM_SF_DBAC, "{dmac_switch_channel_off_enhanced::curr hal chan[%d], dst channel[%d].}",
                  pst_mac_device->pst_device_stru->uc_current_chan_number,
                  pst_fcs_cfg->st_dst_chl.uc_chan_number);

#ifdef _PRE_WLAN_FEATURE_DBAC
    /* ????????????????????one packet */
    if (mac_is_dbac_running(pst_mac_device))
    {
        /* ????DBAC?????? */
        dmac_alg_dbac_pause(pst_mac_device);

        if (pst_mac_device->pst_device_stru->uc_current_chan_number == pst_mac_vap1->st_channel.uc_chan_number)
        {
            pst_current_chan_vap = pst_mac_vap1;
        }
        else
        {
            pst_current_chan_vap = pst_mac_vap2;
        }

        dmac_alg_get_dbac_fakeq_id(pst_current_chan_vap, &uc_fake_q_id);

        pst_fcs_cfg->st_src_chl = pst_current_chan_vap->st_channel;
        mac_fcs_prepare_one_packet_cfg(pst_current_chan_vap, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

        OAM_WARNING_LOG2(pst_current_chan_vap->uc_vap_id, OAM_SF_DBAC, "switch chan off when dbac running. curr chan num:%d, fake_q_id:%d",
                        pst_current_chan_vap->st_channel.uc_chan_number, uc_fake_q_id);
        if (pst_mac_device->pst_device_stru->uc_current_chan_number != pst_current_chan_vap->st_channel.uc_chan_number)
        {
            OAM_WARNING_LOG2(0, OAM_SF_DBAC, "switch chan off when dbac running. hal chan num:%d, curr vap chan num:%d. not same,do not send protect frame",
                            pst_mac_device->pst_device_stru->uc_current_chan_number,
                            pst_current_chan_vap->st_channel.uc_chan_number);

            pst_fcs_cfg->st_one_packet_cfg.en_protect_type = HAL_FCS_PROTECT_TYPE_NONE;
        }

        mac_fcs_start(pst_fcs_mgr, pst_fcs_cfg, 0, uc_fake_q_id);
        mac_fcs_release(pst_fcs_mgr);
    }
    else
#endif
    {
        /* ??????STA????????????????one packet */
        if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap1->en_vap_mode && WLAN_VAP_MODE_BSS_STA == pst_mac_vap2->en_vap_mode)
        {
            /* ????VAP1??fcs???? */
            pst_fcs_cfg->st_src_chl = pst_mac_vap1->st_channel;
            mac_fcs_prepare_one_packet_cfg(pst_mac_vap1, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

            /* ????VAP2??fcs???? */
            pst_fcs_cfg->st_src_chl2 = pst_mac_vap2->st_channel;
            mac_fcs_prepare_one_packet_cfg(pst_mac_vap2, &(pst_fcs_cfg->st_one_packet_cfg2), us_protect_time);
            pst_fcs_cfg->st_one_packet_cfg2.us_timeout = MAC_FCS_DEFAULT_PROTECT_TIME_OUT2;     /* ??????????one packet?????????????????????????? */

            mac_fcs_start_enhanced(pst_fcs_mgr, pst_fcs_cfg);
            mac_fcs_release(pst_fcs_mgr);
        }
        /* ????STA+GO????????????STA??????one packet */
        else
        {
            if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap1->en_vap_mode)
            {
                pst_vap_sta = pst_mac_vap1;
            }
            else
            {
                pst_vap_sta = pst_mac_vap2;
            }

            pst_fcs_cfg->st_src_chl = pst_vap_sta->st_channel;
            mac_fcs_prepare_one_packet_cfg(pst_vap_sta, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

            /* ????FCS?????????? ?????????????????????????????????? */
            mac_fcs_start(pst_fcs_mgr, pst_fcs_cfg, 0, HAL_TX_FAKE_QUEUE_BGSCAN_ID);
            mac_fcs_release(pst_fcs_mgr);
        }
    }

    return OAL_SUCC;
}



oal_void  dmac_scan_switch_channel_off(mac_device_stru *pst_mac_device)
{
    mac_vap_stru                   *pst_mac_vap;    /* ????????????VAP */
    wlan_scan_mode_enum_uint8       en_scan_mode;
    dmac_vap_stru                  *pst_dmac_vap;   /* ??????????VAP */
    oal_uint8                       uc_up_vap_num;
    oal_uint32                      ul_ret;
    mac_vap_stru                   *pst_mac_vap2;

    en_scan_mode = pst_mac_device->st_scan_params.en_scan_mode;

    if (en_scan_mode >= WLAN_SCAN_MODE_BUTT)
    {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_switch_channel_off::scan mode[%d] is invalid.}", en_scan_mode);
        return;
    }

    /* ??????CCA?????????????????????????????? */
    if (WLAN_SCAN_MODE_BACKGROUND_CCA == en_scan_mode)
    {
        //OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_switch_channel_off::CCA ALG scan, don't need switch channel.}");
        return;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_switch_channel_off::pst_dmac_vap null.}");
        return;
    }

    /* ?????????? ???????? */
    dmac_scan_switch_channel_notify_alg(pst_dmac_vap, &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]));

    /* ????????????PNO??????????(??: PNO????????????????????????????????) ?????????? */
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (!pst_mac_device->st_scan_params.en_need_switch_back_home_channel && ((WLAN_SCAN_MODE_FOREGROUND == en_scan_mode) || (WLAN_SCAN_MODE_BACKGROUND_PNO == en_scan_mode)))
#else
    if ((WLAN_SCAN_MODE_FOREGROUND == en_scan_mode) || (WLAN_SCAN_MODE_BACKGROUND_PNO == en_scan_mode))
#endif
    {
        /* ????????????????????????1M dbb scaling??11M???? */
    #if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        dmac_config_update_dsss_scaling_reg(DMAC_ALG_TPC_FAR_DISTANCE);
    #endif  /* _PRE_PLAT_FEATURE_CUSTOMIZE */
    #endif

    #ifdef _PRE_WLAN_FEATURE_PROXYSTA
        if (mac_is_proxysta_enabled(pst_mac_device) && mac_vap_is_vsta(&pst_dmac_vap->st_vap_base_info))
        {
            /* proxysta???????? */
            OAM_INFO_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_switch_channel_off::proxysta DO NOT switch channnel.}");
            return;
        }
    #endif

        OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_DBAC, "dmac_scan_switch_channel_off::scan mode[%d], switch channel to %d",
                        en_scan_mode,pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx].uc_chan_number);
        /* ????????????PNO???????????? ????????home????????????????FIFO */
        dmac_mgmt_switch_channel(pst_mac_device, &pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx], OAL_FALSE);
        return;
    }

    /****** ??????????????FCS?????????? ************/
    uc_up_vap_num = (oal_uint8)mac_device_calc_up_vap_num(pst_mac_device);

    if (1 == uc_up_vap_num)
    {
        if (OAL_SUCC != mac_device_find_up_vap(pst_mac_device, &pst_mac_vap))
        {
            OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_switch_channel_off::up vap is null ptr.}");
            return;
        }

        /* ??????VAP???? */
        dmac_vap_pause_tx(pst_mac_vap);

        /* ???????? */
        dmac_switch_channel_off(pst_mac_device,
                                pst_mac_vap,
                                &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]),
                                pst_mac_device->st_scan_params.us_scan_time);
    }
    else if (2 == uc_up_vap_num)
    {
        ul_ret = mac_device_find_2up_vap(pst_mac_device, &pst_mac_vap, &pst_mac_vap2);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG0(0, OAM_SF_SCAN, "mac_device_find_2up_vap return fail.");
            return;
        }

        dmac_switch_channel_off_enhanced(pst_mac_device,
                                         pst_mac_vap,
                                         pst_mac_vap2,
                                         &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]),
                                         pst_mac_device->st_scan_params.us_scan_time);
    }
    else if(uc_up_vap_num >= 3)
    {
        oal_uint8       uc_vap_idx;
        mac_vap_stru   *pst_vap;

        mac_device_find_up_vap(pst_mac_device, &pst_mac_vap);

        for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++)
        {
            pst_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);

            if (MAC_VAP_STATE_UP == pst_vap->en_vap_state || MAC_VAP_STATE_PAUSE == pst_vap->en_vap_state)
            {
                dmac_vap_pause_tx(pst_vap);
            }
        }

        dmac_switch_channel_off(pst_mac_device,
                                pst_mac_vap,
                                &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]),
                                pst_mac_device->st_scan_params.us_scan_time);
    }
}


oal_void  dmac_scan_switch_channel_back(mac_device_stru *pst_mac_device)
{
    if (mac_is_dbac_running(pst_mac_device))
    {
        if (OAL_FALSE == pst_mac_device->st_scan_params.en_working_in_home_chan)
        {
            /* clear fifo when dbac resume*/
            dmac_tx_clear_tx_queue(pst_mac_device->pst_device_stru);
        }
        /* dbac????????????dbac????dbac???????????????? */
        dmac_alg_dbac_resume(pst_mac_device);
        return;
    }

    if (OAL_FALSE == pst_mac_device->st_scan_params.en_working_in_home_chan)
    {
        /* ????????VAP?????????? */
        OAM_WARNING_LOG4(0, OAM_SF_SCAN, "{dmac_scan_switch_channel_back::switch home channel[%d], band[%d], bw[%d], restore tx queue from %d.}",
                      pst_mac_device->st_home_channel.uc_chan_number,
                      pst_mac_device->st_home_channel.en_band,
                      pst_mac_device->st_home_channel.en_bandwidth,
                      HAL_TX_FAKE_QUEUE_BGSCAN_ID);

        /* ????????????????????????????????????????????????FIFO */
        dmac_mgmt_switch_channel(pst_mac_device, &(pst_mac_device->st_home_channel), OAL_TRUE);

        /* ?????????????????????????????????????? */
        dmac_tx_restore_tx_queue(pst_mac_device->pst_device_stru, HAL_TX_FAKE_QUEUE_BGSCAN_ID);

        /* ????home?????????????????? */
        dmac_vap_resume_tx_by_chl(pst_mac_device, &(pst_mac_device->st_home_channel));
    }

}


OAL_STATIC oal_bool_enum_uint8  dmac_scan_need_switch_home_channel(mac_device_stru *pst_mac_device)
{
    mac_scan_req_stru       *pst_scan_params;

    pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ???????????????????????? */
    /* en_need_switch_back_home_channel ???????????? */
    if (OAL_TRUE == pst_mac_device->st_scan_params.en_need_switch_back_home_channel)
    {
        if (0 == pst_scan_params->uc_scan_channel_interval)
        {
            OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_need_switch_home_channel::scan_channel_interval is 0, set default value 6!}");
            pst_scan_params->uc_scan_channel_interval = MAC_SCAN_CHANNEL_INTERVAL_DEFAULT;
        }

        return (0 == pst_mac_device->uc_scan_chan_idx % pst_scan_params->uc_scan_channel_interval);
    }

    return OAL_FALSE;
}

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)

OAL_STATIC oal_uint32  dmac_scan_check_2g_scan_results(mac_device_stru *pst_mac_device, mac_vap_stru *pst_vap, wlan_channel_band_enum_uint8 en_next_band)
{

    if (((WLAN_SCAN_MODE_FOREGROUND == pst_mac_device->st_scan_params.en_scan_mode)
         || (WLAN_SCAN_MODE_BACKGROUND_STA == pst_mac_device->st_scan_params.en_scan_mode))
         && (OAL_TRUE != pst_mac_device->st_scan_params.bit_is_p2p0_scan))
    {
        if (WLAN_BAND_2G == pst_mac_device->st_scan_params.uc_last_channel_band)
        {
            if (pst_mac_device->en_scan_curr_chan_find_bss_flag == OAL_TRUE)
            {
                pst_mac_device->uc_scan_ap_num_in_2p4++;
                pst_mac_device->en_scan_curr_chan_find_bss_flag = OAL_FALSE;
            }

            if ((WLAN_BAND_5G == en_next_band)
               || (pst_mac_device->uc_scan_chan_idx >= pst_mac_device->st_scan_params.uc_channel_nums))
            {
                if ((pst_mac_device->uc_scan_ap_num_in_2p4 <= 2) || ((pst_mac_device->uc_scan_count % 30) == 0))
                {
                    OAM_INFO_LOG2(0, OAM_SF_DBAC, "{dmac_scan_check_2g_scan_results::2.4G scan ap num = %d, scan_count = %d.}",
                                  pst_mac_device->uc_scan_ap_num_in_2p4,
                                  pst_mac_device->uc_scan_count);
#ifdef _PRE_WLAN_DFT_STAT
                    dmac_dft_report_all_ota_state(pst_vap);
#endif
                }
            }
        }
    }

    return OAL_SUCC;
}
#endif /* _PRE_PRODUCT_ID_HI110X_DEV */


OAL_STATIC oal_void dmac_scan_update_dfs_channel_scan_param(mac_device_stru     *pst_mac_device,
                                                            mac_channel_stru    *pst_mac_channel,
                                                            oal_uint16          *pus_scan_time,
                                                            oal_bool_enum_uint8 *pen_send_probe_req)
{
    mac_vap_stru        *pst_mac_vap;
    mac_scan_req_stru   *pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ????????????????????probe req?????????????????????????? */
    if (OAL_FALSE == mac_is_dfs_channel(pst_mac_channel->en_band, pst_mac_channel->uc_chan_number))
    {
        *pen_send_probe_req = OAL_TRUE;
        *pus_scan_time      = pst_scan_params->us_scan_time;
        return;
    }

    /* ??????????????????????????AP ????????????????????????????????????????????????????probe req ???????? */
    pst_mac_vap = mac_res_get_mac_vap(pst_scan_params->uc_vap_id);
    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_WARNING_LOG1(pst_scan_params->uc_vap_id, OAM_SF_SCAN,
                        "{dmac_scan_update_dfs_channel_scan_param::get vap [%d] fail.}",
                        pst_scan_params->uc_vap_id);
        *pen_send_probe_req = OAL_FALSE;
        *pus_scan_time      = WLAN_DEFAULT_PASSIVE_SCAN_TIME;
        return;
    }
    if ((MAC_VAP_STATE_UP == pst_mac_vap->en_vap_state || MAC_VAP_STATE_PAUSE == pst_mac_vap->en_vap_state)
        && (pst_mac_channel->uc_chan_number == pst_mac_vap->st_channel.uc_chan_number))
    {
        *pen_send_probe_req = OAL_TRUE;
        *pus_scan_time      = pst_scan_params->us_scan_time;
        return;
    }

    /* ??????????????????????????probe req ????????????????60ms
     * ??????????????????????????????????????????????????AP??
     *                     ????????????????
     *                     ??????????????????????????AP??
     *                     ??????probe req ????????????????20ms
     */
    if (pst_scan_params->uc_curr_channel_scan_count == 0)
    {
        /* ??????????????????????????probe req ??????????????60ms */
        *pen_send_probe_req = OAL_FALSE;
        *pus_scan_time      = WLAN_DEFAULT_PASSIVE_SCAN_TIME;
    }
    else
    {
        if (pst_mac_device->en_scan_curr_chan_find_bss_flag == OAL_TRUE)
        {
            /* ????????????????????????????????????????AP,
             * ????????????probe req????????????????????????
             */
            *pen_send_probe_req = OAL_TRUE;
            *pus_scan_time      = pst_scan_params->us_scan_time;
        }
        else
        {
            /* ??????????????????????????????????????????AP,
             * ????????????????0
             */
            *pen_send_probe_req = OAL_FALSE;
            *pus_scan_time      = 0;
        }
    }

    return;
}


OAL_STATIC oal_void dmac_scan_do_next_channel_scan(mac_device_stru  *pst_mac_device,
                                                   dmac_vap_stru    *pst_dmac_vap,
                                                   mac_channel_stru *pst_next_scan_channel)
{
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_to_dmac_device_stru *pst_hal_device = OAL_PTR_NULL;

    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "dmac_btcoex_scan_begin:pst_hal_device is null");
        return;
    }
#endif
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    dmac_scan_check_2g_scan_results(pst_mac_device, &(pst_dmac_vap->st_vap_base_info), pst_next_scan_channel->en_band);
    pst_mac_device->st_scan_params.uc_last_channel_band = pst_next_scan_channel->en_band;
#endif
    pst_mac_device->en_scan_curr_chan_find_bss_flag = OAL_FALSE;       /* ????????????BSS ???????? */

    /* ???????????????? */
    dmac_scan_switch_channel_notify_alg(pst_dmac_vap, pst_next_scan_channel);

    //dmac_tx_clear_tx_queue(pst_mac_device->pst_device_stru);

    /* ?????????????? */ /* ??????????????????????????????????????FIFO */
    dmac_mgmt_switch_channel(pst_mac_device, pst_next_scan_channel, OAL_TRUE);
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if(HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
    {
        GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_BEGIN;
        OAM_WARNING_LOG0(0, OAM_SF_COEX, "{dmac_scan_do_next_channel_scan:: normal scan begin delay by btcoex!}");
    }
    else
#endif
    {
        dmac_scan_begin(pst_mac_device);
    }
}


OAL_STATIC oal_uint32  dmac_scan_curr_channel_scan_time_out(void *p_arg)
{
    mac_device_stru         *pst_mac_device = (mac_device_stru *)p_arg;
    mac_scan_req_stru       *pst_scan_params;
    mac_channel_stru        *pst_next_scan_channel = OAL_PTR_NULL;
    dmac_vap_stru           *pst_dmac_vap;
    hal_to_dmac_device_stru *pst_hal_device = OAL_PTR_NULL;
    oal_uint8                uc_do_meas;
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "dmac_btcoex_scan_begin:pst_hal_device is null");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
    /* ???????????? */
    pst_scan_params = &(pst_mac_device->st_scan_params);

    /* ?????????????????????? */
    pst_scan_params->uc_curr_channel_scan_count++;

    /* ?????????????????????????????????????????????????? */
    if (pst_scan_params->uc_curr_channel_scan_count >= pst_scan_params->uc_max_scan_count_per_channel)
    {
        /* ???????????????????????????????????????????????????????????????????????? */
        uc_do_meas = pst_mac_device->st_scan_params.uc_scan_func & (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS);
        if (uc_do_meas)
        {
            /* ??????CCA???????????????????????????????????????? */
            if (WLAN_SCAN_MODE_BACKGROUND_CCA != pst_mac_device->st_scan_params.en_scan_mode)
            {
                /* ???????????????? */
                dmac_scan_report_channel_statistics_result(pst_mac_device, pst_mac_device->uc_scan_chan_idx);

                /* ???????????????????????????????????????????? */
                OAL_MEMZERO(&(pst_mac_device->st_chan_result), OAL_SIZEOF(mac_scan_chan_stats_stru));
            }
        }

        pst_mac_device->uc_scan_chan_idx += 1;              /* ???????? */
        pst_scan_params->uc_curr_channel_scan_count = 0;    /* ?????????????????? */
    }
    else
    {
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        if(HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
        {
            GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_BEGIN;
            OAM_WARNING_LOG0(0, OAM_SF_COEX, "{dmac_btcoex_scan_begin:: normal scan begin delay by btcoex!}");
        }
        else
#endif
        {
            /* ???????????????????????????????????????????????? */
            dmac_scan_begin(pst_mac_device);
        }
        return OAL_SUCC;
    }

    /* ??????????????dmac vap???????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(pst_mac_device->st_scan_params.uc_vap_id,OAM_SF_PWR,"{dmac_scan_curr_channel_scan_time_out::mac_res_get_dmac_vap fail}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????????????????????????? */
    if (pst_mac_device->uc_scan_chan_idx >= pst_mac_device->st_scan_params.uc_channel_nums)
    {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        dmac_scan_check_2g_scan_results(pst_mac_device, &(pst_dmac_vap->st_vap_base_info), pst_mac_device->st_scan_params.uc_last_channel_band);
#endif
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        if (HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
        {
            GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_END;
        }
        else
#endif
        {
            dmac_scan_end(pst_mac_device);
        }
        return OAL_SUCC;
    }

    if ((WLAN_SCAN_MODE_FOREGROUND == pst_mac_device->st_scan_params.en_scan_mode) ||
            (WLAN_SCAN_MODE_BACKGROUND_PNO == pst_mac_device->st_scan_params.en_scan_mode))
    {
        /* ?????????????????????????? */
        pst_next_scan_channel = &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]);
        dmac_scan_do_next_channel_scan(pst_mac_device, pst_dmac_vap, pst_next_scan_channel);
    }
    else
    {
        /* ?????????????????????????????????????????????????????????????????????????? */
        if (OAL_TRUE == dmac_scan_need_switch_home_channel(pst_mac_device))
        {
#ifdef _PRE_WLAN_FEATURE_BTCOEX
            if (HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
            {
                GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_WAIT;
            }
            else
#endif
            {
                dmac_scan_switch_home_channel_work(pst_mac_device);
            }
            pst_mac_device->st_scan_params.en_working_in_home_chan = OAL_TRUE;
        }
        else
        {
            pst_mac_device->st_scan_params.en_working_in_home_chan = OAL_FALSE;
            /* ?????????????????????????? */
            pst_next_scan_channel = &(pst_mac_device->st_scan_params.ast_channel_list[pst_mac_device->uc_scan_chan_idx]);
            dmac_scan_do_next_channel_scan(pst_mac_device, pst_dmac_vap, pst_next_scan_channel);
        }
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void dmac_pno_scan_send_probe_with_ssid(mac_device_stru *pst_mac_device, oal_uint8 uc_band)
{
    mac_pno_sched_scan_mgmt_stru *pst_pno_sched_scan_mgmt;
    dmac_vap_stru                *pst_dmac_vap;
    oal_uint8                     uc_band_tmp;
    oal_uint8                     uc_loop;
    oal_uint32                    ul_ret;

    if(OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_pno_scan_send_probe_with_ssid::pst_mac_device is null.}");
        return;
    }

    pst_pno_sched_scan_mgmt = pst_mac_device->pst_pno_sched_scan_mgmt;

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_pno_scan_send_probe_with_ssid::pst_dmac_vap null.}");
        return;
    }

    
    if(OAL_PTR_NULL == pst_dmac_vap->st_vap_base_info.pst_mib_info)
    {
        OAM_ERROR_LOG4(0, OAM_SF_SCAN, "{dmac_pno_scan_send_probe_with_ssid:: vap mib info is null,uc_vap_id[%d], p_fn_cb[%p], uc_scan_func[%d], uc_curr_channel_scan_count[%d].}",
                   pst_mac_device->st_scan_params.uc_vap_id,
                   pst_mac_device->st_scan_params.p_fn_cb,
                   pst_mac_device->st_scan_params.uc_scan_func,
                   pst_mac_device->st_scan_params.uc_curr_channel_scan_count);
        return;
    }


    /* ??????????????????????????vap??band??????????5G??11b */
    uc_band_tmp = pst_dmac_vap->st_vap_base_info.st_channel.en_band;

    pst_dmac_vap->st_vap_base_info.st_channel.en_band = uc_band;

    for(uc_loop = 0; uc_loop < pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.l_ssid_count; uc_loop++)
    {
        if(OAL_TRUE == pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.ast_match_ssid_set[uc_loop].en_scan_ssid)
        {
            /* ??????????SSID,??????SSID???? */
            ul_ret = dmac_scan_send_probe_req_frame(pst_dmac_vap, BROADCAST_MACADDR, (oal_int8 *)pst_pno_sched_scan_mgmt->st_pno_sched_scan_params.ast_match_ssid_set[uc_loop].auc_ssid);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{dmac_pno_scan_send_probe_with_ssid::dmac_scan_send_probe_req_frame failed[%u].}", ul_ret);
            }
        }
    }

    pst_dmac_vap->st_vap_base_info.st_channel.en_band = uc_band_tmp;
    return;
}


oal_void dmac_scan_begin(mac_device_stru *pst_mac_device)
{
    mac_scan_req_stru               *pst_scan_params;
    dmac_vap_stru                   *pst_dmac_vap;
    oal_uint32                       ul_ret;
    oal_uint8                        uc_band;
    oal_uint8                        uc_do_bss_scan;
    oal_uint8                        uc_do_meas;
    oal_uint8                        uc_loop;
    oal_uint8                        uc_do_p2p_listen = 0;
    oal_bool_enum_uint8              en_send_probe_req;
    oal_uint16                       us_scan_time;

    /* ???????????? */
    pst_scan_params = &(pst_mac_device->st_scan_params);

    uc_band          = pst_scan_params->ast_channel_list[pst_mac_device->uc_scan_chan_idx].en_band;
    uc_do_bss_scan   = pst_scan_params->uc_scan_func & MAC_SCAN_FUNC_BSS;
    uc_do_p2p_listen = pst_scan_params->uc_scan_func & MAC_SCAN_FUNC_P2P_LISTEN;

    /* ?????????????????????????????????????????????????? */
    uc_do_meas = pst_scan_params->uc_scan_func & (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS);
    if (uc_do_meas)
    {
        /* ???????????????? */
        hal_set_ch_statics_period(pst_mac_device->pst_device_stru, DMAC_SCAN_CHANNEL_STATICS_PERIOD_US);
        hal_set_ch_measurement_period(pst_mac_device->pst_device_stru, DMAC_SCAN_CHANNEL_MEAS_PERIOD_MS);
        hal_enable_ch_statics(pst_mac_device->pst_device_stru, 1);
    }
    pst_dmac_vap = mac_res_get_dmac_vap(pst_scan_params->uc_vap_id);
#ifdef _PRE_WLAN_FEATURE_11K
    if(OAL_TRUE == pst_dmac_vap->bit_11k_enable)
    {
        if (WLAN_SCAN_MODE_RRM_BEACON_REQ == pst_scan_params->en_scan_mode)
        {
            hal_vap_tsf_get_64bit(pst_dmac_vap->pst_hal_vap, (oal_uint32 *)&(pst_dmac_vap->pst_rrm_info->auc_act_meas_start_time[4]), (oal_uint32 *)&(pst_dmac_vap->pst_rrm_info->auc_act_meas_start_time[0]));
            OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{dmac_scan_begin::update start tsf ok, vap id[%d].}", pst_scan_params->uc_vap_id);
        }
    }
#endif

    /* dfs?????????????????????????????????????? */
    dmac_scan_update_dfs_channel_scan_param(pst_mac_device,
                                            &(pst_scan_params->ast_channel_list[pst_mac_device->uc_scan_chan_idx]),
                                            &us_scan_time,
                                            &en_send_probe_req);

    /* ACTIVE??????????????RPOBE REQ?? */
    if (uc_do_bss_scan && (WLAN_SCAN_TYPE_ACTIVE == pst_scan_params->en_scan_type)
        && (OAL_TRUE == en_send_probe_req))
    {
        /* PNO????SSID????,????????16??SSID */
        if(WLAN_SCAN_MODE_BACKGROUND_PNO == pst_mac_device->st_scan_params.en_scan_mode)
        {
            dmac_pno_scan_send_probe_with_ssid(pst_mac_device, uc_band);
        }

        /* ??????????????probe req???????? */
        for (uc_loop = 0; uc_loop < pst_scan_params->uc_max_send_probe_req_count_per_channel; uc_loop++)
        {
            ul_ret = dmac_scan_send_bcast_probe(pst_mac_device, uc_band, uc_loop);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{dmac_scan_begin::dmac_scan_send_bcast_probe failed[%d].}", ul_ret);
            }
        }
    }
#ifdef _PRE_WLAN_FEATURE_11K
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV)
     if(OAL_TRUE == pst_dmac_vap->bit_11k_enable)
     {
         if (WLAN_SCAN_MODE_RRM_BEACON_REQ == pst_scan_params->en_scan_mode
            && WLAN_SCAN_TYPE_PASSIVE == pst_scan_params->en_scan_type)
         {
             hal_disable_non_frame_mgmt_filter(pst_mac_device->pst_device_stru);
         }
     }
#endif
#endif
    /* ????????????????probe request????????????????,????????SSID????????????,????????????????????,???????????????????????? */
    /* ?????????????? */
    FRW_TIMER_CREATE_TIMER(&pst_mac_device->st_scan_timer,
                           dmac_scan_curr_channel_scan_time_out,
                           us_scan_time,
                           pst_mac_device,
                           OAL_FALSE,
                           OAM_MODULE_ID_DMAC,
                           pst_mac_device->ul_core_id);

    /* p2p???????????? */
    /* p2p listen??????????VAP??????????probe rsp??(DSSS ie, ht ie)??????listen?????????? */
    if (uc_do_p2p_listen)
    {
        pst_mac_device->st_p2p_vap_channel = pst_dmac_vap->st_vap_base_info.st_channel;

        pst_dmac_vap->st_vap_base_info.st_channel = pst_scan_params->ast_channel_list[0];

#ifdef _PRE_WLAN_FEATURE_STA_PM
        /* ????p2p ci ??????P2P ????????????????????RF?????? */
        dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
#endif
    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if (!IS_LEGACY_VAP(&(pst_dmac_vap->st_vap_base_info)))
    {
        /* Notify Bt the P2P Scan Start state */
        hal_set_btcoex_soc_gpreg0(OAL_TRUE, BIT14, 14);
        hal_coex_sw_irq_set(HAL_COEX_SW_IRQ_BT);
    }
#endif
    return;
}


oal_void dmac_scan_end(mac_device_stru *pst_mac_device)
{
    dmac_vap_stru              *pst_dmac_vap;
    wlan_scan_mode_enum_uint8   en_scan_mode = WLAN_SCAN_MODE_BUTT;
    oal_uint8                   uc_do_p2p_listen;

    /* ????dmac vap */
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap) {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_end::pst_dmac_vap is null.}");

        /* ????device?????????????????? */
        pst_mac_device->en_curr_scan_state = MAC_SCAN_STATE_IDLE;
        return;
    }

    /* listen??????vap??????listen??????listen?????????????? */
    uc_do_p2p_listen = pst_mac_device->st_scan_params.uc_scan_func & MAC_SCAN_FUNC_P2P_LISTEN;
    if (uc_do_p2p_listen)
    {
        pst_dmac_vap->st_vap_base_info.st_channel = pst_mac_device->st_p2p_vap_channel;
    }

    /* ??????????????????????????mac addr??????????vap??????mac addr */
    dmac_scan_set_vap_mac_addr_by_scan_state(pst_mac_device, OAL_FALSE);

    /* ???????????? */
    en_scan_mode = pst_mac_device->st_scan_params.en_scan_mode;

    /* ??????????????????????cca?????????????????????????????? */
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (((en_scan_mode < WLAN_SCAN_MODE_BUTT) && (en_scan_mode != WLAN_SCAN_MODE_BACKGROUND_CCA)
           && (en_scan_mode != WLAN_SCAN_MODE_BACKGROUND_PNO))
           && (mac_is_proxysta_enabled(pst_mac_device)
           && (pst_mac_device->st_scan_params.uc_channel_nums != 1
           || pst_mac_device->st_scan_params.ast_channel_list[0].uc_chan_number != pst_mac_device->pst_device_stru->uc_current_chan_number)))
#else
    if (((en_scan_mode < WLAN_SCAN_MODE_BUTT) && (en_scan_mode != WLAN_SCAN_MODE_FOREGROUND) && (en_scan_mode != WLAN_SCAN_MODE_BACKGROUND_CCA)
           && (en_scan_mode != WLAN_SCAN_MODE_BACKGROUND_PNO)))
#endif
    {
        dmac_scan_switch_channel_back(pst_mac_device);
        dmac_scan_switch_channel_notify_alg(pst_dmac_vap, &(pst_mac_device->st_home_channel));
    }
    else
    {
        OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_end::switch back not need.}");
    }

    /* ?????????????????????????????????? */
    switch (en_scan_mode)
    {
        case WLAN_SCAN_MODE_FOREGROUND:
        case WLAN_SCAN_MODE_BACKGROUND_STA:
        case WLAN_SCAN_MODE_BACKGROUND_AP:
        {
#ifdef _PRE_WLAN_FEATURE_STA_PM
            /* ??????????*/
            PM_WLAN_DumpSleepCnt();
#endif
            /* ???????????????????????????????? */
            (oal_void)dmac_scan_proc_scan_complete_event(pst_dmac_vap, MAC_SCAN_SUCCESS);
            break;
        }
        case WLAN_SCAN_MODE_BACKGROUND_OBSS:
        {
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
            dmac_scan_proc_obss_scan_complete_event(pst_dmac_vap);
#endif
#endif
            break;
        }
        case WLAN_SCAN_MODE_BACKGROUND_CCA:
        {
            /* CCA??????????????????????device???????????????????????????????????? */
            if (OAL_PTR_NULL != pst_mac_device->st_scan_params.p_fn_cb)
            {
                pst_mac_device->st_scan_params.p_fn_cb(pst_mac_device);
            }
            break;
        }
        case WLAN_SCAN_MODE_BACKGROUND_PNO:
        {
            /* ???????????????????????? */
            hal_set_rx_filter(pst_mac_device->pst_device_stru, 0x37B9EEFA);

            /* ??????????????????ssid??????????????????????; ???????????? */
            if (OAL_TRUE == pst_mac_device->pst_pno_sched_scan_mgmt->en_is_found_match_ssid)
            {
                /* ???????????????????????????????? */
                (oal_void)dmac_scan_proc_scan_complete_event(pst_dmac_vap, MAC_SCAN_PNO);

                /* ????PNO?????????????? */
                OAL_MEM_FREE(pst_mac_device->pst_pno_sched_scan_mgmt, OAL_TRUE);
                pst_mac_device->pst_pno_sched_scan_mgmt = OAL_PTR_NULL;
            }

            /* ??????????????,??????????device???????????? */
        #ifdef _PRE_WLAN_FEATURE_STA_PM
            hal_disable_sta_tsf_tbtt(pst_dmac_vap->pst_hal_vap);
            PM_WLAN_PsmHandle(PM_STA0_ID, PM_WLAN_SHUTDOWN_PROCESS);
        #endif

            break;
        }
        case WLAN_SCAN_MODE_BACKGROUND_CSA:
        {
            g_csa_scan_flag = OAL_FALSE;
            OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_end::scan_mode BACKGROUND_CSA}");
            break;
        }
#ifdef _PRE_WLAN_FEATURE_11K
        case WLAN_SCAN_MODE_RRM_BEACON_REQ:
        {
            /* ?????????????????????????????? */
            /* ???????????????????????????? */
            dmac_rrm_encap_and_send_bcn_rpt(pst_dmac_vap);

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV)
            hal_enable_non_frame_filter(pst_mac_device->pst_device_stru);
#endif
            break;
        }
#endif
        default:
        {
            OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_end::scan_mode[%d] error.}", en_scan_mode);
            break;
        }
    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    /* CCA???????? */
    if (WLAN_SCAN_MODE_BACKGROUND_CCA != en_scan_mode)
    {
        hal_set_btcoex_soc_gpreg1(OAL_FALSE, BIT1, 1);   // ????????????
        hal_set_btcoex_soc_gpreg1(OAL_FALSE, BIT0, 0);   // Wifi??????????????????????????????????
        hal_coex_sw_irq_set(HAL_COEX_SW_IRQ_BT);
    }
#endif

    /* ????device?????????????????? */
    pst_mac_device->en_curr_scan_state = MAC_SCAN_STATE_IDLE;
    pst_mac_device->st_scan_params.en_working_in_home_chan = OAL_FALSE;

    if((OAL_TRUE == pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_waiting_to_shift_channel)
       && (0 == pst_dmac_vap->st_vap_base_info.st_ch_switch_info.uc_new_ch_swt_cnt))
    {
        /* ??????????tbtt????dmac_handle_tbtt_chan_mgmt_sta?????????????????? */
        pst_dmac_vap->st_vap_base_info.st_ch_switch_info.en_channel_swt_cnt_zero = OAL_TRUE;
    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if (!IS_LEGACY_VAP(&(pst_dmac_vap->st_vap_base_info)))
    {
        /* Notify Bt the P2P Scan End state */
        hal_set_btcoex_soc_gpreg0(OAL_FALSE, BIT14, 14);
        hal_coex_sw_irq_set(HAL_COEX_SW_IRQ_BT);
    }
#endif
    return;
}


oal_uint32  dmac_switch_same_channel_off(
                mac_device_stru     *pst_mac_device,
                mac_vap_stru        *pst_mac_vap,
                oal_uint16           us_protect_time)
{
    mac_fcs_mgr_stru         *pst_fcs_mgr;
    mac_fcs_cfg_stru         *pst_fcs_cfg;
#ifdef _PRE_WLAN_FEATURE_STA_PM
    mac_sta_pm_handler_stru  *pst_mac_sta_pm_handle;
#endif
    dmac_vap_stru            *pst_dmac_vap;

    if ((OAL_PTR_NULL == pst_mac_device) || (OAL_PTR_NULL == pst_mac_vap))
    {
        OAM_ERROR_LOG2(0, OAM_SF_SCAN, "dmac_switch_same_channel_off:pst_mac_device=%x,pst_mac_vap=%x",pst_mac_device,pst_mac_vap);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ????vap???? */
    dmac_vap_pause_tx(pst_mac_vap);

    pst_mac_device->st_home_channel = pst_mac_vap->st_channel;

    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        pst_dmac_vap = (dmac_vap_stru *)pst_mac_vap;
#ifdef _PRE_WLAN_FEATURE_STA_PM
        pst_mac_sta_pm_handle = (mac_sta_pm_handler_stru *)(pst_dmac_vap->pst_pm_handler);
        if (STA_PWR_SAVE_STATE_ACTIVE != STA_GET_PM_STATE(pst_mac_sta_pm_handle))
        {
            OAM_WARNING_LOG1(0,0,"dmac_switch_same_channel_off:pm state=%d",STA_GET_PM_STATE(pst_mac_sta_pm_handle));
            return OAL_SUCC;
        }
#endif
    }

    pst_fcs_mgr = dmac_fcs_get_mgr_stru(pst_mac_device);
    pst_fcs_cfg = &(pst_mac_device->st_fcs_cfg);

    OAL_MEMZERO(pst_fcs_cfg, OAL_SIZEOF(mac_fcs_cfg_stru));

    pst_fcs_cfg->st_src_chl = pst_mac_vap->st_channel;
    pst_fcs_cfg->st_dst_chl = pst_mac_vap->st_channel;

    mac_fcs_prepare_one_packet_cfg(pst_mac_vap, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

    /* ????FCS?????????? ?????????????????????????????????? */
    mac_fcs_start_same_channel(pst_fcs_mgr, pst_fcs_cfg, 0, HAL_TX_FAKE_QUEUE_BGSCAN_ID);
    mac_fcs_release(pst_fcs_mgr);
    return OAL_SUCC;
}


oal_uint32  dmac_switch_channel_off_enhanced_self_channel(
                mac_device_stru     *pst_mac_device,
                mac_vap_stru        *pst_mac_vap1,
                mac_vap_stru        *pst_mac_vap2,
                oal_uint16           us_protect_time)
{
    mac_fcs_mgr_stru               *pst_fcs_mgr;
    mac_fcs_cfg_stru               *pst_fcs_cfg;
    mac_vap_stru                   *pst_vap_sta;

    if ((OAL_PTR_NULL == pst_mac_device) || (OAL_PTR_NULL == pst_mac_vap1) || (OAL_PTR_NULL == pst_mac_vap2))
    {
        OAM_ERROR_LOG3(0, OAM_SF_SCAN, "dmac_switch_channel_off_enhanced_self_channel:pst_mac_device=%x,pst_mac_vap1=%x,pst_mac_vap2=%x"
                       ,pst_mac_device,pst_mac_vap1,pst_mac_vap2);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ???????????????????????????????????????????????? */
    if (pst_mac_vap1->st_channel.en_bandwidth >= pst_mac_vap2->st_channel.en_bandwidth)
    {
        pst_mac_device->st_home_channel = pst_mac_vap1->st_channel;
    }
    else
    {
        pst_mac_device->st_home_channel = pst_mac_vap2->st_channel;
    }

    /* ????????VAP?????? */
    dmac_vap_pause_tx(pst_mac_vap1);
    dmac_vap_pause_tx(pst_mac_vap2);

    pst_fcs_mgr = dmac_fcs_get_mgr_stru(pst_mac_device);
    pst_fcs_cfg = &(pst_mac_device->st_fcs_cfg);
    OAL_MEMZERO(pst_fcs_cfg, OAL_SIZEOF(mac_fcs_cfg_stru));

    OAM_WARNING_LOG2(0, OAM_SF_SCAN, "{dmac_switch_channel_off_enhanced::curr hal chan[%d], dst channel[%d].}",
                  pst_mac_device->pst_device_stru->uc_current_chan_number,
                  pst_fcs_cfg->st_dst_chl.uc_chan_number);

    /* ??????STA????????????????one packet */
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap1->en_vap_mode && WLAN_VAP_MODE_BSS_STA == pst_mac_vap2->en_vap_mode)
    {
        /* ????VAP1??fcs???? */
        pst_fcs_cfg->st_src_chl = pst_mac_vap1->st_channel;
        pst_fcs_cfg->st_dst_chl = pst_mac_vap1->st_channel;
        mac_fcs_prepare_one_packet_cfg(pst_mac_vap1, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

        /* ????VAP2??fcs???? */
        pst_fcs_cfg->st_src_chl2 = pst_mac_vap2->st_channel;
        mac_fcs_prepare_one_packet_cfg(pst_mac_vap2, &(pst_fcs_cfg->st_one_packet_cfg2), us_protect_time);
        pst_fcs_cfg->st_one_packet_cfg2.us_timeout = MAC_FCS_DEFAULT_PROTECT_TIME_OUT2;     /* ??????????one packet?????????????????????????? */

        mac_fcs_start_enhanced_same_channel(pst_fcs_mgr, pst_fcs_cfg);
        mac_fcs_release(pst_fcs_mgr);
    }
    /* ????STA+GO????????????STA??????one packet */
    else
    {
        if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap1->en_vap_mode)
        {
            pst_vap_sta = pst_mac_vap1;
        }
        else
        {
            pst_vap_sta = pst_mac_vap2;
        }

        pst_fcs_cfg->st_src_chl = pst_vap_sta->st_channel;
        pst_fcs_cfg->st_dst_chl = pst_vap_sta->st_channel;
        mac_fcs_prepare_one_packet_cfg(pst_vap_sta, &(pst_fcs_cfg->st_one_packet_cfg), us_protect_time);

        /* ????FCS?????????? ?????????????????????????????????? */
        mac_fcs_start_same_channel(pst_fcs_mgr, pst_fcs_cfg, 0, HAL_TX_FAKE_QUEUE_BGSCAN_ID);
        mac_fcs_release(pst_fcs_mgr);
    }

    return OAL_SUCC;
}


oal_void  dmac_scan_abort(mac_device_stru *pst_mac_device)
{
    if (MAC_SCAN_STATE_RUNNING != pst_mac_device->en_curr_scan_state)
    {
        return;
    }

    /* ?????????????? */
    if (OAL_TRUE == pst_mac_device->st_scan_timer.en_is_registerd)
    {
        FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_mac_device->st_scan_timer);
    }

    pst_mac_device->st_scan_params.en_abort_scan_flag = OAL_TRUE;
    dmac_scan_end(pst_mac_device);
    pst_mac_device->st_scan_params.en_abort_scan_flag = OAL_FALSE;

    OAM_WARNING_LOG0(0, OAM_SF_SCAN, "dmac_scan_abort: scan has been aborted");
}


OAL_STATIC oal_uint32 dmac_scan_get_ssid_ie_info(mac_device_stru *pst_mac_device, oal_int8 *pc_ssid, oal_uint8  uc_index)
{
    dmac_vap_stru     *pst_dmac_vap;

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);

    if (IS_LEGACY_VAP(&(pst_dmac_vap->st_vap_base_info)))
    {
        /* ????????????????probe req????ssid???? */
        oal_memcopy(pc_ssid, pst_mac_device->st_scan_params.ast_mac_ssid_set[uc_index].auc_ssid, WLAN_SSID_MAX_LEN);
    }
    else
    {
        /* P2P ??????????????????????ssid ????????P2P ??????????????????????????ssid */
        oal_memcopy(pc_ssid, pst_mac_device->st_scan_params.ast_mac_ssid_set[0].auc_ssid, WLAN_SSID_MAX_LEN);
    }

    return OAL_SUCC;
}



OAL_STATIC oal_uint32 dmac_scan_send_bcast_probe(mac_device_stru *pst_mac_device, oal_uint8 uc_band, oal_uint8  uc_index)
{
    oal_int8           ac_ssid[WLAN_SSID_MAX_LEN] = {'\0'};
    dmac_vap_stru     *pst_dmac_vap;
    oal_uint32         ul_ret;
    oal_uint8          uc_band_tmp;

    if (0 == pst_mac_device->uc_vap_num)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_send_bcast_probe::uc_vap_num=0.}");
        return OAL_FAIL;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_device->st_scan_params.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_send_bcast_probe::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    
    if(OAL_PTR_NULL == pst_dmac_vap->st_vap_base_info.pst_mib_info)
    {
        OAM_ERROR_LOG4(0, OAM_SF_SCAN, "{dmac_scan_send_bcast_probe:: vap mib info is null,uc_vap_id[%d], p_fn_cb[%p], uc_scan_func[%d], uc_curr_channel_scan_count[%d].}",
                   pst_mac_device->st_scan_params.uc_vap_id,
                   pst_mac_device->st_scan_params.p_fn_cb,
                   pst_mac_device->st_scan_params.uc_scan_func,
                   pst_mac_device->st_scan_params.uc_curr_channel_scan_count);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??????????????????????????vap??band??????????5G??11b */
    uc_band_tmp = pst_dmac_vap->st_vap_base_info.st_channel.en_band;

    pst_dmac_vap->st_vap_base_info.st_channel.en_band = uc_band;

    /* ??????????????????????????????ssid ie???? */
    ul_ret = dmac_scan_get_ssid_ie_info(pst_mac_device, ac_ssid, uc_index);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{dmac_scan_send_bcast_probe::get ssid failed, error[%d].}", ul_ret);
        return ul_ret;
    }

    /* ????probe req?? */
    ul_ret = dmac_scan_send_probe_req_frame(pst_dmac_vap, BROADCAST_MACADDR, ac_ssid);

    pst_dmac_vap->st_vap_base_info.st_channel.en_band = uc_band_tmp;

    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_scan_send_bcast_probe::dmac_mgmt_send_probe_request failed[%d].}", ul_ret);
    }

    return ul_ret;
}


oal_void  dmac_scan_switch_home_channel_work(mac_device_stru *pst_mac_device)
{
    mac_scan_req_stru       *pst_scan_params;

    pst_scan_params = &(pst_mac_device->st_scan_params);

    hal_enable_radar_det(pst_mac_device->pst_device_stru, 0);

    /* ??????????????????????????????????mac addr??????????vap??????mac addr */
    dmac_scan_set_vap_mac_addr_by_scan_state(pst_mac_device, OAL_FALSE);

    /* ???????? ???????????? */
    dmac_scan_switch_channel_back(pst_mac_device);

    /* ???????? */
    if (0 == pst_scan_params->us_work_time_on_home_channel)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_switch_home_channel_work:work_time_on_home_channel is 0, set it to default 100ms!}");
        pst_scan_params->us_work_time_on_home_channel = MAC_WORK_TIME_ON_HOME_CHANNEL_DEFAULT;
    }

    /* ?????????????????????????????????????????????????????????????? */
    FRW_TIMER_CREATE_TIMER(&pst_mac_device->st_scan_timer,
                           dmac_scan_switch_home_channel_work_timeout,
                           pst_scan_params->us_work_time_on_home_channel,
                           pst_mac_device,
                           OAL_FALSE,
                           OAM_MODULE_ID_DMAC,
                           pst_mac_device->ul_core_id);

    return;
}


OAL_STATIC oal_uint32  dmac_scan_switch_home_channel_work_timeout(void *p_arg)
{
    mac_device_stru         *pst_mac_device;
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_to_dmac_device_stru *pst_hal_device = OAL_PTR_NULL;
#endif
    pst_mac_device = (mac_device_stru *)p_arg;
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_switch_home_channel_work_timeout::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "dmac_btcoex_scan_begin:pst_hal_device is null");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
    /* ?????????????????????????????????????????????????????????????????????????????????????????? */
    if (MAC_SCAN_STATE_RUNNING != pst_mac_device->en_curr_scan_state)
    {
        OAM_WARNING_LOG0(pst_mac_device->st_scan_params.uc_vap_id, OAM_SF_SCAN,
                         "{dmac_scan_switch_home_channel_work_timeout::scan has been aborted, no need to continue.}");
        return OAL_SUCC;
    }

    dmac_scan_switch_channel_off(pst_mac_device);

    pst_mac_device->st_scan_params.en_working_in_home_chan = OAL_FALSE;

    /* ????????????????????????????????????????????vap??mac addr */
    dmac_scan_set_vap_mac_addr_by_scan_state(pst_mac_device, OAL_TRUE);

    /* ???????????????? */
    OAL_MEMZERO(&(pst_mac_device->st_chan_result), OAL_SIZEOF(mac_scan_chan_stats_stru));
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if(HAL_BTCOEX_SW_POWSAVE_WORK == GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device))
    {
        GET_HAL_BTCOEX_SW_PREEMPT_TYPE(pst_hal_device) = HAL_BTCOEX_SW_POWSAVE_SCAN_BEGIN;
        OAM_WARNING_LOG0(0, OAM_SF_COEX, "{dmac_btcoex_scan_begin:: normal scan begin delay by btcoex!}");
    }
    else
#endif
    {
        dmac_scan_begin(pst_mac_device);
    }
    return OAL_SUCC;
}


OAL_STATIC oal_void  dmac_scan_get_ch_statics_measurement_result(
                mac_device_stru                 *pst_mac_device)
{
    hal_ch_statics_irq_event_stru    st_stats_result;
    oal_uint8                        uc_chan_idx;

    /* ???????? */
    OAL_MEMZERO(&st_stats_result, OAL_SIZEOF(st_stats_result));
    hal_get_ch_statics_result(pst_mac_device->pst_device_stru, &st_stats_result);
    hal_get_ch_measurement_result(pst_mac_device->pst_device_stru, &st_stats_result);

#if defined(_PRE_WLAN_CHIP_TEST) && defined(_PRE_SUPPORT_ACS) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    dmac_acs_channel_meas_comp_handler(pst_mac_device, &st_stats_result);
#endif

    uc_chan_idx  = pst_mac_device->uc_scan_chan_idx;

    /* ????????????/???????????? */
    pst_mac_device->st_chan_result.uc_stats_valid = 1;
    pst_mac_device->st_chan_result.uc_channel_number = pst_mac_device->st_scan_params.ast_channel_list[uc_chan_idx].uc_chan_number;

    if (st_stats_result.c_pri20_idle_power < 0)
    {
        pst_mac_device->st_chan_result.s_free_power_stats_20M  += (oal_int8)st_stats_result.c_pri20_idle_power; /* ??20M???????????? */
        pst_mac_device->st_chan_result.s_free_power_stats_40M  += (oal_int8)st_stats_result.c_pri40_idle_power; /* ??40M???????????? */
        pst_mac_device->st_chan_result.s_free_power_stats_80M  += (oal_int8)st_stats_result.c_pri80_idle_power; /* ????80M???????????? */
        pst_mac_device->st_chan_result.uc_free_power_cnt += 1;
    }
    //else
    //{
    //    OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{dmac_scan_get_ch_statics_measurement_result::c_pri20_idle_power=%u}", (oal_uint8)st_stats_result.c_pri20_idle_power);
    //}

    pst_mac_device->st_chan_result.ul_total_free_time_20M_us += st_stats_result.ul_pri20_free_time_us;
    pst_mac_device->st_chan_result.ul_total_free_time_40M_us += st_stats_result.ul_pri40_free_time_us;
    pst_mac_device->st_chan_result.ul_total_free_time_80M_us += st_stats_result.ul_pri80_free_time_us;
    pst_mac_device->st_chan_result.ul_total_recv_time_us     += st_stats_result.ul_ch_rx_time_us;
    pst_mac_device->st_chan_result.ul_total_send_time_us     += st_stats_result.ul_ch_tx_time_us;
    pst_mac_device->st_chan_result.ul_total_stats_time_us    += st_stats_result.ul_ch_stats_time_us;

}


oal_uint32 dmac_scan_channel_statistics_complete(frw_event_mem_stru *pst_event_mem)
{
    mac_device_stru                 *pst_mac_device;
    frw_event_stru                  *pst_event;
    oal_uint16                       us_total_scan_time_per_chan;
    oal_uint8                        uc_do_meas;        /* ???????????????????????????????? */
    oal_uint8                        uc_chan_stats_cnt;

    //OAM_INFO_LOG0(0, OAM_SF_SCAN, "{dmac_scan_channel_statistics_complete:: channel statistic complete.}");

    /* ??????????DEVICE??????????????ACS???? */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;
    pst_mac_device = mac_res_get_dev(pst_event->st_event_hdr.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_scan_channel_statistics_complete::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????? ???????? */
    uc_do_meas = pst_mac_device->st_scan_params.uc_scan_func & (MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS);
    if (uc_do_meas)
    {
        dmac_scan_get_ch_statics_measurement_result(pst_mac_device);
    }

    /* ?????????????????????????? */
    pst_mac_device->st_chan_result.uc_stats_cnt++;

    /* ??????????????????10ms???????????????????????????????????????????? */
    uc_chan_stats_cnt = pst_mac_device->st_chan_result.uc_stats_cnt;

    us_total_scan_time_per_chan = (oal_uint16)(pst_mac_device->st_scan_params.us_scan_time * pst_mac_device->st_scan_params.uc_max_scan_count_per_channel);
    if (uc_chan_stats_cnt * DMAC_SCAN_CHANNEL_MEAS_PERIOD_MS < us_total_scan_time_per_chan)
    {
        /* ???????????????? */
        hal_set_ch_statics_period(pst_mac_device->pst_device_stru, DMAC_SCAN_CHANNEL_STATICS_PERIOD_US);
        hal_set_ch_measurement_period(pst_mac_device->pst_device_stru, DMAC_SCAN_CHANNEL_MEAS_PERIOD_MS);
        hal_enable_ch_statics(pst_mac_device->pst_device_stru, 1);
    }

    return OAL_SUCC;
}


oal_void dmac_scan_radar_detected(mac_device_stru *pst_mac_device, hal_radar_det_event_stru *pst_radar_det_info)
{
    pst_mac_device->st_chan_result.uc_radar_detected = 1;
    pst_mac_device->st_chan_result.uc_radar_bw       = 0;
}



OAL_STATIC oal_uint32  dmac_scan_report_channel_statistics_result(mac_device_stru *pst_mac_device, oal_uint8 uc_scan_idx)
{
    frw_event_mem_stru         *pst_event_mem;
    frw_event_stru             *pst_event;
    dmac_crx_chan_result_stru  *pst_chan_result_param;

    /* ????????????????HMAC, ???????????? */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_crx_chan_result_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_scan_report_channel_statistics_result::alloc mem fail.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CRX,
                       DMAC_WLAN_CRX_EVENT_SUB_TYPE_CHAN_RESULT,
                       OAL_SIZEOF(dmac_crx_chan_result_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_device->uc_chip_id,
                       pst_mac_device->uc_device_id,
                       0);

    pst_chan_result_param = (dmac_crx_chan_result_stru *)(pst_event->auc_event_data);

    pst_chan_result_param->uc_scan_idx    = uc_scan_idx;
    pst_chan_result_param->st_chan_result = pst_mac_device->st_chan_result;

    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}



oal_uint32 dmac_scan_init(mac_device_stru *pst_device)
{
    /* ??????device?????????????? */
    pst_device->en_curr_scan_state = MAC_SCAN_STATE_IDLE;

    return OAL_SUCC;
}



oal_uint32 dmac_scan_exit(mac_device_stru *pst_device)
{
    return OAL_SUCC;
}

oal_uint32 dmac_sta_csa_stop_timeout_fn(void *arg)
{
    dmac_vap_stru *dmac_vap = (dmac_vap_stru *)(arg);
    if (dmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_BUTT) {
        return OAL_SUCC;
    }
    OAM_WARNING_LOG0(dmac_vap->st_vap_base_info.uc_vap_id, 0, "{dmac_sta_csa_stop_timeout_fn::timer timeout!}");
    dmac_vap->st_vap_base_info.st_ch_switch_info.uc_switch_fail = OAL_FALSE;
    FRW_TIMER_DESTROY_TIMER(&g_csa_stop_timer);

    return OAL_SUCC;
}

/*lint -e19 */

oal_module_symbol(dmac_scan_switch_channel_off);
oal_module_symbol(dmac_scan_begin);
oal_module_symbol(dmac_scan_abort);
oal_module_symbol(dmac_scan_switch_channel_back);
#ifdef _PRE_WLAN_FEATURE_CCA_OPT
oal_module_symbol(dmac_scan_handle_scan_req_entry);
#endif

/*lint +e19 */
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
