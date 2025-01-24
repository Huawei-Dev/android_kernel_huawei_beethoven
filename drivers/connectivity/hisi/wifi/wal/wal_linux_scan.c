


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_net.h"
#include "oal_cfg80211.h"
#include "wlan_spec.h"
#include "wal_linux_event.h"
#include "wal_linux_scan.h"
#include "wal_linux_cfg80211.h"
#include "wal_main.h"
#include "wal_linux_rx_rsp.h"
#include "hmac_vap.h"
#include "hmac_device.h"
#include "mac_device.h"
#include "hmac_resource.h"

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <net/cfg80211.h>
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)

#endif

#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    /* UT??????????frw_event_process_all_event */
    extern oal_void  frw_event_process_all_event(oal_uint ui_data);
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_SCAN_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/

/*****************************************************************************
  3 ????????
*****************************************************************************/


OAL_STATIC oal_void  wal_inform_bss_frame(wal_scanned_bss_info_stru *pst_scanned_bss_info, oal_void *p_data)
{
    oal_cfg80211_bss_stru        *pst_cfg80211_bss;
    oal_wiphy_stru               *pst_wiphy;
    oal_ieee80211_channel_stru   *pst_ieee80211_channel;

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
    struct timespec ts;
#else
    struct timeval tv;
#endif
#endif

    if ((OAL_PTR_NULL == pst_scanned_bss_info) || (OAL_PTR_NULL == p_data))
    {
        OAM_ERROR_LOG2(0, OAM_SF_SCAN, "{wal_inform_bss_frame::input param pointer is null, pst_scanned_bss_info[%p], p_data[%p]!}",
                       pst_scanned_bss_info, p_data);
        return;
    }

    pst_wiphy = (oal_wiphy_stru *)p_data;

    pst_ieee80211_channel = oal_ieee80211_get_channel(pst_wiphy, (oal_int32)pst_scanned_bss_info->s_freq);

    if(NULL == pst_ieee80211_channel)
    {
        OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{wal_inform_bss_frame::get channel failed, wrong s_freq[%d]}",
                       (oal_int32)pst_scanned_bss_info->s_freq);
        return;
    }

    pst_scanned_bss_info->l_signal = pst_scanned_bss_info->l_signal * 100;

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
/* ??????????????????????????cts????2????????bss??timestamp????????(????????????????) */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
    get_monotonic_boottime(&ts);
    pst_scanned_bss_info->pst_mgmt->u.probe_resp.timestamp = ((u64)ts.tv_sec*1000000)
        + ts.tv_nsec / 1000;
#else
    do_gettimeofday(&tv);
    pst_scanned_bss_info->pst_mgmt->u.probe_resp.timestamp = ((u64)tv.tv_sec*1000000)
        + tv.tv_usec;
#endif
#endif

    /* ????????????bss ???? */
    pst_cfg80211_bss = oal_cfg80211_inform_bss_frame(pst_wiphy, pst_ieee80211_channel, pst_scanned_bss_info->pst_mgmt, pst_scanned_bss_info->ul_mgmt_len, pst_scanned_bss_info->l_signal, GFP_ATOMIC);
    if (pst_cfg80211_bss != NULL)
    {
        oal_cfg80211_put_bss(pst_wiphy, pst_cfg80211_bss);
    }

    return;
}


oal_void wal_update_bss(oal_wiphy_stru      *pst_wiphy,
                        hmac_bss_mgmt_stru  *pst_bss_mgmt,
                        oal_uint8           *puc_bssid)
{
    wal_scanned_bss_info_stru     st_scanned_bss_info;
    oal_cfg80211_bss_stru        *pst_cfg80211_bss;
    hmac_scanned_bss_info        *pst_scanned_bss;
    mac_ieee80211_frame_stru     *pst_frame_hdr;
    oal_dlist_head_stru          *pst_entry;
    mac_bss_dscr_stru            *pst_bss_dscr;
    oal_uint8                     uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0))
    enum nl80211_band             en_band;
#else
    enum ieee80211_band           en_band;
