


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_profiling.h"

#include "oam_ext_if.h"

#include "mac_data.h"

#include "dmac_ext_if.h"
#include "dmac_tx_complete.h"
#include "dmac_mgmt_classifier.h"
#include "dmac_blockack.h"
#include "dmac_psm_ap.h"
#include "dmac_uapsd.h"
#include "dmac_chan_mgmt.h"
#include "dmac_ext_if.h"
#include "dmac_dft.h"
#include "mac_data.h"
#include "dmac_p2p.h"
#ifdef _PRE_WLAN_CHIP_TEST
#include "dmac_test_main.h"
#include "dmac_test_sch.h"
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "dmac_psm_sta.h"
#endif
#ifdef _PRE_WLAN_DFT_STAT
#include "mac_board.h"
#include "dmac_device.h"
#endif
#include "dmac_tx_bss_comm.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_TX_COMPLETE_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/
oal_uint8 g_uc_aggr_num_switch = 0;     /* ????????AMPDU???????????? */
oal_uint8 g_uc_max_aggr_num    = 0;     /* ????????AMPDU???????? */
#ifdef _PRE_WLAN_INIT_PTK_TX_PN
dmac_tx_dscr_pn_stru g_ul_prev_iv_word[WLAN_CHIP_MAX_NUM][DMAC_MAX_RA_LUT_INDEX] = {
{   {0, 0x10010000,0x20000000}, {1, 0x10010000,0x20000000}, {2, 0x10010000,0x20000000}, {3, 0x10010000,0x20000000}, {4, 0x10010000,0x20000000},
    {5, 0x10010000,0x20000000}, {6, 0x10010000,0x20000000}, {7, 0x10010000,0x20000000}, {8, 0x10010000,0x20000000}, {9, 0x10010000,0x20000000},
    {10,0x10010000,0x20000000}, {11,0x10010000,0x20000000}, {12,0x10010000,0x20000000}, {13,0x10010000,0x20000000}, {14,0x10010000,0x20000000},
    {15,0x10010000,0x20000000}, {16,0x10010000,0x20000000}, {17,0x10010000,0x20000000}, {18,0x10010000,0x20000000}, {19,0x10010000,0x20000000},
    {20,0x10010000,0x20000000}, {21,0x10010000,0x20000000}, {22,0x10010000,0x20000000}, {23,0x10010000,0x20000000}, {24,0x10010000,0x20000000},
    {25,0x10010000,0x20000000}, {26,0x10010000,0x20000000}, {27,0x10010000,0x20000000}, {28,0x10010000,0x20000000}, {29,0x10010000,0x20000000},
    {30,0x10010000,0x20000000}, {31,0x10010000,0x20000000}  },

{   {0, 0x10010000,0x20000000}, {1, 0x10010000,0x20000000}, {2, 0x10010000,0x20000000}, {3, 0x10010000,0x20000000}, {4, 0x10010000,0x20000000},
    {5, 0x10010000,0x20000000}, {6, 0x10010000,0x20000000}, {7, 0x10010000,0x20000000}, {8, 0x10010000,0x20000000}, {9, 0x10010000,0x20000000},
    {10,0x10010000,0x20000000}, {11,0x10010000,0x20000000}, {12,0x10010000,0x20000000}, {13,0x10010000,0x20000000}, {14,0x10010000,0x20000000},
    {15,0x10010000,0x20000000}, {16,0x10010000,0x20000000}, {17,0x10010000,0x20000000}, {18,0x10010000,0x20000000}, {19,0x10010000,0x20000000},
    {20,0x10010000,0x20000000}, {21,0x10010000,0x20000000}, {22,0x10010000,0x20000000}, {23,0x10010000,0x20000000}, {24,0x10010000,0x20000000},
    {25,0x10010000,0x20000000}, {26,0x10010000,0x20000000}, {27,0x10010000,0x20000000}, {28,0x10010000,0x20000000}, {29,0x10010000,0x20000000},
    {30,0x10010000,0x20000000}, {31,0x10010000,0x20000000}  } };

dmac_tx_dscr_pn_stru g_ul_max_iv_word[WLAN_CHIP_MAX_NUM][DMAC_MAX_RA_LUT_INDEX] = {
{   {0, 0x10010000,0x20000000}, {1, 0x10010000,0x20000000}, {2, 0x10010000,0x20000000}, {3, 0x10010000,0x20000000}, {4, 0x10010000,0x20000000},
    {5, 0x10010000,0x20000000}, {6, 0x10010000,0x20000000}, {7, 0x10010000,0x20000000}, {8, 0x10010000,0x20000000}, {9, 0x10010000,0x20000000},
    {10,0x10010000,0x20000000}, {11,0x10010000,0x20000000}, {12,0x10010000,0x20000000}, {13,0x10010000,0x20000000}, {14,0x10010000,0x20000000},
    {15,0x10010000,0x20000000}, {16,0x10010000,0x20000000}, {17,0x10010000,0x20000000}, {18,0x10010000,0x20000000}, {19,0x10010000,0x20000000},
    {20,0x10010000,0x20000000}, {21,0x10010000,0x20000000}, {22,0x10010000,0x20000000}, {23,0x10010000,0x20000000}, {24,0x10010000,0x20000000},
    {25,0x10010000,0x20000000}, {26,0x10010000,0x20000000}, {27,0x10010000,0x20000000}, {28,0x10010000,0x20000000}, {29,0x10010000,0x20000000},
    {30,0x10010000,0x20000000}, {31,0x10010000,0x20000000}  },

{   {0, 0x10010000,0x20000000}, {1, 0x10010000,0x20000000}, {2, 0x10010000,0x20000000}, {3, 0x10010000,0x20000000}, {4, 0x10010000,0x20000000},
    {5, 0x10010000,0x20000000}, {6, 0x10010000,0x20000000}, {7, 0x10010000,0x20000000}, {8, 0x10010000,0x20000000}, {9, 0x10010000,0x20000000},
    {10,0x10010000,0x20000000}, {11,0x10010000,0x20000000}, {12,0x10010000,0x20000000}, {13,0x10010000,0x20000000}, {14,0x10010000,0x20000000},
    {15,0x10010000,0x20000000}, {16,0x10010000,0x20000000}, {17,0x10010000,0x20000000}, {18,0x10010000,0x20000000}, {19,0x10010000,0x20000000},
    {20,0x10010000,0x20000000}, {21,0x10010000,0x20000000}, {22,0x10010000,0x20000000}, {23,0x10010000,0x20000000}, {24,0x10010000,0x20000000},
    {25,0x10010000,0x20000000}, {26,0x10010000,0x20000000}, {27,0x10010000,0x20000000}, {28,0x10010000,0x20000000}, {29,0x10010000,0x20000000},
    {30,0x10010000,0x20000000}, {31,0x10010000,0x20000000}  } };
#endif
#if (defined(_PRE_DEBUG_MODE) && (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE))
oal_uint32 g_ul_desc_check_count[HAL_TX_QUEUE_BUTT] = {0};
oal_uint32 g_ul_desc_addr[HAL_TX_QUEUE_BUTT] = {0};
#endif

/* ???????????? */
OAL_STATIC oal_uint32  dmac_tx_complete_normal_buffer(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_dscr);
#ifdef _PRE_WLAN_FEATURE_AMPDU
OAL_STATIC oal_uint32  dmac_tx_complete_ampdu_buffer(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_dscr);
#endif
OAL_STATIC oal_uint32  dmac_tx_update_alg_param(hal_tx_dscr_stru *pst_dscr,
                                                  oal_netbuf_stru *pst_buf,
                                                  hal_tx_dscr_ctrl_one_param *pst_tx_dscr_one,
                                                  hal_to_dmac_device_stru * pst_hal_device);
extern oal_uint32  dmac_send_sys_event(
                mac_vap_stru                     *pst_mac_vap,
                wlan_cfgid_enum_uint16            en_cfg_id,
                oal_uint16                        us_len,
                oal_uint8                        *puc_param);
oal_uint32  dmac_config_11i_add_key_set_reg(mac_vap_stru *pst_mac_vap, oal_uint8 uc_key_index, oal_uint8 *puc_mac_addr);


OAL_STATIC oal_uint32  dmac_tx_complete_dump_dscr(
                                          hal_to_dmac_device_stru *pst_dmac_device,
                                          hal_tx_dscr_stru        *pst_base_dscr)
{
    oal_uint32             ul_ret;
    mac_tx_ctl_stru       *pst_tx_cb;
    oal_uint32             ul_dscr_one_size;
    oal_uint32             ul_dscr_two_size;
    oal_switch_enum_uint8  en_frame_switch;
    oal_switch_enum_uint8  en_cb_switch;
    oal_switch_enum_uint8  en_dscr_switch;
    oal_uint8              auc_user_macaddr[WLAN_MAC_ADDR_LEN] = {0};
    oam_user_track_frame_type_enum_uint8 en_frame_type;
    oal_netbuf_stru *pst_netbuf;
    oal_uint8               uc_data_type;
    oal_uint8               uc_dscr_status;
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    mac_ieee80211_frame_stru *pst_frame;
#endif
    pst_netbuf = pst_base_dscr->pst_skb_start_addr;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_netbuf))
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_dump_dscr::pst_netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????? */
    pst_tx_cb = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_netbuf);
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_frame= ((mac_ieee80211_frame_stru *)((oal_uint8 *)pst_tx_cb + OAL_MAX_CB_LEN));
#endif
    /* ???????????? */
    if (WLAN_MANAGEMENT == MAC_GET_CB_FRAME_TYPE(pst_tx_cb)
        || WLAN_CONTROL == MAC_GET_CB_FRAME_TYPE(pst_tx_cb))
    {
        en_frame_type = OAM_USER_TRACK_FRAME_TYPE_MGMT;
    }
    else
    {
        if(OAL_TRUE == MAC_GET_CB_IS_VIPFRAME(pst_tx_cb))
        {
            hal_tx_get_dscr_status(pst_dmac_device, pst_base_dscr, &uc_dscr_status);
            uc_data_type = mac_get_data_type(pst_netbuf);
            /* ???????????????????????? */
            if((MAC_DATA_DHCP == uc_data_type) ||
        #if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
                (MAC_DATA_ARP_REQ == uc_data_type)||
                (MAC_DATA_ARP_RSP == uc_data_type) ||
        #endif
                (MAC_DATA_EAPOL == uc_data_type))
            {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
                OAM_WARNING_LOG4(0, OAM_SF_ANY, "{dmac_tx_complete_dump_dscr::tx datatype=%u to XX:XX:XX:XX:%x:%x  rst==%u}[0:dhcp 1:arp_req 2:arp_rsp 3:eapol]",
                             uc_data_type,  pst_frame->auc_address1[4], pst_frame->auc_address1[5], uc_dscr_status);
#else
                OAM_WARNING_LOG2(0, OAM_SF_ANY, "{dmac_tx_complete_dump_dscr::tx datatype=%u rst==%u}[0:dhcp 1:arp_req 2:arp_rsp 3:eapol]",
                             uc_data_type, uc_dscr_status);
#endif
            }
        }
        en_frame_type = OAM_USER_TRACK_FRAME_TYPE_DATA;
        if (OAL_SWITCH_ON != oam_report_data_get_global_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX))
        {
            return OAL_SUCC;
        }

    }

    ul_ret = dmac_tx_dump_get_switch(en_frame_type, &en_frame_switch, &en_cb_switch, &en_dscr_switch, pst_tx_cb);
    if (OAL_ERR_CODE_PTR_NULL == ul_ret)
    {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_dump_dscr::dmac_tx_dump_get_switch return null, user_idx=%d.", pst_tx_cb->bit_tx_user_idx);
#else
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_dump_dscr::dmac_tx_dump_get_switch return null, user_idx=%d.", pst_tx_cb->us_tx_user_idx);
#endif
        return ul_ret;
    }

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    if (WLAN_MANAGEMENT == pst_frame->st_frame_control.bit_type
        && (WLAN_DISASOC == pst_frame->st_frame_control.bit_sub_type || WLAN_DEAUTH == pst_frame->st_frame_control.bit_sub_type))
    {
        hal_tx_get_dscr_status(pst_dmac_device, pst_base_dscr, &uc_dscr_status);

        OAM_WARNING_LOG4(0, OAM_SF_ANY, "{dmac_tx_complete_dump_dscr::tx disassoc or deauth=%u[10:DISASOC, 12:DEAUTH] to XX:XX:XX:XX:%x:%x. status=%u.}",
                                         pst_frame->st_frame_control.bit_sub_type, pst_frame->auc_address1[4], pst_frame->auc_address1[5], uc_dscr_status);
    }
#endif

    if (OAL_FAIL == ul_ret)
    {
        return OAL_SUCC;
    }

    if (OAL_SWITCH_ON == en_dscr_switch)
    {

        /* ????????mac???? */
        ul_ret = dmac_tx_dump_get_user_macaddr(pst_tx_cb, auc_user_macaddr);
        hal_tx_get_size_dscr(pst_dmac_device, MAC_GET_CB_NETBUF_NUM(pst_tx_cb), &ul_dscr_one_size, &ul_dscr_two_size);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_dump_dscr::dmac_tx_get_user_macaddr failed[%d].", ul_ret);
            return ul_ret;
        }
        oam_report_dscr(auc_user_macaddr,
                        (oal_uint8 *)pst_base_dscr,
                        (oal_uint16)(ul_dscr_one_size + ul_dscr_two_size),
                        OAM_OTA_TYPE_TX_DSCR);
    }
#ifdef _PRE_DEBUG_MODE
    if ((pst_base_dscr->uc_q_num != HAL_TX_QUEUE_HI))
    {
        g_ast_tx_complete_stat[pst_dmac_device->uc_mac_device_id].ul_tx_complete_bh2_num++;
        OAM_INFO_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_dump_dscr::g_ul_tx_complete_bh2_num = %d.", g_ast_tx_complete_stat[pst_dmac_device->uc_mac_device_id].ul_tx_complete_bh2_num);
    }
#endif

    return OAL_SUCC;
}


#ifdef _PRE_DEBUG_MODE_USER_TRACK

OAL_STATIC OAL_INLINE oal_uint32  dmac_tx_complete_check_protocol(
                                        hal_to_dmac_device_stru *pst_hal_device,
                                        dmac_user_stru   *pst_dmac_user,
                                        hal_tx_dscr_stru *pst_dscr)
{
    wlan_phy_protocol_enum_uint8   en_protocol = WLAN_PHY_PROTOCOL_BUTT;

    hal_tx_get_protocol_mode(pst_hal_device, pst_dscr, &en_protocol);

    dmac_user_check_txrx_protocol_change(pst_dmac_user, en_protocol, OAM_USER_INFO_CHANGE_TYPE_TX_PROTOCOL);

    return OAL_SUCC;
}
#endif


