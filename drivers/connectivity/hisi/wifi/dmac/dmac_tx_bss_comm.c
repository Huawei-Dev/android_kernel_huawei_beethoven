


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_util.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "oal_sdio.h"

#include "hal_ext_if.h"
/*TBD??????????????*/
#include "hal_witp_mac.h"


#include "mac_frame.h"
#include "mac_data.h"

#include "dmac_ext_if.h"
#include "dmac_tx_bss_comm.h"
#include "dmac_blockack.h"
#include "dmac_tx_complete.h"
#include "dmac_psm_ap.h"
#include "dmac_uapsd.h"
#include "dmac_mgmt_bss_comm.h"
#include "dmac_11w.h"
#include "dmac_11i.h"
#include "dmac_dft.h"
#include "dmac_alg.h"

#ifdef _PRE_WLAN_FEATURE_DBAC
#include "mac_device.h"
#include "dmac_device.h"
#endif

#ifdef _PRE_WLAN_CHIP_TEST
#include "dmac_test_main.h"
#include "dmac_lpm_test.h"
#include "dmac_test_sch.h"
#include "dmac_config.h"
#endif

#ifdef _PRE_WIFI_DMT
#include "hal_witp_dmt_if.h"
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "dmac_psm_sta.h"
#include "pm_extern.h"
#endif

#ifdef _PRE_WLAN_FEATURE_BTCOEX
#include "dmac_btcoex.h"
#endif

#include "oal_profiling.h"
#include "dmac_config.h"

#include "dmac_auto_adjust_freq.h"
#ifdef _PRE_WLAN_SW_CTRL_RSP
#include "dmac_vap.h"
#include "dmac_resource.h"
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_TX_BSS_COMM_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/
//OAL_STATIC oal_uint32  dmac_tx_process_data_excp(hal_to_dmac_device_stru *pst_hal_device, dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf);
OAL_STATIC oal_uint32  dmac_tx_update_protection_txop_alg(dmac_vap_stru *pst_dmac_vap, hal_tx_txop_alg_stru *pst_txop_alg, oal_uint8 uc_do_default_cfg, oal_bool_enum_uint8 en_mcast);
extern oal_bool_enum_uint8 mac_is_p2p_action_frame(oal_uint8 *puc_data);

/*****************************************************************************
  3 ????????????
*****************************************************************************/
typedef oal_uint32 (*p_dmac_tx_process_data_func)(hal_to_dmac_device_stru *pst_hal_device, dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf);

#ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
/* ??????MAC??????PHY?????????????? */
oal_uint8 g_auc_default_mac_to_phy_protocol_mapping[WLAN_PROTOCOL_BUTT] =
{
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,     /* WLAN_LEGACY_11A_MODE            = 0 */
    WLAN_11B_PHY_PROTOCOL_MODE,             /* WLAN_LEGACY_11B_MODE            = 1 */
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,     /* WLAN_LEGACY_11G_MODE            = 2 */
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,     /* WLAN_MIXED_ONE_11G_MODE         = 3 */
    WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE,     /* WLAN_MIXED_TWO_11G_MODE         = 4 */
    WLAN_HT_PHY_PROTOCOL_MODE,              /* WLAN_HT_MODE                    = 5 */
    WLAN_VHT_PHY_PROTOCOL_MODE,             /* WLAN_VHT_MODE                   = 6 */
    WLAN_HT_PHY_PROTOCOL_MODE,              /* WLAN_HT_ONLY_MODE               = 7 */
    WLAN_VHT_PHY_PROTOCOL_MODE,             /* WLAN_VHT_ONLY_MODE              = 8 */
    WLAN_HT_PHY_PROTOCOL_MODE,              /* WLAN_HT_11G_MODE                = 9 */
};
#endif

/*****************************************************************************
  4 ????????
*****************************************************************************/


oal_uint32  dmac_tx_dump_get_switch(
                                    oam_user_track_frame_type_enum_uint8     en_frame_type,
                                    oal_uint8                               *pen_frame_switch,
                                    oal_uint8                               *pen_cb_switch,
                                    oal_uint8                               *pen_dscr_switch,
                                    mac_tx_ctl_stru                         *pst_tx_cb)
{
    oal_uint32                   ul_ret;
    mac_ieee80211_frame_stru    *pst_frame_hdr;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pen_frame_switch)
        || OAL_UNLIKELY(OAL_PTR_NULL == pen_cb_switch)
        || OAL_UNLIKELY(OAL_PTR_NULL == pen_dscr_switch)
        || OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_cb))
    {
        OAM_ERROR_LOG4(0, OAM_SF_TX,
                        "{dmac_tx_dump_get_switch::param null, pen_frame_switch=%d pen_cb_switch=%d pen_dscr_switch=%d pst_tx_cb=%d.}",
                        pen_frame_switch, pen_cb_switch, pen_dscr_switch, pst_tx_cb);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??????????probe request????probe response???????????????? */
    pst_frame_hdr = mac_get_cb_frame_hdr(pst_tx_cb);
    if (OAL_PTR_NULL == pst_frame_hdr)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_dump_get_switch::pst_frame_hdr null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (WLAN_MANAGEMENT == pst_frame_hdr->st_frame_control.bit_type)
    {
        if(WLAN_PROBE_REQ == pst_frame_hdr->st_frame_control.bit_sub_type
        || WLAN_PROBE_RSP == pst_frame_hdr->st_frame_control.bit_sub_type)
        {
            ul_ret = oam_report_80211_probe_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX,
                                                       pen_frame_switch,
                                                       pen_cb_switch,
                                                       pen_dscr_switch);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump_get_switch::oam_report_80211_probe_get_switch failed[%d].}", ul_ret);
                return ul_ret;
            }

            return OAL_SUCC;
        }
        else if(WLAN_ACTION == pst_frame_hdr->st_frame_control.bit_sub_type
                || WLAN_ACTION_NO_ACK == pst_frame_hdr->st_frame_control.bit_sub_type)
        {
                *pen_cb_switch    = 1;
                *pen_dscr_switch  = 1;
                *pen_frame_switch = 1;
                return OAL_SUCC;
        }
    }
    /* ??????probe request??probe response?????? */
    if (OAL_TRUE == MAC_GET_CB_IS_MCAST(pst_tx_cb))
    {
        ul_ret = oam_report_80211_mcast_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX,
                                                   en_frame_type,
                                                   pen_frame_switch,
                                                   pen_cb_switch,
                                                   pen_dscr_switch);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG2(0, OAM_SF_TX,
                             "{dmac_tx_dump_get_switch::dmac_tx_dump_get_switch failed! ul_ret=[%d],frame_type=[%d]}",
                             ul_ret, en_frame_type);
            return ul_ret;
        }
    }
    else
    {
        /* ??????0xffff???????????????? */
        if (MAX_TX_USER_IDX == MAC_GET_CB_TX_USER_IDX(pst_tx_cb))
        {
            if (WLAN_MANAGEMENT == pst_frame_hdr->st_frame_control.bit_type
                && (WLAN_DISASOC == pst_frame_hdr->st_frame_control.bit_sub_type
                || WLAN_DEAUTH == pst_frame_hdr->st_frame_control.bit_sub_type))
            {
                *pen_cb_switch    = 1;
                *pen_dscr_switch  = 1;
                *pen_frame_switch = 1;
            }
            else
            {
                *pen_cb_switch    = 0;
                *pen_dscr_switch  = 0;
                *pen_frame_switch = 0;
            }

            return OAL_FAIL;
        }

        /* ??????probe response ?????? */
        ul_ret = oam_report_80211_ucast_get_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX,
                                                   en_frame_type,
                                                   pen_frame_switch,
                                                   pen_cb_switch,
                                                   pen_dscr_switch,
                                                   MAC_GET_CB_TX_USER_IDX(pst_tx_cb));
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG3(0, OAM_SF_TX,
                             "{dmac_tx_dump_get_switch::oam_report_80211_ucast_get_switch failed! ul_ret=[%d],frame_type=[%d], user_idx=[%d]}",
                             ul_ret, en_frame_type, MAC_GET_CB_TX_USER_IDX(pst_tx_cb));
            OAM_WARNING_LOG3(0, OAM_SF_TX, "{dmac_tx_dump_get_switch::frame_switch=[%d], cb_switch=[%d], dscr_switch=[%d]",
                             pen_frame_switch, pen_cb_switch, pen_dscr_switch);
            return ul_ret;
        }
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_tx_get_user_macaddr(mac_tx_ctl_stru *pst_tx_cb,
                                                        oal_uint8 auc_user_macaddr[])
{
    mac_user_stru      *pst_mac_user;

    if (OAL_TRUE == MAC_GET_CB_IS_MCAST(pst_tx_cb))
    {
        oal_set_mac_addr(auc_user_macaddr, BROADCAST_MACADDR);
    }
    else
    {
        pst_mac_user = mac_res_get_mac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_cb));
        if (OAL_PTR_NULL == pst_mac_user)
        {
            OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_get_user_macaddr::pst_mac_user[%d] null.", MAC_GET_CB_TX_USER_IDX(pst_tx_cb));

            return OAL_ERR_CODE_PTR_NULL;
        }
        oal_set_mac_addr(auc_user_macaddr, pst_mac_user->auc_user_mac_addr);
    }

    return OAL_SUCC;
}


oal_uint32  dmac_tx_dump_get_user_macaddr(mac_tx_ctl_stru *pst_tx_cb,
                                                     oal_uint8 auc_user_macaddr[])
{
    mac_ieee80211_frame_stru    *pst_frame_hdr;
    oal_uint8                    uc_tx_user_idx;

    pst_frame_hdr = mac_get_cb_frame_hdr(pst_tx_cb);
    if (OAL_PTR_NULL == pst_frame_hdr)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_dump_get_user_macaddr::pst_frame_hdr null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??????????probe request????probe response??GO Neg, GAS ???????????????????????? */
    uc_tx_user_idx  =  (oal_uint8)MAC_GET_CB_TX_USER_IDX(pst_tx_cb);
    if (WLAN_MANAGEMENT == pst_frame_hdr->st_frame_control.bit_type
        && ((WLAN_PROBE_REQ == pst_frame_hdr->st_frame_control.bit_sub_type)
        || (WLAN_PROBE_RSP == pst_frame_hdr->st_frame_control.bit_sub_type)
        || ((WLAN_ACTION == pst_frame_hdr->st_frame_control.bit_sub_type) && (uc_tx_user_idx == (oal_uint8)MAX_TX_USER_IDX))))
    {
        oal_set_mac_addr(auc_user_macaddr, BROADCAST_MACADDR);
        return OAL_SUCC;
    }

    return dmac_tx_get_user_macaddr(pst_tx_cb, auc_user_macaddr);
}



OAL_STATIC oal_void  dmac_tx_dump(dmac_tx_dump_param_stru *pst_tx_dump_param)
{
    oal_uint32               ul_dscr_one_size = 0;
    oal_uint32               ul_dscr_two_size = 0;
    oal_uint32               ul_ret;

    /* ?????????? */
    if (OAL_SWITCH_ON == pst_tx_dump_param->en_frame_switch)
    {
       ul_ret =  oam_report_80211_frame(pst_tx_dump_param->auc_user_macaddr,
                               pst_tx_dump_param->puc_mac_hdr_addr,
                               pst_tx_dump_param->uc_mac_hdr_len,
                               pst_tx_dump_param->puc_mac_payload_addr,
                               pst_tx_dump_param->us_mac_frame_len,
                               OAM_OTA_FRAME_DIRECTION_TYPE_TX);
       if (OAL_SUCC != ul_ret)
       {
           OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump::oam_report_80211_frame return err: 0x%x.}\r\n", ul_ret);
       }
    }

    /* ?????????????????????? */
    if (OAL_SWITCH_ON == pst_tx_dump_param->en_dscr_switch)
    {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        hal_tx_get_size_dscr(pst_tx_dump_param->pst_hal_device,
                             pst_tx_dump_param->pst_tx_cb->bit_netbuf_num,
                             &ul_dscr_one_size,
                             &ul_dscr_two_size);
#else
        hal_tx_get_size_dscr(pst_tx_dump_param->pst_hal_device,
                             pst_tx_dump_param->pst_tx_cb->uc_netbuf_num,
                             &ul_dscr_one_size,
                             &ul_dscr_two_size);
#endif
        ul_ret = oam_report_dscr(pst_tx_dump_param->auc_user_macaddr,
                        (oal_uint8 *)pst_tx_dump_param->pst_tx_dscr,
                        (oal_uint16)(ul_dscr_one_size + ul_dscr_two_size),
                        OAM_OTA_TYPE_TX_DSCR);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump::oam_report_dscr return err: 0x%x.}\r\n", ul_ret);
        }
    }

    /* ????????????CB */
    if (OAL_SWITCH_ON == pst_tx_dump_param->en_cb_switch)
    {
        ul_ret = oam_report_netbuf_cb(pst_tx_dump_param->auc_user_macaddr,
                             OAL_NETBUF_CB(pst_tx_dump_param->pst_netbuf),
                             OAM_OTA_TYPE_TX_CB);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump::oam_report_netbuf_cb return err: 0x%x.}\r\n", ul_ret);
        }
    }
}


OAL_STATIC oal_void dmac_tx_dump_data(
                            hal_to_dmac_device_stru     *pst_hal_device,
                            mac_tx_ctl_stru             *pst_tx_ctl,
                            oal_dlist_head_stru         *pst_tx_dscr_list_hdr)
{
    oal_netbuf_stru         *pst_netbuf = OAL_PTR_NULL;
    hal_tx_dscr_stru        *pst_dscr   = OAL_PTR_NULL;
    oal_dlist_head_stru     *pst_dlist_node;
    oal_switch_enum_uint8    en_frame_switch = 0;
    oal_switch_enum_uint8    en_cb_switch = 0;
    oal_switch_enum_uint8    en_dscr_switch = 0;
    oal_uint32               ul_ret = 0;
    oal_uint8                auc_user_macaddr[WLAN_MAC_ADDR_LEN] = {0};
    dmac_tx_dump_param_stru  st_tx_dump_param;

    pst_dlist_node = pst_tx_dscr_list_hdr->pst_next;
    while (pst_dlist_node != pst_tx_dscr_list_hdr)
    {
        pst_dscr = OAL_DLIST_GET_ENTRY(pst_dlist_node, hal_tx_dscr_stru, st_entry);
        pst_netbuf = pst_dscr->pst_skb_start_addr;

        if(OAL_PTR_NULL != pst_netbuf)
        {
            pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

            /* ???????????? */
            ul_ret = dmac_tx_dump_get_switch(OAM_USER_TRACK_FRAME_TYPE_DATA,
                                             &en_frame_switch,
                                             &en_cb_switch,
                                             &en_dscr_switch,
                                             pst_tx_ctl);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump_data::dmac_tx_dump_get_switch failed[%d].}", ul_ret);
                pst_dlist_node = pst_dlist_node->pst_next;
                continue;
            }

            /* ????????mac??????????SDT???? */
            ul_ret = dmac_tx_dump_get_user_macaddr(pst_tx_ctl, auc_user_macaddr);
            if (OAL_SUCC != ul_ret)
            {
                OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_dump_get_switch::dmac_tx_dump_get_switch failed[%d].}", ul_ret);

                pst_dlist_node = pst_dlist_node->pst_next;
                continue;
            }

            /* ???????????? */
            st_tx_dump_param.pst_hal_device       = pst_hal_device;
            st_tx_dump_param.pst_netbuf           = pst_netbuf;
            st_tx_dump_param.pst_tx_cb            = pst_tx_ctl;
            st_tx_dump_param.pst_tx_dscr          = pst_dscr;

            st_tx_dump_param.puc_mac_hdr_addr     = (oal_uint8 *)(mac_get_cb_frame_hdr(pst_tx_ctl));
            st_tx_dump_param.puc_mac_payload_addr = oal_netbuf_payload(pst_netbuf);
            st_tx_dump_param.uc_mac_hdr_len       = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl);
            st_tx_dump_param.us_mac_frame_len     = (oal_uint16)MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl)
                                                  + (oal_uint16)oal_netbuf_get_len(pst_netbuf);

            st_tx_dump_param.en_frame_switch      = en_frame_switch;
            st_tx_dump_param.en_cb_switch         = en_cb_switch;
            st_tx_dump_param.en_dscr_switch       = en_dscr_switch;
            oal_set_mac_addr(st_tx_dump_param.auc_user_macaddr, auc_user_macaddr);

            dmac_tx_dump(&st_tx_dump_param);
        }
        else
        {
            OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_dump_data::pst_netbuf null.}");
        }

        pst_dlist_node = pst_dlist_node->pst_next;
    }
}


OAL_STATIC oal_void  dmac_tx_dump_mgmt(
                                        hal_to_dmac_device_stru *pst_hal_device,
                                        oal_netbuf_stru *pst_netbuf_mgmt,
                                        oal_uint16 us_frame_len,
                                        oal_uint8  uc_mac_hdr_len,
                                        hal_tx_dscr_stru *pst_mgmt_dscr)
{
    mac_tx_ctl_stru       *pst_tx_cb;
    oal_switch_enum_uint8  en_frame_switch = 0;
    oal_switch_enum_uint8  en_cb_switch = 0;
    oal_switch_enum_uint8  en_dscr_switch = 0;
    oal_uint32             ul_ret;
    oal_uint8              auc_user_macaddr[WLAN_MAC_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    dmac_tx_dump_param_stru  st_tx_dump_param;

    pst_tx_cb = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_netbuf_mgmt);

    /* ???????????????????????????????????????? */
    ul_ret = dmac_tx_dump_get_switch(OAM_USER_TRACK_FRAME_TYPE_MGMT,
                                     &en_frame_switch,
                                     &en_cb_switch,
                                     &en_dscr_switch,
                                     pst_tx_cb);
    if (OAL_ERR_CODE_PTR_NULL == ul_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_dump_mgmt::dmac_tx_dump_get_switch failed[%d].}", ul_ret);
        return;
    }

    if (OAL_SUCC == ul_ret)
    {
        /* ????????mac??????????SDT???? */
        ul_ret = dmac_tx_dump_get_user_macaddr(pst_tx_cb, auc_user_macaddr);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_dump_mgmt::dmac_tx_get_user_macaddr failed[%d].", ul_ret);

            return;
        }
    }
    /* ???????????? */
    st_tx_dump_param.en_cb_switch = en_cb_switch;
    st_tx_dump_param.en_dscr_switch = en_dscr_switch;
    st_tx_dump_param.en_frame_switch = en_frame_switch;
    st_tx_dump_param.pst_hal_device = pst_hal_device;
    st_tx_dump_param.pst_netbuf = pst_netbuf_mgmt;
    st_tx_dump_param.pst_tx_cb = pst_tx_cb;
    st_tx_dump_param.pst_tx_dscr = pst_mgmt_dscr;

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    st_tx_dump_param.puc_mac_hdr_addr =  (oal_uint8 *)(mac_get_cb_frame_hdr(pst_tx_cb));
    st_tx_dump_param.uc_mac_hdr_len = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_cb);
    st_tx_dump_param.puc_mac_payload_addr =  (oal_uint8 *)(mac_get_cb_frame_hdr(pst_tx_cb))+st_tx_dump_param.uc_mac_hdr_len;
    st_tx_dump_param.us_mac_frame_len =  (oal_uint16)MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_cb)
                                                  + (oal_uint16)MAC_GET_CB_MPDU_LEN(pst_tx_cb);
#else
    st_tx_dump_param.puc_mac_hdr_addr = oal_netbuf_header(pst_netbuf_mgmt);
    st_tx_dump_param.puc_mac_payload_addr = oal_netbuf_payload(pst_netbuf_mgmt);
    st_tx_dump_param.uc_mac_hdr_len = uc_mac_hdr_len;
    st_tx_dump_param.us_mac_frame_len = us_frame_len;
#endif

    oal_set_mac_addr(st_tx_dump_param.auc_user_macaddr, auc_user_macaddr);

    dmac_tx_dump(&st_tx_dump_param);
}


oal_void  dmac_tx_pause_info(hal_to_dmac_device_stru *pst_hal_device, dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf)
{
    mac_tx_ctl_stru            *pst_tx_ctl_first;
    dmac_user_stru             *pst_dmac_user;
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
    dmac_user_query_stats_stru *pst_query_stats;
    mac_device_stru            *pst_mac_device;
    mac_rate_info_stru          st_txrate;
    static oal_uint8            uc_fe_print_ctrl = 0;   /* ????????????????????????3?????????? */
#endif /* #if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1102_DEV) */
    dmac_tid_stru              *pst_tid_queue;
    dmac_ba_tx_stru            *pst_tid_ba_hdl;

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    oal_netbuf_stru            *pst_netbuf_tmp;
    oal_dlist_head_stru        *pst_entry_tmp;
    oal_uint32                  ul_count = 0;
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    oal_uint32                 ul_mode_sel;
#endif

    pst_tx_ctl_first  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if(OAL_PTR_NULL == pst_tx_ctl_first)
    {
        return;
    }

    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        return;
    }

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        return;
    }
#endif /* #if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1102_DEV) */

    /* ??TID????0???? */
    pst_tid_queue = &pst_dmac_user->ast_tx_tid_queue[0];
#if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1151)
    OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info::uc_is_paused = %d, uc_num_dq = %d, uc_retry_num = %d, us_mpdu_num = %d",
      pst_tid_queue->uc_is_paused, pst_tid_queue->uc_num_dq, pst_tid_queue->uc_retry_num, pst_tid_queue->us_mpdu_num);
#else
    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info::user idx = %d, uc_is_paused = %d, uc_num_dq = %d, uc_retry_num = %d, us_mpdu_num = %d", 5,
                          pst_tid_queue->us_user_idx, pst_tid_queue->uc_is_paused, pst_tid_queue->uc_num_dq, pst_tid_queue->uc_retry_num, pst_tid_queue->us_mpdu_num);
#endif /* #if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1151) */

    pst_tid_ba_hdl = pst_tid_queue->pst_ba_tx_hdl;

    if (pst_tid_ba_hdl != OAL_PTR_NULL)
    {
        OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info::start = %d, last_seq_num = %d, size = %d, baw_head = %d",
          pst_tid_ba_hdl->us_baw_start, pst_tid_ba_hdl->us_last_seq_num, pst_tid_ba_hdl->us_baw_size, pst_tid_ba_hdl->us_baw_head);
    }

    /* ????????????????????????????????????mpdu??retry????mpdu????????retry num???????????????????????????????? */
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    ul_count = 0;
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry_tmp, &pst_tid_queue->st_retry_q)
    {
        ul_count++;
    }
    if (ul_count != pst_tid_queue->uc_retry_num)
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info::warning!!! retry num:%d != retry q element num.}", ul_count);
    }

    OAL_NETBUF_SEARCH_FOR_EACH(pst_netbuf_tmp, &pst_tid_queue->st_buff_head)
    {
        ul_count++;
    }
    if (ul_count != pst_tid_queue->us_mpdu_num)
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info::warning!!! mpdu num:%d != retry q and netbuf q element num.", ul_count);
    }
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
    pst_query_stats = &pst_dmac_user->st_query_stats;

    /* ??????????????????????????st_query_stats??1102???? */
    /*
    ul_drv_rx_pkts??    ????????(??????????????rx????????????????)????????????????????
    ul_rx_dropped_misc??????????(????????????)????

    tx_total??          ??????????????????????????????
    ul_hw_tx_pkts??     ??????????????(????AMPDU????????BA MAP) ????????????????
    ul_tx_failed??      ??????????????????????????????????????
    ul_hw_tx_failed:    ??????????????????????????????????????????????
    tx_retry_cnt??      ??????????????+????????????????????????????AMPDU= pkt_cnt??retry_cnt
    ul_HccPktMiss:         ??????????????
    */
    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info:TX_pkts: total = %u, tx_dropped = %u, tx_succ = %u, total_retry = %u, hw_tx_fail = %u.}", 5,
        pst_query_stats->ul_tx_total, pst_query_stats->ul_tx_failed, pst_query_stats->ul_hw_tx_pkts, pst_query_stats->ul_tx_retries, pst_query_stats->ul_hw_tx_failed);
    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info:RSSI = %d ; RX_pkts: total = %u, drv_dropped = %u, hcc_flowctrl_miss = %u, replay = %u, replay_droped = %u, rx_filter_encrypt_cnt = %u.}", 7,
        pst_dmac_vap->st_query_stats.ul_signal, pst_query_stats->ul_drv_rx_pkts, pst_query_stats->ul_rx_dropped_misc,g_sdio_stats.ulHccPktMiss, pst_query_stats->ul_rx_replay, pst_query_stats->ul_rx_replay_droped, pst_query_stats->ul_rx_filter_encrypt_cnt);

    dmac_config_get_tx_rate_info(&(pst_dmac_vap->st_tx_alg), &(pst_mac_device->st_mac_rates_11g[0]), &st_txrate);
    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info: tx rate info legacy = %d, mcs = %d, flags = %d, nss = %d, rx rate(kbps) = %d}", 5,
        st_txrate.legacy, st_txrate.mcs, st_txrate.flags, st_txrate.nss, pst_mac_device->pst_device_stru->ul_rx_rate);

    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_pause_info: hardware Q inf be= %d, vi = %d, vo = %d, mgmt = %d, netbuf(1544)free = %d}", 5,
        pst_hal_device->ast_tx_dscr_queue[WLAN_WME_AC_BE].uc_ppdu_cnt,
        pst_hal_device->ast_tx_dscr_queue[WLAN_WME_AC_VI].uc_ppdu_cnt,
        pst_hal_device->ast_tx_dscr_queue[WLAN_WME_AC_VO].uc_ppdu_cnt,
        pst_hal_device->ast_tx_dscr_queue[WLAN_WME_AC_MGMT].uc_ppdu_cnt,
        g_st_netbuf_pool.ast_subpool_table[2].us_free_cnt);

#ifdef _PRE_WLAN_DFT_STAT
#ifdef _PRE_WLAN_FEATURE_STA_PM
    /* mac???????????????????? */
    dmac_mac_key_statis_info(pst_dmac_vap,pst_mac_device);
#endif
#endif /* _PRE_WLAN_DFT_STAT */
    dmac_rx_compatibility_show_stat(pst_dmac_user);
    /* ?????????????????????????? */
    if (pst_hal_device->st_tx_excp_info.us_tx_excp_cnt)
    {
        OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                    "{dmac_tx_pause_info:: Excp_tx_cnt %d; The max_offset_tsf %d, with mpdu_len %d, q_num %d}",
                    pst_hal_device->st_tx_excp_info.us_tx_excp_cnt,
                    pst_hal_device->st_tx_excp_info.us_max_offset_tsf,
                    pst_hal_device->st_tx_excp_info.us_mpdu_len,
                    pst_hal_device->st_tx_excp_info.uc_q_num);
        OAL_MEMZERO((oal_uint8 *)&pst_hal_device->st_tx_excp_info, OAL_SIZEOF(hal_tx_excp_info_stru));
    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_get_btcoex_pa_status(&ul_mode_sel);
    OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_COEX, "{dmac_tx_pause_info:: mode select: 0x%x}", ul_mode_sel);
#endif

    /* ??????????????????????????????????OTA???????????????????? */
    if (0 == ((++uc_fe_print_ctrl) % 3))
    {
#ifdef _PRE_WLAN_DFT_STAT
        dmac_dft_report_all_ota_state(&pst_dmac_vap->st_vap_base_info);
#endif  /* _PRE_WLAN_DFT_STAT */
        /* ????sta?????????????? */
#ifdef _PRE_WLAN_FEATURE_STA_PM
        /* ?????????? */
        dmac_pm_key_info_dump(pst_dmac_vap);

        /* ??????????*/
        PM_WLAN_DumpSleepCnt();
#endif  /* _PRE_WLAN_FEATURE_STA_PM */

    }
#endif  /* #if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1102_DEV) */

    return;
}


oal_void  dmac_tx_excp_free_dscr(oal_dlist_head_stru *pst_tx_dscr_list_hdr, hal_to_dmac_device_stru *pst_hal_device)
{
    oal_netbuf_stru     *pst_netbuf = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_dscr;
    oal_dlist_head_stru *pst_dscr_node;

    while (OAL_FALSE == oal_dlist_is_empty(pst_tx_dscr_list_hdr))
    {
        pst_dscr_node = oal_dlist_delete_head(pst_tx_dscr_list_hdr);
        pst_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_node, hal_tx_dscr_stru, st_entry);

        pst_netbuf = pst_dscr->pst_skb_start_addr;
        pst_dscr->pst_skb_start_addr = OAL_PTR_NULL;
        if (OAL_SUCC != OAL_MEM_FREE((oal_void *)pst_dscr, OAL_TRUE))
        {
            OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_excp_free_dscr::pst_dscr null.}");
        }

        dmac_tx_excp_free_netbuf(pst_netbuf);
    }

    return;
}

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
OAL_STATIC oal_uint64 dmac_tx_generate_tkip_PN(oal_uint64 ull_sn, oal_uint8 uc_key_id)
{
    /* TKIP ????IV ????????
     * TSC1 WEPSeed[1] TSC0 [KEY_ID] TSC2 TSC3 TSC4 TSC5
     *                      |      |
     *                  b0  b4  b5  b6~b7
     *                  00000    1  key_id
     * WEPSeed[1] = (TSC1 | 0x20) & 0x7f
    */
    mac_pn_union   un_tkip_pn;

    un_tkip_pn.uc_pn8[0] = (ull_sn & 0x000000000000FF00) >> 8;
    un_tkip_pn.uc_pn8[1] = (un_tkip_pn.uc_pn8[0] | 0x20) & 0x7F;//WEPSeed[1]
    un_tkip_pn.uc_pn8[2] = (ull_sn & 0x00000000000000FF);
    un_tkip_pn.uc_pn8[3] = ((uc_key_id & 0x03) << 6) | BIT5;
    un_tkip_pn.uc_pn8[4] = (ull_sn & 0x0000000000FF0000) >> 16;
    un_tkip_pn.uc_pn8[5] = (ull_sn & 0x00000000FF000000) >> 24;
    un_tkip_pn.uc_pn8[6] = (ull_sn & 0x000000FF00000000) >> 32;
    un_tkip_pn.uc_pn8[7] = (ull_sn & 0x0000FF0000000000) >> 40;

    return un_tkip_pn.ull_pn64;
}