#endif

    if (pst_wiphy == OAL_PTR_NULL || pst_bss_mgmt == OAL_PTR_NULL || puc_bssid == OAL_PTR_NULL)
    {
        OAM_WARNING_LOG3(0, OAM_SF_ASSOC, "{wal_update_bss::null pointer.wiphy %p, bss_mgmt %p, bssid %p.",
                        pst_wiphy, pst_bss_mgmt, puc_bssid);
        return;
    }

    pst_bss_dscr = OAL_PTR_NULL;
    /* ?????? */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ????????????????????bssid ??????bss ???? */
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = OAL_DLIST_GET_ENTRY(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr    = &(pst_scanned_bss->st_bss_dscr_info);

        /* ??????????????????????MAC ?????????????????????????? */
        if (oal_memcmp(puc_bssid, pst_bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) == 0)
        {
            break;
        }
        pst_bss_dscr = OAL_PTR_NULL;
    }

    /* ?????? */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    if (OAL_PTR_NULL == pst_bss_dscr)
    {
        OAM_WARNING_LOG4(0, OAM_SF_ASSOC, "{wal_update_bss::do not find correspond bss from scan result."
                        "%02X:XX:XX:%02X:%02X:%02X}",
                        puc_bssid[0], puc_bssid[3], puc_bssid[4], puc_bssid[5]);
        return;
    }

    /* ????????????????????????????bss  ????????bss ??????????bss ???? */
    pst_cfg80211_bss = oal_cfg80211_get_bss(pst_wiphy,
                        OAL_PTR_NULL,
                        puc_bssid,
                        (oal_uint8 *)(pst_bss_dscr->ac_ssid),
                        OAL_STRLEN(pst_bss_dscr->ac_ssid));

    if (pst_cfg80211_bss != OAL_PTR_NULL)
    {
        oal_cfg80211_put_bss(pst_wiphy, pst_cfg80211_bss);
    }
    else
    {
        uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0))
        en_band        = (enum nl80211_band)pst_bss_dscr->st_channel.en_band;
#else
        en_band        = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;
#endif

        /* ?????? */
        oal_memset(&st_scanned_bss_info, 0, OAL_SIZEOF(wal_scanned_bss_info_stru));

        /* ????BSS ???????? */
        st_scanned_bss_info.l_signal    = pst_bss_dscr->c_rssi;

        /* ??bss?????????????????? */
        st_scanned_bss_info.s_freq      = (oal_int16)oal_ieee80211_channel_to_frequency(uc_chan_number, en_band);

        /* ?????????????????? */
        st_scanned_bss_info.pst_mgmt    = (oal_ieee80211_mgmt_stru *)(pst_bss_dscr->auc_mgmt_buff);
        st_scanned_bss_info.ul_mgmt_len = pst_bss_dscr->ul_mgmt_len;

        /* ???????????????????????????????? */
        pst_frame_hdr  = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

        /* ????????????????????????????beacon????????????????probe rsp??????????
           ??????????????????????????beacon????????????????????????????01?????? */
        if (WLAN_BEACON == pst_frame_hdr->st_frame_control.bit_sub_type)
        {
            /* ????beacon????????probe rsp */
            pst_frame_hdr->st_frame_control.bit_sub_type = WLAN_PROBE_RSP;
        }

        wal_inform_bss_frame(&st_scanned_bss_info, pst_wiphy);
    }

    return;
}



oal_void  wal_inform_all_bss(oal_wiphy_stru  *pst_wiphy, hmac_bss_mgmt_stru  *pst_bss_mgmt, oal_uint8   uc_vap_id)
{
    mac_bss_dscr_stru              *pst_bss_dscr;
    hmac_scanned_bss_info          *pst_scanned_bss;
    oal_dlist_head_stru            *pst_entry;
    mac_ieee80211_frame_stru       *pst_frame_hdr;
    wal_scanned_bss_info_stru       st_scanned_bss_info;
    oal_uint32                      ul_ret;
    oal_uint32                      ul_bss_num_not_in_regdomain = 0;
    oal_uint8                       uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0))
    enum nl80211_band               en_band;
#else
    enum ieee80211_band             en_band;