OAL_STATIC OAL_INLINE oal_void  dmac_tx_get_vap_id(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_tx_dscr, oal_uint8 *puc_vap_id)
{
    oal_uint8                        uc_hal_vap_id;
    hal_to_dmac_vap_stru            *pst_hal_vap = OAL_PTR_NULL;

    hal_tx_get_vap_id(pst_hal_device, pst_tx_dscr, &uc_hal_vap_id);

    hal_get_hal_vap(pst_hal_device, uc_hal_vap_id, &pst_hal_vap);

    if (OAL_PTR_NULL == pst_hal_vap)
    {
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_get_vap_id::pst_hal_vap null uc_hal_vap_id = %d.}",uc_hal_vap_id);

        *puc_vap_id = 0;
    }
    else
    {
        *puc_vap_id = pst_hal_vap->uc_mac_vap_id;
    }

    return;
}
#if 0

oal_void  dmac_tx_delete_dscr(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_delete_dscr)
{
    oal_uint32          ul_ret;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    hal_tx_set_dscr_status(pst_hal_device, pst_delete_dscr, DMAC_TX_SOFT_PSM_BACK);
#else
    hal_tx_set_dscr_status(pst_hal_device, pst_delete_dscr, DMAC_TX_SOFT_RESET_BACK);
#endif
    ul_ret = dmac_tx_complete_buff(pst_hal_device, pst_delete_dscr);
    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_delete_dscr::dmac_tx_complete_buff failed[%d].", ul_ret);
    }
}
#endif

OAL_STATIC OAL_INLINE oal_void dmac_keepalive_timestamp_update(dmac_user_stru *pst_dmac_user,
                                                           oal_uint8     *puc_frame_hdr,
                                                           oal_uint8      uc_dscr_status)
{
    if ((WLAN_DATA_BASICTYPE != mac_frame_get_type_value(puc_frame_hdr)) ||
        (uc_dscr_status != DMAC_TX_SUCC))
    {
        return;
    }

    /* ?????????????? */
    pst_dmac_user->ul_last_active_timestamp = (oal_uint32)OAL_TIME_GET_STAMP_MS();

}

#ifdef _PRE_WLAN_FEATURE_AMPDU
#ifdef _PRE_DEBUG_MODE

OAL_STATIC oal_void  dmac_tx_complete_get_ampdu_len(
                                        hal_to_dmac_device_stru * pst_hal_device,
                                        hal_tx_dscr_stru *pst_dscr,
                                        oal_uint32       *pul_ampdu_len)
{
    hal_tx_get_ampdu_len(pst_hal_device, pst_dscr, pul_ampdu_len);
}
#endif
#endif

oal_void dmac_tx_delete_ba_fail_process(dmac_user_stru *pst_dmac_user)
{
    mac_device_stru             *pst_mac_device;
    oal_uint8                    uc_tid;
    mac_user_stru               *pst_mac_user = &(pst_dmac_user->st_user_base_info);

    /* ???????????????????????????????? */
    dmac_alg_delete_ba_fail_notify(pst_mac_user);

    pst_mac_device = mac_res_get_dev(pst_mac_user->uc_device_id);

    for (uc_tid = 0; uc_tid < WLAN_TID_MAX_NUM; uc_tid++)
    {
        if (OAL_PTR_NULL != pst_dmac_user->ast_tx_tid_queue[uc_tid].pst_ba_tx_hdl)
        {
            /* ??????ba??????????TID???? */
            dmac_tid_resume(pst_mac_device->pst_device_stru,&pst_dmac_user->ast_tx_tid_queue[uc_tid], DMAC_TID_PAUSE_RESUME_TYPE_BA);
        }
    }

}

oal_void dmac_tx_delete_ba(dmac_user_stru *pst_dmac_user)
{
    oal_uint8                    uc_tid;
    mac_user_stru               *pst_mac_user = &(pst_dmac_user->st_user_base_info);
    frw_event_mem_stru          *pst_event_mem;
    frw_event_stru              *pst_event;
    dmac_to_hmac_ctx_event_stru *pst_del_ba_event;

    for (uc_tid = 0; uc_tid < WLAN_TID_MAX_NUM; uc_tid++)
    {
        if (OAL_PTR_NULL != pst_dmac_user->ast_tx_tid_queue[uc_tid].pst_ba_tx_hdl)
        {
            /* ????????????TID????,??BA???????????? */
            dmac_tid_pause(&pst_dmac_user->ast_tx_tid_queue[uc_tid], DMAC_TID_PAUSE_RESUME_TYPE_BA);
        }
    }

    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_to_hmac_ctx_event_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        /* ????ba?????????????? */
        dmac_tx_delete_ba_fail_process(pst_dmac_user);
        OAM_ERROR_LOG0(pst_mac_user->uc_vap_id, OAM_SF_BA, "{dmac_tx_delete_ba::alloc event failed!}");
        return;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ?????????? */
    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_SDT_REG,
                       DMAC_TO_HMAC_DEL_BA,
                       OAL_SIZEOF(dmac_to_hmac_ctx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_user->uc_chip_id,
                       pst_mac_user->uc_device_id,
                       pst_mac_user->uc_vap_id);

    pst_del_ba_event = (dmac_to_hmac_ctx_event_stru *)(pst_event->auc_event_data);

    pst_del_ba_event->us_user_index     = pst_mac_user->us_assoc_id;
    pst_del_ba_event->uc_tid            = 0xFF;     /* ????HMAC????????TID??BA???? */
    pst_del_ba_event->uc_vap_id         = pst_mac_user->uc_vap_id;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_del_ba_event->uc_cur_protocol   = pst_mac_user->en_cur_protocol_mode;
#endif

    if(OAL_SUCC != frw_event_dispatch_event(pst_event_mem))
    {
        /* ????ba?????????????? */
        dmac_tx_delete_ba_fail_process(pst_dmac_user);
        OAM_ERROR_LOG0(pst_mac_user->uc_vap_id, OAM_SF_TX, "{dmac_tx_delete_ba::post event failed}");
    }

    FRW_EVENT_FREE(pst_event_mem);

    return;
}

#ifdef _PRE_DEBUG_MODE
OAL_STATIC OAL_INLINE oal_void dmac_tx_complete_debug_dscr(hal_to_dmac_device_stru *pst_dmac_device,
                                                                       hal_tx_dscr_stru *pst_base_dscr)
{
    oal_mempool_tx_dscr_addr  *pst_tx_dscr_addr   = OAL_PTR_NULL;
    oal_dlist_head_stru       *pst_dscr_entry;
    hal_tx_dscr_stru          *pst_dscr;
    oal_uint16                 us_dscr_idx_in_list = 0;
    oal_uint16                 us_tx_dscr_cnt_dn = 0;
    mac_tx_ctl_stru           *pst_tx_cb;

    if (1 == oal_mem_get_stop_flag())
    {
       return ;
    }

    pst_tx_dscr_addr = oal_mem_get_tx_dscr_addr();

    if ((pst_tx_dscr_addr->us_rcd_rls_stop_flag > 0) && (pst_tx_dscr_addr->us_rcd_rls_stop_flag < OAL_TX_DSCR_RCD_TAIL_CNT))
    {
        pst_tx_dscr_addr->us_rcd_rls_stop_flag++;
    }

    us_tx_dscr_cnt_dn = pst_tx_dscr_addr->us_tx_dscr_cnt_dn;
    pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].ul_dn_intr_ts = (oal_uint32)OAL_TIME_GET_STAMP_MS();
    pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].ul_tx_dscr_in_dn_intr = (oal_uint32)pst_base_dscr;
    pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].uc_q_num = pst_base_dscr->uc_q_num;
    pst_tx_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_base_dscr->pst_skb_start_addr);
    if (OAL_PTR_NULL != pst_tx_cb)
    {
        pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].uc_mpdu_num = MAC_GET_CB_MPDU_NUM(pst_tx_cb);
    }

    OAL_MEMZERO(pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].ul_tx_dscr_in_q, OAL_SIZEOF(oal_uint32) * OAL_MAX_TX_DSCR_CNT_IN_LIST);
    OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &(pst_dmac_device->ast_tx_dscr_queue[pst_base_dscr->uc_q_num].st_header))
    {
        pst_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
        if (us_dscr_idx_in_list < OAL_MAX_TX_DSCR_CNT_IN_LIST)
        {
            pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].ul_tx_dscr_in_q[us_dscr_idx_in_list++] = (oal_uint32)pst_dscr;
        }
        else
        {
            pst_tx_dscr_addr->ast_tx_dscr_info[us_tx_dscr_cnt_dn].bit_dscr_is_overflow = OAL_TRUE;
            break;
        }
    }
    pst_tx_dscr_addr->us_tx_dscr_cnt_dn = (us_tx_dscr_cnt_dn + 1) % OAL_TX_DSCR_ITEM_NUM;

}
#endif

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif

oal_uint32  dmac_tx_complete_event_handler(frw_event_mem_stru *pst_event_mem)
{
    hal_tx_dscr_stru                *pst_base_dscr;          /* ?????????????????????????????? */
    hal_to_dmac_device_stru         *pst_hal_device;
    frw_event_stru                  *pst_event;
    hal_tx_complete_event_stru      *pst_tx_comp_event;      /* tx complete???? */
    hal_tx_dscr_stru                *pst_curr_dscr;         /*dev???????????? ??????????????*/
    oal_uint32                       ul_ret = OAL_SUCC;
    hal_tx_queue_type_enum           uc_queue_num;           /* ???????????? */
#ifdef _PRE_DEBUG_MODE
    mac_tx_ctl_stru                 *pst_tx_cb;
    oal_dlist_head_stru             *pst_dscr_entry;
    oal_uint16                       us_report_seqnum;
    oal_uint16                       us_curr_seqnum;
    dmac_user_stru                  *pst_dmac_user;
    dmac_tid_stru                   *pst_tid_queue;
#endif

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_TX_COMP_DMAC_START);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_COMP_DMAC_START);

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_event_handler::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????????????????????????????????????????? */
    pst_event         = (frw_event_stru *)pst_event_mem->puc_data;
    pst_tx_comp_event = (hal_tx_complete_event_stru *)pst_event->auc_event_data;
    pst_base_dscr     = pst_tx_comp_event->pst_base_dscr;
    pst_hal_device    = pst_tx_comp_event->pst_hal_device;
    uc_queue_num      = (hal_tx_queue_type_enum)pst_base_dscr->uc_q_num;

#ifdef _PRE_DEBUG_MODE
    /* ????queue_num????????*/
    if(OAL_UNLIKELY((uc_queue_num != pst_base_dscr->uc_q_num) || (uc_queue_num > HAL_TX_QUEUE_HI)))
    {
        OAM_ERROR_LOG3(0, OAM_SF_TX, "dmac_tx_complete_event_handler: report_queue_num=%d, dscr_queue_num=%d, dscr_addr=0x%x", uc_queue_num, pst_base_dscr->uc_q_num, pst_base_dscr);
        oal_mem_stop_rcd_rls();
        oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_base_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        return OAL_FAIL;
    }

    if(OAL_PTR_NULL == pst_base_dscr->pst_skb_start_addr)
    {
        return OAL_FAIL;
    }

    /* ??????????ppdu????????mpdu*/
    if (OAL_UNLIKELY(OAL_FALSE == pst_base_dscr->bit_is_first))
    {
        oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_base_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        OAM_WARNING_LOG1(0, OAM_SF_TX, "dmac_tx_complete_event_handler: is not the first mpud of ppdu, dscr_addr=0x%x", pst_base_dscr);
        oal_mem_stop_rcd_rls();
        /* OTA????????tx???????????????? */
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header))
        {
            pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_curr_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        }
        return OAL_FAIL;
    }

    /* ???????????????????????????? , ??????????????????????????*/
    pst_tx_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_base_dscr->pst_skb_start_addr);
    if (OAL_UNLIKELY(((OAL_TRUE == pst_base_dscr->bit_is_ampdu) && (MAC_GET_CB_MPDU_NUM(pst_tx_cb) != pst_tx_comp_event->uc_dscr_num))
         || ((OAL_FALSE == pst_base_dscr->bit_is_ampdu) && (1 != pst_tx_comp_event->uc_dscr_num))))
    {
        oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_base_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        OAM_WARNING_LOG4(0, OAM_SF_TX, "dmac_tx_complete_event_handler: tid=%d, irq_mpdu_num=%d, cb_mpdu_num=%d, dscr_addr=0x%x",
                              mac_get_cb_tid(pst_tx_cb), pst_tx_comp_event->uc_dscr_num, MAC_GET_CB_MPDU_NUM(pst_tx_cb), pst_base_dscr);
        oal_mem_stop_rcd_rls();
        /* OTA????????tx???????????????? */
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header))
        {
            pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_curr_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        }
    }

    /*????????????????????dev??????????*/
    OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header))
    {
        pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
        if (pst_curr_dscr == pst_base_dscr)
        {
            break;
        }
    }

    if (OAL_UNLIKELY(pst_dscr_entry == &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header)))
    {
        OAM_WARNING_LOG2(0, OAM_SF_TX, "dmac_tx_complete_event_handler: not find this dscr 0x%x in dev_tx_q(q_type =%d)", pst_base_dscr, uc_queue_num);
        oal_mem_stop_rcd_rls();
        oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_base_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        /* ????????tx?????????????????? */
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header))
        {
            pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            OAM_WARNING_LOG3(0, OAM_SF_TX, "{tx_q[%d] dscr::[0x%x].}", uc_queue_num, mac_get_cb_tid(pst_tx_cb), pst_curr_dscr);
        }
        /* ????????tid???????????????????? */
        pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_tx_cb));
        if (OAL_PTR_NULL == pst_dmac_user)
        {
            return OAL_FAIL;
        }

        pst_tid_queue = &(pst_dmac_user->ast_tx_tid_queue[mac_get_cb_tid(pst_tx_cb)]);
    #ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        /* ?????????????? */
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &pst_tid_queue->st_retry_q)
        {
            pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            OAM_WARNING_LOG4(0, OAM_SF_TX, "{retry Q info:user[%d]tid[%d] dscr::[0x%x] retry_mpdu[%d].}", MAC_GET_CB_TX_USER_IDX(pst_tx_cb), mac_get_cb_tid(pst_tx_cb), pst_curr_dscr, pst_tid_queue->uc_retry_num);
        }
        OAM_WARNING_LOG4(0, OAM_SF_TX, "{netbuf Q info:user[%d]tid[%d] netbuf num::[%d] mpdu[%d].}", MAC_GET_CB_TX_USER_IDX(pst_tx_cb), mac_get_cb_tid(pst_tx_cb), pst_tid_queue->st_buff_head.ul_num, pst_tid_queue->us_mpdu_num);

    #else
        OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &pst_tid_queue->st_hdr)
        {
            pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
            OAM_WARNING_LOG3(0, OAM_SF_TX, "{user[%d]tid[%d] dscr::[0x%x].}", MAC_GET_CB_TX_USER_IDX(pst_tx_cb), mac_get_cb_tid(pst_tx_cb), pst_curr_dscr);
        }
    #endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */
        return OAL_FAIL;
    }

    /*??????????????*/
    dmac_tx_complete_debug_dscr(pst_hal_device, pst_base_dscr);
#endif