OAL_STATIC oal_uint64 dmac_tx_generate_ccmp_PN(oal_uint64 ull_sn, oal_uint8 uc_key_id)
{
    /* CCMP ????IV ????????
     * PN0 PN1 RSVD [KEY_ID] PN2 PN3 PN4 PN5
     *              |      |
     *          b0  b4  b5  b6~b7
     *          00000    1  key_id
    */

    mac_pn_union   un_ccmp_pn;

    un_ccmp_pn.uc_pn8[0] = (ull_sn & 0xFF);
    un_ccmp_pn.uc_pn8[1] = (ull_sn & 0x000000000000FF00) >> 8;
    un_ccmp_pn.uc_pn8[2] = 0;
    un_ccmp_pn.uc_pn8[3] = ((uc_key_id & 0x03) << 6) | BIT5;
    un_ccmp_pn.uc_pn8[4] = (ull_sn & 0x0000000000FF0000) >> 16;
    un_ccmp_pn.uc_pn8[5] = (ull_sn & 0x00000000FF000000) >> 24;
    un_ccmp_pn.uc_pn8[6] = (ull_sn & 0x000000FF00000000) >> 32;
    un_ccmp_pn.uc_pn8[7] = (ull_sn & 0x0000FF0000000000) >> 40;

    return un_ccmp_pn.ull_pn64;
}


OAL_STATIC oal_uint64 dmac_set_tx_dscr_PN(dmac_user_stru *pst_dmac_user, hal_tx_dscr_stru *pst_tx_dscr)
{
    mac_pn_union un_pn;

    /* ????sn ????CCMP/TKIP IV ?? */
    if (pst_dmac_user->st_user_base_info.st_key_info.en_cipher_type == WLAN_80211_CIPHER_SUITE_CCMP)
    {
        un_pn.ull_pn64 = dmac_tx_generate_ccmp_PN(pst_dmac_user->ull_sn, pst_dmac_user->st_user_base_info.st_key_info.uc_default_index);
    }
    else
    {
        un_pn.ull_pn64 = dmac_tx_generate_tkip_PN(pst_dmac_user->ull_sn, pst_dmac_user->st_user_base_info.st_key_info.uc_default_index);
    }
    (pst_dmac_user->ull_sn)++;

    /* ??????????iv ????????????????????CCMP header */
    hal_dscr_set_iv_value(pst_tx_dscr, un_pn.ul_pn32[0], un_pn.ul_pn32[1]);

    /* ??????????tx_pn_hw_bypass bit ??1 ??????????????IV ?? */
    hal_dscr_set_tx_pn_hw_bypass(pst_tx_dscr, 1);

    return un_pn.ull_pn64;
}

OAL_STATIC oal_uint8 dmac_tx_sw_set_ccmp_PN(hal_to_dmac_device_stru *pst_hal_device,
                                                        dmac_user_stru *pst_dmac_user,
                                                        hal_tx_dscr_stru *pst_tx_dscr,
                                                        oal_netbuf_stru *pst_netbuf)
{
    oal_uint8 *puc_pn_hdr;
    oal_uint64 ull_iv;

    /*
     * ????????PN ??????
     */
    if ((OAL_FALSE == pst_dmac_user->st_user_base_info.st_cap_info.bit_qos)
        || (OAL_TRUE == pst_dmac_user->st_user_base_info.en_is_multi_user))
    {
        return OAL_FALSE;
    }

    if (pst_dmac_user->st_user_base_info.st_key_info.en_cipher_type != WLAN_80211_CIPHER_SUITE_CCMP
        && pst_dmac_user->st_user_base_info.st_key_info.en_cipher_type != WLAN_80211_CIPHER_SUITE_TKIP)
    {
        return OAL_FALSE;
    }

    /* ????SN ???????????? */
    ull_iv = dmac_set_tx_dscr_PN(pst_dmac_user, pst_tx_dscr);

    /* ????iv ????MAC_HEADER ??????8 ???????????????????????????????? */
    puc_pn_hdr = oal_netbuf_get_pn_hdr_addr(pst_netbuf);
    if (puc_pn_hdr == OAL_PTR_NULL)
    {
        OAM_ERROR_LOG2(0, OAM_SF_TX, "{dmac_tx_sw_generate_iv::pn_hdr poiter null.iv_h %x, iv_l %x}",
                        (ull_iv >> 32)&0xFFFFFFFF,
                        (ull_iv)&0xFFFFFFFF);
        return OAL_FALSE;
    }
    *(oal_uint64 *)puc_pn_hdr = ull_iv;

#ifdef _PRE_WLAN_MAC_BUGFIX_PN_DEBUG
    {
        mac_tx_ctl_stru      *pst_cb;
        OAL_STATIC oal_uint32 ul_seq;
        OAL_STATIC oal_uint8  uc_is_qos = OAL_TRUE;
        pst_cb  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        /* ????????????TID???????????????????????? */
        uc_is_qos = mac_get_cb_is_qosdata(pst_cb);
        if( OAL_TRUE == uc_is_qos)
        {
            hal_tx_get_dscr_seq_num(pst_hal_device, pst_tx_dscr, (oal_uint16 *)&ul_seq);
        }
        else
        {
            hal_get_tx_sequence_num(pst_hal_device, 0, 0, 0, &ul_seq);
        }

        OAM_WARNING_LOG3(0, OAM_SF_TX, "{dmac_tx_sw_generate_iv::seq %04d, iv_h 0x%08X, iv_l 0x%08X}",
                        ul_seq,
                        (ull_iv >> 32)&0xFFFFFFFF,
                        (ull_iv)&0xFFFFFFFF);
        OAM_WARNING_LOG3(0, OAM_SF_TX, "{dmac_tx_sw_generate_iv::uc_is_qos %d, SN_H 0x%08X, SN_L 0x%08X}",
                        uc_is_qos,
                        (pst_dmac_user->ull_sn >>  32) & 0xFFFFFFFF,
                        (pst_dmac_user->ull_sn) & 0xFFFFFFFF);
    }
#endif  /* _PRE_WLAN_MAC_BUGFIX_PN_DEBUG */
    return OAL_TRUE;
}

oal_uint32 dmac_tx_sw_restore_ccmp_PN_from_mac_hdr(hal_to_dmac_device_stru *pst_hal_device,
                                                    hal_tx_dscr_stru       *pst_dscr)
{
    oal_uint8               *puc_pn_hdr;
    oal_netbuf_stru         *pst_buf;

    pst_buf = pst_dscr->pst_skb_start_addr;
    if (pst_buf == OAL_PTR_NULL)
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_sw_restore_ccmp_PN_from_mac_hdr::net_buffer is null.q_num = %d}", pst_dscr->uc_q_num);
        return OAL_FAIL;
    }

    puc_pn_hdr = oal_netbuf_get_pn_hdr_addr(pst_buf);
    if (puc_pn_hdr == OAL_PTR_NULL)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_sw_restore_ccmp_PN_from_mac_hdr::pn_hdr null.}");
        return OAL_FAIL;
    }

    hal_dscr_set_iv_value(pst_dscr,
        ((mac_pn_union*)puc_pn_hdr)->ul_pn32[0],
        ((mac_pn_union*)puc_pn_hdr)->ul_pn32[1]);
    return OAL_SUCC;
}

#endif /* _PRE_WLAN_MAC_BUGFIX_PN */

#ifdef _PRE_WLAN_FEATURE_VOWIFI

OAL_STATIC oal_void dmac_vowifi_update_arp_timestamp(dmac_vap_stru *pst_dmac_vap, oal_uint16 us_idx)
{
    dmac_user_stru *pst_dmac_user;

    if ((!IS_STA(&pst_dmac_vap->st_vap_base_info)) || (!IS_LEGACY_VAP(&pst_dmac_vap->st_vap_base_info)))
    {
        return ;
    }

    if (OAL_PTR_NULL == pst_dmac_vap->pst_vowifi_status)
    {
        return ;
    }

    /* ?????????????? */
    if (us_idx != pst_dmac_vap->st_vap_base_info.uc_assoc_vap_id)
    {
        return ;
    }

    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(us_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_VOWIFI,
                    "{dmac_vowifi_update_arp_timestamp::pst_dmac_user[%d] null!}", us_idx);
        return;
    }

    pst_dmac_vap->pst_vowifi_status->ull_arp_timestamp_ms = OAL_TIME_GET_STAMP_MS();
    pst_dmac_vap->pst_vowifi_status->ul_arp_rx_succ_pkts  = (pst_dmac_user->st_query_stats.ul_drv_rx_pkts >= pst_dmac_user->st_query_stats.ul_rx_dropped_misc)?
                                                     (pst_dmac_user->st_query_stats.ul_drv_rx_pkts - pst_dmac_user->st_query_stats.ul_rx_dropped_misc):
                                                     0;
}
#endif /* _PRE_WLAN_FEATURE_VOWIFI */

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif

oal_uint32  dmac_tx_excp_free_netbuf(oal_netbuf_stru *pst_netbuf)
{
    oal_netbuf_stru             *pst_buf_next = OAL_PTR_NULL;
    mac_ieee80211_frame_stru    *pst_frame_header = OAL_PTR_NULL;
    mac_tx_ctl_stru             *pst_tx_ctl = OAL_PTR_NULL;
    wlan_frame_type_enum_uint8   en_type;
#ifdef _PRE_MEM_DEBUG_MODE
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
    /*lint -e718*//*lint -e746*/
    oal_uint32 ul_return_addr      = (oal_uint32)__return_address();
    /*lint +e718*//*lint +e746*/
#endif
#endif
    if (OAL_PTR_NULL == pst_netbuf)
    {
        return OAL_FAIL;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    en_type = (wlan_frame_type_enum_uint8) MAC_GET_CB_FRAME_TYPE(pst_tx_ctl);
    pst_frame_header = (mac_ieee80211_frame_stru *)mac_get_cb_frame_hdr(pst_tx_ctl);
    if (OAL_PTR_NULL == pst_frame_header)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_excp_free_netbuf::pst_frame_header null.}");

        return OAL_FAIL;
    }
    mac_set_cb_frame_hdr(pst_tx_ctl, OAL_PTR_NULL);

    /*????802.11mac??????skb??????????????????????*/
    if((0 == mac_get_cb_80211_mac_head_type(pst_tx_ctl)) && (WLAN_DATA_BASICTYPE == en_type))
    {
        if (OAL_SUCC != OAL_MEM_FREE((oal_void *)pst_frame_header, OAL_TRUE))
        {
            OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_excp_free_netbuf::pst_frame_header null.}");

            return OAL_FAIL;
        }
    }

    while (OAL_PTR_NULL != pst_netbuf)
    {
        pst_buf_next = oal_get_netbuf_next(pst_netbuf);

        if(OAL_ERR_CODE_OAL_MEM_ALREADY_FREE==oal_netbuf_free(pst_netbuf))
        {
        #ifdef _PRE_MEM_DEBUG_MODE
        #if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
            OAL_IO_PRINT("double free caller[%x]!\r\n",ul_return_addr);
        #endif
        #endif
        }

        pst_netbuf = pst_buf_next;
    }

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_void dmac_save_frag_seq(dmac_user_stru     *pst_dmac_user,
                                                         mac_tx_ctl_stru    *pst_tx_ctl)
{
    oal_uint8                   uc_tid = 0;
    mac_ieee80211_frame_stru   *pst_mac_header = OAL_PTR_NULL;

    pst_mac_header = (mac_ieee80211_frame_stru   *)mac_get_cb_frame_hdr(pst_tx_ctl);
    uc_tid         = mac_get_cb_tid(pst_tx_ctl);

    if (pst_mac_header->st_frame_control.bit_more_frag != 0 && 0 == pst_mac_header->bit_frag_num)
    {
        pst_dmac_user->aus_txseqs_frag[uc_tid] = pst_dmac_user->aus_txseqs[uc_tid];
        pst_dmac_user->aus_txseqs[uc_tid] = DMAC_BA_SEQ_ADD(pst_dmac_user->aus_txseqs[uc_tid], 1);
    }
}


OAL_STATIC OAL_INLINE oal_void dmac_tx_seqnum_set(dmac_user_stru *pst_dmac_user, mac_tx_ctl_stru *pst_tx_ctl)
{
    oal_uint8                   uc_tid = 0;
    mac_ieee80211_frame_stru   *pst_mac_header = OAL_PTR_NULL;

    pst_mac_header = (mac_ieee80211_frame_stru *)mac_get_cb_frame_hdr(pst_tx_ctl);
    uc_tid         = mac_get_cb_tid(pst_tx_ctl);

    if (OAL_TRUE != mac_get_cb_is_qosdata(pst_tx_ctl))
    {
        return;
    }

    if (OAL_LIKELY((0 == pst_mac_header->st_frame_control.bit_more_frag ) && (0 == pst_mac_header->bit_frag_num)))
    {
        pst_mac_header->bit_seq_num 	= pst_dmac_user->aus_txseqs[uc_tid];
        mac_set_cb_seqnum(pst_tx_ctl, pst_dmac_user->aus_txseqs[uc_tid]);
        pst_dmac_user->aus_txseqs[uc_tid] = DMAC_BA_SEQ_ADD(pst_dmac_user->aus_txseqs[uc_tid], 1);
    }
    else
    {
        pst_mac_header->bit_seq_num = pst_dmac_user->aus_txseqs_frag[uc_tid];
    }

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    /* ??Qos????mac????????????DDR??????cache???????? */
    oal_dma_map_single(NULL, pst_mac_header, pst_tx_ctl->uc_frame_header_length, OAL_TO_DEVICE);
#endif
}


OAL_STATIC OAL_INLINE oal_void dmac_tx_seqnum_set_ampdu(dmac_user_stru *pst_dmac_user, mac_tx_ctl_stru *pst_tx_ctl)
{
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    mac_ieee80211_frame_stru   *pst_frame_hdr = OAL_PTR_NULL;
#endif
    oal_uint8                   uc_tid = 0;
    uc_tid                           = mac_get_cb_tid(pst_tx_ctl);

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    pst_frame_hdr                    = mac_get_cb_frame_hdr(pst_tx_ctl);
    pst_frame_hdr->bit_seq_num       = pst_dmac_user->aus_txseqs[uc_tid];
#endif
    mac_set_cb_seqnum(pst_tx_ctl, pst_dmac_user->aus_txseqs[uc_tid]);
    pst_dmac_user->aus_txseqs[uc_tid] = DMAC_BA_SEQ_ADD(pst_dmac_user->aus_txseqs[uc_tid], 1);

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    /* ??Qos????mac????????DDR??????cache?????? */
    oal_dma_map_single(NULL, pst_frame_hdr, pst_tx_ctl->uc_frame_header_length, OAL_TO_DEVICE);
#endif

}
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)

oal_uint32  dmac_tx_get_amsdu_params(
                                        oal_netbuf_stru  *pst_netbuf,
                                        mac_tx_ctl_stru  *pst_tx_ctl,
                                        hal_tx_mpdu_stru *pst_mpdu)
{
    oal_uint8            uc_msdu_num;                /* MPDU????msdu?????? */
    oal_uint8            uc_loop;
    oal_netbuf_stru     *pst_next_buf;
    oal_uint8           *puc_netbuf_data;            /* netbuf??data???? */

    if (OAL_UNLIKELY((OAL_TRUE == mac_get_cb_is_amsdu(pst_tx_ctl)) &&
                     (OAL_TRUE != MAC_GET_CB_IS_FIRST_MSDU(pst_tx_ctl))))
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_get_amsdu_params::not first MSDU in MPDU.");
        return OAL_FAIL;
    }

    uc_msdu_num = MAC_GET_CB_NETBUF_NUM(pst_tx_ctl);
    if((uc_msdu_num >> 1) > WLAN_DSCR_SUBTABEL_MAX_NUM)
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_get_amsdu_params::buffer overflow.");
        return OAL_FAIL;
    }

    for (uc_loop = 0; uc_loop < uc_msdu_num; uc_loop++)
    {
        pst_next_buf = oal_get_netbuf_next(pst_netbuf);

        /* ????SUB-MSDU???????? */
        puc_netbuf_data   = oal_netbuf_payload(pst_netbuf);

        /* ???????????????? */
        if (!(uc_loop & 0x1))
        {
            pst_mpdu->ast_msdu_addr[uc_loop >> 1].ul_msdu_addr0 = (oal_uint32)puc_netbuf_data;
            pst_mpdu->ast_msdu_addr[uc_loop >> 1].us_msdu0_len  = (oal_uint16)oal_netbuf_get_len(pst_netbuf);
        }
        else
        {
            pst_mpdu->ast_msdu_addr[uc_loop >> 1].ul_msdu_addr1 = (oal_uint32)puc_netbuf_data;
            pst_mpdu->ast_msdu_addr[uc_loop >> 1].us_msdu1_len  = (oal_uint16)oal_netbuf_get_len(pst_netbuf);
        }

        pst_netbuf = pst_next_buf;
    }

    /* ?????????????????????????????????????????????????????? */
    if (uc_msdu_num & 0x1)
    {
        pst_mpdu->ast_msdu_addr[uc_msdu_num >> 1].ul_msdu_addr1 = 0;
        pst_mpdu->ast_msdu_addr[uc_msdu_num >> 1].us_msdu1_len  = 0;
    }

    return OAL_SUCC;
}
#endif

oal_netbuf_stru* dmac_tx_dequeue_first_mpdu(oal_netbuf_head_stru  *pst_netbuf_head)
{
    oal_netbuf_stru        *pst_first_net_buf;
    oal_netbuf_stru        *pst_tmp_net_buf;
    oal_netbuf_stru        *pst_net_buf;
    mac_tx_ctl_stru        *pst_tx_ctrl;
    oal_uint8               uc_netbuf_num_per_mpdu;

    if(OAL_PTR_NULL == pst_netbuf_head)
    {
        return OAL_PTR_NULL;
    }

    pst_first_net_buf = OAL_NETBUF_HEAD_NEXT(pst_netbuf_head);
    pst_tx_ctrl       = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_first_net_buf);

    /* mpdu????a-msdu????????????net_buff???? */
    if (OAL_FALSE == mac_get_cb_is_amsdu(pst_tx_ctrl))
    {
        //OAM_INFO_LOG0(0, OAM_SF_TX, "{dmac_dequeue_first_mpdu::not amsdu.}\r\n");

        pst_first_net_buf = oal_netbuf_delist(pst_netbuf_head);

        return pst_first_net_buf;
    }

    /* ?????????????????????????????????? */
    if (OAL_FALSE == MAC_GET_CB_IS_FIRST_MSDU(pst_tx_ctrl))
    {
       /* ??????????????????????????????????????????????????????????????????????
           ????????????????????????????
        */
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_dequeue_first_mpdu::not the first msdu.}");

        return OAL_PTR_NULL;
    }

    /* ??????????????????mpdu??a-msdu??????skb???? */
    uc_netbuf_num_per_mpdu = MAC_GET_CB_NETBUF_NUM(pst_tx_ctrl);

    /* ????????mpdu????????skb??????????????????????????????net_buff?? */
    pst_first_net_buf = oal_netbuf_delist(pst_netbuf_head);
    uc_netbuf_num_per_mpdu--;

    pst_tmp_net_buf = pst_first_net_buf;
    while (0 != uc_netbuf_num_per_mpdu)
    {
        pst_net_buf = oal_netbuf_delist(pst_netbuf_head);
        oal_set_netbuf_prev(pst_net_buf, pst_tmp_net_buf);
        oal_set_netbuf_next(pst_net_buf, OAL_PTR_NULL);

        oal_set_netbuf_next(pst_tmp_net_buf, pst_net_buf);

        pst_tmp_net_buf = pst_net_buf;

        uc_netbuf_num_per_mpdu--;
    }

    return pst_first_net_buf;

}

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT

OAL_STATIC OAL_INLINE oal_void  dmac_tx_queue_mpdu(oal_netbuf_stru *pst_netbuf, oal_netbuf_head_stru *pst_head)
{
    oal_netbuf_stru    *pst_netbuf_tmp;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    mac_tx_ctl_stru    *pst_cb;
    oal_uint8           uc_netbuf_num_in_mpdu;
#endif

    pst_netbuf_tmp = pst_netbuf;
    /* ????????????netbuf?????? */
    while (OAL_PTR_NULL != pst_netbuf_tmp)
    {
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
        /* ????????mpdu????????net_buf??CB??????????mpdu????????????net_buff */
        pst_cb = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_netbuf_tmp);
        uc_netbuf_num_in_mpdu = MAC_GET_CB_NETBUF_NUM(pst_cb);
#endif
        /* ????mpdu????????net_buff???????????????? */
        pst_netbuf = pst_netbuf_tmp;
        while (
        #if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
            (0 != uc_netbuf_num_in_mpdu) &&
        #endif
            (OAL_PTR_NULL != pst_netbuf))
        {
            pst_netbuf_tmp = oal_get_netbuf_next(pst_netbuf);

            oal_netbuf_add_to_list_tail(pst_netbuf, pst_head);

            pst_netbuf = pst_netbuf_tmp;

        #if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
            uc_netbuf_num_in_mpdu--;
        #endif
        }
    }
}


OAL_STATIC OAL_INLINE oal_void  dmac_tx_queue_mpdu_head(oal_netbuf_stru *pst_netbuf, oal_netbuf_head_stru *pst_head)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_netbuf_addlist(pst_head, pst_netbuf);
#else/* 1151????????mpdu??????buf????,???????? */
    dmac_tx_queue_mpdu(pst_netbuf, pst_head);
#endif
    return;
}
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */


OAL_STATIC OAL_INLINE oal_void mac_check_cb_tid(mac_tx_ctl_stru *pst_tx_ctrl)
{
    if(mac_get_cb_tid(pst_tx_ctrl) > (WLAN_WME_MAX_TID_NUM -1))
    {
        oam_report_netbuf_cb(BROADCAST_MACADDR,(oal_uint8 *)pst_tx_ctrl,OAM_OTA_TYPE_TX_CB);
        OAM_ERROR_LOG2(0, OAM_SF_ANY,"{mac_set_cb_tid: uc_tid: [0x%x]; cb addr:0x%x}\r\n",mac_get_cb_tid(pst_tx_ctrl), pst_tx_ctrl);
    }
}

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif

OAL_STATIC OAL_INLINE oal_void  dmac_tx_set_txopps_param(dmac_vap_stru  *pst_dmac_vap,
                                                      dmac_user_stru *pst_dmac_user,
                                                      hal_tx_txop_feature_stru *pst_txop_feature,
                                                      oal_bool_enum_uint8       en_ismcast)
{
    /* ????????Draft P802.11ac_D3.1.pdf 9.17a???? */
    if (en_ismcast)
    {
        pst_txop_feature->st_groupid_partial_aid.uc_group_id    = 63;
        pst_txop_feature->st_groupid_partial_aid.us_partial_aid = 0;
    }
    else
    {
        pst_txop_feature->st_groupid_partial_aid.uc_group_id    = pst_dmac_user->uc_groupid;
        pst_txop_feature->st_groupid_partial_aid.us_partial_aid = pst_dmac_user->us_partial_aid;
    }

    switch (pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        case WLAN_VAP_MODE_BSS_STA:
            pst_txop_feature->st_groupid_partial_aid.uc_txop_ps_not_allowed = OAL_TRUE;
        break;

        case WLAN_VAP_MODE_BSS_AP:
            if (pst_dmac_vap->st_vap_base_info.st_cap_flag.bit_txop_ps)
            {
                pst_txop_feature->st_groupid_partial_aid.uc_txop_ps_not_allowed = OAL_FALSE;
            }
            else
            {
                pst_txop_feature->st_groupid_partial_aid.uc_txop_ps_not_allowed = OAL_TRUE;
            }
        break;

        default:
        break;
    }
}


OAL_STATIC OAL_INLINE oal_void dmac_tx_get_txop(dmac_vap_stru  *pst_dmac_vap,
                                          dmac_user_stru *pst_dmac_user,
                                          hal_tx_txop_feature_stru *pst_txop_feature,
                                          oal_bool_enum_uint8       en_ismcast)
{
    /* ????TXOP???????????? */
    pst_txop_feature->pst_security = &(pst_dmac_user->st_user_base_info.st_user_tx_info.st_security);
    pst_txop_feature->pst_security->en_cipher_protocol_type = pst_dmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_txop_feature->pst_security->uc_cipher_key_id        = pst_dmac_user->st_user_base_info.st_key_info.uc_default_index;

    dmac_tx_set_txopps_param(pst_dmac_vap, pst_dmac_user, pst_txop_feature, en_ismcast);
}
#if 0

OAL_STATIC OAL_INLINE oal_void  dmac_tx_update_bandwidth_mode_ap(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, hal_tx_txop_alg_stru *pst_txop_alg)
{
    oal_uint8   uc_channel_bandwidth = 0;    /* BW20 */

    /* AP????????40MHz???? */
    /* (1) AP????????40MHz???? */
    /* (2) ????STA????40MHz???? */
    if ((WLAN_BAND_WIDTH_40PLUS  == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_40MINUS ==  pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth))
    {
        if (1 == pst_dmac_user->st_user_base_info.st_ht_hdl.bit_supported_channel_width)
        {
            uc_channel_bandwidth = BIT2;     /* BW40 */

        }
    }

    /* AP????????80MHz???? */
    /* (1) AP????????80MHz???? */
    /* (2) ????STA????80MHz???? */
    if ((WLAN_BAND_WIDTH_80PLUSPLUS   == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_80PLUSMINUS  ==  pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth) ||
        (WLAN_BAND_WIDTH_80MINUSPLUS  == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_80MINUSMINUS == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth))
    {
        if (0 == pst_dmac_user->st_user_base_info.st_vht_hdl.bit_supported_channel_width)
        {
            uc_channel_bandwidth = BIT3;     /* BW80 */
        }
    }

    pst_txop_alg->st_rate.uc_channel_bandwidth = OAL_MIN(pst_txop_alg->st_rate.uc_channel_bandwidth, uc_channel_bandwidth);
}


OAL_STATIC OAL_INLINE oal_void  dmac_tx_update_bandwidth_mode_sta(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, hal_tx_txop_alg_stru *pst_txop_alg)
{
    pst_txop_alg->st_rate.uc_channel_bandwidth = 0;             /* BW20 */

    /* STA????????40MHz???? */
    /* (1) STA????????40MHz??????????AP????40MHz???? */
    /* (2) ????AP??????40MHz */
    if ((WLAN_BAND_WIDTH_40PLUS  == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_40MINUS ==  pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth))
    {
        if (1 == pst_dmac_user->st_user_base_info.st_ht_hdl.bit_sta_chan_width)
        {
            pst_txop_alg->st_rate.uc_channel_bandwidth = BIT2;  /* BW40 */
        }
    }

    /* STA????????40MHz???? */
    /* (1) STA????????80MHz??????????AP????80MHz???? */
    /* (2) ????AP??????80MHz */
    if ((WLAN_BAND_WIDTH_80PLUSPLUS   == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_80PLUSMINUS  ==  pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth) ||
        (WLAN_BAND_WIDTH_80MINUSPLUS  == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth)  ||
        (WLAN_BAND_WIDTH_80MINUSMINUS == pst_dmac_vap->st_vap_base_info.st_channel.en_bandwidth))
    {
        if (1 == pst_dmac_user->st_user_base_info.st_vht_hdl.uc_channel_width)
        {
            pst_txop_alg->st_rate.uc_channel_bandwidth = BIT3; /* BW80 */
        }
    }
}


OAL_STATIC OAL_INLINE oal_void  dmac_tx_update_bandwidth_mode(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, hal_tx_txop_alg_stru *pst_txop_alg)
{
    if (WLAN_VAP_MODE_BSS_AP == pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        dmac_tx_update_bandwidth_mode_ap(pst_dmac_vap, pst_dmac_user, pst_txop_alg);
    }
    else if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        dmac_tx_update_bandwidth_mode_sta(pst_dmac_vap, pst_dmac_user, pst_txop_alg);
    }
}
#endif
#ifdef  _PRE_WLAN_FEATURE_TSF_SYNC

oal_uint16 dmac_sync_tx_dscr_tsf(hal_to_dmac_vap_stru *pst_hal_vap, oal_uint16 us_tsf)
{
    oal_uint16      us_tmp_tsf_lo = 0;
    dmac_vap_stru   *pst_dmac_vap;


    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_hal_vap->uc_mac_vap_id);
    if(OAL_PTR_NULL == pst_dmac_vap)
    {
        return us_tmp_tsf_lo;
    }

    us_tmp_tsf_lo = us_tsf - pst_dmac_vap->us_sync_tsf_value; /*sync the tsf*/
    if (pst_dmac_vap->us_sync_tsf_value != 0)
    {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "dmac_sync_tx_dscr_tsf:origi tsf:[%d],after sync us_tmp_tsf_lo:[%d]",us_tsf, us_tmp_tsf_lo);
    }
    return us_tmp_tsf_lo;
}
#endif