#endif

    /* ?????? */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* ????????????bss???? */
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = OAL_DLIST_GET_ENTRY(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr    = &(pst_scanned_bss->st_bss_dscr_info);
        if (OAL_PTR_NULL == pst_bss_dscr)
        {
            OAM_ERROR_LOG1(uc_vap_id, OAM_SF_SCAN,
                "{wal_inform_all_bss::scan result in bss list is invalid. bss_dscr:0x%p}",pst_bss_dscr);
            continue;
        }

        uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0))
        en_band        = (enum nl80211_band)pst_bss_dscr->st_channel.en_band;
#else
        en_band        = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;
#endif

        /* ???????????????????????????????????????????????? */
        ul_ret = mac_is_channel_num_valid(en_band, uc_chan_number);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG2(uc_vap_id, OAM_SF_SCAN,
                             "{wal_inform_all_bss::curr channel[%d] and band[%d] is not in regdomain.}",
                             uc_chan_number, en_band);
            ul_bss_num_not_in_regdomain++;
            continue;
        }

        /* ????WAL_SCAN_REPORT_LIMIT?????????????? */
        if (oal_time_after((oal_uint32)OAL_TIME_GET_STAMP_MS(),(pst_bss_dscr->ul_timestamp + WAL_SCAN_REPORT_LIMIT)))
        {
            continue;
        }

        /* ?????? */
        oal_memset(&st_scanned_bss_info, 0, OAL_SIZEOF(wal_scanned_bss_info_stru));

        st_scanned_bss_info.l_signal    = pst_bss_dscr->c_rssi;

        /*??bss??????????????????*/
        st_scanned_bss_info.s_freq      = (oal_int16)oal_ieee80211_channel_to_frequency(uc_chan_number, en_band);

        /* ?????????????????? */
        st_scanned_bss_info.pst_mgmt    = (oal_ieee80211_mgmt_stru *)(pst_bss_dscr->auc_mgmt_buff);
        st_scanned_bss_info.ul_mgmt_len = pst_bss_dscr->ul_mgmt_len;

        /* ???????????????????????????????? */
        pst_frame_hdr  = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

        /* ????????????????????????????beacon????????????????probe rsp??????????
           ??????????????????????????beacon????????????????????????????01?????? */
    	if (WLAN_BEACON == pst_frame_hdr->st_frame_control.bit_sub_type)
    	{
    	    /* ????beacon????????probe rsp */
    		pst_frame_hdr->st_frame_control.bit_sub_type = WLAN_PROBE_RSP;
    	}

        /* ?????????????????? */
        wal_inform_bss_frame(&st_scanned_bss_info, pst_wiphy);
    }

    /* ?????? */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    OAM_WARNING_LOG2(uc_vap_id, OAM_SF_SCAN,
                     "{wal_inform_all_bss::there are %d bss not in regdomain, so inform kernal bss num is [%d]!}",
                     ul_bss_num_not_in_regdomain, (pst_bss_mgmt->ul_bss_num - ul_bss_num_not_in_regdomain));

    return;
}


OAL_STATIC oal_void free_scan_param_resource(mac_cfg80211_scan_param_stru *pst_scan_param)
{
    if (pst_scan_param->pul_channels_2G != OAL_PTR_NULL)
    {
        oal_free(pst_scan_param->pul_channels_2G);
        pst_scan_param->pul_channels_2G = OAL_PTR_NULL;
    }
    if (pst_scan_param->pul_channels_5G != OAL_PTR_NULL)
    {
        oal_free(pst_scan_param->pul_channels_5G);
        pst_scan_param->pul_channels_5G = OAL_PTR_NULL;
    }
}


OAL_STATIC oal_uint32  wal_set_scan_channel(
                    oal_cfg80211_scan_request_stru    *pst_request,
                    mac_cfg80211_scan_param_stru      *pst_scan_param)