#ifdef _PRE_DEBUG_MODE   /* ???? */
    if ((pst_base_dscr->uc_q_num < HAL_TX_QUEUE_HI))
    {
        g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_complete_bh1_num++;
        OAM_INFO_LOG1(0, OAM_SF_TX, "dmac_tx_complete_event_handler:ul_tx_complete_bh1_num = %d", g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_complete_bh1_num);
    }
#endif

    /* ??dev?????????????????????????????? ?????????????????????????????????????????????? */
    do
    {
        /*??????????????????????*/
        if (OAL_UNLIKELY(oal_dlist_is_empty(&(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header))))
        {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_tx_complete_event_handler:: TX Q(%d) IS EMPTY.}", uc_queue_num);
            break;
        }

        pst_curr_dscr = OAL_DLIST_GET_ENTRY(pst_hal_device->ast_tx_dscr_queue[uc_queue_num].st_header.pst_next, hal_tx_dscr_stru, st_entry);

#ifdef _PRE_DEBUG_MODE
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
        /* ??????????????????????alg???? ppdu number?????????????????????? */
        if(g_ul_desc_addr[uc_queue_num] == (oal_uint32)pst_curr_dscr)
        {
            OAM_WARNING_LOG3(0, OAM_SF_ANY, "{dmac_tx_complete_event_handler::BH after scheduler, noted queue[%d] descriptor address=%08x",
                uc_queue_num, &(pst_hal_device->ast_tx_dscr_queue[uc_queue_num]), pst_curr_dscr);
            g_ul_desc_addr[uc_queue_num] = 0;
            g_ul_desc_check_count[uc_queue_num] = 0;
        }
        else if(g_ul_desc_check_count[uc_queue_num]++ > 100)
        {
            g_ul_desc_check_count[uc_queue_num] = 0;
            g_ul_desc_addr[uc_queue_num] = 0;
        }
#endif

        /*??????????*/
        if (OAL_UNLIKELY(pst_curr_dscr != pst_base_dscr))
        {
            hal_tx_get_dscr_seq_num(pst_hal_device, pst_base_dscr, &us_report_seqnum);  /*??????????????seqnum*/
            hal_tx_get_dscr_seq_num(pst_hal_device, pst_curr_dscr, &us_curr_seqnum);      /*??????????????????seqnum*/
            OAM_WARNING_LOG2(0, OAM_SF_ANY, "{dmac_tx_complete_event_handler::hw_report_seqnum=%d, sw_current_seqnum=%d.}", us_report_seqnum, pst_base_dscr);
            oam_report_dscr(BROADCAST_MACADDR, (oal_uint8 *)pst_base_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
            oam_report_dscr(BROADCAST_MACADDR,(oal_uint8 *)pst_curr_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
        }
#endif
        ul_ret = dmac_tx_complete_buff(pst_hal_device, pst_curr_dscr);
        if (OAL_UNLIKELY(OAL_SUCC != ul_ret)) /*???????? ???????? ????????????????*/
        {
            OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_event_handler::dmac_tx_complete failed[%d].", ul_ret);
        }

    } while(pst_curr_dscr != pst_base_dscr) ;/*????????????????????????????????*/

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_TX_COMP_DMAC_END);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_COMP_DMAC_END);
#ifdef _PRE_WLAN_PROFLING_MIPS
    oal_profiling_stop_tx_save();
#endif

#ifdef _PRE_WLAN_PROFLING_MIPS
    if (g_mips_tx_statistic.uc_flag & BIT0)
    {
        return OAL_SUCC;
    }
#endif

    ul_ret = dmac_tx_complete_schedule(pst_hal_device, HAL_Q_NUM_TO_AC(uc_queue_num));

    return ul_ret;
}


oal_uint32  dmac_tx_complete_buff(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_dscr)
{
    oal_uint32          ul_ret = OAL_SUCC;

    /* ?????????????? */
    dmac_tx_complete_dump_dscr(pst_hal_device, pst_dscr);


    if ((OAL_TRUE == pst_dscr->bit_is_ampdu) || (OAL_TRUE == pst_dscr->bit_is_rifs))
    {
#ifdef _PRE_WLAN_FEATURE_AMPDU
        ul_ret = dmac_tx_complete_ampdu_buffer(pst_hal_device, pst_dscr);
#endif
    }
    else
    {
        ul_ret = dmac_tx_complete_normal_buffer(pst_hal_device, pst_dscr);
    }

    return ul_ret;
}



oal_void  dmac_tx_complete_free_dscr(hal_tx_dscr_stru *pst_dscr)
{
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dscr))
    {
         OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_free_dscr::dscr is NULL.}");
         return ;
    }

    /*????????????????????*/
    oal_dlist_delete_entry(&pst_dscr->st_entry);

    /*??????????????????netbuf*/
    if (OAL_PTR_NULL != pst_dscr->pst_skb_start_addr)
    {
        if (OAL_SUCC != dmac_tx_excp_free_netbuf(pst_dscr->pst_skb_start_addr))
        {
            OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_free_dscr::netbuff free fail, addr=0x%x.}", pst_dscr->pst_skb_start_addr);
        }
        pst_dscr->pst_skb_start_addr = OAL_PTR_NULL;
    }

    OAL_MEM_TRACE(pst_dscr, OAL_FALSE);

    /*??????????????*/
    if (OAL_SUCC != OAL_MEM_FREE(pst_dscr, OAL_TRUE))
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_free_dscr::tx dscr free fail, addr=0x%x.}", pst_dscr);
    }
}


oal_void  dmac_tx_complete_free_dscr_list(hal_tx_dscr_stru *pst_dscr, oal_uint8 uc_dscr_num)
{
    oal_uint8            uc_idx;
    hal_tx_dscr_stru    *pst_dscr_next;

    for (uc_idx = 0; uc_idx < uc_dscr_num; uc_idx++)
    {
        if (OAL_PTR_NULL == pst_dscr)
        {
           OAM_WARNING_LOG2(0, OAM_SF_TX, "{dmac_tx_complete_free_dscr_list::pst_buf null(dscr_num=%d, idx=%d). }", uc_dscr_num, uc_idx);
           break;
        }

        pst_dscr_next = OAL_DLIST_GET_ENTRY(pst_dscr->st_entry.pst_next, hal_tx_dscr_stru, st_entry);
        dmac_tx_complete_free_dscr(pst_dscr);
        pst_dscr = pst_dscr_next;
    }
}


OAL_STATIC OAL_INLINE oal_void  dmac_tx_set_retry_dscr(hal_to_dmac_device_stru *pst_hal_device,
                                                        hal_tx_dscr_stru *pst_dscr,
                                                        oal_uint8 uc_tx_status,
                                                        oal_uint8 uc_tx_chiper_type,
                                                        oal_uint8 uc_chiper_key_id)
{
    oal_uint32 ul_iv_ls_word = 0xFF;
    oal_uint32 ul_iv_ms_word = 0xFF;
#ifdef _PRE_WLAN_MAC_BUGFIX_PN
    oal_bool_enum_uint8      en_tx_pn_hw_bypass;
#endif

    pst_dscr->bit_is_retried = OAL_TRUE;
    hal_tx_set_dscr_status(pst_hal_device, pst_dscr, uc_tx_status);

#ifdef _PRE_WLAN_MAC_BUGFIX_PN
    hal_dscr_get_tx_pn_hw_bypass(pst_dscr, &en_tx_pn_hw_bypass);
    if (en_tx_pn_hw_bypass == OAL_TRUE)
    {
        pst_dscr->bit_is_first = OAL_FALSE;
        hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 1);
        dmac_tx_sw_restore_ccmp_PN_from_mac_hdr(pst_hal_device, pst_dscr);
    }
    else
#endif  /* _PRE_WLAN_MAC_BUGFIX_PN */
    {
        if ((uc_tx_status == DMAC_TX_TIMEOUT)
           || (uc_tx_status == DMAC_TX_RTS_FAIL)
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
           || (uc_tx_status == DMAC_TX_RTS_FAIL_CTS_ERROR)
#endif
           || (uc_tx_status == DMAC_TX_SOFT_PSM_BACK))
        {
            if (OAL_TRUE == pst_dscr->bit_is_first)
            {
                hal_tx_get_dscr_iv_word(pst_dscr, &ul_iv_ms_word, &ul_iv_ls_word, uc_tx_chiper_type, uc_chiper_key_id);
                pst_dscr->bit_is_first = OAL_FALSE;
                hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 0);
                hal_tx_set_dscr_status(pst_hal_device, pst_dscr, DMAC_TX_INVALID);
                return;
            }
            else
            {
                hal_tx_get_dscr_iv_word(pst_dscr, &ul_iv_ms_word, &ul_iv_ls_word, uc_tx_chiper_type, uc_chiper_key_id);
                if (0 == ul_iv_ls_word)
                {
                    hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 0);
                }
                else
                {
                    hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 1);
                }
            }
        }
        else if ((uc_tx_status == DMAC_TX_KEY_SEARCH_FAIL)
           || (uc_tx_status == DMAC_TX_AMPDU_MISMATCH)
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
           || (uc_tx_status == DMAC_TX_FAIL_ABORT)
           || (uc_tx_status == DMAC_TX_FAIL_STATEMACHINE_PHY_ERROR)
#endif
           || (uc_tx_status == DMAC_TX_PENDING))
        {
            pst_dscr->bit_is_first = OAL_FALSE;
            hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 0);
            hal_tx_set_dscr_status(pst_hal_device, pst_dscr, DMAC_TX_INVALID);
            return;
        }
        else
        {
            hal_tx_get_dscr_iv_word(pst_dscr, &ul_iv_ms_word, &ul_iv_ls_word, uc_tx_chiper_type, uc_chiper_key_id);
            pst_dscr->bit_is_first = OAL_FALSE;
            if (0 == ul_iv_ls_word)
            {
                hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 0);
            }
            else
            {
                hal_tx_set_dscr_seqno_sw_generate(pst_hal_device, pst_dscr, 1);
            }

        }
    }

    hal_tx_set_dscr_status(pst_hal_device, pst_dscr, DMAC_TX_INVALID);
}

OAL_STATIC OAL_INLINE oal_uint32 dmac_tx_complete_dscr_back(hal_to_dmac_device_stru *pst_hal_device,
                                                                       dmac_vap_stru           *pst_dmac_vap,
                                                                       dmac_tid_stru           *pst_tid_queue,
                                                                       oal_dlist_head_stru     *pst_pending_q,
                                                                       oal_uint8                uc_retry_num)
{
    oal_uint32 ul_ret = OAL_SUCC;
    oal_uint16 us_seq_num = 0;
    oal_dlist_head_stru *pst_dscr_entry = OAL_PTR_NULL;
    hal_tx_dscr_stru    *pst_tx_dscr    = OAL_PTR_NULL;

    if (oal_dlist_is_empty(pst_pending_q))
    {
        return OAL_SUCC;
    }


    ul_ret = dmac_tid_tx_queue_enqueue_head(pst_tid_queue, pst_pending_q, uc_retry_num);
    if (OAL_SUCC == ul_ret)
    {
        pst_tid_queue->uc_retry_num += uc_retry_num;
        dmac_alg_tid_update_notify(pst_tid_queue);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    #ifdef _PRE_WLAN_DFT_STAT
        if (OAL_PTR_NULL != pst_tid_queue->pst_tid_stats)
        {
            DMAC_TID_STATS_INCR(pst_tid_queue->pst_tid_stats->ul_tid_retry_enqueue_cnt, uc_retry_num);
        }
    #endif
#endif
    }
    else
    {
        /*amdpu??????????????tid??????????????????????????????*/
        if ((DMAC_TX_MODE_NORMAL != pst_tid_queue->en_tx_mode) && (OAL_PTR_NULL != pst_tid_queue->pst_ba_tx_hdl))
        {
            OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, pst_pending_q)
            {
                pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);
                hal_tx_get_dscr_seq_num(pst_hal_device, pst_tx_dscr, &us_seq_num);
                dmac_ba_update_baw(pst_tid_queue->pst_ba_tx_hdl, us_seq_num);
            }
        }
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        pst_dmac_vap->st_vap_base_info.st_vap_stats.ul_tx_dropped_packets += uc_retry_num;
#endif
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::dmac_tid_tx_queue_enqueue failed[%d].", ul_ret);

        dmac_tx_excp_free_dscr(pst_pending_q, pst_hal_device);
    }

    return OAL_SUCC;
}


oal_uint8 dmac_tx_comp_sw_retry_check(oal_netbuf_stru *pst_buf, oal_uint8 uc_sw_retry_limit)
{
    mac_tx_ctl_stru                    *pst_cb;
    mac_ieee80211_frame_stru           *pst_mac_hdr;
    oal_uint8                           uc_frame_sub_type;
    oal_uint8                           uc_is_mgmt;

    if (OAL_PTR_NULL == pst_buf)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_comp_sw_retry_check: pst_buf is null!}");

        return OAL_FALSE;
    }

    pst_cb      = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    pst_mac_hdr = mac_get_cb_frame_hdr(pst_cb);
    if (OAL_PTR_NULL == pst_mac_hdr)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_comp_sw_retry_check: pst_mac_hdr is null!}");

        return OAL_FALSE;
    }

    uc_frame_sub_type = mac_get_frame_sub_type((oal_uint8 *)pst_mac_hdr);
    switch (uc_frame_sub_type)
    {
        /* ?????????????? */
        case WLAN_FC0_SUBTYPE_AUTH:
        case WLAN_FC0_SUBTYPE_ASSOC_REQ:
        case WLAN_FC0_SUBTYPE_ASSOC_RSP:
        case WLAN_FC0_SUBTYPE_REASSOC_REQ:
        case WLAN_FC0_SUBTYPE_REASSOC_RSP:
        case WLAN_FC0_SUBTYPE_ACTION:
            uc_is_mgmt = OAL_TRUE;
            break;

        default:
            uc_is_mgmt = OAL_FALSE;
            break;
    }

    if ((OAL_TRUE == uc_is_mgmt) && ((MAC_GET_CB_RETRIED_NUM(pst_cb)) < uc_sw_retry_limit))
    {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


oal_void dmac_tx_sw_retry(dmac_vap_stru *pst_dmac_vap, oal_netbuf_stru *pst_buf, hal_tx_dscr_stru *pst_dscr)
{
    oal_uint16        us_original_mpdu_len = 0;

    if ((OAL_PTR_NULL == pst_dmac_vap) || (OAL_PTR_NULL == pst_buf) || (OAL_PTR_NULL == pst_dscr))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_sw_retry: null ptr!!!}");
        return;
    }

    us_original_mpdu_len = pst_dscr->us_original_mpdu_len;

    /* ??????????????????net_buff */
    oal_dlist_delete_entry(&pst_dscr->st_entry);
    if (OAL_SUCC != OAL_MEM_FREE(pst_dscr, OAL_TRUE))
    {
        OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_sw_retry::tx dscr free fail, addr=0x%x.}", pst_dscr);
    }

    /* ???????????????????? */
    dmac_tx_mgmt(pst_dmac_vap, pst_buf, us_original_mpdu_len);
}