OAL_STATIC oal_void dmac_tx_get_timestamp(hal_to_dmac_vap_stru *pst_hal_vap, oal_uint16 *pus_tsf)
{
    oal_uint32                   ul_tsf = 0;
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    hal_to_dmac_device_stru     *pst_hal_device;
    hal_to_dmac_vap_stru        *pst_hal_vap_sta0;

    hal_get_hal_to_dmac_device(pst_hal_vap->uc_chip_id,pst_hal_vap->uc_device_id, &pst_hal_device);

    /* proxysta ????tsf??????????????Proxy STA????????????sta??tsf??????????,??????Proxy STA??hal_vap????sta0??hal_vap */
    if ((pst_hal_vap->uc_vap_id >= WLAN_PROXY_STA_START_ID) &&(pst_hal_vap->uc_vap_id <= WLAN_PROXY_STA_END_ID))
    {
        //OAM_ERROR_LOG0(pst_hal_vap->uc_vap_id,OAM_SF_PROXYSTA, "{dmac_tx_get_timestamp::vap is a proxysta}");
        hal_get_hal_vap(pst_hal_device, WLAN_STA0_HAL_VAP_ID, &pst_hal_vap_sta0);
        hal_vap_tsf_get_32bit(pst_hal_vap_sta0, &ul_tsf);
    }
    else
    {
        //OAM_ERROR_LOG0(pst_hal_vap->uc_vap_id,OAM_SF_PROXYSTA, "{dmac_tx_get_timestamp::vap is not a proxysta}");
        hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf);
    }
#else
    hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf);
#endif

#if ((_PRE_OS_VERSION_RAW == _PRE_OS_VERSION) &&defined(_PRE_PRODUCT_ID_HI110X_DEV))
    /*Zourong add for debug,make sure not read tsf when inner tsf is being stopped*/
    if(0!=(READW(0x50000400)&(1<<3)))
    {
        OAM_ERROR_LOG0(pst_hal_vap->uc_vap_id,OAM_SF_TX, "{dmac_tx_get_timestamp::Read TSF When ext tsf is working}");
    }
#endif
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    *pus_tsf = ((ul_tsf >> 10) & 0xffff) - 1; /* ????????????????????16?? */
#else
    *pus_tsf = ((ul_tsf >> 10) & 0xffff); /* ????????????????????16?? */

#ifdef  _PRE_WLAN_FEATURE_TSF_SYNC
    *pus_tsf = dmac_sync_tx_dscr_tsf(pst_hal_vap, *pus_tsf);
#endif

#endif
}


OAL_STATIC oal_uint32  dmac_tx_bf_add_ht_control_field(
                                    hal_to_dmac_device_stru  *pst_hal_device,
                                    mac_tx_ctl_stru          *pst_tx_ctl,
                                    oal_dlist_head_stru      *pst_tx_dscr_list_hdr,
                                    hal_tx_txop_alg_stru     *pst_txop_alg,
                                    hal_tx_ppdu_feature_stru *pst_ppdu_feature)
{
    hal_tx_dscr_stru                        *pst_tx_dscr;
    oal_dlist_head_stru                     *pst_dscr_entry;
    oal_uint8                                uc_htc_hdr_len;

    if (!mac_get_cb_is_qosdata(pst_tx_ctl))
    {
        /* ??QoS??????????HTC?????????????? */
        return OAL_SUCC;
    }

    if (OAL_TRUE == mac_get_cb_is_use_4_addr(pst_tx_ctl))
    {
        uc_htc_hdr_len = MAC_80211_QOS_HTC_4ADDR_FRAME_LEN;
    }
    else
    {
        uc_htc_hdr_len = MAC_80211_QOS_HTC_FRAME_LEN;
    }

    if ((WLAN_STAGGERED_SOUNDING == pst_txop_alg->st_rate.en_sounding_mode)
         && (uc_htc_hdr_len != MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl)))
    {
        /* ??????sounding??????????????HTC??????HTC?? */

        pst_dscr_entry = pst_tx_dscr_list_hdr->pst_next;
        pst_tx_dscr    = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

        dmac_tx_set_htc_field(pst_hal_device, pst_tx_dscr, pst_tx_ctl, pst_ppdu_feature);
    }
    else if ((WLAN_STAGGERED_SOUNDING != pst_txop_alg->st_rate.en_sounding_mode)
        && (uc_htc_hdr_len == MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl)))
    {
        /* ????????sounding????????????HTC??????HTC?? */

        pst_dscr_entry = pst_tx_dscr_list_hdr->pst_next;
        pst_tx_dscr    = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

        dmac_tx_unset_htc_field(pst_hal_device, pst_tx_dscr, pst_tx_ctl, pst_ppdu_feature);
    }

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_uint32 dmac_tx_dscr_queue_add_dscr_list(
                hal_to_dmac_device_stru *       pst_hal_device,
                oal_uint8                       uc_qid,
                oal_dlist_head_stru            *pst_tx_dscr_list_hdr,
                oal_uint8                       uc_ppdu_cnt)
{
    /* ???????????????????????????????????????? ?????????????? */
    oal_dlist_join_tail(&(pst_hal_device->ast_tx_dscr_queue[uc_qid].st_header), pst_tx_dscr_list_hdr);
    pst_hal_device->ast_tx_dscr_queue[uc_qid].uc_ppdu_cnt += uc_ppdu_cnt;

#ifdef _PRE_DEBUG_MODE
    if (pst_hal_device->ast_tx_dscr_queue[uc_qid].uc_ppdu_cnt > 8)
    {
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }
#endif

    return OAL_SUCC;
}

oal_void dmac_post_soft_tx_complete_event(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_base_dscr, oal_uint8 uc_dscr_num)
{
    frw_event_mem_stru         *pst_event_mem = OAL_PTR_NULL;          /* ?????????????????????? */
    frw_event_stru             *pst_hal_to_dmac_event = OAL_PTR_NULL;  /* ??????????????payload???? */
    hal_tx_complete_event_stru *pst_tx_comp_event;

    /* ???????????? */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(hal_tx_complete_event_stru));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        return;
    }

    /* ???????????? */
    pst_hal_to_dmac_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&pst_hal_to_dmac_event->st_event_hdr,
                        FRW_EVENT_TYPE_WLAN_TX_COMP,
                        HAL_TX_COMP_SUB_TYPE_TX,
                        OAL_SIZEOF(hal_tx_complete_event_stru),
                        FRW_EVENT_PIPELINE_STAGE_0,
                        pst_hal_device->uc_chip_id,
                        pst_hal_device->uc_device_id,
                        0);

    /*????tx complete???? */
    pst_tx_comp_event = (hal_tx_complete_event_stru *)(pst_hal_to_dmac_event->auc_event_data);
    pst_tx_comp_event->pst_base_dscr    = pst_base_dscr;        /* ???????????????? */
    pst_tx_comp_event->uc_dscr_num      = uc_dscr_num;          /* ?????????????????? */
    pst_tx_comp_event->pst_hal_device   = pst_hal_device;

    /* ???? */
    frw_event_dispatch_event(pst_event_mem);

    /* ???????????? */
    FRW_EVENT_FREE(pst_event_mem);
}

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT

OAL_STATIC oal_void dmac_tx_data_flush_cache(mac_tx_ctl_stru *pst_tx_ctl, oal_dlist_head_stru *pst_tx_dscr_list_hdr)
{
    oal_dlist_head_stru     *pst_dlist_node;
    oal_netbuf_stru         *pst_netbuf = OAL_PTR_NULL;
    hal_tx_dscr_stru        *pst_dscr   = OAL_PTR_NULL;

    oal_uint16               us_frame_len = 0;     /* ???????????????? */
    oal_uint8               *puc_frame_head;       /* ???????? */
    oal_uint8               *puc_frame_data;       /* ???????? */
    oal_uint8                uc_frame_head_len = 0;

    pst_dlist_node = pst_tx_dscr_list_hdr->pst_next;
    while (pst_dlist_node != pst_tx_dscr_list_hdr)
    {
        pst_dscr = OAL_DLIST_GET_ENTRY(pst_dlist_node, hal_tx_dscr_stru, st_entry);
        pst_netbuf = pst_dscr->pst_skb_start_addr;

        if(OAL_PTR_NULL != pst_netbuf)
        {
            pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
            us_frame_len = (oal_uint16)MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) + (oal_uint16)oal_netbuf_get_len(pst_netbuf);

            if(1 == pst_tx_ctl->bit_80211_mac_head_type)  /* ????mac header??data?????? */
            {
                oal_dma_map_single(NULL, OAL_NETBUF_DATA(pst_netbuf), us_frame_len, OAL_TO_DEVICE);
            }
            else  /* MAC????data?????????????? */
            {
                puc_frame_head = (oal_uint8 *)(mac_get_cb_frame_hdr(pst_tx_ctl));
                uc_frame_head_len = MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl);
                oal_dma_map_single(NULL, puc_frame_head, uc_frame_head_len, OAL_TO_DEVICE);   /* MAC?? */

                puc_frame_data = oal_netbuf_payload(pst_netbuf);
                oal_dma_map_single(NULL, puc_frame_data, us_frame_len - uc_frame_head_len, OAL_TO_DEVICE);  /* payload */
            }
        }
        else
        {
            OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_data_flush_cache::pst_netbuf null.}");
        }

        pst_dlist_node = pst_dlist_node->pst_next;
    }
}
#endif  /* _PRE_WLAN_CACHE_COHERENT_SUPPORT */



oal_uint32  dmac_tx_data(
                dmac_vap_stru            *pst_dmac_vap,
                dmac_user_stru           *pst_dmac_user,
                mac_tx_ctl_stru          *pst_tx_ctl,
                oal_dlist_head_stru      *pst_tx_dscr_list_hdr,
                hal_tx_ppdu_feature_stru *pst_ppdu_feature,
                hal_tx_txop_alg_stru     *pst_txop_alg)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    hal_tx_dscr_stru            *pst_tx_dscr;
    hal_tx_txop_feature_stru     st_txop_feature;
    oal_dlist_head_stru         *pst_dscr_entry;
    hal_tx_dscr_stru            *pst_dscr_temp;
    oal_uint8                    uc_ppdu_cnt;
    mac_tx_ctl_stru             *pst_cb = OAL_PTR_NULL;
    oal_uint8                    uc_ac;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    oal_uint32                   ul_bank3_bcn_period = 0;
#endif
    hal_tx_dscr_stru    *pst_last_dscr;
    oal_dlist_head_stru *pst_last_dscr_entry;
    oal_uint32           ul_tx_q_status;
    hal_tx_queue_type_enum_uint8 uc_q_num;

#ifdef _PRE_DEBUG_MODE
	oal_uint8                    uc_tid;
	dmac_tid_stru               *pst_tid_queue = OAL_PTR_NULL;
#endif

#ifdef _PRE_WLAN_SW_CTRL_RSP
    dmac_device_stru            *pst_dmac_dev = OAL_PTR_NULL;
#endif

    mac_device_stru             *pst_mac_device;
    pst_mac_device = (mac_device_stru*)mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_data::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


    pst_hal_device = pst_dmac_vap->pst_hal_device;

#ifdef _PRE_WLAN_CHIP_TEST
    DMAC_CHIP_TEST_CALL(dmac_test_lpm_txopps_set_partial_aid(pst_dmac_vap, pst_dmac_user, DMAC_TEST_STUB_BEGIN));
    DMAC_CHIP_TEST_CALL(dmac_test_lpm_smps_set_rate(pst_tx_ctl,pst_txop_alg,DMAC_TEST_STUB_BEGIN));
    DMAC_CHIP_TEST_CALL(dmac_test_dfx_set_tx_cnt(pst_txop_alg));
#endif

    dmac_tx_get_txop(pst_dmac_vap, pst_dmac_user, &st_txop_feature, pst_tx_ctl->en_ismcast);

    if ((pst_tx_ctl->bit_is_eapol_key_ptk == OAL_TRUE)
        && (pst_dmac_user->bit_is_rx_eapol_key_open == OAL_TRUE))
    {
        st_txop_feature.pst_security->en_cipher_protocol_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    }

    /*????????????????????????????????????????????more data*/
    if (OAL_TRUE == pst_dmac_user->bit_ps_mode)
    {
        dmac_psm_tx_set_more_data(pst_dmac_user, pst_tx_ctl);
    }
#ifdef _PRE_WLAN_FEATURE_STA_PM
    dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
#endif

    /* STA??????????????????????power_mgmt?? */
#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        dmac_psm_tx_set_power_mgmt_bit(pst_dmac_vap, pst_tx_ctl);
    }
#endif
    pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_tx_dscr_list_hdr->pst_next, hal_tx_dscr_stru, st_entry);

/* ??1102????tsf_restore???? */
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    if ((WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode))
    {
    	/* ??????????????staut bank3 ??beacon??????0 ??????ap??tsf??????sta */
        hal_vap_get_beacon_period(pst_dmac_vap->pst_hal_vap, &ul_bank3_bcn_period);
        if (0 == ul_bank3_bcn_period)
        {
            OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,"tx data staut bank3 bcn period 0 ap sync tsf to sta");
            hal_sta_tsf_restore(pst_dmac_vap->pst_hal_vap);
        }
    }
#endif

    /* debug????????????????????????tsf*/
#ifdef _PRE_DEBUG_MODE
    dmac_tx_get_timestamp(pst_dmac_vap->pst_hal_vap, &pst_ppdu_feature->us_tsf);
#else
    /* release??????VO/VI??????????????tsf*/
    if ((WLAN_WME_AC_VI == mac_get_cb_ac(pst_tx_ctl)) || (WLAN_WME_AC_VO == mac_get_cb_ac(pst_tx_ctl)))
    {
        dmac_tx_get_timestamp(pst_dmac_vap->pst_hal_vap, &pst_ppdu_feature->us_tsf);
    }
    dmac_tx_get_timestamp(pst_dmac_vap->pst_hal_vap, &pst_ppdu_feature->us_tsf);
#endif

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_TX_GET_TSF);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_GET_TSF);

    dmac_tx_bf_add_ht_control_field(pst_hal_device, pst_tx_ctl, pst_tx_dscr_list_hdr, pst_txop_alg, pst_ppdu_feature);

#ifdef _PRE_WLAN_CHIP_TEST
    DMAC_CHIP_TEST_CALL(dmac_test_sch_modify_txdscp_timestamp(&pst_ppdu_feature->us_tsf,
                        g_st_dmac_test_mng.s_ct_sch_lifetime_delay_ms));
    DMAC_CHIP_TEST_CALL(dmac_test_sch_set_long_nav_enable(pst_tx_dscr));
#endif

    /* ????????????,????ampdu?????????????????????? */
    if (OAL_TRUE == pst_ppdu_feature->uc_ampdu_enable)
    {
        hal_tx_ucast_data_set_dscr(pst_hal_device, pst_tx_dscr, &st_txop_feature, pst_txop_alg, pst_ppdu_feature);
        uc_ppdu_cnt = 1;
    }
    else  /*MPDU??????????????????*/
    {
         OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, pst_tx_dscr_list_hdr)
         {
            pst_dscr_temp = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

            pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_dscr_temp->pst_skb_start_addr);

            /* ???? ??????????????hal_tx_umbcast_set_dscr??????????????*/
            if (OAL_TRUE == MAC_GET_CB_IS_MCAST(pst_cb))
            {
                pst_txop_alg = &(pst_dmac_vap->st_tx_data_mcast);
                hal_tx_non_ucast_data_set_dscr(pst_hal_device, pst_dscr_temp, &st_txop_feature, pst_txop_alg, pst_ppdu_feature);

            }
            else
            {
                hal_tx_ucast_data_set_dscr(pst_hal_device, pst_dscr_temp, &st_txop_feature, pst_txop_alg, pst_ppdu_feature);
            }
         }

        uc_ppdu_cnt = MAC_GET_CB_MPDU_NUM(pst_tx_ctl);
        if (0 == uc_ppdu_cnt)
        {
            OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_data::tid %d, uc_dscr_num is zero.}", mac_get_cb_tid(pst_cb));
        }

    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_tx_vip_frame(pst_hal_device, &(pst_dmac_vap->st_vap_base_info), pst_tx_dscr_list_hdr);
#endif

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_TX_SET_DSCR);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_SET_DSCR);

    if (OAL_SWITCH_ON == oam_report_data_get_global_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX))
    {
        dmac_tx_dump_data(pst_hal_device, pst_tx_ctl, pst_tx_dscr_list_hdr);
    }

#ifdef _PRE_WLAN_FEATURE_DFR
    if (OAL_TRUE == pst_hal_device->en_dfr_enable)
    {
        /* ?????????????????????????????? */
        if (OAL_FALSE == pst_hal_device->st_dfr_tx_prot.st_tx_prot_timer.en_is_enabled)
        {
            FRW_TIMER_RESTART_TIMER(&(pst_hal_device->st_dfr_tx_prot.st_tx_prot_timer), WLAN_TX_PROT_TIMEOUT, OAL_TRUE);
        }
    }
#endif

/* ??????????????Memory */
#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    dmac_tx_data_flush_cache(pst_tx_ctl, pst_tx_dscr_list_hdr);
#endif

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    /* ??????????????????????mac??????????????????????????mac?? */
    if (OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag)
    {
        pst_cb = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_tx_dscr->pst_skb_start_addr);
        if(RF_PAYLOAD_ALL_ZERO == pst_dmac_vap->st_vap_base_info.bit_payload_flag)
        {
            OAL_MEMZERO(mac_get_cb_frame_hdr(pst_cb), OAL_SIZEOF(mac_ieee80211_frame_stru));
        }
        else if(RF_PAYLOAD_ALL_ONE == pst_dmac_vap->st_vap_base_info.bit_payload_flag)
        {
            oal_memset(mac_get_cb_frame_hdr(pst_cb), 0xFF, OAL_SIZEOF(mac_ieee80211_frame_stru));
        }
    }

#endif

    /* ?????????????????????????????????? */
    uc_ac = mac_get_cb_ac(pst_tx_ctl);
    dmac_tx_dscr_queue_add_dscr_list(pst_hal_device, HAL_AC_TO_Q_NUM(uc_ac), pst_tx_dscr_list_hdr, uc_ppdu_cnt);

#ifdef _PRE_WLAN_SW_CTRL_RSP
    pst_dmac_dev = dmac_res_get_mac_dev(pst_hal_device->uc_device_id);
    if (OAL_PTR_NULL == pst_dmac_dev)
    {
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_data::pst_dmac_dev null.dev id [%d]}", pst_hal_device->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((uc_ac < WLAN_WME_AC_BUTT)
        && (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
        && (OAL_TRUE == pst_dmac_dev->en_state_in_sw_ctrl_mode))
    {
        dmac_vap_update_rsp_frm_rate(pst_dmac_vap->st_vap_base_info.uc_vap_id,
                                    pst_txop_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_vht_nss_mcs.bit_protocol_mode,
                                    pst_txop_alg->st_rate.uc_channel_bandwidth,
                                    pst_txop_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_vht_nss_mcs.bit_vht_mcs);
    }
#endif

#ifdef _PRE_WLAN_CHIP_TEST
    /* ??????4???? */
    DMAC_CHIP_TEST_CALL(dmac_test_set_addr4(pst_tx_dscr));
    DMAC_CHIP_TEST_CALL(dmac_test_set_dscr_software_retry(pst_hal_device, pst_tx_dscr));
    DMAC_CHIP_TEST_CALL(dmac_test_lpm_txopps_set_partial_aid(pst_dmac_vap, pst_dmac_user, DMAC_TEST_STUB_END));
    DMAC_CHIP_TEST_CALL(dmac_test_lpm_smps_set_rate(pst_tx_ctl,pst_txop_alg,DMAC_TEST_STUB_END));
	//DMAC_CHIP_TEST_CALL(dmac_test_always_tx(pst_hal_device, pst_tx_dscr));
#endif

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    /* ????????chip test,?????????? */
    if (OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag)
    {
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
         hal_tx_ctrl_dscr_link(pst_hal_device, pst_tx_dscr, pst_tx_dscr);
#else
         hal_rf_test_enable_al_tx(pst_hal_device, pst_tx_dscr);
#endif
    }
#endif /* #ifdef _PRE_WLAN_FEATURE_ALWAYS_TX */

#ifdef _PRE_DEBUG_MODE
    g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_data_num += uc_ppdu_cnt;
    OAM_INFO_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "ul_tx_data_num = %d", g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_data_num);

    uc_tid = mac_get_cb_tid(pst_tx_ctl);
    pst_tid_queue = &(pst_dmac_user->ast_tx_tid_queue[uc_tid]);

    if ((pst_tx_dscr->bit_is_ampdu == 0) && (DMAC_TX_MODE_AGGR == pst_tid_queue->en_tx_mode))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_data::sending none_ampdu while ba is setup.}");
    }
    else if ((pst_tx_dscr->bit_is_ampdu == 1) && (DMAC_TX_MODE_NORMAL == pst_tid_queue->en_tx_mode))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_data::sending ampdu while ba is not setup.}");
    }

#endif

    /* ??????????????????????????????????????????????MAC */
    uc_ac = mac_get_cb_ac(pst_tx_ctl);

#ifdef _PRE_WLAN_FEATURE_DBAC
    if ((pst_hal_device->uc_current_chan_number != pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number) &&
         (MAC_VAP_STATE_INIT != pst_dmac_vap->st_vap_base_info.en_vap_state))
    {
        OAM_ERROR_LOG3(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_DBAC, "put tx_dscr in wrong channel. vap chan:%d, hal chan:%d. vap state:%d",
                                pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number,
                                pst_hal_device->uc_current_chan_number,
                                pst_dmac_vap->st_vap_base_info.en_vap_state);

        oam_report_80211_frame(BROADCAST_MACADDR,
                            (oal_uint8*)mac_get_cb_frame_hdr(pst_tx_ctl),
                            MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl),
                            (oal_uint8*)oal_netbuf_payload(pst_tx_dscr->pst_skb_start_addr),
                            MAC_GET_CB_MPDU_LEN(pst_tx_ctl),
                            OAM_OTA_FRAME_DIRECTION_TYPE_TX);
    }
#endif

    uc_q_num = HAL_AC_TO_Q_NUM(uc_ac);

    if (OAL_GET_THRUPUT_BYPASS_ENABLE(OAL_TX_HAL_HARDWARE_BYPASS))
    {
        dmac_post_soft_tx_complete_event(pst_hal_device, pst_tx_dscr, MAC_GET_CB_MPDU_NUM(pst_tx_ctl));
        return OAL_SUCC;
    }

    /* ??????????????????????*/
    hal_get_tx_q_status(pst_hal_device, &ul_tx_q_status, uc_q_num);

    /* ????????????0??????????????????FIFO?? */
    if ((HAL_TX_QUEUE_HI == uc_q_num) && (0 != ul_tx_q_status))
    {
        OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
        /* ???????????????????????????????????? */
        pst_last_dscr_entry = pst_hal_device->ast_tx_dscr_queue[uc_q_num].st_header.pst_prev->pst_prev;
        pst_last_dscr       = OAL_DLIST_GET_ENTRY(pst_last_dscr_entry, hal_tx_dscr_stru, st_entry);

        hal_tx_ctrl_dscr_link(pst_hal_device, pst_last_dscr, pst_tx_dscr);
        if (pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt > 10)
        {
            OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "dmac_tx_data::q_num = [%d], ul_tx_q_status = [%d], ppdu_cnt[%d], tid num [%d]",
                             uc_q_num, ul_tx_q_status, pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt, mac_get_cb_tid(pst_tx_ctl));
        }
    }
    else
    {

        OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
        /* ???????? */
        hal_tx_put_dscr(pst_hal_device, uc_q_num, pst_tx_dscr);
    }

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_TX_PUT_DSCR);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_PUT_DSCR);

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8  dmac_tx_need_enqueue_tid(
                hal_to_dmac_device_stru *pst_hal_device,
#ifdef _PRE_WLAN_FEATURE_DBAC
                mac_vap_stru            *pst_mac_vap,
#endif
                dmac_tid_stru           *pst_tid_queue,
                oal_uint8                uc_q_num)
{
    mac_device_stru   *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_need_enqueue_tid::pst_mac_device null.}");
        return OAL_TRUE;
    }

    /* (1)????????????????????????
       (2)??????????TID????????????????
       (3)????TID??????
       (4)wlan????????????????1?
       (5)ampdu????                      */
       /* DBAC??????????VAP?????????????? */
    if ((DMAC_TX_MODE_NORMAL != pst_tid_queue->en_tx_mode) ||
        (pst_tid_queue->uc_is_paused > 0) ||
        ((0 != pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt) && (HAL_TX_QUEUE_HI != uc_q_num)) ||
#ifdef _PRE_WLAN_FEATURE_DBAC
        (OAL_TRUE == mac_need_enqueue_tid_for_dbac(pst_mac_device, pst_mac_vap)) ||
#endif
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        (OAL_FALSE == oal_dlist_is_empty(&pst_tid_queue->st_retry_q)) ||
        (OAL_FALSE == oal_netbuf_list_empty(&pst_tid_queue->st_buff_head)))
#else
        (OAL_FALSE == oal_dlist_is_empty(&pst_tid_queue->st_hdr)))
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
    {
        return OAL_TRUE;
    }

    /* ??????????????????????????????????????????????????tid??????????5g??11b */
    if (MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state)
    {
        if (uc_q_num < HAL_TX_QUEUE_HI)
        {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}



#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
OAL_STATIC OAL_INLINE oal_uint32  dmac_tid_tx_queue_enqueue(
                mac_device_stru    *pst_device,
                mac_vap_stru       *pst_vap,
                dmac_tid_stru      *pst_tid_queue,
                oal_uint8           uc_mpdu_num,
                oal_netbuf_stru    *pst_netbuf)
#else
OAL_STATIC OAL_INLINE oal_uint32  dmac_tid_tx_queue_enqueue(
                mac_device_stru    *pst_device,
                mac_vap_stru       *pst_vap,
                dmac_tid_stru      *pst_tid_queue,
                hal_tx_dscr_stru   *pst_tx_dscr,
                oal_uint8           uc_mpdu_num,
                oal_netbuf_stru    *pst_netbuf)
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
{
    oal_time_us_stru            st_timestamp_us = {0, 0};
    oal_netbuf_stru            *pst_netbuf_tmp;
    mac_tx_ctl_stru            *pst_cb;
#ifdef _PRE_WLAN_FEATURE_DBAC
    oal_uint32                  ul_ret;
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_DFT_STAT
    dmac_tid_stats_stru   *pst_tid_stats;
    pst_tid_stats = pst_tid_queue->pst_tid_stats;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tid_stats))
    {
        OAM_ERROR_LOG4(pst_tid_queue->uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_enqueue::tid_stats is null. tid:%d,is paused:%d,mpdu_num:%d,user idx:%d}",
                       pst_tid_queue->uc_tid,
                       pst_tid_queue->uc_is_paused,
                       pst_tid_queue->us_mpdu_num,
                       pst_tid_queue->us_user_idx);

        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
#endif

    /* ????device?????????????????? */
    if (OAL_UNLIKELY((pst_device->us_total_mpdu_num + 1) > WLAN_TID_MPDU_NUM_LIMIT))
    {
        OAM_WARNING_LOG0(pst_vap->uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_enqueue::us_total_mpdu_num exceed.}");
        DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_enqueue_full_cnt, uc_mpdu_num);

        return OAL_FAIL;
    }

    pst_netbuf_tmp = pst_netbuf;
    while (OAL_PTR_NULL != pst_netbuf_tmp)
    {
        pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf_tmp);
        /* VO/VI ??????????????????????????*/
        if ((WLAN_WME_AC_VI == mac_get_cb_ac(pst_cb)) || (WLAN_WME_AC_VO == mac_get_cb_ac(pst_cb)))
        {
            /*????????????????????????????netbuf????????????????*/
            if ((0 == st_timestamp_us.i_sec) && (0 ==st_timestamp_us.i_usec))
            {
                dmac_timestamp_get(&st_timestamp_us);
            }

            MAC_GET_CB_TIMESTAMP(pst_cb).i_sec  = st_timestamp_us.i_sec;
            MAC_GET_CB_TIMESTAMP(pst_cb).i_usec = st_timestamp_us.i_usec;
        }
        pst_netbuf_tmp = oal_get_netbuf_next(pst_netbuf_tmp);
    }

    pst_device->us_total_mpdu_num += uc_mpdu_num;
    pst_tid_queue->us_mpdu_num += uc_mpdu_num;
    DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_enqueue_total_cnt, uc_mpdu_num);
    pst_device->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id] += uc_mpdu_num;
    pst_device->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)] += uc_mpdu_num;

#ifdef _PRE_WLAN_FEATURE_FLOWCTL
    dmac_alg_flowctl_backp_notify(pst_vap, pst_device->us_total_mpdu_num, pst_device->aus_ac_mpdu_num);
#endif

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    dmac_tx_queue_mpdu(pst_netbuf, &pst_tid_queue->st_buff_head);
#else
    OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
    oal_dlist_add_tail(&pst_tx_dscr->st_entry, &pst_tid_queue->st_hdr);
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

#ifdef _PRE_WLAN_FEATURE_DBAC
    ul_ret = dmac_alg_enqueue_tid_notify(pst_vap, pst_tid_queue, uc_mpdu_num);
    if (OAL_UNLIKELY(OAL_SUCC !=  ul_ret))
    {
        OAM_WARNING_LOG1(pst_vap->uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_enqueue::dmac_alg_enqueue_tid_notify failed[%d].}", ul_ret);
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}


OAL_INLINE oal_void  dmac_al_tx_set_mpdu_param (mac_vap_stru *pst_mac_vap, oal_uint16 us_payload_len, hal_tx_mpdu_stru *pst_mpdu)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    if(OAL_SWITCH_ON == pst_mac_vap->bit_al_tx_flag)
    {
        pst_mpdu->ast_msdu_addr[0].us_msdu0_len = us_payload_len;
    }
#endif
#endif

}

OAL_INLINE oal_bool_enum_uint8  dmac_is_al_tx (dmac_vap_stru *pst_dmac_vap)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

        /* ??????????????OK */
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
        if((OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag))
        {
            return OAL_TRUE;
        }
#endif