{
    oal_uint32  ul_loop;
    oal_uint32  ul_num_chan_2G;
    oal_uint32  ul_num_chan_5G;

    if (OAL_PTR_NULL == pst_request || OAL_PTR_NULL == pst_scan_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_get_scan_channel_num::set scan channel from scan request failed, pst_request[%p] null ptr or pst_scan_param[%p] null ptr.}",
            pst_request, pst_scan_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (0 == pst_request->n_channels)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_get_scan_channel_num::channels in scan requst is zero.}");
        return OAL_FAIL;
    }

    pst_scan_param->pul_channels_2G = oal_memalloc(pst_request->n_channels * OAL_SIZEOF(oal_uint32));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_scan_param->pul_channels_2G))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{wal_scan_work_func::2.4G channel alloc mem return null ptr!}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_scan_param->pul_channels_5G = oal_memalloc(pst_request->n_channels * OAL_SIZEOF(oal_uint32));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_scan_param->pul_channels_5G))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{wal_scan_work_func::5G channel alloc mem return null ptr!}");
        free_scan_param_resource(pst_scan_param);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    ul_num_chan_2G = 0;
    ul_num_chan_5G = 0;

    for (ul_loop = 0; ul_loop < pst_request->n_channels; ul_loop++)
    {
        oal_uint16  us_center_freq;
        oal_uint32  ul_chan;

        us_center_freq = pst_request->channels[ul_loop]->center_freq;

        /* ???????????????????????? */
        ul_chan = (oal_uint32)oal_ieee80211_frequency_to_channel((oal_int32)us_center_freq);

        if (us_center_freq <= WAL_MAX_FREQ_2G)
        {
            pst_scan_param->pul_channels_2G[ul_num_chan_2G++] = ul_chan;
        }
        else
        {
            pst_scan_param->pul_channels_5G[ul_num_chan_5G++] = ul_chan;
        }
    }

    pst_scan_param->uc_num_channels_2G = (oal_uint8)ul_num_chan_2G;
    pst_scan_param->uc_num_channels_5G = (oal_uint8)ul_num_chan_5G;

    if (0 == ul_num_chan_2G)
    {
        oal_free(pst_scan_param->pul_channels_2G);
        pst_scan_param->pul_channels_2G = OAL_PTR_NULL;
    }
    if (0 == ul_num_chan_5G)
    {
        oal_free(pst_scan_param->pul_channels_5G);
        pst_scan_param->pul_channels_5G = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void wal_set_scan_ssid(oal_cfg80211_scan_request_stru *pst_request, mac_cfg80211_scan_param_stru *pst_scan_param)
{
    oal_int32   l_loop;
    oal_int32   l_ssid_num;

    if ((OAL_PTR_NULL == pst_request) || (OAL_PTR_NULL == pst_scan_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_set_scan_ssid::scan failed, null ptr, pst_request[%p], pst_scan_param[%p].}",
          pst_request, pst_scan_param);

        return;
    }

    pst_scan_param->en_scan_type = OAL_ACTIVE_SCAN; /* active scan */
    pst_scan_param->l_ssid_num   = 0;

    /* ????????????ssid?????? */
    l_ssid_num = pst_request->n_ssids;
    if (l_ssid_num > WLAN_SCAN_REQ_MAX_SSID)
    {
        /* ??????????????????ssid????????????????????????????????????????????????ssid?????????? */
        l_ssid_num = WLAN_SCAN_REQ_MAX_SSID;
    }

    /* ????????????ssid???????????????????????? */
    if ((l_ssid_num > 0) && (OAL_PTR_NULL != pst_request->ssids))
    {
        pst_scan_param->l_ssid_num = l_ssid_num;

        for (l_loop = 0; l_loop < l_ssid_num; l_loop++)
        {
            pst_scan_param->st_ssids[l_loop].uc_ssid_len = pst_request->ssids[l_loop].ssid_len;
            if (pst_scan_param->st_ssids[l_loop].uc_ssid_len > OAL_IEEE80211_MAX_SSID_LEN)
            {
                OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_set_scan_ssid::ssid scan set failed, ssid_len[%d] is exceed, max[%d].}",
                  pst_scan_param->st_ssids[l_loop].uc_ssid_len, OAL_IEEE80211_MAX_SSID_LEN);

                pst_scan_param->st_ssids[l_loop].uc_ssid_len = OAL_IEEE80211_MAX_SSID_LEN;
            }

            oal_memcopy(pst_scan_param->st_ssids[l_loop].auc_ssid, pst_request->ssids[l_loop].ssid,
              pst_scan_param->st_ssids[l_loop].uc_ssid_len);
        }
	}
}