OAL_STATIC oal_uint8  dmac_tx_check_softretry(
                hal_to_dmac_device_stru     *pst_hal_device,
                dmac_user_stru              *pst_dmac_user,
                hal_tx_dscr_stru            *pst_dscr,
                oal_netbuf_stru             *pst_buf,
                hal_tx_dscr_ctrl_one_param  *pst_tx_dscr_one)
{
    dmac_vap_stru                      *pst_dmac_vap;
    dmac_tid_stru                      *pst_tid_queue;
    oal_dlist_head_stru                 st_pending_q;
    mac_tx_ctl_stru                    *pst_cb;
    oal_uint16                          us_tx_user_idx;                             /* dmac tx ?? tx complete ??????user???????????????????? */
    oal_uint8                           uc_dscr_status;
    oal_uint8                           uc_dscr_chiper_type;
    oal_uint8                           uc_dscr_chiper_key_id;
#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1151)
    //oal_uint8                           uc_frame_type;
    oal_uint8                           uc_subtype;
#endif
    mac_ieee80211_frame_stru           *pst_mac_hdr;

    pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_dmac_user->st_user_base_info.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG2(0, OAM_SF_TX, "{dmac_tx_check_softretry: pst_dmac_vap is null, vap id[%d], user assoc id[%d].}",
                       pst_dmac_user->st_user_base_info.uc_vap_id,
                       pst_dmac_user->st_user_base_info.us_assoc_id);
        return OAL_FALSE;
    }

    pst_mac_hdr = mac_get_cb_frame_hdr(pst_cb);
    if (OAL_PTR_NULL == pst_mac_hdr)
    {
        OAM_ERROR_LOG0(pst_dmac_user->st_user_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_check_softretry::pst_mac_header null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_CACHE_COHERENT_SUPPORT
    /* ??cache??????mac?????????????? */
    oal_dma_map_single(NULL, pst_mac_hdr, 24, OAL_FROM_DEVICE);
#endif

#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1151)
    /* ?????????? */
    //uc_frame_type   = mac_frame_get_type_value((oal_uint8 *)pst_mac_hdr);
    uc_subtype      = mac_get_frame_sub_type((oal_uint8 *)pst_mac_hdr);

    /* NULL???????????? */
    if ((WLAN_FC0_SUBTYPE_NODATA | WLAN_FC0_TYPE_DATA) == uc_subtype)
    {
        /* NULL???????????????? */
        return OAL_FALSE;
    }
#endif

    /* ?????????????? */
    hal_tx_get_dscr_status(pst_hal_device, pst_dscr, &uc_dscr_status);
#if 0
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    if (OAL_TRUE == dmac_tx_comp_sw_retry_check(pst_buf, DMAC_MGMT_MAX_SW_RETRIES))
#else
    if ((OAL_TRUE == dmac_tx_comp_sw_retry_check(pst_buf, DMAC_MGMT_MAX_SW_RETRIES))
      || ((DMAC_TX_SOFT_PSM_BACK == uc_dscr_status) && (OAL_TRUE == (oal_bool_enum)IS_P2P_PS_ENABLED(pst_dmac_vap)) && (WLAN_DATA_BASICTYPE != uc_frame_type)))
#endif
#endif
    /* ?????????????????????????????????? ???? ??????????????????????;??????????TID???? */
    if (OAL_TRUE == dmac_tx_comp_sw_retry_check(pst_buf, DMAC_MGMT_MAX_SW_RETRIES))
    {
        /* ??????????????net_buf???????????????? */
        MAC_GET_CB_RETRIED_NUM(pst_cb)++;
        dmac_tx_sw_retry(pst_dmac_vap, pst_buf, pst_dscr);

        return OAL_TRUE;
    }
#if 0
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    if (OAL_TRUE == MAC_GET_CB_IS_NEEDRETRY(pst_cb))
#else
    /* ????????????????:1)??????????TID????;2)???????????????????? */
    if ((OAL_TRUE == MAC_GET_CB_IS_NEEDRETRY(pst_cb))
      || ((DMAC_TX_SOFT_PSM_BACK == uc_dscr_status) && (OAL_TRUE == (oal_bool_enum)IS_P2P_PS_ENABLED(pst_dmac_vap)) && (WLAN_DATA_BASICTYPE == uc_frame_type)))
#endif
#endif
    if (OAL_TRUE == MAC_GET_CB_IS_NEEDRETRY(pst_cb))
    {
        if ((MAC_GET_CB_RETRIED_NUM(pst_cb) < pst_dmac_vap->uc_sw_retry_limit)
            && (DMAC_USER_ALG_SMARTANT_NULLDATA_PROBE != MAC_GET_CB_IS_PROBE_DATA(pst_cb)))//????????????????????
        {
            us_tx_user_idx = MAC_GET_CB_TX_USER_IDX(pst_cb);
            /* ?????????????????????????????????? */
            if(OAL_SUCC != mac_vap_find_user_by_macaddr(&(pst_dmac_vap->st_vap_base_info), pst_dmac_user->st_user_base_info.auc_user_mac_addr, &us_tx_user_idx))
            {
                OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                                 "{dmac_tx_check_softretry::user [%02X:XX:XX:%02X:%02X:%02X] not exist.}",
                                 pst_dmac_user->st_user_base_info.auc_user_mac_addr[0],
                                 pst_dmac_user->st_user_base_info.auc_user_mac_addr[3],
                                 pst_dmac_user->st_user_base_info.auc_user_mac_addr[4],
                                 pst_dmac_user->st_user_base_info.auc_user_mac_addr[5]);

                dmac_tx_complete_free_dscr(pst_dscr);

                return OAL_SUCC;
            }

            dmac_tx_update_alg_param(pst_dscr, pst_buf, pst_tx_dscr_one, pst_hal_device);
            if (OAL_FALSE == MAC_GET_CB_IS_VIPFRAME(pst_cb))
            {
                dmac_alg_tx_complete_notify(&(pst_dmac_user->st_user_base_info), pst_buf, pst_tx_dscr_one);

                /* ??????????????ba??????????????????true */
                if (OAL_TRUE == pst_dmac_user->en_delete_ba_flag)
                {
                    pst_dmac_user->en_delete_ba_flag = OAL_FALSE;

                    /* ????????HMAC???????????? */
                    dmac_tx_delete_ba(pst_dmac_user);
                }
            }

            /* ???????????????? ???????? */
            MAC_GET_CB_RETRIED_NUM(pst_cb)++;
            oal_dlist_init_head(&st_pending_q);
            hal_tx_get_dscr_status(pst_hal_device, pst_dscr, &uc_dscr_status);
            hal_tx_get_dscr_chiper_type(pst_dscr, &uc_dscr_chiper_type, &uc_dscr_chiper_key_id);
            dmac_tx_set_retry_dscr(pst_hal_device, pst_dscr, uc_dscr_status, uc_dscr_chiper_type, uc_dscr_chiper_key_id);
            oal_dlist_delete_entry(&pst_dscr->st_entry);
            oal_dlist_add_tail(&pst_dscr->st_entry, &st_pending_q);
            pst_tid_queue = &(pst_dmac_user->ast_tx_tid_queue[mac_get_cb_tid(pst_cb)]);
            /* ??????????????????????????tid???? */
            dmac_tx_complete_dscr_back(pst_hal_device, pst_dmac_vap, pst_tid_queue, &st_pending_q, 1);

            return OAL_TRUE;
        }
    }

    return OAL_FALSE;

}
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif


OAL_STATIC OAL_INLINE oal_uint32  dmac_al_tx_free_long_frame(hal_tx_dscr_stru   *pst_dscr, oal_netbuf_stru *pst_buf)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    mac_tx_ctl_stru   *pst_tx_ctl;
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    //OAM_INFO_LOG1(0, OAM_SF_TX, "\n zzm--MAC_GET_CB_MPDU_BYTES(pst_tx_ctl) %u", MAC_GET_CB_MPDU_BYTES(pst_tx_ctl));
    //if(WLAN_MEM_NETBUF_SIZE3 < MAC_GET_CB_MPDU_LEN(pst_tx_ctl))
    if(WLAN_MEM_NETBUF_SIZE3 < MAC_GET_CB_MPDU_BYTES(pst_tx_ctl))
    {
        //OAM_INFO_LOG1(0, OAM_SF_TX, "MAC_GET_CB_MPDU_BYTES(pst_tx_ctl) %u", MAC_GET_CB_MPDU_BYTES(pst_tx_ctl));
        oal_dlist_delete_entry(&pst_dscr->st_entry);
        OAL_MEM_FREE(pst_dscr, OAL_TRUE);
        OAL_MEM_MULTI_NETBUF_FREE(pst_buf);
        return OAL_SUCC;
    }
#endif
#endif
    return OAL_FAIL;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#if 0

OAL_STATIC oal_uint32  dmac_psm_enqueue_psm_back_netbuf_tx_complete(
    hal_to_dmac_device_stru *pst_hal_device,
    hal_tx_dscr_stru  *pst_tx_dscr,
    dmac_vap_stru *pst_dmac_vap,
    dmac_user_stru *pst_dmac_user,
    oal_netbuf_stru *pst_net_buf)
{
    hal_tx_queue_type_enum_uint8 en_queue_num;

    if ((OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
        || (OAL_UNLIKELY(OAL_PTR_NULL == pst_tx_dscr))
        || (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
        || (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
        || (OAL_UNLIKELY(OAL_PTR_NULL == pst_net_buf))
        )
    {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{dmac_psm_enqueue_psm_back_netbuf_tx_complete::ptr is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_queue_num = pst_tx_dscr->uc_q_num;


    /* ?????????? */
    oal_dlist_delete_entry(&pst_tx_dscr->st_entry);
    OAL_MEM_FREE(pst_tx_dscr, OAL_TRUE);
    /* ????????????????????????????????enqueue???????????????????????????????????? */

#ifdef _PRE_WLAN_FEATURE_UAPSD
    if((pst_dmac_user->uc_uapsd_flag) & MAC_USR_UAPSD_EN)
    {
        dmac_uapsd_tx_enqueue(pst_dmac_vap, pst_dmac_user, pst_net_buf);
    }
    else
#endif
    {
        dmac_psm_enqueue(pst_dmac_vap, pst_dmac_user, pst_net_buf);
    }
    /* ??????????????ppdu??????????ppducnt??1 */
    pst_hal_device->ast_tx_dscr_queue[en_queue_num].uc_ppdu_cnt
        = OAL_SUB(pst_hal_device->ast_tx_dscr_queue[en_queue_num].uc_ppdu_cnt, 1);

    return OAL_SUCC;

}
#endif
#endif


#ifdef _PRE_WLAN_DFT_STAT
static oal_void dmac_performance_show_mpdu_info(oal_uint8 uc_vap_id,oal_uint8 uc_dscr_status,hal_tx_dscr_ctrl_one_param *pst_tx_dscr_one)
{
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    if(DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(DFX_PERFORMANCE_TX_LOG))
    {/*lint -e437*/
        OAM_WARNING_LOG_ALTER(uc_vap_id, OAM_SF_TX,
                            "{performance debug::uc_dscr_status[%d],mpdu_num[%d],mpdu_len[%d],ac[%d],bw[%d],rate[%d],long_retry[%d],cts_fail[%d].}",
                            8,
                            uc_dscr_status,
                            pst_tx_dscr_one->uc_mpdu_num,
                            pst_tx_dscr_one->us_mpdu_len,
                            pst_tx_dscr_one->uc_ac,
                            pst_tx_dscr_one->uc_bandwidth,
                            pst_tx_dscr_one->ast_per_rate[0],
                            pst_tx_dscr_one->uc_long_retry,
                            pst_tx_dscr_one->uc_cts_failure);/*lint +e437*/
    }
#endif
}
#else
#define dmac_performance_show_mpdu_info(uc_vap_id,uc_dscr_status,pst_tx_dscr_one)
#endif

#ifdef _PRE_WLAN_DFT_STAT
static oal_void dmac_successive_fail_dump_info(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, hal_tx_dscr_stru *pst_dscr)
{
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_to_dmac_device_stru *pst_hal_device;
    mac_device_stru              *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru             *pst_mac_vap = OAL_PTR_NULL;

    if(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num <= DMAC_MAX_TX_SUCCESSIVE_FAIL_PRINT_THRESHOLD)
    {
        return;
    }
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CCA_OPT, "{dmac_successive_fail_dump_info:pst_mac_device == NULL}");
        return;
    }
    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CCA_OPT, "{dmac_successive_fail_dump_info:pst_hal_device == NULL}");
        return;
    }
    pst_mac_vap = &pst_dmac_vap->st_vap_base_info;
    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CCA_OPT, "{dmac_successive_fail_dump_info:pst_mac_vap == NULL}");
        return;
    }
    if((1 == pst_hal_device->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_on) &&
        (pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num <= DMAC_MAX_TX_SUCCESSIVE_FAIL_PRINT_THRESHOLD_BTCOEX))
    {
        return;
    }
    oam_report_dscr(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                     (oal_uint8 *)pst_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
    dmac_dft_report_all_ota_state(pst_mac_vap);
    DMAC_VAP_DFT_STATS_PKT_SET_ZERO(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num);
#endif
#endif
}
#else
#define dmac_successive_fail_dump_info(pst_dmac_vap)
#endif

#ifdef _PRE_WLAN_FEATURE_VOWIFI

oal_void dmac_update_vowifi_tx_cnt(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user)
{

    /* ????????pst_dmac_vap??pst_dmac_user??????!  */
    if (OAL_PTR_NULL == pst_dmac_vap->pst_vowifi_status)
    {
        return;
    }

    pst_dmac_vap->pst_vowifi_status->ul_tx_total  = pst_dmac_user->st_query_stats.ul_tx_total;
    pst_dmac_vap->pst_vowifi_status->ul_tx_failed = pst_dmac_user->st_query_stats.ul_tx_failed;

}

#endif //_PRE_WLAN_FEATURE_VOWIFI

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif


OAL_STATIC oal_uint32  dmac_tx_complete_normal_buffer(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_dscr)
{
    mac_tx_ctl_stru                    *pst_cb;
    dmac_user_stru                     *pst_dmac_user;
    hal_tx_dscr_ctrl_one_param          st_tx_dscr_one = {0};
    oal_uint8                           uc_dscr_status = DMAC_TX_INVALID;
    oal_uint32                          ul_seq    = 0;
    oal_uint8                           uc_data_tx_cnt = 0;
    oal_uint8                           uc_is_qos;
    oal_uint16                          us_user_idx      = 0xffff;
    mac_device_stru                    *pst_mac_device;
    dmac_vap_stru                      *pst_dmac_vap;
    mac_vap_stru                       *pst_mac_vap;
    oal_uint32                          ul_ret;
    oal_uint8                           uc_q_num;
    oal_netbuf_stru                    *pst_buf;
    oal_uint8                           uc_vap_id;
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    oal_uint8                           uc_tid;
#else
    oal_uint8                           uc_data_tx_rate0 = 0;
#endif /* (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151) */
#ifdef _PRE_WLAN_PERFORM_STAT
    oal_uint8                           uc_hw_retry_num    = 0;
    oal_uint8                           uc_rate_index      = 0;
#endif /* _PRE_WLAN_PERFORM_STAT */

#if defined(_PRE_WLAN_FEATURE_DBAC) && defined(_PRE_PRODUCT_ID_HI110X_DEV)
    mac_ieee80211_frame_stru           *pst_mac_header;
    oal_uint8                           uc_mgmt_subtype;
    oal_uint8                           uc_mgmt_type;
#endif /* defined(_PRE_WLAN_FEATURE_DBAC) && defined(_PRE_PRODUCT_ID_HI110X_DEV) */

    uc_q_num = pst_dscr->uc_q_num;

    pst_buf = pst_dscr->pst_skb_start_addr;
    if (OAL_PTR_NULL == pst_buf)
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::pst_buf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    dmac_tx_get_vap_id(pst_hal_device, pst_dscr, &uc_vap_id);

    pst_cb   = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    uc_tid   = mac_get_cb_tid(pst_cb);
#endif
    pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt
                = OAL_SUB(pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt, 1);

    OAL_MEM_TRACE(pst_dscr, OAL_FALSE);

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::pst_mac_device null. device_id=%d.}", pst_hal_device->uc_mac_device_id);
        dmac_tx_complete_free_dscr(pst_dscr);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_DFT_STAT
    /* ???????????????????????? */
    if (HAL_TX_QUEUE_HI == uc_q_num)
    {
        dmac_dft_mgmt_stat_incr(pst_mac_device, oal_netbuf_header(pst_buf), MAC_DEV_MGMT_STAT_TYPE_TX_COMPLETE);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    /* ????02????????????????multi_free?????????? */
    if(OAL_SUCC == dmac_al_tx_free_long_frame(pst_dscr, pst_buf))
    {
        return OAL_SUCC;
    }
#endif

    if (OAL_UNLIKELY(0 == uc_vap_id))
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::vap id is 0, mac vap had been deleted.}");

        dmac_tx_complete_free_dscr(pst_dscr);
        return OAL_SUCC;
    }

    pst_dmac_vap = mac_res_get_dmac_vap(uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        dmac_tx_complete_free_dscr(pst_dscr);
        return OAL_SUCC;
    }
    pst_mac_vap = &(pst_dmac_vap->st_vap_base_info);

    /* ?????????????? */
    hal_tx_get_dscr_status(pst_hal_device, pst_dscr, &uc_dscr_status);

#if defined(_PRE_WLAN_FEATURE_DBAC) && defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_mac_header = (mac_ieee80211_frame_stru *)oal_netbuf_header(pst_buf);
    uc_mgmt_type    = mac_frame_get_type_value((oal_uint8 *)pst_mac_header);
    uc_mgmt_subtype = mac_frame_get_subtype_value((oal_uint8 *)pst_mac_header);

    if (WLAN_MANAGEMENT == uc_mgmt_type && WLAN_DISASOC == uc_mgmt_subtype)
    {
        dmac_alg_dbac_resume(pst_mac_device);
    }
#endif

    /* ????DMAC?????????????? */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_cb));
    if (OAL_PTR_NULL == pst_dmac_user)
    {
        /* ????cb???? bit_need_rsp, ????hmac ????????*/
        if (pst_cb->bit_need_rsp == OAL_TRUE)
        {
            /* mgmt tx ???????? */
            if (uc_dscr_status != DMAC_TX_SUCC)
            {
                dmac_mgmt_tx_complete(&(pst_dmac_vap->st_vap_base_info), pst_cb->bit_mgmt_frame_id, OAL_FAIL, (oal_uint8)MAC_GET_CB_TX_USER_IDX(pst_cb));
            }
            else
            {
                dmac_mgmt_tx_complete(&(pst_dmac_vap->st_vap_base_info), pst_cb->bit_mgmt_frame_id, OAL_SUCC, (oal_uint8)MAC_GET_CB_TX_USER_IDX(pst_cb));
            }
        }

        dmac_tx_complete_free_dscr(pst_dscr);
        return OAL_SUCC;
    }

    /* ??mac??????????????????????????????????????mac???????????????????? */
    /* ????????idx????f??????????????????????????idx??cb?????????????????????????????????????????????????????????????????????? */
    if (MAC_GET_CB_TX_USER_IDX(pst_cb) != pst_mac_vap->us_multi_user_idx)
    {
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, pst_dmac_user->st_user_base_info.auc_user_mac_addr, &us_user_idx);
        if((OAL_SUCC != ul_ret) || (0xffff == us_user_idx) || (us_user_idx != MAC_GET_CB_TX_USER_IDX(pst_cb)))
        {
            dmac_tx_complete_free_dscr(pst_dscr);
            return OAL_SUCC;
        }
    }

#ifdef _PRE_DEBUG_MODE_USER_TRACK
    dmac_tx_complete_check_protocol(pst_hal_device, pst_dmac_user, pst_dscr);
#endif

    if (uc_dscr_status != DMAC_TX_SUCC)
    {
        /* ???????????????????????? */
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num, 1);
        dmac_successive_fail_dump_info(pst_dmac_vap, pst_dmac_user, pst_dscr);
        /* ????????????TID???????????????????????? */
        uc_is_qos = mac_get_cb_is_qosdata(pst_cb);
        if( OAL_TRUE == uc_is_qos)
        {
            hal_tx_get_dscr_seq_num(pst_hal_device, pst_dscr, (oal_uint16 *)&ul_seq);
        }
        else
        {
            hal_get_tx_sequence_num(pst_hal_device, 0, 0, 0, &ul_seq);
        }
#if 0
#ifdef _PRE_WLAN_FEATURE_P2P
        if (!(OAL_TRUE == pst_mac_device->st_p2p_info.en_p2p_ps_pause && DMAC_TX_SOFT_PSM_BACK == uc_dscr_status))
#endif
#endif
        {
            hal_tx_get_dscr_tx_cnt(pst_hal_device, pst_dscr, &uc_data_tx_cnt);
#if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1151)
            OAM_WARNING_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::tx dscr status=%d,tx_cnt=%d,ul_seq=%d, isqos=%d.}",
                                     uc_dscr_status,uc_data_tx_cnt, ul_seq, uc_is_qos);
#else
            hal_tx_dscr_get_rate3(pst_hal_device, pst_dscr, &uc_data_tx_rate0);
            OAM_WARNING_LOG_ALTER(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::tx dscr status=%d,tx_cnt=%d,ul_seq=%d, isqos=%d, rate3=%d Mbps/mcs.}", 5,
                                  uc_dscr_status, uc_data_tx_cnt, ul_seq, uc_is_qos, uc_data_tx_rate0);
#endif /* #if (_PRE_PRODUCT_ID ==_PRE_PRODUCT_ID_HI1151) */

            OAM_WARNING_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::cb user idx=%d,user idx=%d, vap multi useridx=%d, vap id=%d.}",
                             MAC_GET_CB_TX_USER_IDX(pst_cb),
                             us_user_idx,
                             pst_mac_vap->us_multi_user_idx,
                             pst_dmac_user->st_user_base_info.uc_vap_id);
        }
        st_tx_dscr_one.uc_error_mpdu_num = 1;
        OAM_STAT_VAP_INCR(uc_vap_id, tx_mpdu_fail_num, 1);
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_mpdu_fail_num, 1);

        #if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id,
                          uc_tid, tx_mpdu_fail_num, 1);
        #endif

        /* ?????????????????????????????????????????????????????????????????? */
        ul_ret = dmac_tx_check_softretry(pst_hal_device, pst_dmac_user, pst_dscr, pst_buf, &st_tx_dscr_one);
        /* ???????????????????? */
        if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
        {
            oal_uint8 uc_retries;

            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_failed,((OAL_TRUE == ul_ret)? 0:1 ));
            DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_failed, ((OAL_TRUE == ul_ret)? 0:1 ));
            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_long_retry);
            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_short_retry);
            DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_long_retry);
            DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_short_retry);
            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_failed,1);

            uc_retries = st_tx_dscr_one.uc_long_retry + st_tx_dscr_one.uc_short_retry;
            if (uc_retries > 0)
            {
                uc_retries -= 1;    /* ??????????????1?? */
            }

            OAM_STAT_USER_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, tx_ppdu_retries, uc_retries);
        }
#ifdef _PRE_WLAN_DFT_STAT
        DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_cts_fail,st_tx_dscr_one.uc_cts_failure);
#endif
        if (OAL_TRUE == ul_ret)
        {
            pst_dmac_user->st_ps_structure.en_is_pspoll_rsp_processing = OAL_FALSE;
            return OAL_SUCC;
        }

    }
    else
    {
        /* ??????????????MPDU???? */
        DMAC_VAP_DFT_STATS_PKT_SET_ZERO(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num);
        OAM_STAT_VAP_INCR(uc_vap_id, tx_mpdu_succ_num, 1);
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_mpdu_succ_num, 1);
        #if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_mpdu_succ_num, 1);
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_mpdu_bytes, MAC_GET_CB_MPDU_BYTES(pst_cb));
        #endif


        /* ???????????????????? */
        if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
        {
            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_pkts,MAC_GET_CB_NETBUF_NUM(pst_cb));
            DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_bytes,MAC_GET_CB_MPDU_BYTES(pst_cb));
            DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_hw_tx_pkts, MAC_GET_CB_NETBUF_NUM(pst_cb));
            DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_hw_tx_bytes, MAC_GET_CB_MPDU_BYTES(pst_cb));
      #ifdef _PRE_WLAN_FEATURE_VOWIFI
            dmac_update_vowifi_tx_cnt(pst_dmac_vap, pst_dmac_user);
      #endif
        }

        /* ???????? ????linkloss???? */
        if (MAC_VAP_STATE_UP == pst_dmac_vap->st_vap_base_info.en_vap_state)
        {
            dmac_vap_linkloss_clean(pst_dmac_vap);
        }

    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_COMP_GET_DSCR_STAT);

    /* ????keepalive?????? */
    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        dmac_keepalive_timestamp_update(pst_dmac_user, OAL_NETBUF_HEADER(pst_buf), uc_dscr_status);
    }
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    /*sta keepalive ??????????02??????????*/
    else
    {
        dmac_keepalive_timestamp_update(pst_dmac_user, OAL_NETBUF_HEADER(pst_buf), uc_dscr_status);
    }
#endif

    /* ????mib?????? */
    dmac_tx_update_alg_param(pst_dscr, pst_buf, &st_tx_dscr_one, pst_hal_device);

    dmac_performance_show_mpdu_info(uc_vap_id, uc_dscr_status, &st_tx_dscr_one);

#ifdef _PRE_WLAN_FEATURE_UAPSD
    /*UAPSD ????EOSP????????????USP*/
    dmac_uapsd_tx_complete(pst_dmac_user,pst_cb);
#endif

    /* STA??pspoll?????????? */
#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        dmac_psm_tx_complete_sta(pst_dmac_vap, pst_dscr, pst_buf);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_DFS
#if defined (_PRE_WLAN_FEATURE_20_40_80_COEXIST) || defined(_PRE_WLAN_FEATURE_DFS)
    dmac_chan_tx_complete_suspend_tx(pst_mac_device, pst_mac_vap, pst_hal_device, pst_buf);
#endif

#else

#ifdef _PRE_WLAN_FEATURE_20_40_80_COEXIST
    dmac_chan_tx_complete_2040_coexist(pst_mac_device, pst_hal_device, pst_buf);
#endif

#endif /* end of _PRE_WLAN_FEATURE_DFS */

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_COMP_PROCESS_FEATURE);

    /* ??ps_poll??????????????????ps-poll??????????????????????????ps-poll???????? */
    pst_dmac_user->st_ps_structure.en_is_pspoll_rsp_processing = OAL_FALSE;

    /* ???????????????????????? */
    dmac_tx_complete_mgmt_notify(pst_hal_device, pst_dmac_user, pst_dscr, pst_buf);

    if (OAL_FALSE == MAC_GET_CB_IS_VIPFRAME(pst_cb))
    {
        dmac_alg_tx_complete_notify(&(pst_dmac_user->st_user_base_info), pst_buf, &st_tx_dscr_one);

        /* ??????????????ba??????????????????true */
        if (OAL_TRUE == pst_dmac_user->en_delete_ba_flag)
        {
            pst_dmac_user->en_delete_ba_flag = OAL_FALSE;

            /* ????????HMAC???????????? */
            dmac_tx_delete_ba(pst_dmac_user);
        }
    }
    else if((pst_dmac_user->bit_is_rx_eapol_key_open == OAL_FALSE)
            && (OAL_TRUE == mac_is_eapol_key_ptk_4_4(pst_buf)))
    {
        /* ????????????????EAPOL-KEY, ??4/4 EAPOL-KEY ???????????????????? */
        pst_dmac_user->bit_eapol_key_4_4_tx_succ = OAL_TRUE;
        if (pst_dmac_user->bit_ptk_need_install == OAL_TRUE)
        {
            /* ???????????? */
            ul_ret = dmac_config_11i_add_key_set_reg(&(pst_dmac_vap->st_vap_base_info),
                                                    pst_dmac_user->bit_ptk_key_idx,
                                                    pst_dmac_user->st_user_base_info.auc_user_mac_addr);
            OAM_WARNING_LOG4(0, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::set ptk succ.ret %d, key_idx %d. %02X:XX:XX:XX:XX:%02X",
                    ul_ret,
                    pst_dmac_user->bit_ptk_key_idx,
                    pst_dmac_user->st_user_base_info.auc_user_mac_addr[0],
                    pst_dmac_user->st_user_base_info.auc_user_mac_addr[5]);

            pst_dmac_user->bit_ptk_need_install      = OAL_FALSE;
            pst_dmac_user->bit_eapol_key_4_4_tx_succ = OAL_FALSE;
            pst_dmac_user->bit_ptk_key_idx           = 0;
        }
    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_TX_COMP_MGMT_NOTIFY);

#ifdef _PRE_WLAN_PERFORM_STAT
    dmac_stat_tx_thrpt(pst_dmac_user, st_tx_dscr_one.uc_tid ,st_tx_dscr_one);

    if (DMAC_TX_SUCC != uc_dscr_status)
    {
        dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, 0, 1, DMAC_STAT_PER_SW_RETRY_MPDU);
    }

    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, st_tx_dscr_one.uc_mpdu_num, st_tx_dscr_one.uc_error_mpdu_num, DMAC_STAT_PER_MAC_TOTAL);
    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, st_tx_dscr_one.uc_rts_succ + st_tx_dscr_one.uc_cts_failure, st_tx_dscr_one.uc_cts_failure, DMAC_STAT_PER_RTS_FAIL);

    /* ????mac??per???????????????????? */
    for (uc_rate_index = 0; uc_rate_index <= st_tx_dscr_one.uc_last_rate_rank; uc_rate_index++)
    {
        uc_hw_retry_num += st_tx_dscr_one.ast_per_rate[uc_rate_index].rate_bit_stru.bit_tx_count;
    }
    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info),
                        st_tx_dscr_one.uc_tid,
                        (st_tx_dscr_one.uc_mpdu_num * uc_hw_retry_num),
                        (st_tx_dscr_one.uc_mpdu_num * uc_hw_retry_num - 1) + st_tx_dscr_one.uc_error_mpdu_num,
                        DMAC_STAT_PER_HW_SW_FAIL);