#endif
        return OAL_FALSE;
}


OAL_INLINE oal_bool_enum_uint8  dmac_is_vap_state_ok (dmac_vap_stru *pst_dmac_vap)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if (OAL_UNLIKELY(!((MAC_VAP_STATE_UP == pst_dmac_vap->st_vap_base_info.en_vap_state) ||
    (MAC_VAP_STATE_PAUSE == pst_dmac_vap->st_vap_base_info.en_vap_state))))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
        "{dmac_tx_process_data_event:vap state[%d] != MAC_VAP_STATE_{UP|PAUSE}!}\r\n",
        pst_dmac_vap->st_vap_base_info.en_vap_state);
        return OAL_FALSE;
    }
#endif
    return OAL_TRUE;

}


OAL_INLINE oal_bool_enum_uint8  dmac_is_user_state_ok (dmac_user_stru *pst_dmac_user)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if(MAC_USER_STATE_BUTT == pst_dmac_user->st_user_base_info.en_user_asoc_state)
    {
        /*data frames stored in the hmac/dmac pipeline when dmac offload*/
        OAM_WARNING_LOG1(pst_dmac_user->st_user_base_info.uc_vap_id, OAM_SF_TX,
                         "{dmac_is_user_state_ok::user(id[%d]) state is MAC_USER_STATE_BUTT! ", pst_dmac_user->st_user_base_info.us_assoc_id);
        return OAL_FALSE;
    }
#endif
    return OAL_TRUE;
}
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
hal_tx_dscr_stru* dmac_tx_dscr_alloc(hal_to_dmac_device_stru   *pst_hal_device,
                                                        dmac_vap_stru *pst_dmac_vap,
                                                      oal_netbuf_stru *pst_netbuf)
{
    oal_uint32                 ul_ret;
    mac_tx_ctl_stru           *pst_tx_ctl_first;
    hal_tx_mpdu_stru           st_mpdu;
    oal_uint16                 us_tx_dscr_len;
    hal_tx_dscr_stru          *pst_tx_dscr;
    mac_device_stru           *pst_mac_device;
    oal_uint8                  uc_ac;
    oal_uint8                  uc_q_num;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    oal_mem_pool_id_enum_uint8 en_mem_poo_id;
#endif

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if(OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_dscr_alloc::tx pkt is dropped, pst_mac_device null.}");
        return OAL_PTR_NULL;
    }

    pst_tx_ctl_first  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if(OAL_PTR_NULL == pst_tx_ctl_first)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_dscr_alloc::tx pkt is dropped, tx ctl null.}");
        return OAL_PTR_NULL;
    }

    uc_ac       = mac_get_cb_ac(pst_tx_ctl_first);
    uc_q_num    = HAL_AC_TO_Q_NUM(uc_ac);

    /* ????MPDU???????? */
    ul_ret = dmac_tx_get_mpdu_params(pst_netbuf, pst_tx_ctl_first, &st_mpdu);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_dscr_alloc::tx pkt is dropped, dmac_tx_get_mpdu_params failed[%d].", ul_ret);
        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);

        return OAL_PTR_NULL;
    }

    dmac_al_tx_set_mpdu_param(&pst_dmac_vap->st_vap_base_info, MAC_GET_CB_MPDU_BYTES(pst_tx_ctl_first), &st_mpdu);

    //OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_GET_SET_MPDU_PARAMS);

    /*??????????????????????????????????????????hal_tx_get_size_dscr????????*/
    //us_tx_dscr_len = (MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first) == 1) ? WLAN_MEM_SHARED_TX_DSCR_SIZE1 : WLAN_MEM_SHARED_TX_DSCR_SIZE2;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    if (MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first) == 1)
    {
        us_tx_dscr_len = WLAN_MEM_SHARED_TX_DSCR_SIZE1;
        en_mem_poo_id = OAL_MEM_POOL_ID_TX_DSCR_1;
    }
    else
    {
        us_tx_dscr_len = WLAN_MEM_SHARED_TX_DSCR_SIZE2;
        en_mem_poo_id = OAL_MEM_POOL_ID_TX_DSCR_2;
    }

    /* ?????????????????? */
    pst_tx_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(en_mem_poo_id, us_tx_dscr_len, OAL_FALSE);
#else
    us_tx_dscr_len = (MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first) == 1) ? WLAN_MEM_SHARED_TX_DSCR_SIZE1 : WLAN_MEM_SHARED_TX_DSCR_SIZE2;
    pst_tx_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_SHARED_DSCR, us_tx_dscr_len, OAL_FALSE);
#endif
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_dscr))
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_dscr_alloc::tx pkt is dropped, pst_tx_dscr alloc failed.}");

        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);
#ifdef _PRE_WLAN_DFT_STAT
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        dmac_dft_print_mac_phy_rf(pst_mac_device);
#endif
#endif
        return OAL_PTR_NULL;
    }

    OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
    OAL_MEMZERO(pst_tx_dscr, us_tx_dscr_len);

    /* ????mpdu???? ???????? */
    pst_tx_dscr->us_original_mpdu_len = MAC_GET_CB_MPDU_LEN(pst_tx_ctl_first) + MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl_first);
    pst_tx_dscr->bit_is_retried = OAL_FALSE;
    pst_tx_dscr->bit_is_first   = 0;

    /* ??????????ac */
    pst_tx_dscr->uc_q_num = uc_q_num;

    hal_tx_fill_basic_ctrl_dscr(pst_hal_device, pst_tx_dscr, &st_mpdu);

    return pst_tx_dscr;
}

#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */


OAL_STATIC OAL_INLINE oal_void dmac_judge_forbid_rts(dmac_user_stru *pst_dmac_user, hal_tx_txop_alg_stru *pst_txop_alg)
{
    oal_uint8   uc_rate_rank;

    /* ??????RTS????????????????, ????????RTS */
    if (OAL_TRUE == pst_dmac_user->bit_forbid_rts)
    {
        for (uc_rate_rank = 0; uc_rate_rank < HAL_TX_RATE_MAX_NUM; uc_rate_rank++)
        {
            pst_txop_alg->ast_per_rate[uc_rate_rank].rate_bit_stru.bit_rts_cts_enable = OAL_FALSE;
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT

OAL_STATIC oal_bool_enum_uint8 dmac_frame_is_null_data(oal_netbuf_stru *pst_net_buf)
{
    mac_ieee80211_frame_stru       *pst_mac_header;
    oal_uint8                       uc_frame_type;
    oal_uint8                       uc_frame_subtype;

    pst_mac_header  = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_net_buf);
    uc_frame_type    = mac_frame_get_type_value((oal_uint8 *)pst_mac_header);
    uc_frame_subtype = mac_frame_get_subtype_value((oal_uint8 *)pst_mac_header);

    if ((WLAN_DATA_BASICTYPE == uc_frame_type)
         && ((WLAN_NULL_FRAME == uc_frame_subtype) || (WLAN_QOS_NULL_FRAME == uc_frame_subtype)))
    {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
#endif

OAL_STATIC oal_uint32 dmac_tx_check_user_state(dmac_vap_stru *pst_dmac_vap, mac_tx_ctl_stru *pst_tx_ctl_first,
                                               dmac_user_stru *pst_dmac_user)
{
#ifdef _PRE_WLAN_FEATURE_ROAM
    /* ??????????port??????????????eapol??????,????????????????????tid???? */
    if (pst_dmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_ROAMING &&
        mac_user_get_port(&(pst_dmac_user->st_user_base_info)) == OAL_FALSE &&
        MAC_GET_CB_IS_EAPOL(pst_tx_ctl_first) == OAL_FALSE &&
        dmac_is_al_tx(pst_dmac_vap) != OAL_TRUE) {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
            "{dmac_tx_check_state::port not valid.user idx [%d]}", MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first));
        return OAL_ERR_CODE_SECURITY_PORT_INVALID;
    }
#endif
    return OAL_SUCC;
}


OAL_INLINE oal_uint32  dmac_tx_process_data(hal_to_dmac_device_stru *pst_hal_device, dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf)
{
    mac_tx_ctl_stru             *pst_tx_ctl_first;
    dmac_user_stru              *pst_dmac_user;
    dmac_tid_stru               *pst_tid_queue;
    oal_uint32                   ul_ret = OAL_SUCC;
    hal_tx_dscr_stru            *pst_tx_dscr = OAL_PTR_NULL;
    oal_dlist_head_stru          st_tx_dscr_list_hdr;
#ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    oal_uint16                   us_tx_dscr_len = 0;
    hal_tx_mpdu_stru             st_mpdu;
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
    oal_uint8                    uc_mpdu_num;
    oal_bool_enum_uint8          en_is_pkt_need_buff;  /* mpdu???????????????????????? */
    hal_tx_ppdu_feature_stru     st_ppdu_feature;
    hal_tx_txop_alg_stru        *pst_txop_alg = OAL_PTR_NULL;
    mac_device_stru             *pst_mac_device;
    oal_uint8                    uc_ac;
    oal_uint8                    uc_q_num;

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_DATA);

    /* ????netbuf??????????netbuf??CB */
    pst_tx_ctl_first  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if(OAL_PTR_NULL == pst_tx_ctl_first)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{oal_netbuf_cb::skb buf poiter null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_mpdu_num = 1;

    /* ????DMAC?????????????? */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::pst_dmac_user null.user idx [%d]}", MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first));
        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if(OAL_TRUE != dmac_is_al_tx(pst_dmac_vap))
    {
        if(OAL_TRUE != dmac_is_vap_state_ok(pst_dmac_vap)
            || OAL_TRUE != dmac_is_user_state_ok(pst_dmac_user))
        {
            dmac_tx_excp_free_netbuf(pst_netbuf);
            return OAL_SUCC;
        }
    }

    ul_ret = dmac_tx_check_user_state(pst_dmac_vap, pst_tx_ctl_first, pst_dmac_user);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /* ?????????????? */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if(0 != MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first))
    {
        if(0 != g_st_mac_res.st_hash_res.puc_user_cnt[MAC_GET_CB_TX_USER_IDX(pst_tx_ctl_first) - 1])
        {
            if (OAL_UNLIKELY(OAL_FALSE == pst_dmac_user->bit_active_user))
            {
                ul_ret = dmac_user_tx_inactive_user_handler(pst_dmac_user);
                if (OAL_SUCC != ul_ret)
                {
                    OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_process_data::dmac_user_tx_inactive_user_handler failed[%d].", ul_ret);
                    OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);
                    return ul_ret;
                }
            }
        }
    }
#else
    if (OAL_UNLIKELY(OAL_FALSE == pst_dmac_user->bit_active_user))
    {
        ul_ret = dmac_user_tx_inactive_user_handler(pst_dmac_user);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_process_data::dmac_user_tx_inactive_user_handler failed[%d].", ul_ret);
            OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);
            return ul_ret;
        }
    }
#endif

    /* ???????????????????????? */
#ifdef _PRE_WLAN_FEATURE_UAPSD
    if(mac_vap_get_uapsd_en(&pst_dmac_vap->st_vap_base_info)
        &&(OAL_FALSE == MAC_GET_CB_IS_MCAST(pst_tx_ctl_first))
        &&(OAL_FALSE == MAC_GET_CB_IS_FROM_PS_QUEUE(pst_tx_ctl_first))
        &&(MAC_USR_UAPSD_AC_CAN_DELIVERY(mac_get_cb_ac(pst_tx_ctl_first), pst_dmac_user)))
    {
        return dmac_uapsd_tx_enqueue(pst_dmac_vap, pst_dmac_user, pst_netbuf);
    }
#endif
    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_tx_process_data::pst_mac_device[%d] null!}", pst_hal_device->uc_mac_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??????????????????????????????????????,??????????????????????,P2P noa??????????????*/
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if ((0 != pst_dmac_vap->uc_ps_user_num) || ((OAL_TRUE == (oal_uint8)IS_P2P_PS_ENABLED(pst_dmac_vap))&&(OAL_TRUE == pst_mac_device->st_p2p_info.en_p2p_ps_pause)))/* ???????? */
#else
    if (0 != pst_dmac_vap->uc_ps_user_num)
#endif
    {
        en_is_pkt_need_buff = dmac_psm_pkt_need_buff(pst_dmac_vap, pst_dmac_user, pst_netbuf);
        if (OAL_TRUE == en_is_pkt_need_buff)
        {
            return dmac_psm_enqueue(pst_dmac_vap, pst_dmac_user, pst_netbuf);
        }
    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_INACTIVE_USER_HDL);

    /* ?????????????????? */
    ul_ret = dmac_alg_downlink_flowctl_notify(&pst_dmac_vap->st_vap_base_info,
                                     &pst_dmac_user->st_user_base_info,
                                     pst_netbuf);

    /* ??????????????????????????*/
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_alg_process_dropped, 1);
        OAM_INFO_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                      "{dmac_tx_process_data:: alg_flowctl process return [%d], dev_mpdu_num = %d}", ul_ret, pst_mac_device->us_total_mpdu_num);
        OAM_INFO_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data:: be = %d, bk = %d, vi = %d, vo = %d}",
                      pst_mac_device->aus_ac_mpdu_num[WLAN_WME_AC_BE],
                      pst_mac_device->aus_ac_mpdu_num[WLAN_WME_AC_BK],
                      pst_mac_device->aus_ac_mpdu_num[WLAN_WME_AC_VI],
                      pst_mac_device->aus_ac_mpdu_num[WLAN_WME_AC_VO]);
#ifdef _PRE_WLAN_PERFORM_STAT
        dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), mac_get_cb_tid(pst_tx_ctl_first), 1, 1, DMAC_STAT_PER_BUFF_OVERFLOW);
#endif
        return ul_ret;
    }

#ifdef _PRE_WLAN_PERFORM_STAT
    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), mac_get_cb_tid(pst_tx_ctl_first), 1, 0, DMAC_STAT_PER_BUFF_OVERFLOW);
#endif

    /* ???????????????????????? */
    oal_dlist_init_head(&st_tx_dscr_list_hdr);

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_FLOWCTRL);

    uc_ac       = mac_get_cb_ac(pst_tx_ctl_first);
    uc_q_num    = HAL_AC_TO_Q_NUM(uc_ac);

#ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    /* ????MPDU???????? */
    ul_ret = dmac_tx_get_mpdu_params(pst_netbuf, pst_tx_ctl_first, &st_mpdu);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::dmac_tx_get_mpdu_params failed[%d].", ul_ret);

        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);

        return OAL_FAIL;
    }

    dmac_al_tx_set_mpdu_param(&pst_dmac_vap->st_vap_base_info, MAC_GET_CB_MPDU_BYTES(pst_tx_ctl_first), &st_mpdu);

    //OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_GET_SET_MPDU_PARAMS);

    /*??????????????????????????????????????????hal_tx_get_size_dscr????????*/
    us_tx_dscr_len = (MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first) == 1) ? WLAN_MEM_SHARED_TX_DSCR_SIZE1 : WLAN_MEM_SHARED_TX_DSCR_SIZE2;

    /* ?????????????????? */
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_tx_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_TX_DSCR_1, us_tx_dscr_len, OAL_FALSE);
#else
    pst_tx_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_SHARED_DSCR, us_tx_dscr_len, OAL_FALSE);
#endif
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_dscr))
    {
        if (WLAN_MEM_SHARED_TX_DSCR_SIZE1 == us_tx_dscr_len)
        {
            OAM_ERROR_LOG3(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
            "{dmac_tx_process_data::pst_tx_dscr alloc failed, us_tx_dscr_len[%d], NETBUF_NUM[%d], amsdu capable[%d].}",
            us_tx_dscr_len, MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first), pst_tx_ctl_first->en_is_amsdu);
        }
        else
        {
            
            OAM_WARNING_LOG3(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
            "{dmac_tx_process_data::pst_tx_dscr alloc failed, us_tx_dscr_len[%d], NETBUF_NUM[%d], amsdu capable[%d].}",
            us_tx_dscr_len, MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first), pst_tx_ctl_first->en_is_amsdu);
        }

        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);
#ifdef _PRE_WLAN_DFT_STAT
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        dmac_dft_print_mac_phy_rf(pst_mac_device);
#endif
#endif
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
    OAL_MEMZERO(pst_tx_dscr, us_tx_dscr_len);
    /* ????mpdu???? ???????? */
    pst_tx_dscr->us_original_mpdu_len = MAC_GET_CB_MPDU_LEN(pst_tx_ctl_first) + MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl_first);
    pst_tx_dscr->bit_is_retried = OAL_FALSE;
    pst_tx_dscr->bit_is_first   = 0;

    /* ??????????ac */
    pst_tx_dscr->uc_q_num = uc_q_num;

    hal_tx_fill_basic_ctrl_dscr(pst_hal_device, pst_tx_dscr, &st_mpdu);

#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_FILL_DSCR);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* ??cb??????tid???????? */
    mac_check_cb_tid(pst_tx_ctl_first);
#endif

    /* ??????????????TID???? */
    pst_tid_queue = &(pst_dmac_user->ast_tx_tid_queue[mac_get_cb_tid(pst_tx_ctl_first)]);

    /* ????????????????????????TID??????????????????????????????????MPDU??TID???????? */
#ifdef _PRE_WLAN_FEATURE_DBAC
    if (OAL_TRUE == dmac_tx_need_enqueue_tid(pst_hal_device, &pst_dmac_vap->st_vap_base_info, pst_tid_queue, uc_q_num))
#else
    if (OAL_TRUE == dmac_tx_need_enqueue_tid(pst_hal_device, pst_tid_queue, uc_q_num))
#endif
    {
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
        /* ???????????????? */
        if (OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag)
        {
            return OAL_SUCC;
        }
#endif
        pst_tid_queue->uc_vap_id = pst_dmac_user->st_user_base_info.uc_vap_id;
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        ul_ret = dmac_tid_tx_queue_enqueue(pst_mac_device, &pst_dmac_vap->st_vap_base_info, pst_tid_queue, uc_mpdu_num, pst_netbuf);
    #else
        ul_ret = dmac_tid_tx_queue_enqueue(pst_mac_device, &pst_dmac_vap->st_vap_base_info, pst_tid_queue, pst_tx_dscr, uc_mpdu_num, pst_netbuf);
    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        if (OAL_LIKELY(OAL_SUCC == ul_ret))
        {
            dmac_alg_tid_update_notify(pst_tid_queue);
        }
        else
        {
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
            pst_dmac_vap->st_vap_base_info.st_vap_stats.ul_tx_dropped_packets += MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first);
#endif
            OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::dmac_tid_tx_queue_enqueue fail.}");
        #ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
            OAL_MEM_FREE((oal_void *)pst_tx_dscr, OAL_TRUE);
        #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

            /* Amsdu??????????????netbuf list?????????????? */
            dmac_tx_excp_free_netbuf(pst_netbuf);
        }

        OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_TID_ENQUEUE);

#if 0  /* mayuan */
        OAM_INFO_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::uc_ppdu_cnt = %d and pst_tid_queue->uc_is_paused = %d.}",
                    pst_hal_device->ast_tx_dscr_queue[pst_tx_dscr->uc_q_num].uc_ppdu_cnt,
                    pst_tid_queue->c_is_paused);
#endif

#ifdef _PRE_WIFI_DMT
        /* AMPDU???????????????????????????????????????????????????????? */
        if ((1 >= pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt) && (0 == pst_tid_queue->uc_is_paused)
        && (pst_tid_queue->us_mpdu_num >= witp_dmt_get_ampdu_aggr_num()))
#else
        if ((1 >= pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt) && (0 == pst_tid_queue->uc_is_paused))
#endif
        {
            dmac_tx_complete_schedule(pst_hal_device, mac_get_cb_ac(pst_tx_ctl_first));
        }

        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    pst_tx_dscr = dmac_tx_dscr_alloc(pst_hal_device, pst_dmac_vap, pst_netbuf);
    if (OAL_PTR_NULL == pst_tx_dscr)
    {
        if (OAL_FALSE == dmac_frame_is_null_data(pst_netbuf))
        {
            OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::alloc tx dscr failed, pkt need enqueue tid queue.}");
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
            if (OAL_SWITCH_ON == pst_dmac_vap->st_vap_base_info.bit_al_tx_flag)
            {
                /* ???????????????? */
                return OAL_SUCC;
            }
#endif
            pst_tid_queue->uc_vap_id = pst_dmac_user->st_user_base_info.uc_vap_id;

            ul_ret = dmac_tid_tx_queue_enqueue(pst_mac_device, &pst_dmac_vap->st_vap_base_info, pst_tid_queue, uc_mpdu_num, pst_netbuf);
            if (OAL_LIKELY(OAL_SUCC == ul_ret))
            {
                dmac_alg_tid_update_notify(pst_tid_queue);
            }
            else
            {
                OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::enqueue tid queue fail, pkt is dropped.}");
                /* Amsdu??????????????netbuf list?????????????? */
                dmac_tx_excp_free_netbuf(pst_netbuf);
            }

            OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_TID_ENQUEUE);
            return OAL_SUCC;
        }

        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::pkt is dropped, alloc tx dscr failed.}");
        return OAL_FAIL;
    }
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

    /*??????????????????????????*/
    if((OAL_PTR_NULL != pst_mac_device) && (OAL_TRUE == MAC_DEV_IS_RESET_IN_PROGRESS(pst_mac_device)))
    {
       OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::MAC_DEV_IS_RESET_IN_PROGRESS.}");

       OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);

       OAL_MEM_FREE((oal_void *)pst_tx_dscr, OAL_TRUE);
       return OAL_ERR_CODE_RESET_INPROGRESS;
    }

    /* ?????????????????????????????????????? */
    oal_dlist_add_tail(&pst_tx_dscr->st_entry, &st_tx_dscr_list_hdr);

    /* ?????????????????????????? */
    hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_tx_dscr);

    /* ??????ppdu???????? */
    dmac_tx_init_ppdu_feature(pst_dmac_vap, pst_dmac_user, 1, &st_ppdu_feature);
    dmac_tx_update_ppdu_feature(pst_netbuf, &st_ppdu_feature);

    MAC_GET_CB_MPDU_NUM(pst_tx_ctl_first) = 1;

    /* ????TXOP???????????? */
    dmac_tx_get_txop_alg_params(pst_dmac_vap, pst_dmac_user, pst_tx_ctl_first, &pst_txop_alg);
    if (OAL_PTR_NULL == pst_txop_alg)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data::pst_txop_alg null.}");
        OAM_STAT_VAP_INCR(pst_dmac_vap->st_vap_base_info.uc_vap_id, tx_abnormal_mpdu_dropped, 1);

        OAL_MEM_FREE((oal_void *)pst_tx_dscr, OAL_TRUE);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (WLAN_PROT_NO != pst_dmac_vap->st_vap_base_info.st_protection.en_protection_mode)
    {
        /*????????????????????????*/
        dmac_tx_update_protection_txop_alg(pst_dmac_vap, pst_txop_alg, OAL_FALSE, pst_tx_ctl_first->en_ismcast);
    }

    /* ????????????????RTS */
    dmac_judge_forbid_rts(pst_dmac_user, pst_txop_alg);

    /* 2040???????????????????? */
    /* dmac_tx_update_bandwidth_mode(pst_dmac_vap, pst_dmac_user, pst_txop_alg); */

    dmac_save_frag_seq(pst_dmac_user, pst_tx_ctl_first);
    dmac_tx_seqnum_set(pst_dmac_user, pst_tx_ctl_first);

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
    /* ????????PN ?? */
    dmac_tx_sw_set_ccmp_PN(pst_hal_device, pst_dmac_user, pst_tx_dscr, pst_netbuf);
#endif

    /* ???????????????????? */
    if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_tx_ctl_first))
    {
#ifdef _PRE_WLAN_DFT_STAT
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_pkts,
                                    MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first));
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_bytes,
                                    MAC_GET_CB_MPDU_BYTES(pst_tx_ctl_first));
#endif
        DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_total, MAC_GET_CB_NETBUF_NUM(pst_tx_ctl_first));
    }



    /* ???????????? */
    dmac_tx_data(pst_dmac_vap, pst_dmac_user, pst_tx_ctl_first, &st_tx_dscr_list_hdr, &st_ppdu_feature, pst_txop_alg);

    return OAL_SUCC;
}



oal_uint32  dmac_tx_process_data_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru            *pst_event;
    frw_event_hdr_stru        *pst_event_hdr;
    hal_to_dmac_device_stru   *pst_hal_device;
    dmac_vap_stru             *pst_dmac_vap;
    dmac_tx_event_stru        *pst_tx_event;
    oal_netbuf_stru           *pst_netbuf;
    mac_tx_ctl_stru           *pst_tx_ctl;
    oal_netbuf_stru           *pst_netbuf_next;
    oal_uint32                 ul_ret;
    oal_uint8                  uc_data_type;

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_START);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_process_data_event::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????????????????????????payload?????? */
    pst_event     = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event_hdr = &pst_event->st_event_hdr;
    pst_tx_event  = (dmac_tx_event_stru *)pst_event->auc_event_data;

    /* ????vap???????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_event_hdr->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data_event::mac_res_get_dmac_vap fail.vap_id = %d}",pst_event_hdr->uc_vap_id);
        dmac_tx_excp_free_netbuf(pst_tx_event->pst_netbuf);
        OAM_STAT_VAP_INCR(pst_event_hdr->uc_vap_id, tx_abnormal_mpdu_dropped, 1);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hal_device = pst_dmac_vap->pst_hal_device;

    /* ????device???????? */
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data_event::pst_hal_device null.}");

        dmac_tx_excp_free_netbuf(pst_tx_event->pst_netbuf);

        OAM_STAT_VAP_INCR(pst_event_hdr->uc_vap_id, tx_abnormal_mpdu_dropped, 1);
        return OAL_SUCC;
    }

    /* ????pps???? */
    dmac_auto_freq_pps_count(1);

    pst_netbuf = pst_tx_event->pst_netbuf;

    /* ???????????????? */
    while (pst_netbuf != OAL_PTR_NULL)
    {
        pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        /* ????amsdu??????amsdu??????????????????????????????next??????NULL */
        if (OAL_TRUE != mac_get_cb_is_amsdu(pst_tx_ctl))
        {
            pst_netbuf_next = oal_get_netbuf_next(pst_netbuf);
            oal_set_netbuf_next(pst_netbuf, OAL_PTR_NULL);
        }
        else
        {
            pst_netbuf_next = OAL_PTR_NULL;
        }

        /* ???????????????????????????? */
#ifdef _PRE_WLAN_FEATURE_ROAM
        if (pst_dmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING &&
            MAC_GET_CB_IS_EAPOL(pst_tx_ctl) == OAL_TRUE)
        {
            OAM_WARNING_LOG2(0, OAM_SF_TX, "{dmac_tx_process_data_event::dmac_tx_process_data statel[%d] len[%d].}", pst_dmac_vap->st_vap_base_info.en_vap_state, pst_tx_event->us_frame_len);
            ul_ret = dmac_tx_force(pst_dmac_vap, pst_netbuf, pst_tx_event->us_frame_len, 0);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG2(0, OAM_SF_TX, "{dmac_tx_process_data_event::dmac_tx_force [roam] len[%d] fail[%lu]}", pst_tx_event->us_frame_len, ul_ret);
                dmac_tx_excp_free_netbuf(pst_netbuf);
            }
            return ul_ret;
        }
        else
        {
            pst_tx_ctl->bit_roam_data = OAL_FALSE;
        }
#endif //_PRE_WLAN_FEATURE_ROAM

        /* ???????????????????????? */
        if(OAL_TRUE == MAC_GET_CB_IS_VIPFRAME(pst_tx_ctl))
        {
            uc_data_type =  mac_get_data_type(pst_netbuf);
            if((MAC_DATA_DHCP == uc_data_type) ||
        #if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
              (MAC_DATA_ARP_REQ == uc_data_type) ||
              (MAC_DATA_ARP_RSP == uc_data_type) ||
        #endif
              (MAC_DATA_EAPOL == uc_data_type))
            {
                 OAM_WARNING_LOG3(pst_event_hdr->uc_vap_id, OAM_SF_WPA, "{dmac_tx_process_data_event::datatype==%u, qos==%u, usridx=%d}[0:dhcp 1:arp_req 2:arp_rsp 3:eapol]", uc_data_type, mac_get_cb_is_qosdata(pst_tx_ctl), MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));

                 /* ??????????????TID???????????????? */
                 dmac_tx_pause_info(pst_hal_device, pst_dmac_vap, pst_netbuf);

#ifdef _PRE_WLAN_FEATURE_VOWIFI
                dmac_vowifi_update_arp_timestamp(pst_dmac_vap, MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
#endif


            }
        }

        ul_ret = dmac_tx_process_data(pst_hal_device, pst_dmac_vap, pst_netbuf);
        if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
        {
            OAM_INFO_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_TX, "{dmac_tx_process_data_event::dmac_tx_process_data fail[%u].}", ul_ret);
            dmac_tx_excp_free_netbuf(pst_netbuf);
        }

        pst_netbuf = pst_netbuf_next;
    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_DMAC_END);

    return OAL_SUCC;
}
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif

#ifdef _PRE_WLAN_FEATURE_DBAC

