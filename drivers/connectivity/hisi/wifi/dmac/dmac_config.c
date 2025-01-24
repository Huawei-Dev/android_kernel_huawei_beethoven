


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_sdio_comm.h"
#include "oal_sdio.h"
#include "oal_types.h"
#include "oal_pci_if.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "hal_ext_if.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hal_mac.h"
#endif
#include "hal_device.h"
#include "oal_mem.h"
#ifdef _PRE_WLAN_ALG_ENABLE
#include "alg_ext_if.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "alg_tpc.h"
#endif
#endif
#include "mac_data.h"
#include "mac_device.h"
#include "mac_ie.h"
#include "mac_regdomain.h"
#include "mac_vap.h"
#include "mac_board.h"
#include "dmac_ext_if.h"
#include "dmac_main.h"
#include "dmac_vap.h"
#include "dmac_rx_data.h"
#include "dmac_mgmt_classifier.h"
#include "dmac_mgmt_sta.h"
#include "dmac_tx_complete.h"
#include "dmac_tx_bss_comm.h"
#include "dmac_uapsd.h"
#include "dmac_blockack.h"
#include "dmac_beacon.h"
#include "dmac_user.h"
#include "dmac_11i.h"
#include "dmac_wep.h"
#include "dmac_uapsd.h"
#include "dmac_acs.h"
#include "dmac_reset.h"
#include "dmac_config.h"
#include "dmac_stat.h"
#include "dmac_data_acq.h"
#include "dmac_user_track.h"
#include "dmac_mgmt_bss_comm.h"
#include "dmac_txopps.h"
#include "dmac_dft.h"
#include "dmac_ap_pm.h"
#include "dmac_beacon.h"
#include "dmac_scan.h"
#include "dmac_psm_ap.h"
#include "dmac_device.h"
#include "dmac_resource.h"
#include "oal_gpio.h"
#include "oal_sdio.h"

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))
#include "cali_data.h"
#include "pm_extern.h"
#endif
#ifdef _PRE_WLAN_CHIP_TEST
#include "dmac_test_main.h"
#include "dmac_lpm_test.h"
#include "dmac_frame_filter_test.h"
#include "dmac_wmm_test.h"
#endif
#ifdef _PRE_WLAN_DFT_STAT
#include "hal_witp_phy_reg.h"
#include "hal_witp_pa_reg.h"
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#include "hal_phy_reg.h"
#include "hal_mac_reg.h"
#endif
#endif

#ifdef _PRE_WIFI_DMT
#include "hal_witp_dmt_if.h"
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "dmac_psm_sta.h"
#include "pm_extern.h"
#include "dmac_sta_pm.h"
#endif

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

#include "dmac_11w.h"
#endif
#include "dmac_chan_mgmt.h"

#include "dmac_reset.h"
#include "oal_net.h"
#include "dmac_config.h"
#include "dmac_main.h"
#include "dmac_rx_filter.h"

#ifdef _PRE_WLAN_FEATURE_BTCOEX
#include "dmac_btcoex.h"
#include "hal_coex_reg.h"
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#include "core_cr4.h"
#endif
#include "oal_profiling.h"

#ifdef _PRE_SUPPORT_ACS
#include "dmac_acs.h"
#endif
#ifdef _PRE_WLAN_DFT_STAT
#include "mac_board.h"
#endif

#include "dmac_arp_offload.h"

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
#include "dmac_auto_adjust_freq.h"
#endif

#ifdef    _PRE_WLAN_FEATURE_GREEN_AP
#include "dmac_green_ap.h"
#endif

#ifdef _PRE_WLAN_RF_CALI
#include "dmac_auto_cali.h"
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_CONFIG_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
extern oal_uint16 g_usUsedMemForStop;
extern oal_uint16 g_usUsedMemForstart;
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
extern dmac_beacon_linkloss_stru g_st_beacon_linkloss;
extern oal_uint32 g_d2h_hcc_assemble_count;
extern oal_uint32 g_aul_phy_power0_ref_2g[];
extern oal_uint32 g_aul_phy_power0_ref_5g[];
extern hal_customize_stru g_st_customize;
extern oal_uint8 g_auc_rate_pow_table_margin[ALG_TPC_RATE_TPC_CODE_TABLE_LEN][WLAN_BAND_BUTT];
extern oal_uint16 g_aus_rf_pow_limit[20][2];
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#ifdef _PRE_WLAN_CFGID_DEBUG
extern OAL_CONST dmac_config_syn_stru g_ast_dmac_config_syn_debug[];
extern oal_uint32 dmac_get_config_debug_arrysize(oal_void);
#endif

/*????legacy rate????????*/
oal_uint8 g_auc_legacy_rate_idx_table[WLAN_LEGACY_RATE_VALUE_BUTT]=
{
/*WLAN_LEGACY_11b_RESERVED1       = 0*/  1,
/*WLAN_SHORT_11b_2M_BPS           = 1*/  2,
/*WLAN_SHORT_11b_5_HALF_M_BPS     = 2*/  5,
/*WLAN_SHORT_11b_11_M_BPS         = 3*/  11,

/*WLAN_LONG_11b_1_M_BPS           = 4*/  1,
/*WLAN_LONG_11b_2_M_BPS           = 5*/  2,
/*WLAN_LONG_11b_5_HALF_M_BPS      = 6*/  5,
/*WLAN_LONG_11b_11_M_BPS          = 7*/  11,

/*WLAN_LEGACY_OFDM_48M_BPS        = 8*/   48,
/*WLAN_LEGACY_OFDM_24M_BPS        = 9*/   24,
/*WLAN_LEGACY_OFDM_12M_BPS        = 10*/  12,
/*WLAN_LEGACY_OFDM_6M_BPS         = 11*/  6,
/*WLAN_LEGACY_OFDM_54M_BPS        = 12*/  54,
/*WLAN_LEGACY_OFDM_36M_BPS        = 13*/  36,
/*WLAN_LEGACY_OFDM_18M_BPS        = 14*/  18,
/*WLAN_LEGACY_OFDM_9M_BPS         = 15*/  9,
};

/*****************************************************************************
  ????????????????????
*****************************************************************************/
OAL_CONST dmac_config_channel_bw_map_stru g_ast_channel_bw_map_2G[MAC_CHANNEL_FREQ_2_BUTT] =
{
    {1,  WLAN_BAND_WIDTH_40PLUS},
    {2,  WLAN_BAND_WIDTH_40PLUS},
    {3,  WLAN_BAND_WIDTH_40PLUS},
    {4,  WLAN_BAND_WIDTH_40PLUS},
    {5,  WLAN_BAND_WIDTH_40PLUS},
    {6,  WLAN_BAND_WIDTH_40PLUS},
    {7,  WLAN_BAND_WIDTH_40PLUS},
    {8,  WLAN_BAND_WIDTH_40MINUS},
    {9,  WLAN_BAND_WIDTH_40MINUS},
    {10, WLAN_BAND_WIDTH_40MINUS},
    {11, WLAN_BAND_WIDTH_40MINUS},
    {12, WLAN_BAND_WIDTH_40MINUS},
    {13, WLAN_BAND_WIDTH_40MINUS},
    {14, WLAN_BAND_WIDTH_40MINUS}
};

OAL_CONST dmac_config_channel_bw_map_stru g_ast_channel_bw_map_5G[MAC_CHANNEL_FREQ_5_BUTT] =
{
    {36,  WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {40,  WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {44,  WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {48,  WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {52,  WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {56,  WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {60,  WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {64,  WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {100, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {104, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {108, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {112, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {116, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {120, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {124, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {128, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {132, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {136, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {140, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {144, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {149, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {153, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {157, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {161, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {165, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},

    {184, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSPLUS},
    {188, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSPLUS},
    {192, WLAN_BAND_WIDTH_40PLUS,  WLAN_BAND_WIDTH_80PLUSMINUS},
    {196, WLAN_BAND_WIDTH_40MINUS, WLAN_BAND_WIDTH_80MINUSMINUS},
};

/*****************************************************************************
  3 ????????????
*****************************************************************************/
OAL_STATIC oal_void  dmac_config_set_dscr_fbm(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_pgl(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_mtpgl(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_ta(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_ra(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_cc(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_data0(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_data1(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_data2(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_data3(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_power(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_shortgi(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_preamble_mode(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_rtscts(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_lsigtxop(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_smooth(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_snding(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_txbf(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
OAL_STATIC oal_void  dmac_config_set_dscr_stbc(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
#endif
OAL_STATIC oal_void  dmac_config_get_dscr_ess(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_dyn_bw(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_dyn_bw_exist(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_void  dmac_config_set_dscr_ch_bw_exist(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap);
OAL_STATIC oal_uint32  dmac_config_get_version(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param);
OAL_STATIC oal_uint32  dmac_config_get_ant(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param);
OAL_STATIC oal_uint32   dmac_config_rx_fcs_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param);


#if !((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION))
extern oal_uint32  dmac_config_set_meta(mac_vap_stru *pst_mac_vap, oal_uint8 us_len, oal_uint8 *puc_param);
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
extern void OML_LOG_SetLevel(oal_uint8 ucModuleID, oal_uint8 ucLevel);
#endif


OAL_STATIC dmac_set_dscr_func  g_dmac_config_set_dscr_param[] =
{
    dmac_config_set_dscr_fbm,       /* fbm, ??????????????13????freq bandwidth mode */
    dmac_config_set_dscr_pgl,       /* ?????? */
    dmac_config_set_dscr_mtpgl,     /* ?????? */
    OAL_PTR_NULL,                   /* ?????? */
    dmac_config_set_dscr_ta,        /* ta??tx antena????????????????15????TXRTS Antenna */
    dmac_config_set_dscr_ra,        /* ra: rx antena, ??????????????15????RXCTRL Antenna */
    dmac_config_set_dscr_cc,        /* cc: channel code, ??????????????13????channel code */
    dmac_config_set_dscr_data0,     /* data0????????????????14????32bit 10?????? */
    dmac_config_set_dscr_data1,     /* data1????????????????19???? */
    dmac_config_set_dscr_data2,     /* data2????????????????20???? */
    dmac_config_set_dscr_data3,     /* data3????????????????21???? */
    dmac_config_set_dscr_power,     /* tx power: ??????????????22?? */
    dmac_config_set_dscr_shortgi,         /* ????short GI??long GI*/
    dmac_config_set_dscr_preamble_mode,   /* ????preamble mode*/
    dmac_config_set_dscr_rtscts,         /* ????rts/cts????????*/
    dmac_config_set_dscr_lsigtxop,       /* ????lsig txop????????*/
    dmac_config_set_dscr_smooth,        /* ?????????????????????????????? */
    dmac_config_set_dscr_snding,        /* ????Sounding???? */
    dmac_config_set_dscr_txbf,          /* ????txbf???? */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    dmac_config_set_dscr_stbc,          /* ????stbc???? */
#endif
    dmac_config_get_dscr_ess,           /* ?????????????? */
    dmac_config_set_dscr_dyn_bw,        /* ????DYN_BANDWIDTH_IN_NON_HT */
    dmac_config_set_dscr_dyn_bw_exist,  /* ????DYN_BANDWIDTH_IN_NON_HT exist */
    dmac_config_set_dscr_ch_bw_exist,   /* ????CH_BANDWIDTH_IN_NON_HT exist*/
};

oal_uint32 g_ul_first_timestamp = 0;    /*????????????????????????*/

/*****************************************************************************
3 ????????????
*****************************************************************************/

/*****************************************************************************

 4 ????????
*****************************************************************************/


oal_uint32  dmac_send_sys_event(
                mac_vap_stru                     *pst_mac_vap,
                wlan_cfgid_enum_uint16            en_cfg_id,
                oal_uint16                        us_len,
                oal_uint8                        *puc_param)
{
    oal_uint32                  ul_ret;
    frw_event_mem_stru         *pst_event_mem;
    dmac_to_hmac_cfg_msg_stru  *pst_syn_msg;
    frw_event_stru             *pst_event;

    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_to_hmac_cfg_msg_stru) - 4 + us_len);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_send_sys_event::pst_event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ?????????? */
    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                        FRW_EVENT_TYPE_HOST_SDT_REG,
                        DMAC_TO_HMAC_SYN_CFG,
                        OAL_SIZEOF(dmac_to_hmac_cfg_msg_stru) - 4 + us_len,
                        FRW_EVENT_PIPELINE_STAGE_1,
                        pst_mac_vap->uc_chip_id,
                        pst_mac_vap->uc_device_id,
                        pst_mac_vap->uc_vap_id);

    pst_syn_msg = (dmac_to_hmac_cfg_msg_stru *)pst_event->auc_event_data;

    DMAC_INIT_SYN_MSG_HDR(pst_syn_msg, en_cfg_id, us_len);

    /* ???????????????????? */
    oal_memcopy(pst_syn_msg->auc_msg_body, puc_param, us_len);

    /* ???????? */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_send_sys_event::frw_event_dispatch_event failed[%d].}",ul_ret);
        FRW_EVENT_FREE(pst_event_mem);
        return ul_ret;
    }

    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}


OAL_STATIC oal_void  dmac_config_set_dscr_fbm(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    pst_dmac_vap->st_tx_alg.st_rate.uc_channel_bandwidth = (oal_uint8)l_value;
    (WLAN_BAND_ASSEMBLE_AUTO == pst_dmac_vap->st_tx_alg.st_rate.uc_channel_bandwidth)?(pst_dmac_vap->bit_bw_cmd = 0):(pst_dmac_vap->bit_bw_cmd = 1);
}


OAL_STATIC oal_void  dmac_config_set_dscr_pgl(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    pst_dmac_vap->st_tx_alg.st_tx_power.bit_pa_gain_level3 = (oal_uint8)l_value;
}


OAL_STATIC oal_void  dmac_config_set_dscr_mtpgl(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    pst_dmac_vap->st_tx_alg.st_tx_power.bit_pa_gain_level0 = (oal_uint8)l_value;
}


OAL_STATIC oal_void  dmac_config_set_dscr_ta(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    pst_dmac_vap->st_tx_alg.st_antenna_params.uc_tx_rts_antenna = (oal_uint8)l_value;
}


OAL_STATIC oal_void  dmac_config_set_dscr_ra(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    pst_dmac_vap->st_tx_alg.st_antenna_params.uc_rx_ctrl_antenna = (oal_uint8)l_value;
}


OAL_STATIC oal_void  dmac_config_set_dscr_cc(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    hal_to_dmac_device_stru         *pst_hal_device_base;
    wlan_phy_protocol_enum_uint8    en_curr_prot;

    pst_hal_device_base = pst_dmac_vap->pst_hal_device;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device_base))
    {
       OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                             "{dmac_config_set_dscr_cc::pst_hal_device_base null.}");
       return;
    }

    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.en_channel_code = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.en_channel_code  = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.en_channel_code = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.en_channel_code = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.en_channel_code = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.en_channel_code = (oal_uint8)l_value;
            en_curr_prot = pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_cc::invalid frame type[%02x].}", uc_type);
            return;
    }

    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_hal_device_base->uc_al_tx_flag)
    {
       hal_set_tx_dscr_field(pst_hal_device_base, (oal_uint8)l_value, HAL_RF_TEST_CHAN_CODE);
    }

    /* ?????????? */
    if ((1 == l_value) && (WLAN_11B_PHY_PROTOCOL_MODE == en_curr_prot || WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE == en_curr_prot))
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "{dmac_config_set_dscr_cc::can not set channel code to 1 in non-HT mode.}");
    }
    else if ((1 == l_value) || (0 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_cc::channel code=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_cc::invalid channel code=%d", l_value);
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_data0(oal_int32 l_value, oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].ul_value = (oal_uint32)l_value;
            break;
    #endif

        default:
            break;

    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_data1(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[1].ul_value = (oal_uint32)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void dmac_config_set_dscr_data2(oal_int32 l_value, oal_uint8 uc_type,dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[2].ul_value = (oal_uint32)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_data3(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[3].ul_value = (oal_uint32)l_value;
        break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[3].ul_value = (oal_uint32)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_power(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    oal_uint32 *pul_txpower;

    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_alg.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_data_mcast.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_data_bcast.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_alg_vht.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_alg_ht.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_alg_11ag.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pul_txpower  = (oal_uint32 *)(&(pst_dmac_vap->st_tx_alg_11b.st_tx_power));
            *pul_txpower = (oal_uint32)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_shortgi(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
    hal_to_dmac_device_stru         *pst_hal_device_base;

    pst_hal_device_base = pst_dmac_vap->pst_hal_device;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device_base))
    {
       OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                             "{dmac_config_set_dscr_cc::pst_hal_device_base null.}");
       return;
    }
#endif

    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.bit_short_gi_enable = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_shortgi::uc_type=%d", uc_type);
            return;
    }

#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_hal_device_base->uc_al_tx_flag)
    {
       hal_set_tx_dscr_field(pst_hal_device_base, pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value, HAL_RF_TEST_DATA_RATE_ZERO);
    }

    if ((0 == l_value) || (1 == l_value))
    {
        pst_dmac_vap->uc_short_gi = (oal_uint8)l_value;
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_shortgi::short gi enable l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_shortgi::invalid short gi enable l_value=%d", l_value);
    }
#endif
}


OAL_STATIC oal_void  dmac_config_set_dscr_preamble_mode(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
        hal_to_dmac_device_stru         *pst_hal_device_base;

        pst_hal_device_base = pst_dmac_vap->pst_hal_device;
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device_base))
        {
           OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                                 "{dmac_config_set_dscr_cc::pst_hal_device_base null.}");
           return;
        }
#endif

    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_preamble_mode::invalid frame type=%d", uc_type);
            return;
    }

#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
	/* ?????????????????? */
    if (OAL_SWITCH_ON == pst_hal_device_base->uc_al_tx_flag)
    {
       hal_set_tx_dscr_field(pst_hal_device_base, pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value, HAL_RF_TEST_DATA_RATE_ZERO);
    }

    if ((0 == l_value) || (1 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_preamble_mode::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_preamble_mode::invalid l_value=%d", l_value);
    }
#endif
}



OAL_STATIC oal_void  dmac_config_set_dscr_rtscts(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.bit_rts_cts_enable = (oal_uint8)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_lsigtxop(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.bit_lsig_txop = (oal_uint8)l_value;
            break;
    #endif

        default:
            break;
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_smooth(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.uc_smoothing = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.uc_smoothing  = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.uc_smoothing = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.uc_smoothing = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.uc_smoothing = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.uc_smoothing = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_smooth::invalid frame type=%d", uc_type);
            return;
    }

    if ((0 == l_value) || (1 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_smooth::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_smooth::invalid l_value=%d", l_value);
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_snding(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.en_sounding_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.en_sounding_mode  = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.en_sounding_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.en_sounding_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.en_sounding_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.en_sounding_mode = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_snding::invalid frame type=%d", uc_type);
            return;
    }

    /* ????TX?????? */
    if ((l_value >= 0) && (l_value <= 3))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_snding::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_snding::invalid l_value=%d", l_value);
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_txbf(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.bit_txbf_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_txbf_mode  = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.bit_txbf_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.bit_txbf_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.bit_txbf_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.bit_txbf_mode = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_txbf::invalid frame type=%d", uc_type);
            return;
    }

    /* ????TX?????? */
    if ((l_value >= 0) && (l_value <= 3))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_txbf::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_txbf::invalid l_value=%d", l_value);
    }
}
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)

OAL_STATIC oal_void  dmac_config_set_dscr_stbc(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.ast_per_rate[0].rate_bit_stru.bit_stbc_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].ast_per_rate[0].rate_bit_stru.bit_stbc_mode  = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.ast_per_rate[0].rate_bit_stru.bit_stbc_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.ast_per_rate[0].rate_bit_stru.bit_stbc_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.ast_per_rate[0].rate_bit_stru.bit_stbc_mode = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.ast_per_rate[0].rate_bit_stru.bit_stbc_mode = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_stbc::invalid frame type=%d", uc_type);
            return;
    }

    /* ????TX?????? */
    if ((l_value >= 0) && (l_value <= 3))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_stbc::l_valuee=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_stbc::invalid l_value=%d", l_value);
    }
}
#endif

OAL_STATIC oal_void  dmac_config_get_dscr_ess(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    oal_uint8   uc_ess_num;

    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_alg.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_data_mcast.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_data_bcast.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            uc_ess_num = pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            uc_ess_num = pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            uc_ess_num = pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            uc_ess_num = pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.uc_extend_spatial_streams;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_alg_vht.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_alg_ht.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_alg_11ag.st_rate.uc_extend_spatial_streams;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            uc_ess_num = pst_dmac_vap->st_tx_alg_11b.st_rate.uc_extend_spatial_streams;
            break;
    #endif

        default:
            uc_ess_num = 0;
            break;
    }

    /* ????TX?????? */
    OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_get_dscr_ess::uc_ess_num=%d", uc_ess_num);
}


OAL_STATIC oal_void  dmac_config_set_dscr_dyn_bw(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.dyn_bandwidth_in_non_ht  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.dyn_bandwidth_in_non_ht   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.dyn_bandwidth_in_non_ht   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.dyn_bandwidth_in_non_ht   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.dyn_bandwidth_in_non_ht   = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.dyn_bandwidth_in_non_ht = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw::invalid frame type=%d", uc_type);
            return;
    }

    if ((0 == l_value) || (1 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw::invalid l_value=%d", l_value);
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_dyn_bw_exist(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.dyn_bandwidth_in_non_ht_exist  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.dyn_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.dyn_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.dyn_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.dyn_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.dyn_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw_exist::invalid frame type=%d", uc_type);
            return;
    }

    if ((0 == l_value) || (1 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw_exist::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_dyn_bw_exist::invalid l_value=%d", l_value);
    }
}


OAL_STATIC oal_void  dmac_config_set_dscr_ch_bw_exist(oal_int32 l_value,oal_uint8 uc_type, dmac_vap_stru *pst_dmac_vap)
{
    switch (uc_type)
    {
        case MAC_VAP_CONFIG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MCAST_DATA:
            pst_dmac_vap->st_tx_data_mcast.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_BCAST_DATA:
            pst_dmac_vap->st_tx_data_bcast.st_rate.ch_bandwidth_in_non_ht_exist  = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_2G].st_rate.ch_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_UCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_ucast[WLAN_BAND_5G].st_rate.ch_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_2G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_2G].st_rate.ch_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_MBCAST_MGMT_5G:
            pst_dmac_vap->ast_tx_mgmt_bmcast[WLAN_BAND_5G].st_rate.ch_bandwidth_in_non_ht_exist   = (oal_uint8)l_value;
            break;
    #ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
        case MAC_VAP_CONFIG_VHT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_vht.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_HT_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_ht.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11AG_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11ag.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
        case MAC_VAP_CONFIG_11B_UCAST_DATA:
            pst_dmac_vap->st_tx_alg_11b.st_rate.ch_bandwidth_in_non_ht_exist = (oal_uint8)l_value;
            break;
    #endif

        default:
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_ch_bw_exist::invalid frame type=%d", uc_type);
            return;
    }

    if ((0 == l_value) || (1 == l_value))
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_ch_bw_exist::l_value=%d", l_value);
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr_ch_bw_exist::invalid l_valueype=%d", l_value);
    }
}


oal_uint32  dmac_get_cmd_one_arg(oal_int8 *pc_cmd, oal_int8 *pc_arg, oal_uint32 *pul_cmd_offset)
{
    oal_int8   *pc_cmd_copy;
    oal_uint32  ul_pos = 0;

    if (OAL_UNLIKELY((OAL_PTR_NULL == pc_cmd) || (OAL_PTR_NULL == pc_arg) || (OAL_PTR_NULL == pul_cmd_offset)))
    {
        OAM_ERROR_LOG3(0, OAM_SF_CFG,
                       "{dmac_get_cmd_one_arg::param null, pc_cmd=%d pc_arg=%d pul_cmd_offset=%d.}",
                       pc_cmd, pc_arg, pul_cmd_offset);

        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_cmd_copy = pc_cmd;

    /* ???????????????????? */
    while (' ' == *pc_cmd_copy)
    {
        ++pc_cmd_copy;
    }

    while ((' ' != *pc_cmd_copy) && ('\0' != *pc_cmd_copy))
    {
        pc_arg[ul_pos] = *pc_cmd_copy;
        ++ul_pos;
        ++pc_cmd_copy;

        if (OAL_UNLIKELY(ul_pos >= DMAC_HIPRIV_CMD_NAME_MAX_LEN))
        {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_get_cmd_one_arg::ul_pos=%d", ul_pos);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }
    }

    pc_arg[ul_pos]  = '\0';

    /* ???????????????????????? */
    if (0 == ul_pos)
    {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{dmac_get_cmd_one_arg::ul_pos=0.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pul_cmd_offset = (oal_uint32)(pc_cmd_copy - pc_cmd);

    return OAL_SUCC;
}



OAL_STATIC oal_void  dmac_config_reg_display_test(mac_vap_stru *pst_mac_vap, oal_uint32 ul_start_addr, oal_uint32 ul_end_addr)
{
    mac_device_stru      *pst_device;
    oal_uint32            ul_addr;
    oal_uint32            ul_val = 0;

    if (ul_start_addr % 4 != 0 || ul_end_addr % 4 != 0)
    {
        return;
    }

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        return;
    }

    for (ul_addr = ul_start_addr; ul_addr <= ul_end_addr; ul_addr += 4)
    {
        hal_reg_info(pst_device->pst_device_stru, ul_addr, &ul_val);
        OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_reg_display_test::reg_info addr=0x%08x, value=0x%08x", ul_addr, ul_val);
        OAL_IO_PRINT("dmac_config_reg_display_test::reg_info addr=0x%08x, value=0x%08x\r\n", ul_addr, ul_val);
    }
}




OAL_STATIC oal_void  dmac_config_reg_display_test16(mac_vap_stru *pst_mac_vap, oal_uint32 ul_start_addr, oal_uint32 ul_end_addr)
{
    mac_device_stru      *pst_device;
    oal_uint32            ul_addr;
    oal_uint16            us_val = 0;

    if (ul_start_addr % 2 != 0 || ul_end_addr % 2 != 0)
    {
        OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_reg_display_test16::not mod 2, start[%08x], end[%08x].", ul_start_addr, ul_end_addr);
        return;
    }

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{dmac_config_reg_display_test16::pst_device null.");
        return;
    }

    for (ul_addr = ul_start_addr; ul_addr <= ul_end_addr; ul_addr += 2)
    {
        hal_reg_info16(pst_device->pst_device_stru, ul_addr, &us_val);
        OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_reg_display_test::reg_info addr=0x%08x, value=0x%08x", ul_addr, us_val);
        OAL_IO_PRINT("dmac_config_reg_display_test::reg_info addr=0x%08x, value=0x%08x\r\n", ul_addr, us_val);
    }
}




OAL_STATIC oal_void  dmac_config_reg_write_test(mac_vap_stru *pst_mac_vap, oal_uint32 ul_addr, oal_uint32 ul_val)
{
    mac_device_stru      *pst_device;
    oal_int8              ac_buf[64] = {0};

    if (ul_addr % 4 != 0)
    {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_config_reg_write_test::ul_addr=%d", ul_addr);
        return;
    }

    OAL_SPRINTF((char*)ac_buf, OAL_SIZEOF(ac_buf), "reg_write:addr=0x%08x, val=0x%08x.\n", ul_addr, ul_val);
    OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_reg_write_test::reg_write:addr=0x%08x, val=0x%08x.", ul_addr, ul_val);
    oam_print(ac_buf);

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        return;
    }

    hal_reg_write(pst_device->pst_device_stru, ul_addr, ul_val);
}


OAL_STATIC oal_void  dmac_config_reg_write_test16(mac_vap_stru *pst_mac_vap, oal_uint32 ul_addr, oal_uint16 us_val)
{
    mac_device_stru      *pst_device;
    oal_int8              ac_buf[64] = {0};

    if (ul_addr % 2 != 0)
    {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_config_reg_write_test::ul_addr=%d", ul_addr);
        return;
    }

    OAL_SPRINTF((char*)ac_buf, OAL_SIZEOF(ac_buf), "reg_write:addr=0x%08x, val=0x%04x.\n", ul_addr, us_val);
    OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_reg_write_test::reg_write:addr=0x%08x, val=0x%04x.", ul_addr, us_val);
    oam_print(ac_buf);

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        return;
    }

    hal_reg_write16(pst_device->pst_device_stru, ul_addr, us_val);
}


oal_uint32  dmac_config_set_bss_type(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    /* ????mib?? */
    return mac_mib_set_bss_type(pst_mac_vap, uc_len, puc_param);
}


#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL

OAL_STATIC oal_uint32  dmac_config_get_hipkt_stat(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_get_hipkt_stat();

    return OAL_SUCC;
}
#endif


oal_uint32  dmac_config_set_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_mode_param_stru     *pst_prot_param;
    dmac_vap_stru               *pst_dmac_vap;
    mac_device_stru             *pst_mac_device;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_mode::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mode::pst_mac_device null.}");
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    /* ????dmac vap?????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_TXBF, "{dmac_config_set_mode::pst_dmac_vap null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    #if 0 // gaolin??ACS????????????????????????????DMAC????????????????????band??channel????
    /* ??????????????????????????*/
    if (WLAN_BAND_WIDTH_BUTT != pst_mac_device->en_max_bandwidth)
    {
        return OAL_SUCC;
    }
    #endif

    pst_prot_param = (mac_cfg_mode_param_stru *)puc_param;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* ????????????, band, bandwidth??mac_vap?? */
    pst_mac_vap->en_protocol                              = pst_prot_param->en_protocol;
    pst_mac_vap->st_channel.en_band                       = pst_prot_param->en_band;
    pst_mac_vap->st_channel.en_bandwidth                  = pst_prot_param->en_bandwidth;
    pst_mac_vap->st_ch_switch_info.en_user_pref_bandwidth = pst_prot_param->en_bandwidth;

#ifdef _PRE_WLAN_FEATURE_11AC2G
    if ((WLAN_VHT_MODE == pst_mac_vap->en_protocol)
        && (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band))
    {
        mac_vap_set_11ac2g(pst_mac_vap, OAL_TRUE);
    }
    else
    {
        mac_vap_set_11ac2g(pst_mac_vap, OAL_FALSE);
    }
#endif



    /* ????????????vap???? */
    mac_vap_init_by_protocol(pst_mac_vap, pst_prot_param->en_protocol);

    /* ????????????????Mib */
    mac_vap_change_mib_by_bandwidth(pst_mac_vap, pst_prot_param->en_bandwidth);

    /* ????device???????????????????? */
    pst_mac_device->en_max_bandwidth = pst_prot_param->en_bandwidth;
    pst_mac_device->en_max_band      = pst_prot_param->en_band;

#endif
#ifdef _PRE_WLAN_FEATURE_TXBF
    if ((pst_mac_vap->en_protocol >= WLAN_HT_MODE)
        && (OAL_TRUE == pst_mac_device->bit_su_bfmee))
    {
        pst_mac_vap->st_cap_flag.bit_11ntxbf = OAL_TRUE;
    }
    else
    {
        pst_mac_vap->st_cap_flag.bit_11ntxbf = OAL_FALSE;
    }
#endif


    if (pst_mac_vap->en_protocol < WLAN_HT_MODE)
    {
        pst_dmac_vap->en_bfee_actived       = OAL_FALSE;
        pst_dmac_vap->en_bfer_actived       = WLAN_BFER_ACTIVED;
        pst_dmac_vap->en_mu_bfee_actived    = OAL_FALSE;
        pst_dmac_vap->en_txstbc_actived     = OAL_FALSE;
    }
    else
    {
        pst_dmac_vap->en_bfee_actived       = OAL_TRUE;
        pst_dmac_vap->en_bfer_actived       = WLAN_BFER_ACTIVED;
        pst_dmac_vap->en_mu_bfee_actived    = OAL_TRUE;
        pst_dmac_vap->en_txstbc_actived     = WLAN_TXSTBC_ACTIVED;
#ifdef _PRE_WLAN_FEATURE_TXOPPS
        if (pst_mac_vap->en_protocol == WLAN_VHT_MODE ||
             pst_mac_vap->en_protocol == WLAN_VHT_ONLY_MODE)
        {
            pst_dmac_vap->en_mu_bfee_actived   = WLAN_MU_BFEE_ACTIVED;
        }
#endif
    }

    hal_disable_machw_phy_and_pa(pst_mac_device->pst_device_stru);

    /* ??hal???????????? */
    hal_set_freq_band(pst_mac_device->pst_device_stru, pst_prot_param->en_band);
#if 0
#ifdef _PRE_WLAN_FEATURE_DFS
    /* mayuan DFS begin */
    /* ???????????????????????????????? */
    if (WLAN_BAND_5G == pst_prot_param->en_band)
    {
        if (0 != pst_mac_device->us_dfs_timeout)
        {
            hal_enable_radar_det(pst_mac_device->pst_device_stru, 0);
            /* ?????????? */
            FRW_TIMER_CREATE_TIMER(&pst_mac_device->st_dfs.st_dfs_radar_timer,
                                   dmac_mgmt_scan_dfs_timeout,
                                   pst_mac_device->us_dfs_timeout,
                                   pst_mac_device,
                                   OAL_FALSE,
                                   OAM_MODULE_ID_DMAC,
                                   pst_mac_device->ul_core_id);
        }
    }
    /* mayuan DFS end */
#endif
#endif
    /* ??hal???????????? */
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
    /*dummy*/
#else
    if (pst_prot_param->en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS)
    {
        OAM_ERROR_LOG0(0, OAM_SF_RX, "{dmac_config_set_mode:: fpga is not support 80M.}\r\n");
        pst_prot_param->en_bandwidth = WLAN_BAND_WIDTH_20M;
    }
#endif
    hal_set_bandwidth_mode(pst_mac_device->pst_device_stru, pst_prot_param->en_bandwidth);

    hal_enable_machw_phy_and_pa(pst_mac_device->pst_device_stru);

    /* ???????? */
    dmac_alg_cfg_bandwidth_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_mac_addr(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_staion_id_param_stru  *pst_param;
    dmac_vap_stru                 *pst_dmac_vap = (dmac_vap_stru *)pst_mac_vap;
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    mac_device_stru               *pst_mac_device;
    hal_to_dmac_device_stru       *pst_hal_device;
#endif
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                     uc_hipriv_ack = OAL_FALSE;
#endif

    pst_param = (mac_cfg_staion_id_param_stru *)puc_param;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifdef _PRE_WLAN_FEATURE_P2P
    /* P2P ????MAC ????mib ??????????P2P DEV ??P2P_CL/P2P_GO,P2P_DEV MAC ??????????p2p0 MIB ?? */
    if (pst_param->en_p2p_mode == WLAN_P2P_DEV_MODE)
    {
        /* ??????p2p0 device????????MAC ??????auc_p2p0_dot11StationID ?????? */
        oal_set_mac_addr(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID,
                        pst_param->auc_station_id);
    }
    else
#endif
    {
        /* ????mib??, Station_ID */
        mac_mib_set_station_id(pst_mac_vap, OAL_SIZEOF(mac_cfg_staion_id_param_stru), puc_param);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PROXYSTA, "{dmac_config_set_mac_addr::mac_res_get_dev pst_mac_device null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hal_device = pst_mac_device->pst_device_stru;

    if (mac_vap_is_vsta(pst_mac_vap))
    {
        /* ????mib??, Station_ID */
        mac_mib_set_station_id(pst_mac_vap, OAL_SIZEOF(mac_cfg_staion_id_param_stru), puc_param);

        /* ??Proxy STA??mac ??????????peer addr ????????,????hal_vap_id??Proxy STA??mac_addr?????????? */
        hal_ce_add_peer_macaddr(pst_hal_device, pst_dmac_vap->pst_hal_vap->uc_vap_id, mac_mib_get_StationID(pst_mac_vap));
    }
    else
    {
        /* hal????mac???? */
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_param->auc_station_id);
        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PROXYSTA,"{dmac_config_set_mac_addr::set mac addr succ!}");
    }
#else
    /* hal????mac???? */
#ifdef _PRE_WLAN_FEATURE_P2P
    if (pst_param->en_p2p_mode == WLAN_P2P_DEV_MODE)
    {
        /* ????p2p0 ????????????????????p2p0_hal_vap ????????????MAC ??????????hal_vap ?? */
        hal_vap_set_macaddr(pst_dmac_vap->pst_p2p0_hal_vap, pst_param->auc_station_id);
    }
    else
#endif
    {
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_param->auc_station_id);
    }
    //OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,"{dmac_config_set_mac_addr::set mac addr succ!}");
#endif

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


oal_uint32  dmac_config_set_freq(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32               l_value;
    mac_device_stru        *pst_mac_device;
    oal_uint8               uc_channel_idx;
    oal_uint32              ul_ret;
#if  defined(_PRE_WLAN_HW_TEST) ||  defined(_PRE_WLAN_FEATURE_EQUIPMENT_TEST)
    mac_cfg_mode_param_stru     st_prot_param;
    oal_uint8                   uc_msg_len = 0;
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_cfg_channel_param_stru l_channel_param;
#endif
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_freq::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    l_value = *((oal_int32 *)puc_param);

#ifdef _PRE_WIFI_DMT
    if (l_value >= 36)
    {
        g_ul_dmt_scan_flag = 1;   /* DMT????????????????????????????????????????????   */
    }
#endif

    ul_ret = mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band, (oal_uint8)l_value, &uc_channel_idx);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{dmac_config_set_freq::mac_get_channel_idx_from_num failed[%d], band[%d], channel num[%d].}", ul_ret, pst_mac_vap->st_channel.en_band, (oal_uint8)l_value);
        return ul_ret;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_mac_vap->st_channel.uc_chan_number = (oal_uint8)l_value;
    pst_mac_vap->st_channel.uc_idx         = uc_channel_idx;
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

#if 0
    pst_mac_device->uc_max_channel = (oal_uint8)l_value;    /* hi1102-cb allways set at both side */
#else
    mac_device_get_channel(pst_mac_device, &l_channel_param);
    l_channel_param.uc_channel = (oal_uint8)l_value;
    mac_device_set_channel(pst_mac_device, &l_channel_param);
#endif

#endif

#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    /* ????????????40M??80M???????? */
    if (WLAN_BAND_WIDTH_40M == pst_mac_vap->st_channel.en_bandwidth ||  WLAN_BAND_WIDTH_80M == pst_mac_vap->st_channel.en_bandwidth)
    {
        if (WLAN_BAND_WIDTH_40M == pst_mac_vap->st_channel.en_bandwidth)
        {
            if (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band)
            {
                pst_mac_vap->st_channel.en_bandwidth = g_ast_channel_bw_map_2G[uc_channel_idx].en_bandwidth_40;
            }
            else if (WLAN_BAND_5G == pst_mac_vap->st_channel.en_band)
            {
                pst_mac_vap->st_channel.en_bandwidth = g_ast_channel_bw_map_5G[uc_channel_idx].en_bandwidth_40;
            }
        }
        else if(WLAN_BAND_WIDTH_80M == pst_mac_vap->st_channel.en_bandwidth)
        {
            if (WLAN_BAND_5G == pst_mac_vap->st_channel.en_band)
            {
                pst_mac_vap->st_channel.en_bandwidth = g_ast_channel_bw_map_5G[uc_channel_idx].en_bandwidth_80;
            }
        }

        /* ????????????????dmac_config_set_mode???? */
        st_prot_param.en_band       = pst_mac_vap->st_channel.en_band;
        st_prot_param.en_bandwidth  = pst_mac_vap->st_channel.en_bandwidth;
        st_prot_param.en_protocol   = pst_mac_vap->en_protocol;
        dmac_config_set_mode(pst_mac_vap, uc_msg_len, (oal_uint8 *)&st_prot_param);
    }
#endif

    /* ???????? */
    dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);

    hal_disable_machw_phy_and_pa(pst_mac_device->pst_device_stru);

#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
    /* ????hal?????????????? */
    hal_set_primary_channel(pst_mac_device->pst_device_stru,
                           (oal_uint8)l_value,
                            pst_mac_vap->st_channel.en_band,
                            uc_channel_idx,
                            pst_mac_vap->st_channel.en_bandwidth);
#else
    if (pst_mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS)
    {
        OAM_ERROR_LOG0(0, OAM_SF_RX, "{dmac_config_set_freq:: fpga is not support 80M.}\r\n");
        /* ????hal?????????????? */
        hal_set_primary_channel(pst_mac_device->pst_device_stru,
                                (oal_uint8)l_value,
                                pst_mac_vap->st_channel.en_band,
                                uc_channel_idx,
                                WLAN_BAND_WIDTH_20M);
    }
    else
    {
        /* ????hal?????????????? */
        hal_set_primary_channel(pst_mac_device->pst_device_stru,
                                (oal_uint8)l_value,
                                pst_mac_vap->st_channel.en_band,
                                uc_channel_idx,
                                pst_mac_vap->st_channel.en_bandwidth);
    }
#endif

    hal_enable_machw_phy_and_pa(pst_mac_device->pst_device_stru);

#ifdef _PRE_WLAN_DFT_EVENT
    oam_event_chan_report((oal_uint8)l_value);
#endif

#ifdef _PRE_WLAN_HW_TEST
    if (HAL_ALWAYS_RX_RESERVED == pst_mac_device->pst_device_stru->bit_al_rx_flag)
    {
        if (WLAN_BAND_2G == (pst_mac_vap->st_channel.en_band) && (WLAN_VHT_MODE == pst_mac_vap->en_protocol))
        {
            hal_set_phy_tx_scale(pst_mac_device->pst_device_stru, OAL_TRUE);

            st_prot_param.en_band       = pst_mac_vap->st_channel.en_band;
            st_prot_param.en_bandwidth  = pst_mac_vap->st_channel.en_bandwidth;
            st_prot_param.en_protocol   = pst_mac_vap->en_protocol;

            dmac_config_set_mode(pst_mac_vap, uc_msg_len, (oal_uint8 *)&st_prot_param);
        }
        else
        {
            hal_set_phy_tx_scale(pst_mac_device->pst_device_stru, OAL_FALSE);
        }
    }
#endif

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_concurrent(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32  l_value;

    l_value = *((oal_int32 *)puc_param);

    mac_res_set_max_asoc_user((oal_uint16)l_value);

    //OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_concurrent::l_value=%d.}", l_value);
    return OAL_SUCC;
}


oal_uint32  dmac_config_set_ssid(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    /* ????mib?? */
    return mac_mib_set_ssid(pst_mac_vap, uc_len, puc_param);
}


oal_uint32  dmac_config_stop_sched_scan(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                         *pst_mac_device;
    mac_pno_sched_scan_mgmt_stru            *pst_pno_scan_mgmt;

    /* ????mac device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SCAN, "{dmac_config_stop_sched_scan::pst_mac_device is null.}");
        return OAL_FAIL;
    }

    /* ????pno??????????????????????????pno??????????????????????null */
    if (OAL_PTR_NULL == pst_mac_device->pst_pno_sched_scan_mgmt)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{dmac_config_stop_sched_scan::pno sched scan already stop.}");
        return OAL_SUCC;
    }

    /* ????pno?????????????? */
    pst_pno_scan_mgmt = pst_mac_device->pst_pno_sched_scan_mgmt;

    /* ????PNO?????????????? */
    dmac_scan_stop_pno_sched_scan_timer((void *)pst_pno_scan_mgmt);

    /* ????????PNO?????????????????????????? */
    if ((MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state) &&
        (WLAN_SCAN_MODE_BACKGROUND_PNO == pst_mac_device->st_scan_params.en_scan_mode))
    {
        /* ?????????????? */
        if (OAL_TRUE == pst_mac_device->st_scan_timer.en_is_registerd)
        {
            FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_mac_device->st_scan_timer);
        }
        dmac_scan_end(pst_mac_device);

        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_stop_sched_scan::stop pno scan.}");

        /* ?????????????????? */
        pst_mac_device->en_curr_scan_state = MAC_SCAN_STATE_IDLE;
    }

    /* ????PNO?????????????? */
    OAL_MEM_FREE(pst_mac_device->pst_pno_sched_scan_mgmt, OAL_TRUE);
    pst_mac_device->pst_pno_sched_scan_mgmt = OAL_PTR_NULL;

    OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_stop_sched_scan::stop schedule scan success.}");

    return OAL_SUCC;
}


oal_uint32  dmac_config_scan_abort(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru     *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_scan_abort::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????????????? */
    if (MAC_SCAN_STATE_RUNNING == pst_mac_device->en_curr_scan_state)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_scan_abort::stop scan.}");
        dmac_scan_abort(pst_mac_device);
    }

    return OAL_SUCC;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)


oal_uint32  dmac_config_set_shortgi(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    shortgi_cfg_stru    *shortgi_cfg;

    shortgi_cfg = (shortgi_cfg_stru *)puc_param;

    switch(shortgi_cfg->uc_shortgi_type)
    {
        case SHORTGI_20_CFG_ENUM:

            if (0 != shortgi_cfg->uc_enable)
            {
                pst_mac_vap->pst_mib_info->st_phy_ht.en_dot11ShortGIOptionInTwentyImplemented = OAL_TRUE;
            }
            else
            {
                pst_mac_vap->pst_mib_info->st_phy_ht.en_dot11ShortGIOptionInTwentyImplemented = OAL_FALSE;
            }
            break;

        case SHORTGI_40_CFG_ENUM:

            if (0 != shortgi_cfg->uc_enable)
            {
                mac_mib_set_ShortGIOptionInFortyImplemented(pst_mac_vap, OAL_TRUE);
            }
            else
            {
                mac_mib_set_ShortGIOptionInFortyImplemented(pst_mac_vap, OAL_FALSE);
            }
            break;

        case SHORTGI_80_CFG_ENUM:

            if (0 != shortgi_cfg->uc_enable)
            {
                pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn80Implemented = OAL_TRUE;
            }
            else
            {
                pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn80Implemented = OAL_FALSE;
            }
            break;
        default:
            break;

    }
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_vap_state_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru *pst_dmac_vap;
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_dmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                        "{dmac_config_vap_state_syn::mac_res_get_dmac_vap fail,vap_id:%u.}",
                        pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    pst_mac_vap->en_vap_state = (mac_vap_state_enum_uint8)(*puc_param);

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
#ifdef _PRE_WLAN_FEATURE_STA_PM
        /* STA ??????????UP??????????keepalive?????? */
        if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) {
            dmac_pm_sta_post_event(pst_dmac_vap, STA_PWR_EVENT_KEEPALIVE, 0, OAL_PTR_NULL);
        }
#endif

        /* STA ??????????UP??????????keepalive?????? */
        if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_UP) {
            /* ????????keepalive */
            pst_dmac_vap->st_vap_base_info.st_cap_flag.bit_keepalive = OAL_FALSE;
        }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
        if ((pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_FAKE_UP) &&
            (pst_dmac_vap->pst_hal_device != OAL_PTR_NULL)) {
            dmac_btcoex_ps_pause_check_and_notify(pst_dmac_vap->pst_hal_device);
        }
#endif
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_STA_PM
#ifdef _PRE_WLAN_FEATURE_STA_UAPSD

OAL_STATIC oal_uint32 dmac_config_set_uapsd_para(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_uapsd_sta_stru   *pst_uapsd_info = (mac_cfg_uapsd_sta_stru *)puc_param;

    /* uc_max_sp_len */
    if (pst_uapsd_info->uc_max_sp_len > 6)
    {
       OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_set_uapsd_para::uc_max_sp_len[%d] > 6!}\r\n", pst_uapsd_info->uc_max_sp_len);
       return OAL_FAIL;
    }

    mac_vap_set_uapsd_para(pst_mac_vap, pst_uapsd_info);

    return OAL_SUCC;
}
#endif
#endif
#endif  /* _PRE_MULTI_CORE_MODE_OFFLOAD_DMAC */


OAL_STATIC oal_uint32  dmac_config_user_asoc_state_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

    mac_h2d_user_asoc_state_stru *pst_user_asoc_info;
    mac_user_stru                *pst_mac_user;

    pst_user_asoc_info = (mac_h2d_user_asoc_state_stru *)puc_param;

    OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_user_asoc_state_syn::us_user_idx = %d, user state = %d.}",
                  pst_user_asoc_info->us_user_idx, pst_user_asoc_info->en_asoc_state);

    /* ????DMAC?????????????? */
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_user_asoc_info->us_user_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_user))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_TX, "{dmac_config_user_asoc_state_syn::pst_mac_user null.user idx [%d]}", pst_user_asoc_info->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????user???????? */
    mac_user_set_asoc_state(pst_mac_user, pst_user_asoc_info->en_asoc_state);
#endif
    return OAL_SUCC;
}



oal_uint32 dmac_config_user_cap_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_user_stru              *pst_mac_user;
    mac_h2d_usr_cap_stru       *pst_mac_h2d_usr_cap;
    pst_mac_h2d_usr_cap = (mac_h2d_usr_cap_stru *)puc_param;
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_h2d_usr_cap->us_user_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_user))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_TX, "{dmac_config_user_cap_syn::pst_mac_user null.user idx [%d]}", pst_mac_h2d_usr_cap->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }


#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    dmac_11w_update_users_status((dmac_vap_stru *)pst_mac_vap, pst_mac_user, pst_mac_h2d_usr_cap->st_user_cap_info.bit_pmf_active);
#endif /* #if(_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT) */

    oal_memcopy(&pst_mac_user->st_cap_info, &pst_mac_h2d_usr_cap->st_user_cap_info, OAL_SIZEOF(mac_h2d_usr_cap_stru));

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_config_btcoex_assoc_state_syn(pst_mac_vap, pst_mac_user);
#endif
#ifdef _PRE_WLAN_FEATURE_SMARTANT
    dmac_config_dual_antenna_vap_check(pst_mac_vap);
#endif

#endif /* (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) */
    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_user_rate_info_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

    mac_h2d_usr_rate_info_stru        *pst_usr_info;
    dmac_user_stru                    *pst_dmac_user;

    if (OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_user_rate_info_syn::the input is null pointer.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_usr_info = (mac_h2d_usr_rate_info_stru *)puc_param;


    /* ????DMAC?????????????? */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(pst_usr_info->us_user_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_TX, "{dmac_config_user_rate_info_syn::pst_dmac_user null.user idx [%d]}", pst_usr_info->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_set_protocol_mode(&(pst_dmac_user->st_user_base_info), pst_usr_info->en_protocol_mode);

    /* ????legacy?????????? */
    mac_user_set_avail_op_rates(&pst_dmac_user->st_user_base_info, pst_usr_info->uc_avail_rs_nrates, pst_usr_info->auc_avail_rs_rates);

    /* ????ht?????????? */
    mac_user_set_ht_hdl(&pst_dmac_user->st_user_base_info, &pst_usr_info->st_ht_hdl);

    /* ????vht?????????? */
    mac_user_set_vht_hdl(&pst_dmac_user->st_user_base_info, &pst_usr_info->st_vht_hdl);

    /* ???????????????? */
    if (pst_dmac_user->st_user_base_info.st_vht_hdl.bit_vht_txop_ps)
    {
        pst_mac_vap->st_cap_flag.bit_txop_ps = OAL_TRUE;
    }
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_sta_usr_info_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_h2d_usr_info_stru           *pst_usr_info;
    mac_user_stru                   *pst_mac_user;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_user_ht_hdl_stru     st_ht_hdl;
#endif
    pst_usr_info = (mac_h2d_usr_info_stru *)puc_param;

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_usr_info->us_user_idx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_user))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_TX, "{dmac_config_sta_usr_info_syn::pst_mac_user null.user idx [%d]}", pst_usr_info->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* ????mac????????????mac?????? */
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        oal_set_mac_addr(pst_mac_user->auc_user_mac_addr, pst_mac_vap->auc_bssid);
    }

    /* ????USR???? */
    mac_user_set_bandwidth_cap(pst_mac_user, pst_usr_info->en_bandwidth_cap);
    mac_user_set_bandwidth_info(pst_mac_user, pst_usr_info->en_avail_bandwidth, pst_usr_info->en_cur_bandwidth);

    mac_user_get_ht_hdl(pst_mac_user, &st_ht_hdl);
    st_ht_hdl.uc_max_rx_ampdu_factor    = pst_usr_info->uc_arg1;
    st_ht_hdl.uc_min_mpdu_start_spacing = pst_usr_info->uc_arg2;
    mac_user_set_ht_hdl(pst_mac_user, &st_ht_hdl);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

    /* ????user pmf?????? */
    dmac_11w_update_users_status((dmac_vap_stru *)pst_mac_vap, pst_mac_user, pst_usr_info->en_user_pmf);
#endif /* #if(_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT) */

    /* ???????????? */
    mac_user_set_avail_protocol_mode(pst_mac_user, pst_usr_info->en_avail_protocol_mode);
    mac_user_set_cur_protocol_mode(pst_mac_user, pst_usr_info->en_avail_protocol_mode);
    mac_user_set_protocol_mode(pst_mac_user, pst_usr_info->en_protocol_mode);
    mac_user_set_asoc_state(pst_mac_user, pst_usr_info->en_user_asoc_state);

    /* ??????slottime */
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        dmac_user_init_slottime(pst_mac_vap, pst_mac_user);
    }

#ifdef _PRE_WLAN_SW_CTRL_RSP
    dmac_user_update_sw_ctrl_rsp(pst_mac_vap, pst_mac_user);
#endif /* _PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV */
#endif /* _PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE */

    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        /* ?????????????????????? */
        dmac_alg_cfg_user_bandwidth_notify(pst_mac_vap, pst_mac_user);
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_vap_info_syn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

    mac_h2d_vap_info_stru   *pst_vap_info;

    pst_vap_info = (mac_h2d_vap_info_stru *)puc_param;

    /* ????vap???? */
    pst_mac_vap->us_sta_aid = pst_vap_info->us_sta_aid;
    mac_vap_set_uapsd_cap(pst_mac_vap, pst_vap_info->uc_uapsd_cap);
#endif
    return OAL_SUCC;
}


oal_uint32  dmac_config_d2h_user_info_syn(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    oal_uint32                  ul_ret = OAL_SUCC;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_d2h_syn_info_stru       st_mac_d2h_info;

    /* ??????????????hmac */
    st_mac_d2h_info.en_avail_bandwidth = pst_mac_user->en_avail_bandwidth;
    st_mac_d2h_info.en_cur_bandwidth   = pst_mac_user->en_cur_bandwidth;
    st_mac_d2h_info.en_bandwidth_cap   = pst_mac_user->en_bandwidth_cap;
    st_mac_d2h_info.us_user_idx        = pst_mac_user->us_assoc_id;

    /* ??????????????hmac */
    oal_memcopy(&(st_mac_d2h_info.st_channel), &(pst_mac_vap->st_channel), OAL_SIZEOF(mac_channel_stru));

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                       "{dmac_config_d2h_user_info_syn::en_avail_bandwidth:%d,en_cur_bandwidth:%d}",
                         pst_mac_user->en_avail_bandwidth,
                         pst_mac_user->en_cur_bandwidth);
    /***************************************************************************
        ????????HMAC??, ????USER??????????HMAC
    ***************************************************************************/
    ul_ret = dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_USR_INFO_SYN, OAL_SIZEOF(st_mac_d2h_info), (oal_uint8 *)(&st_mac_d2h_info));
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_d2h_user_info_syn::dmac_send_sys_event failed[%d],user_id[%d].}",
                    ul_ret, pst_mac_user->us_assoc_id);
    }
#endif

    return ul_ret;
}

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ

oal_uint32 dmac_config_set_device_freq(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    dmac_freq_control_stru *device_freq_handle;
    config_device_freq_h2d_stru   *pst_device_freq;
    oal_uint8               uc_index;
    device_pps_freq_level_stru*    device_ba_pps_freq_level;

    pst_device_freq = (config_device_freq_h2d_stru *)puc_param;
    device_freq_handle = dmac_get_auto_freq_handle();
    device_ba_pps_freq_level = dmac_get_ba_pps_freq_level();

    /* ?????????????? */
    if (FREQ_SYNC_DATA == pst_device_freq->uc_set_type)
    {
        for(uc_index = 0; uc_index < 4; uc_index++)
        {
            device_ba_pps_freq_level->ul_speed_level = pst_device_freq->st_device_data[uc_index].ul_speed_level;
            device_ba_pps_freq_level->ul_cpu_freq_level = pst_device_freq->st_device_data[uc_index].ul_cpu_freq_level;
            device_ba_pps_freq_level++;
        }
    }
    else if(FREQ_SET_FREQ == pst_device_freq->uc_set_type)/* ?????????????? */
    {
        device_freq_handle->uc_req_freq_level = pst_device_freq->uc_set_freq;
        dmac_auto_set_device_freq();
    }
    else if(FREQ_SET_MODE == pst_device_freq->uc_set_type)
    {
        /* ?????????????? */
       if (OAL_FALSE == pst_device_freq->uc_device_freq_enable)
       {
           dmac_set_auto_freq_deinit();

           /* ??????device?????????????????? */
           device_freq_handle->uc_req_freq_level = FREQ_HIGHEST;
           dmac_auto_set_device_freq();
       }
       else
       {
            dmac_set_auto_freq_init();

            /* ?????????????????? */
            for(uc_index = 0; uc_index < 4; uc_index++)
            {
                device_ba_pps_freq_level->ul_speed_level = pst_device_freq->st_device_data[uc_index].ul_speed_level;
                device_ba_pps_freq_level->ul_cpu_freq_level = pst_device_freq->st_device_data[uc_index].ul_cpu_freq_level;
                device_ba_pps_freq_level++;
            }
       }

       OAM_WARNING_LOG1(0, OAM_SF_RSSI,"{dmac_config_set_device_freq::enable mode[%d][1:enable,0:disable].}", device_freq_handle->uc_auto_freq_enable);
    }
    else if (FREQ_GET_FREQ == pst_device_freq->uc_set_type)
    {

        OAM_WARNING_LOG1(0, OAM_SF_RSSI,"{dmac_config_set_device_freq::cpu level[%d].}", PM_WLAN_GetMaxCpuFreq());
        for(uc_index = 0; uc_index < 4; uc_index++)
        {
            OAM_WARNING_LOG2(0, OAM_SF_RSSI,"{dmac_config_set_device_freq::devive pkts[%d]freq level[%d]].}",
            device_ba_pps_freq_level->ul_speed_level, device_ba_pps_freq_level->ul_cpu_freq_level);
            device_ba_pps_freq_level++;
        }
    }
#endif

    return OAL_SUCC;
}
#endif

oal_void  dmac_config_get_tx_rate_info(hal_tx_txop_alg_stru     *pst_tx_alg,
                                                        mac_data_rate_stru       *pst_mac_rates_11g,
                                                        mac_rate_info_stru       *pst_rate_info)
{
    oal_uint32                          ul_loop = 0;
    oal_uint8                           uc_tx_dscr_protocol_type = 0;
    wlan_legacy_rate_value_enum_uint8   en_legacy_rate = 0;

    /* ?????????? */
    OAL_MEMZERO(pst_rate_info, OAL_SIZEOF(*pst_rate_info));

    /* ??????protocol type??????????0-3,????0??11b type, 1??legacy OFDM type??2??HT type??3??VHT type */
    uc_tx_dscr_protocol_type = pst_tx_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode;
    switch(uc_tx_dscr_protocol_type)
    {
        /* 11b or legacy OFDM type */
        case 0:
        case 1:
            en_legacy_rate = pst_tx_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_legacy_rate;

            for (ul_loop = 0; ul_loop < MAC_DATARATES_PHY_80211G_NUM; ul_loop++)
            {
                if (en_legacy_rate == pst_mac_rates_11g[ul_loop].uc_phy_rate)
                {
                    pst_rate_info->legacy = pst_mac_rates_11g[ul_loop].uc_mbps;
                    break;
                }
            }

            /* ??????????????legacy???????????????????????? */
            if (ul_loop >= MAC_DATARATES_PHY_80211G_NUM)
            {
                OAM_ERROR_LOG2(0, OAM_SF_ANY,
                               "{dmac_config_get_tx_rate_info::protocol_type[%d], legacy_rate[%d] is invalid, may be software config error.}",
                               uc_tx_dscr_protocol_type, en_legacy_rate);
            }
            break;

        /* HT type */
        case 2:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_MCS;
            pst_rate_info->mcs = pst_tx_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_ht_rate.bit_ht_mcs;
            if (pst_tx_alg->ast_per_rate[0].rate_bit_stru.bit_short_gi_enable)
            {
                pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_SHORT_GI;
            }
            break;

        /* VHT type */
        case 3:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_VHT_MCS;
            pst_rate_info->mcs = pst_tx_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_vht_nss_mcs.bit_vht_mcs;
            if (pst_tx_alg->ast_per_rate[0].rate_bit_stru.bit_short_gi_enable)
            {
                pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_SHORT_GI;
            }

            /* ????nss mode */
            pst_rate_info->nss = pst_tx_alg->ast_per_rate[0].rate_bit_stru.un_nss_rate.st_vht_nss_mcs.bit_nss_mode + 1;
            break;

        default:
            OAM_ERROR_LOG1(0, OAM_SF_RSSI,
                           "{dmac_config_get_tx_rate_info:: protocol type[%d] invalid, software config error.}",
                           uc_tx_dscr_protocol_type);
            break;
    }

    /* ???????????????????????????? */
    switch (pst_tx_alg->st_rate.uc_channel_bandwidth)
    {
        case WLAN_BAND_ASSEMBLE_40M:
        case WLAN_BAND_ASSEMBLE_40M_DUP:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_40_MHZ_WIDTH;
            break;

        case WLAN_BAND_ASSEMBLE_80M:
        case WLAN_BAND_ASSEMBLE_80M_DUP:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_80_MHZ_WIDTH;
            break;

        case WLAN_BAND_ASSEMBLE_160M:
        case WLAN_BAND_ASSEMBLE_160M_DUP:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_160_MHZ_WIDTH;
            break;

        case WLAN_BAND_ASSEMBLE_80M_80M:
            pst_rate_info->flags |= MAC_RATE_INFO_FLAGS_80P80_MHZ_WIDTH;
            break;

        default:
            /* do nothing */
            break;
    }

    OAM_INFO_LOG4(0, OAM_SF_ANY,
                     "{dmac_config_get_tx_rate_info::protocol_type[%d],legacy_rate[%d],short_gi[%d],bandwidth[%d].}",
                     uc_tx_dscr_protocol_type, en_legacy_rate,
                     pst_tx_alg->ast_per_rate[0].rate_bit_stru.bit_short_gi_enable,
                     pst_tx_alg->st_rate.uc_channel_bandwidth);

    OAM_INFO_LOG4(0, OAM_SF_ANY,
                     "{dmac_config_get_tx_rate_info::legacy[%d]:mcs[%d]:flags[%d]:nss[%d].}",
                     pst_rate_info->legacy, pst_rate_info->mcs, pst_rate_info->flags, pst_rate_info->nss);

    return;
}


OAL_STATIC oal_uint32  dmac_config_proc_query_sta_info_event(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                         *pst_mac_device;
    dmac_vap_stru                           *pst_dmac_vap;
    dmac_user_stru                          *pst_dmac_user;
    dmac_query_request_event                *pst_query_requset_event;
    dmac_query_station_info_response_event   st_query_station_info;
#ifdef _PRE_WLAN_FEATURE_STA_PM
    mac_sta_pm_handler_stru     *pst_mac_sta_pm_handle;
#endif
    dmac_device_stru                  *pst_dmac_device;

    pst_query_requset_event     = (dmac_query_request_event *)puc_param;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{dmac_config_proc_query_sta_info_event::pst_mac_device is null, device_id[%d].}",
                         pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    if (OAL_QUERY_STATION_INFO_EVENT == pst_query_requset_event->query_event)
    {
        pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
        if (OAL_PTR_NULL == pst_dmac_vap)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_proc_query_sta_info_event::pst_dmac_vap null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
        {
            /*station_info_extend????*/
#ifdef _PRE_WLAN_FEATURE_STA_PM
            pst_mac_sta_pm_handle = (mac_sta_pm_handler_stru *)(pst_dmac_vap->pst_pm_handler);
            if (OAL_PTR_NULL == pst_mac_sta_pm_handle)
            {
                OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{dmac_config_proc_query_sta_info_event::pst_mac_sta_pm_handle null}");
                return OAL_ERR_CODE_PTR_NULL;
            }
            st_query_station_info.st_station_info_extend.ul_bcn_cnt = pst_mac_sta_pm_handle->aul_pmDebugCount[PM_MSG_PSM_BEACON_CNT];
            st_query_station_info.st_station_info_extend.ul_bcn_tout_cnt = pst_mac_sta_pm_handle->aul_pmDebugCount[PM_MSG_BEACON_TIMEOUT_CNT];
#else
            st_query_station_info.st_station_info_extend.ul_bcn_cnt = 0;
            st_query_station_info.st_station_info_extend.ul_bcn_tout_cnt = 0;
#endif
            pst_dmac_device = dmac_res_get_mac_dev(pst_mac_device->uc_device_id);
            if (OAL_PTR_NULL == pst_dmac_device)
            {
                OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_proc_query_sta_info_event::pst_dmac_device null.}");
                return OAL_ERR_CODE_PTR_NULL;
            }
            st_query_station_info.st_station_info_extend.uc_distance = pst_dmac_device->st_dmac_alg_stat.en_dmac_device_distance_enum;
            st_query_station_info.st_station_info_extend.uc_cca_intr = pst_dmac_device->st_dmac_alg_stat.en_cca_intf_state;

            /* sta???????????? */
            st_query_station_info.query_event        = pst_query_requset_event->query_event;
            /*????????????RSSI*/
            st_query_station_info.ul_signal          = pst_dmac_vap->st_query_stats.ul_signal;
            /*????????????????????????????????????????????*/
            st_query_station_info.ul_rx_packets      = pst_dmac_vap->st_query_stats.ul_drv_rx_pkts;
            st_query_station_info.ul_tx_packets      = pst_dmac_vap->st_query_stats.ul_hw_tx_pkts;
            st_query_station_info.ul_rx_bytes        = pst_dmac_vap->st_query_stats.ul_drv_rx_bytes;
            st_query_station_info.ul_tx_bytes        = pst_dmac_vap->st_query_stats.ul_hw_tx_bytes;
            st_query_station_info.ul_tx_failed       = pst_dmac_vap->st_query_stats.ul_tx_failed;
            st_query_station_info.ul_tx_retries      = pst_dmac_vap->st_query_stats.ul_tx_retries;
            st_query_station_info.ul_rx_dropped_misc = pst_dmac_vap->st_query_stats.ul_rx_dropped_misc;

        }
        else if(WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
        {
            pst_dmac_user = mac_vap_get_dmac_user_by_addr(pst_mac_vap, pst_query_requset_event->auc_query_sta_addr);
            if (OAL_PTR_NULL == pst_dmac_user)
            {
                OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_proc_query_sta_info_event::pst_dmac_user null.}");
                return OAL_ERR_CODE_PTR_NULL;
            }

            /* sta???????????? */
            st_query_station_info.query_event        = pst_query_requset_event->query_event;
            /*????????????RSSI*/
            st_query_station_info.ul_signal          = pst_dmac_user->st_query_stats.ul_signal;

            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                             "{dmac_config_proc_query_sta_info_event::get rssi=%d in ap mode!", st_query_station_info.ul_signal );

            /*????????????????????????????????????????????*/
            st_query_station_info.ul_rx_packets      = pst_dmac_user->st_query_stats.ul_drv_rx_pkts;
            st_query_station_info.ul_tx_packets      = pst_dmac_user->st_query_stats.ul_hw_tx_pkts;
            st_query_station_info.ul_rx_bytes        = pst_dmac_user->st_query_stats.ul_drv_rx_bytes;
            st_query_station_info.ul_tx_bytes        = pst_dmac_user->st_query_stats.ul_hw_tx_bytes;
            st_query_station_info.ul_tx_failed       = pst_dmac_user->st_query_stats.ul_tx_failed;
            st_query_station_info.ul_tx_retries      = pst_dmac_user->st_query_stats.ul_tx_retries;
            st_query_station_info.ul_rx_dropped_misc = pst_dmac_user->st_query_stats.ul_rx_dropped_misc;
        }
        else
        {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_RSSI,
                             "{dmac_config_proc_query_sta_info_event::vap_mode[%d] don't support query statistics info.}",
                             pst_mac_vap->en_vap_mode);
            return OAL_FAIL;
        }

        /* ???????????? */
        dmac_config_get_tx_rate_info(&(pst_dmac_vap->st_tx_alg), &(pst_mac_device->st_mac_rates_11g[0]), &(st_query_station_info.st_txrate));

        dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_QUERY_STATION_STATS, OAL_SIZEOF(dmac_query_station_info_response_event), (oal_uint8 *)&st_query_station_info);

    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_query_rssi(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_user_stru             *pst_dmac_user;
    mac_cfg_query_rssi_stru    *pst_param;

    pst_param = (mac_cfg_query_rssi_stru *)puc_param;

    pst_dmac_user = mac_res_get_dmac_user(pst_param->us_user_id);
    if (OAL_PTR_NULL == pst_dmac_user)
    {
        pst_param->c_rssi       = -127;
    }
    else
    {
        pst_param->c_rssi       = pst_dmac_user->c_rx_rssi;
    }

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_rssi_stru), (oal_uint8 *)pst_param);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_query_rate(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_user_stru                      *pst_mac_user;
    dmac_user_stru                     *pst_dmac_user;
    mac_cfg_query_rate_stru            *pst_param;
    dmac_tx_normal_rate_stats_stru     *pst_rate;

    pst_param = (mac_cfg_query_rate_stru *)puc_param;

    pst_mac_user  = mac_res_get_mac_user(pst_param->us_user_id);
    pst_dmac_user = mac_res_get_dmac_user(pst_param->us_user_id);
    if (OAL_PTR_NULL == pst_mac_user || OAL_PTR_NULL == pst_dmac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_query_rate: dmac user is null ptr. user id:%d", pst_param->us_user_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    dmac_tid_get_normal_rate_stats(pst_mac_user, 0, &pst_rate);

    pst_param->ul_tx_rate       = pst_rate->ul_rate_kbps;
    pst_param->ul_rx_rate       = pst_dmac_user->ul_rx_rate;
#ifdef _PRE_WLAN_DFT_STAT
    pst_param->uc_cur_per       = pst_rate->uc_per;
    pst_param->uc_bestrate_per  = pst_rate->uc_best_rate_per;
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    pst_param->ul_tx_rate_min   = pst_dmac_user->ul_tx_minrate;
    pst_param->ul_tx_rate_max   = pst_dmac_user->ul_tx_maxrate;
    pst_param->ul_rx_rate_min   = pst_dmac_user->ul_rx_rate_min;
    pst_param->ul_rx_rate_max   = pst_dmac_user->ul_rx_rate_max;

    pst_dmac_user->ul_rx_rate_min = 0;
    pst_dmac_user->ul_rx_rate_max = 0;
#endif

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_QUERY_RATE, OAL_SIZEOF(mac_cfg_query_rate_stru), (oal_uint8 *)pst_param);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_DFT_STAT

OAL_STATIC oal_uint32  dmac_config_query_ani(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_device_stru           *pst_dmac_device;
    mac_cfg_query_ani_stru     *pst_param;

    pst_param = (mac_cfg_query_ani_stru *)puc_param;

    pst_dmac_device = dmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_dmac_device)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_query_ani: dmac device is null ptr. device id:%d", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_param->uc_device_distance       = pst_dmac_device->st_dmac_alg_stat.en_dmac_device_distance_enum;
    pst_param->uc_intf_state_cca        = pst_dmac_device->st_dmac_alg_stat.en_cca_intf_state;
    pst_param->uc_intf_state_co         = pst_dmac_device->st_dmac_alg_stat.en_co_intf_state;

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_QUERY_ANI, OAL_SIZEOF(mac_cfg_query_ani_stru), (oal_uint8 *)pst_param);

    return OAL_SUCC;
}

#endif

#ifdef _PRE_WLAN_FEATURE_DFS

OAL_STATIC oal_uint32  dmac_config_dfs_radartool(mac_vap_stru *pst_mac_vap, oal_uint8 us_len, oal_uint8 *puc_param)
{
#ifndef _PRE_WLAN_PROFLING_MIPS
    mac_device_stru       *pst_mac_device;
    oal_int8              *pc_token;
    oal_int8              *pc_end;
    oal_int8              *pc_ctx;
    oal_int8              *pc_sep = " ";
    oal_bool_enum_uint8    en_val;
    oal_uint32             ul_val;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????????? */
    pc_token = oal_strtok((oal_int8 *)puc_param, pc_sep, &pc_ctx);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (0 == oal_strcmp(pc_token, "dfsenable"))
    {
        /* ????DFS????????*/
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        en_val = (oal_bool_enum_uint8)oal_strtol(pc_token, &pc_end, 10);

        mac_dfs_set_dfs_enable(pst_mac_device, en_val);
    }
    else if (0 == oal_strcmp(pc_token, "cacenable"))
    {
        /* ????CAC????????????*/
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        en_val = (oal_bool_enum_uint8)oal_strtol(pc_token, &pc_end, 10);

        mac_dfs_set_cac_enable(pst_mac_device, en_val);
    }
    else if (0 == oal_strcmp(pc_token, "dfsdebug"))
    {
        /* ????debug level */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);

        mac_dfs_set_debug_level(pst_mac_device, (oal_uint8)ul_val);
    }
    else if(0 == oal_strcmp(pc_token, "offchannum"))
    {
        /* ????OFF-CHAN CAC????????*/
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_bool_enum_uint8)oal_strtol(pc_token, &pc_end, 10);

        mac_dfs_set_offchan_number(pst_mac_device, ul_val);
    }
    else if(0 == oal_strcmp(pc_token, "ctsdura"))
    {
        /* ????cts duration */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        pst_mac_device->st_dfs.st_dfs_info.uc_cts_duration = (oal_uint8)oal_strtol(pc_token, &pc_end, 10);
    }
    else if(0 == oal_strcmp(pc_token, "radarfilter"))
    {
        /* ???????????? chirp_enable */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 10);

        pst_mac_device->pst_device_stru->st_dfs_radar_filter.en_chirp_enable = (oal_bool_enum_uint8)ul_val;

        /* ???????????? chirp_cnt */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 10);

        pst_mac_device->pst_device_stru->st_dfs_radar_filter.ul_chirp_cnt_threshold = ul_val;

        /* ???????????? chirp threshold */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 10);

        pst_mac_device->pst_device_stru->st_dfs_radar_filter.ul_chirp_time_threshold = ul_val;

        /* ???????????? threshold */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 10);

        pst_mac_device->pst_device_stru->st_dfs_radar_filter.ul_time_threshold = ul_val;

        pst_mac_device->pst_device_stru->st_dfs_radar_filter.ul_last_burst_timestamp = 0;
        pst_mac_device->pst_device_stru->st_dfs_radar_filter.ul_last_burst_timestamp_for_chirp = 0;
    }
    else if(0 == oal_strcmp(pc_token, "enabletimer"))
    {
        /* ?????????????????????? */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pc_token))
        {
            return OAL_ERR_CODE_PTR_NULL;
        }

        ul_val = (oal_bool_enum_uint8)oal_strtol(pc_token, &pc_end, 10);
        pst_mac_device->us_dfs_timeout = (oal_uint16)ul_val;
        //OAM_INFO_LOG1(0, OAM_SF_DFS, "[DFS]enable timer: %d. ", pst_mac_device->us_dfs_timeout);
    }
    else if(0 == oal_strcmp(pc_token, "offchanenable"))
    {
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
        /* 1102 DBAC todo ????????DBAC??????GO??????beacon??*/
        hal_vap_start_tsf(((dmac_vap_stru *)pst_mac_vap)->pst_hal_vap, OAL_FALSE);
#endif
    }
    else
    {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }
#endif
    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32  dmac_config_set_shpreamble(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    /* ????mib?? */
    mac_mib_set_shpreamble(pst_mac_vap, uc_len, puc_param);
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_MONITOR

OAL_STATIC oal_uint32  dmac_config_set_addr_filter(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint8        uc_value;
    mac_device_stru *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_get_cmd_one_arg::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_value = (*((oal_int32 *)puc_param) == 0) ? 0 : 1;

    if (0 != uc_value)
    {
        /* ????monitor?????????????????????? */
        hal_enable_monitor_mode(pst_mac_device->pst_device_stru);
    }
    else
    {
        /* ????monitor???????????????????? */
        hal_disable_monitor_mode(pst_mac_device->pst_device_stru);
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC oal_uint32  dmac_config_set_prot_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32 l_value;

    l_value = *((oal_int32 *)puc_param);

    pst_mac_vap->st_protection.en_protection_mode = (oal_uint8)l_value;

    return OAL_SUCC;
}


oal_uint32  dmac_config_set_bintval(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru  *pst_dmac_vap;
    oal_int32       l_value;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_device_stru             *pst_mac_device;
    oal_uint8                    uc_vap_idx;
    mac_vap_stru*               pst_vap;
#endif

    l_value = *((oal_int32 *)puc_param);
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_mac_device              = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hmac_config_set_bintval::pst_mac_device[%d] is NULL!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_device_set_beacon_interval(pst_mac_device, *((oal_uint32 *)puc_param));

    /* ????device??????vap */
    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++)
    {
        pst_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (OAL_PTR_NULL == pst_vap)
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_config_set_bintval::pst_mac_vap(%d) null.}",
                           pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        /* ????AP VAP????beacon interval */
        if ((WLAN_VAP_MODE_BSS_AP == pst_vap->en_vap_mode))
        {
             /* ????mib?? */
            mac_mib_set_beacon_period(pst_vap, (oal_uint8)uc_len, puc_param);
        }
    }
#endif

    hal_vap_set_machw_beacon_period(pst_dmac_vap->pst_hal_vap, (oal_uint16)l_value);
    return OAL_SUCC;
}


oal_uint32  dmac_config_set_dtimperiod(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* ????mib?? */
    mac_mib_set_dtim_period(pst_mac_vap, uc_len, puc_param);
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_nobeacon(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint8            uc_value;
    dmac_vap_stru       *pst_dmac_vap;

    uc_value      = (*((oal_int32 *)puc_param) == 0) ? 0 : 1;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_nobeacon::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (0 != uc_value)
    {
        hal_vap_beacon_suspend(pst_dmac_vap->pst_hal_vap);
    }
    else
    {
        hal_vap_beacon_resume(pst_dmac_vap->pst_hal_vap);
    }

    //OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_set_nobeacon succ!");

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_txchain(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32           l_value;
    mac_device_stru    *pst_mac_device;

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_txchain::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    l_value = *((oal_int32 *)puc_param);

#if 0
    pst_mac_device->uc_tx_chain = (oal_uint8)l_value;
#else
    mac_device_set_txchain(pst_mac_device, (oal_uint8)l_value);
#endif

    /* ???????????????????????????????? */
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_rxchain(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32           l_value;
    mac_device_stru    *pst_mac_device;

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_rxchain::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    l_value = *((oal_int32 *)puc_param);

#if 0
    pst_mac_device->uc_rx_chain = (oal_uint8)l_value;
#else
    mac_device_set_rxchain(pst_mac_device, (oal_uint8)l_value);
#endif
    /* ????HAL???? */
    hal_set_rx_multi_ant(pst_mac_device->pst_device_stru, pst_mac_device->uc_rx_chain);
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_txpower(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int32                       l_value;
    mac_regclass_info_stru          *pst_regdom_info;
    oal_uint8                       uc_cur_ch_num;
    wlan_channel_band_enum_uint8    en_freq_band;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    l_value = *((oal_int32 *)puc_param);

    mac_vap_set_tx_power(pst_mac_vap, (oal_uint8)l_value);

    /* ????????????????????????TPC????????????*/
    en_freq_band  = pst_mac_vap->st_channel.en_band;
    uc_cur_ch_num = pst_mac_vap->st_channel.uc_chan_number;

    pst_regdom_info = mac_get_channel_num_rc_info(en_freq_band, uc_cur_ch_num);
    if (OAL_PTR_NULL == pst_regdom_info)
    {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_txpower::this channel isnot support by this country.freq_band = %d,cur_ch_num = %d}",
                            en_freq_band,uc_cur_ch_num);
    }
    else
    {
        pst_regdom_info->uc_max_reg_tx_pwr = (oal_uint8)l_value;
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_config_set_txpower::max_reg_tx_pwr[%d]!}\r\n", pst_regdom_info->uc_max_reg_tx_pwr);
    }

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_set_cwmin(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32 *pul_param;
    oal_uint8   uc_cwmin;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    uc_cwmin  = (oal_uint8)pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_cwmin::invalid param, ul_ac=%d uc_cwmin=%d.", ul_ac, uc_cwmin);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].ul_dot11EDCATableCWmin = uc_cwmin;
    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_set_cwmax(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32 *pul_param;
    oal_uint8   uc_cwmax;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    uc_cwmax  = (oal_uint8)pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_cwmax::invalid param, ul_ac=%d uc_cwmax=%d.", ul_ac, uc_cwmax);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].ul_dot11EDCATableCWmax = (oal_uint32)uc_cwmax;

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_aifsn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_aifsn::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].ul_dot11EDCATableAIFSN = ul_value;

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_set_txop_limit(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_txop_limit::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].ul_dot11EDCATableTXOPLimit = ul_value;

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_set_msdu_lifetime(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_msdu_lifetime::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].ul_dot11EDCATableMSDULifetime = ul_value;

    return OAL_SUCC;
}




OAL_STATIC oal_uint32  dmac_config_set_edca_mandatory(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_edca_mandatory::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->ast_wlan_mib_edca[ul_ac].en_dot11EDCATableMandatory = (oal_uint8)ul_value;

    return OAL_SUCC;
}



oal_uint32  dmac_config_set_qap_cwmin(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32 *pul_param;
    oal_uint8   uc_cwmax = 0;
    oal_uint8   uc_cwmin = 0;
    oal_uint8   uc_cwmin_pre = 0;

    dmac_vap_stru               *pst_dmac_vap;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    uc_cwmin  = (oal_uint8)pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_cwmin::invalid param, ul_ac=%d uc_cwmin=%d.", ul_ac, uc_cwmin);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].ul_dot11QAPEDCATableCWmin = uc_cwmin;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_cwmin::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }


    hal_vap_get_edca_machw_cw(pst_dmac_vap->pst_hal_vap, &uc_cwmax, &uc_cwmin_pre, (oal_uint8)ul_ac);
    hal_vap_set_edca_machw_cw(pst_dmac_vap->pst_hal_vap, uc_cwmax, uc_cwmin, (oal_uint8)ul_ac);

    return OAL_SUCC;
}



oal_uint32  dmac_config_set_qap_cwmax(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32 *pul_param;
    oal_uint8   uc_cwmax = 0;
    oal_uint8   uc_cwmin = 0;
    oal_uint8   uc_cwmax_pre = 0;

    dmac_vap_stru               *pst_dmac_vap;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    uc_cwmax  = (oal_uint8)pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_cwmax::invalid param, ul_ac=%d uc_cwmax=%d.", ul_ac, uc_cwmax);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].ul_dot11QAPEDCATableCWmax = uc_cwmax;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_cwmax::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    hal_vap_get_edca_machw_cw(pst_dmac_vap->pst_hal_vap, &uc_cwmax_pre, &uc_cwmin, (oal_uint8)ul_ac);
    hal_vap_set_edca_machw_cw(pst_dmac_vap->pst_hal_vap, uc_cwmax, uc_cwmin, (oal_uint8)ul_ac);

    return OAL_SUCC;
}


oal_uint32  dmac_config_set_qap_aifsn(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;
    dmac_vap_stru               *pst_dmac_vap;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_aifsn::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].ul_dot11QAPEDCATableAIFSN = ul_value;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_aifsn::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    hal_vap_set_machw_aifsn_ac(pst_dmac_vap->pst_hal_vap, (wlan_wme_ac_type_enum_uint8)ul_ac, (oal_uint8)ul_value);

    return OAL_SUCC;
}




oal_uint32  dmac_config_set_qap_txop_limit(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;
    oal_uint16  us_txop_bk = 0;
    oal_uint16  us_txop_be = 0;
    oal_uint16  us_txop_vi = 0;
    oal_uint16  us_txop_vo = 0;
    oal_uint16  us_pre_value = 0;

    dmac_vap_stru               *pst_dmac_vap;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_txop_limit::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].ul_dot11QAPEDCATableTXOPLimit = ul_value;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_txop_limit::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }


    switch (ul_ac)
    {
        case WLAN_WME_AC_BK:
            hal_vap_get_machw_txop_limit_bkbe(pst_dmac_vap->pst_hal_vap, &us_txop_be, &us_pre_value);
            hal_vap_set_machw_txop_limit_bkbe(pst_dmac_vap->pst_hal_vap, us_txop_be, (oal_uint16)ul_value);
            break;

        case WLAN_WME_AC_BE:
            hal_vap_get_machw_txop_limit_bkbe(pst_dmac_vap->pst_hal_vap, &us_pre_value, &us_txop_bk);
            hal_vap_set_machw_txop_limit_bkbe(pst_dmac_vap->pst_hal_vap, (oal_uint16)ul_value, us_txop_bk);
            break;

        case WLAN_WME_AC_VI:
            hal_vap_get_machw_txop_limit_vivo(pst_dmac_vap->pst_hal_vap, &us_txop_vo, &us_pre_value);
            hal_vap_set_machw_txop_limit_vivo(pst_dmac_vap->pst_hal_vap, us_txop_vo, (oal_uint16)ul_value);
            break;

        case WLAN_WME_AC_VO:
            hal_vap_get_machw_txop_limit_vivo(pst_dmac_vap->pst_hal_vap, &us_pre_value, &us_txop_vi);
            hal_vap_set_machw_txop_limit_vivo(pst_dmac_vap->pst_hal_vap, (oal_uint16)ul_value, us_txop_vi);
            break;

        default:
            break;
    }

    return OAL_SUCC;
}



oal_uint32  dmac_config_set_qap_msdu_lifetime(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    oal_uint16  us_lifetime_bk = 0;
    oal_uint16  us_lifetime_be = 0;
    oal_uint16  us_lifetime_vi = 0;
    oal_uint16  us_lifetime_vo = 0;
    oal_uint16  us_pre_value = 0;

    dmac_vap_stru               *pst_dmac_vap;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_msdu_lifetime::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].ul_dot11QAPEDCATableMSDULifetime = ul_value;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_wfa_cfg_aifsn::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }


    switch (ul_ac)
    {
        case WLAN_WME_AC_BK:
            hal_vap_get_machw_edca_bkbe_lifetime(pst_dmac_vap->pst_hal_vap, &us_lifetime_be, &us_pre_value);
            hal_vap_set_machw_edca_bkbe_lifetime(pst_dmac_vap->pst_hal_vap, us_lifetime_be, (oal_uint16)ul_value);
            break;

        case WLAN_WME_AC_BE:
            hal_vap_get_machw_edca_bkbe_lifetime(pst_dmac_vap->pst_hal_vap, &us_pre_value, &us_lifetime_bk);
            hal_vap_set_machw_edca_bkbe_lifetime(pst_dmac_vap->pst_hal_vap, (oal_uint16)ul_value, us_lifetime_bk);
            break;

        case WLAN_WME_AC_VI:
            hal_vap_get_machw_edca_vivo_lifetime(pst_dmac_vap->pst_hal_vap, &us_lifetime_vo, &us_pre_value);
            hal_vap_set_machw_edca_vivo_lifetime(pst_dmac_vap->pst_hal_vap, us_lifetime_vo, (oal_uint16)ul_value);
            break;

        case WLAN_WME_AC_VO:
            hal_vap_get_machw_edca_vivo_lifetime(pst_dmac_vap->pst_hal_vap, &us_pre_value, &us_lifetime_vi);
            hal_vap_set_machw_edca_vivo_lifetime(pst_dmac_vap->pst_hal_vap, (oal_uint16)ul_value, us_lifetime_vi);
            break;

        default:
            break;
    }

    return OAL_SUCC;
}




OAL_STATIC oal_uint32  dmac_config_set_qap_edca_mandatory(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32  ul_ac;
    oal_uint32  ul_value;
    oal_uint32 *pul_param;

    pul_param = (oal_uint32 *)puc_param;

    ul_ac     = pul_param[1];
    ul_value  = pul_param[2];

    if (ul_ac >= WLAN_WME_AC_BUTT)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_qap_edca_mandatory::invalid param, ul_ac=%d ul_value=%d.", ul_ac, ul_value);
        return OAL_FAIL;
    }

    pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[ul_ac].en_dot11QAPEDCATableMandatory = (oal_uint8)ul_value;

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_channel(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_channel_param_stru      *pst_channel_param;
    mac_device_stru                 *pst_mac_device;
    oal_uint8                        uc_channel_idx = 0;
    oal_uint32                       ul_beacon_rate;
    mac_vap_stru                    *pst_vap_up;

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_channel::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_channel_param = (mac_cfg_channel_param_stru *)puc_param;

    mac_get_channel_idx_from_num(pst_channel_param->en_band, pst_channel_param->uc_channel, &uc_channel_idx);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_mac_vap->st_channel.uc_chan_number  = pst_channel_param->uc_channel;
    pst_mac_vap->st_channel.en_band         = pst_channel_param->en_band;
    pst_mac_vap->st_channel.en_bandwidth    = pst_channel_param->en_bandwidth;

    pst_mac_vap->st_channel.uc_idx = uc_channel_idx;

    if(MAC_VAP_STATE_UP != pst_mac_vap->en_vap_state)
    {
        pst_mac_vap->en_vap_state = MAC_VAP_STATE_AP_WAIT_START;
    }

    //OAM_INFO_LOG3(0, OAM_SF_2040, "dmac_config_set_channel::chan=%d band=%d bandwidth=%d\r\n", pst_channel_param->uc_channel, pst_channel_param->en_band,
      //                                            pst_channel_param->en_bandwidth);
    /* ????????????????Mib */
    mac_vap_change_mib_by_bandwidth(pst_mac_vap, pst_channel_param->en_bandwidth);

    mac_device_set_channel(pst_mac_device, pst_channel_param);
#endif

    /* ????beacon?????????? */
    if ((WLAN_BAND_2G == pst_channel_param->en_band) || (WLAN_BAND_5G == pst_channel_param->en_band))
    {
        ul_beacon_rate = ((dmac_vap_stru *)pst_mac_vap)->ast_tx_mgmt_ucast[pst_channel_param->en_band].ast_per_rate[0].ul_value;
        hal_vap_set_beacon_rate(((dmac_vap_stru *)pst_mac_vap)->pst_hal_vap, ul_beacon_rate);
    }
    else
    {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_config_set_channel::en_band=%d", pst_channel_param->en_band);
    }

    /* disable TSF */
    hal_vap_set_machw_tsf_disable(((dmac_vap_stru *)pst_mac_vap)->pst_hal_vap);

#ifdef _PRE_WLAN_FEATURE_DFS
    /* ?????????????????? */
    if ((WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)&&(OAL_TRUE == mac_dfs_get_dfs_enable(pst_mac_device)))
    {
        oal_bool_enum_uint8      en_enable_dfs;

        en_enable_dfs = mac_is_ch_in_radar_band(pst_mac_device->en_max_band, pst_mac_vap->st_channel.uc_idx);
        hal_enable_radar_det(pst_mac_device->pst_device_stru, en_enable_dfs);
    }
#endif

    /* ??hal???????????? */
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
    /*dummy*/
#else
    if (pst_channel_param->en_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS)
    {
        OAM_ERROR_LOG0(0, OAM_SF_RX, "{dmac_config_set_channel:: fpga is not support 80M.}\r\n");
        pst_channel_param->en_bandwidth = WLAN_BAND_WIDTH_20M;
    }
#endif

    /* ???????? */
    dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
    dmac_alg_cfg_bandwidth_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);

    mac_device_find_up_sta(pst_mac_device, &pst_vap_up);

    dmac_mgmt_connect_set_channel(pst_mac_device, pst_vap_up, &(pst_mac_vap->st_channel));

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    
#ifdef _PRE_WLAN_FEATURE_DBAC
    if(mac_is_dbac_enabled(pst_mac_device))
    {

    }
    else
#endif
    {
        hal_vap_start_tsf(((dmac_vap_stru *)pst_mac_vap)->pst_hal_vap, OAL_FALSE);
    }
#endif

    return OAL_SUCC;
}

#ifdef _PRE_DEBUG_MODE

OAL_STATIC oal_void dmac_config_set_beacon_debug(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_param)
{
    mac_beacon_param_stru           *pst_beacon_param;
    oal_uint32                       ul_loop;
    oal_uint8                        uc_vap_id;

    pst_beacon_param = (mac_beacon_param_stru*)puc_param;
    uc_vap_id = pst_mac_vap->uc_vap_id;

    OAM_INFO_LOG1(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug::crypto_mode=%d.}", pst_beacon_param->uc_crypto_mode);
    OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug:gourp %d, group_size %d, auth %d.} bit_wpa %d, bit_wpa2 %d \r\n",
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGroupCipher,
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.ul_dot11RSNAConfigGroupCipherSize,
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAAuthenticationSuiteSelected);

    OAM_INFO_LOG2(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug:bit_wpa %d, bit_wpa2 %d.}",
        pst_mac_vap->st_cap_flag.bit_wpa,
        pst_mac_vap->st_cap_flag.bit_wpa2);

    for(ul_loop = 0; ul_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; ul_loop++)
    {
        OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug:[%d] %d, %d}",
            (ul_loop + 1),
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[ul_loop].uc_dot11RSNAConfigPairwiseCipherImplemented,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[ul_loop].ul_dot11RSNAConfigPairwiseCipherSizeImplemented);
    }

    for(ul_loop = 0; ul_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; ul_loop++)
    {
        OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug:[%d] %d, %d}",
            ul_loop + 1,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[ul_loop].uc_dot11RSNAConfigPairwiseCipherImplemented,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[ul_loop].ul_dot11RSNAConfigPairwiseCipherSizeImplemented);
    }

    OAM_INFO_LOG1(uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon_debug:rsn_capability %02x}", pst_beacon_param->us_rsn_capability);
}


OAL_STATIC oal_void dmac_config_add_beacon_debug(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_param)
{
    mac_beacon_param_stru           *pst_beacon_param;
    oal_uint32                       ul_loop;
    oal_uint8                        uc_vap_id;

    pst_beacon_param = (mac_beacon_param_stru*)puc_param;
    uc_vap_id = pst_mac_vap->uc_vap_id;

    OAM_INFO_LOG1(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug::crypto_mode=%d.}", pst_beacon_param->uc_crypto_mode);
    OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug:gourp %d, group_size %d, auth %d.} bit_wpa %d, bit_wpa2 %d \r\n",
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGroupCipher,
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.ul_dot11RSNAConfigGroupCipherSize,
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAAuthenticationSuiteSelected);

    OAM_INFO_LOG2(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug:bit_wpa %d, bit_wpa2 %d.}",
        pst_mac_vap->st_cap_flag.bit_wpa,
        pst_mac_vap->st_cap_flag.bit_wpa2);

    for(ul_loop = 0; ul_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; ul_loop++)
    {
        OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug:[%d] %d, %d}",
            (ul_loop + 1),
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[ul_loop].uc_dot11RSNAConfigPairwiseCipherImplemented,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[ul_loop].ul_dot11RSNAConfigPairwiseCipherSizeImplemented);
    }

    for(ul_loop = 0; ul_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; ul_loop++)
    {
        OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug:[%d] %d, %d}",
            ul_loop + 1,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[ul_loop].uc_dot11RSNAConfigPairwiseCipherImplemented,
            pst_mac_vap->pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[ul_loop].ul_dot11RSNAConfigPairwiseCipherSizeImplemented);
    }

    OAM_INFO_LOG1(uc_vap_id, OAM_SF_CFG, "{dmac_config_add_beacon_debug:rsn_capability %02x}", pst_beacon_param->us_rsn_capability);
}
#endif


OAL_STATIC oal_uint32  dmac_config_set_beacon(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                 *pst_mac_device;
    mac_beacon_param_stru           *pst_beacon_param;
    dmac_vap_stru                   *pst_dmac_vap;

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_beacon_param = (mac_beacon_param_stru*)puc_param;
    pst_beacon_param = pst_beacon_param;/* ????pclint??????pst_beacon_param?????? */

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* 1102??????????start ap??change beacon????????????????????????change beacon????????????beacon????
       ??dtim????????????change beacon????interval??dtim period??????????????????????????????mib?? */
    if ((0 != pst_beacon_param->l_dtim_period) || (0 != pst_beacon_param->l_interval))
    {
        /* ????mib */
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11DTIMPeriod   = (oal_uint32)pst_beacon_param->l_dtim_period;
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11BeaconPeriod = (oal_uint32)pst_beacon_param->l_interval;
    }

    /* ????????????????????vap????ssid???????????? */
    mac_vap_set_hide_ssid(pst_mac_vap, pst_beacon_param->uc_hidden_ssid);

    if (MAC_ADD_BEACON == pst_beacon_param->en_operation_type)
    {
        mac_vap_add_beacon(pst_mac_vap, pst_beacon_param);
    }
    else
	{
        mac_vap_set_beacon(pst_mac_vap, pst_beacon_param);
	}

    /* ????short gi */
    pst_mac_vap->pst_mib_info->st_phy_ht.en_dot11ShortGIOptionInTwentyImplemented = pst_beacon_param->en_shortgi_20;
    mac_mib_set_ShortGIOptionInFortyImplemented(pst_mac_vap, pst_beacon_param->en_shortgi_40);
    pst_mac_vap->pst_mib_info->st_wlan_mib_phy_vht.en_dot11VHTShortGIOptionIn80Implemented = pst_beacon_param->en_shortgi_80;

    mac_vap_init_by_protocol(pst_mac_vap, pst_beacon_param->en_protocol);
    mac_vap_init_rates(pst_mac_vap);

#ifdef _PRE_WLAN_FEATURE_11AC2G
    if ((WLAN_VHT_MODE == pst_mac_vap->en_protocol)
        && (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band))
    {
        mac_vap_set_11ac2g(pst_mac_vap, OAL_TRUE);
    }
    else
    {
        mac_vap_set_11ac2g(pst_mac_vap, OAL_FALSE);
    }
#endif

#endif

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_beacon::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_DEBUG_MODE
    if (MAC_ADD_BEACON == pst_beacon_param->en_operation_type)
    {
        dmac_config_add_beacon_debug(pst_mac_vap, puc_param);
    }
    else
	{
        dmac_config_set_beacon_debug(pst_mac_vap, puc_param);
	}
#endif
    dmac_vap_init_tx_frame_params(pst_dmac_vap, OAL_TRUE);
    dmac_vap_init_tx_ucast_data_frame(pst_dmac_vap);

    hal_vap_set_machw_beacon_period(pst_dmac_vap->pst_hal_vap, (oal_uint16)pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.ul_dot11BeaconPeriod);

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_add_user(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint16                      us_user_idx;
    mac_cfg_add_user_param_stru    *pst_add_user;
    dmac_user_stru                 *pst_dmac_user;
    mac_device_stru                *pst_device;
    oal_uint32                      ul_tid_idx;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_user_ht_hdl_stru            st_ht_hdl;
#endif
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_user::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_add_user = (mac_cfg_add_user_param_stru *)puc_param;

    us_user_idx = pst_add_user->us_user_idx;

    /* ????dmac user */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(us_user_idx);
    if (OAL_PTR_NULL == pst_dmac_user)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_user::pst_dmac_user null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* TBD hmac_config_add_user??????????????????????????????duankaiyong&guyanjie */

    /* ????qos???????????????????????????????????????? */
    mac_user_set_qos(&pst_dmac_user->st_user_base_info, OAL_TRUE);

    /* ????HT?? */
    mac_user_get_ht_hdl(&pst_dmac_user->st_user_base_info, &st_ht_hdl);
    st_ht_hdl.en_ht_capable = pst_add_user->en_ht_cap;

    if (OAL_TRUE == pst_add_user->en_ht_cap)
    {
        pst_dmac_user->st_user_base_info.en_cur_protocol_mode                = WLAN_HT_MODE;
        pst_dmac_user->st_user_base_info.en_avail_protocol_mode              = WLAN_HT_MODE;
    }

    /* ????HT??????????:???????????????????? ?????????????????????????? 2012->page:786 */
    st_ht_hdl.uc_min_mpdu_start_spacing = 6;
    st_ht_hdl.uc_max_rx_ampdu_factor    = 3;
    mac_user_set_ht_hdl(&pst_dmac_user->st_user_base_info, &st_ht_hdl);

    mac_user_set_asoc_state(&pst_dmac_user->st_user_base_info, MAC_USER_STATE_ASSOC);
#endif

    /* ??????dmac_ht_handle_stru??????????uc_ampdu_max_size?? */
    for (ul_tid_idx = 0; ul_tid_idx < WLAN_TID_MAX_NUM; ul_tid_idx++)
    {
        pst_dmac_user->ast_tx_tid_queue[ul_tid_idx].st_ht_tx_hdl.us_ampdu_max_size = 65535;
    }

    /* lut idx???????????????????????????????? */
    hal_machw_seq_num_index_update_per_tid(pst_device->pst_device_stru,
                                           pst_dmac_user->uc_lut_index,
                                           OAL_TRUE);

    /* ??????????????????????????????STA VAP????UP???? */
    if ((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)&&
        (IS_LEGACY_VAP(pst_mac_vap)))
    {
        /*??vap????????????????*/
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_UP);
    }

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_del_user(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint16                      us_user_idx;
    mac_cfg_del_user_param_stru    *pst_del_user;
    dmac_user_stru                 *pst_dmac_user;
    mac_device_stru                *pst_mac_device;

    pst_del_user = (mac_cfg_del_user_param_stru *)puc_param;

    us_user_idx = pst_del_user->us_user_idx;

    /* ????dmac user */
    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(us_user_idx);
    if (OAL_PTR_NULL == pst_dmac_user)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_del_user::pst_dmac_user null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_dmac_user->st_user_base_info.uc_vap_id, OAM_SF_CFG, "{dmac_config_del_user::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????RA LUT IDX */
    mac_user_del_ra_lut_index(pst_mac_device->auc_ra_lut_index_table, pst_dmac_user->uc_lut_index);
    hal_ce_del_peer_macaddr(((dmac_vap_stru *)pst_mac_vap)->pst_hal_device, pst_dmac_user->uc_lut_index);
    /* ??????????????????????????????STA VAP????FAKE_UP???? */
    if ((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)&&
        (IS_LEGACY_VAP(pst_mac_vap)))

    {
        /*??vap????????????????*/
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_STA_FAKE_UP);
    }

    dmac_alg_del_assoc_user_notify((dmac_vap_stru *)pst_mac_vap, pst_dmac_user);

    return OAL_SUCC;
}

#if 0

OAL_STATIC oal_uint32 dmac_config_ota_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    wal_specific_event_type_param_stru      *pst_specific_event_param;
    oal_uint8                                uc_vap_id_loop;
    oal_uint32                               ul_ret;

    pst_specific_event_param = (wal_specific_event_type_param_stru *)puc_param;

    OAL_IO_PRINT("dmac_config_ota_switch: en_ota_type:%d  en_switch_type:%d \n", pst_specific_event_param->l_event_type, pst_specific_event_param->l_param);

    for (uc_vap_id_loop = 0; uc_vap_id_loop < WLAN_VAP_SUPPOTR_MAX_NUM_SPEC; uc_vap_id_loop++)
    {
        ul_ret = oam_ota_set_switch(uc_vap_id_loop,
                                    (oal_switch_enum_uint8)pst_specific_event_param->l_param,
                                    (oam_ota_type_enum_uint8)pst_specific_event_param->l_event_type);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG0(uc_vap_id_loop, OAM_SF_ANY, "{dmac_config_ota_switch::ota switch set failed!}\r\n");
            return ul_ret;
        }
    }
    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_RF_110X_CALI_DPD

#define DPD_CALI_LUT_LENGTH 128
oal_uint32  dmac_dpd_data_processed_recv(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru             *pst_event;
    oal_netbuf_stru            *pst_dpd_data_netbuf;
    dpd_cali_data_stru         *pst_dpd_cali_data_processed;
    dmac_tx_event_stru         *pst_dtx_event;
    mac_vap_stru               *pst_mac_vap;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
      OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "{dmac_dpd_data_processed_recv::pst_event_mem null.}");
      return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event           = (frw_event_stru *)pst_event_mem->puc_data;
    pst_dtx_event       = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_dpd_data_netbuf = pst_dtx_event->pst_netbuf;

    pst_dpd_cali_data_processed = (dpd_cali_data_stru *)OAL_NETBUF_DATA(pst_dpd_data_netbuf);

    pst_mac_vap = mac_res_get_mac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CALIBRATE, "{dmac_dpd_data_processed_recv::mac_res_get_mac_vap fail,vap_id:%u.}",
                         pst_event->st_event_hdr.uc_vap_id);
        oal_netbuf_free(pst_dpd_data_netbuf);
        return OAL_FAIL;
    }

    if (0 == pst_dpd_cali_data_processed->us_data_len)
    {
        OAM_WARNING_LOG0(0, 0, "dmac_dpd_data_processed_recv: DPD FAIL");

        oal_netbuf_free(pst_dpd_data_netbuf);
        pst_mac_vap->st_cap_flag.bit_dpd_enbale = OAL_FALSE;

        return OAL_FAIL;
    }

    // write dpd data back to register
    hal_dpd_cali_func(HAL_DPD_CALI_DATA_WRITE, 0, 0, pst_dpd_cali_data_processed->us_dpd_data);
    oal_netbuf_free(pst_dpd_data_netbuf);


    pst_mac_vap->st_cap_flag.bit_dpd_done = OAL_TRUE;

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_dpd_cali_data_send(mac_vap_stru *pst_mac_vap, oal_void *p_param)
{
    frw_event_mem_stru      *pst_event_mem;
    frw_event_stru          *pst_event;
    dmac_tx_event_stru      *pst_dpd_event;
    oal_netbuf_stru         *pst_netbuf_dpd_data;
    dpd_cali_data_stru      *pst_dpd_cali_data;


    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_tx_event_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CALIBRATE,
                       "{dmac_send_dpd_calibrated_data::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_netbuf_dpd_data = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, OAL_SIZEOF(dpd_cali_data_stru), OAL_NETBUF_PRIORITY_HIGH);
    if (OAL_PTR_NULL == pst_netbuf_dpd_data)
    {
        FRW_EVENT_FREE(pst_event_mem);
        OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "{dmac_dpd_cali_data_send::pst_netbuf_dpd_data null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CRX,
                       DMAC_TO_HMAC_DPD_CALIBRATED_DATA_SEND,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    OAL_MEMZERO(oal_netbuf_cb(pst_netbuf_dpd_data), OAL_TX_CB_LEN);

    pst_dpd_cali_data = (dpd_cali_data_stru *)(OAL_NETBUF_DATA(pst_netbuf_dpd_data));
    oal_memcopy(pst_dpd_cali_data, p_param, OAL_SIZEOF(dpd_cali_data_stru));

    pst_dpd_event              = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_dpd_event->pst_netbuf  = pst_netbuf_dpd_data;
    pst_dpd_event->us_frame_len = OAL_SIZEOF(dpd_cali_data_stru);

    frw_event_dispatch_event(pst_event_mem);

    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}

oal_void dmac_start_dpd_calibration(mac_vap_stru *pst_mac_vap)
{
   oal_uint32 ul_status = OAL_FAIL;
   //oal_uint8 index = 0;
   oal_uint32 ul_wifibt_mode_sel;
   dpd_cali_data_stru dpd_cali_data_read;
   //dpd_cali_data_stru dpd_cali_data_calc;

   if(OAL_PTR_NULL == pst_mac_vap)
   {
     OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "dmac_start_dpd_calibration fail PTR_NULL\n\r");
     return;
   }

   OAM_WARNING_LOG0(0, OAM_SF_CALIBRATE, "DPD Calibration Start\n\r");

   hal_btcoex_get_rf_control(30000, &ul_wifibt_mode_sel, 0xff);

   if (ul_wifibt_mode_sel)
   {
        OAM_WARNING_LOG0(0, 0, "DPD RETURN GET RF CONTROL FAIL");
        return;
   }

   hal_dpd_cali_func(HAL_DPD_CALI_START, 0, 0, 0);

   hal_set_btcoex_occupied_period(0);
   hal_dpd_cali_func(HAL_DPD_CALI_STATUS, &ul_status, 0, 0);

   if (OAL_SUCC == ul_status)
   {
        hal_dpd_cali_func(HAL_DPD_CALI_DATA_READ, 0, dpd_cali_data_read.us_dpd_data, 0);
        dpd_cali_data_read.us_data_len = HI1102_DMAC_DPD_CALI_LUT_LENGTH;

#if 0
        for(index = 0; index < HI1102_DMAC_DPD_CALI_LUT_LENGTH; index++)
        {
            OAM_WARNING_LOG1(0, 0, "{dmac_start_dpd_calibration::[%x]!}\r\n", dpd_cali_data_read.us_dpd_data[index]);
            //dpd_cali_data_calc.us_dpd_data[index] = 0;
        }

        hi1102_rf_cali_dpd_corr_calc(dpd_cali_data_read.us_dpd_data, dpd_cali_data_calc.us_dpd_data);
        OAL_IO_PRINT("show dpd results.\n\r", dpd_cali_data_calc.us_dpd_data[index]);
        for(index = 0; index < HI1102_DMAC_DPD_CALI_LUT_LENGTH; index++)
        {
            OAL_IO_PRINT("out = 0x%6x\n\r", dpd_cali_data_calc.us_dpd_data[index]);
        }
        hal_dpd_cali_func(HAL_DPD_CALI_DATA_WRITE, 0, 0, dpd_cali_data_calc.us_dpd_data);
        hal_dpd_cali_func(HAL_DPD_CALI_DATA_READ, 0, dpd_cali_data_read.us_dpd_data, 0);
        dpd_cali_data_read.us_data_len = HI1102_DMAC_DPD_CALI_LUT_LENGTH;
        for(index = 0; index < HI1102_DMAC_DPD_CALI_LUT_LENGTH; index++)
        {
            OAL_IO_PRINT("read lut = 0x%6x\n\r", dpd_cali_data_read.us_dpd_data[index]);
            dpd_cali_data_calc.us_dpd_data[index] = 0;
        }
#else
     dmac_dpd_cali_data_send(pst_mac_vap, (oal_void*)(&dpd_cali_data_read));
#endif
   }
}

//
//
//

#endif/*_PRE_WLAN_RF_110X_CALI_DPD end*/


oal_uint32  dmac_cali_hmac2dmac_recv(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
    frw_event_stru             *pst_event;
    oal_netbuf_stru            *pst_cali_data_netbuf;
    oal_uint8                  *puc_cali_data;
    dmac_tx_event_stru         *pst_dtx_event;
   //oal_uint8                  *puc_content;
   //oal_uint32                  ul_byte;

   OAM_WARNING_LOG0(0, OAM_SF_CALIBRATE, "{dmac_cali_hmac2dmac_recv function called.}");
   //OAL_IO_PRINT("dmac_cali_hmac2dmac_recv: start\r\n");

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "{dmac_cali_hmac2dmac_recv::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event           = (frw_event_stru *)pst_event_mem->puc_data;
    pst_dtx_event       = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_cali_data_netbuf = pst_dtx_event->pst_netbuf;

    puc_cali_data = (oal_uint8 *)OAL_NETBUF_DATA(pst_cali_data_netbuf);
#if 0
    puc_content = (oal_uint8 *)puc_cali_data;

    for (ul_byte = 0; ul_byte < OAL_SIZEOF(oal_cali_param_stru); ul_byte+=4)
    {
        OAL_IO_PRINT("%02X %02X %02X %02X\r\n", puc_content[ul_byte], puc_content[ul_byte+1],
                      puc_content[ul_byte+2], puc_content[ul_byte+3]);
    }

    for(idx = 0; idx < DPD_CALI_LUT_LENGTH; idx++)
    {
      OAM_ERROR_LOG1(0, OAM_SF_CALIBRATE, "{dmac_dpd_data_processed_recv  received: 0x%X}", pst_dpd_cali_data_processed->us_dpd_data[idx]);
    }
#endif
    // write dpd data back to register


    hal_cali_send_func(puc_cali_data, pst_dtx_event->us_frame_len, pst_dtx_event->us_remain);

    oal_netbuf_free(pst_cali_data_netbuf);
#endif
    return OAL_SUCC;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
typedef enum
{
    CHECK_LTE_GPIO_INIT            = 0,    /* ?????? */
    CHECK_LTE_GPIO_LOW             = 1,    /* ???????????? */
    CHECK_LTE_GPIO_HIGH            = 2,    /*???????????? */
    CHECK_LTE_GPIO_RESUME          = 3,    /*?????????????? */
    CHECK_LTE_GPIO_DEV_LEVEL       = 4,    /*????device GPIO??????????*/
    CHECK_LTE_GPIO_BUTT
}damc_check_lte_gpio_step;

OAL_STATIC oal_uint32 dmac_config_lte_gpio_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32                           ul_mode_value;
    oal_uint8                            uc_gpio_level = 0;
    oal_uint8                            uc_lte_check_result = 0;
    dmac_atcmdsrv_atcmd_response_event   st_atcmdsrv_lte_gpio_check;

    ul_mode_value = *(oal_uint32 *)puc_param;
    /*??????GPIO????*/
    if(CHECK_LTE_GPIO_INIT == ul_mode_value)
    {
        hal_set_lte_gpio_mode(ul_mode_value);
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
        ConfigGpioPin(GPIO_BASE_ADDR,GPIO_2,GPIO_INOUT_IN, GPIO_TYPE_IO);
        ConfigGpioPin(GPIO_BASE_ADDR,GPIO_5,GPIO_INOUT_IN, GPIO_TYPE_IO);
        ConfigGpioPin(GPIO_BASE_ADDR,GPIO_6,GPIO_INOUT_IN, GPIO_TYPE_IO);
#endif
    }
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
    else if(CHECK_LTE_GPIO_LOW == ul_mode_value)/*??GPIO??????????0*/
    {
        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_2,GPIO_LEVEL_LOW);

        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_5,GPIO_LEVEL_LOW);

        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_6,GPIO_LEVEL_LOW);
    }
    else if(CHECK_LTE_GPIO_HIGH == ul_mode_value) /*??GPIO??????????1*/
    {
        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_2,GPIO_LEVEL_HIGH);

        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_5,GPIO_LEVEL_HIGH);

        oal_set_gpio_level(GPIO_BASE_ADDR,GPIO_6,GPIO_LEVEL_HIGH);
    }
    else if (CHECK_LTE_GPIO_DEV_LEVEL == ul_mode_value)
    {
        oal_get_gpio_level(GPIO_BASE_ADDR, GPIO_2, &uc_gpio_level);
        uc_lte_check_result |= ((uc_gpio_level << 2) & 0xFF);
        oal_get_gpio_level(GPIO_BASE_ADDR, GPIO_5, &uc_gpio_level);
        uc_lte_check_result |= ((uc_gpio_level << 5) & 0xFF);
        oal_get_gpio_level(GPIO_BASE_ADDR, GPIO_6, &uc_gpio_level);
        uc_lte_check_result |= ((uc_gpio_level << 6) & 0xFF);
    }
#endif
    else if(CHECK_LTE_GPIO_RESUME == ul_mode_value) /*??????????????*/
    {
        hal_set_lte_gpio_mode(ul_mode_value);
    }
    else
    {
        return OAL_FAIL;
    }

    /*????host??????????*/
    st_atcmdsrv_lte_gpio_check.uc_event_id = OAL_ATCMDSRV_LTE_GPIO_CHECK;
    st_atcmdsrv_lte_gpio_check.uc_reserved = uc_lte_check_result;

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHECK_LTE_GPIO, OAL_SIZEOF(dmac_atcmdsrv_atcmd_response_event), (oal_uint8 *)&st_atcmdsrv_lte_gpio_check);

    return OAL_SUCC;
}

#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_uint32  dmac_config_wfa_cfg_aifsn(mac_vap_stru *pst_mac_vap, oal_uint8 us_len, oal_uint8 *puc_param)
{
    mac_edca_cfg_stru   *pst_edca_cfg_param;
    dmac_vap_stru               *pst_dmac_vap;

    /* ?????????????? */
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{dmac_config_wfa_cfg_aifsn::input params is invalid, %p, %p.}",
                         pst_mac_vap, puc_param);
        return OAL_FAIL;
    }

    pst_edca_cfg_param = (mac_edca_cfg_stru *)puc_param;
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{dmac_config_wfa_cfg_aifsn::mac_res_get_dmac_vap fail or pst_dmac_vap->pst_hal_vap NULL,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    hal_vap_set_machw_aifsn_ac_wfa(pst_dmac_vap->pst_hal_vap, pst_edca_cfg_param->en_ac, (oal_uint8)pst_edca_cfg_param->us_val, pst_edca_cfg_param->en_switch);
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_wfa_cfg_cw(mac_vap_stru *pst_mac_vap, oal_uint8 us_len, oal_uint8 *puc_param)
{
    mac_edca_cfg_stru   *pst_edca_cfg_param;
    dmac_vap_stru               *pst_dmac_vap;

    /* ?????????????? */
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{dmac_config_wfa_cfg_cw::input params is invalid, %p, %p.}",
                         pst_mac_vap, puc_param);
        return OAL_FAIL;
    }

    pst_edca_cfg_param = (mac_edca_cfg_stru *)puc_param;
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{dmac_config_wfa_cfg_aifsn::mac_res_get_dmac_vap fail or pst_dmac_vap->pst_hal_vap NULL,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    hal_vap_set_edca_machw_cw_wfa(pst_dmac_vap->pst_hal_vap, (oal_uint8)pst_edca_cfg_param->us_val, pst_edca_cfg_param->en_ac, pst_edca_cfg_param->en_switch);
    return OAL_SUCC;
}

#endif /* DMAC_OFFLOAD */


OAL_STATIC oal_uint32  dmac_config_dump_all_rx_dscr(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru    *pst_device;

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_dump_all_rx_dscr::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    hal_dump_all_rx_dscr(pst_device->pst_device_stru);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_connect(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_uint32                                   ul_ret;
    mac_cfg80211_connect_security_stru          *pst_conn;
    dmac_vap_stru                               *pst_dmac_vap;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_connect::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_conn = (mac_cfg80211_connect_security_stru *)puc_param;

    ul_ret = mac_vap_init_privacy(pst_mac_vap, pst_conn);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_connect::mac_11i_init_privacy fail [%d].}", ul_ret);
        return ul_ret;
    }
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL != pst_dmac_vap)
    {
        pst_dmac_vap->st_query_stats.ul_signal = pst_conn->c_rssi;
    }
#ifdef _PRE_WLAN_FEATURE_11R
    if(OAL_TRUE == pst_dmac_vap->bit_11r_enable)
    {
        ul_ret = mac_mib_init_ft_cfg(pst_mac_vap, pst_conn->auc_mde);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_connect::mac_mib_init_ft_cfg fail[%d]!}\r\n", ul_ret);
            return ul_ret;
        }
    }

#endif //_PRE_WLAN_FEATURE_11R

    /* ??????????RSSI */

    /* ???????? */
    dmac_alg_cfg_start_connect_notify(pst_mac_vap, pst_conn->c_rssi);
#endif

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_BTCOEX

OAL_STATIC oal_uint32 dmac_config_print_btcoex_status(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    hal_get_btcoex_statistic(OAL_FALSE);
    return OAL_SUCC;
}

#endif

#ifdef _PRE_WLAN_FEATURE_LTECOEX

OAL_STATIC oal_uint32 dmac_config_ltecoex_mode_set(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                *pst_macdev;

    OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_config_ltecoex_mode_set! ltecoex_mode:%d}" , *puc_param);

    pst_macdev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_macdev))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_ltecoex_mode_set::dev is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if(*puc_param)
    {
        /*????LTE??????????????*/
        pst_macdev->pst_device_stru->ul_lte_coex_status = 1;

        /*???? mac????????????COEX_ABORT_CTRL ?? bit0????1*/
        hal_set_btcoex_sw_all_abort_ctrl(1);

        /*????PTA??lte??????????*/
        hal_ltecoex_req_mask_ctrl(0);
    }
    else
    {
        /*????LTE??????????????*/
        pst_macdev->pst_device_stru->ul_lte_coex_status = 0;

        /*????BT????????,??????mac??????????*/
        if(0 == pst_macdev->pst_device_stru->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_on)
        {
            hal_set_btcoex_sw_all_abort_ctrl(0);
            hal_set_btcoex_hw_rx_priority_dis(1);
        }

        /*????PTA??lte??????????*/
        hal_ltecoex_req_mask_ctrl(0xF);
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32  dmac_config_user_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    /* mips????????,??????? */
#ifndef _PRE_WLAN_PROFLING_MIPS
    dmac_user_stru                  *pst_dmac_user;
    mac_cfg_user_info_param_stru    *pst_dmac_event;
    oam_output_type_enum_uint8      en_output_type  = OAM_OUTPUT_TYPE_BUTT;
    oal_uint8                       uc_tid_index;
    //oal_uint32                      ul_tid_mpdu_num    = 0;

    /* ????dmac user */
    pst_dmac_event = (mac_cfg_user_info_param_stru *)puc_param;
    pst_dmac_user  = (dmac_user_stru *)mac_res_get_dmac_user(pst_dmac_event->us_user_idx);

    if (OAL_PTR_NULL == pst_dmac_user)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_user_info::pst_dmac_user null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_KEEPALIVE, "{dmac_config_user_info::us_assoc_id is %d, last_active_timestamp[%u]}",
                     pst_dmac_user->st_user_base_info.us_assoc_id, pst_dmac_user->ul_last_active_timestamp);
#if 0
    for (uc_tid_index = 0; uc_tid_index < WLAN_TID_MAX_NUM; uc_tid_index ++)
    {
        ul_tid_mpdu_num += pst_dmac_user->ast_tx_tid_queue[uc_tid_index].us_mpdu_num;
        if(pst_dmac_user->ast_tx_tid_queue[uc_tid_index].us_mpdu_num)
        {
            OAL_IO_PRINT("TID[%d],mpdu_num[%d],retry[%d],paused[%d]\r\n",
                   uc_tid_index,
                   pst_dmac_user->ast_tx_tid_queue[uc_tid_index].us_mpdu_num,
                   pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_retry_num,
                   pst_dmac_user->ast_tx_tid_queue[uc_tid_index].c_is_paused);
        }
    }
    OAL_IO_PRINT("dmac_config_user_info:totoal tid_mpdu_num %d\r\n", ul_tid_mpdu_num);
#endif
    oam_get_output_type(&en_output_type);
    if (OAM_OUTPUT_TYPE_SDT != en_output_type)
    {
#if 0
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_lut_index :        %d \n", pst_dmac_user->uc_lut_index);
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "bit_ps :              %d \n", pst_dmac_user->bit_ps_mode);
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "en_vip_flag :         %d \n", pst_dmac_user->en_vip_flag);
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "en_smartant_training :%d \n", pst_dmac_user->en_smartant_training);
        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "\n");

        for (uc_tid_index = 0; uc_tid_index < WLAN_TID_MAX_NUM; uc_tid_index ++)
        {
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "tid               %d \n", uc_tid_index);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_is_paused : %d \n", (oal_uint8)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].c_is_paused);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_num_dq : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_dq);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_retry_num : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_retry_num);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "us_mpdu_num : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].us_mpdu_num);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "ul_mpdu_avg_len : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].ul_mpdu_avg_len);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_num_tx_ba : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_tx_ba);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_num_rx_ba : %d \n", pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_rx_ba);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "pst_ba_tx_hdl : %u \n", (oal_uint32)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].pst_ba_tx_hdl);
            OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "pst_ba_rx_hdl : %u \n\n", (oal_uint32)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].pst_ba_rx_hdl);
        }

        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "power save related user_info:\n");
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "mpdu num in ps_queue-->%d\n", oal_atomic_read(&pst_dmac_user->st_ps_structure.uc_mpdu_num));
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "user ps mode is -->%d\n", pst_dmac_user->bit_ps_mode);
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "pspoll process state is -->%d\n", pst_dmac_user->st_ps_structure.en_is_pspoll_rsp_processing);
#endif

        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_lut_index[%d]; bit_ps[%d];en_vip_flag[%d];en_smartant_training[%d]\n",
                      pst_dmac_user->uc_lut_index, pst_dmac_user->bit_ps_mode, pst_dmac_user->en_vip_flag, pst_dmac_user->en_smartant_training);

        for (uc_tid_index = 0; uc_tid_index < WLAN_TID_MAX_NUM; uc_tid_index++)
        {
            OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "tid[%d]; uc_is_paused[%d]\n",
                          uc_tid_index, (oal_uint8)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_is_paused);

            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_num_dq[%d]; uc_retry_num[%d]; us_mpdu_num[%d]; ul_mpdu_avg_len[%d]\n",
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_dq,
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_retry_num,
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].us_mpdu_num,
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].ul_mpdu_avg_len);

            OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_num_tx_ba[%d]; uc_num_rx_ba[%d]; pst_ba_tx_hdl[%u]; pst_ba_rx_hdl[%u]\n",
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_tx_ba,
                          pst_dmac_user->ast_tx_tid_queue[uc_tid_index].uc_num_rx_ba,
                          (oal_uint32)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].pst_ba_tx_hdl,
                          (oal_uint32)pst_dmac_user->ast_tx_tid_queue[uc_tid_index].pst_ba_rx_hdl);
        }

        OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "power save related user_info:mpdu num in ps_queue[%d]; user ps mode[%d]; pspoll process state[%d]\n",
                      oal_atomic_read(&pst_dmac_user->st_ps_structure.uc_mpdu_num),
                      pst_dmac_user->bit_ps_mode,
                      pst_dmac_user->st_ps_structure.en_is_pspoll_rsp_processing);
    }
    else
    {
        oam_ota_report((oal_uint8 *)&pst_dmac_user->uc_lut_index,
                       (oal_uint16)(OAL_SIZEOF(dmac_user_stru) - OAL_SIZEOF(mac_user_stru)),
                       0, 0, OAM_OTA_TYPE_DMAC_USER);
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_dscr(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_set_dscr_param_stru     *pst_event_set_dscr;
    dmac_vap_stru                   *pst_dmac_vap;

    pst_event_set_dscr = (mac_cfg_set_dscr_param_stru *)puc_param;

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
    /* ????????????set_ucast_data????, ??????????????????????????valid??????0 */
    if (pst_event_set_dscr->en_type == MAC_VAP_CONFIG_UCAST_DATA)
    {
        pst_dmac_vap->un_mode_valid.uc_mode_param_valid = 0;
    }
    /* ????, ??????????????set_mode_ucast_data????, ??????????????????valid??????1 */
    else if (pst_event_set_dscr->en_type >= MAC_VAP_CONFIG_MODE_UCAST_DATA)
    {
        switch (pst_event_set_dscr->en_type)
        {
            case MAC_VAP_CONFIG_VHT_UCAST_DATA:
                pst_dmac_vap->un_mode_valid.st_spec_mode.bit_vht_param_vaild = 1;
                break;
            case MAC_VAP_CONFIG_HT_UCAST_DATA:
                pst_dmac_vap->un_mode_valid.st_spec_mode.bit_ht_param_vaild = 1;
                break;
            case MAC_VAP_CONFIG_11AG_UCAST_DATA:
                pst_dmac_vap->un_mode_valid.st_spec_mode.bit_11ag_param_vaild = 1;
                break;
            case MAC_VAP_CONFIG_11B_UCAST_DATA:
                pst_dmac_vap->un_mode_valid.st_spec_mode.bit_11b_param_vaild = 1;
                break;
            default:
                OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_dscr::pst_event_set_dscr->en_type = %u, invalid}", pst_event_set_dscr->en_type);
                return OAL_FAIL;
        }
    }
#endif

    g_dmac_config_set_dscr_param[pst_event_set_dscr->uc_function_index](pst_event_set_dscr->l_value,
                                                                        pst_event_set_dscr->en_type,
                                                                        pst_dmac_vap);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_set_log_level(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    // ????device log level??????????????????
    return oam_log_set_vap_level(pst_mac_vap->uc_vap_id, *puc_param);
}

#ifdef _PRE_WLAN_FEATURE_GREEN_AP

OAL_STATIC oal_uint32 dmac_config_set_green_ap_en(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint8      uc_param = *(oal_uint8 *)puc_param;

    OAL_IO_PRINT("dmac_config_set_green_ap_en %d\r\n", uc_param);

    if (0 == uc_param)
    {
        dmac_green_ap_stop(pst_mac_vap->uc_device_id);
    }
    else if (1 == uc_param)
    {
        dmac_green_ap_start(pst_mac_vap->uc_device_id);
    }
    else if (2 == uc_param)
    {
        dmac_green_ap_dump_info(pst_mac_vap->uc_device_id);
    }

    return OAL_SUCC;
}
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
OAL_STATIC oal_uint32 dmac_config_set_log_lowpower(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    // ????device log level??????????????????
    oal_uint8      uc_param = *(oal_uint8 *)puc_param;
    return oam_log_set_pm_enable((oal_uint8)uc_param) ;
}

OAL_STATIC oal_uint32 dmac_config_set_pm_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    // ????device log level??????????????????
    oal_uint8      uc_param = *(oal_uint8 *)puc_param;

    g_pm_switch = uc_param;

    OAM_WARNING_LOG1(0, OAM_SF_PWR, "dmac_config_set_pm_switch %d", g_pm_switch);

    return OAL_SUCC ;
}

/*****************************************************************************
 . . .  : dmac_config_get_ant
 ....  : ......
 ....  : pst_mac_vap: ..VAP...
             uc_len     : ....
             puc_param  : ..
 ....  : .
 . . .  : ...
 ....  :
 ....  :

 ....      :
  1..    .   : 2014.3.28.
    .    .   : zhangyu
    ....   : .....
*****************************************************************************/
OAL_STATIC oal_uint32  dmac_config_get_ant(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)

{
    oal_uint8                    uc_result;

    dmac_atcmdsrv_atcmd_response_event    st_atcmdsrv_dbb_num_info;

    oal_get_gpio_level(BCPU_GPIO_BASE, WLAN_ANT_GPIO, &uc_result);
    OAM_WARNING_LOG1(0, OAM_SF_PWR, "dmac_config_get_ant  ant:%d", uc_result);

    st_atcmdsrv_dbb_num_info.uc_event_id = OAL_ATCMDSRV_GET_ANT;
    st_atcmdsrv_dbb_num_info.ul_event_para = uc_result;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_GET_ANT, OAL_SIZEOF(dmac_atcmdsrv_atcmd_response_event), (oal_uint8 *)&st_atcmdsrv_dbb_num_info);


    return OAL_SUCC;
}


#endif


OAL_STATIC oal_uint32  dmac_config_set_country(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_regdomain_info_stru *pst_mac_regdom;
    mac_regdomain_info_stru *pst_regdomain_info;
    oal_uint8                uc_rc_num;
    oal_uint32               ul_size;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    mac_device_stru                     *pst_mac_device = OAL_PTR_NULL;
    hal_to_dmac_device_stru             *pst_hal_device = OAL_PTR_NULL;
#endif

    pst_mac_regdom = (mac_regdomain_info_stru *)puc_param;

    /* ???????????????? */
    uc_rc_num = pst_mac_regdom->uc_regclass_num;

    /* ???????????? */
    ul_size = OAL_SIZEOF(mac_regclass_info_stru) * uc_rc_num + MAC_RD_INFO_LEN;
    /*??????????????????*/
    mac_get_regdomain_info(&pst_regdomain_info);

    /* ?????????????? */
    oal_memcopy(pst_regdomain_info, pst_mac_regdom, ul_size);

    /* ???????????????????? */
    mac_init_channel_list();

#ifdef _PRE_SUPPORT_ACS
    dmac_acs_report_support_chan(pst_mac_vap);
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    /* ??????FCC????????????????????????dbb scaling???????????????? */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_DFS, "dmac_config_set_country::pst_mac_device null.device:%u",pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hal_device = pst_mac_device->pst_device_stru;
    /* ????????FCC???????????????????????? */
    pst_hal_device->en_current_reg_domain = (hal_regdomain_enum)pst_mac_regdom->uc_regdomain_type;
    OAM_WARNING_LOG1(0, OAM_SF_DFS, "{dmac_config_set_country::set country, reg_domain: %d}",
                pst_hal_device->en_current_reg_domain);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_random_mac_oui(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru  *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_DFS, "dmac_config_set_random_mac_oui::pst_mac_device null.device:%u",pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if(uc_len < WLAN_RANDOM_MAC_OUI_LEN)
    {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{dmac_config_set_random_mac_oui::len is short:%d.}", uc_len);
        return OAL_FAIL;
    }

    oal_memcopy(pst_mac_device->auc_mac_oui, puc_param, WLAN_RANDOM_MAC_OUI_LEN);

    OAM_WARNING_LOG3(0, OAM_SF_ANY, "{dmac_config_set_random_mac_oui::mac_ou:0x%.2x:%.2x:%.2x}\r\n",
                     pst_mac_device->auc_mac_oui[0],
                     pst_mac_device->auc_mac_oui[1],
                     pst_mac_device->auc_mac_oui[2]);

    return OAL_SUCC;
}


#ifdef _PRE_WLAN_FEATURE_DFS

OAL_STATIC oal_uint32  dmac_config_set_country_for_dfs(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_dfs_domain_enum_uint8       en_dfs_domain;
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_mac_device;

    en_dfs_domain = *puc_param;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_DFS, "dmac_config_set_country_for_dfs::mac_res_get_dev fail.device:%u",pst_mac_vap->uc_device_id);
        return OAL_FAIL;
    }

    pst_hal_device = pst_mac_device->pst_device_stru;

    if(OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_DFS, "dmac_config_set_country_for_dfs::pointer null.");
        return OAL_FAIL;
    }

    hal_radar_config_reg(pst_hal_device, en_dfs_domain);

    pst_hal_device->st_dfs_radar_filter.ul_last_burst_timestamp           = 0;
    pst_hal_device->st_dfs_radar_filter.ul_last_burst_timestamp_for_chirp = 0;
    pst_hal_device->st_dfs_radar_filter.en_chirp_enable                   = 1;
    pst_hal_device->st_dfs_radar_filter.ul_time_threshold                 = 100;
    switch(en_dfs_domain)
    {
        case MAC_DFS_DOMAIN_ETSI:
            pst_hal_device->st_dfs_radar_filter.ul_chirp_cnt_threshold  = 3;
            pst_hal_device->st_dfs_radar_filter.ul_chirp_time_threshold = 100;
            break;
        case MAC_DFS_DOMAIN_FCC:
            pst_hal_device->st_dfs_radar_filter.ul_chirp_cnt_threshold  = 4;
            pst_hal_device->st_dfs_radar_filter.ul_chirp_time_threshold = 12000;
            break;
        default :
            break;
    }

    OAM_WARNING_LOG0(0, OAM_SF_DFS, "{dmac_config_set_country_for_dfs::set radar filter params.}");

    return OAL_SUCC;
}
#endif
#if 0


OAL_STATIC oal_uint32  dmac_config_alrxtx_set_pm(mac_vap_stru *pst_mac_vap, oal_uint32 ul_switch)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_bool_enum_uint8              en_pm_switch;

    en_pm_switch = ((OAL_SWITCH_OFF == ul_switch) ? OAL_TRUE : OAL_FALSE);
    dmac_config_set_pm_switch(pst_mac_vap, 0, &en_pm_switch);
#endif

    return OAL_SUCC;
}




OAL_STATIC oal_uint32  dmac_config_list_channel(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_regdomain_info_stru *pst_regdomain_info                     = OAL_PTR_NULL;
    oal_uint8                uc_chan_num;
    oal_uint8                uc_chan_idx;
    oal_uint32               ul_ret                                 = OAL_FAIL;
    oal_int8                 ac_string[OAM_PRINT_FORMAT_LENGTH];
    oal_int8                *pc_str_offset;
    mac_regclass_info_stru  *pst_rc_info;

    mac_get_regdomain_info(&pst_regdomain_info);

    OAL_SPRINTF(ac_string, sizeof(ac_string), "Country is: %s %s\n",
                pst_regdomain_info->ac_country,
                "2G(chan_num reg_tx_pwr max_tx_pwr):\n");
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_2_BUTT; uc_chan_idx++)
    {
        pc_str_offset = ac_string + OAL_STRLEN(ac_string);
        ul_ret = mac_is_channel_idx_valid(MAC_RC_START_FREQ_2, uc_chan_idx);
        if (OAL_SUCC == ul_ret)
        {
            mac_get_channel_num_from_idx(MAC_RC_START_FREQ_2, uc_chan_idx, &uc_chan_num);
            pst_rc_info = mac_get_channel_idx_rc_info(WLAN_BAND_2G, uc_chan_idx);
            OAL_IO_PRINT("%d,%d,%d\r\n", uc_chan_num, pst_rc_info->uc_max_reg_tx_pwr, pst_rc_info->uc_max_tx_pwr);
            OAL_SPRINTF(pc_str_offset, 10, "%d %d %d\n", uc_chan_num, pst_rc_info->uc_max_reg_tx_pwr, pst_rc_info->uc_max_tx_pwr);
        }
    }

    oam_print(ac_string);

    OAL_SPRINTF(ac_string, sizeof(ac_string), "%s", "5G(chan_num reg_tx_pwr max_tx_pwr):\n");
    for (uc_chan_idx = 0; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT/2; uc_chan_idx++)
    {
        pc_str_offset = ac_string + OAL_STRLEN(ac_string);
        ul_ret = mac_is_channel_idx_valid(MAC_RC_START_FREQ_5, uc_chan_idx);
        if (OAL_SUCC == ul_ret)
        {
            mac_get_channel_num_from_idx(MAC_RC_START_FREQ_5, uc_chan_idx, &uc_chan_num);
            pst_rc_info = mac_get_channel_idx_rc_info(WLAN_BAND_5G, uc_chan_idx);
            OAL_SPRINTF(pc_str_offset, 12, "%d %d %d\n", uc_chan_num, pst_rc_info->uc_max_reg_tx_pwr, pst_rc_info->uc_max_tx_pwr);
        }
    }
    oam_print(ac_string);

    ac_string[0] = '\0';
    for (uc_chan_idx = MAC_CHANNEL_FREQ_5_BUTT/2; uc_chan_idx < MAC_CHANNEL_FREQ_5_BUTT; uc_chan_idx++)
    {
        pc_str_offset = ac_string + OAL_STRLEN(ac_string);
        ul_ret = mac_is_channel_idx_valid(MAC_RC_START_FREQ_5, uc_chan_idx);
        if (OAL_SUCC == ul_ret)
        {
            mac_get_channel_num_from_idx(MAC_RC_START_FREQ_5, uc_chan_idx, &uc_chan_num);
            pst_rc_info = mac_get_channel_idx_rc_info(WLAN_BAND_5G, uc_chan_idx);
            OAL_SPRINTF(pc_str_offset, 12, "%d %d %d\n", uc_chan_num, pst_rc_info->uc_max_reg_tx_pwr, pst_rc_info->uc_max_tx_pwr);
        }
    }
    oam_print(ac_string);
    return OAL_SUCC;
}
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_uint32   dmac_config_set_regdomain_pwr(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_regdomain_max_pwr_stru *pst_cfg;

    pst_cfg = (mac_cfg_regdomain_max_pwr_stru *)puc_param;

    mac_regdomain_set_max_power(pst_cfg->uc_pwr, pst_cfg->en_exceed_reg);

    return OAL_SUCC;

}
#endif


OAL_STATIC oal_uint32 dmac_config_reduce_sar(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32              ul_ret;

    /* ????supplicant?????????????????????????????????????????????????????? */
    g_uc_sar_pwr_limit = (*puc_param >= HAL_SAR_PWR_LIMIT_THRESHOLD) ? INVALID_SAR_PWR_LIMIT : *puc_param;

    /* ???????????????? */
    ul_ret = dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_reduce_sar:dmac_alg_cfg_channel_notify FAIL!}");
        return ul_ret;
    }

    return OAL_SUCC;
}


#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
OAL_STATIC oal_uint32  dmac_config_set_rate(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_non_ht_rate_stru        *pst_event_set_rate;
    dmac_vap_stru                   *pst_dmac_vap;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint32                       ul_ret;
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_rate::pst_dmac_vap or pst_dmac_vap->pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


    /* ???????? */
    pst_event_set_rate = (mac_cfg_non_ht_rate_stru *)puc_param;
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_legacy_rate = pst_event_set_rate->en_rate;
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode = pst_event_set_rate->en_protocol_mode;

    /* ??????????????????1Mbps */
    if (WLAN_LEGACY_11b_RESERVED1 == pst_event_set_rate->en_rate)
    {
        pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = 1;
    }

    /* ???????????? */
    pst_dmac_vap->uc_protocol_rate_dscr = (oal_uint8)((pst_event_set_rate->en_protocol_mode << 6) | pst_event_set_rate->en_rate);

#if (_PRE_MULTI_CORE_MODE==_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC)
    /* ????????????????????????????????DAC??LPF???? */
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_rate->en_protocol_mode, pst_event_set_rate->en_rate);
#else
#ifdef _PRE_WLAN_HW_TEST
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_rate->en_protocol_mode, pst_event_set_rate->en_rate);
#endif
#endif

    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_dmac_vap->pst_hal_device->uc_al_tx_flag)
    {
       hal_set_tx_dscr_field(pst_dmac_vap->pst_hal_device, pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value, HAL_RF_TEST_DATA_RATE_ZERO);

#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
        ul_ret = dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_rate:dmac_config_al_tx_set_pow fail!}");
            return ul_ret;
        }
#endif
    }

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_rate::en_rate=%d,protocol=%d.",
                    pst_event_set_rate->en_rate, pst_event_set_rate->en_protocol_mode);

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_mcs(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_tx_comp_stru            *pst_event_set_mcs;
    dmac_vap_stru                   *pst_dmac_vap;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint32                       ul_ret;
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcs::pst_dmac_vap or pst_dmac_vap->pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*????MF????*/
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_preamble_mode = 0;

    pst_event_set_mcs = (mac_cfg_tx_comp_stru *)puc_param;
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_ht_rate.bit_ht_mcs = pst_event_set_mcs->uc_param;

#if (_PRE_MULTI_CORE_MODE==_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC)
    /* ????????????????????????????????DAC??LPF???? */
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_mcs->en_protocol_mode, pst_event_set_mcs->uc_param);
#else
#ifdef _PRE_WLAN_HW_TEST
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_mcs->en_protocol_mode, pst_event_set_mcs->uc_param);
#endif
#endif

    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode = pst_event_set_mcs->en_protocol_mode;

    /* ?????????????????? */
    pst_dmac_vap->uc_protocol_rate_dscr = (oal_uint8)((pst_event_set_mcs->en_protocol_mode << 6) | pst_event_set_mcs->uc_param);

    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_dmac_vap->pst_hal_device->uc_al_tx_flag)
    {
        hal_set_tx_dscr_field(pst_dmac_vap->pst_hal_device, pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value, HAL_RF_TEST_DATA_RATE_ZERO);

#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
        ul_ret = dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcs:dmac_config_al_tx_set_pow fail!}");
            return ul_ret;
        }
#endif
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcs::tx dscr mcs=%d.", pst_event_set_mcs->uc_param);

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_mcsac(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_tx_comp_stru            *pst_event_set_mcsac;
    dmac_vap_stru                   *pst_dmac_vap;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint32                       ul_ret;
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcsac::pst_dmac_vap or pst_dmac_vap->pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


    /* ???????? */
    pst_event_set_mcsac = (mac_cfg_tx_comp_stru *)puc_param;
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_vht_nss_mcs.bit_vht_mcs = pst_event_set_mcsac->uc_param;
    pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate.st_legacy_rate.bit_protocol_mode = pst_event_set_mcsac->en_protocol_mode;

    /* ???????????? */
    pst_dmac_vap->uc_protocol_rate_dscr = (oal_uint8)((pst_event_set_mcsac->en_protocol_mode << 6) | pst_event_set_mcsac->uc_param);
#if (_PRE_MULTI_CORE_MODE==_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC)
    /* ????????????????????????????????DAC??LPF???? */
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_mcsac->en_protocol_mode, pst_event_set_mcsac->uc_param);
#else
#ifdef _PRE_WLAN_HW_TEST
    hal_set_dac_lpf_gain(pst_dmac_vap->pst_hal_device, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.en_bandwidth, pst_event_set_mcsac->en_protocol_mode, pst_event_set_mcsac->uc_param);
#endif
#endif

    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_dmac_vap->pst_hal_device->uc_al_tx_flag)
    {
        hal_set_tx_dscr_field(pst_dmac_vap->pst_hal_device, pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].ul_value, HAL_RF_TEST_DATA_RATE_ZERO);

#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
        ul_ret = dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcsac:dmac_config_al_tx_set_pow fail!}");
            return ul_ret;
        }
#endif
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_mcs::tx dscr mcsac=%d.", pst_event_set_mcsac->uc_param);

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_bw(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_tx_comp_stru            *pst_event_set_bw;
    dmac_vap_stru                   *pst_dmac_vap;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_bw::pst_dmac_vap or pst_dmac_vap->pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


     /* ???????? */
    pst_event_set_bw = (mac_cfg_tx_comp_stru *)puc_param;
    pst_dmac_vap->st_tx_data_mcast.st_rate.uc_channel_bandwidth = pst_event_set_bw->uc_param;

    pst_dmac_vap->uc_bw_flag = pst_event_set_bw->uc_param;

    /* ?????????????????? */
    if (OAL_SWITCH_ON == pst_dmac_vap->pst_hal_device->uc_al_tx_flag)
    {
        hal_set_tx_dscr_field(pst_dmac_vap->pst_hal_device, pst_event_set_bw->uc_param, HAL_RF_TEST_BAND_WIDTH);
    }

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX

OAL_STATIC oal_uint32 dmac_config_al_tx_set_addresses(
                mac_vap_stru                           *pst_vap,
                mac_ieee80211_frame_stru  *pst_hdr)
{
    /* From DS?????????? */
    mac_hdr_set_from_ds((oal_uint8 *)pst_hdr, 0);

    /* to DS?????????? */
    mac_hdr_set_to_ds((oal_uint8 *)pst_hdr, 1);

    /* Set Address1 field in the WLAN Header with BSSID */
    oal_set_mac_addr(pst_hdr->auc_address1, BROADCAST_MACADDR);


    /* Set Address2 field in the WLAN Header with the source address */
    oal_set_mac_addr(pst_hdr->auc_address2, pst_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

    /* Set Address3 field in the WLAN Header with the destination address */
    oal_set_mac_addr(pst_hdr->auc_address3, BROADCAST_MACADDR);

	return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_al_tx_set_frame_ctrl(
                                                      mac_tx_ctl_stru                         *pst_tx_ctl,
                                                      mac_ieee80211_frame_stru  *pst_hdr)
{
    /* ?????????????? */
    mac_hdr_set_frame_control((oal_uint8 *)pst_hdr, WLAN_FC0_TYPE_DATA | WLAN_FC0_SUBTYPE_DATA);

    MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl) = MAC_80211_FRAME_LEN;

	return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_al_tx_packet_send(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf)
{
    frw_event_mem_stru    *pst_event_mem;
    //oal_uint32             ul_ret = HMAC_TX_PASS;
    frw_event_stru        *pst_event;
    dmac_tx_event_stru    *pst_dtx_stru;

    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_tx_event_stru));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(pst_vap->uc_vap_id, OAM_SF_TX, "{hmac_tx_lan_to_wlan::FRW_EVENT_ALLOC failed.}");
#if(_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        pst_vap->st_vap_stats.ul_tx_dropped_packets++;
#endif
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ?????????? */
    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                        FRW_EVENT_TYPE_HOST_DRX,
                        DMAC_TX_HOST_DRX,
                        OAL_SIZEOF(dmac_tx_event_stru),
                        FRW_EVENT_PIPELINE_STAGE_1,
                        pst_vap->uc_chip_id,
                        pst_vap->uc_device_id,
                        pst_vap->uc_vap_id);

    pst_dtx_stru             = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_dtx_stru->pst_netbuf = pst_buf;

    dmac_tx_process_data_event(pst_event_mem);

     /* ???????? */
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;

}

OAL_STATIC oal_uint32 dmac_config_al_tx_packet(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf, oal_uint32 ul_len)
{
    mac_tx_ctl_stru                     *pst_tx_ctl;       /* SKB CB */
    mac_ieee80211_qos_htc_frame_stru    *pst_hdr;

    /* ??????CB tx rx???? , CB?????????????????????? ????????????????????????????????*/

    /* netbuff????????????  */
    pst_tx_ctl = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_buf);
    MAC_GET_CB_MPDU_NUM(pst_tx_ctl)                  = 1;
    MAC_GET_CB_NETBUF_NUM(pst_tx_ctl)                       = 1;
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl)             = WLAN_DATA_BASICTYPE;
    MAC_GET_CB_IS_PROBE_DATA(pst_tx_ctl)          = DMAC_USER_ALG_NON_PROBE;
    //MAC_GET_CB_EN_ACK_POLICY(pst_tx_ctl)             = WLAN_TX_NORMAL_ACK;
    mac_set_cb_ack_policy(pst_tx_ctl, WLAN_TX_NO_ACK);
    MAC_GET_CB_TX_VAP_INDEX(pst_tx_ctl)           = pst_vap->uc_vap_id;
    mac_set_cb_ac(pst_tx_ctl, WLAN_WME_AC_VO);
    MAC_GET_CB_EVENT_TYPE(pst_tx_ctl)             = FRW_EVENT_TYPE_WLAN_DTX;
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl)           = (oal_uint8)pst_vap->us_multi_user_idx;
    mac_set_cb_80211_mac_head_type(pst_tx_ctl, 0); /*????mac????????skb????????????????????????*/
    MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctl)    = MAC_80211_FRAME_LEN;
    MAC_GET_CB_IS_MCAST(pst_tx_ctl) = OAL_TRUE;

    pst_hdr = (mac_ieee80211_qos_htc_frame_stru *)OAL_NETBUF_HEADER(pst_buf);
    pst_hdr->bit_qc_amsdu = OAL_FALSE;

    /* ????802.11??,??data_tx??????802.11????????0 */
    dmac_config_al_tx_set_frame_ctrl(pst_tx_ctl,(mac_ieee80211_frame_stru *)pst_hdr);
    dmac_config_al_tx_set_addresses(pst_vap, (mac_ieee80211_frame_stru *)pst_hdr);
    mac_set_cb_frame_hdr(pst_tx_ctl, (mac_ieee80211_frame_stru *)pst_hdr);

    /* ????frame???? */
    MAC_GET_CB_MPDU_BYTES(pst_tx_ctl) = (oal_uint16)ul_len;
    //MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (oal_uint16)ul_len;
    return OAL_SUCC;
}

OAL_STATIC oal_netbuf_stru*  dmac_config_create_al_tx_packet(oal_uint32 ul_size,
                                                    mac_rf_payload_enum_uint8        en_payload_flag)
{
    oal_netbuf_stru         *pst_buf;
    oal_uint32               ul_loop = 0;
    oal_uint8                       *puc_data;

    //pst_buf = OAL_MEM_NETBUF_ALLOC((oal_uint16)ul_size, OAL_TRUE);/* ???????????????????????????? */

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if(WLAN_LARGE_NETBUF_SIZE >= ul_size)
    {
        pst_buf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, (oal_uint16)ul_size, OAL_NETBUF_PRIORITY_MID);
    }
    else
    {
        pst_buf = OAL_MEM_MULTI_NETBUF_ALLOC((oal_uint16)ul_size);
    }
#else
    pst_buf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, (oal_uint16)ul_size, OAL_NETBUF_PRIORITY_MID);
#endif

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_buf))
    {
        OAM_ERROR_LOG0(0, OAM_SF_TX, "dmac_config_create_al_tx_packet::alloc Fail");
        return OAL_PTR_NULL;
    }

    //oal_netbuf_put(pst_buf, ul_size);
    puc_data = OAL_NETBUF_DATA(pst_buf);
    switch (en_payload_flag)
    {
        case RF_PAYLOAD_ALL_ZERO:
            OAL_MEMZERO(puc_data, ul_size);
            break;
        case RF_PAYLOAD_ALL_ONE:
            oal_memset(puc_data, 0xFF, ul_size);
            break;
        case RF_PAYLOAD_RAND:
            puc_data[0] = oal_gen_random(18, 1);
            for (ul_loop = 1; ul_loop < ul_size; ul_loop++)
            {
                puc_data[ul_loop] = oal_gen_random(18, 0);
            }
            break;
        default:
        	break;
    }

    pst_buf->next = OAL_PTR_NULL;
    //pst_buf->prev = OAL_PTR_NULL;

    OAL_MEMZERO(oal_netbuf_cb(pst_buf), OAL_TX_CB_LEN);

    return pst_buf;
}


OAL_STATIC oal_uint32  dmac_config_al_tx_bcast_pkt(mac_vap_stru *pst_mac_vap, oal_uint32 ul_len)
{
    oal_netbuf_stru                *pst_netbuf;
    oal_uint32                      ul_ret;

    /* ???? */
    pst_netbuf = dmac_config_create_al_tx_packet(ul_len, (oal_uint8)pst_mac_vap->bit_payload_flag);

    if (OAL_PTR_NULL == pst_netbuf)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_bcast_pkt::return null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????cb??mac?? */
    ul_ret = dmac_config_al_tx_packet(pst_mac_vap, pst_netbuf, ul_len);

    /* ???????? */
    ul_ret |= dmac_config_al_tx_packet_send(pst_mac_vap, pst_netbuf);

    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_bcast_pkt::hmac_tx_lan_to_wlan return error %d!}\r\n", ul_ret);
    }

    return ul_ret;
}



OAL_STATIC oal_uint32  dmac_config_set_always_tx_1102(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_tx_comp_stru            *pst_event_set_al_tx;
    dmac_vap_stru                   *pst_dmac_vap;
    oal_uint32                       ul_ret;
#ifndef WIN32
    mac_device_stru                 *pst_mac_device;
#endif  /* WIN32 */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

    oal_uint8                        uc_pm_off;
#endif
    pst_event_set_al_tx = (mac_cfg_tx_comp_stru *)puc_param;

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_always_tx::pst_dmac_vap or pst_dmac_vap->pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


    /* ???????????????? */
    mac_vap_set_al_tx_flag(pst_mac_vap, pst_event_set_al_tx->uc_param);
    mac_vap_set_al_tx_payload_flag(pst_mac_vap, pst_event_set_al_tx->en_payload_flag);
    pst_dmac_vap->pst_hal_device->uc_al_tx_flag = pst_event_set_al_tx->uc_param;
    pst_dmac_vap->pst_hal_device->bit_al_tx_flag = pst_event_set_al_tx->uc_param;

    /* ???????????? */
    pst_mac_vap->st_cap_flag.bit_keepalive = OAL_TRUE;
    //hal_rf_test_disable_al_tx(pst_hal_device_base);


#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    uc_pm_off = 0;

    dmac_config_set_pm_switch(pst_mac_vap, 0, &uc_pm_off);

#ifndef WIN32
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_config_set_always_tx::pst_mac_device[%d] is NULL!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }


#ifdef _PRE_WLAN_FEATURE_STA_PM
    dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
#endif   /* _PRE_WLAN_FEATURE_STA_PM */
#endif  /* WIN32 */
#endif

    if (OAL_SWITCH_ON == pst_dmac_vap->pst_hal_device->uc_al_tx_flag)
    {
        pst_mac_vap->st_cap_flag.bit_keepalive = OAL_FALSE;
        /* hal_rf_test_enable_al_tx????????????????dmac_tx_data??????  */
        /* ????????????51????????????????????is_not_first_run */
        if (OAL_TRUE == pst_mac_vap->bit_first_run)
        {
            return OAL_SUCC;
        }
        mac_vap_set_al_tx_first_run(pst_mac_vap, OAL_TRUE);


        /* ????TPC code???????????????????? */
        ul_ret = dmac_alg_cfg_channel_notify(pst_mac_vap, CH_BW_CHG_TYPE_MOVE_WORK);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_always_tx_1102:dmac_config_al_tx_set_pow fail!}");
            return ul_ret;
        }

        return dmac_config_al_tx_bcast_pkt(pst_mac_vap, pst_event_set_al_tx->ul_payload_len);

    }
    else
    {
        pst_mac_vap->st_cap_flag.bit_keepalive = OAL_TRUE;
        hal_rf_test_disable_al_tx(pst_dmac_vap->pst_hal_device);
    #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        dmac_config_update_rate_pow_table();        /* resume rate_pow_table */
        //dmac_config_update_scaling_reg();           /* resume phy scaling reg */
    #endif  /* _PRE_PLAT_FEATURE_CUSTOMIZE */
        mac_vap_set_al_tx_first_run(pst_mac_vap, OAL_FALSE);
    }

    return OAL_SUCC;
}
#endif /* #ifdef _PRE_WLAN_FEATURE_ALWAYS_TX */

#ifdef _PRE_WLAN_RF_CALI

OAL_STATIC oal_uint32  dmac_config_set_cali_vref(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_set_cali_vref_stru      *pst_cali_vref;

    /* ???????? */
    pst_cali_vref = (mac_cfg_set_cali_vref_stru *)puc_param;
    if (OAL_PTR_NULL == pst_cali_vref)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_cali_vref::pst_cali_vref null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hal_rf_cali_set_vref(pst_mac_vap->st_channel.en_band,pst_cali_vref->uc_chain_idx,
                            pst_cali_vref->uc_band_idx, pst_cali_vref->us_vref_value);

    return OAL_SUCC;

}
#endif

#ifdef _PRE_WLAN_FEATURE_11K


OAL_STATIC oal_uint32  dmac_config_bcn_table_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                   *pst_dmac_vap;
    oal_uint8                        uc_switch;

    uc_switch = puc_param[0];

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_bcn_table_switch::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_dmac_vap->bit_bcn_table_switch = uc_switch;
    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_bcn_table_switch:para val[%d]!}", uc_switch);
    return OAL_SUCC;
}
#endif //_PRE_WLAN_FEATURE_11K



OAL_STATIC oal_uint32  dmac_config_voe_enable(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                   *pst_dmac_vap;
    oal_uint8                        uc_switch;

    uc_switch = puc_param[0];

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_voe_enable::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    //pst_dmac_vap->bit_voe_enable = uc_switch;
#ifdef _PRE_WLAN_FEATURE_11K
    pst_dmac_vap->bit_11k_enable = ((uc_switch & 0x07) & BIT2) ? OAL_TRUE : OAL_FALSE;
    pst_dmac_vap->bit_11v_enable = ((uc_switch & 0x07) & BIT1) ? OAL_TRUE : OAL_FALSE;
#endif
#ifdef _PRE_WLAN_FEATURE_11R
    pst_dmac_vap->bit_11r_enable = ((uc_switch & 0x07) & BIT0) ? OAL_TRUE : OAL_FALSE;
#endif
    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_voe_enable:para val[%d]!}", uc_switch);
    return OAL_SUCC;
}

oal_uint32 g_ul_al_ampdu_num               = WLAN_AMPDU_TX_MAX_NUM; /*ampdu ????????????*/
//oal_uint32 g_ul_first_timestamp = 0;    /*????????????????????????*/


OAL_STATIC oal_uint32  dmac_config_always_rx_set_hal(hal_to_dmac_device_stru *pst_hal_device_base, oal_uint8 uc_switch)
{
    /*????????*/
    oal_irq_disable();

    /* ???????????? */
    hal_set_machw_tx_suspend(pst_hal_device_base);

    /* ????mac phy???? */
    hal_disable_machw_phy_and_pa(pst_hal_device_base);

    /*????????????????????????*/
    frw_event_flush_event_queue(FRW_EVENT_TYPE_WLAN_TX_COMP);
    frw_event_flush_event_queue(FRW_EVENT_TYPE_WLAN_CRX);
    frw_event_flush_event_queue(FRW_EVENT_TYPE_WLAN_DRX);

    /* flush????5?????????? */
    dmac_tx_reset_flush(pst_hal_device_base);

    /*??????????????????*/
    hal_clear_hw_fifo(pst_hal_device_base);

    hal_psm_clear_mac_rx_isr(pst_hal_device_base);

    /*????macphy*/
    hal_reset_phy_machw(pst_hal_device_base,
                        HAL_RESET_HW_TYPE_MAC_PHY,
                        HAL_RESET_MAC_ALL,
                        OAL_FALSE,
                        OAL_FALSE);

    /*??????bit*/
    pst_hal_device_base->bit_al_rx_flag = uc_switch;

    /*??????????????????????????????????????????????????????????*/
    //dmac_reset_rx_dscr_queue_flush(pst_hal_device_base);
    if(uc_switch)
    {
        /* ?????????????????????? */
        hal_rx_destroy_dscr_queue(pst_hal_device_base,OAL_TRUE);

        /* ???????????????????????? */
        hal_al_rx_init_dscr_queue(pst_hal_device_base);
    }
    else
    {
        /* ?????????????????????????? */
        hal_al_rx_destroy_dscr_queue(pst_hal_device_base);

        /* ???????????????????? */
        hal_rx_init_dscr_queue(pst_hal_device_base,OAL_TRUE);
    }

    /*????????*/
    oal_irq_enable();

    /* ???? mac phy????*/
    hal_enable_machw_phy_and_pa(pst_hal_device_base);

    /* ???????????? */
    hal_set_machw_tx_resume(pst_hal_device_base);

    /*??????????*/
    hal_config_always_rx(pst_hal_device_base, uc_switch);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_always_rx(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                   *pst_dmac_vap;
    hal_to_dmac_device_stru         *pst_hal_device_base;
    mac_device_stru                 *pst_mac_device;
    oal_uint8                        uc_al_rx_flag = 0;
#ifndef WIN32
    oal_uint8                        uc_pm_off;
#endif
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                        uc_hipriv_ack = OAL_FALSE;
#endif

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_always_rx::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hal_device_base = pst_dmac_vap->pst_hal_device;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device_base))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_always_rx::pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????????????????????????????????????????????????? */
    pst_hal_device_base->bit_al_txrx_ampdu_num = g_ul_al_ampdu_num;

    /* ???????????????????????????????????????? */
    if(*(oal_bool_enum_uint8 *)puc_param == pst_hal_device_base->bit_al_rx_flag)
    {
    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
        uc_hipriv_ack = OAL_TRUE;
        dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif
        return OAL_SUCC;
    }

    /* ???????????????? */
    uc_al_rx_flag = *(oal_bool_enum_uint8 *)puc_param;
#ifndef WIN32
    uc_pm_off = 0;

    dmac_config_set_pm_switch(pst_mac_vap, 0, &uc_pm_off);
#endif

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
#ifdef _PRE_WLAN_FEATURE_STA_PM
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_config_set_always_rx::pst_mac_device[%d] is NULL!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
#endif  /* _PRE_WLAN_FEATURE_STA_PM */
#endif

    if ((HAL_ALWAYS_RX_RESERVED == uc_al_rx_flag)
    || (HAL_ALWAYS_RX_DISABLE == uc_al_rx_flag))
    {
        dmac_config_always_rx_set_hal(pst_hal_device_base, uc_al_rx_flag);
    }

#ifdef _PRE_WLAN_FEATURE_ANTI_INTERF
    if (HAL_ALWAYS_RX_RESERVED == uc_al_rx_flag)
    {
        /* ???????????????????????? */
        dmac_alg_anti_intf_switch(pst_mac_device, OAL_FALSE);
    }
    else
    {
        /* ?????????????????????????????? */
        dmac_alg_anti_intf_switch(pst_mac_device, 2);
    }
#endif

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))

OAL_STATIC oal_void  dmac_config_report_efuse_reg(mac_vap_stru *pst_mac_vap)
{
    mac_device_stru      *pst_device;
    oal_uint32            ul_addr;
    oal_uint16            us_val = 0;
    oal_uint16            g_us_efuse_reg_buffer[16];
    oal_uint8             uc_reg_num = 0;

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{dmac_config_report_efuse_reg::pst_device null.");
        return;
    }

    for (ul_addr = 0x50000744; ul_addr <= 0x50000780; ul_addr += 4)
    {
        hal_reg_info16(pst_device->pst_device_stru, ul_addr, &us_val);
        g_us_efuse_reg_buffer[uc_reg_num] = us_val;
        uc_reg_num++;
    }

    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_REG_INFO, OAL_SIZEOF(g_us_efuse_reg_buffer), (oal_uint8 *)&g_us_efuse_reg_buffer);
}

#endif

oal_uint32  dmac_config_pcie_pm_level(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    mac_cfg_pcie_pm_level_stru      *pst_pcie_pm_level;
    mac_device_stru                 *pst_device;

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_pcie_pm_level = (mac_cfg_pcie_pm_level_stru *)puc_param;

    hal_set_pcie_pm_level(pst_device->pst_device_stru, pst_pcie_pm_level->uc_pcie_pm_level);
#endif

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_reg_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int8             *pc_token;
    oal_int8             *pc_end;
    oal_int8             *pc_ctx;
    oal_int8             *pc_sep = " ";
    oal_bool_enum_uint8   en_reg_info32 = OAL_TRUE;

    struct dmac_reg_info_stru
    {
        oal_int8     *pc_reg_type;
        oal_uint32    ul_start_addr;
        oal_uint32    ul_end_addr;

    }st_dmac_reg_info = {0};

    /* ????????????(32/16) */
    pc_token = oal_strtok((oal_int8 *)puc_param, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    /*lint -e960*/
    /* ????????"regtype(soc/mac/phy/all) startaddr endaddr"??????????????32???????????? */
    if (0 == oal_strcmp(pc_token, "all"))
    {
#ifdef _PRE_WLAN_DFT_STAT
        dmac_dft_report_all_ota_state(pst_mac_vap);
#endif
        return OAL_SUCC;
    }
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
    /*????: ????????efuse????????*/
    if (0 == oal_strcmp(pc_token, "efuse"))
    {
        dmac_config_report_efuse_reg(pst_mac_vap);
        return OAL_SUCC;
    }
#endif
    if ((0 != oal_strcmp(pc_token, "soc")) &&
    (0 != oal_strcmp(pc_token, "mac")) &&
    (0 != oal_strcmp(pc_token, "phy")))
    {
        if (0 == oal_strcmp(pc_token, "16"))
        {
            en_reg_info32 = OAL_FALSE;
        }

        /* ?????????????????????? */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (NULL == pc_token)
        {
            return OAL_FAIL;
        }
        /* ???????? */
        if ((0 != oal_strcmp(pc_token, "soc")) &&
        (0 != oal_strcmp(pc_token, "mac")) &&
        (0 != oal_strcmp(pc_token, "phy")))
        {
            return OAL_FAIL;
        }

    }

    /*lint +e960*/

    st_dmac_reg_info.pc_reg_type = pc_token;

    /* ???????????? */
    pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    st_dmac_reg_info.ul_start_addr = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);

    /* ???????????? */
    pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    st_dmac_reg_info.ul_end_addr = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);

    if (OAL_TRUE == en_reg_info32)
    {
        /*lint -e960*/
        dmac_config_reg_display_test(pst_mac_vap, st_dmac_reg_info.ul_start_addr, st_dmac_reg_info.ul_end_addr);
        /*lint +e960*/
        return OAL_SUCC;
    }
    /*lint -e960*/
    dmac_config_reg_display_test16(pst_mac_vap, st_dmac_reg_info.ul_start_addr, st_dmac_reg_info.ul_end_addr);
    /*lint +e960*/
    return OAL_SUCC;
}

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))


OAL_STATIC oal_uint32  dmac_config_sdio_flowctrl(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int8             *pc_token;
    oal_int8             *pc_end;
    oal_int8             *pc_ctx;               /* ???????????????????????????????? */
    oal_int8             *pc_sep = " ";         /* ????????????:?????????????? */
    oal_uint16            us_UsedMemForStop;
    oal_uint16            us_UsedMemForstart;

    /* ???????????????? */
    /* strtok???????????????????????????????? */
    pc_token = oal_strtok((oal_int8 *)puc_param, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    us_UsedMemForstart = (oal_uint16)oal_strtol(pc_token, &pc_end, 10);

    /* ???????????????? */
    /* ????strtok???????????????????? */
    pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    us_UsedMemForStop = (oal_uint16)oal_strtol(pc_token, &pc_end, 10);
    /* cfg_hisi.ini?????????????? */
    g_usUsedMemForstart  = (us_UsedMemForstart >= 1 && us_UsedMemForstart <= OAL_SDIO_FLOWCTRL_MAX) ? us_UsedMemForstart : g_usUsedMemForstart;
    g_usUsedMemForStop  = (us_UsedMemForStop >= 1 && us_UsedMemForStop <= OAL_SDIO_FLOWCTRL_MAX) ? us_UsedMemForStop : g_usUsedMemForStop;

    OAM_WARNING_LOG2(0, OAM_SF_CFG, "{dmac_config_sdio_flowctrl::UsedMemForStop=%d, UsedMemForstart=%d.", us_UsedMemForStop, us_UsedMemForstart);

    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32  dmac_config_reg_write(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_int8             *pc_token;
    oal_int8             *pc_end;
    oal_int8             *pc_ctx;
    oal_int8             *pc_sep = " ";
    oal_bool_enum_uint8   en_reg_info32 = OAL_TRUE;
    struct dmac_reg_write_stru
    {
        oal_int8     *pc_reg_type;
        oal_uint32    ul_addr;
        oal_uint32    ul_val;    /* ??32???????? */
    }st_dmac_reg_write = {0};

    /* ??????????????(32/16) */
    pc_token = oal_strtok((oal_int8 *)puc_param, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    /*lint -e960*/
    /* ????????" regtype(soc/mac/phy) addr val"??????????????32???????????? */
    if ((0 != oal_strcmp(pc_token, "soc")) && (0 != oal_strcmp(pc_token, "mac")) && (0 != oal_strcmp(pc_token, "phy")))
    {
        if (0 == oal_strcmp(pc_token, "16"))
        {
            en_reg_info32 = OAL_FALSE;
        }

        /* ?????????????????????? */
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        if (NULL == pc_token)
        {
            return OAL_FAIL;
        }
        /* ???????? */
        if ((0 != oal_strcmp(pc_token, "soc")) && (0 != oal_strcmp(pc_token, "mac")) && (0 != oal_strcmp(pc_token, "phy")))
        {
            return OAL_FAIL;
        }

    }
    /*lint +e960*/
    st_dmac_reg_write.pc_reg_type = pc_token;

    /* ?????????????? */
    pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    st_dmac_reg_write.ul_addr = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);

    /* ???????????????? */
    pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
    if (NULL == pc_token)
    {
        return OAL_FAIL;
    }

    if (OAL_TRUE == en_reg_info32)
    {
        st_dmac_reg_write.ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);
        /*lint -e960*/
        dmac_config_reg_write_test(pst_mac_vap, st_dmac_reg_write.ul_addr, st_dmac_reg_write.ul_val);
        /*lint +e960*/
        return OAL_SUCC;
    }

    st_dmac_reg_write.ul_val = (oal_uint32)oal_strtol(pc_token, &pc_end, 16);

    /*lint -e960*/
    dmac_config_reg_write_test16(pst_mac_vap, st_dmac_reg_write.ul_addr, (oal_uint16)(st_dmac_reg_write.ul_val));
    /*lint +e960*/

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SMARTANT
oal_uint32 gul_dual_antenna_enable = 0;

oal_uint32 dmac_dual_antenna_register_dmac_misc_event(hal_dmac_misc_sub_type_enum en_event_type, oal_uint32 (*p_func)(frw_event_mem_stru *))
{
    if(en_event_type >= HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT || NULL == p_func)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "dmac_alg_register_dmac_misc_event fail");
        return  OAL_FAIL;
    }
    g_ast_dmac_misc_event_sub_table[en_event_type].p_func = p_func;
    return OAL_SUCC;
}



oal_uint32  dmac_dual_antenna_unregister_dmac_misc_event(hal_dmac_misc_sub_type_enum en_event_type)
{
    if(en_event_type >= HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT)
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "dmac_alg_unregister_dmac_misc_event fail");
        return  OAL_FAIL;
    }
    g_ast_dmac_misc_event_sub_table[en_event_type].p_func = NULL;
    return OAL_SUCC;
}


oal_uint32 dmac_dual_antenna_notify_alg(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    mac_device_stru *pst_mac_device;
    oal_uint32 *pul_status;
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_dual_antenna_notify_alg::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_event = (frw_event_stru *)pst_event_mem->puc_data;
    pst_mac_device = mac_res_get_dev(pst_event->st_event_hdr.uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_dual_antenna_notify_alg::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#if 1
    pul_status = (oal_uint32 *)pst_event->auc_event_data;
    OAM_WARNING_LOG1(0, OAM_SF_SMART_ANTENNA, "notify alg, status:%d.", *pul_status);
    dmac_alg_cfg_dual_antenna_state_notify(pst_mac_device, *pul_status);
#endif
    return OAL_SUCC;
}


oal_uint32 dmac_config_dual_antenna_vap_check(mac_vap_stru *pst_mac_vap)
{
    oal_uint32 ul_result;
    mac_device_stru *pst_mac_device;
    oal_uint8 uc_vap_idx;
    mac_vap_stru *pst_mac_vap_temp = OAL_PTR_NULL;
    oal_uint8 uc_no_vap = 1;
    hal_dual_antenna_check_status_stru *pst_dual_antenna_check_status;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_config_dual_antenna_vap_check::pst_mac_device null.}");
        return DUAL_ANTENNA_ALG_CLOSE;
    }
    pst_dual_antenna_check_status = &(pst_mac_device->pst_device_stru->st_dual_antenna_check_status);

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++)
    {
        pst_mac_vap_temp = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_vap_temp))
        {
            OAM_WARNING_LOG1(0, OAM_SF_SMART_ANTENNA, "{dmac_config_dual_antenna_vap_check::vap is null! vap id is %d}", pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        uc_no_vap = 0;

        if ((WLAN_VAP_MODE_BSS_AP == pst_mac_vap_temp->en_vap_mode)
            || (WLAN_P2P_CL_MODE == pst_mac_vap_temp->en_p2p_mode))
        {
            pst_dual_antenna_check_status->bit_vap_mode = 1;
        }
        else if ((WLAN_LEGACY_VAP_MODE == pst_mac_vap_temp->en_p2p_mode)
                && (WLAN_BAND_5G == pst_mac_vap_temp->st_channel.en_band))
        {
            pst_dual_antenna_check_status->bit_vap_mode = 1;
        }
        else
        {
            pst_dual_antenna_check_status->bit_vap_mode = 0;
        }

        if (pst_dual_antenna_check_status->bit_vap_mode)
        {
            OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_SMART_ANTENNA,
                                "{dmac_config_dual_antenna_vap_mode_check::dual_antenna on 1, vap_mode: %d, p2p_mode: %d, channel: %d.}",
                                pst_mac_vap_temp->en_vap_mode, pst_mac_vap_temp->en_p2p_mode, pst_mac_vap_temp->st_channel.en_band);
            break;
        }
    }

    pst_dual_antenna_check_status->bit_vap_mode |= uc_no_vap;

    hal_dual_antenna_switch(pst_dual_antenna_check_status->bit_vap_mode, 0, &ul_result);

    return DUAL_ANTENNA_AVAILABLE;
}


oal_uint32 dmac_config_dual_antenna_check(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32 ul_ret;
    mac_device_stru *pst_mac_device;
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_config_dual_antenna_check::pst_mac_device null.}");
        return DUAL_ANTENNA_ALG_CLOSE;
    }
    hal_dual_antenna_check(pst_mac_device->pst_device_stru, &ul_ret);
    return ul_ret;
}


oal_uint32 dmac_dual_antenna_set_ant(oal_uint8 uc_param)
{
    oal_uint32 ul_result = DUAL_ANTENNA_ALG_CLOSE;
    hal_dual_antenna_switch(uc_param, 1, &ul_result);
    OAM_WARNING_LOG2(0, OAM_SF_SMART_ANTENNA, "{dmac_dual_antenna_set_ant::dual_antenna set to %d, result:%d.}",
                    uc_param, ul_result);
    return ul_result;
}

oal_uint32 dmac_dual_antenna_set_ant_at(oal_uint8 uc_param)
{
    oal_uint32 ul_result = DUAL_ANTENNA_ALG_CLOSE;
    hal_dual_antenna_switch_at(uc_param, &ul_result);
    OAM_WARNING_LOG2(0, OAM_SF_SMART_ANTENNA, "{dmac_dual_antenna_set_ant_at::dual_antenna set to %d, result:%d.}",
                    uc_param, ul_result);
    return ul_result;
}


oal_uint32 dmac_config_dual_antenna_set_ant(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    if(OAL_PTR_NULL == puc_param)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_config_set_ant:: point null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return dmac_dual_antenna_set_ant_at( *puc_param);
}


oal_uint32 dmac_dual_antenna_init(oal_void)
{
    if (OAL_SUCC != dmac_dual_antenna_register_dmac_misc_event(HAL_EVENT_DMAC_DUAL_ANTENNA_SWITCH, dmac_dual_antenna_notify_alg))
    {
        OAM_ERROR_LOG0(0, OAM_SF_SMART_ANTENNA, "{dmac_device_init HAL_EVENT_DMAC_DUAL_ANTENNA_SWITCH fail!}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


oal_uint32  dmac_config_get_ant_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_atcmdsrv_ant_info_response_event       st_atcmdsrv_ant_info;
    oal_uint8                                   uc_ant_type;                            //??????????????0??WIFI????????1??????????
    oal_uint32                                  ul_last_ant_change_time_ms;                //????????????????
    oal_uint32                                  ul_ant_change_number;                   //????????????
    oal_uint32                                  ul_main_ant_time_s;                       //????WIFI??????????
    oal_uint32                                  ul_aux_ant_time_s;                        //??????????(????????)????
    oal_uint32                                  ul_total_time_s;                        //WIFI????????
    dmac_alg_cfg_get_ant_info_notify(pst_mac_vap, &uc_ant_type, &ul_last_ant_change_time_ms,
        &ul_ant_change_number, &ul_main_ant_time_s, &ul_aux_ant_time_s, &ul_total_time_s);
    st_atcmdsrv_ant_info.uc_event_id                = OAL_ATCMDSRV_GET_ANT_INFO;
    st_atcmdsrv_ant_info.uc_ant_type                = uc_ant_type;
    st_atcmdsrv_ant_info.ul_last_ant_change_time_ms = ul_last_ant_change_time_ms;
    st_atcmdsrv_ant_info.ul_ant_change_number       = ul_ant_change_number;
    st_atcmdsrv_ant_info.ul_main_ant_time_s         = ul_main_ant_time_s;
    st_atcmdsrv_ant_info.ul_aux_ant_time_s          = ul_aux_ant_time_s;
    st_atcmdsrv_ant_info.ul_total_time_s            = ul_total_time_s;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_GET_ANT_INFO,
        OAL_SIZEOF(dmac_atcmdsrv_ant_info_response_event), (oal_uint8 *)&st_atcmdsrv_ant_info);
    return OAL_SUCC;
}
oal_uint32  dmac_config_double_ant_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_query_response_event                    st_atcmdsrv_double_ant_switch;
    mac_device_stru                             *pst_mac_device;
    oal_uint32                                   ul_ret = OAL_FAIL;
    oal_uint8 uc_switch;
    oal_uint8 uc_by_ini;

    uc_switch = *puc_param;
    uc_by_ini = *(puc_param + 1);

    if (uc_by_ini)
    {
        OAM_WARNING_LOG1(0, OAM_SF_SMART_ANTENNA, "{dmac_config_double_ant_switch::ini, enable: %d.}", uc_switch);
        gul_dual_antenna_enable = uc_switch;

        /* ???????? */
        hal_dual_antenna_init();
    }

    pst_mac_device = ALG_GET_DEVICE_STRU(pst_mac_vap->uc_device_id);
    ul_ret = dmac_alg_cfg_double_ant_switch_notify(pst_mac_device, uc_switch);

    st_atcmdsrv_double_ant_switch.query_event   = OAL_ATCMDSRV_DOUBLE_ANT_SW;
    st_atcmdsrv_double_ant_switch.reserve[0]       = (oal_uint8)ul_ret;//????????????????
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_DOUBLE_ANT_SW,
        OAL_SIZEOF(dmac_query_response_event), (oal_uint8 *)&st_atcmdsrv_double_ant_switch);
    return OAL_SUCC;
}
#endif

OAL_STATIC oal_uint32  dmac_config_vap_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    dmac_vap_stru               *pst_dmac_vap;
    hal_to_dmac_device_stru     *pst_hal_device_base;
    mac_device_stru             *pst_mac_device;
#endif
    oal_uint8                    uc_hipriv_ack = OAL_FALSE;

    /* 51???????? */
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    pst_dmac_vap    = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_vap_info::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hal_device_base = pst_dmac_vap->pst_hal_device;
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device_base) || OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_vap_info::pst_hal_device_base = %p,pst_mac_device = %p.}",pst_hal_device_base,pst_mac_device);
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAL_IO_PRINT("al_tx:  %d\t al_rx: %d\n"
                 "txch:   %d\t rxch:  %d\n"
                 "rate/mcs/mcsac: %d\n"
                "\n******************************************************\n\n",
                pst_hal_device_base->bit_al_tx_flag,pst_hal_device_base->bit_al_rx_flag,
                pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.bit_tx_chain_selection,
                pst_mac_device->uc_rx_chain,
                DMAC_GET_VAP_RATE((pst_dmac_vap->uc_protocol_rate_dscr >> 6) & 0x3,
                pst_dmac_vap->st_tx_data_mcast.ast_per_rate[0].rate_bit_stru.un_nss_rate));

#endif
    /* ????????????Success */
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    //print protection info
    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "en_protection_mode=%d. uc_obss_non_erp_aging_cnt=%d. uc_obss_non_ht_aging_cnt=%d. bit_auto_protection=%d",
                                                                                                                pst_mac_vap->st_protection.en_protection_mode,
                                                                                                                pst_mac_vap->st_protection.uc_obss_non_erp_aging_cnt,
                                                                                                                pst_mac_vap->st_protection.uc_obss_non_ht_aging_cnt,
                                                                                                                pst_mac_vap->st_protection.bit_auto_protection);

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "bit_obss_non_erp_present=%d. bit_obss_non_ht_present=%d. bit_rts_cts_protect_mode=%d. bit_lsig_txop_protect_mode=%d.",
                                                                                                                pst_mac_vap->st_protection.bit_obss_non_erp_present,
                                                                                                                pst_mac_vap->st_protection.bit_obss_non_ht_present,
                                                                                                                pst_mac_vap->st_protection.bit_rts_cts_protect_mode,
                                                                                                                pst_mac_vap->st_protection.bit_lsig_txop_protect_mode);

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_sta_no_short_slot_num=%d. uc_sta_no_short_preamble_num=%d. uc_sta_non_erp_num=%d. uc_sta_non_ht_num=%d.",
                                                                                                                pst_mac_vap->st_protection.uc_sta_no_short_slot_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_no_short_preamble_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_non_erp_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_non_ht_num);

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "uc_sta_non_gf_num=%d. uc_sta_20M_only_num=%d. uc_sta_no_40dsss_cck_num=%d. uc_sta_no_lsig_txop_num=%d. ",
                                                                                                                pst_mac_vap->st_protection.uc_sta_non_gf_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_20M_only_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_no_40dsss_cck_num,
                                                                                                                pst_mac_vap->st_protection.uc_sta_no_lsig_txop_num);

    return OAL_SUCC;
}


OAL_STATIC oal_void  dmac_config_set_machw_wmm(hal_to_dmac_vap_stru *pst_hal_vap, mac_vap_stru *pst_mac_vap)
{
    wlan_wme_ac_type_enum_uint8     en_ac_type;
    /* ????AIFSN */
    hal_vap_set_machw_aifsn_all_ac(pst_hal_vap,
                                   (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BK].ul_dot11QAPEDCATableAIFSN,
                                   (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BE].ul_dot11QAPEDCATableAIFSN,
                                   (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VI].ul_dot11QAPEDCATableAIFSN,
                                   (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VO].ul_dot11QAPEDCATableAIFSN);

    /* cwmin cwmax */
    for (en_ac_type = 0; en_ac_type < WLAN_WME_AC_BUTT; en_ac_type++)
    {
        hal_vap_set_edca_machw_cw(pst_hal_vap,
                                 (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[en_ac_type].ul_dot11QAPEDCATableCWmax,
                                 (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[en_ac_type].ul_dot11QAPEDCATableCWmin,
                                  en_ac_type);
    }

#if 0
    hal_vap_set_machw_cw_bk(pst_hal_vap,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BK].ul_dot11QAPEDCATableCWmax,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BK].ul_dot11QAPEDCATableCWmin);
    hal_vap_set_machw_cw_be(pst_hal_vap,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BE].ul_dot11QAPEDCATableCWmax,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BE].ul_dot11QAPEDCATableCWmin);
    hal_vap_set_machw_cw_vi(pst_hal_vap,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VI].ul_dot11QAPEDCATableCWmax,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VI].ul_dot11QAPEDCATableCWmin);
    hal_vap_set_machw_cw_vo(pst_hal_vap,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VO].ul_dot11QAPEDCATableCWmax,
                            (oal_uint8)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VO].ul_dot11QAPEDCATableCWmin);
#endif
    /* txop */
    hal_vap_set_machw_txop_limit_bkbe(pst_hal_vap,
                                      (oal_uint16)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BE].ul_dot11QAPEDCATableTXOPLimit,
                                      (oal_uint16)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_BK].ul_dot11QAPEDCATableTXOPLimit);
    hal_vap_set_machw_txop_limit_vivo(pst_hal_vap,
                                      (oal_uint16)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VO].ul_dot11QAPEDCATableTXOPLimit,
                                      (oal_uint16)pst_mac_vap->pst_mib_info->st_wlan_mib_qap_edac[WLAN_WME_AC_VI].ul_dot11QAPEDCATableTXOPLimit);


}
#ifdef _PRE_WLAN_FEATURE_IP_FILTER

oal_void dmac_clear_ip_filter_btable(mac_vap_stru *pst_mac_vap)
{
    /* ?????????? */
    g_st_dmac_board.st_rx_ip_filter.uc_btable_items_num = 0;
    if (OAL_PTR_NULL == g_st_dmac_board.st_rx_ip_filter.pst_filter_btable)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_clear_ip_filter_btable::The pst_filter_btable is NULL!!}");
        return;
    }
    OAL_MEMZERO((oal_uint8 *)(g_st_dmac_board.st_rx_ip_filter.pst_filter_btable), MAC_MAX_IP_FILTER_BTABLE_SIZE);
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{dmac_clear_ip_filter_btable::Btable clear done.}");
}


oal_uint32 dmac_update_ip_filter_btable(mac_ip_filter_cmd_stru *pst_cmd_info)
{
    oal_uint8    uc_items_idx;
    oal_uint8    uc_add_items_num;

    if (OAL_PTR_NULL == g_st_dmac_board.st_rx_ip_filter.pst_filter_btable)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_update_ip_filter_btable::The pst_filter_btable is NULL!!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    g_st_dmac_board.st_rx_ip_filter.uc_btable_items_num = 0;
    OAL_MEMZERO((oal_uint8 *)(g_st_dmac_board.st_rx_ip_filter.pst_filter_btable), MAC_MAX_IP_FILTER_BTABLE_SIZE);
    uc_add_items_num = OAL_MIN(g_st_dmac_board.st_rx_ip_filter.uc_btable_size, pst_cmd_info->uc_item_count);
    for(uc_items_idx = 0; uc_items_idx < uc_add_items_num; uc_items_idx++)
    {
        g_st_dmac_board.st_rx_ip_filter.pst_filter_btable[uc_items_idx].uc_protocol = pst_cmd_info->ast_filter_items_items[uc_items_idx].uc_protocol;
        g_st_dmac_board.st_rx_ip_filter.pst_filter_btable[uc_items_idx].us_port = pst_cmd_info->ast_filter_items_items[uc_items_idx].us_port;
    }
    g_st_dmac_board.st_rx_ip_filter.uc_btable_items_num = uc_add_items_num;
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_update_ip_filter_btable::Btable update done,renew %d items.}", uc_add_items_num);

    return OAL_SUCC;
}


oal_uint32 dmac_config_update_ip_filter(frw_event_mem_stru *pst_event_mem)
{
    oal_uint32                  ul_ret;
    frw_event_stru             *pst_event;
    dmac_tx_event_stru         *pst_dtx_event;
    mac_ip_filter_cmd_stru     *pst_cmd_info;
    mac_vap_stru               *pst_mac_vap;


    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "{dmac_config_update_ip_filter::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event        = (frw_event_stru *)pst_event_mem->puc_data;
    pst_dtx_event    = (dmac_tx_event_stru *)pst_event->auc_event_data;
    if(OAL_PTR_NULL == pst_dtx_event->pst_netbuf)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::The cmd_info is NULL!.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cmd_info = (mac_ip_filter_cmd_stru *)OAL_NETBUF_DATA(pst_dtx_event->pst_netbuf);
    pst_mac_vap  = mac_res_get_mac_vap(pst_event->st_event_hdr.uc_vap_id);
    if(OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::Can not find mac_vap %d, ignore the cmd!.}",
                        pst_event->st_event_hdr.uc_vap_id);
        oal_netbuf_free(pst_dtx_event->pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????????????????????? */
    if (OAL_TRUE != pst_mac_vap->st_cap_flag.bit_ip_filter)
    {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::Func not enable, ignore the cmd!.}");
        oal_netbuf_free(pst_dtx_event->pst_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* step2, ?????????????????????? */
    if (MAC_IP_FILTER_ENABLE == pst_cmd_info->en_cmd)
    {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::Change state from [%d] to [%d].}",
                        g_st_dmac_board.st_rx_ip_filter.en_state,
                        ((OAL_TRUE == pst_cmd_info->en_enable)? MAC_RX_IP_FILTER_WORKING : MAC_RX_IP_FILTER_STOPED));
        g_st_dmac_board.st_rx_ip_filter.en_state = (OAL_TRUE == pst_cmd_info->en_enable)? MAC_RX_IP_FILTER_WORKING : MAC_RX_IP_FILTER_STOPED;
    }
    else if(MAC_IP_FILTER_UPDATE_BTABLE == pst_cmd_info->en_cmd)
    {
        /* ?????????? */
        ul_ret = dmac_update_ip_filter_btable(pst_cmd_info);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::update_ip_filter_btable FAIL!!}");
            oal_netbuf_free(pst_dtx_event->pst_netbuf);
            return ul_ret;
        }
    }
    else if (MAC_IP_FILTER_CLEAR == pst_cmd_info->en_cmd)
    {
        /* ?????????? */
        dmac_clear_ip_filter_btable(pst_mac_vap);

    }
    else
    {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_config_update_ip_filter::Command does not support!cmd_len %d.}", pst_cmd_info->en_cmd);
        oal_netbuf_free(pst_dtx_event->pst_netbuf);
        return OAL_FAIL;
    }

    oal_netbuf_free(pst_dtx_event->pst_netbuf);

    return OAL_SUCC;
}

#endif //_PRE_WLAN_FEATURE_IP_FILTER
#ifdef _PRE_WLAN_FEATURE_VOWIFI

OAL_STATIC oal_uint32  dmac_config_vowifi_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32            ul_ret;
    mac_cfg_vowifi_stru  *pst_cfg_vowifi;
    dmac_vap_stru        *pst_dmac_vap;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_vowifi_info::null param,pst_mac_vap=%d puc_param=%d.}",
                       pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_tbtt_event_sta, pst_dmac_vap is null.");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((OAL_PTR_NULL == pst_dmac_vap->pst_vowifi_status)||
        (OAL_PTR_NULL == pst_mac_vap->pst_vowifi_cfg_param))
    {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_vowifi_info::null param,pst_vowifi_status=%p pst_vowifi_cfg_param=%p.}",
                       pst_dmac_vap->pst_vowifi_status, pst_mac_vap->pst_vowifi_cfg_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_vowifi = (mac_cfg_vowifi_stru *)puc_param;
    ul_ret = mac_vap_set_vowifi_param(pst_mac_vap, pst_cfg_vowifi->en_vowifi_cfg_cmd, pst_cfg_vowifi->uc_value);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_vowifi_info::param[%d] set failed[%d].}", pst_cfg_vowifi->en_vowifi_cfg_cmd, ul_ret);
    }

    /* ????vowifi?????????????????????????? */
    dmac_vap_vowifi_init(pst_dmac_vap);
    return ul_ret;
}

oal_uint32  dmac_config_vowifi_report(dmac_vap_stru *pst_dmac_vap)
{
    oal_uint32       ul_ret;
    mac_vap_stru    *pst_mac_vap;
    dmac_user_stru  *pst_dmac_user;

    /* ??????Legacy sta???????????? */
    pst_mac_vap = &pst_dmac_vap->st_vap_base_info;
    if ((OAL_PTR_NULL == pst_dmac_vap->pst_vowifi_status)||
        (OAL_PTR_NULL == pst_mac_vap->pst_vowifi_cfg_param))
    {
        return OAL_SUCC;
    }

    /* ????up??????????vowifi????????????????vowifi???? */
    if (MAC_VAP_STATE_UP != pst_mac_vap->en_vap_state)
    {
        return OAL_SUCC;
    }

    /*??????????????*/
    if (VOWIFI_LOW_THRES_REPORT != pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode &&
        VOWIFI_HIGH_THRES_REPORT != pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode)
    {
        return OAL_SUCC;
    }

    /* "????vowifi????????"??????????????????????vowifi???? */
    if (OAL_FALSE == pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_reported)
    {
        OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_VOWIFI, "{dmac_config_vowifi_report::Mode[%d],rssi_thres[%d],period_ms[%d],trigger_count[%d].}",
                        pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode,
                        ((VOWIFI_LOW_THRES_REPORT == pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode)? pst_mac_vap->pst_vowifi_cfg_param->c_rssi_low_thres : pst_mac_vap->pst_vowifi_cfg_param->c_rssi_high_thres),
                        pst_mac_vap->pst_vowifi_cfg_param->us_rssi_period_ms,
                        pst_mac_vap->pst_vowifi_cfg_param->uc_trigger_count_thres);

        ul_ret = dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_VOWIFI_REPORT, 0, OAL_PTR_NULL);
        if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
        {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_VOWIFI, "{dmac_config_vowifi_report::dmac_send_sys_event failed[%d].}",ul_ret);
            return ul_ret;
        }

        pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_reported = OAL_TRUE;
    }
    else
    {
        OAM_WARNING_LOG0(0, OAM_SF_VOWIFI, "{dmac_config_vowifi_report::vowifi been reported once!}");
    }

    pst_dmac_user = (dmac_user_stru *)mac_res_get_dmac_user(pst_mac_vap->uc_assoc_vap_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_VOWIFI, "{dmac_config_vowifi_report::pst_dmac_user[%d] NULL.}", pst_mac_vap->uc_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????????????????????*/
    pst_dmac_vap->pst_vowifi_status->uc_rssi_trigger_cnt   = 0;
    pst_dmac_vap->pst_vowifi_status->ull_rssi_timestamp_ms = OAL_TIME_GET_STAMP_MS();
    pst_dmac_vap->pst_vowifi_status->ul_tx_failed        = pst_dmac_user->st_query_stats.ul_tx_failed;
    pst_dmac_vap->pst_vowifi_status->ul_tx_total         = pst_dmac_user->st_query_stats.ul_tx_total;

    return OAL_SUCC;
}

#endif /* _PRE_WLAN_FEATURE_VOWIFI */
#ifdef _PRE_WLAN_FEATURE_P2P

OAL_STATIC oal_uint32  dmac_add_p2p_cl_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                *pst_device;
    mac_cfg_add_vap_param_stru     *pst_param;
    dmac_vap_stru                  *pst_dmac_vap;
    hal_to_dmac_vap_stru           *pst_hal_vap = OAL_PTR_NULL;
    oal_uint8                       uc_vap_idx;
    mac_vap_stru                   *pst_mac_cfg_vap;

    pst_param = (mac_cfg_add_vap_param_stru *)puc_param;

    /* ????device */
    pst_mac_cfg_vap = (mac_vap_stru*)mac_res_get_mac_vap(pst_param->uc_cfg_vap_indx);
    if (OAL_PTR_NULL == pst_mac_cfg_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::pst_cfg_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_device = mac_res_get_dev(pst_mac_cfg_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device || OAL_PTR_NULL == pst_device->pst_device_stru)
    {
        OAM_ERROR_LOG0(pst_mac_cfg_vap->uc_vap_id, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::pst_device or pst_device->pst_device_stru null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????P2P CL ??P2P DEV ????VAP????P2P CL ??????????????????????????????dmac vap ???? */
    uc_vap_idx     = pst_device->st_p2p_info.uc_p2p0_vap_idx;/* uc_p2p0_vap_idx??????p2p0 ???????? */
    pst_dmac_vap   = (dmac_vap_stru *)mac_res_get_dmac_vap(uc_vap_idx);

    /* ????hmac??dmac????vap idx???????? */
    if (uc_vap_idx != pst_param->uc_vap_id)
    {
        OAM_ERROR_LOG2(pst_mac_cfg_vap->uc_vap_id, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::HMAC and DMAC vap indx not same!.saved_vap_id[%d], new_vap_id[%d]}",
                        uc_vap_idx, pst_param->uc_vap_id);
        return OAL_ERR_CODE_ADD_VAP_INDX_UNSYNC;
    }



#if defined (_PRE_WLAN_FEATURE_PROXYSTA)
    if (mac_vap_is_vsta(pst_mac_vap))
    {
        /* ??????Proxy STA??????hal ??????Proxy STA??????????????????hal?????? */
        hal_add_vap(pst_device->pst_device_stru, WLAN_VAP_MODE_PROXYSTA, uc_vap_idx, &pst_hal_vap);
    }
    else
    {
        hal_add_vap(pst_device->pst_device_stru, pst_param->en_vap_mode, uc_vap_idx, &pst_hal_vap);
    }
#elif defined (_PRE_WLAN_FEATURE_P2P)
    /* ??????vap ??????????????hal_vap */
    /* ????MAC ????????P2P ??????????????????
    *  GO ??????????hal_vap_id = 1,
    *  CL ??????????hal_vap_id = 5,
    *  ??????P2P ????????hal_add_vap ??????  */
    /*
    *  |7      4|3      0|
    *  |p2p_mode|vap_mode|
    */

    hal_add_vap(pst_device->pst_device_stru, ((pst_param->en_vap_mode) | ((oal_uint8)(pst_param->en_p2p_mode << 4))),
                uc_vap_idx, &pst_hal_vap);
#else
    /* ??????vap ??????????????hal_vap */
    hal_add_vap(pst_device->pst_device_stru, pst_param->en_vap_mode, uc_vap_idx, &pst_hal_vap);
#endif

    if (OAL_PTR_NULL == pst_hal_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::pst_hal_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap->pst_hal_vap = pst_hal_vap;

    /* ??????dmac vap?????????????????? */
    pst_dmac_vap->pst_hal_device = pst_device->pst_device_stru;

    //pst_dmac_vap->st_vap_base_info.uc_p2p_gocl_hal_vap_id = pst_hal_vap->uc_vap_id;


    /* ????RTS???????? */
    //hal_vap_set_machw_prot_params(pst_dmac_vap->pst_hal_vap, &(pst_dmac_vap->st_tx_alg.st_rate), &(pst_dmac_vap->st_tx_alg.ast_per_rate[0]));


    /* P2P ????MAC ???? */
    hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);


    /* ????????WMM???? */
    dmac_config_set_machw_wmm(pst_dmac_vap->pst_hal_vap, pst_mac_vap);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* device??sta??????1 */
    if (WLAN_VAP_MODE_BSS_STA == pst_param->en_vap_mode)
    {
        /* ??????uc_assoc_vap_id????????????ap?????? */
        mac_vap_set_assoc_id(pst_mac_vap, 0xff);
    }
    mac_vap_set_p2p_mode(&pst_dmac_vap->st_vap_base_info, pst_param->en_p2p_mode);
    mac_inc_p2p_num(&pst_dmac_vap->st_vap_base_info);


#endif
    /* ??????????VAP??????AP??STA??WDS???? */
    dmac_alg_create_vap_notify(pst_dmac_vap);
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_ps_stop_check_and_notify();
#endif
#ifdef _PRE_WLAN_FEATURE_STA_PM
    /* ??????P2P CLIENT ?????????? */
    dmac_pm_sta_attach(pst_dmac_vap);
#endif
    /* ????hmac??dmac????muti user idx???????? */
    if (pst_param->us_muti_user_id != pst_mac_vap->us_multi_user_idx)
    {
        OAM_ERROR_LOG0(pst_mac_cfg_vap->uc_vap_id, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::HMAC and DMAC muti user indx not same!.}");
        return OAL_ERR_CODE_ADD_MULTI_USER_INDX_UNSYNC;
    }
#if 0
    /* ??dmac ??mac_vap ??uc_p2p0_hal_vap_id/uc_p2p_gocl_hal_vap_id ??????hmac ??mac_vap ?????? */
    /* ????????????????????????????hmac */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(mac_add_vap_sync_data_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(0, OAM_SF_P2P, "{dmac_add_p2p_cl_vap::pst_event_memory null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event_up              = (frw_event_stru *)pst_event_mem->puc_data;
    pst_mac_add_vap_sync_data = (mac_add_vap_sync_data_stru *)pst_event_up->auc_event_data;
    //pst_mac_add_vap_sync_data->uc_p2p0_hal_vap_id = pst_dmac_vap->st_vap_base_info.uc_p2p0_hal_vap_id;
    //pst_mac_add_vap_sync_data->uc_p2p_gocl_hal_vap_id = pst_dmac_vap->st_vap_base_info.uc_p2p_gocl_hal_vap_id;
    //dmac_send_sys_event((mac_vap_stru *)&(pst_dmac_vap->st_vap_base_info), WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_add_vap_sync_data_stru), (oal_uint8 *)pst_mac_add_vap_sync_data);
    FRW_EVENT_FREE(pst_event_mem);
#endif
    /*??????p2p????????*/
    OAL_MEMZERO(&(pst_dmac_vap->st_p2p_ops_param), OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));
    OAL_MEMZERO(&(pst_dmac_vap->st_p2p_noa_param), OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_add_p2p_cl_vap::add vap [%d] sucess.}", uc_vap_idx);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_del_p2p_cl_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                  *pst_dmac_vap;
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_mac_device;
    pst_dmac_vap   = (dmac_vap_stru *)pst_mac_vap;
    pst_hal_device = pst_dmac_vap->pst_hal_device;

    pst_mac_device = mac_res_get_dev(pst_hal_device->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_COEX, "{dmac_config_del_vap::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device->st_p2p_info.en_p2p_ps_pause = OAL_FALSE;
    /* ????P2P ?????????? */
    hal_vap_set_noa(pst_dmac_vap->pst_hal_vap, 0, 0, 0, 0);
    hal_vap_set_ops(pst_dmac_vap->pst_hal_vap, 0, 0);
    OAL_MEMZERO(&(pst_dmac_vap->st_p2p_ops_param), OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));
    OAL_MEMZERO(&(pst_dmac_vap->st_p2p_noa_param), OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));

#ifdef _PRE_WLAN_FEATURE_STA_PM
    /* ??????????????*/
    dmac_pm_sta_detach(pst_dmac_vap);
#endif
    /* ??????p2p0,????p2p0_hal_vap */

    hal_del_vap(pst_hal_device, pst_mac_vap->en_vap_mode, pst_dmac_vap->pst_hal_vap->uc_vap_id);

#ifdef _PRE_WLAN_FEATURE_P2P
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_dec_p2p_num(pst_mac_vap);
#endif
#endif

    /* ????p2p cl ??????????dmac vap ??????hal_vap ????????p2p0_hal_vap,????p2p0 ???????? */
    pst_dmac_vap->pst_hal_vap    = pst_dmac_vap->pst_p2p0_hal_vap;
    mac_vap_set_p2p_mode(&pst_dmac_vap->st_vap_base_info, WLAN_P2P_DEV_MODE);
	//pst_dmac_vap->st_vap_base_info.uc_p2p_gocl_hal_vap_id = pst_dmac_vap->st_vap_base_info.uc_p2p0_hal_vap_id;
    oal_memcopy(pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID,
                pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID,
                WLAN_MAC_ADDR_LEN);

    /* ???????????????????? */
    dmac_alg_delete_vap_notify(pst_dmac_vap);
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_ps_stop_check_and_notify();
#endif
    if (WLAN_VAP_MODE_CONFIG == pst_mac_vap->en_vap_mode)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_del_p2p_cl_vap::config vap should not be here.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

#endif


OAL_STATIC oal_uint32  dmac_config_add_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                *pst_device;
    mac_cfg_add_vap_param_stru     *pst_param;
    dmac_vap_stru                  *pst_dmac_vap;
    hal_to_dmac_device_stru        *pst_hal_device;
    hal_to_dmac_vap_stru           *pst_hal_vap = OAL_PTR_NULL;
    oal_uint32                      ul_ret;
    oal_uint8                       uc_vap_idx;
    mac_vap_stru                   *pst_mac_cfg_vap;
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    mac_vap_stru                   *pst_msta;
#endif

    pst_param = (mac_cfg_add_vap_param_stru *)puc_param;

#ifdef _PRE_WLAN_FEATURE_P2P
    if (WLAN_P2P_CL_MODE == pst_param->en_p2p_mode)
    {
        return dmac_add_p2p_cl_vap(pst_mac_vap, uc_len, puc_param);
    }
#endif
    /* ????device */
    pst_mac_cfg_vap = (mac_vap_stru*)mac_res_get_mac_vap(pst_param->uc_cfg_vap_indx);
    if (OAL_PTR_NULL == pst_mac_cfg_vap)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_add_vap::add vap failed. pst_cfg_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_device = mac_res_get_dev(pst_mac_cfg_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device)
    {
        OAM_ERROR_LOG0(pst_mac_cfg_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_vap::add vap failed. pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    uc_vap_idx = pst_param->uc_vap_id;
    /* ????dmac vap???????? */
    ul_ret = mac_res_alloc_dmac_vap(uc_vap_idx);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG0(pst_mac_cfg_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_vap::mac_res_alloc_dmac_vap error.}");
        return ul_ret;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(uc_vap_idx);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_vap::add vap failed. pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if(MAC_VAP_VAILD == pst_dmac_vap->st_vap_base_info.uc_init_flag)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                        "{dmac_config_add_vap::add vap maybe double init!}");
    }
#endif
#ifdef _PRE_WLAN_FEATURE_UAPSD
    if (WLAN_VAP_MODE_BSS_AP == pst_param->en_vap_mode)
    {
        g_uc_uapsd_cap = pst_param->bit_uapsd_enable;
    }
#endif
    ul_ret = dmac_vap_init(pst_dmac_vap,
                           pst_device->uc_chip_id,
                           pst_device->uc_device_id,
                           uc_vap_idx,
                           pst_param);
    if (OAL_SUCC != ul_ret)
    {
        if (OAL_PTR_NULL != pst_dmac_vap->st_vap_base_info.pst_mib_info)
        {
            OAL_MEM_FREE(pst_dmac_vap->st_vap_base_info.pst_mib_info, OAL_TRUE);
            pst_dmac_vap->st_vap_base_info.pst_mib_info = OAL_PTR_NULL;
        }
        /* ?????????? */
        mac_res_free_mac_vap(uc_vap_idx);
        return ul_ret;
    }

    /* ????hal_add_vap */
    pst_hal_device = pst_device->pst_device_stru;

#if defined (_PRE_WLAN_FEATURE_PROXYSTA)
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_psta_init_vap(&pst_dmac_vap->st_vap_base_info, pst_param);
#endif
    dmac_psta_init_vap(pst_dmac_vap);

    /* ??????Proxy STA??????hal ??????Proxy STA??????????????????hal?????? */
    hal_add_vap(pst_hal_device, mac_vap_is_vsta(&pst_dmac_vap->st_vap_base_info) ? WLAN_VAP_MODE_PROXYSTA : pst_param->en_vap_mode, uc_vap_idx, &pst_hal_vap);
#elif defined (_PRE_WLAN_FEATURE_P2P)
    /* ??????vap ??????????????hal_vap */
    /* ????MAC ????????P2P ??????????????????
    *  GO ??????????hal_vap_id = 1,
    *  CL ??????????hal_vap_id = 5,
    *  ??????P2P ????????hal_add_vap ??????  */
    /*
    *  |7      4|3      0|
    *  |p2p_mode|vap_mode|
    */

    /* ??????vap ??????????????hal_vap */
    hal_add_vap(pst_hal_device, (pst_param->en_vap_mode) | (oal_uint8)(pst_param->en_p2p_mode << 4) ,
                uc_vap_idx, &pst_hal_vap);
#else
    /* ??????vap ??????????????hal_vap */
    hal_add_vap(pst_hal_device, pst_param->en_vap_mode, uc_vap_idx, &pst_hal_vap);
#endif

    if (OAL_PTR_NULL == pst_hal_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_event_add_vap::add hal_vap failed. pst_hal_vap null.}");

        if (OAL_PTR_NULL != pst_dmac_vap->st_vap_base_info.pst_mib_info)
        {
            OAL_MEM_FREE(pst_dmac_vap->st_vap_base_info.pst_mib_info, OAL_TRUE);
            pst_dmac_vap->st_vap_base_info.pst_mib_info = OAL_PTR_NULL;
        }

        /* ?????????? */
        mac_res_free_mac_vap(uc_vap_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    /* ??????????P2P CL ??P2P DEV */
    if (WLAN_P2P_DEV_MODE == pst_param->en_p2p_mode)
    {
        pst_dmac_vap->pst_p2p0_hal_vap = pst_hal_vap;
        pst_device->st_p2p_info.uc_p2p0_vap_idx = pst_mac_vap->uc_vap_id;
    }
#endif /* _PRE_WLAN_FEATURE_P2P */
    pst_dmac_vap->pst_hal_vap = pst_hal_vap;

    /* ??????dmac vap?????????????????? */
    pst_dmac_vap->pst_hal_device = pst_hal_device;

    /* ????RTS???????? */
    //hal_vap_set_machw_prot_params(pst_dmac_vap->pst_hal_vap, &(pst_dmac_vap->st_tx_alg.st_rate), &(pst_dmac_vap->st_tx_alg.ast_per_rate[0]));

#if defined (_PRE_WLAN_FEATURE_PROXYSTA)
    if (mac_vap_is_vsta(pst_mac_vap))
    {
        dmac_vap_psta_lut_idx(pst_dmac_vap) = pst_hal_vap->uc_vap_id;
    }
    else
    {
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, mac_mib_get_StationID(pst_mac_vap));
    }
#elif defined (_PRE_WLAN_FEATURE_P2P)
    /* P2P ????MAC ???? */
    if ((WLAN_P2P_DEV_MODE == pst_param->en_p2p_mode) && (WLAN_VAP_MODE_BSS_STA == pst_param->en_vap_mode))
    {
        /* ????p2p0 MAC ???? */
        hal_vap_set_macaddr(pst_dmac_vap->pst_p2p0_hal_vap, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID);
    }
    else
    {
        /* ????????vap ??mac ???? */
        hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    }
#else
    /* ????MAC???? */
    hal_vap_set_macaddr(pst_dmac_vap->pst_hal_vap, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
#endif

    /* ????????WMM???? */
    dmac_config_set_machw_wmm(pst_dmac_vap->pst_hal_vap, pst_mac_vap);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_device_set_vap_id(pst_device, &(pst_dmac_vap->st_vap_base_info),uc_vap_idx, pst_param->en_vap_mode, pst_param->en_p2p_mode, OAL_TRUE);

    /* ??????STA AID ?? 0 */
    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        mac_vap_set_aid(pst_mac_vap, 0);
    }
#endif
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_ps_stop_check_and_notify();
#endif
    /* ??????linkloss???????? */
    dmac_vap_linkloss_init(pst_dmac_vap);

    /* ??????????VAP??????AP??STA??WDS???? */
    switch(pst_param->en_vap_mode)
    {
        case WLAN_VAP_MODE_BSS_AP:
            /* ??????????????vap?????? */
            /* ????????????VAP?????????? */
            dmac_alg_create_vap_notify(pst_dmac_vap);
        #ifdef _PRE_WLAN_FEATURE_UAPSD
            pst_dmac_vap->uc_uapsd_max_depth = DMAC_UAPSD_QDEPTH_DEFAULT;
            #if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
                mac_vap_set_uapsd_en(&pst_dmac_vap->st_vap_base_info, OAL_TRUE);
            #endif
        #endif

        #ifdef  _PRE_WLAN_FEATURE_PM
            if(OAL_TRUE == pst_device->en_pm_enable)
            {
                dmac_pm_ap_attach(pst_dmac_vap);
            }
        #endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

            /* add vap??,ap??????????,???????????? */
            hal_pm_wlan_servid_register(pst_dmac_vap->pst_hal_vap, &ul_ret);

            if (OAL_SUCC != ul_ret)
            {
                OAM_ERROR_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,
                                   "{dmac_pm_sta_attach:hal_pm_wlan_servid_register hal vap:[%d]fail",pst_dmac_vap->pst_hal_vap->uc_vap_id);

            }

            dmac_pm_enable_front_end(pst_device,OAL_TRUE);

            /* ap?????? */
            pst_hal_device->uc_full_phy_freq_user_cnt++;

            OAM_WARNING_LOG1(0,OAM_SF_PWR,"dmac_config_add_vap ap::full phy freq user[%d]",pst_hal_device->uc_full_phy_freq_user_cnt);

            hal_process_phy_freq(pst_device->pst_device_stru);

#endif
            break;

        case WLAN_VAP_MODE_BSS_STA:
            /* ??????????????vap?????? */
            /* ????????????VAP?????????? */
            if(IS_LEGACY_VAP(&pst_dmac_vap->st_vap_base_info))
            {
                dmac_alg_create_vap_notify(pst_dmac_vap);
#ifdef  _PRE_WLAN_FEATURE_STA_PM    //??????????????????ap(liuzhengqi)
                /* P2P DEVICE ?? P2P client ????vap ??????????P2P DEVICE ???????????????? */
                dmac_pm_sta_attach(pst_dmac_vap);
#endif
            }
            break;

        case WLAN_VAP_MODE_WDS:

            break;

        default:

            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "{dmac_vap_init::invalid vap mode[%d].", pst_param->en_vap_mode);
            return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* ????????dmac???????? */
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (mac_vap_is_vsta(&pst_dmac_vap->st_vap_base_info))
    {
        pst_msta = mac_find_main_proxysta(pst_device);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_msta))
        {
            /* ????proxysta????msta??????????msta??????????????vsta????????????????????vsta?????????????? */
            OAM_ERROR_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PROXYSTA, "{dmac_config_add_vap::msta is null, vsta cannot create.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        else
        {
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PROXYSTA, "{dmac_config_add_vap::vsta multi user id is[%d], the same with msta.}",  pst_msta->us_multi_user_idx);
            mac_vap_set_multi_user_idx(pst_mac_vap, pst_msta->us_multi_user_idx);
        }
    }
    else
#endif
    {
        dmac_user_add_multi_user(pst_mac_vap, pst_param->us_muti_user_id);
        mac_vap_set_multi_user_idx(pst_mac_vap, pst_param->us_muti_user_id);
        /* ????hmac??dmac????muti user idx???????? */
        if (pst_param->us_muti_user_id != pst_mac_vap->us_multi_user_idx)
        {
            OAM_ERROR_LOG0(pst_mac_cfg_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_vap::HMAC and DMAC muti user indx not same!.}");
            return OAL_ERR_CODE_ADD_MULTI_USER_INDX_UNSYNC;
        }
    }

#if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP)

    pst_dmac_vap->ul_user_pmf_status = 0;
#endif /* #if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP) */
#if 0
#ifdef _PRE_WLAN_FEATURE_P2P
    /* ??????????P2P_DEV,????????device ??mac_device ????????p2p0_vap_id ?????????? */
    if (WLAN_P2P_DEV_MODE == en_p2p_mode)
    {
		/* ??????p2p0 ????????????????p2p0_vap ??vap_id ??????device?????????? */
        pst_device->st_p2p_info.uc_p2p0_vap_idx = pst_dmac_vap->st_vap_base_info.uc_vap_id;
    }

    /* ??dmac ??mac_vap ??uc_p2p0_hal_vap_id/uc_p2p_gocl_hal_vap_id ??????hmac ??mac_vap ?????? */
    /* ????????????????????????????hmac */
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(mac_add_vap_sync_data_stru));
    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_query_event_response::pst_event_memory null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event_up              = (frw_event_stru *)pst_event_mem->puc_data;
    pst_mac_add_vap_sync_data = (mac_add_vap_sync_data_stru *)pst_event_up->auc_event_data;
    pst_mac_add_vap_sync_data->uc_p2p0_hal_vap_id = pst_dmac_vap->st_vap_base_info.uc_p2p0_hal_vap_id;
    pst_mac_add_vap_sync_data->uc_p2p_gocl_hal_vap_id = pst_dmac_vap->st_vap_base_info.uc_p2p_gocl_hal_vap_id;
    dmac_send_sys_event((mac_vap_stru *)&(pst_dmac_vap->st_vap_base_info), WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_add_vap_sync_data_stru), (oal_uint8 *)pst_mac_add_vap_sync_data);
    FRW_EVENT_FREE(pst_event_mem);

    /*GO ??????p2p????????*/
    if (WLAN_P2P_GO_MODE == en_p2p_mode)
    {
        OAL_MEMZERO(&(pst_dmac_vap->st_p2p_ops_param), OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));
        OAL_MEMZERO(&(pst_dmac_vap->st_p2p_noa_param), OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));
    }

#endif
#endif

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_add_vap::add vap succ. mode[%d] state[%d].}",
                             pst_mac_vap->en_vap_mode, pst_mac_vap->en_vap_state);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_PROXYSTA

oal_uint32 dmac_config_proxysta_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32       ul_value;
    mac_device_stru *pst_mac_device;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_proxysta_switch:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_proxysta_switch::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_value = *(oal_uint32 *)puc_param;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    dmac_custom_init(ul_value);
    mac_is_proxysta_enabled(pst_mac_device) = ul_value ? OAL_TRUE : OAL_FALSE;
#endif

    hal_set_proxysta_enable(pst_mac_device->pst_device_stru, (oal_int32)ul_value);

    return  OAL_SUCC;
}
#endif


#if 0

OAL_STATIC oal_uint32  dmac_config_tdls_prohibited(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_uint8         uc_value;

    uc_value = *((oal_uint8 *)puc_param);

    /* ????tdls prohibited,1??????????,0?????????? */
    mac_vap_set_tdls_prohibited(pst_mac_vap, uc_value);
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_tdls_channel_switch_prohibited(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_uint8         uc_value;

    uc_value = *((oal_uint8 *)puc_param);

    mac_vap_set_tdls_channel_switch_prohibited(pst_mac_vap, uc_value);

    //OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_tdls_channel_switch_prohibited succ!");
#endif
    return OAL_SUCC;
}
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)


OAL_STATIC oal_uint32 dmac_config_offload_start_vap(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_param)
{
    mac_cfg_start_vap_param_stru *pst_start_vap_param;

    pst_start_vap_param = (mac_cfg_start_vap_param_stru *)puc_param;

    pst_mac_vap->st_channel.en_band = pst_start_vap_param->uc_band;
    pst_mac_vap->st_channel.en_bandwidth = pst_start_vap_param->uc_bandwidth;
    mac_vap_init_by_protocol(pst_mac_vap, pst_start_vap_param->uc_protocol);

    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        /* ????bssid */
        mac_vap_set_bssid(pst_mac_vap, pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);

        /*??vap????????????????*/
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_UP);

    }
    else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {

#ifdef _PRE_WLAN_FEATURE_P2P
        /* P2P0 ??P2P-P2P0 ??VAP ??????P2P CL UP????????????????vap ???? */
        if (WLAN_P2P_CL_MODE == pst_start_vap_param->en_p2p_mode)
        {
            pst_mac_vap->st_channel.uc_chan_number = 0;
        }
        else
#endif
        {
        #ifdef _PRE_WLAN_FEATURE_ROAM
            if (MAC_VAP_STATE_ROAMING != pst_mac_vap->en_vap_state)
        #endif //_PRE_WLAN_FEATURE_ROAM
            {
                pst_mac_vap->st_channel.uc_chan_number = 0;
                mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_STA_FAKE_UP);
            }
        }
    }

    /* ???????????? */
    mac_vap_init_rates(pst_mac_vap);
    return OAL_SUCC;
}

#endif


OAL_STATIC oal_uint32  dmac_config_del_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                  *pst_dmac_vap = (dmac_vap_stru *)pst_mac_vap;
    hal_to_dmac_device_stru        *pst_hal_device = pst_dmac_vap->pst_hal_device;
    oal_uint8                       uc_vap_id = pst_dmac_vap->st_vap_base_info.uc_vap_id;
#ifdef _PRE_WLAN_FEATURE_P2P
    mac_cfg_del_vap_param_stru     *pst_del_vap_param;
    wlan_p2p_mode_enum_uint8        en_p2p_mode = WLAN_LEGACY_VAP_MODE;
    mac_device_stru                *pst_mac_device;
#endif

    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_del_vap::del vap. mode[%d] state[%d] p2p_mode[%d].}",
                      pst_mac_vap->en_vap_mode, pst_mac_vap->en_vap_state, pst_mac_vap->en_p2p_mode);

#if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP)
    pst_dmac_vap->ul_user_pmf_status = 0;
#endif /* #if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP) */
    // ????CSA??????????
    if (g_csa_stop_timer.en_is_registerd) {
        FRW_TIMER_DESTROY_TIMER(&g_csa_stop_timer);
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    pst_del_vap_param = (mac_cfg_del_vap_param_stru *)puc_param;
    en_p2p_mode       = pst_del_vap_param->en_p2p_mode;
    if (WLAN_P2P_CL_MODE == en_p2p_mode) {
        return dmac_del_p2p_cl_vap(pst_mac_vap, uc_len, puc_param);
    }
    if (WLAN_P2P_GO_MODE == en_p2p_mode) {
        pst_mac_device = mac_res_get_dev(pst_hal_device->uc_device_id);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
        {
            OAM_ERROR_LOG0(uc_vap_id, OAM_SF_COEX, "{dmac_config_del_vap::pst_mac_device null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        pst_mac_device->st_p2p_info.en_p2p_ps_pause = OAL_FALSE;
        /* ????P2P ?????????? */
        hal_vap_set_noa(pst_dmac_vap->pst_hal_vap, 0, 0, 0, 0);
        hal_vap_set_ops(pst_dmac_vap->pst_hal_vap, 0, 0);
        OAL_MEMZERO(&(pst_dmac_vap->st_p2p_ops_param), OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));
        OAL_MEMZERO(&(pst_dmac_vap->st_p2p_noa_param), OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));
    }
    /* ??????p2p0,????p2p0_hal_vap */
    if (WLAN_P2P_DEV_MODE == en_p2p_mode)
    {
        hal_del_vap(pst_hal_device, pst_mac_vap->en_vap_mode, pst_dmac_vap->pst_p2p0_hal_vap->uc_vap_id);
        pst_dmac_vap->pst_p2p0_hal_vap    = OAL_PTR_NULL;
    }
    else
#endif
    {
        /* STA??????????????*/
#ifdef _PRE_WLAN_FEATURE_STA_PM
        if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode) {
            dmac_pm_sta_detach(pst_dmac_vap);
        }
#endif
        if (WLAN_VAP_MODE_BSS_AP == pst_dmac_vap->st_vap_base_info.en_vap_mode)
        {
            /* ??????????????*/
#ifdef _PRE_WLAN_FEATURE_PM
            dmac_pm_ap_deattach(pst_dmac_vap);
#endif

            hal_pm_wlan_servid_unregister(pst_dmac_vap->pst_hal_vap);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
            if (pst_hal_device->uc_full_phy_freq_user_cnt > 0)
            {
                pst_hal_device->uc_full_phy_freq_user_cnt--;
            }
            else
            {
                OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,"dmac_config_del_vap::full_phy_freq_user_cnt is 0");
            }

            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_del_vap ap::full phy freq user cnt[%d]",pst_hal_device->uc_full_phy_freq_user_cnt);
#endif
        }


        /* ????????hal_del_vap */
        hal_del_vap(pst_hal_device, pst_mac_vap->en_vap_mode, pst_dmac_vap->pst_hal_vap->uc_vap_id);
		/* ???????????????????? */
        dmac_alg_delete_vap_notify(pst_dmac_vap);
    }
    pst_dmac_vap->pst_hal_device = OAL_PTR_NULL;
    pst_dmac_vap->pst_hal_vap    = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_VOWIFI
    dmac_vap_vowifi_exit(pst_dmac_vap);
#endif

    if (WLAN_VAP_MODE_CONFIG != pst_mac_vap->en_vap_mode)
    {
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
        if(!mac_vap_is_vsta(&pst_dmac_vap->st_vap_base_info))
#endif
        {
            dmac_user_del_multi_user(pst_mac_vap, pst_mac_vap->us_multi_user_idx);
        }
    }
    else
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_del_vap::config vap should not be here.}");
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
    if (OAL_PTR_NULL != pst_dmac_vap->pst_ip_addr_info)
    {
        OAL_MEM_FREE(pst_dmac_vap->pst_ip_addr_info, OAL_TRUE);
        pst_dmac_vap->pst_ip_addr_info = OAL_PTR_NULL;
    }
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_vap_exit(pst_mac_vap);
#endif

    /* ????tim_bitmap */
    if (OAL_PTR_NULL != pst_dmac_vap->puc_tim_bitmap)
    {
        OAL_MEM_FREE(pst_dmac_vap->puc_tim_bitmap, OAL_TRUE);
        pst_dmac_vap->puc_tim_bitmap = OAL_PTR_NULL;
    }
#ifdef _PRE_WLAN_FEATURE_11K
    /* ????????????????????rrm info */
    if (OAL_PTR_NULL != pst_dmac_vap->pst_rrm_info)
    {
        if (pst_dmac_vap->pst_rrm_info->st_offset_timer.en_is_registerd)
        {
            FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_dmac_vap->pst_rrm_info->st_offset_timer);
        }
        if (pst_dmac_vap->pst_rrm_info->st_quiet_timer.en_is_registerd)
        {
            FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_dmac_vap->pst_rrm_info->st_quiet_timer);
        }
        OAL_MEM_FREE(pst_dmac_vap->pst_rrm_info, OAL_TRUE);
        pst_dmac_vap->pst_rrm_info = OAL_PTR_NULL;
    }
#endif //_PRE_WLAN_FEATURE_11K
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_ps_stop_check_and_notify();
    /* ?????????????? */
    dmac_btcoex_wlan_priority_set(pst_mac_vap, 0, 0);
    hal_set_btcoex_occupied_period(0);
#endif
#ifdef _PRE_WLAN_FEATURE_SMARTANT
    dmac_config_dual_antenna_vap_check(pst_mac_vap);
#endif

    if (OAL_PTR_NULL != pst_dmac_vap)
    {
        mac_res_free_mac_vap(uc_vap_id);
        OAM_WARNING_LOG2(uc_vap_id, OAM_SF_CFG, "{dmac_config_del_vap:: del vap[%d] success, multe user idx[%d].}", uc_vap_id, pst_mac_vap->us_multi_user_idx);
    }
    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_start_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru               *pst_dmac_vap;
    oal_uint32                    ul_ret;
    wlan_protocol_enum_uint8      en_protocol;
    hal_to_dmac_device_stru      *pst_hal_device;
    mac_cfg_start_vap_param_stru *pst_start_vap_param;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8                     uc_hipriv_ack = OAL_FALSE;
#endif
#ifdef _PRE_PRODUCT_ID_HI110X_DEV
    oal_int8                      ac_ssid[WLAN_SSID_MAX_LEN] = {'\0'};
#endif


    pst_start_vap_param = (mac_cfg_start_vap_param_stru *)puc_param;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_start_vap::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    ul_ret = dmac_config_offload_start_vap(pst_mac_vap, puc_param);
    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_start_vap::dmac_config_offload_start_vap failed[%d].}", ul_ret);
        return ul_ret;
    }
#endif
    en_protocol    = pst_mac_vap->en_protocol;
    pst_hal_device = pst_dmac_vap->pst_hal_device;

    /* ????MAC EIFS_TIME ?????? */
    hal_config_eifs_time(pst_hal_device, en_protocol);

#ifdef _PRE_WLAN_FEATURE_P2P
    /* P2P0 ??P2P-P2P0 ??VAP ??????P2P CL UP????????????????vap ???? */
    if (WLAN_P2P_DEV_MODE == pst_start_vap_param->en_p2p_mode)
    {
        /* ????PA_CONTROL??vap_control??: bit0~3????AP0~3 bit4????sta */
        hal_vap_set_opmode(pst_dmac_vap->pst_p2p0_hal_vap, pst_dmac_vap->st_vap_base_info.en_vap_mode);
    }
    else
#endif
    {
        /* ????PA_CONTROL??vap_control??: bit0~3????AP0~3 bit4????sta */
        hal_vap_set_opmode(pst_dmac_vap->pst_hal_vap, pst_dmac_vap->st_vap_base_info.en_vap_mode);
    }

    /* ?????????????????????????????????????? */
    dmac_vap_init_tx_frame_params(pst_dmac_vap, pst_start_vap_param->en_mgmt_rate_init_flag);

#ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
    /* ?????????????????????????????????????????? */
    pst_dmac_vap->un_mode_valid.uc_mode_param_valid = 0;
    OAL_MEMZERO(&pst_dmac_vap->st_tx_alg_vht, OAL_SIZEOF(hal_tx_txop_alg_stru));
    OAL_MEMZERO(&pst_dmac_vap->st_tx_alg_ht, OAL_SIZEOF(hal_tx_txop_alg_stru));
    OAL_MEMZERO(&pst_dmac_vap->st_tx_alg_11ag, OAL_SIZEOF(hal_tx_txop_alg_stru));
    OAL_MEMZERO(&pst_dmac_vap->st_tx_alg_11b, OAL_SIZEOF(hal_tx_txop_alg_stru));
#endif

    /* ?????????????????????????? */
    dmac_vap_init_tx_ucast_data_frame(pst_dmac_vap);

    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        /* ????beacon?? */

        /* ???????????????????????????????? */
        if (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band)
        {
            mac_mib_set_ShortPreambleOptionImplemented(pst_mac_vap, WLAN_LEGACY_11B_MIB_SHORT_PREAMBLE);
            mac_mib_set_SpectrumManagementRequired(pst_mac_vap, OAL_FALSE);
        }
        else
        {
            mac_mib_set_ShortPreambleOptionImplemented(pst_mac_vap, WLAN_LEGACY_11B_MIB_LONG_PREAMBLE);
            mac_mib_set_SpectrumManagementRequired(pst_mac_vap, OAL_TRUE);
        }

#ifdef _PRE_WLAN_HW_TEST
        /*1151??????????????*/
        if(HAL_ALWAYS_RX_RESERVED != pst_dmac_vap->pst_hal_device->bit_al_rx_flag)
#endif
        {
            ul_ret = dmac_beacon_alloc(pst_dmac_vap);
            if (OAL_SUCC != ul_ret)
            {
                OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_start_vap::dmac_beacon_alloc failed[%d].}", ul_ret);
                return ul_ret;
            }
        }
        dmac_alg_vap_up_notify(pst_mac_vap);
    }
    else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
    #ifdef _PRE_WLAN_FEATURE_TXOPPS
        /* ????sta??????????txop ps??????????mac txop ps?????????? */
        dmac_txopps_init_machw_sta(pst_dmac_vap);
    #endif
    }
    else
    {

    }

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_vap_up_handle(pst_mac_vap);
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
    {
    mac_device_stru              *pst_mac_device;

    /* ????p2p ci ??????P2P start vap??????????RF?????? */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{dmac_config_start_vap::start vap FAIL.get mac_device is null.device_id[%d]",
                         pst_mac_vap->uc_device_id);
        return OAL_FAIL;
    }

    dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);
    }
#endif

    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                     "{dmac_config_start_vap::start vap success.vap_mode[%d] state[%d], p2p mode[%d]",
                     pst_mac_vap->en_vap_mode,
                     pst_mac_vap->en_vap_state,
                     pst_mac_vap->en_p2p_mode);

    /* ????????????Success */
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

#ifdef _PRE_PRODUCT_ID_HI110X_DEV
    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        ul_ret = dmac_scan_send_probe_req_frame(pst_dmac_vap, BROADCAST_MACADDR, ac_ssid);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{dmac_config_start_vap::dmac_mgmt_send_probe_request failed[%d].}", ul_ret);
        }
    }
#endif /* _PRE_PRODUCT_ID_HI110X_DEV */


    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_down_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru   *pst_mac_device;
    dmac_user_stru     *pst_dmac_user;
    dmac_vap_stru     *pst_dmac_vap;
#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    oal_uint8           uc_hipriv_ack = OAL_FALSE;
#endif

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_down_vap::pst_mac_device[%d] null.}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap  = (dmac_vap_stru *)pst_mac_vap;
    pst_dmac_user = (dmac_user_stru *)mac_res_get_mac_user(pst_mac_vap->us_multi_user_idx);
    if (OAL_PTR_NULL == pst_dmac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_down_vap::pst_multi_dmac_user[%d] null.}", pst_mac_vap->us_multi_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (!mac_vap_is_vsta(pst_mac_vap))
#endif
    {
        /* ????????????tid???????????????? */
        dmac_tid_clear(&pst_dmac_user->st_user_base_info, pst_mac_device);


        /* ????PA_CONTROL??vap_control??: bit0~3????AP0~3 bit4????sta */
        hal_vap_clr_opmode(pst_dmac_vap->pst_hal_vap, pst_dmac_vap->st_vap_base_info.en_vap_mode);
    }

    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        /* ????????????????????????????????????????mpdu */
        dmac_psm_clear_ps_queue(pst_dmac_user);

        /* ????beacon?? */
        dmac_beacon_free(pst_dmac_vap);
#ifdef _PRE_WLAN_FEATURE_DBAC
        dmac_alg_vap_down_notify(pst_mac_vap);
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
        hal_enable_radar_det(pst_dmac_vap->pst_hal_device, 0);
#endif
    }
    else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        /* ????sta tsf?????? */
        hal_disable_sta_tsf_tbtt(pst_dmac_vap->pst_hal_vap);
    }
    else
    {

    }

    /* ???????????? */
    dmac_mgmt_scan_vap_down(pst_mac_vap);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

#ifdef _PRE_WLAN_FEATURE_P2P
    if (WLAN_P2P_CL_MODE == mac_get_p2p_mode(pst_mac_vap))
    {
    	mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
    }
    else
#endif
    {
    	mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_INIT);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_btcoex_vap_down_handle(pst_mac_vap);
#endif


    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_down_vap::down vap succ.vap mode[%d] state[%d].}",
                      pst_mac_vap->en_vap_mode, pst_mac_vap->en_vap_state);

#ifdef _PRE_WLAN_FEATURE_EQUIPMENT_TEST
    uc_hipriv_ack = OAL_TRUE;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_CHIP_CHECK_SWITCH, OAL_SIZEOF(oal_uint8), &uc_hipriv_ack);
#endif

    return OAL_SUCC;
}


oal_void  dmac_config_set_wmm_open_cfg(hal_to_dmac_vap_stru *pst_hal_vap, mac_wme_param_stru  *pst_wmm)
{
    wlan_wme_ac_type_enum_uint8     en_ac_type;

    if(OAL_PTR_NULL == pst_hal_vap || OAL_PTR_NULL == pst_wmm)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_wmm_open_cfg::param null.}");

        return;
    }
    for (en_ac_type = 0; en_ac_type < WLAN_WME_AC_BUTT; en_ac_type++)
    {
        hal_vap_set_edca_machw_cw(pst_hal_vap,
                                 (oal_uint8)(pst_wmm[en_ac_type].ul_logcwmax),
                                 (oal_uint8)(pst_wmm[en_ac_type].ul_logcwmin),
                                  en_ac_type);

        hal_vap_set_machw_aifsn_ac(pst_hal_vap, en_ac_type, (oal_uint8)pst_wmm[en_ac_type].ul_aifsn);
    }

#if 0
    hal_vap_set_machw_cw_bk(pst_hal_vap, (oal_uint8)(pst_wmm[WLAN_WME_AC_BK].ul_logcwmax), (oal_uint8)(pst_wmm[WLAN_WME_AC_BK].ul_logcwmin));
    hal_vap_set_machw_cw_be(pst_hal_vap, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmax, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmin);
    hal_vap_set_machw_cw_vi(pst_hal_vap, (oal_uint8)pst_wmm[WLAN_WME_AC_VI].ul_logcwmax, (oal_uint8)pst_wmm[WLAN_WME_AC_VI].ul_logcwmin);
    hal_vap_set_machw_cw_vo(pst_hal_vap, (oal_uint8)pst_wmm[WLAN_WME_AC_VO].ul_logcwmax, (oal_uint8)pst_wmm[WLAN_WME_AC_VO].ul_logcwmin);

    hal_vap_set_machw_aifsn_ac(pst_hal_vap, WLAN_WME_AC_BK, (oal_uint8)pst_wmm[WLAN_WME_AC_BK].ul_aifsn);
    hal_vap_set_machw_aifsn_ac(pst_hal_vap, WLAN_WME_AC_BE, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_aifsn);
    hal_vap_set_machw_aifsn_ac(pst_hal_vap, WLAN_WME_AC_VI, (oal_uint8)pst_wmm[WLAN_WME_AC_VI].ul_aifsn);
    hal_vap_set_machw_aifsn_ac(pst_hal_vap, WLAN_WME_AC_VO, (oal_uint8)pst_wmm[WLAN_WME_AC_VO].ul_aifsn);
#endif

    hal_vap_set_machw_txop_limit_bkbe(pst_hal_vap, (oal_uint16)pst_wmm[WLAN_WME_AC_BE].ul_txop_limit, (oal_uint16)pst_wmm[WLAN_WME_AC_BK].ul_txop_limit);
    hal_vap_set_machw_txop_limit_vivo(pst_hal_vap, (oal_uint16)pst_wmm[WLAN_WME_AC_VO].ul_txop_limit, (oal_uint16)pst_wmm[WLAN_WME_AC_VI].ul_txop_limit);

}


oal_void  dmac_config_set_wmm_close_cfg(hal_to_dmac_vap_stru *pst_hal_vap, mac_wme_param_stru  *pst_wmm)
{
    if(OAL_PTR_NULL == pst_hal_vap || OAL_PTR_NULL == pst_wmm)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_wmm_close_cfg::param null.}");

        return;
    }
   // hal_vap_set_machw_cw_vo(pst_hal_vap, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmax, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmin);
    hal_vap_set_edca_machw_cw(pst_hal_vap, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmax, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_logcwmin, WLAN_WME_AC_VO);
    hal_vap_set_machw_aifsn_ac(pst_hal_vap, WLAN_WME_AC_VO, (oal_uint8)pst_wmm[WLAN_WME_AC_BE].ul_aifsn);
    hal_vap_set_machw_txop_limit_vivo(pst_hal_vap, (oal_uint16)pst_wmm[WLAN_WME_AC_BE].ul_txop_limit, (oal_uint16)pst_wmm[WLAN_WME_AC_VI].ul_txop_limit);
}


oal_void dmac_config_set_wmm_register(mac_vap_stru *pst_mac_vap,  oal_bool_enum_uint8 en_wmm)
{
    mac_device_stru         *pst_mac_device;
    dmac_vap_stru           *pst_dmac_vap;
    mac_wme_param_stru      *pst_wmm;
    oal_uint                 ul_irq_flag;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device || OAL_PTR_NULL == pst_mac_device->pst_device_stru)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_device_id, OAM_SF_CFG, "{dmac_config_set_wmm_register::mac_res_get_dev fail or pst_mac_device->pst_device_stru NULL,device_id:%u.}",
                         pst_mac_vap->uc_device_id);
        return;
    }
    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_wmm_register::mac_res_get_dmac_vap fail or pst_dmac_vap->pst_hal_vap NULL,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return;
    }

    pst_wmm = mac_get_wmm_cfg(pst_mac_vap->en_vap_mode);

    /* ?????????????????????????????? */
    oal_irq_save(&ul_irq_flag, OAL_5115IRQ_DCSWR);

    /* ???????????? */
    /* hal_set_machw_tx_suspend(pst_hal_device); */

    /* ?????????? */
    /* hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf); */

     /* ????????abort */
    /* hal_set_tx_abort_en(pst_hal_device, 1); */
    /* ????wmm */
    if(!en_wmm)
    {
        /* ???????????? */
        dmac_flush_txq_to_tid_of_vo(pst_mac_device->pst_device_stru);
        /* ????WMM */
        hal_disable_machw_edca(pst_mac_device->pst_device_stru);
        /* ????????WMM???? */
        dmac_config_set_wmm_close_cfg(pst_dmac_vap->pst_hal_vap, pst_wmm);

        /* ??WMM??mib??????????Qos????0 */
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QosOptionImplemented = OAL_FALSE;
    }
    else
    {
        /* ????WMM */
        hal_enable_machw_edca(pst_mac_device->pst_device_stru);
        /* ????????WMM???? */
        dmac_config_set_wmm_open_cfg(pst_dmac_vap->pst_hal_vap, pst_wmm);

        /* ??WMM??mib??????????Qos????1 */
        pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QosOptionImplemented = OAL_TRUE;
    }

    /* ????abort */
    /* hal_set_tx_abort_en(pst_hal_device, 0); */

    /* ???????????????? */
    /* hal_set_machw_tx_resume(pst_hal_device); */

    /* ?????????????? */
    /* hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf_passed); */

    /* ?????? */
    oal_irq_restore(&ul_irq_flag, OAL_5115IRQ_DCSWR);
}


OAL_STATIC oal_uint32  dmac_config_wmm_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru         *pst_mac_device;
    oal_bool_enum_uint8            en_wmm = OAL_FALSE;
    en_wmm = *(oal_bool_enum_uint8 *)puc_param;
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if(OAL_PTR_NULL == pst_mac_device)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ?????????????????????????????????? */
    if(en_wmm == pst_mac_device->en_wmm)
    {
        return OAL_SUCC;
    }
    /* ????dev????wmm_en??????????????4???? */
    pst_mac_device->en_wmm = en_wmm;

    dmac_config_set_wmm_register(pst_mac_vap, en_wmm);
    return OAL_SUCC;
}



OAL_STATIC oal_uint32  dmac_config_set_rts_param(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru            *pst_device           = OAL_PTR_NULL;
    mac_cfg_rts_tx_param_stru  *pst_mac_rts_tx_param = OAL_PTR_NULL;
    hal_cfg_rts_tx_param_stru   st_hal_rts_tx_param;

    pst_mac_rts_tx_param = (mac_cfg_rts_tx_param_stru *)puc_param;

    /*mac??????????hal????*/
    oal_memcopy(&st_hal_rts_tx_param, pst_mac_rts_tx_param, OAL_SIZEOF(st_hal_rts_tx_param));

    pst_device= mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_rts_param::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    hal_set_rts_rate_params(pst_device->pst_device_stru, &st_hal_rts_tx_param);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_update_protection_tx_param(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru                 *pst_mac_device;
    dmac_vap_stru                   *pst_dmac_vap;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if(OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_update_protection_tx_param::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (0 == pst_mac_device->uc_vap_num)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_update_protection_tx_param::can't find existed vap.}");
        return OAL_FAIL;
    }

    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_update_protection_tx_param::pst_dmac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    dmac_tx_update_protection_all_txop_alg(pst_dmac_vap);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  dmac_config_set_protection(mac_vap_stru *pst_mac_vap,
            oal_uint8 uc_len, oal_uint8 *puc_param)
{
    wlan_mib_Dot11OperationEntry_stru *pst_mib;
    mac_h2d_protection_stru     *pst_h2d_prot;
    dmac_user_stru              *pst_dmac_user;
    oal_uint32                  ul_ret;

    DMAC_CHECK_NULL(puc_param);

    pst_h2d_prot = (mac_h2d_protection_stru *)puc_param;

    //mac_dump_protection(pst_mac_vap, puc_param);

    if (pst_h2d_prot->ul_sync_mask & H2D_SYNC_MASK_BARK_PREAMBLE)
    {
        pst_dmac_user = mac_res_get_dmac_user(pst_h2d_prot->us_user_idx);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_user))
        {
            OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_protection:: user is null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        mac_user_set_barker_preamble_mode(&pst_dmac_user->st_user_base_info, pst_h2d_prot->st_user_cap_info.bit_barker_preamble_mode);

    }

    if (pst_h2d_prot->ul_sync_mask & H2D_SYNC_MASK_MIB)
    {
        pst_mib = &pst_mac_vap->pst_mib_info->st_wlan_mib_operation;
        pst_mib->en_dot11HTProtection = pst_h2d_prot->en_dot11HTProtection;
        pst_mib->en_dot11RIFSMode = pst_h2d_prot->en_dot11RIFSMode;
        pst_mib->en_dot11LSIGTXOPFullProtectionActivated =
            pst_h2d_prot->en_dot11LSIGTXOPFullProtectionActivated;
        pst_mib->en_dot11NonGFEntitiesPresent = pst_h2d_prot->en_dot11NonGFEntitiesPresent;
    }

    if (pst_h2d_prot->ul_sync_mask & H2D_SYNC_MASK_PROT)
    {
        oal_memcopy((oal_uint8*)&pst_mac_vap->st_protection, (oal_uint8*)&pst_h2d_prot->st_protection,
                    OAL_SIZEOF(mac_protection_stru));
    }

    ul_ret = dmac_config_update_protection_tx_param(pst_mac_vap, uc_len, puc_param);

    return ul_ret;
}


OAL_STATIC oal_uint32  dmac_config_get_version(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru             *pst_device;
    oal_uint32                   ul_hw_version = 0;
    oal_uint32                   ul_hw_version_data = 0;
    oal_uint32                   ul_hw_version_num = 0;
    oal_uint8                    auc_sw_version[] = "Hi1151 V100R001C02B201_20160607";
    oal_int8                     ac_tmp_buff[200];

    dmac_atcmdsrv_atcmd_response_event    st_atcmdsrv_dbb_num_info;


    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);

    if (OAL_PTR_NULL == pst_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_get_version::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????FPGA???? */
    hal_get_hw_version(pst_device->pst_device_stru, &ul_hw_version, &ul_hw_version_data, &ul_hw_version_num);

    OAL_SPRINTF((char*)ac_tmp_buff, OAL_SIZEOF(ac_tmp_buff), "Software Version: %s. \nFPGA Version: %04x-%04x-%02x.\n", auc_sw_version, ul_hw_version, ul_hw_version_data, ul_hw_version_num);
    oam_print(ac_tmp_buff);

    st_atcmdsrv_dbb_num_info.uc_event_id = OAL_ATCMDSRV_DBB_NUM_INFO_EVENT;
    st_atcmdsrv_dbb_num_info.ul_event_para = ul_hw_version;
    dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_GET_VERSION, OAL_SIZEOF(dmac_atcmdsrv_atcmd_response_event), (oal_uint8 *)&st_atcmdsrv_dbb_num_info);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32   dmac_config_rx_fcs_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    hal_to_dmac_device_stru        *pst_hal_device;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    dmac_atcmdsrv_atcmd_response_event   st_atcmdsrv_rx_pkcg;
#endif
    hal_get_hal_to_dmac_device(pst_mac_vap->uc_chip_id, pst_mac_vap->uc_device_id, &pst_hal_device);
    OAM_WARNING_LOG2(0, OAM_SF_CFG, "dmac_config_rx_fcs_info:packets info, succ[0x%x], fail[0x%x]\n", pst_hal_device->ul_rx_normal_mdpu_succ_num, pst_hal_device->ul_rx_ppdu_fail_num);

    /*??????????????????????????????????host??*/
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        st_atcmdsrv_rx_pkcg.uc_event_id = OAL_ATCMDSRV_GET_RX_PKCG;
        st_atcmdsrv_rx_pkcg.ul_event_para = pst_hal_device->ul_rx_normal_mdpu_succ_num;
        st_atcmdsrv_rx_pkcg.s_always_rx_rssi = pst_hal_device->s_always_rx_rssi;
        dmac_send_sys_event(pst_mac_vap, WLAN_CFGID_RX_FCS_INFO, OAL_SIZEOF(dmac_atcmdsrv_atcmd_response_event), (oal_uint8 *)&st_atcmdsrv_rx_pkcg);
#endif

    /* ???????? */
    pst_hal_device->ul_rx_normal_mdpu_succ_num = 0;
    pst_hal_device->ul_rx_ppdu_fail_num = 0;


    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SMPS

oal_uint32  dmac_config_set_smps_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_smps_mode_stru                 *pst_smps_mode;
    mac_device_stru                        *pst_mac_device;
    mac_vap_stru                           *pst_mac_vap_tmp;
    wlan_mib_mimo_power_save_enum_uint8     en_mib_smps_mode;
    oal_uint8                               uc_vap_idx;

    pst_smps_mode           = (mac_cfg_smps_mode_stru *)puc_param;

    /* ????device?????????? */
    pst_mac_device   = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SMPS, "{dmac_config_set_smps_mode::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_mib_smps_mode = (wlan_mib_mimo_power_save_enum_uint8)(pst_smps_mode->uc_smps_mode);
    if (en_mib_smps_mode >= WLAN_MIB_MIMO_POWER_SAVE_BUTT)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SMPS, "{dmac_config_set_smps_mode::unexpect mode[%d].}", en_mib_smps_mode);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????device??????vap??????vap?????????? */
    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++)
    {
        pst_mac_vap_tmp = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (OAL_PTR_NULL == pst_mac_vap_tmp)
        {
            continue;
        }

        if (OAL_TRUE != pst_mac_vap_tmp->pst_mib_info->st_wlan_mib_sta_config.en_dot11HighThroughputOptionImplemented)
        {
            continue;
        }
        /* ??????????????AP????????AP?????????? */
        if (WLAN_VAP_MODE_BSS_AP != pst_mac_vap_tmp->en_vap_mode)
        {
            /* STA?????????? */
            /* ????STA??SMPS????????????SM Power Save Frame?? */
            continue;
        }

        /* ????mib?? */
        pst_mac_vap_tmp->pst_mib_info->st_wlan_mib_ht_sta_cfg.en_dot11MIMOPowerSave = en_mib_smps_mode;
        mac_vap_set_smps(pst_mac_vap_tmp, en_mib_smps_mode);

        /* VAP??user???????????????? */
        if (OAL_FALSE == oal_dlist_is_empty(&pst_mac_vap->st_mac_user_list_head))
        {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_smps_mode::st_mac_user_list_head is not empty.}");
        }
    }

    if (WLAN_MIB_MIMO_POWER_SAVE_MIMO == en_mib_smps_mode)
    {
        pst_mac_device->en_smps = OAL_FALSE;
        pst_mac_device->uc_dev_smps_mode = HAL_SMPS_MODE_DISABLE;
    }
    else
    {
        pst_mac_device->en_smps = OAL_TRUE;
        pst_mac_device->uc_dev_smps_mode = en_mib_smps_mode;
    }

    /* ??SMPS?????????? */
    hal_set_smps_mode(pst_mac_device->pst_device_stru, pst_mac_device->uc_dev_smps_mode);

    return OAL_SUCC;
}

#endif


oal_uint32 dmac_config_set_app_ie_to_vap(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_app_ie_stru         *pst_app_ie;

    if ((OAL_PTR_NULL == puc_param) || (OAL_PTR_NULL == pst_mac_vap))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_app_ie_to_vap::param is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_app_ie = (oal_app_ie_stru *)puc_param;

    mac_vap_save_app_ie(pst_mac_vap, pst_app_ie, pst_app_ie->en_app_ie_type);
    return OAL_SUCC;
}


oal_uint32 dmac_config_cancel_remain_on_channel(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_scan_req_stru       *pst_scan_req_params;
    mac_device_stru         *pst_mac_device;

    /* ????mac dev */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_config_cancel_remain_on_channel::pst_mac_device[%d] is NULL!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 1.?????????????????????????????????????????????? */
    if (MAC_SCAN_STATE_RUNNING != pst_mac_device->en_curr_scan_state)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_cancel_remain_on_channel::scan is not running.}");
        return OAL_SUCC;
    }

    pst_scan_req_params = &(pst_mac_device->st_scan_params);

    /* 2.??????????????vap??????p2p???????????????????????????????????????????????????????????? */
    if ((pst_mac_vap->uc_vap_id != pst_scan_req_params->uc_vap_id) ||
        (MAC_SCAN_FUNC_P2P_LISTEN != pst_scan_req_params->uc_scan_func))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                        "{dmac_config_cancel_remain_on_channel::other vap is scanning, scan_func[%d].}",
                        pst_scan_req_params->uc_scan_func);
        return OAL_SUCC;
    }

    /* 3.???????????????????????????? */
    if (OAL_TRUE == pst_mac_device->st_scan_timer.en_is_registerd)
    {
        FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&pst_mac_device->st_scan_timer);
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{dmac_config_cancel_remain_on_channel::stop scan.}");

        dmac_scan_end(pst_mac_device);
    }
    else
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "shouldn't go here");
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_P2P

oal_uint32  dmac_config_set_p2p_ps_ops(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru               *pst_dmac_vap;
    hal_to_dmac_vap_stru        *pst_hal_vap;
    mac_cfg_p2p_ops_param_stru  *pst_p2p_ops;


    pst_p2p_ops = (mac_cfg_p2p_ops_param_stru *)puc_param;
    /* ????hal_vap ?????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_p2p_ps_ops::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }


    /*  sanity check, CTwindow 7bit*/
    if(pst_p2p_ops->uc_ct_window < 127)
    {
        /*wpa_supplicant??????ctwindow size,????dmac,??????????*/
        pst_dmac_vap->st_p2p_ops_param.uc_ct_window= pst_p2p_ops->uc_ct_window;
        return OAL_SUCC;
    }
    /*  sanity check, opps ctrl 1bit*/
    if(pst_p2p_ops->en_ops_ctrl <  2)
    {
        if((pst_p2p_ops->en_ops_ctrl ==  0)&& IS_P2P_OPPPS_ENABLED(pst_dmac_vap))
        {
            /* ???????? */
            dmac_p2p_handle_ps(pst_dmac_vap, OAL_FALSE);
        }
        pst_dmac_vap->st_p2p_ops_param.en_ops_ctrl = pst_p2p_ops->en_ops_ctrl;
    }
    else
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_set_p2p_ps_ops:invalid ops ctrl value[%d]\r\n",
                           pst_p2p_ops->en_ops_ctrl);
        return OAL_FAIL;
    }
    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_set_p2p_ps_ops:ctrl[%d] ct_window[%d] vap state[%d]\r\n",
                pst_dmac_vap->st_p2p_ops_param.en_ops_ctrl,
                pst_dmac_vap->st_p2p_ops_param.uc_ct_window,
                pst_mac_vap->en_vap_state);
    pst_hal_vap  = pst_dmac_vap->pst_hal_vap;
    /* ????P2P ops ?????? */
    hal_vap_set_ops(pst_hal_vap, pst_dmac_vap->st_p2p_ops_param.en_ops_ctrl, pst_dmac_vap->st_p2p_ops_param.uc_ct_window);
    return OAL_SUCC;
}


oal_uint32  dmac_config_set_p2p_ps_noa(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru               *pst_dmac_vap;
    mac_cfg_p2p_noa_param_stru  *pst_p2p_noa;
    oal_uint32                   ul_current_tsf_lo;
    mac_device_stru             *pst_mac_device;

    pst_p2p_noa = (mac_cfg_p2p_noa_param_stru *)puc_param;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_device))
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{dmac_config_set_p2p_ps_noa::pst_mac_device[%d] is NULL!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if(IS_P2P_GO(pst_mac_vap))
    {
#ifdef _PRE_WLAN_FEATURE_DBAC
        if (mac_is_dbac_running(pst_mac_device))
        {
            /* dbac?????? go??noa??dbac???? */
            return OAL_SUCC;
        }
#endif

        /*??????GO, interval????beacon interval*/
        pst_p2p_noa->ul_interval = pst_mac_device->ul_beacon_interval * 1024;
    }

    /* ????dmac_vap ?????? */
    pst_dmac_vap = (dmac_vap_stru *)mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id,OAM_SF_ANY,"{dmac_config_set_p2p_ps_noa::mac_res_get_dmac_vap fail or pst_dmac_vap->pst_hal_vap NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ????????????encap probe rsp,beacon*/
    if (pst_p2p_noa->uc_count != 0)
	{
        pst_dmac_vap->st_p2p_noa_param.uc_count = pst_p2p_noa->uc_count;
        pst_dmac_vap->st_p2p_noa_param.ul_duration = pst_p2p_noa->ul_duration;
        pst_dmac_vap->st_p2p_noa_param.ul_interval = pst_p2p_noa->ul_interval;
        hal_vap_tsf_get_32bit(pst_dmac_vap->pst_hal_vap, &ul_current_tsf_lo);
        pst_p2p_noa->ul_start_time += ul_current_tsf_lo;
        pst_dmac_vap->st_p2p_noa_param.ul_start_time = pst_p2p_noa->ul_start_time;
    }
    else
    {
    	if(IS_P2P_NOA_ENABLED(pst_dmac_vap))
        {
            /* ???????? */
            dmac_p2p_handle_ps(pst_dmac_vap, OAL_FALSE);
        }

        OAL_MEMZERO(&(pst_dmac_vap->st_p2p_noa_param), OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));
    }
    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_set_p2p_ps_noa:start_time:%d, duration:%d, interval:%d, count:%d\r\n",
                    pst_dmac_vap->st_p2p_noa_param.ul_start_time,
                    pst_dmac_vap->st_p2p_noa_param.ul_duration,
                    pst_dmac_vap->st_p2p_noa_param.ul_interval,
                    pst_dmac_vap->st_p2p_noa_param.uc_count);
    /* ????P2P noa ?????? */
    hal_vap_set_noa(pst_dmac_vap->pst_hal_vap,
                    pst_dmac_vap->st_p2p_noa_param.ul_start_time,
                    pst_dmac_vap->st_p2p_noa_param.ul_duration,
                    pst_dmac_vap->st_p2p_noa_param.ul_interval,
                    pst_dmac_vap->st_p2p_noa_param.uc_count);
    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM

oal_uint32  dmac_config_set_sta_ps_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru                   *pst_dmac_vap;
    mac_cfg_ps_mode_param_stru      *pst_ps_mode_param;

    pst_ps_mode_param = (mac_cfg_ps_mode_param_stru *)puc_param;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_set_sta_ps_mode::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    if (pst_ps_mode_param->uc_vap_ps_mode < NUM_PS_MODE)
    {
        pst_dmac_vap->uc_cfg_pm_mode = pst_ps_mode_param->uc_vap_ps_mode;
    }
    else
    {
        OAM_WARNING_LOG1(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,"dmac set pm mode[%d]> max mode fail",pst_ps_mode_param->uc_vap_ps_mode);
    }

    return OAL_SUCC;

}

oal_void dmac_set_sta_pm_on(mac_vap_stru *pst_mac_vap, oal_uint8 uc_vap_ps_mode)
{
    dmac_vap_stru                   *pst_dmac_vap;
    mac_sta_pm_handler_stru         *pst_sta_pm_handle;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id,OAM_SF_PWR,"{dmac_set_sta_pm_on::mac_res_get_dmac_vap fail}");
        return;
    }
    pst_sta_pm_handle = (mac_sta_pm_handler_stru *)(pst_dmac_vap->pst_pm_handler);

    if (OAL_PTR_NULL == pst_sta_pm_handle)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{dmac_set_sta_pm_on::pst_sta_pm_handle null}");
        return;
    }

    /* STA PM ?????????? */
    dmac_sta_initialize_psm_globals(pst_sta_pm_handle);

    /* ?????????????????????????????????????????????????????????????? */
    if ((uc_vap_ps_mode == pst_sta_pm_handle->uc_vap_ps_mode))
    {
        OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{dmac_set_sta_pm_on:the same ps mode:[%d],hw_ps:[%d]}", pst_sta_pm_handle->uc_vap_ps_mode, pst_sta_pm_handle->en_hw_ps_enable);
        return;
    }

    if (uc_vap_ps_mode == NO_POWERSAVE)
    {
        pst_sta_pm_handle->uc_vap_ps_mode = uc_vap_ps_mode;
        mac_mib_set_powermanagementmode(pst_mac_vap, WLAN_MIB_PWR_MGMT_MODE_ACTIVE);
    }

    /* ??????????????????????????,????????????????????????????????????,????activity?????? */
    if ((MAC_VAP_STATE_UP == pst_dmac_vap->st_vap_base_info.en_vap_state) || (MAC_VAP_STATE_PAUSE == pst_dmac_vap->st_vap_base_info.en_vap_state))
    {
        pst_sta_pm_handle->uc_vap_ps_mode = uc_vap_ps_mode;
        if (uc_vap_ps_mode != NO_POWERSAVE)
        {
            mac_mib_set_powermanagementmode(pst_mac_vap, WLAN_MIB_PWR_MGMT_MODE_PWRSAVE);

            /* ????????????????????keepalive???? */
            dmac_psm_update_keepalive(pst_dmac_vap);
        }

        dmac_psm_start_activity_timer(pst_dmac_vap,pst_sta_pm_handle);
    }
    else
    {
        OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR,"{dmac_set_sta_pm_on::vap state:[%d],assoc aid:[%d] not start timer}",pst_dmac_vap->st_vap_base_info.en_vap_state,pst_dmac_vap->st_vap_base_info.us_sta_aid);
        return;
    }

    OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{dmac_set_sta_pm_on:ps mode:[%d],aid[%d]}", uc_vap_ps_mode,pst_dmac_vap->st_vap_base_info.us_sta_aid);
}

#ifdef _PRE_PSM_DEBUG_MODE
extern  oal_uint8 g_pm_wlan_debug;
oal_uint32 dmac_show_ps_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_ps_info_stru            *pst_ps_info;
    dmac_vap_stru                   *pst_dmac_vap;
    mac_sta_pm_handler_stru         *pst_sta_pm_handle;
    oal_uint8                        uc_psm_info_enable;  //????psm??????????
    oal_uint8                        uc_psm_debug_mode;
    oal_uint8                        uc_doze_trans_flag;
    oal_uint32                       uiCnt;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap || OAL_PTR_NULL == pst_dmac_vap->pst_hal_device)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_show_ps_info::mac_res_get_dmac_vap fail or pst_dmac_vap->pst_hal_device NULL,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    pst_sta_pm_handle = (mac_sta_pm_handler_stru *)(pst_dmac_vap->pst_pm_handler);

    if (OAL_PTR_NULL == pst_sta_pm_handle)
    {
        OAM_WARNING_LOG0(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{dmac_show_ps_info::pst_sta_pm_handle null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_ps_info = (mac_cfg_ps_info_stru *)puc_param;
    uc_psm_info_enable = pst_ps_info->uc_psm_info_enable;
    uc_psm_debug_mode  = pst_ps_info->uc_psm_debug_mode;


    /* ??doze?????? */
    uc_doze_trans_flag = (pst_sta_pm_handle->en_beacon_frame_wait) | (pst_sta_pm_handle->st_null_wait.en_doze_null_wait << 1) | (pst_sta_pm_handle->en_more_data_expected << 2)
                 | (pst_sta_pm_handle->st_null_wait.en_active_null_wait << 3) | (pst_sta_pm_handle->en_direct_change_to_active << 4);

    if (1 == uc_psm_info_enable)
    {
        for(uiCnt= 0; uiCnt < PM_MSG_COUNT; uiCnt++)
        {
            OAM_WARNING_LOG2(pst_dmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_PWR, "{pm debug msg[%d] [%d]}",uiCnt,pst_sta_pm_handle->aul_pmDebugCount[uiCnt]);
        }
    }
    /* ????????????2?????????????????? */
    else if (2 == uc_psm_info_enable)
    {
        OAM_WARNING_LOG_ALTER(pst_dmac_vap->st_vap_base_info.uc_vap_id,OAM_SF_PWR,"{dmac_show_ps_info::now pm state:[%d],ps mode:[%d],don't doze reason[0x%x],aid[%d],rfpwron[%d],paldo[%d].}",6,
                     STA_GET_PM_STATE(pst_sta_pm_handle),pst_sta_pm_handle->uc_vap_ps_mode,uc_doze_trans_flag,pst_dmac_vap->st_vap_base_info.us_sta_aid,g_us_PmWifiSleepRfPwrOn,g_us_PmEnablePaldo);

        OAM_WARNING_LOG4(pst_dmac_vap->st_vap_base_info.uc_vap_id,OAM_SF_PWR,"{full phy freq user[%d],psm timer:timout[%d],cnt[%d].}",pst_dmac_vap->pst_hal_device->uc_full_phy_freq_user_cnt,g_device_wlan_pm_timeout,g_pm_timer_restart_cnt,
                           pst_sta_pm_handle->uc_psm_timer_restart_cnt);

        dmac_pm_key_info_dump(pst_dmac_vap);
    }
    else
    {
        OAL_MEMZERO(&(pst_sta_pm_handle->aul_pmDebugCount),OAL_SIZEOF(pst_sta_pm_handle->aul_pmDebugCount));
        g_deepsleep_fe_awake_cnt   = 0;
        g_lightsleep_fe_awake_cnt  = 0;
    }

    if (OAL_TRUE == uc_psm_debug_mode)
    {
        g_pm_wlan_debug = 1;
    }
    else
    {
        g_pm_wlan_debug = 0;
    }

    return OAL_SUCC;
}
#endif
 
oal_uint32 dmac_set_psm_param(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_ps_param_stru           *pst_ps_param;
    dmac_vap_stru                   *pst_dmac_vap;
    oal_uint16                       us_beacon_timeout;
    oal_uint16                       us_tbtt_offset;
    oal_uint16                       us_ext_tbtt_offset;
    oal_uint16                       us_dtim3_on;

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_set_psm_param::mac_res_get_dmac_vap fail,vap_id:%u.}",
                         pst_mac_vap->uc_vap_id);
        return OAL_FAIL;
    }

    pst_ps_param = (mac_cfg_ps_param_stru *)puc_param;
    us_beacon_timeout = pst_ps_param->us_beacon_timeout;
    us_tbtt_offset = pst_ps_param->us_tbtt_offset;
    us_ext_tbtt_offset = pst_ps_param->us_ext_tbtt_offset;
    us_dtim3_on        = pst_ps_param->us_dtim3_on;

    /* beacon timeout value */
    if (us_beacon_timeout != 0)
    {
        //hal_set_psm_listen_interval(pst_dmac_vap->pst_hal_vap, us_listen_interval);
        //hal_set_psm_listen_interval_count(pst_dmac_vap->pst_hal_vap, us_listen_interval);
        pst_dmac_vap->us_beacon_timeout = us_beacon_timeout;
        hal_set_beacon_timeout_val(pst_dmac_vap->pst_hal_device, us_beacon_timeout);
    }

    /* INTER TBTT OFFSET */
    if (us_tbtt_offset != 0)
    {
        pst_dmac_vap->us_in_tbtt_offset = us_tbtt_offset;
        hal_set_psm_tbtt_offset(pst_dmac_vap->pst_hal_vap, us_tbtt_offset);
    }

    /* EXT TBTT OFFSET*/
    if (us_ext_tbtt_offset != 0)
    {
        pst_dmac_vap->us_ext_tbtt_offset = us_ext_tbtt_offset;
        hal_set_psm_ext_tbtt_offset(pst_dmac_vap->pst_hal_vap, us_ext_tbtt_offset);
    }

    g_uc_max_powersave = (0==us_dtim3_on)?0:1;

    dmac_psm_update_dtime_period(pst_mac_vap,
                                    (oal_uint8)pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.ul_dot11DTIMPeriod,
                                    pst_dmac_vap->st_vap_base_info.pst_mib_info->st_wlan_mib_sta_config.ul_dot11BeaconPeriod);

    dmac_psm_update_keepalive(pst_dmac_vap);

    return OAL_SUCC;
}

 
oal_uint32 dmac_psm_check_module_ctrl(mac_vap_stru *pst_mac_vap, mac_pm_ctrl_type_enum_uint8 pm_ctrl_type, mac_pm_switch_enum_uint8 pm_enable, oal_uint8 *puc_psm_result)
{
    oal_uint32      ul_sta_pm_close_status;
    dmac_vap_stru  *pst_dmac_vap            = OAL_PTR_NULL;

    if(OAL_PTR_NULL == pst_mac_vap ||
       OAL_PTR_NULL == puc_psm_result)
    {
        OAM_ERROR_LOG2(0, OAM_SF_PWR,"{dmac_psm_check_module_ctrl::Param NULL, pst_mac_vap = 0x%X, puc_psm_result = 0x%X.}",
                         pst_mac_vap,puc_psm_result);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap    = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);

    if(OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PWR,"{dmac_psm_check_module_ctrl::pst_dmac_vap is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR,"{dmac_psm_check_module_ctrl::module[%d],open/close[%d],last open_by_host[%d],close status[%d].}",pm_ctrl_type, pm_enable, pst_dmac_vap->uc_sta_pm_open_by_host,pst_dmac_vap->uc_sta_pm_close_status);

    ul_sta_pm_close_status = pst_dmac_vap->uc_sta_pm_close_status;

    if(MAC_STA_PM_SWITCH_ON == pm_enable)
    {
        //???? pm_close ?????????? bit ??
        ul_sta_pm_close_status &= (~(oal_uint32)(1<<(oal_uint32)pm_ctrl_type));
        if(MAC_STA_PM_CTRL_TYPE_HOST == pm_ctrl_type)
        {
            pst_dmac_vap->uc_sta_pm_open_by_host = MAC_STA_PM_SWITCH_ON;
        }
    }
    else if(MAC_STA_PM_SWITCH_OFF == pm_enable)
    {
        //?? pm_close ?????????? bit ??
        ul_sta_pm_close_status |= (1<<(oal_uint32)pm_ctrl_type);
        if(MAC_STA_PM_CTRL_TYPE_HOST == pm_ctrl_type)
        {
            pst_dmac_vap->uc_sta_pm_open_by_host = MAC_STA_PM_SWITCH_OFF;
        }
    }
    else
    {
        //????????????
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,"{dmac_psm_check_module_ctrl::pm_enable = %d ERROR!!}",pm_enable);
    }

    pst_dmac_vap->uc_sta_pm_close_status = (oal_uint8)ul_sta_pm_close_status;

    /* ?????????????????????????????????? */
    if(0 != pst_dmac_vap->uc_sta_pm_close_status)
    {
        *puc_psm_result = MAC_STA_PM_SWITCH_OFF;
    }
    else
    {
        /* HOST ????????????????????*/
        *puc_psm_result = pst_dmac_vap->uc_sta_pm_open_by_host;
    }

    return OAL_SUCC;
}

 
 oal_uint32 dmac_config_set_sta_pm_on(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
 {
    mac_cfg_ps_open_stru            *pst_sta_pm_open = OAL_PTR_NULL;
    dmac_vap_stru                   *pst_dmac_vap = OAL_PTR_NULL;
    oal_uint8                        uc_final_open_sta_pm = 0;
    oal_uint32                       ul_ret = OAL_SUCC;
    oal_uint8                        uc_cfg_pm_mode;
    mac_device_stru                 *pst_mac_device;

    pst_dmac_vap    = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_set_sta_pm_on::pst_dmac_vap null}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_dmac_vap->st_vap_base_info.uc_device_id);
    if(OAL_PTR_NULL == pst_mac_device)
    {
      OAM_ERROR_LOG0(0, OAM_SF_PWR, "{dmac_config_set_sta_pm_on::pst_mac_device null.}");

      return OAL_ERR_CODE_PTR_NULL;
    }

    pst_sta_pm_open = (mac_cfg_ps_open_stru *)puc_param;

    if (WLAN_VAP_MODE_BSS_STA != pst_mac_vap->en_vap_mode)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR,"{dmac_config_set_sta_pm_on::vap mode is:[%d] not sta}",pst_mac_vap->en_vap_mode);
        return OAL_SUCC;
    }

    ul_ret = dmac_psm_check_module_ctrl(pst_mac_vap, pst_sta_pm_open->uc_pm_ctrl_type, pst_sta_pm_open->uc_pm_enable, &uc_final_open_sta_pm);

    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_sta_pm_on::dmac_psm_check_module_ctrl failed, ul_ret=%d.}", ul_ret);
        return ul_ret;
    }

    if (MAC_STA_PM_SWITCH_ON == uc_final_open_sta_pm)
    {
        uc_cfg_pm_mode = pst_dmac_vap->uc_cfg_pm_mode;

        /*????????????????pm mode???? NO_POWERSAVE */
        if (NO_POWERSAVE == uc_cfg_pm_mode)
        {
            OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG,"{dmac_config_set_sta_pm_on::modual[%d]want to open[%d]pm,but pm mode is no powersave}",pst_sta_pm_open->uc_pm_ctrl_type,pst_sta_pm_open->uc_pm_enable);
            uc_cfg_pm_mode = MIN_FAST_PS;
        }
    }
    else
    {
        /* ????????????????????????work?? */
        PM_WLAN_PsmHandle(pst_dmac_vap->pst_hal_vap->uc_service_id, PM_WLAN_WORK_PROCESS);

        /* ?????????????????????? */
        dmac_pm_enable_front_end(pst_mac_device, OAL_TRUE);

        /* ????????????,??????????????NO_POWERSAVE */
        uc_cfg_pm_mode = NO_POWERSAVE;

    }

    dmac_set_sta_pm_on(pst_mac_vap, uc_cfg_pm_mode);

    return OAL_SUCC;

 }
#endif


OAL_STATIC oal_uint32 dmac_config_nss(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    mac_user_nss_stru  *pst_user_nss;
    mac_user_stru      *pst_mac_user;

    pst_user_nss = (mac_user_nss_stru *)puc_param;
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_user_nss->us_user_idx);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_spatial_stream::mac user(idx=%d) is null!", pst_user_nss->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_set_num_spatial_stream(pst_mac_user, pst_user_nss->uc_num_spatial_stream);
    mac_user_set_avail_num_spatial_stream(pst_mac_user, pst_user_nss->uc_avail_num_spatial_stream);
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_opmode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY

    mac_user_opmode_stru  *pst_user_opmode;
    mac_user_stru         *pst_mac_user;

    pst_user_opmode = (mac_user_opmode_stru *)puc_param;
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_user_opmode->us_user_idx);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_opmode::mac user(idx=%d) is null!", pst_user_opmode->us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_avail_bf_num_spatial_stream(pst_mac_user, pst_user_opmode->uc_avail_bf_num_spatial_stream);
    if (OAL_FALSE == mac_check_is_assoc_frame(pst_user_opmode->uc_frame_type))
    {
        /* ???????????????? */
        dmac_alg_cfg_user_spatial_stream_notify(pst_mac_user);
    }

    mac_user_set_avail_num_spatial_stream(pst_mac_user, pst_user_opmode->uc_avail_num_spatial_stream);
    if (OAL_FALSE == mac_check_is_assoc_frame(pst_user_opmode->uc_frame_type))
    {
        /* ???????????????? */;
        dmac_alg_cfg_user_spatial_stream_notify(pst_mac_user);
    }

    mac_user_set_bandwidth_info(pst_mac_user, pst_user_opmode->en_avail_bandwidth, pst_user_opmode->en_cur_bandwidth);
    if (OAL_FALSE == mac_check_is_assoc_frame(pst_user_opmode->uc_frame_type))
    {
        /* ???????????????? */;
        dmac_alg_cfg_user_bandwidth_notify(pst_mac_vap, pst_mac_user);
    }
#endif
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD

OAL_STATIC oal_uint32 dmac_config_ip_add(dmac_vap_stru *pst_dmac_vap, dmac_ip_addr_config_stru *pst_ip_addr_info)
{
    mac_vap_stru              *pst_mac_vap       = &pst_dmac_vap->st_vap_base_info;
    oal_uint32                 ul_loop;
    oal_bool_enum_uint8        en_comp           = OAL_FALSE;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_dmac_vap->pst_ip_addr_info))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_add::IP record array memory is not alloced.}");
        return OAL_FAIL;
    }

    if (DMAC_CONFIG_IPV4 == pst_ip_addr_info->en_type)
    {
        for (ul_loop = 0; ul_loop < DMAC_MAX_IPV4_ENTRIES; ul_loop++)
        {
            if ((OAL_FALSE == en_comp) && (0 == (pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.ul_value)))
            {
                en_comp = OAL_TRUE; /* ???????? */
                oal_memcopy(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.auc_value, pst_ip_addr_info->auc_ip_addr, OAL_IPV4_ADDR_SIZE);
                oal_memcopy(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_mask.auc_value, pst_ip_addr_info->auc_mask_addr, OAL_IPV4_ADDR_SIZE);
            }
            else if (0 == oal_memcmp(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.auc_value, pst_ip_addr_info->auc_ip_addr, OAL_IPV4_ADDR_SIZE))
            {
                if (OAL_TRUE == en_comp)
                {
                    oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.auc_value, 0, OAL_IPV4_ADDR_SIZE);
                    oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_mask.auc_value, 0, OAL_IPV4_ADDR_SIZE);
                }
                else
                {
                    en_comp = OAL_TRUE;
                }
            }
        }
    }
    else if (DMAC_CONFIG_IPV6 == pst_ip_addr_info->en_type)
    {
        for (ul_loop = 0; ul_loop < DMAC_MAX_IPV6_ENTRIES; ul_loop++)
        {
            if ((OAL_FALSE == en_comp) && OAL_IPV6_IS_UNSPECIFIED_ADDR(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr))
            {
                en_comp = OAL_TRUE; /* ???????? */
                oal_memcopy(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr, pst_ip_addr_info->auc_ip_addr, OAL_IPV6_ADDR_SIZE);
            }
            else if (0 == oal_memcmp(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr, pst_ip_addr_info->auc_ip_addr, OAL_IPV6_ADDR_SIZE))
            {
                if (OAL_TRUE == en_comp)
                {
                    oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr, 0, OAL_IPV6_ADDR_SIZE);
                }
                else
                {
                    en_comp = OAL_TRUE;
                }
            }
        }
    }
    else
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_add::IP type[%d] is wrong.}", pst_ip_addr_info->en_type);
        return OAL_FAIL;
    }

    if (OAL_FALSE == en_comp)
    {
        if (DMAC_CONFIG_IPV4 == pst_ip_addr_info->en_type)
        {
            OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_add::Add IPv4 address[%d.X.X.%d] failed, there is no empty array.}",
                           ((oal_uint8 *)&(pst_ip_addr_info->auc_ip_addr))[0],
                           ((oal_uint8 *)&(pst_ip_addr_info->auc_ip_addr))[3]);
        }
        else if (DMAC_CONFIG_IPV6 == pst_ip_addr_info->en_type)
        {
            OAM_ERROR_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_add::Add IPv6 address[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x] failed, there is no empty array.}",
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[0]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[1]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[6]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[7]));
        }
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 dmac_config_ip_del(dmac_vap_stru *pst_dmac_vap, dmac_ip_addr_config_stru *pst_ip_addr_info)
{
    mac_vap_stru              *pst_mac_vap       = &pst_dmac_vap->st_vap_base_info;
    oal_uint32                 ul_loop;
    oal_bool_enum_uint8        en_comp           = OAL_FALSE;

    if (DMAC_CONFIG_IPV4 == pst_ip_addr_info->en_type)
    {
        for (ul_loop = 0; ul_loop < DMAC_MAX_IPV4_ENTRIES; ul_loop++)
        {
            if ((OAL_FALSE == en_comp) && (0 == oal_memcmp(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.auc_value, pst_ip_addr_info->auc_ip_addr, OAL_IPV4_ADDR_SIZE)))
            {
                en_comp = OAL_TRUE; /* ???????? */
                oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_local_ip.auc_value, 0, OAL_IPV4_ADDR_SIZE);
                oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv4_entry[ul_loop].un_mask.auc_value, 0, OAL_IPV4_ADDR_SIZE);
                break;
            }
        }
    }
    else if (DMAC_CONFIG_IPV6 == pst_ip_addr_info->en_type)
    {
        for (ul_loop = 0; ul_loop < DMAC_MAX_IPV6_ENTRIES; ul_loop++)
        {
            if ((OAL_FALSE == en_comp) && (0 == oal_memcmp(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr, pst_ip_addr_info->auc_ip_addr, OAL_IPV6_ADDR_SIZE)))
            {
                en_comp = OAL_TRUE; /* ???????? */
                oal_memset(pst_dmac_vap->pst_ip_addr_info->ast_ipv6_entry[ul_loop].auc_ip_addr, 0, OAL_IPV6_ADDR_SIZE);
                break;
            }
        }
    }
    else
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_del::IP type[%d] is wrong.}", pst_ip_addr_info->en_type);
        return OAL_FAIL;
    }

    if (OAL_FALSE == en_comp)
    {
        if (DMAC_CONFIG_IPV4 == pst_ip_addr_info->en_type)
        {
            OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_del::Delete IPv4 address[%d.X.X.%d] failed, there is not the IP address.}",
                           ((oal_uint8 *)&(pst_ip_addr_info->auc_ip_addr))[0],
                           ((oal_uint8 *)&(pst_ip_addr_info->auc_ip_addr))[3]);
        }
        else if (DMAC_CONFIG_IPV6 == pst_ip_addr_info->en_type)
        {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_ip_del::Delete IPv6 address[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x] failed, there is not the IP address.}",
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[0]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[1]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[6]),
                           OAL_NET2HOST_SHORT(((oal_uint16 *)&(pst_ip_addr_info->auc_ip_addr))[7]));
        }
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


oal_uint32 dmac_config_set_ip_addr(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru             *pst_dmac_vap      = (dmac_vap_stru *)pst_mac_vap;
    dmac_ip_addr_config_stru  *pst_ip_addr_info  = (dmac_ip_addr_config_stru *)puc_param;

    switch (pst_ip_addr_info->en_oper)
    {
        case DMAC_IP_ADDR_ADD:
        {
            return dmac_config_ip_add(pst_dmac_vap, pst_ip_addr_info);
        }

        case DMAC_IP_ADDR_DEL:
        {
            return dmac_config_ip_del(pst_dmac_vap, pst_ip_addr_info);
        }

        default:
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_set_ip_addr::IP operation[%d] is wrong.}", pst_ip_addr_info->en_oper);
            break;
        }
    }
    return OAL_FAIL;
}
#endif
#ifdef _PRE_WLAN_FEATURE_ROAM

OAL_INLINE oal_void dmac_frame_modify_bssid(oal_netbuf_stru *pst_netbuf, oal_uint8 *puc_bssid)
{
    mac_ieee80211_qos_htc_frame_addr4_stru   *pst_mac_hdr;
    mac_tx_ctl_stru                          *pst_tx_ctl;
    oal_uint8                                 uc_is_tods;
    oal_uint8                                 uc_is_from_ds;

    pst_mac_hdr = (mac_ieee80211_qos_htc_frame_addr4_stru *)oal_netbuf_data(pst_netbuf);
    pst_tx_ctl  = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    uc_is_tods    = mac_hdr_get_to_ds((oal_uint8 *)pst_mac_hdr);
    uc_is_from_ds = mac_hdr_get_from_ds((oal_uint8 *)pst_mac_hdr);

    /*************************************************************************/
    /*                  80211 MAC HEADER                                     */
    /* --------------------------------------------------------------------- */
    /* | To   | From |  ADDR1 |  ADDR2 | ADDR3  | ADDR3  | ADDR4  | ADDR4  | */
    /* | DS   |  DS  |        |        | MSDU   | A-MSDU |  MSDU  | A-MSDU | */
    /* ----------------------------------------------------------------------*/
    /* |  0   |   0  |  RA=DA |  TA=SA | BSSID  |  BSSID |   N/A  |   N/A  | */
    /* |  0   |   1  |  RA=DA |TA=BSSID|   SA   |  BSSID |   N/A  |   N/A  | */
    /* |  1   |   0  |RA=BSSID| RA=SA  |   DA   |  BSSID |   N/A  |   N/A  | */
    /* |  1   |   1  |  RA    |   TA   |   DA   |  BSSID |   SA   |  BSSID | */
    /*************************************************************************/

    if ((0 == uc_is_tods) && (0 == uc_is_from_ds))
    {
        oal_set_mac_addr(pst_mac_hdr->auc_address3, puc_bssid);
        return;
    }

    if ((0 == uc_is_tods) && (1 == uc_is_from_ds))
    {
        oal_set_mac_addr(pst_mac_hdr->auc_address2, puc_bssid);
        if (mac_get_cb_is_amsdu(pst_tx_ctl))
        {
            oal_set_mac_addr(pst_mac_hdr->auc_address3, puc_bssid);
        }
        return;
    }

    if ((1 == uc_is_tods) && (0 == uc_is_from_ds))
    {
        oal_set_mac_addr(pst_mac_hdr->auc_address1, puc_bssid);
        if (mac_get_cb_is_amsdu(pst_tx_ctl))
        {
            oal_set_mac_addr(pst_mac_hdr->auc_address3, puc_bssid);
        }
        return;
    }

    if ((1 == uc_is_tods) && (1 == uc_is_from_ds))
    {
        if (mac_get_cb_is_amsdu(pst_tx_ctl))
        {
            oal_set_mac_addr(pst_mac_hdr->auc_address3, puc_bssid);
            oal_set_mac_addr(pst_mac_hdr->auc_address4, puc_bssid);
        }
        return;
    }

    return;
}


OAL_INLINE oal_uint32  dmac_roam_reset_ba(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user, mac_device_stru *pst_mac_device)
{
    hal_to_dmac_device_stru                *pst_hal_device;
    dmac_tid_stru                          *pst_tid_queue;
    oal_uint8                               uc_tid_idx;

    pst_hal_device = pst_dmac_vap->pst_hal_device;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_roam_reset_ba::pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_tid_idx = 0; uc_tid_idx < WLAN_TID_MAX_NUM; uc_tid_idx ++)
    {
        pst_tid_queue = &pst_dmac_user->ast_tx_tid_queue[uc_tid_idx];

        /* ????BA?????????? */
        if (OAL_PTR_NULL != pst_tid_queue->pst_ba_rx_hdl)
        {
            dmac_ba_reset_rx_handle(pst_mac_device, &(pst_tid_queue->pst_ba_rx_hdl),uc_tid_idx, pst_dmac_user);
            OAM_WARNING_LOG1(0, OAM_SF_ROAM, "{dmac_roam_reset_ba:: TID[%d]:reset rx ba.}", uc_tid_idx);
        }

        if (OAL_PTR_NULL != pst_tid_queue->pst_ba_tx_hdl)
        {
            dmac_ba_reset_tx_handle(pst_mac_device, &(pst_tid_queue->pst_ba_tx_hdl), uc_tid_idx);
            OAM_WARNING_LOG1(0, OAM_SF_ROAM, "{dmac_roam_reset_ba:: TID[%d]:reset tx ba.}", uc_tid_idx);
        }

    }

    return OAL_SUCC;
}


OAL_INLINE oal_uint32  dmac_roam_update_framer(dmac_vap_stru *pst_dmac_vap, dmac_user_stru *pst_dmac_user)
{
    hal_to_dmac_device_stru                *pst_hal_device;
    dmac_tid_stru                          *pst_tid_queue;
    oal_dlist_head_stru                    *pst_dscr_entry;
    hal_tx_dscr_stru                       *pst_tx_dscr;
    oal_netbuf_stru                        *pst_netbuf;
    oal_uint16                              us_mpdu_num;
    oal_uint8                               uc_tid_idx;

    pst_hal_device = pst_dmac_vap->pst_hal_device;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_hal_device))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_user_update_framer::pst_hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_tid_idx = 0; uc_tid_idx < WLAN_TID_MAX_NUM; uc_tid_idx ++)
    {
        pst_tid_queue = &pst_dmac_user->ast_tx_tid_queue[uc_tid_idx];
#ifdef _PRE_WLAN_FEATURE_TX_DSCR_OPT
        if (OAL_FALSE == oal_dlist_is_empty(&pst_tid_queue->st_retry_q))
        {
            /* ???????????? */
            us_mpdu_num = 0;
            OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &pst_tid_queue->st_retry_q)
            {
                pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

                pst_netbuf  = pst_tx_dscr->pst_skb_start_addr;
                dmac_frame_modify_bssid(pst_netbuf, pst_dmac_user->st_user_base_info.auc_user_mac_addr);
                us_mpdu_num++;
            }
            OAM_WARNING_LOG2(0, OAM_SF_ROAM, "{dmac_roam_update_framer:: TID[%d]:%d mpdu is updated in retry_q.}", uc_tid_idx, us_mpdu_num);
        }
        if (OAL_FALSE == oal_netbuf_list_empty(&pst_tid_queue->st_buff_head))
        {
            /* netbuf???????? */
            us_mpdu_num = 0;
            OAL_NETBUF_SEARCH_FOR_EACH(pst_netbuf, &pst_tid_queue->st_buff_head)
            {
                dmac_frame_modify_bssid(pst_netbuf, pst_dmac_user->st_user_base_info.auc_user_mac_addr);
                us_mpdu_num++;
            }
            OAM_WARNING_LOG2(0, OAM_SF_ROAM, "{dmac_roam_update_framer:: TID[%d]:%d mpdu is updated in buff_q.}", uc_tid_idx, us_mpdu_num);
        }
#else
        if (OAL_FALSE == oal_dlist_is_empty(&pst_tid_queue->st_hdr))
        {
            us_mpdu_num = 0;
            OAL_DLIST_SEARCH_FOR_EACH(pst_dscr_entry, &pst_tid_queue->st_retry_q)
            {
                pst_tx_dscr = OAL_DLIST_GET_ENTRY(pst_dscr_entry, hal_tx_dscr_stru, st_entry);

                pst_netbuf  = pst_tx_dscr->pst_skb_start_addr;
                dmac_frame_modify_bssid(pst_netbuf, pst_dmac_user->st_user_base_info.auc_user_mac_addr);
                us_mpdu_num++;
            }
            OAM_WARNING_LOG2(0, OAM_SF_ROAM, "{dmac_roam_update_framer:: TID[%d]:%d mpdu is updated in hdr_q.}", uc_tid_idx, us_mpdu_num);
        }
#endif /* _PRE_WLAN_FEATURE_TX_DSCR_OPT */

    }

    return OAL_SUCC;
}


oal_uint32  dmac_config_roam_enable(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint8                       uc_roaming_mode;
    mac_device_stru                *pst_mac_device;
    mac_user_stru                  *pst_mac_user;
#ifdef _PRE_WLAN_FEATURE_SMARTANT
    oal_uint32 ul_dual_antenna_result;
#endif

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_config_roam_enable::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_roaming_mode  = (oal_uint8)*puc_param;

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_config_roam_enable::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        OAM_ERROR_LOG1(0, OAM_SF_ROAM,
                       "{dmac_config_roam_enable::pst_mac_user[%d] null.}", pst_mac_vap->uc_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if ((uc_roaming_mode == 1) && (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP || pst_mac_vap->en_vap_state == MAC_VAP_STATE_PAUSE))
    {

        /* ????tid????????????????
        dmac_tid_clear(pst_mac_user, pst_mac_device);*/
        dmac_user_pause((dmac_user_stru *)pst_mac_user);
        dmac_roam_reset_ba((dmac_vap_stru *)pst_mac_vap, (dmac_user_stru *)pst_mac_user, pst_mac_device);

        /*????????????????????dbac?????????????????????????????????? */
        dmac_alg_vap_down_notify(pst_mac_vap);

        /* ?????????????????????????????????????????????????????? */
        PM_WLAN_DumpSleepCnt();

        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM, "{dmac_config_roam_enable:: [MAC_VAP_STATE_UP]->[MAC_VAP_STATE_ROAMING]}");
        pst_mac_vap->en_vap_state = MAC_VAP_STATE_ROAMING;
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        dmac_btcoex_ps_pause_check_and_notify(pst_mac_device->pst_device_stru);
        dmac_btcoex_wlan_priority_set(pst_mac_vap, 1, BTCOEX_PRIO_TIMEOUT_100MS);
#endif //_PRE_WLAN_FEATURE_BTCOEX
#ifdef _PRE_WLAN_FEATURE_SMARTANT
        pst_mac_device->pst_device_stru->st_dual_antenna_check_status.bit_roam = 1;
        hal_dual_antenna_switch(1, 0, &ul_dual_antenna_result);
        OAM_WARNING_LOG1(0, OAM_SF_SMART_ANTENNA, "{dmac_config_roam_enable::dual_antenna to 1, result:%d.}", ul_dual_antenna_result);
#endif
    }
    else if ((uc_roaming_mode == 0) && (pst_mac_vap->en_vap_state == MAC_VAP_STATE_ROAMING))
    {
        /* ?????????????????????????????????????????????????????? */
        PM_WLAN_DumpSleepCnt();

        /* ????????????????occupied_period??????BT?????? */
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        hal_set_btcoex_occupied_period(0);
        dmac_btcoex_wlan_priority_set(pst_mac_vap, 0, 0);
#endif //_PRE_WLAN_FEATURE_BTCOEX
        dmac_roam_update_framer((dmac_vap_stru *)pst_mac_vap, (dmac_user_stru *)pst_mac_user);
        dmac_user_resume((dmac_user_stru *)pst_mac_user);
#ifdef _PRE_WLAN_FEATURE_SMARTANT
        pst_mac_device->pst_device_stru->st_dual_antenna_check_status.bit_roam = 0;
        hal_dual_antenna_switch(0, 0, &ul_dual_antenna_result);
        OAM_WARNING_LOG1(0, OAM_SF_SMART_ANTENNA, "{dmac_config_roam_enable::dual_antenna roam open, alg result:%d.}", ul_dual_antenna_result);
#endif

        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM, "{dmac_config_roam_enable:: [MAC_VAP_STATE_ROAMING]->[MAC_VAP_STATE_UP]}");
        pst_mac_vap->en_vap_state = MAC_VAP_STATE_UP;
    }
    else
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ROAM, "{dmac_config_roam_enable::unexpect state[%d] or mode[%d]}",
                       pst_mac_vap->en_vap_state, uc_roaming_mode);
    }
    return OAL_SUCC;
}

oal_uint32  dmac_config_set_roam_trigger(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_roam_trigger_stru   *pst_roam_trigger;
    dmac_vap_stru           *pst_dmac_vap;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_config_set_roam_trigger::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_roam_trigger  = (mac_roam_trigger_stru *)puc_param;
    pst_dmac_vap      = (dmac_vap_stru *)pst_mac_vap;

    pst_dmac_vap->st_roam_trigger.c_trigger_2G = pst_roam_trigger->c_trigger_2G;
    pst_dmac_vap->st_roam_trigger.c_trigger_5G = pst_roam_trigger->c_trigger_5G;

    /* ????????????reset???????????????????????????????????????????? */
    pst_dmac_vap->st_roam_trigger.ul_cnt        = 0;
    pst_dmac_vap->st_roam_trigger.ul_time_stamp = 0;

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ROAM, "{dmac_config_set_roam_trigger, trigger[%d, %d]}",
                     pst_roam_trigger->c_trigger_2G, pst_roam_trigger->c_trigger_5G);

    return OAL_SUCC;
}


oal_uint32  dmac_config_roam_hmac_sync_dmac(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_user_stru                   *pst_mac_user;
    mac_h2d_roam_sync_stru          *pst_sync_param = OAL_PTR_NULL;
    mac_device_stru                 *pst_mac_device;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{dmac_config_roam_hmac_sync_dmac::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        /* ???????????? kick user ???????????? log level*/
        OAM_WARNING_LOG1(0, OAM_SF_ANY,
                       "{dmac_config_roam_hmac_sync_dmac::pst_mac_user[%d] null.}", pst_mac_vap->uc_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ROAM, "{dmac_config_roam_enable::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_sync_param = (mac_h2d_roam_sync_stru *)puc_param;

    if(OAL_TRUE == pst_sync_param->ul_back_to_old)
    {
        /* ????????bss???????? */
        pst_mac_vap->us_sta_aid = pst_sync_param->us_sta_aid;
        oal_memcopy(&(pst_mac_vap->st_channel), &pst_sync_param->st_channel, OAL_SIZEOF(mac_channel_stru));
        oal_memcopy(&(pst_mac_user->st_cap_info), &pst_sync_param->st_cap_info, OAL_SIZEOF(mac_user_cap_info_stru));
        oal_memcopy(&(pst_mac_user->st_key_info), &pst_sync_param->st_key_info, OAL_SIZEOF(mac_key_mgmt_stru));
        oal_memcopy(&(pst_mac_user->st_user_tx_info),&pst_sync_param->st_user_tx_info, OAL_SIZEOF(mac_user_tx_param_stru));

        /* ?????????????????????????????????????????????????? */
        dmac_user_pause((dmac_user_stru *)pst_mac_user);
        dmac_roam_reset_ba((dmac_vap_stru *)pst_mac_vap, (dmac_user_stru *)pst_mac_user, pst_mac_device);
    }

    /* ????????8021x?????????????????????? */
    mac_user_set_port(pst_mac_user, OAL_TRUE);

    OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_roam_hmac_sync_dmac:: Sync Done!!}");

    return OAL_SUCC;
}



oal_uint32  dmac_config_roam_notify_state(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru   *pst_dmac_vap           = OAL_PTR_NULL;
    oal_uint32       ul_ip_addr_obtained    = OAL_FALSE;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{dmac_config_roam_notify_state::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ip_addr_obtained     = *(oal_uint32 *)puc_param;
    pst_dmac_vap            = (dmac_vap_stru *)pst_mac_vap;

    pst_dmac_vap->st_roam_trigger.ul_ip_addr_obtained = ul_ip_addr_obtained;
    pst_dmac_vap->st_roam_trigger.ul_ip_obtain_stamp = (oal_uint32)OAL_TIME_GET_STAMP_MS();
    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{dmac_config_roam_notify_state:: ul_ip_addr_obtained = %d!!}",ul_ip_addr_obtained);

    return OAL_SUCC;
}

#endif  //_PRE_WLAN_FEATURE_ROAM

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

oal_uint32 dmac_config_suspend_action_sync(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_device_stru             *pst_mac_device;
    mac_cfg_suspend_stru        *pst_suspend;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_suspend_action_sync:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_suspend_action_sync::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_suspend = (mac_cfg_suspend_stru *)puc_param;

    pst_mac_device->uc_in_suspend = pst_suspend->uc_in_suspend; //??????????

    pst_mac_device->uc_arpoffload_switch = pst_suspend->uc_arpoffload_switch; //arp ????

    /*????????dtim2????*/
    dmac_psm_max_powersave_enable(pst_mac_device);

    return OAL_SUCC;
}
#endif

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)

OAL_STATIC oal_uint32 dmac_config_set_txrx_chain(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    dmac_vap_stru               *pst_dmac_vap;
    oal_uint8                   uc_chain;
    mac_device_stru             *pst_mac_device;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_config_set_txrx_chain:: pst_mac_vap or puc_param is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????device */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_txrx_chain::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{dmac_config_set_txrx_chain::pst_dmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_chain = *puc_param;

    if (0 == uc_chain)
    {
        pst_mac_device->uc_tx_chain     = WITP_TX_CHAIN_ZERO;
        g_l_rf_channel_num              = 1;
        g_l_rf_single_tran              = 0;
    }
    else if (1 == uc_chain)
    {
        pst_mac_device->uc_tx_chain     = WITP_TX_CHAIN_ONE;
        g_l_rf_channel_num              = 2;
        g_l_rf_single_tran              = 1;
    }
    else if (2 == uc_chain)
    {
        pst_mac_device->uc_tx_chain     = WITP_TX_CHAIN_DOUBLE;
        g_l_rf_channel_num              = 2;
        g_l_rf_single_tran              = 0;
    }
    else
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "dmac_config_set_txrx_chain:uc_chain = %d\n", uc_chain);
    }

    pst_dmac_vap->uc_vap_tx_chain = pst_mac_device->uc_tx_chain;

    hal_set_txrx_chain(pst_dmac_vap->pst_hal_device);

    /* ?????????????????????????? */
    dmac_vap_init_tx_ucast_data_frame(pst_dmac_vap);

    /* ?????????????????????????????????????? */
    dmac_vap_init_tx_frame_params(pst_dmac_vap, OAL_TRUE);

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_PKT_MEM_OPT
oal_uint32 dmac_pkt_mem_opt_stat_event_process(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru          *pst_event;
    frw_event_hdr_stru      *pst_event_hdr;
    oal_bool_enum_uint8      en_dscr_opt_state;
    mac_device_stru         *pst_mac_device;
    hal_to_dmac_device_stru *pst_hal_device;

    /* ????????????????????????payload?????? */
    pst_event       = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event_hdr   = &(pst_event->st_event_hdr);

    en_dscr_opt_state = pst_event->auc_event_data[0];

    pst_mac_device = (mac_device_stru*)mac_res_get_dev(pst_event_hdr->uc_device_id);
    if(OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "{dmac_dscr_opt_stat_event_process::mac device is null.}");
        return OAL_FAIL;
    }
    pst_hal_device = pst_mac_device->pst_device_stru;
    if(OAL_PTR_NULL == pst_hal_device)
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "{dmac_dscr_opt_stat_event_process::hal device is null.}");
        return OAL_FAIL;
    }

    if(en_dscr_opt_state)
    {
        pst_hal_device->ul_rx_normal_dscr_cnt = HAL_NORMAL_RX_MAX_RX_OPT_BUFFS;
    }
    else
    {
        pst_hal_device->ul_rx_normal_dscr_cnt = HAL_NORMAL_RX_MAX_BUFFS;
    }

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{dmac_dscr_opt_stat_event_process::rx dscr opt change to %d. }", pst_hal_device->ul_rx_normal_dscr_cnt);

    return OAL_SUCC;
}
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC oal_void dmac_config_set_device_cap_rely_on_customize(mac_vap_stru *pst_mac_vap)
{
    mac_device_stru              *pst_mac_device;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{dmac_config_set_device_cap_rely_on_customize:: pointer is null,pst_mac_vap[0x%x] .}", pst_mac_vap);
        return;
    }
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "dmac_config_set_device_cap_rely_on_customize: mac_device is null.device_id[%d]", pst_mac_vap->uc_device_id);
        return;
    }
    /* ??????????5g????????device???????????????????? */
    if (!g_st_customize.st_cali.uc_band_5g_enable)
    {
        pst_mac_device->en_bandwidth_cap = WLAN_BW_CAP_40M;
        pst_mac_device->en_band_cap = WLAN_BAND_CAP_2G;
    }
}

oal_uint32 dmac_config_set_linkloss_threshold(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    mac_cfg_linkloss_threshold*     pst_threshold;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_linkloss_threshold:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_threshold = (mac_cfg_linkloss_threshold *)puc_param;
    g_st_beacon_linkloss.us_linkloss_threshold_wlan_near = pst_threshold->uc_linkloss_threshold_wlan_near;
    g_st_beacon_linkloss.us_linkloss_threshold_wlan_far  = pst_threshold->uc_linkloss_threshold_wlan_far;
    g_st_beacon_linkloss.us_linkloss_threshold_p2p       = pst_threshold->uc_linkloss_threshold_p2p;

    return OAL_SUCC;
}

oal_uint32 dmac_config_set_all_log_level(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32      ul_ret = 0;
    oal_uint8       uc_vap_idx;
    oal_uint8       uc_level;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_all_log_level:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }
    uc_level = (oal_uint8)(*puc_param);

    for (uc_vap_idx = 0; uc_vap_idx < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_idx++)
    {
        ul_ret += oam_log_set_vap_level(uc_vap_idx, uc_level);

        if (OAL_SUCC != ul_ret)
        {
            return ul_ret;
        }
    }
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_BTCOEX

oal_uint32 dmac_config_set_btcoex_ps_switch(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_all_log_level:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }
    g_st_customize.ul_btcoex_ps_switch = *(oal_uint32 *)puc_param;
    if (1 == g_st_customize.ul_btcoex_ps_switch)
    {
        hal_set_btcoex_soc_gpreg1(OAL_TRUE, BTCOEX_WIFI_STATUS_REG1_PS_MODE_MASK, BTCOEX_WIFI_STATUS_REG1_PS_MODE_OFFSET);
    }
    else if (0 == g_st_customize.ul_btcoex_ps_switch)
    {
        hal_set_btcoex_soc_gpreg1(OAL_FALSE, BTCOEX_WIFI_STATUS_REG1_PS_MODE_MASK, BTCOEX_WIFI_STATUS_REG1_PS_MODE_OFFSET);
    }
    else
    {
        OAM_ERROR_LOG1(0,OAM_SF_CFG,"dmac_config_set_btcoex_ps_mode:btcoex_ps_mode[%d] is error",g_st_customize.ul_btcoex_ps_switch);
    }
    return OAL_SUCC;
}
#endif

oal_uint32 dmac_config_set_d2h_hcc_assemble_cnt(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32      ul_val;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_d2h_hcc_assemble_cnt:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_val = (oal_uint32)(*puc_param);
    g_d2h_hcc_assemble_count = (1 <= ul_val && HISDIO_DEV2HOST_SCATT_MAX >= ul_val) ? ul_val : g_d2h_hcc_assemble_count;

    return OAL_SUCC;
}

oal_uint32 dmac_config_set_chn_est_ctrl(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_d2h_hcc_assemble_cnt:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    g_st_customize.ul_chn_est_ctrl = *(oal_uint32*)puc_param;

    return OAL_SUCC;
}


oal_uint32 dmac_config_set_power_ref(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    return OAL_SUCC;
}


oal_uint32 dmac_config_set_pm_cfg_param(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    st_pm_cfg_param*        pst_pm_cfg_param;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_d2h_hcc_assemble_cnt:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_pm_cfg_param = (st_pm_cfg_param *)puc_param;

    g_st_customize.st_pm.ul_rtc_clk_freq = pst_pm_cfg_param->ul_rtc_clk_freq;
    g_st_customize.st_pm.uc_clk_type = pst_pm_cfg_param->uc_clk_type;

    return OAL_SUCC;
}

oal_uint32 dmac_config_set_cus_rf(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint32      ul_offset = 0;
    oal_uint8       uc_agc_ref;
    oal_uint8       uc_band_idx;
    mac_device_stru* pst_mac_dev;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_cus_rf:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_dev)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{dmac_config_set_cus_rf:: mac device[%d] null ptr .}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_memcopy(&g_st_customize.st_rf, puc_param, OAL_SIZEOF(g_st_customize.st_rf));
    ul_offset += OAL_SIZEOF(g_st_customize.st_rf);
    oal_memcopy(&g_st_customize.st_ratio_temp_pwr_comp, puc_param + ul_offset, OAL_SIZEOF(g_st_customize.st_ratio_temp_pwr_comp));
    ul_offset += OAL_SIZEOF(g_st_customize.st_ratio_temp_pwr_comp);
    oal_memcopy(&g_st_customize.st_ce_5g_hi_band_params, puc_param + ul_offset, OAL_SIZEOF(g_st_customize.st_ce_5g_hi_band_params));

    /* refresh cca ed threshold after phy init */
    hal_set_ed_high_th(pst_mac_dev->pst_device_stru,
                       HAL_CCA_OPT_ED_HIGH_20TH_DEF + g_st_customize.st_rf.c_delta_cca_ed_high_20th_2g,
                       HAL_CCA_OPT_ED_HIGH_40TH_DEF + g_st_customize.st_rf.c_delta_cca_ed_high_40th_2g);
    OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "dmac_config_set_cus_rf::delta_20_2g=%d, delta_40_2g=%d, delta_20_5g=%d, delta_40_5g=%d.",
                    g_st_customize.st_rf.c_delta_cca_ed_high_20th_2g,
                    g_st_customize.st_rf.c_delta_cca_ed_high_40th_2g,
                    g_st_customize.st_rf.c_delta_cca_ed_high_20th_5g,
                    g_st_customize.st_rf.c_delta_cca_ed_high_40th_5g);

    OAM_WARNING_LOG_ALTER(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                    "dmac_config_set_cus_rf::[CE HI BAND] tx power[%d], dbbscale [0x%X, 0x%X, 0x%X], mcs8_9 compensation [0x%X, 0x%X].",
                    6,
                    g_st_customize.st_ce_5g_hi_band_params.uc_max_txpower,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_11a_ht20_vht20,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_ht40_vht40,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_vht80,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_ht40_vht40_mcs8_9_comp,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_vht80_mcs8_9_comp);

    /* ???? 2g power0 ref */
    for (uc_band_idx = 0; uc_band_idx < HAL_DEVICE_2G_BAND_NUM_FOR_LOSS; ++uc_band_idx)
    {
        uc_agc_ref = (oal_uint8)(g_st_customize.st_rf.ac_gain_db_2g[uc_band_idx].c_rf_gain_db_mult4 + ((DMAC_RX_LPF_GAIN + DMAC_DBM_CH + DMAC_SINGLE_DOUBLE_SWITCH_GAIN) << 2));

        g_aul_phy_power0_ref_2g[uc_band_idx] = OAL_JOIN_WORD32(uc_agc_ref, uc_agc_ref - DMAC_RSSI_REF_DIFFERENCE, uc_agc_ref - DMAC_RADAR_REF_DIFFERENCE, 0);

        OAM_WARNING_LOG2(0, 0, "g_aul_phy_power0_ref_2g band:%d pwrref:0x%2x.\r\n", uc_band_idx+1, g_aul_phy_power0_ref_2g[uc_band_idx]);
    }

    /* ???? 5g power0 ref */
    for (uc_band_idx = 0; uc_band_idx < HAL_DEVICE_5G_BAND_NUM_FOR_LOSS; ++uc_band_idx)
    {
        uc_agc_ref = (oal_uint8)(g_st_customize.st_rf.ac_gain_db_5g[uc_band_idx].c_rf_gain_db_mult4 + ((DMAC_RX_LPF_GAIN + DMAC_DBM_CH + DMAC_SINGLE_DOUBLE_SWITCH_GAIN) << 2));

        g_aul_phy_power0_ref_5g[uc_band_idx] = OAL_JOIN_WORD32(uc_agc_ref, uc_agc_ref - DMAC_RSSI_REF_DIFFERENCE, uc_agc_ref - DMAC_RADAR_REF_DIFFERENCE, 0);

        OAM_WARNING_LOG2(0, 0, "g_aul_phy_power0_ref_5g band:%d pwrref:0x%2x.\r\n", uc_band_idx+1, g_aul_phy_power0_ref_5g[uc_band_idx]);
    }

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, 0, "dmac_config_set_cus_rf far_dist_pow_gain_switch[%d], far_dist_dsss_scale_promote_switch[%d].\r\n",
        g_st_customize.st_rf.uc_far_dist_pow_gain_switch,
        g_st_customize.st_rf.uc_far_dist_dsss_scale_promote_switch);

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_DOWNLOAD_PM
extern oal_uint16 g_us_download_rate_limit_pps;
oal_uint32 dmac_config_set_download_rate_limit(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    g_us_download_rate_limit_pps = *(oal_uint16*)puc_param;
    //OAM_WARNING_LOG1(0, OAM_SF_PWR, "dmac_config_set_download_rate_limit: %d", g_us_download_rate_limit_pps);
    return OAL_SUCC;
}
#endif

oal_uint32 dmac_config_set_cus_dts_cali(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1151)
    mac_device_stru              *pst_mac_device;
    hal_device_stru              *pst_device;
#endif
    oal_uint32                  ul_offset = 0;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_cus_dts_cali:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_memcopy(&g_st_customize.st_cali, puc_param, OAL_SIZEOF(g_st_customize.st_cali));
    ul_offset += OAL_SIZEOF(g_st_customize.st_cali);
    /* ??????????5g????????device???????????????????? */
    dmac_config_set_device_cap_rely_on_customize(pst_mac_vap);
    oal_memcopy(&g_st_customize.ast_band_edge_limit, puc_param + ul_offset, OAL_SIZEOF(g_st_customize.ast_band_edge_limit));
    ul_offset += OAL_SIZEOF(g_st_customize.ast_band_edge_limit);
    oal_memcopy(&g_st_customize.st_rf_reg, puc_param + ul_offset, OAL_SIZEOF(g_st_customize.st_rf_reg));
    ul_offset += OAL_SIZEOF(g_st_customize.st_rf_reg);

#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1151)
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "dmac_config_set_cus_dts_cali: mac_device is null.device_id[%d]", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_device = (hal_device_stru *)pst_mac_device->pst_device_stru;
    /* ????PA??PHY?????? */
    hal_disable_machw_phy_and_pa(&pst_device->st_hal_device_base);
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)

    /* ??????PHY */
    //hal_initialize_phy(&pst_device->st_hal_device_base);
    hal_set_rf_custom_reg(&pst_device->st_hal_device_base);
#endif
    /* FPGA zhangyu Debug */
    /* ??????RF???? */

    hal_initialize_rf_sys(&pst_device->st_hal_device_base);

#endif
    SDIO_SendMsgSync(D2H_MSG_WLAN_READY);

    return OAL_SUCC;
}

oal_void dmac_config_update_rate_pow_table(oal_void)
{
    hal_cfg_customize_nvram_params_stru* past_src = g_st_customize.ast_nvram_params;

    /* update 24g:pauc_dst[][WLAN_BAND_2G] */
    /*11b 0~3*/
    /*    1 Mb */   g_auc_rate_pow_table_margin[0][0] = past_src[0].uc_max_txpower;
    /*    2 Mb */   g_auc_rate_pow_table_margin[1][0] = past_src[0].uc_max_txpower;
    /*  5.5 Mb */   g_auc_rate_pow_table_margin[2][0] = past_src[1].uc_max_txpower;
    /*   11 Mb */   g_auc_rate_pow_table_margin[3][0] = past_src[1].uc_max_txpower;

    /*11g 4~11*/
    /*    6 Mb */   g_auc_rate_pow_table_margin[4][0] = past_src[2].uc_max_txpower;
    /*    9 Mb */   g_auc_rate_pow_table_margin[5][0] = past_src[2].uc_max_txpower;
    /*   12 Mb */   g_auc_rate_pow_table_margin[6][0] = past_src[3].uc_max_txpower;
    /*   18 Mb */   g_auc_rate_pow_table_margin[7][0] = past_src[3].uc_max_txpower;
    /*   24 Mb */   g_auc_rate_pow_table_margin[8][0] = past_src[4].uc_max_txpower;
    /*   36 Mb */   g_auc_rate_pow_table_margin[9][0] = past_src[4].uc_max_txpower;
    /*   48 Mb */   g_auc_rate_pow_table_margin[10][0] = past_src[5].uc_max_txpower;
    /*   54 Mb */   g_auc_rate_pow_table_margin[11][0] = past_src[6].uc_max_txpower;

    /*11n&11ac20M 12~20*/
    /*  6.5 Mb */   g_auc_rate_pow_table_margin[12][0] = past_src[7].uc_max_txpower;
    /*   13 Mb */   g_auc_rate_pow_table_margin[13][0] = past_src[7].uc_max_txpower;
    /* 19.5 Mb */   g_auc_rate_pow_table_margin[14][0] = past_src[8].uc_max_txpower;
    /*   26 Mb */   g_auc_rate_pow_table_margin[15][0] = past_src[8].uc_max_txpower;
    /*   39 Mb */   g_auc_rate_pow_table_margin[16][0] = past_src[9].uc_max_txpower;
    /*   52 Mb */   g_auc_rate_pow_table_margin[17][0] = past_src[9].uc_max_txpower;
    /* 58.5 Mb */   g_auc_rate_pow_table_margin[18][0] = past_src[10].uc_max_txpower;
    /*   65 Mb */   g_auc_rate_pow_table_margin[19][0] = past_src[11].uc_max_txpower;

    /*   mcs8 */    g_auc_rate_pow_table_margin[20][0] = past_src[12].uc_max_txpower;

    /*11n&11ac40M 21~30*/
    /* 13.5 Mb */   g_auc_rate_pow_table_margin[21][0] = past_src[13].uc_max_txpower;
    /*   27 Mb */   g_auc_rate_pow_table_margin[22][0] = past_src[13].uc_max_txpower;
    /* 40.5 Mb */   g_auc_rate_pow_table_margin[23][0] = past_src[14].uc_max_txpower;
    /*   54 Mb */   g_auc_rate_pow_table_margin[24][0] = past_src[14].uc_max_txpower;
    /*   81 Mb */   g_auc_rate_pow_table_margin[25][0] = past_src[15].uc_max_txpower;
    /*  108 Mb */   g_auc_rate_pow_table_margin[26][0] = past_src[15].uc_max_txpower;
    /*121.5 Mb */   g_auc_rate_pow_table_margin[27][0] = past_src[16].uc_max_txpower;
    /*  135 Mb */   g_auc_rate_pow_table_margin[28][0] = past_src[17].uc_max_txpower;
    /*  162 Mb */   g_auc_rate_pow_table_margin[29][0] = past_src[18].uc_max_txpower;
    /*  180 Mb */   g_auc_rate_pow_table_margin[30][0] = past_src[19].uc_max_txpower;

    /*  MCS32  31*/
    /*    6 Mb */   g_auc_rate_pow_table_margin[31][0] = past_src[20].uc_max_txpower;

    /* update 5g:pauc_dst[][WLAN_BAND_5G] */
    /*11g 4~11*/
    /*    6 Mb */   g_auc_rate_pow_table_margin[4][1] = past_src[21].uc_max_txpower;
    /*    9 Mb */   g_auc_rate_pow_table_margin[5][1] = past_src[21].uc_max_txpower;
    /*   12 Mb */   g_auc_rate_pow_table_margin[6][1] = past_src[22].uc_max_txpower;
    /*   18 Mb */   g_auc_rate_pow_table_margin[7][1] = past_src[22].uc_max_txpower;
    /*   24 Mb */   g_auc_rate_pow_table_margin[8][1] = past_src[23].uc_max_txpower;
    /*   36 Mb */   g_auc_rate_pow_table_margin[9][1] = past_src[23].uc_max_txpower;
    /*   48 Mb */   g_auc_rate_pow_table_margin[10][1] = past_src[24].uc_max_txpower;
    /*   54 Mb */   g_auc_rate_pow_table_margin[11][1] = past_src[25].uc_max_txpower;

    /*11n&11ac20M 12~20*/
    /*  6.5 Mb */   g_auc_rate_pow_table_margin[12][1] = past_src[26].uc_max_txpower;
    /*   13 Mb */   g_auc_rate_pow_table_margin[13][1] = past_src[26].uc_max_txpower;
    /* 19.5 Mb */   g_auc_rate_pow_table_margin[14][1] = past_src[27].uc_max_txpower;
    /*   26 Mb */   g_auc_rate_pow_table_margin[15][1] = past_src[27].uc_max_txpower;
    /*   39 Mb */   g_auc_rate_pow_table_margin[16][1] = past_src[28].uc_max_txpower;
    /*   52 Mb */   g_auc_rate_pow_table_margin[17][1] = past_src[28].uc_max_txpower;
    /* 58.5 Mb */   g_auc_rate_pow_table_margin[18][1] = past_src[29].uc_max_txpower;
    /*   65 Mb */   g_auc_rate_pow_table_margin[19][1] = past_src[30].uc_max_txpower;

    /*   mcs8 */    g_auc_rate_pow_table_margin[20][1] = past_src[31].uc_max_txpower;

    /*11n&11ac40M 21~30*/
    /* 13.5 Mb */   g_auc_rate_pow_table_margin[21][1] = past_src[32].uc_max_txpower;
    /*   27 Mb */   g_auc_rate_pow_table_margin[22][1] = past_src[32].uc_max_txpower;
    /* 40.5 Mb */   g_auc_rate_pow_table_margin[23][1] = past_src[33].uc_max_txpower;
    /*   54 Mb */   g_auc_rate_pow_table_margin[24][1] = past_src[33].uc_max_txpower;
    /*   81 Mb */   g_auc_rate_pow_table_margin[25][1] = past_src[34].uc_max_txpower;
    /*  108 Mb */   g_auc_rate_pow_table_margin[26][1] = past_src[34].uc_max_txpower;
    /*121.5 Mb */   g_auc_rate_pow_table_margin[27][1] = past_src[35].uc_max_txpower;
    /*  135 Mb */   g_auc_rate_pow_table_margin[28][1] = past_src[36].uc_max_txpower;
    /*  162 Mb */   g_auc_rate_pow_table_margin[29][1] = past_src[37].uc_max_txpower;
    /*  180 Mb */   g_auc_rate_pow_table_margin[30][1] = past_src[37].uc_max_txpower;

    /*  MCS32  31*/
    /*    6 Mb */   g_auc_rate_pow_table_margin[31][1] = past_src[38].uc_max_txpower;

    /*11ac80M  32~41*/
    /* 29.3 Mb */   g_auc_rate_pow_table_margin[32][1] = past_src[39].uc_max_txpower;
    /* 58.5 Mb */   g_auc_rate_pow_table_margin[33][1] = past_src[39].uc_max_txpower;
    /* 87.8 Mb */   g_auc_rate_pow_table_margin[34][1] = past_src[40].uc_max_txpower;
    /*  117 Mb */   g_auc_rate_pow_table_margin[35][1] = past_src[40].uc_max_txpower;
    /*175.5 Mb */   g_auc_rate_pow_table_margin[36][1] = past_src[41].uc_max_txpower;
    /*  234 Mb */   g_auc_rate_pow_table_margin[37][1] = past_src[41].uc_max_txpower;
    /*263.3 Mb */   g_auc_rate_pow_table_margin[38][1] = past_src[42].uc_max_txpower;
    /*292.5 Mb */   g_auc_rate_pow_table_margin[39][1] = past_src[43].uc_max_txpower;
    /*  351 Mb */   g_auc_rate_pow_table_margin[40][1] = past_src[44].uc_max_txpower;
    /*  390 Mb */   g_auc_rate_pow_table_margin[41][1] = past_src[44].uc_max_txpower;
}

oal_void dmac_config_update_scaling_reg(oal_uint8 uc_device_id)
{
    mac_device_stru         *pst_mac_device;

    pst_mac_device = mac_res_get_dev(uc_device_id);
    if ((OAL_PTR_NULL == pst_mac_device) || (OAL_PTR_NULL == pst_mac_device->pst_device_stru))
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{dmac_config_update_scaling_reg::get mac_device is null. device_id[%d]}", uc_device_id);
        return;
    }

    hal_update_scaling_reg(pst_mac_device->pst_device_stru, (oal_void *)&g_st_customize.ast_nvram_params);
#if 0
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV)
    /* 11b DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_SCALING_VALUE_11B_REG,
                        OAL_JOIN_WORD32(past_src[0].uc_dbb_scale, past_src[0].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));

    /* 11g DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11G_REG,
                        OAL_JOIN_WORD32(past_src[2].uc_dbb_scale, past_src[2].uc_dbb_scale, past_src[3].uc_dbb_scale, past_src[3].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11G_REG,
                        OAL_JOIN_WORD32(past_src[4].uc_dbb_scale, past_src[4].uc_dbb_scale, past_src[5].uc_dbb_scale, past_src[6].uc_dbb_scale));

    /* 11n 2G HT20 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11N_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[7].uc_dbb_scale, past_src[7].uc_dbb_scale, past_src[8].uc_dbb_scale, past_src[8].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11N_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[9].uc_dbb_scale, past_src[9].uc_dbb_scale, past_src[10].uc_dbb_scale, past_src[11].uc_dbb_scale));

    /* 11n 2G HT40 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11N40M_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[13].uc_dbb_scale, past_src[13].uc_dbb_scale, past_src[14].uc_dbb_scale, past_src[14].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11N40M_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[15].uc_dbb_scale, past_src[15].uc_dbb_scale, past_src[16].uc_dbb_scale, past_src[17].uc_dbb_scale));

    /* 11a DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11A_REG,
                        OAL_JOIN_WORD32(past_src[21].uc_dbb_scale, past_src[21].uc_dbb_scale, past_src[22].uc_dbb_scale, past_src[22].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11A_REG,
                        OAL_JOIN_WORD32(past_src[23].uc_dbb_scale, past_src[23].uc_dbb_scale, past_src[24].uc_dbb_scale, past_src[25].uc_dbb_scale));

    /* 11n 5G HT20 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U0_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[31].uc_dbb_scale, 0x00, 0x00, 0x00));
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[26].uc_dbb_scale, past_src[26].uc_dbb_scale, past_src[27].uc_dbb_scale, past_src[27].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[28].uc_dbb_scale, past_src[28].uc_dbb_scale, past_src[29].uc_dbb_scale, past_src[30].uc_dbb_scale));

    /* 11n 5G HT40 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[32].uc_dbb_scale, past_src[32].uc_dbb_scale, past_src[33].uc_dbb_scale, past_src[33].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[34].uc_dbb_scale, past_src[34].uc_dbb_scale, past_src[35].uc_dbb_scale, past_src[36].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U3_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[37].uc_dbb_scale, past_src[37].uc_dbb_scale, 0x00, 0x00));

    /* 11ac 5G HT80 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U1_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[39].uc_dbb_scale, past_src[39].uc_dbb_scale, 0x00, 0x00));
    HI1102_REG_WRITE32(HI1102_PHY_U2_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[40].uc_dbb_scale, past_src[40].uc_dbb_scale, past_src[41].uc_dbb_scale, past_src[41].uc_dbb_scale));
    HI1102_REG_WRITE32(HI1102_PHY_U3_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[42].uc_dbb_scale, past_src[43].uc_dbb_scale, past_src[44].uc_dbb_scale, past_src[44].uc_dbb_scale));

    /* 11n 2G&5G HT40 MCS32 DBB scaling */
    HI1102_REG_WRITE32(HI1102_PHY_U3_SCALING_VALUE_11N40M_REG,
                        OAL_JOIN_WORD32(past_src[38].uc_dbb_scale, past_src[20].uc_dbb_scale, 0x00, 0x00));
#endif
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV)
    /* 11b DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_SCALING_VALUE_11B_REG,
                        OAL_JOIN_WORD32(past_src[0].uc_dbb_scale, past_src[0].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));

    /* 11g DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11G_REG,
                        OAL_JOIN_WORD32(past_src[2].uc_dbb_scale, past_src[2].uc_dbb_scale, past_src[3].uc_dbb_scale, past_src[3].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11G_REG,
                        OAL_JOIN_WORD32(past_src[4].uc_dbb_scale, past_src[4].uc_dbb_scale, past_src[5].uc_dbb_scale, past_src[6].uc_dbb_scale));

    /* 11n 2G HT20 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11N_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[7].uc_dbb_scale, past_src[7].uc_dbb_scale, past_src[8].uc_dbb_scale, past_src[8].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11N_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[9].uc_dbb_scale, past_src[9].uc_dbb_scale, past_src[10].uc_dbb_scale, past_src[11].uc_dbb_scale));

    /* 11n 2G HT40 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11N40M_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[13].uc_dbb_scale, past_src[13].uc_dbb_scale, past_src[14].uc_dbb_scale, past_src[14].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11N40M_2D4G_REG,
                        OAL_JOIN_WORD32(past_src[15].uc_dbb_scale, past_src[15].uc_dbb_scale, past_src[16].uc_dbb_scale, past_src[17].uc_dbb_scale));

    /* 11a DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11A_REG,
                        OAL_JOIN_WORD32(past_src[21].uc_dbb_scale, past_src[21].uc_dbb_scale, past_src[22].uc_dbb_scale, past_src[22].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11A_REG,
                        OAL_JOIN_WORD32(past_src[23].uc_dbb_scale, past_src[23].uc_dbb_scale, past_src[24].uc_dbb_scale, past_src[25].uc_dbb_scale));

    /* 11n 5G HT20 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U0_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[31].uc_dbb_scale, 0x00, 0x00, 0x00));
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[26].uc_dbb_scale, past_src[26].uc_dbb_scale, past_src[27].uc_dbb_scale, past_src[27].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11N_5G_REG,
                        OAL_JOIN_WORD32(past_src[28].uc_dbb_scale, past_src[28].uc_dbb_scale, past_src[29].uc_dbb_scale, past_src[30].uc_dbb_scale));

    /* 11n 5G HT40 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[32].uc_dbb_scale, past_src[32].uc_dbb_scale, past_src[33].uc_dbb_scale, past_src[33].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[34].uc_dbb_scale, past_src[34].uc_dbb_scale, past_src[35].uc_dbb_scale, past_src[36].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U3_SCALING_VALUE_11N40M_5G_REG,
                        OAL_JOIN_WORD32(past_src[37].uc_dbb_scale, past_src[37].uc_dbb_scale, 0x00, 0x00));

    /* 11ac 5G HT80 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U1_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[39].uc_dbb_scale, past_src[39].uc_dbb_scale, 0x00, 0x00));
    HI1103_REG_WRITE32(HI1103_PHY_U2_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[40].uc_dbb_scale, past_src[40].uc_dbb_scale, past_src[41].uc_dbb_scale, past_src[41].uc_dbb_scale));
    HI1103_REG_WRITE32(HI1103_PHY_U3_SCALING_VALUE_11AC_REG,
                        OAL_JOIN_WORD32(past_src[42].uc_dbb_scale, past_src[43].uc_dbb_scale, past_src[44].uc_dbb_scale, past_src[44].uc_dbb_scale));

    /* 11n 2G&5G HT40 MCS32 DBB scaling */
    HI1103_REG_WRITE32(HI1103_PHY_U3_SCALING_VALUE_11N40M_REG,
                        OAL_JOIN_WORD32(past_src[38].uc_dbb_scale, past_src[20].uc_dbb_scale, 0x00, 0x00));
#endif
#endif
}


oal_void dmac_config_update_dsss_scaling_reg(dmac_alg_tpc_user_distance_enum_uint8 en_dmac_device_distance_enum)
{
    hal_cfg_customize_nvram_params_stru* past_src = g_st_customize.ast_nvram_params;

    /* ??????DSSS SCALE PROMOTE??????????????????????????????SAR(sar????????????) */
    if (
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        (0 == g_st_customize.st_rf.uc_far_dist_dsss_scale_promote_switch) ||
#endif
        (g_uc_sar_pwr_limit < HAL_SAR_PWR_LIMIT_THRESHOLD))
    {
        return;
    }

    /* 1_2M_scaling????????????5.5_11M_scaling???????????????? */
    if (past_src[0].uc_dbb_scale >= past_src[1].uc_dbb_scale)
    {
        return;
    }

    /* ????????????????????1_2M_scaling????5.5_11M_scaling???????? */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV)
    if (DMAC_ALG_TPC_FAR_DISTANCE == en_dmac_device_distance_enum)
    {
        HI1102_REG_WRITE32(HI1102_PHY_SCALING_VALUE_11B_REG,
                            OAL_JOIN_WORD32(past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));
    }
    else
    {
        HI1102_REG_WRITE32(HI1102_PHY_SCALING_VALUE_11B_REG,
                            OAL_JOIN_WORD32(past_src[0].uc_dbb_scale, past_src[0].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));
    }
#endif
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV)
    if (DMAC_ALG_TPC_FAR_DISTANCE == en_dmac_device_distance_enum)
    {
        HI1103_REG_WRITE32(HI1103_PHY_SCALING_VALUE_11B_REG,
                            OAL_JOIN_WORD32(past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));
    }
    else
    {
        HI1103_REG_WRITE32(HI1103_PHY_SCALING_VALUE_11B_REG,
                            OAL_JOIN_WORD32(past_src[0].uc_dbb_scale, past_src[0].uc_dbb_scale, past_src[1].uc_dbb_scale, past_src[1].uc_dbb_scale));
    }
#endif
}



oal_uint32 dmac_config_set_cus_nvram_params(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_set_cus_nvram_params:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_memcopy(&g_st_customize.ast_nvram_params, puc_param, OAL_SIZEOF(g_st_customize.ast_nvram_params));
    g_st_customize.uc_dpd_enable = (oal_uint8)*(puc_param + OAL_SIZEOF(g_st_customize.ast_nvram_params));

    /* close dpd alg */
    //pst_mac_vap->st_cap_flag.bit_dpd_enbale = g_st_customize.uc_dpd_enable;

    /* update rate_pow_table */
    dmac_config_update_rate_pow_table();
    /* update phy scaling reg */
    dmac_config_update_scaling_reg(pst_mac_vap->uc_device_id);

    return OAL_SUCC;
}

oal_uint32 dmac_config_customize_info(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    oal_uint8 uc_loop;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{dmac_config_customize_info:: pointer is null,pst_mac_vap[0x%x], puc_param[0x%x] .}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* ????????:???????????????? */
    OAM_WARNING_LOG2(0, OAM_SF_CFG, "[CUSTOMIZE::[dpd:%d][11ac2g_cap_bit:%d].]", g_st_customize.uc_dpd_enable, pst_mac_vap->st_cap_flag.bit_11ac2g);

    /* ????nvram ???? */
    for (uc_loop = 0; uc_loop < NUM_OF_NV_MAX_TXPOWER; uc_loop++)
    {
        OAM_WARNING_LOG3(0, OAM_SF_CFG, "[CUSTOMIZE] nvram idx %02d ---- tx_power 0x%02X ---- dbbscale 0x%02X",
                        uc_loop, g_st_customize.ast_nvram_params[uc_loop].uc_max_txpower, g_st_customize.ast_nvram_params[uc_loop].uc_dbb_scale);
    }

    /* ????FCC ?????????????? */
    for (uc_loop = 0; uc_loop < NUM_OF_BAND_EDGE_LIMIT; uc_loop++)
    {
        OAM_WARNING_LOG3(0, OAM_SF_CFG, "[CUSTOMIZE] fcc_edge_band idx %02d ---- tx_power 0x%03d ---- dbbscale 0x%02X",
                        uc_loop, g_st_customize.ast_band_edge_limit[uc_loop].uc_max_txpower, g_st_customize.ast_band_edge_limit[uc_loop].uc_dbb_scale);
    }

    /* ????CE ??band ?????????? */
    OAM_WARNING_LOG4(0, OAM_SF_CFG, "[CUSTOMIZE] ce_high_band tx_power %d, dbbscale 20MHz 0x%X, 40MHz 0x%X, 80MHz 0x%X",
                    g_st_customize.st_ce_5g_hi_band_params.uc_max_txpower,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_11a_ht20_vht20,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_ht40_vht40,
                    g_st_customize.st_ce_5g_hi_band_params.uc_dbb_scale_vht80);

    return OAL_SUCC;
}
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

/* ???????????????????? */
OAL_STATIC oal_uint32 dmac_config_rx_filter_frag(mac_vap_stru *pst_mac_vap, oal_uint8 uc_len, oal_uint8 *puc_param)
{
    g_rx_filter_frag = !!(*puc_param);
    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{dmac_config_rx_filter_frag::filter frag[%d].}",
        g_rx_filter_frag);
    return OAL_SUCC;
}

/*****************************************************************************
    HMAC??DMAC??????????????????????
*****************************************************************************/
OAL_STATIC OAL_CONST dmac_config_syn_stru g_ast_dmac_config_syn[] =
{
    /* ????ID                    ????2??????            ???????? */
    {WLAN_CFGID_BSS_TYPE,           {0, 0},         dmac_config_set_bss_type},
    {WLAN_CFGID_ADD_VAP,            {0, 0},         dmac_config_add_vap},
    {WLAN_CFGID_START_VAP,          {0, 0},         dmac_config_start_vap},
    {WLAN_CFGID_DEL_VAP,            {0, 0},         dmac_config_del_vap},
    {WLAN_CFGID_DOWN_VAP,           {0, 0},         dmac_config_down_vap},
    {WLAN_CFGID_MODE,               {0, 0},         dmac_config_set_mode},
    {WLAN_CFGID_CURRENT_CHANEL,     {0, 0},         dmac_config_set_freq},
    {WLAN_CFGID_STATION_ID,         {0, 0},         dmac_config_set_mac_addr},
    {WLAN_CFGID_CONCURRENT,         {0, 0},         dmac_config_set_concurrent},
    {WLAN_CFGID_SSID,               {0, 0},         dmac_config_set_ssid},
#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL
    {WLAN_CFGID_GET_HIPKT_STAT,     {0, 0},         dmac_config_get_hipkt_stat},
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    {WLAN_CFGID_SHORTGI,            {0, 0},         dmac_config_set_shortgi},   /* hi1102-cb add sync */
    {WLAN_CFGID_VAP_STATE_SYN,      {0, 0},         dmac_config_vap_state_syn},
    {WLAN_CFGID_REGDOMAIN_PWR,      {0, 0},         dmac_config_set_regdomain_pwr},
#endif
    {WLAN_CFGID_SCAN_ABORT,         {0, 0},         dmac_config_scan_abort},
    {WLAN_CFGID_STOP_SCHED_SCAN,    {0, 0},         dmac_config_stop_sched_scan},
    {WLAN_CFGID_USER_ASOC_STATE_SYN,{0, 0},         dmac_config_user_asoc_state_syn},
    {WLAN_CFGID_USER_RATE_SYN,      {0, 0},         dmac_config_user_rate_info_syn},
    {WLAN_CFGID_USR_INFO_SYN,       {0, 0},         dmac_config_sta_usr_info_syn},
    {WLAN_CFGID_STA_VAP_INFO_SYN,   {0, 0},         dmac_config_vap_info_syn},

#ifdef _PRE_WLAN_FEATURE_TXOPPS
    {WLAN_CFGID_STA_TXOP_AID,         {0, 0},      dmac_txopps_set_machw_partialaid_sta},
#endif

    {WLAN_CFGID_SHORT_PREAMBLE,     {0, 0},         dmac_config_set_shpreamble},
#ifdef _PRE_WLAN_FEATURE_MONITOR
    {WLAN_CFGID_ADDR_FILTER,        {0, 0},         dmac_config_set_addr_filter},
#endif
    {WLAN_CFGID_PROT_MODE,          {0, 0},         dmac_config_set_prot_mode},
    {WLAN_CFGID_BEACON_INTERVAL,    {0, 0},         dmac_config_set_bintval},
    {WLAN_CFGID_NO_BEACON,          {0, 0},         dmac_config_set_nobeacon},
    {WLAN_CFGID_TX_CHAIN,           {0, 0},         dmac_config_set_txchain},
    {WLAN_CFGID_RX_CHAIN,           {0, 0},         dmac_config_set_rxchain},
    {WLAN_CFGID_TX_POWER,           {0, 0},         dmac_config_set_txpower},
    {WLAN_CFGID_DTIM_PERIOD,        {0, 0},         dmac_config_set_dtimperiod},
#if 0
    {WLAN_CFGID_OTA_SWITCH,         {0, 0},         dmac_config_ota_switch},
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    {WLAN_CFGID_WFA_CFG_AIFSN,         {0, 0},      dmac_config_wfa_cfg_aifsn},
    {WLAN_CFGID_WFA_CFG_CW,         {0, 0},         dmac_config_wfa_cfg_cw},
    {WLAN_CFGID_CHECK_LTE_GPIO,         {0, 0},     dmac_config_lte_gpio_mode},
#endif /* DMAC_OFFLOAD */


#ifdef _PRE_WLAN_FEATURE_UAPSD
    {WLAN_CFGID_UAPSD_EN,           {0, 0},         dmac_config_set_uapsden},
    {WLAN_CFGID_UAPSD_UPDATE,       {0, 0},         dmac_config_set_uapsd_update},
#endif
    {WLAN_CFGID_EDCA_TABLE_CWMIN,           {0, 0},         dmac_config_set_cwmin},
    {WLAN_CFGID_EDCA_TABLE_CWMAX,           {0, 0},         dmac_config_set_cwmax},
    {WLAN_CFGID_EDCA_TABLE_AIFSN,           {0, 0},         dmac_config_set_aifsn},
    {WLAN_CFGID_EDCA_TABLE_TXOP_LIMIT,      {0, 0},         dmac_config_set_txop_limit},
    {WLAN_CFGID_EDCA_TABLE_MSDU_LIFETIME,   {0, 0},         dmac_config_set_msdu_lifetime},
    {WLAN_CFGID_EDCA_TABLE_MANDATORY,       {0, 0},         dmac_config_set_edca_mandatory},
    {WLAN_CFGID_QEDCA_TABLE_CWMIN,          {0, 0},         dmac_config_set_qap_cwmin},
    {WLAN_CFGID_QEDCA_TABLE_CWMAX,          {0, 0},         dmac_config_set_qap_cwmax},
    {WLAN_CFGID_QEDCA_TABLE_AIFSN,          {0, 0},         dmac_config_set_qap_aifsn},
    {WLAN_CFGID_QEDCA_TABLE_TXOP_LIMIT,     {0, 0},         dmac_config_set_qap_txop_limit},
    {WLAN_CFGID_QEDCA_TABLE_MSDU_LIFETIME,  {0, 0},         dmac_config_set_qap_msdu_lifetime},
    {WLAN_CFGID_QEDCA_TABLE_MANDATORY,      {0, 0},         dmac_config_set_qap_edca_mandatory},

    {WLAN_CFGID_VAP_INFO,                   {0, 0},         dmac_config_vap_info},
    {WLAN_CFGID_ADD_USER,                   {0, 0},         dmac_config_add_user},
    {WLAN_CFGID_DEL_USER,                   {0, 0},         dmac_config_del_user},
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    {WLAN_CFGID_BTCOEX_STATUS_PRINT,        {0, 0},         dmac_config_print_btcoex_status},
#endif
#ifdef _PRE_WLAN_FEATURE_LTECOEX
    {WLAN_CFGID_LTECOEX_MODE_SET,           {0, 0},         dmac_config_ltecoex_mode_set},
#endif

    {WLAN_CFGID_SET_LOG_LEVEL,              {0, 0},         dmac_config_set_log_level},
#ifdef _PRE_WLAN_FEATURE_GREEN_AP
    {WLAN_CFGID_GREEN_AP_EN,                {0, 0},         dmac_config_set_green_ap_en},
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    {WLAN_CFGID_SET_LOG_PM,                 {0, 0},         dmac_config_set_log_lowpower},
    {WLAN_CFGID_SET_PM_SWITCH,              {0, 0},         dmac_config_set_pm_switch},
#ifdef _PRE_WLAN_FEATURE_SMARTANT
    {WLAN_CFGID_SET_ANT,              {0, 0},         dmac_config_dual_antenna_set_ant},
    {WLAN_CFGID_GET_ANT_INFO,         {0, 0},         dmac_config_get_ant_info},
    {WLAN_CFGID_DOUBLE_ANT_SW,        {0, 0},         dmac_config_double_ant_switch},
#endif
    {WLAN_CFGID_GET_ANT,              {0, 0},         dmac_config_get_ant},
#endif
    /*wpa-wpa2*/
    {WLAN_CFGID_ADD_KEY,           {0, 0},              dmac_config_11i_add_key},
	//{WLAN_CFGID_ADD_KEY_KEY,       {0, 0},              dmac_config_11i_add_key},
    //{WLAN_CFGID_ADD_KEY_SEQ,       {0, 0},              dmac_config_11i_add_key_seq},
    {WLAN_CFGID_REMOVE_KEY,        {0, 0},              dmac_config_11i_remove_key},
    {WLAN_CFGID_DEFAULT_KEY,       {0, 0},              dmac_config_11i_set_default_key},
    //{WLAN_CFGID_ADD_WEP_KEY,       {0, 0},              dmac_config_wep_add_key},
    {WLAN_CFGID_ADD_WEP_ENTRY,     {0, 0},              dmac_config_wep_add_entry},
    {WLAN_CFGID_REMOVE_WEP_KEY,    {0, 0},              dmac_config_wep_remove_key},
	{WLAN_CFGID_CONNECT_REQ,        {0, 0},             dmac_config_connect},

#ifdef _PRE_WLAN_FEATURE_WAPI
    {WLAN_CFGID_ADD_WAPI_KEY,       {0, 0},             dmac_config_wapi_add_key},
#endif

    {WLAN_CFGID_INIT_SECURTIY_PORT, {0, 0},             dmac_config_11i_init_port},
    {WLAN_CFGID_DUMP_ALL_RX_DSCR,  {0, 0},              dmac_config_dump_all_rx_dscr},
    {WLAN_CFGID_USER_INFO,         {0, 0},              dmac_config_user_info},
    {WLAN_CFGID_SET_DSCR,          {0, 0},              dmac_config_set_dscr},
    {WLAN_CFGID_COUNTRY,           {0, 0},              dmac_config_set_country},
    {WLAN_CFGID_SET_RANDOM_MAC_OUI, {0, 0},             dmac_config_set_random_mac_oui},
#ifdef _PRE_WLAN_FEATURE_DFS
    {WLAN_CFGID_COUNTRY_FOR_DFS,   {0, 0},              dmac_config_set_country_for_dfs},
#endif
#if defined (_PRE_WLAN_CHIP_TEST) || defined (_PRE_WLAN_FEATURE_ALWAYS_TX)
    {WLAN_CFGID_SET_RATE,          {0, 0},              dmac_config_set_rate},
    {WLAN_CFGID_SET_MCS,           {0, 0},              dmac_config_set_mcs},
    {WLAN_CFGID_SET_MCSAC,         {0, 0},              dmac_config_set_mcsac},
    {WLAN_CFGID_SET_BW,            {0, 0},              dmac_config_set_bw},
#endif

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    {WLAN_CFGID_SET_ALWAYS_TX_1102,{0, 0},              dmac_config_set_always_tx_1102},
#endif
    {WLAN_CFGID_SET_ALWAYS_RX,     {0, 0},              dmac_config_set_always_rx},
    {WLAN_CFGID_RX_FILTER_FRAG,    {0, 0},              dmac_config_rx_filter_frag},
    {WLAN_CFGID_REG_INFO,          {0, 0},              dmac_config_reg_info},

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
    {WLAN_CFGID_SDIO_FLOWCTRL,     {0, 0},              dmac_config_sdio_flowctrl},
#endif
    {WLAN_CFGID_REG_WRITE,         {0, 0},              dmac_config_reg_write},
    {WLAN_CFGID_CFG80211_SET_CHANNEL,      {0, 0},      dmac_config_set_channel},

    {WLAN_CFGID_CFG80211_CONFIG_BEACON,    {0, 0},      dmac_config_set_beacon},

#if 0
    {WLAN_CFGID_TDLS_PROHI,                {0, 0},      dmac_config_tdls_prohibited},
    {WLAN_CFGID_TDLS_CHASWI_PROHI,         {0, 0},      dmac_config_tdls_channel_switch_prohibited},
#endif
    {WLAN_CFGID_WMM_SWITCH,                {0, 0},      dmac_config_wmm_switch},

#ifdef _PRE_SUPPORT_ACS
    {WLAN_CFGID_ACS_PARAM,          {0, 0},             dmac_acs_recv_msg},
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
    {WLAN_CFGID_RADARTOOL,          {0, 0},             dmac_config_dfs_radartool},
#endif

    {WLAN_CFGID_SET_RTS_PARAM,      {0, 0},             dmac_config_set_rts_param},

    {WLAN_CFGID_UPDTAE_PROT_TX_PARAM,{0, 0},            dmac_config_update_protection_tx_param},
    {WLAN_CFGID_SET_PROTECTION,      {0, 0},            dmac_config_set_protection},

    {WLAN_CFGID_GET_VERSION,         {0, 0},            dmac_config_get_version},
    {WLAN_CFGID_RX_FCS_INFO,         {0, 0},            dmac_config_rx_fcs_info},

#ifdef _PRE_WLAN_FEATURE_SMPS
    {WLAN_CFGID_SET_SMPS,            {0, 0},            dmac_config_set_smps_mode},
#endif

    {WLAN_CFGID_SET_WPS_P2P_IE,    {0, 0},              dmac_config_set_app_ie_to_vap},
    {WLAN_CFGID_CFG80211_CANCEL_REMAIN_ON_CHANNEL,  {0, 0},  dmac_config_cancel_remain_on_channel},
#ifdef _PRE_WLAN_FEATURE_STA_PM
    {WLAN_CFGID_SET_PS_MODE,         {0, 0},                 dmac_config_set_sta_ps_mode},  /* hi1102 pspoll???????? */
#ifdef _PRE_PSM_DEBUG_MODE
    {WLAN_CFGID_SHOW_PS_INFO,        {0, 0},                 dmac_show_ps_info},/* sta psm ???????????? */
#endif
    {WLAN_CFGID_SET_PSM_PARAM,       {0, 0},                 dmac_set_psm_param},/* sta psm tbtt offset/listen interval ???? */
    {WLAN_CFGID_SET_STA_PM_ON,        {0, 0},                dmac_config_set_sta_pm_on},/* sta psm tbtt offset/listen interval ???? */

#ifdef _PRE_WLAN_FEATURE_STA_UAPSD
    {WLAN_CFGID_SET_UAPSD_PARA,        {0, 0},              dmac_config_set_uapsd_para},
#endif
#endif

    {WLAN_CFGID_QUERY_STATION_STATS, {0, 0},                dmac_config_proc_query_sta_info_event},
    {WLAN_CFGID_QUERY_RSSI,         {0, 0},                 dmac_config_query_rssi},
    {WLAN_CFGID_QUERY_RATE,         {0, 0},                 dmac_config_query_rate},

#ifdef _PRE_WLAN_DFT_STAT
    {WLAN_CFGID_QUERY_ANI,          {0, 0},                 dmac_config_query_ani},
#endif

#ifdef _PRE_WLAN_FEATURE_P2P
    {WLAN_CFGID_SET_P2P_PS_OPS,     {0, 0},                 dmac_config_set_p2p_ps_ops},
    {WLAN_CFGID_SET_P2P_PS_NOA,     {0, 0},                 dmac_config_set_p2p_ps_noa},
#endif

    {WLAN_CFGID_NSS,                {0, 0},             dmac_config_nss},
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    {WLAN_CFGID_PROXYSTA_SWITCH,     {0, 0},            dmac_config_proxysta_switch},
#endif
    {WLAN_CFGID_UPDATE_OPMODE,      {0, 0},             dmac_config_opmode},
    {WLAN_CFGID_USER_CAP_SYN,       {0, 0},             dmac_config_user_cap_syn},

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
    {WLAN_CFGID_SET_IP_ADDR,                {0, 0},     dmac_config_set_ip_addr},
#endif
#ifdef _PRE_WLAN_FEATURE_ROAM
    {WLAN_CFGID_SET_ROAMING_MODE,       {0, 0},         dmac_config_roam_enable},
    {WLAN_CFGID_SET_ROAM_TRIGGER,       {0, 0},         dmac_config_set_roam_trigger},
    {WLAN_CFGID_ROAM_HMAC_SYNC_DMAC,    {0, 0},         dmac_config_roam_hmac_sync_dmac},
    {WLAN_CFGID_ROAM_NOTIFY_STATE,      {0, 0},         dmac_config_roam_notify_state},
#endif //_PRE_WLAN_FEATURE_ROAM
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    {WLAN_CFGID_SET_TXRX_CHAIN,         {0, 0},         dmac_config_set_txrx_chain},
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    {WLAN_CFGID_SUSPEND_ACTION_SYN,        {0, 0},      dmac_config_suspend_action_sync},
#endif
#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    {WLAN_CFGID_SET_DEVICE_FREQ,   {0, 0},         dmac_config_set_device_freq},
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    {WLAN_CFGID_SET_BTCOEX_PS_SWITCH,     {0, 0},       dmac_config_set_btcoex_ps_switch},
#endif
    {WLAN_CFGID_SET_LINKLOSS_THRESHOLD,     {0, 0},       dmac_config_set_linkloss_threshold},
    {WLAN_CFGID_SET_ALL_LOG_LEVEL,          {0, 0},       dmac_config_set_all_log_level},
    {WLAN_CFGID_SET_D2H_HCC_ASSEMBLE_CNT,   {0, 0},       dmac_config_set_d2h_hcc_assemble_cnt},
    {WLAN_CFGID_SET_CHN_EST_CTRL,           {0, 0},       dmac_config_set_chn_est_ctrl},
    {WLAN_CFGID_SET_POWER_REF,              {0, 0},       dmac_config_set_power_ref},
    {WLAN_CFGID_SET_PM_CFG_PARAM,           {0, 0},       dmac_config_set_pm_cfg_param},
    {WLAN_CFGID_SET_CUS_RF,                 {0, 0},       dmac_config_set_cus_rf},
#ifdef _PRE_WLAN_DOWNLOAD_PM
    {WLAN_CFGID_SET_CUS_DOWNLOAD_RATE_LIMIT,{0, 0},       dmac_config_set_download_rate_limit},
#endif
    {WLAN_CFGID_SET_CUS_DTS_CALI,           {0, 0},       dmac_config_set_cus_dts_cali},
    {WLAN_CFGID_SET_CUS_NVRAM_PARAM,        {0, 0},       dmac_config_set_cus_nvram_params},
    /* show customize info */
    {WLAN_CFGID_SHOW_DEV_CUSTOMIZE_INFOS,   {0, 0},       dmac_config_customize_info},
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#ifdef _PRE_WLAN_FEATURE_VOWIFI
    {WLAN_CFGID_VOWIFI_INFO,                {0, 0},       dmac_config_vowifi_info},
#endif /* _PRE_WLAN_FEATURE_VOWIFI */
    {WLAN_CFGID_REDUCE_SAR,                 {0, 0},       dmac_config_reduce_sar},
#ifdef _PRE_WLAN_RF_CALI
    {WLAN_CFGID_AUTO_CALI,                  {0, 0},         dmac_config_auto_cali},
    {WLAN_CFGID_SET_CALI_VREF,              {0, 0},         dmac_config_set_cali_vref},
#endif

#ifdef _PRE_WLAN_FEATURE_11K
    {WLAN_CFGID_BCN_TABLE_SWITCH,           {0, 0},     dmac_config_bcn_table_switch},
#endif

    {WLAN_CFGID_VOE_ENABLE,                 {0, 0},     dmac_config_voe_enable},
    {WLAN_CFGID_BUTT,               {0, 0},             OAL_PTR_NULL},
};


oal_uint32 dmac_cali_to_hmac(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru                     *pst_event;
    frw_event_hdr_stru                 *pst_event_hdr;

    pst_event               = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event_hdr           = &(pst_event->st_event_hdr);

    //OAL_IO_PRINT("dmac_cali_to_hmac start\r\n");
    FRW_EVENT_HDR_MODIFY_PIPELINE_AND_SUBTYPE(pst_event_hdr, DMAC_MISC_SUB_TYPE_CALI_TO_HMAC);
    /* ???????? */
    frw_event_dispatch_event(pst_event_mem);

    return OAL_SUCC;
}

/*Get the cfgid entry*/
OAL_STATIC dmac_config_syn_stru* dmac_config_get_cfgid_map(dmac_config_syn_stru* pst_cfgid_map,
                                                    oal_uint16 en_cfgid,
                                                    oal_uint32 ul_cfgid_nums)
{
    oal_uint16 us_cfgid;
    dmac_config_syn_stru* pst_current_cfgid;

    for(us_cfgid = 0; us_cfgid < ul_cfgid_nums; us_cfgid++)
    {
        pst_current_cfgid = pst_cfgid_map + us_cfgid;
        if (pst_current_cfgid->en_cfgid == en_cfgid)
        {
            return pst_current_cfgid;
        }
    }

    return NULL;
}



oal_uint32  dmac_event_config_syn(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru             *pst_event;
    frw_event_hdr_stru         *pst_event_hdr;
    hmac_to_dmac_cfg_msg_stru  *pst_hmac2dmac_msg;
    mac_vap_stru               *pst_mac_vap;
    mac_device_stru            *pst_mac_device;
    oal_uint32                  ul_ret;
    dmac_config_syn_stru*       pst_cfgid_entry;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_event_mem))
    {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{dmac_event_config_syn::pst_event_mem null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event         = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event_hdr     = &(pst_event->st_event_hdr);
    pst_hmac2dmac_msg = (hmac_to_dmac_cfg_msg_stru *)pst_event->auc_event_data;

    //OAM_INFO_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::a dmac config syn event occur, cfg_id=%d.}", pst_hmac2dmac_msg->en_syn_id);
    /* ????dmac vap */
    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_event_hdr->uc_vap_id);

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::pst_mac_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????mac device */
    pst_mac_device = (mac_device_stru *)mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    if(OAL_TRUE == MAC_DEV_IS_RESET_IN_PROGRESS(pst_mac_device))
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::MAC_DEV_IS_RESET_IN_PROGRESS.}");

        return OAL_FAIL;
    }

    /* ????cfg id?????????????? */
    pst_cfgid_entry = dmac_config_get_cfgid_map((dmac_config_syn_stru*)g_ast_dmac_config_syn,
                                                pst_hmac2dmac_msg->en_syn_id,
                                                OAL_ARRAY_SIZE(g_ast_dmac_config_syn));
    if(NULL == pst_cfgid_entry)
    {
#ifdef _PRE_WLAN_CFGID_DEBUG
        pst_cfgid_entry = dmac_config_get_cfgid_map((dmac_config_syn_stru*)g_ast_dmac_config_syn_debug,
                                                pst_hmac2dmac_msg->en_syn_id,
                                                dmac_get_config_debug_arrysize());
        if(NULL == pst_cfgid_entry)
        {
            OAM_ERROR_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::invalid en_cfgid[%d].", pst_hmac2dmac_msg->en_syn_id);
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
#else
        OAM_ERROR_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::invalid en_cfgid[%d].", pst_hmac2dmac_msg->en_syn_id);
        return OAL_ERR_CODE_INVALID_CONFIG;
#endif
    }

    /* ??????????p_set_func ???? */
    if (NULL == pst_cfgid_entry->p_set_func)
    {
        OAM_ERROR_LOG1(pst_event_hdr->uc_vap_id, OAM_SF_CFG, "{dmac_event_config_syn::invalid p_set_func cfgid[%d].", pst_hmac2dmac_msg->en_syn_id);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* ???????????? */
    ul_ret = pst_cfgid_entry->p_set_func(pst_mac_vap, (oal_uint8)(pst_hmac2dmac_msg->us_len), (oal_uint8 *)pst_hmac2dmac_msg->auc_msg_body);
    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG2(pst_event_hdr->uc_vap_id, OAM_SF_CFG,
                         "{dmac_event_config_syn::p_set_func failed, ul_ret=%d en_syn_id=%d.", ul_ret, pst_hmac2dmac_msg->en_syn_id);
        return ul_ret;
    }

    return OAL_SUCC;
}
/*lint -e578*//*lint -e19*/

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    oal_module_symbol(dmac_config_set_qap_cwmin);
    oal_module_symbol(dmac_config_set_qap_cwmax);
    oal_module_symbol(dmac_config_set_qap_aifsn);
#endif
/*lint +e578*//*lint +e19*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