#endif

#ifdef _PRE_WLAN_CHIP_TEST

    DMAC_CHIP_TEST_CALL(dmac_test_sch_stat_tx_mpdu_num(pst_dmac_user, st_tx_dscr_one.uc_tid ,st_tx_dscr_one, pst_cb));

#endif


#ifdef _PRE_DEBUG_MODE
    if ((pst_dscr->uc_q_num != HAL_TX_QUEUE_HI))
    {
        g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_complete_bh3_num++;
        OAM_INFO_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_normal_buffer::ul_tx_complete_bh3_num = %d.", g_ast_tx_complete_stat[pst_hal_device->uc_mac_device_id].ul_tx_complete_bh3_num);
    }
#endif

    dmac_tx_complete_free_dscr(pst_dscr);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_INIT_PTK_TX_PN

OAL_STATIC oal_bool_enum dmac_check_iv_word(oal_uint32 ul_iv_ms_word, oal_uint32 ul_iv_ls_word, oal_uint8 uc_chip_id,  oal_uint8 uc_ra_lut_index)
{
    /* ????iv word ?????????????? */
    if ((ul_iv_ms_word == 0) && (ul_iv_ls_word == 0))
    {
        return OAL_TRUE;
    }

    /* ra lut index ?????????? */
    if (OAL_UNLIKELY(uc_ra_lut_index != g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx))
    {
        OAM_WARNING_LOG2(0, OAM_SF_TX, "{dmac_check_iv_word::uc_ra_lut_index[%d] != g_ul_prev_iv_word.uc_pn_peer_idx[%d].}",
                                            uc_ra_lut_index, g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx);
        return OAL_TRUE;
    }

    /* ????????iv ms word??iv ms word??????????????????????;
       ??iv ms word????????????????????iv ls word??????????1000??????????????
       ??????iv word??????FFFFFFFFFFFF???????????????????? */
    if ((ul_iv_ms_word < g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb) &&
        (g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb - ul_iv_ms_word > 1))
    {
        return OAL_FALSE;
    }

    if ((ul_iv_ms_word > g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb) &&
        (ul_iv_ms_word - g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb > 1))
    {
        return OAL_FALSE;
    }

    if (ul_iv_ms_word == g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb)
    {
        if ((ul_iv_ls_word > g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb) &&
            (ul_iv_ls_word - g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb > 1000))
        {
            return OAL_FALSE;
        }

        if ((ul_iv_ls_word < g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb) &&
            (g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb - ul_iv_ls_word > 1000))
        {
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}


OAL_STATIC oal_bool_enum dmac_update_iv_word(oal_uint32 ul_iv_ms_word, oal_uint32 ul_iv_ls_word, oal_uint8 uc_chip_id, oal_uint8 uc_ra_lut_index)
{

    if (OAL_UNLIKELY(uc_ra_lut_index != g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx) ||
        OAL_UNLIKELY(uc_ra_lut_index != g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx))
    {
        OAM_WARNING_LOG3(0, OAM_SF_TX, "{dmac_update_iv_word::uc_ra_lut_index[%d] != g_ul_prev_iv_word.uc_pn_peer_idx[%d] or g_ul_max_iv_word.uc_pn_peer_idx[%d].}",
                                                    uc_ra_lut_index, g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx, g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].uc_pn_peer_idx);
        return OAL_FALSE;
    }

    if ((ul_iv_ms_word != 0) || (ul_iv_ls_word != 0))
    {
        g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb = ul_iv_ms_word;
        g_ul_prev_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb = ul_iv_ls_word;

        if ((ul_iv_ms_word > g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb) ||
            ((ul_iv_ms_word == g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb) &&
             (ul_iv_ls_word > g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb)))
        {
            g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_msb = ul_iv_ms_word;
            g_ul_max_iv_word[uc_chip_id][uc_ra_lut_index].ul_pn_lsb = ul_iv_ls_word;
        }
    }

    return OAL_TRUE;
}


oal_bool_enum dmac_init_iv_word_lut(hal_to_dmac_device_stru *pst_hal_device, hal_security_key_stru *pst_security_key)
{
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_init_iv_word_lut::pst_hal_device null.}\r\n");
        return OAL_FALSE;
    }

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_security_key))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_init_iv_word_lut::pst_security_key null.}\r\n");
        return OAL_FALSE;
    }

    if ((WLAN_KEY_TYPE_PTK != pst_security_key->en_key_type) ||
        (WLAN_80211_CIPHER_SUITE_CCMP != pst_security_key->en_cipher_type))
    {
        return OAL_FALSE;
    }

    if (OAL_UNLIKELY( pst_security_key->uc_lut_idx != g_ul_prev_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].uc_pn_peer_idx) ||
        OAL_UNLIKELY( pst_security_key->uc_lut_idx != g_ul_max_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].uc_pn_peer_idx))
    {
        OAM_WARNING_LOG3(0, OAM_SF_TX, "{dmac_init_iv_word_lut::uc_ra_lut_index[%d] != g_ul_prev_iv_word.uc_pn_peer_idx[%d] or g_ul_max_iv_word.uc_pn_peer_idx[%d].}",
                                            pst_security_key->uc_lut_idx, g_ul_prev_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].uc_pn_peer_idx, g_ul_max_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].uc_pn_peer_idx);
        return OAL_FALSE;
    }

    g_ul_prev_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].ul_pn_msb = 0x10010000;
    g_ul_prev_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].ul_pn_lsb = 0x20000000;

    g_ul_max_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].ul_pn_msb = 0x10010000;
    g_ul_max_iv_word[pst_hal_device->uc_chip_id][pst_security_key->uc_lut_idx].ul_pn_lsb = 0x20000000;

    return OAL_TRUE;
}

#endif

#ifdef _PRE_WLAN_FEATURE_AMPDU

OAL_STATIC oal_uint32  dmac_tx_complete_ampdu_buffer(hal_to_dmac_device_stru *pst_hal_device, hal_tx_dscr_stru *pst_dscr)
{
    oal_uint8                    uc_dscr_index;
    oal_uint8                    uc_dscr_status = DMAC_TX_INVALID;
    oal_uint8                    uc_dscr_chiper_type;
    oal_uint8                    uc_dscr_chiper_key_id;
    oal_uint8                    uc_retry_num   = 0;
    oal_uint8                    uc_q_num;
    oal_uint8                    uc_dscr_num;
    oal_uint8                    uc_vap_id;
    oal_uint8                    uc_tid;
    oal_bool_enum_uint8          en_dropped     = OAL_FALSE;

    oal_bool_enum_uint8          en_need_bar    = OAL_FALSE;
    oal_uint16                   us_ssn         = 0;
    oal_uint16                   us_seq_num     = 0;
    oal_uint8                    uc_data_tx_cnt = 0;
    oal_uint16                   us_user_idx    = 0xffff;
    oal_uint32                   ul_ret         = OAL_SUCC;
    mac_tx_ctl_stru             *pst_cb;
    dmac_user_stru              *pst_dmac_user;
    hal_tx_dscr_ctrl_one_param   st_tx_dscr_one = {0};
    hal_tx_dscr_stru            *pst_dscr_next;
    oal_uint32                   aul_ba_bitmap[DMAC_BA_BMP_SIZE >> 5] = {0};
    dmac_tid_stru               *pst_tid_queue;
    dmac_ba_tx_stru             *pst_ba_hdl;
    oal_dlist_head_stru          st_pending_q;
    dmac_vap_stru               *pst_dmac_vap;
    mac_vap_stru                *pst_mac_vap;
    oal_netbuf_stru             *pst_buf;
    oal_uint32                   ul_pktno;
    oal_bool_enum_uint8          en_is_vipframe = OAL_FALSE;
    dmac_user_alg_probe_enum_uint8  en_is_probe_data;
#ifdef _PRE_DEBUG_MODE
    oal_uint32                   ul_ampdu_len;
    mac_ieee80211_frame_stru    *pst_frame_hdr;
    hal_tx_dscr_stru            *pst_dscr_debug;
    hal_tx_dscr_stru            *pst_dscr_next_debug;
    oal_netbuf_stru             *pst_buf_debug;
    mac_tx_ctl_stru             *pst_cb_debug;
    oal_uint8                    uc_loop_debug;
    oal_switch_enum_uint8    en_frame_switch = 0;
    oal_switch_enum_uint8    en_cb_switch = 0;
    oal_switch_enum_uint8    en_dscr_switch = 0;
    oal_switch_enum_uint8    en_ota_switch = 0;
#endif

#ifdef _PRE_WLAN_PERFORM_STAT
    oal_uint8                    uc_hw_retry_num    = 0;
    oal_uint8                    uc_rate_index      = 0;
#endif

    uc_q_num = pst_dscr->uc_q_num;

    pst_buf = pst_dscr->pst_skb_start_addr;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_buf))
    {
        OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::pst_buf null.}");
        dmac_tx_complete_free_dscr_list(pst_dscr, 1);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cb          = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
    uc_dscr_num     = MAC_GET_CB_MPDU_NUM(pst_cb);
    ul_pktno        = MAC_GET_CB_ALG_PKTNO(pst_cb);
    en_is_vipframe  = MAC_GET_CB_IS_VIPFRAME(pst_cb);
    en_is_probe_data= MAC_GET_CB_IS_PROBE_DATA(pst_cb);
    uc_tid          = mac_get_cb_tid(pst_cb);

    dmac_tx_get_vap_id(pst_hal_device, pst_dscr, &uc_vap_id);

#ifdef _PRE_DEBUG_MODE
    if (OAL_TRUE == en_is_vipframe)
    {
        OAM_WARNING_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer:: VIP frame is aggregated. tidno = %u}", uc_tid);
        pst_frame_hdr = mac_get_cb_frame_hdr(pst_cb);
        oam_report_80211_frame(BROADCAST_MACADDR,
                           (oal_uint8 *)pst_frame_hdr,
                           MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb),
                           oal_netbuf_payload(pst_buf),
                           MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb) + MAC_GET_CB_MPDU_LEN(pst_cb),
                           OAM_OTA_FRAME_DIRECTION_TYPE_TX);
    }
#endif

    pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt = OAL_SUB(pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt, 1);

    if (OAL_UNLIKELY(0 == uc_vap_id))
    {
        OAM_WARNING_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::vap id is 0, mac vap had been deleted.}");
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_SUCC;
    }

    /* ????DMAC?????????????? */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(MAC_GET_CB_TX_USER_IDX(pst_cb));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::pst_dmac_user null.}");
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_dmac_user->st_user_base_info.uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_vap = &(pst_dmac_vap->st_vap_base_info);

    /* ??mac?????????????????????????????????????? */
    /* ????????idx????f??????????????????????????idx??cb?????????????????????????????????????????????????????????????????????? */
    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, pst_dmac_user->st_user_base_info.auc_user_mac_addr, &us_user_idx);
    if(OAL_UNLIKELY((OAL_SUCC != ul_ret) || (0xffff == us_user_idx) || (us_user_idx != MAC_GET_CB_TX_USER_IDX(pst_cb))))
    {
        OAM_WARNING_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::drop tx dscr normally.}\r\n");
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_SUCC;
    }

    /* ??????????????TID???? */
    pst_tid_queue = &(pst_dmac_user->ast_tx_tid_queue[uc_tid]);
    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_AMPDU_PREPARE);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_AMPDU_PREPARE);

    /* ????mib?????? */
    dmac_tx_update_alg_param(pst_dscr, pst_buf, &st_tx_dscr_one, pst_hal_device);
    dmac_performance_show_mpdu_info(uc_vap_id, uc_dscr_status, &st_tx_dscr_one);
    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_UPDATE_MIB);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_UPDATE_MIB);

    /* ??ps_poll??????????????????ps-poll??????????????????????????ps-poll???????? */
    pst_dmac_user->st_ps_structure.en_is_pspoll_rsp_processing = OAL_FALSE;

    /* ????????BA???????? */
    pst_ba_hdl = pst_tid_queue->pst_ba_tx_hdl;

    /* ba handle???????? */
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_ba_hdl) )
    {
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_UNLIKELY(DMAC_BA_COMPLETE != pst_ba_hdl->en_ba_conn_status))
    {
        //OAM_INFO_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::en_ba_conn_status=%d.}", pst_ba_hdl->en_ba_conn_status);
        dmac_tx_complete_free_dscr_list(pst_dscr, uc_dscr_num);
        return OAL_FAIL;
    }

    /* ????rifs??????????RIFS??????????????BAR */
    if (OAL_UNLIKELY(OAL_PTR_NULL != mac_get_cb_bar_dscr(pst_cb)))
    {
        dmac_tx_complete_free_dscr_list(mac_get_cb_bar_dscr(pst_cb), 1);
        uc_dscr_num--;
        pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt = OAL_SUB(pst_hal_device->ast_tx_dscr_queue[uc_q_num].uc_ppdu_cnt, uc_dscr_num);
    }

    /* ?????????????? */
    hal_tx_get_dscr_status(pst_hal_device, pst_dscr, &uc_dscr_status);
    hal_tx_get_dscr_chiper_type(pst_dscr, &uc_dscr_chiper_type, &uc_dscr_chiper_key_id);
#ifdef _PRE_WLAN_FEATURE_UAPSD
    /* ????keepalive?????? */
    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        dmac_keepalive_timestamp_update(pst_dmac_user, OAL_NETBUF_HEADER(pst_buf), uc_dscr_status);
    }
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    /*sta keepalive ????????????02??????????*/
    else
    {
        dmac_keepalive_timestamp_update(pst_dmac_user, OAL_NETBUF_HEADER(pst_buf), uc_dscr_status);
    }
#endif

    /* STA??pspoll?????????? */
#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        dmac_psm_tx_complete_sta(pst_dmac_vap, pst_dscr, pst_buf);
    }
#endif

#ifdef _PRE_DEBUG_MODE
//#if ((_PRE_TARGET_PRODUCT_TYPE_5610DMB != _PRE_CONFIG_TARGET_PRODUCT) )
    //g_ast_tx_ba_track[uc_device_id][pst_ba_hdl->uc_tx_ba_lut].ast_seq_bitmap_log[g_ast_tx_ba_track[uc_device_id][pst_ba_hdl->uc_tx_ba_lut].us_track_tail].un_mix_dscr.st_report.uc_report_mpdu_num  = uc_dscr_num;
    //g_ast_tx_ba_track[uc_device_id][pst_ba_hdl->uc_tx_ba_lut].ast_seq_bitmap_log[g_ast_tx_ba_track[uc_device_id][pst_ba_hdl->uc_tx_ba_lut].us_track_tail].un_mix_dscr.st_report.uc_tx_status        = uc_dscr_status;