oal_bool_enum_uint8 dmac_is_need_enqueue_fake_for_dbac(mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_netbuf)
{
#ifdef _PRE_WLAN_FEATURE_P2P
    mac_ieee80211_frame_stru       *pst_mac_header;
    oal_uint8                      *puc_sa = OAL_PTR_NULL;
#endif
    if (pst_mac_device->pst_device_stru->uc_current_chan_number != pst_mac_vap->st_channel.uc_chan_number)
    {
        return OAL_TRUE;
    }
    else
    {
        if (dmac_alg_dbac_is_pause(pst_mac_device))
        {
#ifdef _PRE_WLAN_FEATURE_P2P
            if (MAC_SCAN_FUNC_P2P_LISTEN == pst_mac_device->st_scan_params.uc_scan_func)
            {
                pst_mac_header  = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_netbuf);
                mac_rx_get_sa(pst_mac_header, &puc_sa);

                if (!oal_compare_mac_addr(puc_sa, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID))
                {
                    return OAL_FALSE;
                }
            }
#endif
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}
#endif

oal_void  dmac_proc_restore_tx_queue(hal_to_dmac_device_stru     *pst_hal_device,
                                                hal_tx_dscr_queue_header_stru *pst_fake_queue)
{
    hal_tx_dscr_stru                *pst_tx_dscr;
    hal_tx_dscr_stru                *pst_tx_dscr_next;
    oal_uint8                        uc_q_idx;
    oal_dlist_head_stru             *pst_dscr_entry;
    oal_bool_enum_uint8              en_mpdu_skip;
    oal_uint32                       ul_tx_q_full;
    oal_uint32                       ul_put_cnt = 0;
    hal_tx_dscr_stru                *pst_tail_dscr;


    for (uc_q_idx = 0; uc_q_idx < HAL_TX_QUEUE_BUTT; uc_q_idx++)
    {
        /* ?????????????????????????????????? */
        oal_dlist_join_head(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header,
                            &pst_fake_queue[uc_q_idx].st_header);
        pst_hal_device->ast_tx_dscr_queue[uc_q_idx].uc_ppdu_cnt = pst_fake_queue[uc_q_idx].uc_ppdu_cnt;
        oal_dlist_init_head(&pst_fake_queue[uc_q_idx].st_header);
        pst_fake_queue[uc_q_idx].uc_ppdu_cnt = 0;

        ul_put_cnt      = 0;
        ul_tx_q_full    = 0;
        pst_tail_dscr   = OAL_PTR_NULL;

        en_mpdu_skip = OAL_FALSE;

        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header)
        {
            pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            if (OAL_FALSE == en_mpdu_skip)
            {
                hal_get_tx_q_status(pst_hal_device, &ul_tx_q_full, uc_q_idx);

                if (ul_tx_q_full)
                {
                    if (OAL_PTR_NULL == pst_tail_dscr)
                    {
                        /* ????????????????????????fifo???? */
                        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_restore_tx_queue::hw queue full while no frame put.}");
                        return;
                    }
                    hal_tx_ctrl_dscr_link(pst_hal_device, pst_tail_dscr, pst_tx_dscr);
                }
                else
                {
                    hal_tx_put_dscr(pst_hal_device, uc_q_idx, pst_tx_dscr);
                    ul_put_cnt++;
                }
            }

            hal_get_tx_dscr_next(pst_hal_device, pst_tx_dscr, &pst_tx_dscr_next);

            if (pst_tx_dscr_next != OAL_PTR_NULL)
            {
                en_mpdu_skip = OAL_TRUE;
            }
            else
            {
                en_mpdu_skip = OAL_FALSE;
                pst_tail_dscr = pst_tx_dscr;
            }

            if (ul_put_cnt > DMAC_MAX_FIFO_PUT)
            {
                OAM_ERROR_LOG3(0, OAM_SF_TX, "{dmac_tx_restore_tx_queue::TX FIFO OVERRUN invoked, ppdu=%d Q=%d cnt=%d}",
                                pst_hal_device->ast_tx_dscr_queue[uc_q_idx].uc_ppdu_cnt, uc_q_idx, ul_put_cnt);
            }
        }
    }

}


oal_uint32  dmac_tx_restore_tx_queue(hal_to_dmac_device_stru *pst_hal_device, oal_uint8 uc_fake_q_id)
{
    hal_tx_dscr_queue_header_stru   *pst_fake_queue;

    if (OAL_UNLIKELY(uc_fake_q_id >= HAL_TX_FAKE_QUEUE_NUM))
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_restore_tx_queue::uc_fake_q_id is invalid, %d.}", uc_fake_q_id);
        return OAL_FAIL;
    }

    pst_fake_queue = pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q_id];
    dmac_proc_restore_tx_queue(pst_hal_device, pst_fake_queue);

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_tx_mgmt_hw_send(hal_to_dmac_device_stru *pst_hal_device, dmac_vap_stru *pst_dmac_vap, hal_tx_dscr_stru *pst_mgmt_dscr, mac_tx_ctl_stru *pst_tx_ctl)
{
    hal_tx_dscr_stru    *pst_last_dscr;
    oal_dlist_head_stru *pst_last_dscr_entry;
    oal_uint32           ul_tx_q_status;
    hal_tx_queue_type_enum_uint8 uc_q_num;
    oal_uint8           uc_ac;

    uc_ac = mac_get_cb_ac(pst_tx_ctl);
    uc_q_num = HAL_AC_TO_Q_NUM(uc_ac);

    /*****************************************************************************
        Step 1. ??????????????????????????
    *****************************************************************************/
    oal_dlist_add_tail(&pst_mgmt_dscr->st_entry, &(pst_hal_device->ast_tx_dscr_queue[uc_q_num].st_header));
    pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt++;

    /*****************************************************************************
        Step 2. ????????????????????????????
        1) ????????????????FIFO??????????????????????????????????????????????
        2) ????????????????????????????????FIFO
    *****************************************************************************/
    /* ?????????????????????? */
    hal_get_tx_q_status(pst_hal_device, &ul_tx_q_status, uc_q_num);

#ifdef _PRE_DEBUG_MODE
    g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_mgnt_num++;
    OAM_INFO_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "dmac_tx_mgmt_hw_send:ul_tx_mgnt_num = %d", g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_mgnt_num);
#endif

    /* ????????????0??????????????????FIFO?? */
    if (0 != ul_tx_q_status)
    {
        OAL_MEM_TRACE(pst_mgmt_dscr, OAL_FALSE);
        /* ???????????????????????????????????? */
        pst_last_dscr_entry = pst_hal_device->ast_tx_dscr_queue[uc_q_num].st_header.pst_prev->pst_prev;
        pst_last_dscr       = OAL_DLIST_GET_ENTRY(pst_last_dscr_entry, hal_tx_dscr_stru, st_entry);

        hal_tx_ctrl_dscr_link(pst_hal_device, pst_last_dscr, pst_mgmt_dscr);
        OAM_WARNING_LOG3(0, OAM_SF_TX, "dmac_tx_mgmt_hw_send::uc_q_num = [%d], ul_tx_q_status = [%d], ppdu_cnt[%d]",
                         uc_q_num, ul_tx_q_status, pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt);
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        if ((pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt % 10) == 0)
        {
            hal_dft_print_rfhw_stat(pst_hal_device);
            hal_get_btcoex_statistic(OAL_FALSE);
        }

        if ((pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt % 30) == 0)
        {
            hal_dft_report_all_reg_state(pst_hal_device);
        }
#endif
    }
    else
    {

        OAL_MEM_TRACE(pst_mgmt_dscr, OAL_FALSE);
        /* ???????? */
        hal_tx_put_dscr(pst_hal_device, uc_q_num, pst_mgmt_dscr);
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_ROAM

oal_uint32  dmac_tx_force(dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf, oal_uint16 us_len, oal_uint8 uc_mgmt)
{
    hal_to_dmac_device_stru            *pst_hal_device;
    mac_tx_ctl_stru                    *pst_tx_ctl;
    hal_tx_dscr_stru                   *pst_tx_dscr;
    dmac_user_stru                     *pst_dmac_user;
    mac_ieee80211_frame_stru           *pst_mac_header;
    mac_device_stru                    *pst_mac_device;
    hal_tx_txop_alg_stru               *pst_txop_alg;
    hal_tx_txop_feature_stru            st_txop_feature     = {0};
    wlan_security_txop_params_stru      security_info = {0};
    hal_tx_ppdu_feature_stru            st_ppdu_feature;
    hal_tx_mpdu_stru                    st_mpdu;
    wlan_channel_band_enum_uint8        en_band;
    oal_uint32                          ul_ret;
    oal_uint8                           uc_ac;
    oal_uint8                           uc_vapid;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    oal_uint32                          ul_bank3_bcn_period;
#endif
#ifdef _PRE_WLAN_FEATURE_DBAC
    oal_uint8                           uc_q_id = 0;
#endif //_PRE_WLAN_FEATURE_DBAC
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    bt_status_stru                     *pst_bt_status;
#endif //_PRE_WLAN_FEATURE_BTCOEX
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_netbuf)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* netbuf???? */
    OAL_MEM_NETBUF_TRACE(pst_netbuf, OAL_TRUE);

    uc_vapid = pst_dmac_vap->st_vap_base_info.uc_vap_id;

    pst_hal_device = pst_dmac_vap->pst_hal_device;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }
    if (OAL_TRUE == MAC_DEV_IS_RESET_IN_PROGRESS(pst_mac_device))
    {
        return OAL_ERR_CODE_RESET_INPROGRESS;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    uc_ac      = mac_get_cb_ac(pst_tx_ctl);

    /* ??????????????????netbuf?????? */
    if (MAC_GET_CB_NETBUF_NUM(pst_tx_ctl) != 1)
    {
       OAM_WARNING_LOG1(uc_vapid, OAM_SF_TX, "{dmac_tx_force::netbuf num[%d] != 1}", MAC_GET_CB_NETBUF_NUM(pst_tx_ctl));
       return OAL_FAIL;
    }

    /* ???????????????????? */
    if (mac_is_protectedframe((oal_uint8 *)OAL_NETBUF_HEADER(pst_netbuf)) == 1)
    {
       OAM_WARNING_LOG0(uc_vapid, OAM_SF_TX, "{dmac_tx_force::protectedframe detected}");
       return OAL_FAIL;
    }

    /* ?????????????????????????? */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_WARNING_LOG0(uc_vapid, OAM_SF_TX, "{dmac_tx_force::pst_dmac_user null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_tx_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_TX_DSCR_1, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAL_TRUE);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_dscr))
    {
        OAM_ERROR_LOG0(uc_vapid, OAM_SF_TX, "{dmac_tx_force::pst_tx_dscr alloc failed.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);

    OAL_MEMZERO(pst_tx_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1);

    pst_mac_header = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_netbuf);

    pst_mac_header->bit_frag_num = 0;

    if (uc_mgmt)
    {
        pst_mac_header->bit_seq_num  = 0;
        dmac_get_mgmt_mpdu_param(pst_netbuf, pst_tx_ctl, us_len, &st_mpdu);
    }
    else
    {
        dmac_tx_seqnum_set(pst_dmac_user, pst_tx_ctl);

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
        /* ????????PN ?? */
        dmac_tx_sw_set_ccmp_PN(pst_hal_device, pst_dmac_user, pst_tx_dscr, pst_netbuf);
#endif

        ul_ret = dmac_tx_get_mpdu_params(pst_netbuf, pst_tx_ctl, &st_mpdu);
        if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
        {
            OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_force::dmac_tx_get_mpdu_params failed[%d].", ul_ret);
            OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
            return OAL_FAIL;
        }
#ifdef _PRE_WLAN_FEATURE_BTCOEX

        /* ??BT??????????????????????????????????EAPOL??????occupied_period??????????????????EAPOL?????? */
        pst_bt_status = &(pst_hal_device->st_btcoex_btble_status.un_bt_status.st_bt_status);
        if ((mac_get_data_type(pst_netbuf) == MAC_DATA_EAPOL) && (pst_bt_status->bit_bt_on) && !(pst_bt_status->bit_bt_sco))
        {
            hal_set_btcoex_occupied_period(60000);//60ms
        }
#endif //_PRE_WLAN_FEATURE_BTCOEX
    }

    /* ???????????????????????????????? */
    en_band = pst_dmac_vap->st_vap_base_info.st_channel.en_band;
    if (OAL_TRUE == MAC_GET_CB_IS_MCAST(pst_tx_ctl))
    {
        pst_txop_alg = &(pst_dmac_vap->ast_tx_mgmt_bmcast[en_band]);
    }
    else
    {
        pst_txop_alg = &(pst_dmac_vap->ast_tx_mgmt_ucast[en_band]);
    }

    if (0 == uc_mgmt)
    {
        pst_tx_ctl->bit_roam_data = OAL_TRUE;
    }
    else
    {
        pst_tx_ctl->bit_roam_data = OAL_FALSE;
    }

    /* ??????ppdu???????? */
    dmac_tx_init_ppdu_feature(pst_dmac_vap, OAL_PTR_NULL, 1, &st_ppdu_feature);

    /* ??????????txop */
    dmac_tx_set_txopps_param(pst_dmac_vap, pst_dmac_user, &st_txop_feature, MAC_GET_CB_IS_MCAST(pst_tx_ctl));

    st_txop_feature.pst_security = &security_info;
    st_txop_feature.pst_security->en_cipher_key_type       = WLAN_KEY_TYPE_TX_GTK;
    st_txop_feature.pst_security->en_cipher_protocol_type  = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    st_txop_feature.pst_security->uc_cipher_key_id         = 0;

    /* ?????????????????????? */
    hal_tx_fill_basic_ctrl_dscr(pst_hal_device, pst_tx_dscr, &st_mpdu);
    pst_tx_dscr->us_original_mpdu_len = us_len;
    pst_tx_dscr->bit_is_retried       = OAL_FALSE;
    pst_tx_dscr->bit_is_first         = 0;
    pst_tx_dscr->uc_q_num             = HAL_AC_TO_Q_NUM(uc_ac);

/* ??1102????tsf_restore???? */
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    if ((WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode))
    {
    	/* ??????????????staut bank3 ??beacon??????0 ??????ap??tsf??????sta */
        hal_vap_get_beacon_period(pst_dmac_vap->pst_hal_vap, &ul_bank3_bcn_period);
        if (0 == ul_bank3_bcn_period)
        {
            OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,"tx force staut bank3 bcn period 0 ap sync tsf to sta");
            hal_sta_tsf_restore(pst_dmac_vap->pst_hal_vap);
        }
    }
#endif

    dmac_tx_get_timestamp(pst_dmac_vap->pst_hal_vap, &st_ppdu_feature.us_tsf);
    /* ?????????????????????????????????????????? */
    hal_tx_non_ucast_data_set_dscr(pst_hal_device, pst_tx_dscr, &st_txop_feature, pst_txop_alg, &st_ppdu_feature);
    hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_tx_dscr);

    dmac_tx_dump_mgmt(pst_hal_device, pst_netbuf, us_len, st_mpdu.st_mpdu_mac_hdr.uc_mac_hdr_len, pst_tx_dscr);

#ifdef _PRE_WLAN_FEATURE_DBAC
    /* DBAC?????????????????????????????????????????????????????????????? */
    if (mac_is_dbac_running(pst_mac_device))
    {
        if (dmac_is_need_enqueue_fake_for_dbac(pst_mac_device, &(pst_dmac_vap->st_vap_base_info), pst_netbuf))
        {
            dmac_alg_get_dbac_fakeq_id(&pst_dmac_vap->st_vap_base_info, &uc_q_id);

            oal_dlist_add_tail(&pst_tx_dscr->st_entry, &(pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_tx_dscr->uc_q_num].st_header));
            pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_tx_dscr->uc_q_num].uc_ppdu_cnt++;

            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_DBAC, "dmac_tx_force: buffered by dbac. fake q id:%d", uc_q_id);

            return OAL_SUCC;
        }
    }
    else if (pst_hal_device->uc_current_chan_number != pst_dmac_vap->st_vap_base_info.st_channel.uc_chan_number)
    {
        OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
        return OAL_FAIL;
    }
#endif
    /* ?????????????????????????? */
    dmac_tx_mgmt_hw_send(pst_hal_device, pst_dmac_vap, pst_tx_dscr, pst_tx_ctl);

    return OAL_SUCC;
}

#endif //_PRE_WLAN_FEATURE_ROAM

#define PROBE_REQ_THREHOD_MS 100


OAL_STATIC oal_bool_enum_uint8 dmac_tx_mgmt_buffer_proc(
                mac_device_stru     *pst_mac_device,
                dmac_vap_stru       *pst_dmac_vap,
                hal_tx_dscr_stru    *pst_mgmt_dscr,
                oal_netbuf_stru     *pst_netbuf_mgmt)
{
    hal_to_dmac_device_stru        *pst_hal_device;
    oal_uint8                       uc_q_id = 0;
    oal_uint8                       uc_mgmt_type;
    oal_uint8                       uc_mgmt_subtype;
    mac_ieee80211_frame_stru       *pst_mac_header;

    pst_hal_device = pst_dmac_vap->pst_hal_device;

#ifdef _PRE_WLAN_FEATURE_DBAC
    /* DBAC?????????????????????????????????????????????????????????????? */
    if (mac_is_dbac_running(pst_mac_device))
    {
        if (dmac_is_need_enqueue_fake_for_dbac(pst_mac_device, &(pst_dmac_vap->st_vap_base_info), pst_netbuf_mgmt))
        {
            dmac_alg_get_dbac_fakeq_id(&pst_dmac_vap->st_vap_base_info, &uc_q_id);

            oal_dlist_add_tail(&pst_mgmt_dscr->st_entry, &(pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_mgmt_dscr->uc_q_num].st_header));
            pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_mgmt_dscr->uc_q_num].uc_ppdu_cnt++;

            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_DBAC, "dmac_tx_mgmt: buffered by dbac. fake q id:%d", uc_q_id);

            return OAL_TRUE;
        }

        return OAL_FALSE;
    }
#endif

    pst_mac_header  = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_netbuf_mgmt);
    uc_mgmt_type    = mac_frame_get_type_value((oal_uint8 *)pst_mac_header);
    uc_mgmt_subtype = mac_frame_get_subtype_value((oal_uint8 *)pst_mac_header);

    /* ???????????????????? */
    if (MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state &&
        MAC_SCAN_FUNC_P2P_LISTEN != pst_mac_device->st_scan_params.uc_scan_func &&
        WLAN_SCAN_MODE_FOREGROUND != pst_mac_device->st_scan_params.en_scan_mode &&
        WLAN_SCAN_MODE_BACKGROUND_CCA != pst_mac_device->st_scan_params.en_scan_mode &&
        WLAN_SCAN_MODE_BACKGROUND_PNO != pst_mac_device->st_scan_params.en_scan_mode)
    {
        /*????????????,??????????????????,????????????????????*/
        if (MAC_VAP_STATE_UP != pst_dmac_vap->st_vap_base_info.en_vap_state)
        {
            uc_q_id = HAL_TX_FAKE_QUEUE_BGSCAN_ID;

            oal_dlist_add_tail(&pst_mgmt_dscr->st_entry,
                               &(pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_mgmt_dscr->uc_q_num].st_header));

            pst_hal_device->ast_tx_dscr_queue_fake[uc_q_id][pst_mgmt_dscr->uc_q_num].uc_ppdu_cnt++;

            OAM_WARNING_LOG3(0, OAM_SF_DBAC, "dmac_tx_mgmt: buffered by scan. scan mode:%d, type:%d, subtype:%d",
                             pst_mac_device->st_scan_params.en_scan_mode,
                             uc_mgmt_type,
                             uc_mgmt_subtype);

            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}


oal_uint32  dmac_tx_mgmt(dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_netbuf_mgmt, oal_uint16 us_len)
{
    hal_tx_mpdu_stru                st_mpdu;
    mac_tx_ctl_stru                *pst_tx_ctl;
    hal_tx_dscr_stru               *pst_mgmt_dscr;
    hal_tx_txop_feature_stru        st_txop_feature     = {0};
    hal_tx_txop_alg_stru            st_txop_alg;
#ifdef _PRE_WLAN_CHIP_TEST
    hal_tx_txop_alg_stru           *pst_txop_alg        = OAL_PTR_NULL;
#endif
    hal_tx_ppdu_feature_stru        st_ppdu_feature;
    hal_to_dmac_device_stru        *pst_hal_device;
    wlan_security_txop_params_stru  st_security         = {0};
    dmac_user_stru                 *pst_dmac_user;
    oal_bool_enum_uint8             en_is_pkt_need_buff;  /* mpdu???????????????????????? */
    mac_ieee80211_frame_stru       *pst_mac_header;
    mac_device_stru                *pst_mac_device;
    mac_user_stru                  *pst_multi_user;
    wlan_channel_band_enum_uint8    en_band;
    oal_uint8                       uc_ac;
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    oal_uint32                      ul_relt;
#endif
    oal_uint8                       uc_mgmt_subtype;
    oal_uint8                       uc_mgmt_type;
    oal_bool_enum_uint8             en_need_buffer = OAL_FALSE;
    oal_uint8                       uc_subtype;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    oal_uint32                      ul_bank3_bcn_period;
#endif

#if defined(_PRE_WLAN_FEATURE_DBAC) && defined(_PRE_PRODUCT_ID_HI110X_DEV)
    mac_vap_stru                   *pst_mac_vap1;           /* ????UP??1??VAP */
    mac_vap_stru                   *pst_mac_vap2;           /* ????UP??1??VAP */
    mac_vap_stru                   *pst_current_chan_vap;   /* ????????????VAP */
    oal_uint8                       uc_fake_q_id = 0;
    mac_fcs_mgr_stru               *pst_fcs_mgr;
    mac_fcs_cfg_stru               *pst_fcs_cfg;
#endif

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    oal_uint16                     us_cache_frame_len = 0;  /* ?????????????? */
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
    oal_uint16                      us_user_idx;
#endif

    OAL_MEM_NETBUF_TRACE(pst_netbuf_mgmt, OAL_TRUE);

    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_netbuf_mgmt)
    {
        OAM_ERROR_LOG2(0, OAM_SF_TX, "{dmac_tx_mgmt::param null, pst_dmac_vap=%d, pst_netbuf_mgmt=%d.}",
                       pst_dmac_vap, pst_netbuf_mgmt);

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hal_device = pst_dmac_vap->pst_hal_device;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_mgmt::pst_hal_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if((OAL_PTR_NULL == pst_mac_device) || (OAL_TRUE == MAC_DEV_IS_RESET_IN_PROGRESS(pst_mac_device)))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_mgmt::MAC_DEV_IS_RESET_IN_PROGRESS.}");

        return OAL_ERR_CODE_RESET_INPROGRESS;
    }

    pst_mac_header = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_netbuf_mgmt);

    if ((WLAN_CONTROL != pst_mac_header->st_frame_control.bit_type) ||
        (WLAN_BLOCKACK_REQ != pst_mac_header->st_frame_control.bit_sub_type))
    {
        pst_mac_header->bit_frag_num = 0;
        pst_mac_header->bit_seq_num  = 0;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf_mgmt);

    dmac_get_mgmt_mpdu_param(pst_netbuf_mgmt, pst_tx_ctl, us_len, &st_mpdu);

    /*
       ????????????????????????????????????????????????????????????????????????,
       ????????????????????????????????????????????
    */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
    if (OAL_PTR_NULL != pst_dmac_user)
    {
     #ifdef _PRE_WLAN_FEATURE_UAPSD
        if(mac_vap_get_uapsd_en(&pst_dmac_vap->st_vap_base_info)
            &&(OAL_FALSE == MAC_GET_CB_IS_MCAST(pst_tx_ctl))
            &&(OAL_FALSE == MAC_GET_CB_IS_FROM_PS_QUEUE(pst_tx_ctl))
            &&(pst_dmac_user->uc_uapsd_flag & MAC_USR_UAPSD_TRIG))
        {
            mac_set_cb_is_need_pause_tid(pst_tx_ctl, OAL_FALSE);
            return dmac_uapsd_tx_enqueue(pst_dmac_vap, pst_dmac_user, pst_netbuf_mgmt);

        }
     #endif

       en_is_pkt_need_buff = dmac_psm_pkt_need_buff(pst_dmac_vap, pst_dmac_user, pst_netbuf_mgmt);
       if (OAL_TRUE == en_is_pkt_need_buff)
       {
           oal_set_netbuf_prev(pst_netbuf_mgmt, OAL_PTR_NULL);
           oal_set_netbuf_next(pst_netbuf_mgmt, OAL_PTR_NULL);
           mac_set_cb_is_need_pause_tid(pst_tx_ctl, OAL_FALSE);

           return dmac_psm_enqueue(pst_dmac_vap, pst_dmac_user, pst_netbuf_mgmt);
       }

       /* ????group id ?? partial aid */
       dmac_tx_set_txopps_param(pst_dmac_vap, pst_dmac_user, &st_txop_feature, pst_tx_ctl->en_ismcast);
    }

    en_band = pst_dmac_vap->st_vap_base_info.st_channel.en_band;

    if (OAL_TRUE == MAC_GET_CB_IS_MCAST(pst_tx_ctl))
    {
        /* ???????????????????? ???????? */
#ifdef _PRE_WLAN_CHIP_TEST
        pst_txop_alg = &(pst_dmac_vap->ast_tx_mgmt_bmcast[en_band]);
#endif
        oal_memcopy(&st_txop_alg, &(pst_dmac_vap->ast_tx_mgmt_bmcast[en_band]), OAL_SIZEOF(hal_tx_txop_alg_stru));
    }
    else
    {
        /* ???????? ?????? ???????? */
#ifdef _PRE_WLAN_CHIP_TEST
        pst_txop_alg = &(pst_dmac_vap->ast_tx_mgmt_ucast[en_band]);
#endif
        oal_memcopy(&st_txop_alg, &(pst_dmac_vap->ast_tx_mgmt_ucast[en_band]), OAL_SIZEOF(hal_tx_txop_alg_stru));

    }

    /* STA????????????????????????????????power_mgmt?? */
#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        /* tx cb????user idx???? + ??????????ap???? */
        if (OAL_SUCC == mac_vap_find_user_by_macaddr(&(pst_dmac_vap->st_vap_base_info), pst_mac_header->auc_address1, &us_user_idx))
        {
            /* pst_dmac_user????tx cb????user idx????,????????????,??????????????????????????,??????????????????,??????????????tx cb user idx???? */
            if (pst_dmac_user != OAL_PTR_NULL)
            {
                /* ????????null????pm??,???????????????????? */
                if ((WLAN_NULL_FRAME !=  mac_get_cb_frame_hdr(pst_tx_ctl)->st_frame_control.bit_sub_type)
                    &&(WLAN_QOS_NULL_FRAME != mac_get_cb_frame_hdr(pst_tx_ctl)->st_frame_control.bit_sub_type))
                {
                    dmac_psm_tx_set_power_mgmt_bit(pst_dmac_vap, pst_tx_ctl);
                }
            }
            else
            {
                OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_mgmt:staut send mgmt type[%d][%d] to [%d]user,but tx cb user idx[%d] invaild!!!}",
                        pst_mac_header->st_frame_control.bit_type,pst_mac_header->st_frame_control.bit_sub_type,us_user_idx,MAC_GET_CB_TX_USER_IDX(pst_tx_ctl));
            }
        }
    }
#endif


    /* ??????ppdu???????? */
    dmac_tx_init_ppdu_feature(pst_dmac_vap, pst_dmac_user, 1, &st_ppdu_feature);

    /* ??????????????????????????TX??????ra lut index??????????????????(8, index 0~7) ??????AP????????????????probe req*/
    if (MAX_TX_USER_IDX == MAC_GET_CB_TX_USER_IDX(pst_tx_ctl))
    {
        st_ppdu_feature.st_ppdu_addr_index.uc_ra_lut_index = (oal_uint8)g_us_wlan_assoc_user_max_num;
    }

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_mgmt_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_TX_DSCR_1, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAL_TRUE);
#else
    pst_mgmt_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_SHARED_DSCR, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAL_TRUE);
#endif
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mgmt_dscr))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_mgmt::pst_mgmt_dscr null.}");
#ifdef _PRE_WLAN_DFT_STAT
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

        dmac_dft_print_mac_phy_rf(pst_mac_device);
#endif
#endif
        return OAL_FAIL;
    }

    OAL_MEM_TRACE(pst_mgmt_dscr, OAL_FALSE);

    OAL_MEMZERO(pst_mgmt_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1);

    /* ????MPDU???????? */
    pst_mgmt_dscr->us_original_mpdu_len = us_len;
    uc_ac = mac_get_cb_ac(pst_tx_ctl);
    pst_mgmt_dscr->uc_q_num = HAL_AC_TO_Q_NUM(uc_ac);

    /* ????????????????*/
    st_security.en_cipher_key_type      = WLAN_KEY_TYPE_TX_GTK;
    st_security.en_cipher_protocol_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;

    /* ????????????????????1??,????????????????????????*/
    /* ????????????2????????*/
    /* 1??WEP????????????????????????,????????????????????????*/
    /* 2??WPA2????????????????11W????????11W????????????????*/
    if (mac_is_protectedframe((oal_uint8 *)OAL_NETBUF_HEADER(pst_netbuf_mgmt)) == 1)
    {
        pst_multi_user = mac_res_get_mac_user(pst_dmac_vap->st_vap_base_info.us_multi_user_idx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_multi_user))
        {
            OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_mgmt::pst_multi_user[%d] is NULL!}", pst_dmac_vap->st_vap_base_info.us_multi_user_idx);
            OAL_MEM_FREE(pst_mgmt_dscr, OAL_TRUE);
            return OAL_ERR_CODE_PTR_NULL;
        }
        //st_security.en_cipher_protocol_type = pst_multi_user->en_cipher_type;
        st_security.en_cipher_protocol_type = pst_multi_user->st_user_tx_info.st_security.en_cipher_protocol_type;
        st_security.en_cipher_key_type      = pst_multi_user->st_user_tx_info.st_security.en_cipher_key_type;

        if (OAL_TRUE == mac_is_wep_enabled(&pst_dmac_vap->st_vap_base_info))

        {
            /* TBD?????????? */
            st_security.uc_cipher_key_id = mac_vap_get_default_key_id(&pst_dmac_vap->st_vap_base_info);
        }
    }

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

    /* ????PMF???????????????? */
    dmac_11w_set_protectframe(pst_dmac_vap, &st_security, pst_netbuf_mgmt);

    if ((OAL_TRUE == ETHER_IS_MULTICAST(pst_mac_header->auc_address1)) &&
        (0 == mac_is_protectedframe((oal_uint8 *)OAL_NETBUF_HEADER(pst_netbuf_mgmt))))
    {
        /* ?????????????????? */
        ul_relt = dmac_bip_crypto(pst_dmac_vap, pst_netbuf_mgmt, &st_security, &us_len);
        if(OAL_SUCC != ul_relt)
        {
            OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                           "{dmac_tx_mgmt::dmac_bip_crypto failed[%d].}", ul_relt);
            OAL_MEM_FREE(pst_mgmt_dscr, OAL_TRUE);
            return OAL_ERR_CODE_PMF_BIP_CRIPTO_FAIL;
        }
        /* ???????????????? */
        pst_mgmt_dscr->us_original_mpdu_len    = us_len;
        st_mpdu.ast_msdu_addr[0].us_msdu0_len  = us_len - st_mpdu.st_mpdu_mac_hdr.uc_mac_hdr_len;
     }