OAL_STATIC oal_uint32  wal_wait_for_scan_timeout_fn(void *p_arg)
{
    hmac_vap_stru                  *pst_hmac_vap = (hmac_vap_stru *)p_arg;
    mac_vap_stru                   *pst_mac_vap  = &(pst_hmac_vap->st_vap_base_info);
    hmac_device_stru               *pst_hmac_device;
    hmac_bss_mgmt_stru             *pst_bss_mgmt;
    hmac_scan_stru                 *pst_scan_mgmt;
    oal_wiphy_stru                 *pst_wiphy;

    OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{wal_wait_for_scan_timeout_fn:: 5 seconds scan timeout proc.}");

    /* ????????????????????????vap????????????????vap?????????????????????????????????????????????????? */
    if ((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode) &&
        (MAC_VAP_STATE_STA_WAIT_SCAN == pst_mac_vap->en_vap_state))
    {
        /* ????vap??????SCAN_COMP */
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
    }

    /* ????hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_hmac_device)
    {
        OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{wal_wait_for_scan_complete_time_out::pst_hmac_device[%d] is null.}",
                        pst_mac_vap->uc_device_id);
        return OAL_FAIL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);
    pst_wiphy      = pst_hmac_device->pst_device_base_info->pst_wiphy;

    /* ?????????????????????????? */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    /* ??????????????????request???????? */
    oal_spin_lock(&(pst_scan_mgmt->st_scan_request_spinlock));

    if (OAL_PTR_NULL != pst_scan_mgmt->pst_request)
    {
        /* ??????????????????bss */
        wal_inform_all_bss(pst_wiphy, pst_bss_mgmt, pst_mac_vap->uc_vap_id);

        /* ???? kernel scan ???????? */
        oal_cfg80211_scan_done(pst_scan_mgmt->pst_request, 0);

        pst_scan_mgmt->pst_request = OAL_PTR_NULL;
        pst_scan_mgmt->en_complete = OAL_TRUE;

        /* ??????????????????OAL_WAIT_QUEUE_WAKE_UP?????????? */
        OAL_SMP_MB();
        OAL_WAIT_QUEUE_WAKE_UP_INTERRUPT(&pst_scan_mgmt->st_wait_queue);
    }

    /* ?????????????????????????? */
    oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));

    return OAL_SUCC;
}


OAL_STATIC oal_void  wal_start_timer_for_scan_timeout(oal_uint8   uc_vap_id)
{
    hmac_vap_stru               *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru             *pst_mac_device = OAL_PTR_NULL;
    oal_uint32                   ul_timeout;

    /* ??????????5?? */
    ul_timeout = WAL_MAX_SCAN_TIME_PER_SCAN_REQ;

    /* ????hmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_hmac_vap is null!}");
        return;
    }

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_mac_device is null!}");
        return;
    }

    /* ???????????????????????????????????????????????????????????????? */
    FRW_TIMER_CREATE_TIMER(&(pst_hmac_vap->st_scan_timeout),
                           wal_wait_for_scan_timeout_fn,
                           ul_timeout,
                           pst_hmac_vap,
                           OAL_FALSE,
                           OAM_MODULE_ID_WAL,
                           pst_mac_device->ul_core_id);

    return;
}