//#endif
#endif

#if 0
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if(DMAC_TX_SOFT_PSM_BACK == uc_dscr_status)
    {
        dmac_psm_enqueue_psm_back_netbuf_tx_complete(pst_hal_device, pst_dscr, pst_dmac_vap, pst_dmac_user, pst_buf);
        return OAL_SUCC;
    }
#endif
#endif
/*delete for alg integrate,TBD*/

    if (OAL_LIKELY(uc_dscr_status == DMAC_TX_SUCC))
    {
        DMAC_VAP_DFT_STATS_PKT_SET_ZERO(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num);
#ifdef _PRE_DEBUG_MODE_USER_TRACK
        dmac_tx_complete_check_protocol(pst_hal_device, pst_dmac_user, pst_dscr);
#endif

        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_ampdu_succ_num,1);
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_mpdu_in_ampdu,uc_dscr_num);
#ifdef _PRE_WLAN_DFT_STAT
        if((uc_dscr_num >= DMAC_AMPDU_LENGTH_COUNT_LEVEL_1) && (uc_dscr_num <= DMAC_AMPDU_LENGTH_COUNT_LEVEL_14))
        {
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.aul_tx_count_per_apmpdu_length[DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_0], 1);
        }
        else if((uc_dscr_num >= DMAC_AMPDU_LENGTH_COUNT_LEVEL_15) && (uc_dscr_num <= DMAC_AMPDU_LENGTH_COUNT_LEVEL_17))
        {
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.aul_tx_count_per_apmpdu_length[DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_1], 1);
        }
        else if((uc_dscr_num >= DMAC_AMPDU_LENGTH_COUNT_LEVEL_18) && (uc_dscr_num <= DMAC_AMPDU_LENGTH_COUNT_LEVEL_30))
        {
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.aul_tx_count_per_apmpdu_length[DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_2], 1);
        }
        else if((uc_dscr_num >= DMAC_AMPDU_LENGTH_COUNT_LEVEL_31) && (uc_dscr_num <= DMAC_AMPDU_LENGTH_COUNT_LEVEL_32))
        {
            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.aul_tx_count_per_apmpdu_length[DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_3], 1);
        }
#endif

#ifdef _PRE_DEBUG_MODE
        /* ????AMPDU?????????????????? */
        dmac_tx_complete_get_ampdu_len(pst_hal_device, pst_dscr, &ul_ampdu_len);
        OAM_STAT_VAP_INCR(uc_vap_id, tx_ampdu_succ_num, 1);
        OAM_STAT_VAP_INCR(uc_vap_id, tx_ampdu_bytes, ul_ampdu_len);
        OAM_STAT_VAP_INCR(uc_vap_id, tx_mpdu_in_ampdu, uc_dscr_num);
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_ampdu_succ_num, 1);
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_ampdu_bytes, ul_ampdu_len);
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_mpdu_in_ampdu, uc_dscr_num);
#endif
        /* ???????? ????BA */
        OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_TX_COMP_CHECK);
        OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_TX_COMP_CHECK);

        /* ????BA???? ssn ?? bitmap */
        hal_tx_get_dscr_ba_ssn(pst_hal_device, pst_dscr, &us_ssn);
        hal_tx_get_dscr_ba_bitmap(pst_hal_device, pst_dscr, aul_ba_bitmap);
        /*OAM_INFO_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::pst_dscr=%x us_ssn=%d bitmap0=0x%x bitmap1=0x%x.}",
                                              pst_dscr, us_ssn, aul_ba_bitmap[0], aul_ba_bitmap[1]); */

        /* ???????? ????linkloss???? */
        if (MAC_VAP_STATE_UP == pst_dmac_vap->st_vap_base_info.en_vap_state)
        {
            dmac_vap_linkloss_clean(pst_dmac_vap);
        }
    }
    else  /*????????ba*/
    {
        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.uc_tx_successive_mpdu_fail_num, 1);
        dmac_successive_fail_dump_info(pst_dmac_vap, pst_dmac_user, pst_dscr);
        hal_tx_get_dscr_seq_num(pst_hal_device, pst_dscr, &us_seq_num);
        hal_tx_get_dscr_tx_cnt(pst_hal_device, pst_dscr, &uc_data_tx_cnt);

#ifdef _PRE_DEBUG_MODE
        pst_dscr_debug = pst_dscr;
        pst_buf_debug = pst_dscr_debug->pst_skb_start_addr;
        pst_cb_debug  = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_buf_debug);
        en_ota_switch = oam_report_data_get_global_switch(OAM_OTA_FRAME_DIRECTION_TYPE_TX);
        if (OAL_SWITCH_ON == en_ota_switch)
        {
                dmac_tx_dump_get_switch(OAM_USER_TRACK_FRAME_TYPE_DATA,
                                                 &en_frame_switch,
                                                 &en_cb_switch,
                                                 &en_dscr_switch,
                                                 pst_cb_debug);

                if (OAL_SWITCH_ON == en_dscr_switch)
                {
                    oam_report_dscr(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                                    (oal_uint8 *)pst_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
                }
        }
        pst_dscr_debug = pst_dscr;
        for (uc_loop_debug = 0; uc_loop_debug < uc_dscr_num; uc_loop_debug++)
        {
            pst_buf_debug = pst_dscr_debug->pst_skb_start_addr;
            pst_cb_debug  = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_buf_debug);

            if (DMAC_TX_AMPDU_MISMATCH == uc_dscr_status)
            {
                oam_report_dscr(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                            (oal_uint8 *)pst_dscr_debug, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);
            }

            if ((OAL_SWITCH_ON == en_ota_switch)  && (OAL_SWITCH_ON == en_frame_switch))
            {
                    oam_report_80211_frame(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                                       (oal_uint8 *)mac_get_cb_frame_hdr(pst_cb_debug),
                                       MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb_debug),
                                       oal_netbuf_payload(pst_buf_debug),
                                       MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb_debug) + MAC_GET_CB_MPDU_LEN(pst_cb_debug),
                                       OAM_OTA_FRAME_DIRECTION_TYPE_TX);
            }

            pst_dscr_next_debug = OAL_DLIST_GET_ENTRY(pst_dscr_debug->st_entry.pst_next, hal_tx_dscr_stru, st_entry);
            pst_dscr_debug      = pst_dscr_next_debug;
        }
#endif
        /* ????AMPDU?????????????????? */

        DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_ampdu_fail_num,1);

        OAM_STAT_VAP_INCR(uc_vap_id, tx_ampdu_fail_num, 1);
        OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_ampdu_fail_num, 1);
    }

    oal_dlist_init_head(&st_pending_q);
    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_EXTRACT_BA);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_EXTRACT_BA);

    pst_dscr_next = pst_dscr;

    for (uc_dscr_index = 0; uc_dscr_index < uc_dscr_num; uc_dscr_index++)
    {
        if (OAL_UNLIKELY(oal_dlist_is_empty(&pst_hal_device->ast_tx_dscr_queue[uc_q_num].st_header)))
        {
            OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::q empty.}");
            break;
        }

        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dscr_next))
        {
            OAM_ERROR_LOG0(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer:: pst_dscr_next NULL .}");
            break;
        }

        pst_dscr = pst_dscr_next;
        OAL_MEM_TRACE(pst_dscr, OAL_FALSE);
        pst_dscr_next = OAL_DLIST_GET_ENTRY(pst_dscr->st_entry.pst_next, hal_tx_dscr_stru, st_entry);
        /* ????buf */
        pst_buf = pst_dscr->pst_skb_start_addr;
        pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);
        uc_tid = mac_get_cb_tid(pst_cb);
        us_seq_num = mac_get_cb_seqnum(pst_cb);

        oal_dlist_delete_entry(&pst_dscr->st_entry);

        /* ????seqnum??ba???????? ??seqnum????lsn????seqnum????ssn ????????????????????*/
        if (OAL_UNLIKELY(OAL_TRUE == dmac_ba_seqnum_out_window(pst_ba_hdl, us_seq_num)))
        {
            OAM_ERROR_LOG4(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::seqnum(%d) out of ba window(%d~%d), pst_ba_hdl->uc_tx_ba_lut=%d.}",
                           us_seq_num, pst_ba_hdl->us_baw_start, pst_ba_hdl->us_last_seq_num, pst_ba_hdl->uc_tx_ba_lut);

            oam_report_dscr(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                            (oal_uint8 *)pst_dscr, WLAN_MEM_SHARED_TX_DSCR_SIZE1, OAM_OTA_TYPE_TX_DSCR);

            oam_report_80211_frame(pst_dmac_user->st_user_base_info.auc_user_mac_addr,
                                   (oal_uint8 *)mac_get_cb_frame_hdr(pst_cb),
                                   MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb),
                                   oal_netbuf_payload(pst_buf),
                                   MAC_GET_CB_FRAME_HEADER_LENGTH(pst_cb) + MAC_GET_CB_MPDU_LEN(pst_cb),
                                   OAM_OTA_FRAME_DIRECTION_TYPE_TX);

            dmac_move_ba_window_ahead(pst_ba_hdl, us_seq_num);

            st_tx_dscr_one.uc_error_mpdu_num++;
            en_dropped  = OAL_TRUE;

            DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_mpdu_fail_in_ampdu,1);

            OAM_STAT_VAP_INCR(uc_vap_id, tx_mpdu_fail_in_ampdu, 1);
            if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
            {
                DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_failed,1);
            }
            OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, uc_tid, tx_mpdu_fail_in_ampdu, 1);
        }
        /* ??????????ba??????seqnum????ba??bitmap???? */
        else if ((!DMAC_BA_ISSET(aul_ba_bitmap, DMAC_BA_INDEX(us_ssn, us_seq_num))) || (uc_dscr_status != DMAC_TX_SUCC))
        {
            st_tx_dscr_one.uc_error_mpdu_num++;

            if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
            {
                DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_failed,1);
            }
            if ((MAC_GET_CB_RETRIED_NUM(pst_cb) < pst_dmac_vap->uc_sw_retry_limit)
                && (OAL_FALSE == OAL_GET_THRUPUT_BYPASS_ENABLE(OAL_TX_HAL_HARDWARE_BYPASS))
                && (DMAC_USER_ALG_SMARTANT_NULLDATA_PROBE != MAC_GET_CB_IS_PROBE_DATA(pst_cb)))//????????????????????
            {
                /* ???????????????? ???????? */
                dmac_tx_set_retry_dscr(pst_hal_device, pst_dscr, uc_dscr_status, uc_dscr_chiper_type, uc_dscr_chiper_key_id);
                oal_dlist_add_tail(&pst_dscr->st_entry, &st_pending_q);
                uc_retry_num++;
                MAC_GET_CB_RETRIED_NUM(pst_cb)++;
                /*OAM_INFO_LOG1(uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_ampdu_buffer::uc_retried_num=%d.}", MAC_GET_CB_RETRIED_NUM(pst_cb));*/

                /* ???????????? */
                if ((uc_dscr_index == OAL_SUB(uc_dscr_num, 1)) && (OAL_FALSE == en_is_vipframe))
                {
                    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_COMP_ACK);
                    MAC_SET_CB_ALG_PKTNO((mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf), ul_pktno);
                    MAC_GET_CB_IS_PROBE_DATA((mac_tx_ctl_stru *)(oal_netbuf_cb(pst_buf))) = en_is_probe_data;
                    dmac_alg_tx_complete_notify(&pst_dmac_user->st_user_base_info, pst_buf, &st_tx_dscr_one);
                    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_COMP_ALG_NOTIFY);
                }

                continue;
            }
            else
            {
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
                pst_mac_vap->st_vap_stats.ul_tx_dropped_packets++;
#endif
                /* ?????????? ??bar */
                en_need_bar = OAL_TRUE;

                en_dropped  = OAL_TRUE;

                DMAC_VAP_DFT_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_mpdu_fail_in_ampdu,1);

                OAM_STAT_VAP_INCR(uc_vap_id, tx_mpdu_fail_in_ampdu, 1);
                OAM_STAT_TID_INCR(pst_dmac_user->st_user_base_info.us_assoc_id,  uc_tid, tx_mpdu_fail_in_ampdu, 1);
#ifdef _PRE_WLAN_PERFORM_STAT
                dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, 0, 1, DMAC_STAT_PER_SW_RETRY_SUB_AMPDU);
#endif
            }

        }

        if (OAL_FALSE == en_dropped)
        {
            /* ???????????????????? */
            if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
            {
                DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_pkts,MAC_GET_CB_NETBUF_NUM(pst_cb));
                DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_hw_tx_bytes,MAC_GET_CB_MPDU_BYTES(pst_cb));
                DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_hw_tx_pkts, MAC_GET_CB_NETBUF_NUM(pst_cb));
                DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_hw_tx_bytes, MAC_GET_CB_MPDU_BYTES(pst_cb));
          #ifdef _PRE_WLAN_FEATURE_VOWIFI
                dmac_update_vowifi_tx_cnt(pst_dmac_vap, pst_dmac_user);
          #endif
            }

        }
        else /* ???????? */
        {
            if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
            {
                DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_failed,1);
                DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_failed,  1);
            }

        }
        en_dropped = OAL_FALSE;


        oal_bit_set_bit_eight_byte(&st_tx_dscr_one.ull_ampdu_result, uc_dscr_index);
        dmac_ba_update_baw(pst_ba_hdl, us_seq_num);

        /* ???????????? */
        if ((uc_dscr_index == OAL_SUB(uc_dscr_num, 1)) && (OAL_FALSE == en_is_vipframe) && (OAL_FALSE == OAL_GET_THRUPUT_BYPASS_ENABLE(OAL_TX_HAL_HARDWARE_BYPASS)))
        {
            OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_COMP_ACK);
            MAC_SET_CB_ALG_PKTNO((mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf), ul_pktno);
            MAC_GET_CB_IS_PROBE_DATA((mac_tx_ctl_stru *)(oal_netbuf_cb(pst_buf))) = en_is_probe_data;
            dmac_alg_tx_complete_notify(&pst_dmac_user->st_user_base_info, pst_buf, &st_tx_dscr_one);
            OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_COMP_ALG_NOTIFY);
        }

        dmac_tx_complete_free_dscr(pst_dscr);
    }/*????mpdu??????????*/

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_COMP_ACK);