#endif /* #if(_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)  */
    st_txop_feature.pst_security    = &st_security;

    /* ?????????????????? */
    hal_tx_fill_basic_ctrl_dscr(pst_hal_device, pst_mgmt_dscr, &st_mpdu);

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    /* ??1102????tsf_restore???? */
    if ((WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode))
    {
    	/* ??????????????staut bank3 ??beacon??????0 ??????ap??tsf??????sta */
        hal_vap_get_beacon_period(pst_dmac_vap->pst_hal_vap, &ul_bank3_bcn_period);
        if (0 == ul_bank3_bcn_period)
        {
            OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,"tx mgmt staut bank3 bcn period 0 ap sync tsf to sta");
            hal_sta_tsf_restore(pst_dmac_vap->pst_hal_vap);
        }
    }

    if ((WLAN_MANAGEMENT == pst_mac_header->st_frame_control.bit_type) &&
        (WLAN_PROBE_RSP == pst_mac_header->st_frame_control.bit_sub_type))
    {
        st_txop_alg.ast_per_rate[0].rate_bit_stru.bit_tx_count = 3;
        st_txop_alg.ast_per_rate[1].rate_bit_stru.bit_tx_count = 0;
        st_txop_alg.ast_per_rate[2].rate_bit_stru.bit_tx_count = 0;
        st_txop_alg.ast_per_rate[3].rate_bit_stru.bit_tx_count = 0;
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        if (pst_dmac_vap->pst_hal_device->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_sco)
        {
            st_txop_alg.ast_per_rate[0].rate_bit_stru.bit_tx_count = 1;
        }
#endif
    }
#endif

    dmac_tx_get_timestamp(pst_dmac_vap->pst_hal_vap, &st_ppdu_feature.us_tsf);

    /* ?????????????????????????????????????????? */
    hal_tx_non_ucast_data_set_dscr(pst_hal_device, pst_mgmt_dscr, &st_txop_feature, &st_txop_alg, &st_ppdu_feature);
    hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_mgmt_dscr);

#ifdef _PRE_WLAN_CHIP_TEST
    DMAC_CHIP_TEST_CALL(dmac_test_dfx_set_tx_cnt(pst_txop_alg));
#endif


#ifdef _PRE_WLAN_DFT_STAT
    /* ?????????????????? */
    dmac_dft_mgmt_stat_incr(pst_mac_device, (oal_uint8 *)pst_mac_header, MAC_DEV_MGMT_STAT_TYPE_TX);
#endif

    uc_subtype     =  mac_get_frame_sub_type((oal_uint8 *)pst_mac_header);
    if ((WLAN_FC0_SUBTYPE_NODATA | WLAN_FC0_TYPE_DATA) != uc_subtype  && (WLAN_FC0_SUBTYPE_QOS_NULL | WLAN_FC0_TYPE_DATA) != uc_subtype)/*null data????????????*/
    {
        dmac_tx_dump_mgmt(pst_hal_device, pst_netbuf_mgmt, us_len, st_mpdu.st_mpdu_mac_hdr.uc_mac_hdr_len, pst_mgmt_dscr);
    }

    /*  ??????????netbuf??????DDR??????cache?????? */
#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    us_cache_frame_len = (oal_uint16)MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) + (oal_uint16)MAC_GET_CB_MPDU_LEN(pst_tx_ctl);
    oal_dma_map_single(NULL, OAL_NETBUF_DATA(pst_netbuf_mgmt), us_cache_frame_len, OAL_TO_DEVICE);
#endif

    /*****************************************************************************
        ????????????????????????????????????????????
    *****************************************************************************/
    uc_mgmt_type    = mac_frame_get_type_value((oal_uint8 *)pst_mac_header);
    uc_mgmt_subtype = mac_frame_get_subtype_value((oal_uint8 *)pst_mac_header);

    /* ???????????????????????? */
    if (WLAN_MANAGEMENT == uc_mgmt_type && WLAN_PROBE_REQ == uc_mgmt_subtype)
    {
        /* probe req???????????????????????? */
        /* do nothing */
        //OAM_INFO_LOG0(0, OAM_SF_SCAN, "dmac_tx_mgmt: tx probe req");
    }
    else
    {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        if (WLAN_MANAGEMENT == uc_mgmt_type && WLAN_DISASOC == uc_mgmt_subtype)
        {
#ifdef _PRE_WLAN_DFT_STAT
            dmac_dft_report_all_ota_state(&(pst_dmac_vap->st_vap_base_info));
#endif
        }
#endif

/* 1102 sta+p2p????????????????1151 DBAC?????? */
#if defined(_PRE_WLAN_FEATURE_DBAC) && defined(_PRE_PRODUCT_ID_HI110X_DEV)
        if ((WLAN_MANAGEMENT == uc_mgmt_type && WLAN_DISASOC == uc_mgmt_subtype)
             && mac_is_dbac_running(pst_mac_device)
             && (MAC_VAP_STATE_PAUSE == pst_dmac_vap->st_vap_base_info.en_vap_state))
        {
            /* ????????VAP?????? */
            if (OAL_SUCC == mac_device_find_2up_vap(pst_mac_device, &pst_mac_vap1, &pst_mac_vap2))
            {
                pst_fcs_mgr = dmac_fcs_get_mgr_stru(pst_mac_device);
                pst_fcs_cfg = &(pst_mac_device->st_fcs_cfg);
                OAL_MEMZERO(pst_fcs_cfg, OAL_SIZEOF(mac_fcs_cfg_stru));
                dmac_vap_pause_tx(pst_mac_vap1);
                dmac_vap_pause_tx(pst_mac_vap2);
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
                pst_fcs_cfg->st_dst_chl = pst_dmac_vap->st_vap_base_info.st_channel;
                mac_fcs_prepare_one_packet_cfg(pst_current_chan_vap, &(pst_fcs_cfg->st_one_packet_cfg), 20);
                mac_fcs_start(pst_fcs_mgr, pst_fcs_cfg, 0, uc_fake_q_id);
                mac_fcs_release(pst_fcs_mgr);
            }
        }
        else
#endif

        {
            en_need_buffer = dmac_tx_mgmt_buffer_proc(pst_mac_device, pst_dmac_vap, pst_mgmt_dscr, pst_netbuf_mgmt);

            if (OAL_TRUE == en_need_buffer)
            {
                OAL_MEM_TRACE(pst_mgmt_dscr, OAL_FALSE);

                /* ????????????????OTA?????????????????? */
                oam_report_80211_frame(BROADCAST_MACADDR,
                                        (oal_uint8*)mac_get_cb_frame_hdr(pst_tx_ctl),
                                        MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl),
                                        (oal_uint8*)oal_netbuf_payload(pst_netbuf_mgmt),
                                        us_len,
                                        OAM_OTA_FRAME_DIRECTION_TYPE_TX);

                return OAL_SUCC;
            }
        }
    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if ((pst_mac_device->pst_device_stru->st_btcoex_btble_status.un_ble_status.st_ble_status.bit_ble_scan)
        && (!(pst_mac_device->pst_device_stru->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_sco)))
    {
        if ((WLAN_AUTH == uc_mgmt_subtype) || (WLAN_ASSOC_REQ == uc_mgmt_subtype))
        {
            hal_set_btcoex_occupied_period(50000);//50ms
        }
    }
#endif

#ifdef _PRE_WLAN_FEATURE_DFR
    if (OAL_TRUE == pst_hal_device->en_dfr_enable)
    {
        /* ???????????????????????????? */
        if (OAL_FALSE == pst_hal_device->st_dfr_tx_prot.st_tx_prot_timer.en_is_enabled)
        {
            FRW_TIMER_RESTART_TIMER(&pst_hal_device->st_dfr_tx_prot.st_tx_prot_timer, WLAN_TX_PROT_TIMEOUT, OAL_TRUE);
        }
    }
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
    {
        /* ??p2p dev?????????????????????????? */
        if ((IS_P2P_DEV(&pst_dmac_vap->st_vap_base_info)))
        {
            dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
        }
        /* cl || legacy sta?????????????? */
        else
        {
            dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
            dmac_pm_sta_post_event(pst_dmac_vap, STA_PWR_EVENT_TX_MGMT, 0, OAL_PTR_NULL);
        }
    }
#endif

    dmac_tx_mgmt_hw_send(pst_hal_device, pst_dmac_vap, pst_mgmt_dscr, pst_tx_ctl);

    /* pst_hal_device->uc_mpdu_num++; */ /* ?????????? */


    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_tx_rifs_process(hal_to_dmac_device_stru *pst_hal_dev,
                                      dmac_vap_stru *pst_dmac_vap,
                                      dmac_tid_stru *pst_tid_queue,
                                      hal_tx_ppdu_feature_stru *pst_ppdu_feature,
                                      oal_dlist_head_stru *pst_dscr_list)
{
    hal_tx_dscr_stru    *pst_first_dscr;
    hal_tx_dscr_stru    *pst_last_dscr;
    hal_tx_dscr_stru    *pst_bar_dscr;
    oal_netbuf_stru     *pst_bar_buf;
    dmac_ba_tx_stru     *pst_tx_ba_handle;
    mac_tx_ctl_stru     *pst_tx_ctl;
    oal_uint16           us_bar_len;
    hal_tx_mpdu_stru     st_mpdu;
    oal_dlist_head_stru *pst_dscr_entry;
    hal_tx_dscr_stru    *pst_dscr;
    mac_ieee80211_qos_frame_addr4_stru *pst_4addr_hdr;
    mac_ieee80211_qos_frame_stru       *pst_hdr;


    pst_tx_ba_handle = pst_tid_queue->pst_ba_tx_hdl;

    /* ????bar?????? */
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_bar_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_TX_DSCR_1, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAL_TRUE);
#else
    pst_bar_dscr = (hal_tx_dscr_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_SHARED_DSCR, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAL_TRUE);
#endif
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_bar_dscr))
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_rifs_process::pst_bar_dscr null.}");

        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    OAL_MEM_TRACE(pst_bar_dscr, OAL_FALSE);
    OAL_MEMZERO(pst_bar_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1);

    /* ????bar???? */
    pst_bar_buf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, WLAN_SHORT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_MID);
    OAL_MEM_NETBUF_TRACE(pst_bar_buf, OAL_FALSE);
    if (OAL_PTR_NULL == pst_bar_buf)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_rifs_process::pst_bar_buf null.}");

        OAL_MEM_FREE(pst_bar_dscr, OAL_TRUE);
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAL_MEM_NETBUF_TRACE(pst_bar_buf, OAL_TRUE);

    /* ??bar?? */
    us_bar_len = dmac_ba_encap_blockack_req(pst_dmac_vap, pst_bar_buf, pst_tx_ba_handle, pst_tid_queue->uc_tid);
    if (0 == us_bar_len)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_rifs_process::us_bar_len=0.}");
        OAL_MEM_FREE(pst_bar_dscr, OAL_TRUE);
        oal_netbuf_free(pst_bar_buf);

        return OAL_FAIL;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_bar_buf);
    OAL_MEMZERO(pst_tx_ctl, OAL_NETBUF_CB_SIZE());

    MAC_GET_CB_EVENT_TYPE(pst_tx_ctl) = FRW_EVENT_TYPE_WLAN_CTX;
    MAC_GET_CB_IS_MCAST(pst_tx_ctl)   = OAL_FALSE;
    mac_set_cb_is_bar(pst_tx_ctl, OAL_TRUE);
    MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_CTL_HEADER_LEN;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl)            = us_bar_len;


    oal_set_netbuf_prev(pst_bar_buf, OAL_PTR_NULL);
    oal_set_netbuf_next(pst_bar_buf, OAL_PTR_NULL);

    dmac_get_mgmt_mpdu_param(pst_bar_buf, pst_tx_ctl, us_bar_len, &st_mpdu);

    /* ????bar?????????????? */
    hal_tx_fill_basic_ctrl_dscr(pst_hal_dev, pst_bar_dscr, &st_mpdu);

    pst_last_dscr = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_list->pst_prev, hal_tx_dscr_stru, st_entry);

    hal_tx_ctrl_dscr_link(pst_hal_dev, pst_last_dscr, pst_bar_dscr);
    hal_tx_ctrl_dscr_unlink(pst_hal_dev, pst_bar_dscr);

    /* ??bar????rifs burst?????? */
    oal_dlist_add_tail(&pst_bar_dscr->st_entry, pst_dscr_list);

    OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, pst_dscr_list)
    {
        pst_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

        pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_dscr->pst_skb_start_addr);

        if (OAL_TRUE == mac_get_cb_is_use_4_addr(pst_tx_ctl))
        {
            pst_4addr_hdr = (mac_ieee80211_qos_frame_addr4_stru *)mac_get_cb_frame_hdr(pst_tx_ctl);
            pst_4addr_hdr->bit_qc_ack_polocy = WLAN_TX_NO_ACK;
        }
        else
        {
            pst_hdr = (mac_ieee80211_qos_frame_stru *)mac_get_cb_frame_hdr(pst_tx_ctl);
            pst_hdr->bit_qc_ack_polocy = WLAN_TX_NO_ACK;
        }
    }

    pst_ppdu_feature->uc_ampdu_enable       = OAL_FALSE;
    pst_ppdu_feature->uc_rifs_enable        = OAL_TRUE;
    pst_ppdu_feature->ul_ampdu_length       = 0;
    pst_ppdu_feature->us_min_mpdu_length    = 0;

    pst_ppdu_feature->uc_mpdu_num++;

    pst_first_dscr = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_list->pst_next, hal_tx_dscr_stru, st_entry);
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_first_dscr->pst_skb_start_addr);

    mac_set_cb_bar_dscr(pst_tx_ctl, pst_bar_dscr);
    return OAL_SUCC;
}

/*????????ampdu????????*/
#ifdef _PRE_PROFILING_MODE
oal_uint32 g_ul_tx_mpdu_num = 0;
oal_uint32 g_ul_tx_ampdu_num = 0;
#endif

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif


oal_uint32  dmac_tid_tx_queue_remove_ampdu(
                hal_to_dmac_device_stru   *pst_hal_device,
                dmac_vap_stru             *pst_dmac_vap,
                dmac_user_stru            *pst_user,
                dmac_tid_stru             *pst_tid_queue,
                oal_uint8                  uc_mpdu_num)
{
    mac_device_stru            *pst_dev;
    oal_uint8                   uc_mpdu_idx         = 0;
    oal_dlist_head_stru        *pst_dscr_entry      = OAL_PTR_NULL;
#ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    oal_dlist_head_stru        *pst_dscr_entry_next = OAL_PTR_NULL;
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
    hal_tx_dscr_stru           *pst_tx_dscr         = OAL_PTR_NULL;   /*??????????????????*/
    hal_tx_dscr_stru           *pst_tx_dscr_prev    = OAL_PTR_NULL;   /*????????????????*/
    hal_tx_txop_alg_stru       *pst_txop_alg        = OAL_PTR_NULL;
    mac_tx_ctl_stru            *pst_cb              = OAL_PTR_NULL;
    mac_tx_ctl_stru            *pst_cb_first        = OAL_PTR_NULL;
    oal_netbuf_stru            *pst_netbuf;
    hal_tx_ppdu_feature_stru    st_ppdu_feature;
    oal_uint16                  us_seq_num          = 0;
    dmac_ba_tx_stru            *pst_tx_ba_handle;
    oal_uint16                  us_roundup_len;
    oal_uint16                  us_baw_end;               /*??????????????seq num*/
    oal_uint16                  us_new_mpdu_max_num = 0;  /*????????????????????mpdu????*/
    oal_uint16                  us_pad_len          = 0;
    oal_uint16                  us_null_len         = 0;
    oal_uint32                  ul_ret;
    oal_uint16                  us_mpdu_len;
    oal_uint16                  us_ext_mpdu_len;  /*????MPDU??????????????*/
    oal_dlist_head_stru         st_dscr_head;
    oal_uint8                   uc_vap_id;
    oal_uint8                   uc_retry_mpdu_num   = 0;  /*amdpu??????mpdu??????*/
#ifdef _PRE_DEBUG_MODE
    oal_dlist_head_stru        *pst_dscr_tra_entry  = OAL_PTR_NULL;
    hal_tx_dscr_stru           *pst_trace_dscr      = OAL_PTR_NULL;
#endif
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    oal_uint32                  ul_have_short_packet = OAL_FALSE;
#endif
    oal_uint32                  ul_max_ampdu_length;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_DFT_STAT
    dmac_tid_stats_stru        *pst_tid_stats;

    pst_tid_stats = pst_tid_queue->pst_tid_stats;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tid_stats))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::tid_stats is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
#endif
    uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;

    pst_tx_ba_handle = pst_tid_queue->pst_ba_tx_hdl;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_ba_handle))
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::pst_tx_ba_handle null.}");

        return OAL_FAIL;
    }

    /* ????device?????????????????? */
    pst_dev = mac_res_get_dev(pst_user->st_user_base_info.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dev))
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::pst_dev null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((WLAN_VHT_MODE == pst_user->st_user_base_info.en_cur_protocol_mode)
      || (WLAN_VHT_ONLY_MODE == pst_user->st_user_base_info.en_cur_protocol_mode))
    {
        ul_max_ampdu_length = pst_tid_queue->st_ht_tx_hdl.ul_ampdu_max_size_vht;
    }
    else
    {
        ul_max_ampdu_length = pst_tid_queue->st_ht_tx_hdl.us_ampdu_max_size;
    }

    /* ??????ppdu feature???? */
    dmac_tx_init_ppdu_feature(pst_dmac_vap, pst_user, 0, &st_ppdu_feature);

    us_baw_end = DMAC_BA_SEQNO_ADD(pst_tx_ba_handle->us_baw_start, pst_tx_ba_handle->us_baw_size);
    us_baw_end = DMAC_BA_SEQNO_SUB(us_baw_end, 1);

    /*??????????????????mpdu??????????*/
    us_new_mpdu_max_num = DMAC_BA_SEQNO_SUB(us_baw_end, pst_tx_ba_handle->us_last_seq_num);

    us_ext_mpdu_len = WLAN_HDR_FCS_LENGTH + WLAN_DELIMETER_LENGTH;
    /* ?????????????????????????? */
    if (WLAN_80211_CIPHER_SUITE_CCMP == pst_user->st_user_base_info.st_key_info.en_cipher_type)
    {
        us_ext_mpdu_len += WLAN_CCMP_ENCRYP_LEN;
    }

    oal_dlist_init_head(&st_dscr_head);

#ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    pst_dscr_entry_next = pst_tid_queue->st_hdr.pst_next;
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_GET_INFO);
    while (uc_mpdu_idx != uc_mpdu_num)
    {
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        /* ????????:??????????????netbuf???? */
        if (OAL_FALSE == oal_dlist_is_empty(&pst_tid_queue->st_retry_q))
        {
            /* ???????????? */
            pst_dscr_entry = oal_dlist_delete_head(&pst_tid_queue->st_retry_q);
            pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            oal_dlist_add_tail(pst_dscr_entry, &st_dscr_head);
        }
        else if (OAL_FALSE == oal_netbuf_list_empty(&pst_tid_queue->st_buff_head))
        {
            /* netbuf???????? */
            pst_netbuf  = dmac_tx_dequeue_first_mpdu(&pst_tid_queue->st_buff_head);
            pst_tx_dscr = dmac_tx_dscr_alloc(pst_hal_device, pst_dmac_vap, pst_netbuf);
            if (OAL_PTR_NULL == pst_tx_dscr)
            {
                OAM_WARNING_LOG3(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::put the pkt back into tid netbuf Q, alloc tx dscr failed. retry = %d, mpdu = %d, uc_mpdu_num = %d}",
                                 pst_tid_queue->uc_retry_num, pst_tid_queue->us_mpdu_num, uc_mpdu_num);

                dmac_tx_queue_mpdu_head(pst_netbuf, &pst_tid_queue->st_buff_head);
                break;
            }
            oal_dlist_add_tail(&pst_tx_dscr->st_entry, &st_dscr_head);
        }
        else
        {
            /* ?????????????????? */
            break;
        }
#else
        if (pst_dscr_entry_next == (&pst_tid_queue->st_hdr))
        {
            //OAM_INFO_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::no more mpdus in tid queue.}\r\n");
            break;
        }

        pst_dscr_entry      = pst_dscr_entry_next;
        pst_dscr_entry_next = pst_dscr_entry->pst_next;

        pst_tx_dscr         = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        pst_tx_dscr->bit_is_first = 0;

        if (0 == pst_tid_queue->uc_retry_num) /*????????*/
        {
            /* ?????????????????????????? */
            if ((uc_mpdu_idx - uc_retry_mpdu_num) == us_new_mpdu_max_num)
            {
            #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
                OAM_WARNING_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::tx ba win full, pst_tx_dscr->bit_is_retried=%d, idx=%d, mpdu=%d, max=%d.}", pst_tx_dscr->bit_is_retried, uc_mpdu_idx, uc_retry_mpdu_num, us_new_mpdu_max_num);
                if (OAL_TRUE == pst_tx_dscr->bit_is_retried)
                {
                    /* ?????????????????????????????????????????? */
                    pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                    oal_dlist_add_head(pst_dscr_entry, &pst_tid_queue->st_retry_q);
                }
                else
                {
                    /* ????netbuf???? */
                    pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                    pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
                    pst_netbuf     = pst_tx_dscr->pst_skb_start_addr;

                    dmac_tx_queue_mpdu_head(pst_netbuf, &pst_tid_queue->st_buff_head);
                    OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
                }
            #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
                break;
            }
        }
        else /*??????*/
        {
            /* ????seq num */
            hal_tx_get_dscr_seq_num(pst_hal_device, pst_tx_dscr, &us_seq_num);
            /*OAM_INFO_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::seq num of a retried packet=%d.}",us_seq_num);*/

            /* ?????????????????????????? */
            if (!DMAC_BAW_WITHIN(pst_tx_ba_handle->us_baw_start, pst_tx_ba_handle->us_baw_size, us_seq_num))
            {
                OAM_WARNING_LOG3(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::a retry packet shall be dropped. baw_start=%d baw_size=%d sqe_num=%d.}",
                                 pst_tx_ba_handle->us_baw_start, pst_tx_ba_handle->us_baw_size, us_seq_num);
                pst_tid_queue->uc_retry_num--;
                pst_tid_queue->us_mpdu_num--;
                pst_dev->us_total_mpdu_num--;
                pst_dev->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id]--;
                pst_dev->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)]--;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
                DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_ampdu_cnt, 1);
                DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_total_cnt, 1);
#endif
                oal_dlist_delete_entry(&pst_tx_dscr->st_entry);
                dmac_free_tx_dscr(pst_tx_dscr);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)

        #ifdef _PRE_WLAN_DFT_STAT
                if (OAL_PTR_NULL != pst_tid_queue->pst_tid_stats)
                {
                    DMAC_TID_STATS_INCR(pst_tid_queue->pst_tid_stats->ul_tid_retry_dequeue_cnt, 1);
                }
        #endif
#endif
                continue;
            }

            pst_tid_queue->uc_retry_num--;
            uc_retry_mpdu_num++; /*??????????????????????????????????????????????????uc_retry_mpdu_num??????????uc_mpdu_idx*/
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    #ifdef _PRE_WLAN_DFT_STAT
            if (OAL_PTR_NULL != pst_tid_queue->pst_tid_stats)
            {
                DMAC_TID_STATS_INCR(pst_tid_queue->pst_tid_stats->ul_tid_retry_dequeue_cnt, 1);
            }
    #endif
#endif
        }

        us_mpdu_len = pst_tx_dscr->us_original_mpdu_len + us_ext_mpdu_len;
        /* ?????????????????????? */
        if ((st_ppdu_feature.ul_ampdu_length + us_mpdu_len + us_pad_len + us_null_len) > ul_max_ampdu_length)
        {
            /* ????????????retry_num??????????????????????????????break????????????????????????????*/
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
            if (OAL_TRUE == pst_tx_dscr->bit_is_retried)
            {
                pst_tid_queue->uc_retry_num++;

                /* ?????????????????????????????????????????? */
                pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                oal_dlist_add_head(pst_dscr_entry, &pst_tid_queue->st_retry_q);
            }
            else
            {
                /* ????netbuf???? */
                pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
                pst_netbuf     = pst_tx_dscr->pst_skb_start_addr;

                dmac_tx_queue_mpdu_head(pst_netbuf, &pst_tid_queue->st_buff_head);
                OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
            }
    #else
            if(OAL_TRUE == pst_tx_dscr->bit_is_retried)
            {
                pst_tid_queue->uc_retry_num++;
            }
    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

            break;
        }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
        /* ?????????????????????? */
        if (us_mpdu_len < BT_WLAN_COEX_SMALL_PKT_THRES)
        {
            ul_have_short_packet = OAL_TRUE;
        }

        /*
           BT?????????? ????????????????????????
        */
        if ((OAL_TRUE == ul_have_short_packet))
        {
    #if 0
            /*????BT??BLE??????????????????????????????????*/
            hal_get_btcoex_soc_bt_gpreg((oal_uint16 *)(&us_bt_status));
            hal_get_btcoex_soc_ble_gpreg((oal_uint8 *)(&uc_ble_status));

            /*????(us_bt_status.bit_bt_ps == OAL_FALSE)????????????????????????????????????????????*/
            if ((us_bt_status.bit_bt_on) || (uc_ble_status.bit_ble_on))
    #endif
            if (pst_hal_device->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_on)
            {
                if ((st_ppdu_feature.ul_ampdu_length + us_mpdu_len + us_pad_len + us_null_len) > BT_WLAN_COEX_SMALL_FIFO_THRES)
                {
            #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
                    if (OAL_TRUE == pst_tx_dscr->bit_is_retried)
                    {
                        pst_tid_queue->uc_retry_num++;

                        /* ?????????????????????????????????????????? */
                        pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                        oal_dlist_add_head(pst_dscr_entry, &pst_tid_queue->st_retry_q);
                    }
                    else
                    {
                        /* ????netbuf???? */
                        pst_dscr_entry = oal_dlist_delete_tail(&st_dscr_head);
                        pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
                        pst_netbuf     = pst_tx_dscr->pst_skb_start_addr;

                        dmac_tx_queue_mpdu_head(pst_netbuf, &pst_tid_queue->st_buff_head);
                        OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
                    }
            #else
                    if(OAL_TRUE == pst_tx_dscr->bit_is_retried)
                    {
                        pst_tid_queue->uc_retry_num++;
                    }
            #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
                    break;
                }

                if (pst_tx_dscr->us_original_mpdu_len < BT_WLAN_COEX_UNAVAIL_PAYLOAD_THRES)
                {
                    OAM_ERROR_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::ampdu length bug, length is %d}", pst_tx_dscr->us_original_mpdu_len);
                }
            }
        }
#endif

        pst_netbuf              = pst_tx_dscr->pst_skb_start_addr;
        pst_cb                  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

        if (0 != uc_mpdu_idx)
        {
           /*??????????link*/
           hal_tx_ctrl_dscr_link(pst_hal_device, pst_tx_dscr_prev, pst_tx_dscr);
        }
        else
        {
            MAC_GET_CB_TX_USER_IDX(pst_cb)  =(oal_uint8)pst_user->st_user_base_info.us_assoc_id;
            pst_cb_first = pst_cb;

            /***** ???????? *****/
            if (OAL_TRUE == MAC_GET_CB_IS_VIPFRAME(pst_cb_first))
            {
                MAC_GET_CB_IS_VIPFRAME(pst_cb_first)  = OAL_FALSE;
                MAC_GET_CB_IS_NEEDRETRY(pst_cb_first) = OAL_FALSE;
                MAC_GET_CB_RETRIED_NUM(pst_cb_first)      = 0;
            }

            MAC_GET_CB_MPDU_NUM(pst_cb) = uc_mpdu_num;
            if (0 == uc_mpdu_num)
            {
                OAM_ERROR_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::tid %d, uc_dscr_num is zero.}", mac_get_cb_tid(pst_cb));
            }

            /* ????TXOP???????????? */
            dmac_tx_get_txop_alg_params(pst_dmac_vap, pst_user, pst_cb, &pst_txop_alg);
            if (OAL_UNLIKELY(OAL_PTR_NULL == pst_txop_alg))
            {
                OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::pst_txop_alg null.}");
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
                pst_dmac_vap->st_vap_base_info.st_vap_stats.ul_tx_dropped_packets += MAC_GET_CB_NETBUF_NUM(pst_cb);