oal_uint32 wal_scan_work_func(hmac_scan_stru                     *pst_scan_mgmt,
                                  oal_net_device_stru                   *pst_netdev,
                                  oal_cfg80211_scan_request_stru        *pst_request)
{
    mac_cfg80211_scan_param_stru st_scan_param;
    oal_uint32                   ul_ret;
    mac_vap_stru                *pst_mac_vap = OAL_NET_DEV_PRIV(pst_netdev);
    oal_uint8                    uc_vap_id;

    if(NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{wal_scan_work_func::pst_mac_vap is null!}");
        return OAL_FAIL;
    }

    uc_vap_id = pst_mac_vap->uc_vap_id;

    oal_memset(&st_scan_param, 0, sizeof(mac_cfg80211_scan_param_stru));

    /* ?????????????????????????? */
    ul_ret = wal_set_scan_channel(pst_request, &st_scan_param);
    if (ul_ret != OAL_SUCC)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_scan_work_func::wal_set_scan_channel proc failed, err_code[%d]!}", ul_ret);
        return OAL_FAIL;
    }

    /* ??????????????ssid */
    wal_set_scan_ssid(pst_request, &st_scan_param);

    /* ??????????????ie */
    st_scan_param.puc_ie    = pst_request->ie;
    st_scan_param.ul_ie_len = pst_request->ie_len;

    /* P2P WLAN/P2P ??????????????????????ssid ??????????p2p device ????????????
        ssid ??"DIRECT-"????????p2p device ?????????? */
    /* ??????????????device ??????p2p device(p2p0) */
    st_scan_param.bit_is_p2p0_scan      = OAL_FALSE;


    if (IS_P2P_SCAN_REQ(pst_request))
    {
        st_scan_param.bit_is_p2p0_scan = OAL_TRUE;
    }

    /* ??????????????????????????????,???????????? */
    pst_scan_mgmt->en_complete = OAL_FALSE;

    /* ???????????????????????? */
    ul_ret = wal_cfg80211_start_scan(pst_netdev, &st_scan_param);
    if( OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_scan_work_func::wal_cfg80211_start_scan proc failed, err_code[%d]!}", ul_ret);
        /* ??????????????hmac???? */
        free_scan_param_resource(&st_scan_param);

        pst_scan_mgmt->en_complete = OAL_TRUE;
        return OAL_FAIL;
    }

    /* win32 UT?????????????????????? */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    frw_event_process_all_event(0);
#endif

    /* ???????????????????????????? */
    wal_start_timer_for_scan_timeout(uc_vap_id);

    return OAL_SUCC;
}


oal_int32 wal_send_scan_abort_msg(oal_net_device_stru   *pst_net_dev)
{
    wal_msg_write_stru              st_write_msg;
    oal_uint32                      ul_pedding_data = 0;       /* ?????????????????????????????????? */
    oal_int32                       l_ret = 0;
    wal_msg_stru                    *pst_rsp_msg = OAL_PTR_NULL;

    /* ??????????device?????????? */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SCAN_ABORT, OAL_SIZEOF(ul_pedding_data));

    oal_memcopy(st_write_msg.auc_value, (oal_int8 *)&ul_pedding_data, OAL_SIZEOF(ul_pedding_data));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_pedding_data),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg))
    {
        OAM_WARNING_LOG0(0,OAM_SF_SCAN,"{wal_send_scan_abort_msg::wal_check_and_release_msg_resp fail.}");
    }

    if (OAL_SUCC != l_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{wal_send_scan_abort_msg::fail to stop scan, error[%d]}", l_ret);
    }



    return l_ret;
}