#ifdef _PRE_WLAN_PERFORM_STAT
    /* ???????????? */
    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, st_tx_dscr_one.uc_mpdu_num, st_tx_dscr_one.uc_error_mpdu_num, DMAC_STAT_PER_MAC_TOTAL);
    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info), st_tx_dscr_one.uc_tid, st_tx_dscr_one.uc_rts_succ + st_tx_dscr_one.uc_cts_failure, st_tx_dscr_one.uc_cts_failure, DMAC_STAT_PER_RTS_FAIL);

    for (uc_rate_index = 0; uc_rate_index <= st_tx_dscr_one.uc_last_rate_rank; uc_rate_index++)
    {
        uc_hw_retry_num += st_tx_dscr_one.ast_per_rate[uc_rate_index].rate_bit_stru.bit_tx_count;
    }

    dmac_stat_tid_per(&(pst_dmac_user->st_user_base_info),
                        st_tx_dscr_one.uc_tid,
                        (st_tx_dscr_one.uc_mpdu_num * uc_hw_retry_num),
                        (st_tx_dscr_one.uc_mpdu_num * (uc_hw_retry_num - 1) + st_tx_dscr_one.uc_error_mpdu_num),
                        DMAC_STAT_PER_HW_SW_FAIL);

    dmac_stat_tx_thrpt(pst_dmac_user, st_tx_dscr_one.uc_tid, st_tx_dscr_one);
#endif
/* ???????????????????? */
    if (FRW_EVENT_TYPE_HOST_DRX == MAC_GET_CB_EVENT_TYPE(pst_cb))
    {
        oal_uint8 uc_retries;

        /* ?????????? : ampdu????????????????*mpdu???? + ??????????????mpdu???? */
        DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_retries,(st_tx_dscr_one.uc_long_retry * uc_dscr_num + uc_retry_num));
        DMAC_USER_STATS_PKT_INCR(pst_dmac_user->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_short_retry);
        DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_retries,(st_tx_dscr_one.uc_long_retry * uc_dscr_num + uc_retry_num));
        DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_retries,st_tx_dscr_one.uc_short_retry);

        uc_retries = st_tx_dscr_one.uc_long_retry + st_tx_dscr_one.uc_short_retry;
        if (uc_retries > 0)
        {
            uc_retries -= 1;    /* ??????????????1?? */
        }

        OAM_STAT_USER_INCR(pst_dmac_user->st_user_base_info.us_assoc_id, tx_ppdu_retries, uc_retries);
    }
#ifdef _PRE_WLAN_DFT_STAT
    DMAC_VAP_STATS_PKT_INCR(pst_dmac_vap->st_query_stats.ul_tx_cts_fail,st_tx_dscr_one.uc_cts_failure);
#endif

#ifdef _PRE_WLAN_CHIP_TEST
    DMAC_CHIP_TEST_CALL(dmac_test_sch_stat_tx_mpdu_num(pst_dmac_user, st_tx_dscr_one.uc_tid, st_tx_dscr_one, pst_cb));
#endif

    if (OAL_TRUE == en_need_bar)
    {
        dmac_ba_send_bar(pst_ba_hdl, pst_dmac_user, pst_tid_queue);
    }

    /* ??????????????????????????tid???? */
    dmac_tx_complete_dscr_back(pst_hal_device, pst_dmac_vap, pst_tid_queue, &st_pending_q, uc_retry_num);

    /* ??????????????ba??????????????????true */
    if (OAL_TRUE == pst_dmac_user->en_delete_ba_flag)
    {
        pst_dmac_user->en_delete_ba_flag = OAL_FALSE;

        /* ????????HMAC???????????? */
        dmac_tx_delete_ba(pst_dmac_user);
    }

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_AMPDU_COMP_ENQUEUE_RETRY);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_AMPDU_COMP_ENQUEUE_RETRY);

    return OAL_SUCC;
}
#endif


OAL_STATIC OAL_INLINE oal_uint32  dmac_tx_update_alg_param(
                hal_tx_dscr_stru               *pst_dscr,
                oal_netbuf_stru                *pst_buf,
                hal_tx_dscr_ctrl_one_param     *pst_tx_dscr_one,
                hal_to_dmac_device_stru        *pst_hal_device)
{
    mac_tx_ctl_stru            *pst_cb;

    pst_cb = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_buf);

    hal_tx_get_dscr_ctrl_one_param(pst_hal_device, pst_dscr, pst_tx_dscr_one);

    pst_tx_dscr_one->us_mpdu_len = MAC_GET_CB_MPDU_LEN(pst_cb);
    pst_tx_dscr_one->uc_tid      = mac_get_cb_tid(pst_cb);

    return OAL_SUCC;
}


oal_uint32  dmac_tx_complete_schedule(hal_to_dmac_device_stru *pst_hal_device, oal_uint8 uc_ac_num)
{
    dmac_user_stru                  *pst_dmac_user = OAL_PTR_NULL;
    dmac_vap_stru                   *pst_dmac_vap = OAL_PTR_NULL;
    mac_tid_schedule_output_stru     st_schedule_ouput = {0};
    dmac_tid_stru                   *pst_dmac_tid = OAL_PTR_NULL;
    oal_uint32                       ul_ret = 0;
#ifdef _PRE_WLAN_FEATURE_DFR
    oal_uint16                       us_seq_num = 0;
#endif
    oal_uint8                        uc_tx_idx;
    oal_uint8                        uc_dscr_num;
    oal_uint8                        uc_dscr_idx;

#ifdef _PRE_WLAN_FEATURE_FLOWCTL
    mac_device_stru                 *pst_dev;
#endif

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_SCHEDULE_START);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_SCHEDULE_START);

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_schedule::pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = dmac_alg_tx_schedule_notify(pst_hal_device, uc_ac_num, &st_schedule_ouput);

    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_schedule::dmac_alg_tx_schedule_notify failed[%d].", ul_ret);
        return ul_ret;
    }

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_SCHEDULE);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_SCHEDULE);

    /* OAM_INFO_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_schedule::uc_mpdu_num=%d.}", st_schedule_ouput.uc_mpdu_num[0]); */

    if (0 == st_schedule_ouput.uc_mpdu_num[0])
    {
    #ifdef _PRE_WLAN_FEATURE_DFR
        /* ??BA??????????tid????????????return */
        if (OAL_FALSE == st_schedule_ouput.en_ba_is_jamed)
    #endif
        {
            return OAL_SUCC;
        }
    }

    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(st_schedule_ouput.us_user_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_schedule::pst_dmac_user null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_tid  = &(pst_dmac_user->ast_tx_tid_queue[st_schedule_ouput.uc_tid_num]);
    /* OAM_INFO_LOG1(0, OAM_SF_TX, "{dmac_tx_complete_schedule::uc_tid_num=%d.}", st_schedule_ouput.uc_tid_num); */

#ifdef _PRE_WLAN_PERFORM_STAT
    /* tid delay???????????? */
    dmac_stat_tid_delay(pst_dmac_tid);
#endif

    /* ??????????tid??????????????vap???????????????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_dmac_user->st_user_base_info.uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "{dmac_tx_complete_schedule::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_DFR
    if ((0 == st_schedule_ouput.uc_mpdu_num[0]) && (OAL_TRUE == st_schedule_ouput.en_ba_is_jamed))
    {
        /* ??BA??????????????????*/
        OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_schedule::us_user_idx=%d, uc_tid_num=%d, ba jamed.}",
                            st_schedule_ouput.us_user_idx, st_schedule_ouput.uc_tid_num);
        us_seq_num = DMAC_BA_SEQNO_ADD(pst_dmac_tid->pst_ba_tx_hdl->us_baw_start, pst_dmac_tid->pst_ba_tx_hdl->us_baw_size);
        us_seq_num = DMAC_BA_SEQNO_SUB(us_seq_num, 1);
        dmac_move_ba_window_ahead(pst_dmac_tid->pst_ba_tx_hdl, us_seq_num);
        OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_schedule::flush jamed, ssn =%d, lsn=%d.}",
                            pst_dmac_tid->pst_ba_tx_hdl->us_baw_start, pst_dmac_tid->pst_ba_tx_hdl->us_last_seq_num);
        return OAL_SUCC;
    }
#endif

    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_SCHEDULE_GET_TID);
    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_SCHEDULE_GET_TID);

    if (DMAC_TX_MODE_NORMAL == pst_dmac_tid->en_tx_mode)
    {
        uc_dscr_num = st_schedule_ouput.uc_mpdu_num[0];

#ifdef _PRE_DEBUG_MODE
        if (st_schedule_ouput.uc_mpdu_num[0] > 1)
        {
            OAM_INFO_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_schedule::not ampdu st_schedule_ouput.uc_mpdu_num=%d.}", uc_dscr_num);
        }
#endif
        for (uc_dscr_idx = 0; uc_dscr_idx < uc_dscr_num; uc_dscr_idx++)
        {
            ul_ret = dmac_tid_tx_queue_remove(pst_hal_device, pst_dmac_vap, pst_dmac_tid, 1);
        }
    }
    else
    {
        for (uc_tx_idx = 0; uc_tx_idx < DMAC_TX_QUEUE_AGGR_DEPTH; uc_tx_idx++)
        {
            uc_dscr_num = OAL_MIN(st_schedule_ouput.uc_mpdu_num[uc_tx_idx], pst_dmac_tid->pst_ba_tx_hdl->uc_ampdu_max_num);
            /*??????????ampdu??????????????????????????????????*/
            if (HAL_ALWAYS_TX_AMPDU_ENABLE == pst_hal_device->bit_al_tx_flag)
            {
                uc_dscr_num = WLAN_AMPDU_TX_MAX_NUM;
            }

            if (0 != uc_dscr_num)
            {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#ifdef _PRE_WLAN_DFT_STAT
                if(DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(DFX_PERFORMANCE_TX_LOG))
                {
                    OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX,
                                        "{dmac_tx_complete_schedule::alg_num[%d],tid_num=[%d],retry_num[%d],tx_idx[%d],tid[%d],ssn[%d],bw_size[%d],lsn[%d].}",
                                        8,
                                        st_schedule_ouput.uc_mpdu_num[uc_tx_idx],
                                        pst_dmac_tid->us_mpdu_num,
                                        pst_dmac_tid->uc_retry_num,
                                        uc_tx_idx,
                                        pst_dmac_tid->uc_tid,
                                        pst_dmac_tid->pst_ba_tx_hdl->us_baw_start,
                                        pst_dmac_tid->pst_ba_tx_hdl->us_baw_size,
                                        pst_dmac_tid->pst_ba_tx_hdl->us_last_seq_num);
                }
#endif
#endif
#ifdef _PRE_PROFILING_MODE
                /*profiling????ampdu??????????????????????*/
                uc_dscr_num = WLAN_AMPDU_TX_MAX_NUM;
#endif

                /* ????????????????????????????????????debug????????RL???????? */
                if (0 != g_uc_aggr_num_switch)
                {
                    uc_dscr_num = g_uc_max_aggr_num;
                }
                ul_ret = dmac_tid_tx_queue_remove_ampdu(pst_hal_device,
                                                        pst_dmac_vap,
                                                        pst_dmac_user,
                                                        pst_dmac_tid,
                                                        uc_dscr_num);
            }

            /* OAM_INFO_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_schedule::uc_dscr_num=%d.}", uc_dscr_num); */
        }
    }

#ifdef _PRE_WLAN_FEATURE_FLOWCTL
    pst_dev = (mac_device_stru *)mac_res_get_dev(pst_hal_device->uc_mac_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dev))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_tx_complete_schedule::pst_dev[%d] null!}", pst_hal_device->uc_mac_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    dmac_alg_flowctl_backp_notify(&(pst_dmac_vap->st_vap_base_info), pst_dev->us_total_mpdu_num, pst_dev->aus_ac_mpdu_num);
#endif
    dmac_alg_tid_update_notify(pst_dmac_tid);

    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_TX, "{dmac_tx_complete_schedule::dmac_tid_tx_queue_remove or dmac_tid_tx_queue_remove_ampdu failed.}");
        return ul_ret;
    }

    OAL_MIPS_TX_STATISTIC(DMAC_PROFILING_FUNC_SCHEDULE_END);
    OAM_PROFILING_TX_STATISTIC(OAL_PTR_NULL, OAM_PROFILING_FUNC_SCHEDULE_END);

    return OAL_SUCC;
}
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif


oal_void dmac_tx_reset_flush(hal_to_dmac_device_stru *pst_device)
{
    oal_uint8                       uc_queue_num;
    hal_tx_dscr_stru                *pst_dscr;
    oal_uint32                      ul_ret;
    oal_netbuf_stru                 *pst_buf = OAL_PTR_NULL;

    /*????????????????????????*/
    frw_event_flush_event_queue(FRW_EVENT_TYPE_WLAN_TX_COMP);

    /*????????????*/
    for(uc_queue_num=0; uc_queue_num < HAL_TX_QUEUE_NUM; uc_queue_num++)
    {
        while(OAL_TRUE != oal_dlist_is_empty(&pst_device->ast_tx_dscr_queue[uc_queue_num].st_header))
        {
            pst_dscr = OAL_DLIST_GET_ENTRY(pst_device->ast_tx_dscr_queue[uc_queue_num].st_header.pst_next, hal_tx_dscr_stru, st_entry);
            pst_buf = pst_dscr->pst_skb_start_addr;
            if (OAL_PTR_NULL == pst_buf)
            {
                OAM_WARNING_LOG0(0, OAM_SF_TX, "{dmac_tx_reset_flush::pst_buf is null, memory has been covered.}");
                oal_dlist_delete_head(&(pst_device->ast_tx_dscr_queue[uc_queue_num].st_header));
                /* ?????????????? */
                OAL_MEM_FREE((oal_void *)pst_dscr, OAL_TRUE);
                continue;
            }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
            hal_tx_set_dscr_status(pst_device, pst_dscr, DMAC_TX_SOFT_PSM_BACK);
#else
            hal_tx_set_dscr_status(pst_device, pst_dscr, DMAC_TX_SOFT_RESET_BACK);
#endif
            ul_ret = dmac_tx_complete_buff(pst_device, pst_dscr);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(0, OAM_SF_TX, "{dmac_reset_txq_flush::dmac_tx_complete_buff failed[%d].", ul_ret);
                continue;
            }
        }
    }
    return;
}


oal_uint32  dmac_mgmt_tx_complete(mac_vap_stru *pst_mac_vap,
                                  oal_uint8     mgmt_frame_id,
                                  oal_uint32    uc_dscr_status,
                                  oal_uint8     uc_user_idx)
{
    frw_event_mem_stru         *pst_event_mem;
    frw_event_stru             *pst_event;
    dmac_crx_mgmt_tx_status_stru *pst_mgmt_tx_status_param;

    /* ??mgmt tx ??????HMAC, ???????????? */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_crx_mgmt_tx_status_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "dmac_mgmt_tx_complete::alloc mem fail");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    pst_mgmt_tx_status_param = (dmac_crx_mgmt_tx_status_stru *)(pst_event->auc_event_data);
    pst_mgmt_tx_status_param->uc_dscr_status = uc_dscr_status;
    pst_mgmt_tx_status_param->mgmt_frame_id = mgmt_frame_id;
    pst_mgmt_tx_status_param->uc_user_idx   = uc_user_idx;

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CFG80211_MGMT_TX_STATUS, OAL_SIZEOF(dmac_crx_mgmt_tx_status_stru), (oal_uint8 *)pst_mgmt_tx_status_param);
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}

/*lint -e19*/
#if (defined(_PRE_DEBUG_MODE) && (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE))
oal_module_symbol(g_ul_desc_addr);
#endif
/*lint +e19*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