#endif
        #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
                /* ???????????????????????? */
                OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &st_dscr_head)
                {
                    pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
                    oal_dlist_delete_entry(pst_dscr_entry);
                    dmac_free_tx_dscr(pst_tx_dscr);
                }

                /* ???????????? */
                uc_mpdu_idx++;
                st_ppdu_feature.uc_mpdu_num += uc_mpdu_idx;
                pst_tid_queue->us_mpdu_num -= uc_mpdu_idx;
                pst_dev->us_total_mpdu_num -= uc_mpdu_idx;
                pst_dev->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id] -= uc_mpdu_idx;
                pst_dev->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)] -= uc_mpdu_idx;

                DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_ampdu_cnt, uc_mpdu_idx);
                DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_total_cnt, uc_mpdu_idx);
        #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

                return OAL_ERR_CODE_PTR_NULL;
            }

            if (WLAN_PROT_NO != pst_dmac_vap->st_vap_base_info.st_protection.en_protection_mode)
            {
                /*????????????????????????*/
                dmac_tx_update_protection_txop_alg(pst_dmac_vap, pst_txop_alg, OAL_FALSE, MAC_GET_CB_IS_MCAST(pst_cb));
            }

            /* ????????????????RTS */
            dmac_judge_forbid_rts(pst_user, pst_txop_alg);

            /* 2040???????????????????? */
            /* dmac_tx_update_bandwidth_mode(pst_dmac_vap, pst_user, pst_txop_alg); */

            OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_AGGR_PREPARE);
            OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_AGGR_PREPARE);

            /* ??????min mpdu len */
            st_ppdu_feature.us_min_mpdu_length = dmac_ba_calculate_min_mpdu_len(pst_user, pst_txop_alg);
            OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_CALCULATE_MINLEN);
            OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_CALCULATE_MINLEN);

            /*OAM_INFO_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::us_min_mpdu_len=%d.}", st_ppdu_feature.us_min_mpdu_length);*/
        }

        /* ??ampdu????????????????????pad??null data???????????? */
        st_ppdu_feature.ul_ampdu_length += us_pad_len + us_null_len + us_mpdu_len;

        /* ???? pad len??mpdu??????????4????????*/
        us_pad_len = (4 - (us_mpdu_len & 0x3)) & 0x3;
        us_roundup_len = us_mpdu_len + us_pad_len;

        us_null_len = OAL_MAX(us_roundup_len, st_ppdu_feature.us_min_mpdu_length) - us_roundup_len;

        /*OAM_INFO_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::ul_ampdu_length=%d us_roundup_len=%d us_pad_len=%d us_null_len=%d .}", \
                      st_ppdu_feature.ul_ampdu_length, us_roundup_len, us_pad_len, us_null_len);*/

        pst_tx_dscr_prev = pst_tx_dscr;
        uc_mpdu_idx++;

        /*??????????????seq num??????????*/
        if (OAL_FALSE == pst_tx_dscr->bit_is_retried)
        {
            /* ????seq num */
            dmac_tx_seqnum_set_ampdu(pst_user, pst_cb);

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
            /* ????????PN ?? */
            dmac_tx_sw_set_ccmp_PN(pst_hal_device, pst_user, pst_tx_dscr, pst_netbuf);
#endif
            /*????????????*/
            dmac_ba_addto_baw(pst_tx_ba_handle);
        }

        /* ???????????????????? */
        if ((FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
            && (OAL_TRUE != pst_tx_dscr->bit_is_retried))
        {
#ifdef _PRE_WLAN_DFT_STAT
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_pkts,
                                        MAC_GET_CB_NETBUF_NUM(pst_cb));
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_bytes,
                                        MAC_GET_CB_MPDU_BYTES(pst_cb));
#endif
            DMAC_USER_STATS_PKT_INCR(pst_user->st_query_stats.ul_tx_total, MAC_GET_CB_NETBUF_NUM(pst_cb));
        }

    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_LINK_MPDUS);

#ifdef _PRE_DEBUG_MODE
    if (OAL_PTR_NULL == pst_cb_first || OAL_PTR_NULL == pst_txop_alg)
    {
        if (pst_tid_queue->us_mpdu_num > 0)
        {
            OAM_WARNING_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::no element can be removed from the tid[%d].", pst_tid_queue->uc_tid);

            OAM_WARNING_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::bitmap is %x %x %x %x.",
                             pst_tx_ba_handle->aul_tx_buf_bitmap[0], pst_tx_ba_handle->aul_tx_buf_bitmap[1],
                             pst_tx_ba_handle->aul_tx_buf_bitmap[2], pst_tx_ba_handle->aul_tx_buf_bitmap[3]);

            OAM_WARNING_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::win start is %d win head is %d win tail is %d lsn is %d",
                             pst_tx_ba_handle->us_baw_start,
                             pst_tx_ba_handle->us_baw_head,
                             pst_tx_ba_handle->us_baw_tail,
                             pst_tx_ba_handle->us_last_seq_num);
        }

#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        OAM_ERROR_LOG2(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::pst_cb_first null 0x%X or pst_txop_alg null 0x%X.}", pst_cb_first, pst_txop_alg);

        /* ???????????????????????? */
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &st_dscr_head)
        {
            pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            oal_dlist_delete_entry(pst_dscr_entry);
            dmac_free_tx_dscr(pst_tx_dscr);
        }

        /* ???????????? */
        st_ppdu_feature.uc_mpdu_num += uc_mpdu_idx;
        pst_tid_queue->us_mpdu_num -= uc_mpdu_idx;
        pst_dev->us_total_mpdu_num -= uc_mpdu_idx;
        pst_dev->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id] -= uc_mpdu_idx;
        pst_dev->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)] -= uc_mpdu_idx;

        DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_ampdu_cnt, uc_mpdu_idx);
        DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_total_cnt, uc_mpdu_idx);
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        return OAL_FAIL;
    }
#endif

    /*??????????mpdu????*/
    if (0 == uc_mpdu_idx)
    {
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &st_dscr_head)
        {
            OAM_ERROR_LOG2(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::tx pkt dropped .}", pst_tid_queue->us_mpdu_num, pst_tid_queue->uc_retry_num);
            pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            oal_dlist_delete_entry(pst_dscr_entry);
            dmac_free_tx_dscr(pst_tx_dscr);
        }
    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        return OAL_SUCC;
    }
    else
    {
        /*????????????*/
        //dmac_ba_addto_baw_multi(pst_tx_ba_handle, uc_mpdu_idx - uc_retry_mpdu_num);
        /*OAM_INFO_LOG4(uc_vap_id, OAM_SF_AMPDU,
                      "{dmac_tid_tx_queue_remove_ampdu::baw info:us_baw_start=%d us_last_seq_num=%d us_baw_head=%d us_baw_tail=%d}",
                      pst_tx_ba_handle->us_baw_start, pst_tx_ba_handle->us_last_seq_num,
                      pst_tx_ba_handle->us_baw_head, pst_tx_ba_handle->us_baw_tail);*/

        /*????????????tid??????????????????*/
        if (OAL_PTR_NULL != pst_tx_dscr_prev)
        {
            hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_tx_dscr_prev);

    #ifndef _PRE_WLAN_FEATURE_TX_DSCR_OPT
            oal_dlist_remove_head(&st_dscr_head, &(pst_tid_queue->st_hdr), &(pst_tx_dscr_prev->st_entry));
    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        }

        /*????????*/
        st_ppdu_feature.uc_mpdu_num += uc_mpdu_idx;
        pst_tid_queue->us_mpdu_num -= uc_mpdu_idx;
        pst_dev->us_total_mpdu_num -= uc_mpdu_idx;
        pst_dev->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id] -= uc_mpdu_idx;
        pst_dev->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)] -= uc_mpdu_idx;

        DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_ampdu_cnt, uc_mpdu_idx);
        DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_total_cnt, uc_mpdu_idx);

#ifdef _PRE_PROFILING_MODE
        g_ul_tx_mpdu_num +=uc_mpdu_idx;
        g_ul_tx_ampdu_num ++;
#endif
    }

#ifdef _PRE_DEBUG_MODE
    OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_tra_entry, &st_dscr_head)
    {
        pst_trace_dscr = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dscr_tra_entry, hal_tx_dscr_stru, st_entry);
        OAL_MEM_TRACE(pst_trace_dscr, OAL_FALSE);
    }
#ifdef _PRE_WLAN_FEATURE_AMPDU
//#if ((_PRE_TARGET_PRODUCT_TYPE_5610DMB != _PRE_CONFIG_TARGET_PRODUCT) )
#if 0
    /*????ba handle????*/
    dmac_ba_track_ba_hander(pst_dev->uc_device_id, pst_tx_ba_handle, pst_tid_queue->uc_tid, OAL_TRUE);
    /*????tid queue????????*/
    dmac_ba_track_tid_queue(pst_dev->uc_device_id, pst_hal_device, pst_tx_ba_handle->uc_tx_ba_lut, pst_tid_queue);
    /*????????????????????*/
    dmac_ba_track_schedule_seqnum(pst_dev->uc_device_id, pst_tx_ba_handle->uc_tx_ba_lut, &st_dscr_head);
    /*??????????????????????????*/
    dmac_ba_track_tail_update(pst_dev->uc_device_id, pst_tx_ba_handle->uc_tx_ba_lut);
//#endif
#endif
#endif   /* end of _PRE_WLAN_FEATURE_AMPDU */
#endif   /* end of _PRE_DEBUG_MODE */

    /*OAM_INFO_LOG4(uc_vap_id, OAM_SF_AMPDU,
                  "{dmac_tid_tx_queue_remove_ampdu::baw info after aggr %d %d %d %d.}",
                  pst_tx_ba_handle->us_baw_start, pst_tx_ba_handle->us_last_seq_num,
                  pst_tx_ba_handle->us_baw_head, pst_tx_ba_handle->us_baw_tail);*/

    /* 11ac AMPDU??????????MPDU??????pading */
    if (OAL_UNLIKELY(OAL_PTR_NULL != pst_txop_alg)) {
        if (WLAN_VHT_PHY_PROTOCOL_MODE == pst_txop_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_ht_rate.bit_protocol_mode)
        {
            st_ppdu_feature.ul_ampdu_length += us_pad_len;
        }
    }

    if (DMAC_TX_MODE_RIFS != pst_tid_queue->en_tx_mode)
    {
        st_ppdu_feature.uc_ampdu_enable = OAL_TRUE;
    }
    else
    {
        ul_ret = dmac_tx_rifs_process(pst_hal_device, pst_dmac_vap, pst_tid_queue, &st_ppdu_feature, &st_dscr_head);
        if (OAL_SUCC != ul_ret)
        {
            st_ppdu_feature.uc_ampdu_enable = OAL_TRUE;
        }
    }

    if (OAL_LIKELY(OAL_PTR_NULL != pst_cb_first))
    {
        MAC_GET_CB_MPDU_NUM(pst_cb_first) = st_ppdu_feature.uc_mpdu_num;
    }

    /*OAM_INFO_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove_ampdu::st_ppdu_feature uc_mpdu_num=%d.}", st_ppdu_feature.uc_mpdu_num);*/

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_REMOVE_QUEUE);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_REMOVE_QUEUE);

    dmac_tx_data(pst_dmac_vap, pst_user, pst_cb_first, &st_dscr_head, &st_ppdu_feature, pst_txop_alg);

    return OAL_SUCC;
}

oal_uint32  dmac_tid_tx_dequeue(hal_to_dmac_device_stru    *pst_hal_device,
                                        dmac_vap_stru      *pst_dmac_vap,
                                        dmac_tid_stru      *pst_tid_queue,
                                        hal_tx_dscr_stru   *pst_tx_dscr,
                                        mac_device_stru    *pst_dev,
                                        dmac_user_stru     *pst_user)
{
    mac_tx_ctl_stru            *pst_cb;
    oal_netbuf_stru            *pst_netbuf;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_DFT_STAT
    dmac_tid_stats_stru *pst_tid_stats;
    pst_tid_stats = pst_tid_queue->pst_tid_stats;
    if (OAL_PTR_NULL == pst_tid_stats)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tid_tx_dequeue::dequeue from netbuff Q or retry Q failed, pst_tid_stats NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
#endif

    OAL_MEM_TRACE(pst_tx_dscr, OAL_FALSE);
    pst_tx_dscr->bit_is_first = 0;

    pst_tid_queue->us_mpdu_num--;
    if ((OAL_TRUE == pst_tx_dscr->bit_is_retried) && (pst_tid_queue->uc_retry_num > 0))
    {
        pst_tid_queue->uc_retry_num--;
    }

    DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_normal_cnt, 1);
    DMAC_TID_STATS_INCR(pst_tid_stats->ul_tid_dequeue_total_cnt, 1);

    pst_dev->us_total_mpdu_num--;
    pst_dev->aus_vap_mpdu_num[pst_tid_queue->uc_vap_id]--;
    pst_dev->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid_queue->uc_tid)]--;

    pst_netbuf = pst_tx_dscr->pst_skb_start_addr;
    pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if (OAL_PTR_NULL == pst_cb)
    {
        OAM_ERROR_LOG0(pst_tid_queue->uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_dequeue::dequeue from netbuff Q or retry Q failed, pst_cb null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    MAC_GET_CB_TX_USER_IDX(pst_cb)  = (oal_uint8)pst_user->st_user_base_info.us_assoc_id;

    /* ???????????????????? */
    if ((FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
        && (OAL_TRUE != pst_tx_dscr->bit_is_retried))
    {
#ifdef _PRE_WLAN_DFT_STAT
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_pkts,
                               MAC_GET_CB_NETBUF_NUM(pst_cb));
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_drv_tx_bytes,
                                MAC_GET_CB_MPDU_BYTES(pst_cb));
#endif
        DMAC_USER_STATS_PKT_INCR(pst_user->st_query_stats.ul_tx_total, MAC_GET_CB_NETBUF_NUM(pst_cb));
    }


    /*????????qos??????????????seqnum*/
    if ((OAL_FALSE == pst_tx_dscr->bit_is_retried) && (OAL_TRUE == mac_get_cb_is_qosdata(pst_cb)))
    {
        dmac_save_frag_seq(pst_user, pst_cb);
        dmac_tx_seqnum_set(pst_user, pst_cb);

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
        /* ????????PN ?? */
        dmac_tx_sw_set_ccmp_PN(pst_hal_device, pst_user, pst_tx_dscr, pst_netbuf);
#endif
    }

    return OAL_SUCC;
}



oal_uint32  dmac_tid_tx_queue_remove(hal_to_dmac_device_stru   *pst_hal_device,
                                     dmac_vap_stru             *pst_dmac_vap,
                                     dmac_tid_stru             *pst_tid_queue,
                                     oal_uint8                  uc_dscr_num)
{
    mac_device_stru            *pst_dev = OAL_PTR_NULL;
    dmac_user_stru             *pst_user = OAL_PTR_NULL;
    oal_dlist_head_stru        *pst_dscr_entry  = OAL_PTR_NULL;
    hal_tx_dscr_stru           *pst_tx_dscr = OAL_PTR_NULL;
    hal_tx_txop_alg_stru       *pst_txop_alg = OAL_PTR_NULL;
    mac_tx_ctl_stru            *pst_cb = OAL_PTR_NULL;
    oal_netbuf_stru            *pst_netbuf = OAL_PTR_NULL;
    hal_tx_ppdu_feature_stru    st_ppdu_feature;
    oal_uint8                   uc_dscr_idx;
    hal_tx_dscr_stru           *pst_tx_dscr_prev = OAL_PTR_NULL;
    oal_dlist_head_stru         st_dscr_head;
    oal_uint8                   uc_vap_id;
    oal_uint8                   uc_dscr_num_tmp;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_DFT_STAT
    dmac_tid_stats_stru *pst_tid_stats;
    pst_tid_stats = pst_tid_queue->pst_tid_stats;
    if (OAL_PTR_NULL == pst_tid_stats)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tid_tx_queue_remove::tid_stats is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif
#endif
    uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;
    pst_user = (dmac_user_stru *)mac_res_get_dmac_user(pst_tid_queue->us_user_idx);
    if (OAL_PTR_NULL == pst_user)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::pst_user null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????device?????????????????? */
    pst_dev = mac_res_get_dev(pst_user->st_user_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_dev)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::pst_dev null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_dlist_init_head(&st_dscr_head);

    uc_dscr_num_tmp = uc_dscr_num;

    /* ???????????????? */
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
    for (uc_dscr_idx = 0; uc_dscr_idx < uc_dscr_num; uc_dscr_idx++)
    {
        if (OAL_TRUE == oal_dlist_is_empty(&(pst_tid_queue->st_retry_q)))
        {
            break;
        }

        pst_dscr_entry = oal_dlist_delete_head(&(pst_tid_queue->st_retry_q));
        pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
        oal_dlist_add_tail(pst_dscr_entry, &st_dscr_head);

        if (0 != uc_dscr_idx)
        {
            hal_tx_ctrl_dscr_link(pst_hal_device, pst_tx_dscr_prev, pst_tx_dscr);
        }
        pst_tx_dscr_prev = pst_tx_dscr;

        dmac_tid_tx_dequeue(pst_hal_device, pst_dmac_vap, pst_tid_queue, pst_tx_dscr, pst_dev, pst_user);
        pst_netbuf = pst_tx_dscr->pst_skb_start_addr;
        pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    }

    /* ?????????????? */
    uc_dscr_num -= uc_dscr_idx;
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

    /* ????TID?????? */
    for (uc_dscr_idx = 0; uc_dscr_idx < uc_dscr_num; uc_dscr_idx++)
    {
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        if (OAL_TRUE == oal_netbuf_list_empty(&pst_tid_queue->st_buff_head))
        {
            break;
        }

        pst_netbuf  = dmac_tx_dequeue_first_mpdu(&pst_tid_queue->st_buff_head);
        pst_tx_dscr = dmac_tx_dscr_alloc(pst_hal_device, pst_dmac_vap, pst_netbuf);
        if (OAL_PTR_NULL == pst_tx_dscr)
        {
            OAM_WARNING_LOG3(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::put the pkt back into tid netbuf Q, alloc tx dscr failed. retry = %d, mpdu = %d, uc_mpdu_num = %d}",
                             pst_tid_queue->uc_retry_num, pst_tid_queue->us_mpdu_num, uc_dscr_num);

            dmac_tx_queue_mpdu(pst_netbuf, &pst_tid_queue->st_buff_head);
            break;
        }
        oal_dlist_add_tail(&pst_tx_dscr->st_entry, &st_dscr_head);
    #else
        /* ????tid?????????????????????????? */
        if (oal_dlist_is_empty(&(pst_tid_queue->st_hdr)))
        {
            //OAM_INFO_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::empty tid.}\r\n");
            break;
        }

        pst_dscr_entry = oal_dlist_delete_head(&(pst_tid_queue->st_hdr));
        pst_tx_dscr    = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
        oal_dlist_add_tail(pst_dscr_entry, &st_dscr_head);

    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        if (0 != uc_dscr_idx)
        {
            hal_tx_ctrl_dscr_link(pst_hal_device, pst_tx_dscr_prev, pst_tx_dscr);
        }

        pst_tx_dscr_prev = pst_tx_dscr;

        dmac_tid_tx_dequeue(pst_hal_device, pst_dmac_vap, pst_tid_queue, pst_tx_dscr, pst_dev, pst_user);
        pst_netbuf = pst_tx_dscr->pst_skb_start_addr;
        pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    }

    if (oal_dlist_is_empty(&st_dscr_head))
    {
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        /* ???????????????????????????????????????????????????????????? */
        OAM_WARNING_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::no packets can be transmitted.}");
    #else
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::no packets can be transmitted.}");
    #endif /* */
        return OAL_FAIL;
    }

    if (OAL_PTR_NULL != pst_tx_dscr_prev)
    {
        hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_tx_dscr_prev);
    }

    /* ??????ppdu feature???? */
    dmac_tx_init_ppdu_feature(pst_dmac_vap, pst_user, 1, &st_ppdu_feature);
    dmac_tx_update_ppdu_feature(pst_netbuf, &st_ppdu_feature);
    hal_tx_ctrl_dscr_unlink(pst_hal_device, pst_tx_dscr);

    if (OAL_PTR_NULL == pst_cb)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tid_tx_queue_remove::pst_cb null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    MAC_GET_CB_MPDU_NUM(pst_cb) = uc_dscr_num_tmp;
    if (0 == uc_dscr_num_tmp)
    {
        OAM_ERROR_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::tid %d, uc_dscr_num_tmp is zero.}", mac_get_cb_tid(pst_cb));
    }

    /* ????TXOP???????????? */
    dmac_tx_get_txop_alg_params(pst_dmac_vap, pst_user, pst_cb, &pst_txop_alg);
    if (OAL_PTR_NULL == pst_txop_alg)
    {
        OAM_WARNING_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tid_tx_queue_remove::pst_txop_alg null.}");
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        if (OAL_PTR_NULL != pst_cb)
        {
            pst_dmac_vap->st_vap_base_info.st_vap_stats.ul_tx_dropped_packets += MAC_GET_CB_NETBUF_NUM(pst_cb);
        }
#endif
        dmac_tx_excp_free_dscr(&st_dscr_head, pst_hal_device);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_PTR_NULL == pst_cb)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (WLAN_PROT_NO != pst_dmac_vap->st_vap_base_info.st_protection.en_protection_mode)
    {
        /*????????????????????????*/
        dmac_tx_update_protection_txop_alg(pst_dmac_vap, pst_txop_alg, OAL_FALSE, pst_cb->en_ismcast);
    }

    /* ????????????????RTS */
    dmac_judge_forbid_rts(pst_user, pst_txop_alg);

    /* 2040???????????????????? */
    /* dmac_tx_update_bandwidth_mode(pst_dmac_vap, pst_user, pst_txop_alg); */

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_MPDU_REMOVE_QUEUE);

    dmac_tx_data(pst_dmac_vap, pst_user, pst_cb, &st_dscr_head, &st_ppdu_feature, pst_txop_alg);
    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_uint32 dmac_tx_update_protection_lsig_txop(dmac_vap_stru *pst_dmac_vap, hal_tx_txop_alg_stru *pst_txop_alg, oal_uint8 uc_do_default_cfg)
{
    mac_protection_stru          *pst_protection;
    wlan_phy_protocol_enum_uint8  en_protocol_mode;

    en_protocol_mode = pst_txop_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_ht_rate.bit_protocol_mode;
    pst_protection = &(pst_dmac_vap->st_vap_base_info.st_protection);

    /*????VAP??????L-SIG TXOP????????????ht????????????????????????HT, ??????????????????????bit_lsig_txop??1*/
    if ((OAL_SWITCH_ON == pst_protection->bit_lsig_txop_protect_mode)
         &&(WLAN_HT_PHY_PROTOCOL_MODE == en_protocol_mode))
    {
        pst_txop_alg->st_rate.bit_lsig_txop = OAL_TRUE;
    }
    else
    {
        /*??????????????????????????????lsig txop????????0*/
        if (OAL_TRUE == uc_do_default_cfg)
        {
            pst_txop_alg->st_rate.bit_lsig_txop = OAL_FALSE;
        }
        else /*??????????????????bit_lsig_txop??1????0*/
        {
            /*????????????????????????????????????????bit_lsig_txop???????? ??????????????*/
        }
    }

    return OAL_SUCC;
}


#if 0

OAL_STATIC OAL_INLINE oal_uint32 dmac_tx_update_protection_rtscts_enable(dmac_vap_stru *pst_dmac_vap, hal_tx_txop_alg_stru *pst_txop_alg, oal_uint8 uc_do_default_cfg, oal_bool_enum_uint8 en_mcast)
{
    mac_protection_stru          *pst_protection;
    wlan_phy_protocol_enum_uint8  en_protocol_mode;
    oal_uint8                     uc_rts_cts_enable = 0xFF;

    en_protocol_mode = pst_txop_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_ht_rate.bit_protocol_mode;
    pst_protection = &(pst_dmac_vap->st_vap_base_info.st_protection);

    /*????VAP??????RTS-CTS????????erp?????? ????????????????????11b, ??????????????????????bit_rts_cts_enable??1*/
    if ((WLAN_PROT_ERP == pst_protection->en_protection_mode)
         &&(OAL_SWITCH_ON == pst_protection->bit_rts_cts_protect_mode)
         && (WLAN_11B_PHY_PROTOCOL_MODE != en_protocol_mode))
    {
        uc_rts_cts_enable = OAL_TRUE;
    }

    /*????VAP??????RTS-CTS????????ht?????? ??????????????????HT??VHT, ??????????????????????bit_rts_cts_enable??1*/
    else if ((WLAN_PROT_HT == pst_protection->en_protection_mode)
         && (OAL_SWITCH_ON == pst_protection->bit_rts_cts_protect_mode)
         && ((WLAN_HT_PHY_PROTOCOL_MODE == en_protocol_mode) || (WLAN_VHT_PHY_PROTOCOL_MODE == en_protocol_mode)))
    {
        uc_rts_cts_enable = OAL_TRUE;
    }
    else
    {
        /*??????????????????????????????bit_rts_cts_enable????????0*/
        if (OAL_TRUE == uc_do_default_cfg)
        {
            uc_rts_cts_enable = OAL_FALSE;
        }
        else /*??????????????????bit_lsig_txop??1????0*/
        {
            /*????????????????????????????????????????bit_rts_cts_enable???????? ??????????????*/
        }
    }

    /* ????????????RTS */
    if (en_mcast == OAL_TRUE)
    {
        uc_rts_cts_enable = OAL_FALSE;
    }

    /*OAM_INFO_LOG2(0, OAM_SF_TX, "{dmac_tx_update_protection_rtscts_enable::rts_cts_enable=%d en_protocol_mode=%d}",
                        uc_rts_cts_enable, en_protocol_mode);*/

    /*uc_rts_cts_enable????0xFF????????????????????bit_rts_cts_enable????*/
    if (0xFF != uc_rts_cts_enable)
    {
        pst_txop_alg->ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = uc_rts_cts_enable;
        pst_txop_alg->ast_per_rate[1].rate_bit_stru.bit_rts_cts_enable = uc_rts_cts_enable;
        pst_txop_alg->ast_per_rate[2].rate_bit_stru.bit_rts_cts_enable = uc_rts_cts_enable;
        pst_txop_alg->ast_per_rate[3].rate_bit_stru.bit_rts_cts_enable = uc_rts_cts_enable;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC OAL_INLINE oal_uint32 dmac_tx_update_protection_preamble_mode(dmac_vap_stru *pst_dmac_vap, hal_tx_txop_alg_stru *pst_txop_alg, oal_uint8 uc_do_default_cfg)
{
    dmac_user_stru               *pst_dmac_user;
    mac_protection_stru          *pst_protection;
    wlan_phy_protocol_enum_uint8  en_protocol_mode;
    oal_uint8                     uc_preamble_mode = 0;
    oal_uint8                     uc_rate_index = 0;
    oal_uint8                     uc_legacy_rate = 0;

    pst_protection = &(pst_dmac_vap->st_vap_base_info.st_protection);

    for(uc_rate_index = 0; uc_rate_index < HAL_TX_RATE_MAX_NUM; uc_rate_index++)
    {
        /*????????count??0?? ??????????????????*/
        if (0 == pst_txop_alg->ast_per_rate[uc_rate_index].rate_bit_stru.bit_tx_count)
        {
            continue;
        }

        uc_preamble_mode = 0xFF;
        en_protocol_mode = pst_txop_alg->ast_per_rate[uc_rate_index].rate_bit_stru.un_nss_rate.st_ht_rate.bit_protocol_mode;
        uc_legacy_rate   = pst_txop_alg->ast_per_rate[uc_rate_index].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_legacy_rate;

        /*11b???????? ????????????????long preamble*/
        if (WLAN_11B_PHY_PROTOCOL_MODE == en_protocol_mode)
        {
            /*VAP ?? AP??????*/
            if (WLAN_VAP_MODE_BSS_AP == pst_dmac_vap->st_vap_base_info.en_vap_mode)
            {
                /* ????????????short preamble??????AP?????? ????AP??????????short preamble??????????????1mpbs????????????AP????long preamble??????????????*/
                if ((0 != pst_protection->uc_sta_no_short_preamble_num)
                     || (OAL_TRUE != mac_mib_get_ShortPreambleOptionImplemented(&(pst_dmac_vap->st_vap_base_info)))
                     || (WLAN_PHY_RATE_1M == uc_legacy_rate))
                {
                    uc_preamble_mode = WLAN_LEGACY_11B_DSCR_LONG_PREAMBLE;
                }
                else
                {
                    /*??????????????????????????????????????short preamble*/
                    if (OAL_TRUE == uc_do_default_cfg)
                    {
                        uc_preamble_mode = WLAN_LEGACY_11B_DSCR_SHORT_PREAMBLE;
                    }
                    else /*????????????????????*/
                    {
                        /*????????????????????????????????????????uc_preamble_mode???????? ??????????????*/
                    }
                }

            }
            else/*VAP ?? STA??????*/
            {
                pst_dmac_user = mac_res_get_dmac_user(pst_dmac_vap->st_vap_base_info.uc_assoc_vap_id); /*user????????AP??????*/
                if (OAL_PTR_NULL == pst_dmac_user)
                {
                    OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_update_protection_preamble_mode::pst_dmac_user null, assoc_vap_id [%d].}",
                                    pst_dmac_vap->st_vap_base_info.uc_assoc_vap_id);
                    return OAL_ERR_CODE_PTR_NULL;
                }

                /* ??????????beacon??????????ERP??????????baker preamble mode??1?? ????STA??????????short preamble?? ??????????STA????long preamble??????????????*/
                if ((OAL_TRUE == pst_dmac_user->st_user_base_info.st_cap_info.bit_barker_preamble_mode)
                      || (OAL_TRUE != mac_mib_get_ShortPreambleOptionImplemented(&(pst_dmac_vap->st_vap_base_info)))
                      || (WLAN_PHY_RATE_1M == uc_legacy_rate))
                {
                    uc_preamble_mode = WLAN_LEGACY_11B_DSCR_LONG_PREAMBLE;
                }
                else
                {
                    /*??????????????????????????????????????short preamble*/
                    if (OAL_TRUE == uc_do_default_cfg)
                    {
                        uc_preamble_mode = WLAN_LEGACY_11B_DSCR_SHORT_PREAMBLE;
                    }
                    else /*????????????????????*/
                    {
                        /*????????????????????????????????????????uc_preamble_mode???????? ??????????????*/
                    }
                }
            }
        }
        /*HT??????*/
        else if (WLAN_HT_PHY_PROTOCOL_MODE == en_protocol_mode)
        {
            /*??????GF?????????? ????????????????????MF??????????*/
            if (WLAN_PROT_GF == pst_protection->en_protection_mode)
            {
                uc_preamble_mode = OAL_FALSE;
            }
            else
            {
                /*??????????????????????????????????????,1151??????????????????MF??????????*/
                if (OAL_TRUE == uc_do_default_cfg)
                {
                    uc_preamble_mode = OAL_FALSE;
                }
                else /*????????????????????*/
                {
                    /*????????????????????????????????????????uc_preamble_mode???????? ??????????????*/
                }
            }
        }
        else /*??????????????preamble??0*/
        {
            uc_preamble_mode = 0;
        }

        if(0xFF != uc_preamble_mode)
        {
            pst_txop_alg->ast_per_rate[uc_rate_index].rate_bit_stru.bit_preamble_mode = uc_preamble_mode;
        }

    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_tx_update_protection_txop_alg(dmac_vap_stru *pst_dmac_vap, hal_tx_txop_alg_stru *pst_txop_alg, oal_uint8 uc_do_default_cfg, oal_bool_enum_uint8 en_mcast)
{
    oal_uint32 ul_ret = OAL_SUCC;

    /*????lsig txop????*/
    dmac_tx_update_protection_lsig_txop(pst_dmac_vap, pst_txop_alg, uc_do_default_cfg);

    
#if 0
    /*????rts cts enable????*/
    dmac_tx_update_protection_rtscts_enable(pst_dmac_vap, pst_txop_alg, uc_do_default_cfg, en_mcast);
#endif

    /*????preamble????*/
    ul_ret = dmac_tx_update_protection_preamble_mode(pst_dmac_vap, pst_txop_alg, uc_do_default_cfg);

    return ul_ret;
}
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif


oal_uint32  dmac_tx_clear_tx_queue(hal_to_dmac_device_stru *pst_hal_device)
{
    oal_uint8            uc_q_id            = 0;
    oal_dlist_head_stru *pst_dlist_entry    = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr        = OAL_PTR_NULL;
    oal_uint8            uc_loop_idx        = 0;

    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_clear_tx_queue::pst_hal_device null.}");

        return OAL_FAIL;
    }

    pst_hal_device->bit_clear_fifo_st = OAL_TRUE;
    hal_clear_hw_fifo(pst_hal_device);

    /*????????????????????????*/
    frw_event_flush_event_queue(FRW_EVENT_TYPE_WLAN_TX_COMP);

    /* ????5?????????? ???????????????????????????????????????????????????????????????????? */
    for (uc_loop_idx = 0; uc_loop_idx <= HAL_TX_QUEUE_HI; uc_loop_idx++)
    {
        uc_q_id = HAL_TX_QUEUE_HI - uc_loop_idx;
        while(!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header))
        {
            pst_dlist_entry = pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header.pst_next;

            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);

            dmac_tx_complete_free_dscr(pst_tx_dscr);
        }

        pst_hal_device->ast_tx_dscr_queue[uc_q_id].uc_ppdu_cnt = 0;
    }

    return OAL_SUCC;
}