oal_int32 wal_force_scan_complete(oal_net_device_stru   *pst_net_dev,
                                         oal_bool_enum          en_is_aborted)
{
    mac_vap_stru            *pst_mac_vap;
    hmac_vap_stru           *pst_hmac_vap;
    hmac_device_stru        *pst_hmac_device;
    hmac_scan_stru          *pst_scan_mgmt;

    pst_mac_vap  = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{wal_force_scan_complete::Cannot find mac_vap by net_dev!}");
        return -OAL_EINVAL;
    }

    /* ????hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_hmac_device)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_complete::pst_hmac_device[%d] is null!}",
                         pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    /* ???????????????????? */
    if (OAL_PTR_NULL == pst_scan_mgmt->pst_request)
    {
        /* ?????????????????????????????????????????? */
        if ((OAL_TRUE == pst_hmac_device->st_scan_mgmt.en_is_scanning) &&
            (pst_mac_vap->uc_vap_id == pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_vap_id))
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "{wal_force_scan_complete::may be internal scan, stop scan!}");
            /* ???????? */
            wal_send_scan_abort_msg(pst_net_dev);
        }

        return OAL_SUCC;
    }

    /* ????hmac vap */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_complete::hmac_vap is null, vap_id[%d]!}", pst_mac_vap->uc_vap_id);
        return -OAL_EINVAL;
    }

    /* ??????????????????request???????? */
    oal_spin_lock(&(pst_scan_mgmt->st_scan_request_spinlock));

    /* ?????????????????????????????????????????????????????????????? */
    if ((OAL_PTR_NULL != pst_scan_mgmt->pst_request)
        && (pst_net_dev->ieee80211_ptr == pst_scan_mgmt->pst_request->wdev))
    {
        /* ?????????????????????? */
        if (OAL_TRUE == pst_hmac_vap->st_scan_timeout.en_is_registerd)
        {
            FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&(pst_hmac_vap->st_scan_timeout));
        }

        /* ???????????????? */
        wal_inform_all_bss(pst_hmac_device->pst_device_base_info->pst_wiphy,
                           &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt),
                           pst_mac_vap->uc_vap_id);

        /* ???????????????? */
        oal_cfg80211_scan_done(pst_scan_mgmt->pst_request, en_is_aborted);

        pst_scan_mgmt->pst_request = OAL_PTR_NULL;
        pst_scan_mgmt->en_complete = OAL_TRUE;
        /* ?????????????????????????? */
        oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));
        /* ????device???????? */
        wal_send_scan_abort_msg(pst_net_dev);

        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_force_scan_complete::vap_id[%d] notify kernel scan abort!}",
                         pst_mac_vap->uc_vap_id);

        /* ??????????????????OAL_WAIT_QUEUE_WAKE_UP?????????? */
        OAL_SMP_MB();
        OAL_WAIT_QUEUE_WAKE_UP_INTERRUPT(&pst_scan_mgmt->st_wait_queue);
    }
    else
    {
        /* ?????????????????????????? */
        oal_spin_unlock(&(pst_scan_mgmt->st_scan_request_spinlock));
    }

    return OAL_SUCC;
}


oal_int32 wal_stop_sched_scan(oal_net_device_stru *pst_netdev)
{
    hmac_device_stru               *pst_hmac_device;
    hmac_scan_stru                 *pst_scan_mgmt;
    mac_vap_stru                   *pst_mac_vap;
    wal_msg_write_stru              st_write_msg;
    oal_uint32                      ul_pedding_data = 0;       /* ?????????????????????????????????? */
    oal_int32                       l_ret = 0;

    /* ?????????????? */
    if (OAL_PTR_NULL == pst_netdev)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{wal_stop_sched_scan::pst_netdev is null}");
        return -OAL_EINVAL;
    }

    /* ????net_device ??????????mac_device_stru ???? */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_netdev);
    if(OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{wal_stop_sched_scan:: pst_mac_vap is null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_device = (hmac_device_stru *)hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_hmac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_SCAN, "{wal_stop_sched_scan:: pst_mac_device[%d] is null!}",
                    pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    pst_scan_mgmt = &(pst_hmac_device->st_scan_mgmt);

    OAM_WARNING_LOG2(0, OAM_SF_SCAN, "{wal_stop_sched_scan::sched scan req[0x%x],sched scan complete[%d]}",pst_scan_mgmt->pst_sched_scan_req,pst_scan_mgmt->en_sched_scan_complete);

    if ((OAL_PTR_NULL != pst_scan_mgmt->pst_sched_scan_req) &&
        (OAL_TRUE != pst_scan_mgmt->en_sched_scan_complete))
    {
        /* ?????????????????????????????????????????? */
        if (OAL_PTR_NULL == pst_scan_mgmt->pst_request)
        {
            oal_cfg80211_sched_scan_result(pst_hmac_device->pst_device_base_info->pst_wiphy);
        }

        pst_scan_mgmt->pst_sched_scan_req     = OAL_PTR_NULL;
        pst_scan_mgmt->en_sched_scan_complete = OAL_TRUE;

        /* ??????????device??????PNO???????? */
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_STOP_SCHED_SCAN, OAL_SIZEOF(ul_pedding_data));

        oal_memcopy(st_write_msg.auc_value, (oal_int8 *)&ul_pedding_data, OAL_SIZEOF(ul_pedding_data));

        l_ret = wal_send_cfg_event(pst_netdev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_pedding_data),
                                   (oal_uint8 *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
        if (OAL_SUCC != l_ret)
        {
            OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{wal_stop_sched_scan::fail to stop pno sched scan, error[%d]}", l_ret);
        }
    }

    return OAL_SUCC;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