oal_void dmac_proc_save_tx_queue(hal_to_dmac_device_stru       *pst_hal_device,
                                            hal_tx_dscr_queue_header_stru *pst_fake_queue)
{
    oal_uint8            uc_q_id            = 0;
    oal_dlist_head_stru *pst_dlist_entry    = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr        = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr_tmp    = OAL_PTR_NULL;
    oal_uint8            uc_tx_status;
    mac_tx_ctl_stru     *pst_cb;
    oal_netbuf_stru     *pst_buf;


    /*
     * ????5?????????? ??????????????????????????????????????????????????????????????(BAR)????????
     * ??????????????????????save??
     *
     * ??????????????:
     *  1??????????????????????????????AMPDU????????????????flush event??????BAR??
     *     BAR??????????????(??????????????)????????save
     *  2??save??????????????????????AMPDU??????irq loss??????BAR????BAR????????????????
     *     ??????????????????????????save??????????????????????????????
     */
    for (uc_q_id = 0; uc_q_id <= HAL_TX_QUEUE_HI; uc_q_id++)
    {
        while(!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header))
        {
            pst_dlist_entry = pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header.pst_next;

            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);
            if(OAL_PTR_NULL == pst_tx_dscr)
            {
                OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_save_tx_queue::pst_tx_dscr poiter is null.}");
                break;
            }
            hal_tx_get_dscr_status(pst_hal_device, pst_tx_dscr, &uc_tx_status);

            // ????????????????????????????status????????????9??????????????????
            // ??????????????????????????????status??????????????????????seq number
            if (DMAC_TX_PENDING == uc_tx_status)
            {
                pst_buf = pst_tx_dscr->pst_skb_start_addr;
                pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
                (MAC_GET_CB_RETRIED_NUM(pst_cb))++;

                if (MAC_GET_CB_RETRIED_NUM(pst_cb) > DMAC_MAX_PENDING_RETRY)
                {
                    OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_save_tx_queue::retry cnt=%d, break}", MAC_GET_CB_RETRIED_NUM(pst_cb));
                    dmac_tx_complete_buff(pst_hal_device, pst_tx_dscr);
                    break;
                }

                pst_tx_dscr_tmp  = pst_tx_dscr;
                do
                {
                    pst_tx_dscr_tmp->bit_is_retried = OAL_TRUE;

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
                    {
                        oal_bool_enum_uint8      en_tx_pn_hw_bypass;
                        hal_dscr_get_tx_pn_hw_bypass(pst_tx_dscr_tmp, &en_tx_pn_hw_bypass);
                        if (en_tx_pn_hw_bypass == OAL_TRUE)
                        {
                            pst_tx_dscr_tmp->bit_is_first = OAL_FALSE;
                            hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_tx_dscr_tmp, 1);
                            dmac_tx_sw_restore_ccmp_PN_from_mac_hdr(pst_hal_device, pst_tx_dscr_tmp);
                        }
                        else
                        {
                            hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_tx_dscr_tmp, 0);
                        }
                    }
#else
                    hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_tx_dscr_tmp, 0);
#endif // _PRE_WLAN_MAC_BUGFIX_PN

                    hal_tx_set_dscr_status(pst_hal_device, pst_tx_dscr_tmp, DMAC_TX_INVALID);
                    hal_get_tx_dscr_next(pst_hal_device, pst_tx_dscr_tmp, &pst_tx_dscr_tmp);
                }while(OAL_PTR_NULL != pst_tx_dscr_tmp);

                break; // ????????????????????????
            }
            else if (DMAC_TX_INVALID != uc_tx_status)
            {
                dmac_tx_complete_buff(pst_hal_device, pst_tx_dscr);

                continue;
            }
            else
            {
                break;
            }
        }
    }

    for (uc_q_id = 0; uc_q_id <= HAL_TX_QUEUE_HI; uc_q_id++)
    {
        /* ?????????????????????????????????? */
        oal_dlist_join_head(&pst_fake_queue[uc_q_id].st_header, &pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header);
        pst_fake_queue[uc_q_id].uc_ppdu_cnt += pst_hal_device->ast_tx_dscr_queue[uc_q_id].uc_ppdu_cnt;

        oal_dlist_init_head(&pst_hal_device->ast_tx_dscr_queue[uc_q_id].st_header);
        pst_hal_device->ast_tx_dscr_queue[uc_q_id].uc_ppdu_cnt = 0;
    }

    return;
}


oal_uint32  dmac_tx_save_tx_queue(hal_to_dmac_device_stru *pst_hal_device, oal_uint8 uc_fake_q_id)
{
    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        MAC_ERR_LOG(0, "pst_hal_device is null");
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_save_tx_queue::pst_hal_device null.}");

        return OAL_FAIL;
    }

    if (OAL_UNLIKELY(uc_fake_q_id >= HAL_TX_FAKE_QUEUE_NUM))
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "dmac_tx_save_tx_queue: uc_fake_q_id is invalid, %d", uc_fake_q_id);
        return OAL_FAIL;
    }

    dmac_proc_save_tx_queue(pst_hal_device, pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q_id]);

    return OAL_SUCC;
}


oal_uint32  dmac_tx_clear_fake_queue(hal_to_dmac_device_stru *pst_hal_device, oal_uint8 uc_fake_q_id)
{
    hal_tx_dscr_queue_header_stru  *pst_fake_queue;
    oal_uint8                       uc_q_id;
    oal_uint8                       uc_loop_idx;
    oal_dlist_head_stru            *pst_dlist_entry;
    hal_tx_dscr_stru               *pst_tx_dscr;

    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "pst_hal_device is null.}");
        return OAL_FAIL;
    }

    if (OAL_UNLIKELY(uc_fake_q_id >= HAL_TX_FAKE_QUEUE_NUM))
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "uc_fake_q_id is invalid, %d", uc_fake_q_id);
        return OAL_FAIL;
    }

    pst_fake_queue = pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q_id];

    /* ????5?????????? ???????????????????????????????????????????????????????????????????? */
    for (uc_loop_idx = 0; uc_loop_idx <= HAL_TX_QUEUE_HI; uc_loop_idx++)
    {
        uc_q_id = HAL_TX_QUEUE_HI - uc_loop_idx;

        while(!oal_dlist_is_empty(&pst_fake_queue[uc_q_id].st_header))
        {
            pst_dlist_entry = pst_fake_queue[uc_q_id].st_header.pst_next;

            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);

            dmac_tx_complete_free_dscr(pst_tx_dscr);
        }

        pst_fake_queue[uc_q_id].uc_ppdu_cnt = 0;
    }

    return OAL_SUCC;
}


oal_uint32  dmac_tx_fake_queue_empty_assert(
                hal_to_dmac_device_stru *pst_hal_device,
                oal_uint8                uc_fake_q,
                oal_uint32               ul_file_id)
{
    hal_tx_dscr_queue_header_stru  *pst_fake_queue;
    oal_uint8                       uc_q_idx;
    oal_dlist_head_stru            *pst_dlist_entry;
    hal_tx_dscr_stru               *pst_tx_dscr;
    oal_netbuf_stru                *pst_netbuf;
    mac_tx_ctl_stru                *pst_tx_ctl;
    oal_uint32                      ul_ret = OAL_TRUE;

    pst_fake_queue = pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q];

    for (uc_q_idx = 0; uc_q_idx < HAL_TX_QUEUE_BUTT; uc_q_idx++)
    {
        if (!oal_dlist_is_empty(&pst_fake_queue[uc_q_idx].st_header))
        {
            /* ??????????????????????????????OTA???????? */

            pst_dlist_entry = pst_fake_queue[uc_q_idx].st_header.pst_next;
            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);
            oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_tx_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);

            pst_netbuf = pst_tx_dscr->pst_skb_start_addr;

            pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

            oam_report_80211_frame(BROADCAST_MACADDR,
                                    (oal_uint8*)mac_get_cb_frame_hdr(pst_tx_ctl),
                                    MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl),
                                    (oal_uint8*)oal_netbuf_payload(pst_netbuf),
                                    MAC_GET_CB_MPDU_LEN(pst_tx_ctl),
                                    OAM_OTA_FRAME_DIRECTION_TYPE_TX);

            OAM_ERROR_LOG4(0, OAM_SF_DBAC, "fake q is not empty. q_id:%d, ac:%d, file_id:%d, ppdu_cnt:%d",
                            uc_fake_q, uc_q_idx, ul_file_id, pst_fake_queue[uc_q_idx].uc_ppdu_cnt);
            ul_ret = OAL_FALSE;
        }
    }

    return ul_ret;
}


oal_uint32  dmac_tx_switch_tx_queue(hal_to_dmac_device_stru *pst_hal_device, oal_uint8 uc_fake_q_id1, oal_uint8 uc_fake_q_id2)
{
    oal_uint8                       uc_q_idx;
    oal_uint8                       uc_ppdu_num;
    hal_tx_dscr_queue_header_stru  *pst_fake_queue1;
    hal_tx_dscr_queue_header_stru  *pst_fake_queue2;
    oal_dlist_head_stru             st_head_tmp;

    pst_fake_queue1 = pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q_id1];
    pst_fake_queue2 = pst_hal_device->ast_tx_dscr_queue_fake[uc_fake_q_id2];

    for (uc_q_idx = 0; uc_q_idx < HAL_TX_QUEUE_BUTT; uc_q_idx++)
    {
        oal_dlist_init_head(&st_head_tmp);
        oal_dlist_join_head(&st_head_tmp, &pst_fake_queue1[uc_q_idx].st_header);

        oal_dlist_init_head(&pst_fake_queue1[uc_q_idx].st_header);
        oal_dlist_join_head(&pst_fake_queue1[uc_q_idx].st_header, &pst_fake_queue2[uc_q_idx].st_header);

        oal_dlist_init_head(&pst_fake_queue2[uc_q_idx].st_header);
        oal_dlist_join_head(&pst_fake_queue2[uc_q_idx].st_header, &st_head_tmp);

        uc_ppdu_num                           = pst_fake_queue1[uc_q_idx].uc_ppdu_cnt;
        pst_fake_queue1[uc_q_idx].uc_ppdu_cnt = pst_fake_queue2[uc_q_idx].uc_ppdu_cnt;
        pst_fake_queue2[uc_q_idx].uc_ppdu_cnt = uc_ppdu_num;
    }

    return OAL_SUCC;
}


#if 0

oal_uint32  dmac_flush_txq_to_tid(hal_to_dmac_device_stru *pst_hal_device)
{
    oal_uint8            uc_q_idx           = 0;
    oal_dlist_head_stru *pst_dlist_entry    = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr        = OAL_PTR_NULL;
    mac_tx_ctl_stru     *pst_cb             = OAL_PTR_NULL;
    dmac_tid_stru       *pst_tid            = OAL_PTR_NULL;
    dmac_user_stru      *pst_dmac_user      = OAL_PTR_NULL;
    mac_device_stru     *pst_mac_device;
#ifdef _PRE_WLAN_FEATURE_FLOWCTL
    mac_vap_stru        *pst_vap            = OAL_PTR_NULL;
#endif

    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        MAC_ERR_LOG(0, "pst_hal_device is null");
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid::pst_hal_device null.}");

        return OAL_FAIL;
    }

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        MAC_ERR_LOG(0, "pst_mac_device is null");
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid::pst_mac_device null.}");

        return OAL_FAIL;
    }

    /* ???????????????????????????????????????????? */
    while(!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[HAL_TX_QUEUE_HI].st_header))
    {
        pst_dlist_entry = oal_dlist_delete_tail(&pst_hal_device->ast_tx_dscr_queue[HAL_TX_QUEUE_HI].st_header);
        pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);
        oal_netbuf_free(pst_tx_dscr->pst_skb_start_addr);
        OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
    }

    /* ????????4?????????? */
    for (uc_q_idx = 0; uc_q_idx < HAL_TX_QUEUE_HI; uc_q_idx++)
    {
        if (!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header))
        {
            dmac_tx_dump_data(pst_hal_device, pst_cb, &pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header);
        }

        /* ????????????????????????????tid???????? */
        while(!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header))
        {
            pst_dlist_entry = oal_dlist_delete_tail(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header);

            if(pst_hal_device->ast_tx_dscr_queue[uc_q_idx].uc_ppdu_cnt == 0)
            {
                MAC_ERR_VAR(0, "hw queue%d.len == 0\n", uc_q_idx);
                OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid::uc_q_idx=%d.}", uc_q_idx);

                continue;
            }
            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);

            pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_tx_dscr->pst_skb_start_addr);

            pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_cb));
            pst_tid       = &pst_dmac_user->ast_tx_tid_queue[mac_get_cb_tid(pst_cb)];

            oal_dlist_add_head(pst_dlist_entry, &pst_tid->st_hdr);
            pst_tid->us_mpdu_num++;
            pst_mac_device->us_total_mpdu_num++;
            pst_mac_device->aus_vap_mpdu_num[pst_tid->uc_vap_id]++;
            pst_mac_device->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid->uc_tid)]++;
#ifdef _PRE_WLAN_FEATURE_FLOWCTL
            pst_vap = mac_res_get_mac_vap(pst_tid->uc_vap_id);
            dmac_alg_flowctl_backp_notify(pst_vap, pst_mac_device->us_total_mpdu_num, pst_mac_device->aus_ac_mpdu_num);
#endif
        }

        pst_hal_device->ast_tx_dscr_queue[uc_q_idx].uc_ppdu_cnt = 0;
    }

    return OAL_SUCC;
}
#endif

oal_uint32  dmac_flush_txq_to_tid_of_vo(hal_to_dmac_device_stru *pst_hal_device)
{
    oal_uint8            uc_q_idx           = 0;
    oal_dlist_head_stru *pst_dlist_entry    = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr        = OAL_PTR_NULL;
    mac_tx_ctl_stru     *pst_cb             = OAL_PTR_NULL;
    dmac_tid_stru       *pst_tid            = OAL_PTR_NULL;
    dmac_user_stru      *pst_dmac_user      = OAL_PTR_NULL;
    mac_device_stru                     *pst_mac_device;
    mac_ieee80211_qos_htc_frame_stru *pst_frame_head = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_FLOWCTL
    mac_vap_stru        *pst_vap            = OAL_PTR_NULL;
#endif
    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        MAC_ERR_LOG(0, "pst_hal_device is null");
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid_of_vo::pst_hal_device null.}");

        return OAL_FAIL;
    }

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_device_id);
    if(OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        MAC_ERR_LOG(0, "pst_mac_device is null");
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid_of_vo::pst_mac_device null.}");

        return OAL_FAIL;
    }

    /* ????????4?????????? */
    for (uc_q_idx = 0; uc_q_idx < HAL_TX_QUEUE_HI; uc_q_idx++)
    {
        /* ????????????????????????????tid???????? */
        while(!oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header))
        {
            pst_dlist_entry = oal_dlist_delete_tail(&pst_hal_device->ast_tx_dscr_queue[uc_q_idx].st_header);
            pst_tx_dscr     = (hal_tx_dscr_stru *)OAL_DLIST_GET_ENTRY(pst_dlist_entry, hal_tx_dscr_stru, st_entry);

            pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_tx_dscr->pst_skb_start_addr);

            pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_cb));
            if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
            {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_flush_txq_to_tid_of_vo::mac_res_get_dmac_user fail.user_idx = %u}",MAC_GET_CB_TX_USER_IDX(pst_cb));
                continue;
            }
            /* ????????????cb??edca?????????? */
            pst_tx_dscr->uc_q_num = HAL_TX_QUEUE_VO;
            mac_set_cb_tid(pst_cb, MAC_WMM_SWITCH_TID);
            mac_set_cb_ac(pst_cb, WLAN_WME_AC_VO);
            /* ??????????tid */
            pst_frame_head = (mac_ieee80211_qos_htc_frame_stru *)mac_get_cb_frame_hdr(pst_cb);
            pst_frame_head->bit_qc_tid = MAC_WMM_SWITCH_TID;

            /* ????WMM??????????Q??????TID??6?????? */
            pst_tid       = &pst_dmac_user->ast_tx_tid_queue[MAC_WMM_SWITCH_TID];
        #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
            pst_tx_dscr->bit_is_retried = OAL_TRUE;
            oal_dlist_add_head(pst_dlist_entry, &pst_tid->st_retry_q);
        #else
            oal_dlist_add_head(pst_dlist_entry, &pst_tid->st_hdr);
        #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
            pst_tid->us_mpdu_num++;
            pst_mac_device->us_total_mpdu_num++;
            pst_mac_device->aus_vap_mpdu_num[pst_tid->uc_vap_id]++;
            pst_mac_device->aus_ac_mpdu_num[WLAN_WME_TID_TO_AC(pst_tid->uc_tid)]++;
#ifdef _PRE_WLAN_FEATURE_FLOWCTL
            pst_vap = mac_res_get_mac_vap(pst_tid->uc_vap_id);
            dmac_alg_flowctl_backp_notify(pst_vap, pst_mac_device->us_total_mpdu_num, pst_mac_device->aus_ac_mpdu_num);
#endif
        }
        pst_hal_device->ast_tx_dscr_queue[uc_q_idx].uc_ppdu_cnt = 0;
    }


    return OAL_SUCC;
}


oal_uint32  dmac_tx_update_protection_all_txop_alg(dmac_vap_stru *pst_dmac_vap)
{
    if(OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_update_protection_all_txop_alg::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*??????????????txop alg????*/
    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->st_tx_alg), OAL_TRUE, OAL_FALSE);

    /*??????????????txop alg????*/
    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->st_tx_data_mcast), OAL_TRUE, OAL_TRUE);

     /*??????????????txop alg????*/
    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->st_tx_data_bcast), OAL_TRUE, OAL_TRUE);

    /*??????????????txop alg????*/
    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G]), OAL_TRUE, OAL_TRUE);

    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G]), OAL_TRUE, OAL_TRUE);

    /*??????????????????txop alg????*/
    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G]), OAL_TRUE, OAL_TRUE);

    dmac_tx_update_protection_txop_alg(pst_dmac_vap, &(pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G]), OAL_TRUE, OAL_TRUE);

    return OAL_SUCC;
}

#if defined (_PRE_WLAN_FEATURE_UAPSD) || defined (_PRE_WLAN_FEATURE_STA_PM)

oal_uint32 dmac_send_qosnull(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, oal_uint8 uc_ac, oal_bool_enum_uint8 en_ps)
{
    oal_netbuf_stru                 *pst_net_buf;
    mac_tx_ctl_stru                 *pst_tx_ctrl;
    oal_uint32                       ul_ret;
    mac_ieee80211_qos_frame_stru    *pst_mac_header;
    oal_uint16                       us_tx_direction = WLAN_FRAME_FROM_AP;
    /* ???????? */
    if ((OAL_PTR_NULL == pst_dmac_vap) || (OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_ERROR_LOG0(0, OAM_SF_KEEPALIVE, "{dmac_uapsd_send_qosnull:: func input  is null.}");
        return OAL_ERR_CODE_KEEPALIVE_PTR_NULL;
    }
    if (uc_ac >= MAC_AC_PARAM_LEN)
    {
        OAM_ERROR_LOG1(0, OAM_SF_KEEPALIVE, "{dmac_uapsd_send_qosnull:: uc_ac %d is too large.}", uc_ac);
        return OAL_FAIL;
    }
    /* ????net_buff */
    pst_net_buf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, WLAN_SHORT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (OAL_PTR_NULL == pst_net_buf)
    {
        OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_KEEPALIVE, "{dmac_uapsd_send_qosnull::pst_net_buf failed.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    OAL_MEM_NETBUF_TRACE(pst_net_buf, OAL_TRUE);
    oal_set_netbuf_prev(pst_net_buf, OAL_PTR_NULL);
    oal_set_netbuf_next(pst_net_buf, OAL_PTR_NULL);
    /* null??????????From AP || To AP */
    us_tx_direction = (WLAN_VAP_MODE_BSS_AP == pst_dmac_vap->st_vap_base_info.en_vap_mode) ? WLAN_FRAME_FROM_AP : WLAN_FRAME_TO_AP;
    /* ???????? */
    OAL_MEMZERO(OAL_NETBUF_HEADER(pst_net_buf), OAL_SIZEOF(mac_ieee80211_qos_frame_stru));
    mac_null_data_encap(OAL_NETBUF_HEADER(pst_net_buf),
                        ((oal_uint16)(WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_DATA | WLAN_FC0_SUBTYPE_QOS_NULL) | us_tx_direction),
                        pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                        pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    pst_mac_header = (mac_ieee80211_qos_frame_stru*)OAL_NETBUF_HEADER(pst_net_buf);
    pst_mac_header->bit_qc_tid = WLAN_WME_AC_TO_TID(uc_ac);
    pst_mac_header->bit_qc_eosp = 0;
    pst_mac_header->st_frame_control.bit_power_mgmt = en_ps;

    /*??????????????QOS NULL DATA??????normal ack*/
    pst_mac_header->bit_qc_ack_polocy = WLAN_TX_NORMAL_ACK;
    /* ????cb???? */
    pst_tx_ctrl = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_net_buf);
    OAL_MEMZERO(pst_tx_ctrl, OAL_SIZEOF(mac_tx_ctl_stru));
    /* ????tx???? */
    mac_set_cb_is_qosdata(pst_tx_ctrl, OAL_TRUE);
    mac_set_cb_ack_policy(pst_tx_ctrl, WLAN_TX_NORMAL_ACK);
    MAC_GET_CB_EVENT_TYPE(pst_tx_ctrl)      = FRW_EVENT_TYPE_WLAN_DTX;
    mac_set_cb_is_bar(pst_tx_ctrl, OAL_FALSE);
    mac_set_cb_ac(pst_tx_ctrl,uc_ac);
    MAC_GET_CB_IS_FIRST_MSDU(pst_tx_ctrl)    = OAL_TRUE;
    MAC_GET_CB_RETRIED_NUM(pst_tx_ctrl)      = 0;
    mac_set_cb_tid(pst_tx_ctrl, WLAN_WME_AC_TO_TID(uc_ac));
    MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctrl)     = pst_dmac_vap->st_vap_base_info.uc_vap_id;
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctrl)      = (oal_uint8)pst_dmac_user->st_user_base_info.us_assoc_id;
    /* ????tx rx???????? */
    //MAC_GET_CB_FRAME_TYPE(pst_tx_ctrl)              = WLAN_DATA_NULL;
    MAC_GET_CB_IS_MCAST(pst_tx_ctrl)                = OAL_FALSE;
    mac_set_cb_is_amsdu(pst_tx_ctrl, OAL_FALSE);
    MAC_GET_CB_IS_FROM_PS_QUEUE(pst_tx_ctrl)       = OAL_FALSE;
    MAC_GET_CB_IS_PROBE_DATA(pst_tx_ctrl)           = OAL_FALSE;
    mac_set_cb_is_use_4_addr(pst_tx_ctrl, OAL_FALSE);
    mac_set_cb_frame_hdr(pst_tx_ctrl, (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_net_buf));
    MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctrl)     = OAL_SIZEOF(mac_ieee80211_qos_frame_stru);
    MAC_GET_CB_MPDU_NUM(pst_tx_ctrl)                = 1;
    MAC_GET_CB_NETBUF_NUM(pst_tx_ctrl)              = 1;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctrl)                = 0;
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctrl)             = (oal_uint8)pst_dmac_user->st_user_base_info.us_assoc_id;
    //MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctrl)        = 1;/* NULL????????10?? */
    ul_ret = dmac_tx_process_data(pst_dmac_vap->pst_hal_device, pst_dmac_vap, pst_net_buf);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_KEEPALIVE,
                       "{dmac_uapsd_send_qosnull::dmac_tx_process_data failed[%d].}", ul_ret);
        dmac_tx_excp_free_netbuf(pst_net_buf);
        return ul_ret;
    }
    return OAL_SUCC;
}
#endif


oal_uint32 dmac_alg_downlink_flowctl_notify(mac_vap_stru *pst_vap,
                                                                mac_user_stru *pst_user,
                                                                oal_netbuf_stru *pst_buf)
{
    oal_uint32                         ul_ret;
    dmac_txrx_output_type_enum_uint8   en_output;
    mac_device_stru                   *pst_mac_device;
    dmac_vap_stru               *pst_dmac_vap;

    pst_mac_device = mac_res_get_dev(pst_vap->uc_device_id);
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device) || (OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_ERROR_LOG0(pst_vap->uc_vap_id, OAM_SF_ANY, "{dmac_alg_downlink_flowctl_notify::pst_mac_device  or pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????????? ????????(??tx buffer??????????????????????????????) */
    if (OAL_PTR_NULL == gst_alg_main.p_downlink_flowctl_func)
    {
        if( WLAN_TID_MPDU_NUM_LIMIT <= pst_mac_device->us_total_mpdu_num)
        {
            OAM_ERROR_LOG1(pst_vap->uc_vap_id, OAM_SF_ACS, "tx buffer is full[%d],the coming buffer drop", pst_mac_device->us_total_mpdu_num);
            return OAL_FAIL;
        }
        else
        {
            return OAL_SUCC;
        }
    }
    else
    {
        /* ?????????????????????????????????? */
        if (OAL_FALSE == pst_dmac_vap->en_multi_user_multi_ac_flag)
        {
            /* ???????????????? ???????????????? */
            ul_ret = gst_alg_main.p_downlink_flowctl_func(pst_vap, pst_user, pst_buf, &en_output);
            if ((OAL_SUCC != ul_ret)||(DMAC_TXRX_DROP == en_output))
            {
                OAM_INFO_LOG2(pst_vap->uc_vap_id, OAM_SF_ANY, "{dmac_alg_downlink_flowctl_notify::ul_ret=%d, en_output=%d}",
                              ul_ret, en_output);
                return OAL_FAIL;
            }
        }
	return OAL_SUCC;
    }

}

/*lint -e578*//*lint -e19*/
#if 0
oal_module_symbol(dmac_flush_txq_to_tid);
#endif
oal_module_symbol(dmac_tx_save_tx_queue);
oal_module_symbol(dmac_tx_clear_fake_queue);
oal_module_symbol(dmac_tx_restore_tx_queue);
oal_module_symbol(dmac_tx_switch_tx_queue);
oal_module_symbol(dmac_tx_process_data_event);
oal_module_symbol(dmac_tx_mgmt);
oal_module_symbol(dmac_tx_clear_tx_queue);
oal_module_symbol(dmac_alg_downlink_flowctl_notify);
oal_module_symbol(dmac_tx_fake_queue_empty_assert);
/*lint +e578*//*lint +e19*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

