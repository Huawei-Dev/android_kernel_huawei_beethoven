



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "wlan_spec.h"
#include "mac_board.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_C


/*****************************************************************************
  2 ????????????
*****************************************************************************/

/* HOST CRX???? */
frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT];

/* DMAC??????HOST_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT];

/* DMAC??????WLAN_DTX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT];

#ifndef _PRE_WLAN_PROFLING_MIPS
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif
#endif
/* DMAC??????WLAN_CTX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];

/* DMAC????,WLAN_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* DMAC??????ERROR_IRQ?????????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table[HAL_EVENT_ERROR_IRQ_SUB_TYPE_BUTT];

/* DMAC????,WLAN_CRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMAC??????TX_COMP???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_BUTT];

/* DMAC????, TBTT?????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tbtt_event_sub_table[HAL_EVENT_TBTT_SUB_TYPE_BUTT];

/*DMAC????, MISC?????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_misc_event_sub_table[HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT];

/* WLAN_DTX ???????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX_BUTT];

/* HMAC???? WLAN_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? WLAN_CRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? TBTT???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_tbtt_event_sub_table[DMAC_TBTT_EVENT_SUB_TYPE_BUTT];

/* HMAC???? ????HOST???????????????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT];

/* HMAC???? MISC???????????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT];

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
/* HMAC???? IPC???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_host_drx_event_sub_table[DMAC_TX_HOST_DRX_BUTT];
#endif


#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
frw_event_sub_table_item_stru g_ast_hmac_hcc_rx_event_sub_table[DMAC_HCC_RX_EVENT_SUB_TYPE_BUTT];

#ifndef _PRE_WLAN_PROFLING_MIPS
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif
#endif

#endif
/*****************************************************************************
DFX????????????????
*****************************************************************************/
#ifdef _PRE_WLAN_DFT_STAT
dfx_performance_log_switch_enum_uint8 g_auc_dfx_performance_log_switch[DFX_PERFORMANCE_LOG_BUTT] = {0};
#endif

/*****************************************************************************
  3 ????????
*****************************************************************************/



oal_void  event_fsm_table_register(oal_void)
{
    /* Part1: ??????Dmac????????*/

    /* ????DMAC????HOST_CRX???? */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_CRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_host_crx_table);

    /* ????DMAC????HOST_DRX?????????????? */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_DRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_tx_host_drx);

    /* ????DMAC????WLAN_DTX?????????????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_tx_wlan_dtx);

    /* ????DMAC????WLAN_CTX?????????????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CTX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_wlan_ctx_event_sub_table);

    /* ????DMAC????WLAN_DRX???????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_wlan_drx_event_sub_table);

    /* ????DMAC????WLAN_CRX????pipeline 0???? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_wlan_crx_event_sub_table);

    /* ????DMAC????TX_COMP???????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_TX_COMP, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_tx_comp_event_sub_table);

    /* ????DMAC????TBTT???????? */
    frw_event_table_register(FRW_EVENT_TYPE_TBTT, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_tbtt_event_sub_table);

    /* ????DMAC????ERR???????? */
    frw_event_table_register(FRW_EVENT_TYPE_HIGH_PRIO, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_high_prio_event_sub_table);

    /* ????DMAC????MISC???????? */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_misc_event_sub_table);

    /* Part2: ??????Hmac????????*/

    /* ????WLAN_DTX???????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_hmac_wlan_dtx_event_sub_table);

    /* ????WLAN_DRX???????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_drx_event_sub_table);

    /* ????HMAC????WLAN_CRX???????? */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_crx_event_sub_table);

     /* ????DMAC????MISC???????? */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_misc_event_sub_table);

    /* ????TBTT???????? */
    frw_event_table_register(FRW_EVENT_TYPE_TBTT, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_tbtt_event_sub_table);

    /* ???????????????????????? */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_SDT_REG, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_ctx_event_sub_table);
}


oal_void  event_fsm_unregister(oal_void)
{
    /* Part1: ??????Dmac????????*/

    /* ??????DMAC????HOST_CRX???? */
    OAL_MEMZERO(g_ast_dmac_host_crx_table, OAL_SIZEOF(g_ast_dmac_host_crx_table));

    /* ??????DMAC????HOST_DRX?????????????? */
    OAL_MEMZERO(g_ast_dmac_tx_host_drx, OAL_SIZEOF(g_ast_dmac_tx_host_drx));

    /* ??????DMAC????WLAN_DTX?????????????? */
    OAL_MEMZERO(g_ast_dmac_tx_wlan_dtx, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx));

    /* ??????DMAC????WLAN_DRX???????? */
    OAL_MEMZERO(g_ast_dmac_wlan_drx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table));

    /* ??????DMAC????WLAN_CRX???????? */
    OAL_MEMZERO(g_ast_dmac_wlan_crx_event_sub_table, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table));

    /* ??????DMAC????TX_COMP???????? */
    OAL_MEMZERO(g_ast_dmac_tx_comp_event_sub_table, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table));

    /* ??????DMAC????TBTT???????? */
    OAL_MEMZERO(g_ast_dmac_tbtt_event_sub_table, OAL_SIZEOF(g_ast_dmac_tbtt_event_sub_table));

    /* ??????DMAC????MAC_ERROR???????? */
    OAL_MEMZERO(g_ast_dmac_high_prio_event_sub_table, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table));

    /* ??????DMAC????MAC_ERROR???????? */
    OAL_MEMZERO(g_ast_dmac_misc_event_sub_table, OAL_SIZEOF(g_ast_dmac_misc_event_sub_table));

    /* Part2: ??????Hmac????????*/
    OAL_MEMZERO(g_ast_hmac_wlan_dtx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_dtx_event_sub_table));

    OAL_MEMZERO(g_ast_hmac_wlan_drx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_drx_event_sub_table));

    OAL_MEMZERO(g_ast_hmac_wlan_crx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_crx_event_sub_table));

    OAL_MEMZERO(g_ast_hmac_tbtt_event_sub_table, OAL_SIZEOF(g_ast_hmac_tbtt_event_sub_table));

    OAL_MEMZERO(g_ast_hmac_wlan_ctx_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_ctx_event_sub_table));

    OAL_MEMZERO(g_ast_hmac_wlan_misc_event_sub_table, OAL_SIZEOF(g_ast_hmac_wlan_misc_event_sub_table));

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    OAL_MEMZERO(g_ast_hmac_hcc_rx_event_sub_table, OAL_SIZEOF(g_ast_hmac_hcc_rx_event_sub_table));
#endif
}
/*lint -e19*/
oal_module_symbol(g_ast_dmac_host_crx_table);
oal_module_symbol(g_ast_dmac_tx_host_drx);
oal_module_symbol(g_ast_dmac_tx_wlan_dtx);
oal_module_symbol(g_ast_dmac_wlan_ctx_event_sub_table);
oal_module_symbol(g_ast_dmac_wlan_drx_event_sub_table);
oal_module_symbol(g_ast_dmac_high_prio_event_sub_table);
oal_module_symbol(g_ast_dmac_wlan_crx_event_sub_table);
oal_module_symbol(g_ast_dmac_tx_comp_event_sub_table);
oal_module_symbol(g_ast_dmac_tbtt_event_sub_table);
oal_module_symbol(g_ast_dmac_misc_event_sub_table);

oal_module_symbol(g_ast_hmac_wlan_dtx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_drx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_crx_event_sub_table);
oal_module_symbol(g_ast_hmac_tbtt_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_ctx_event_sub_table);
oal_module_symbol(g_ast_hmac_wlan_misc_event_sub_table);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
oal_module_symbol(g_ast_hmac_hcc_rx_event_sub_table);
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
oal_module_symbol(g_ast_hmac_host_drx_event_sub_table);
#endif
oal_module_symbol(event_fsm_table_register);

oal_module_symbol(event_fsm_unregister);
/*lint +e19*/
/*****************************************************************************
DFX????????????
*****************************************************************************/
#ifdef _PRE_WLAN_DFT_STAT

oal_uint32 dfx_get_performance_log_switch_enable(dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type)
{
    return g_auc_dfx_performance_log_switch[uc_performance_log_switch_type];
}

oal_void dfx_set_performance_log_switch_enable(dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type, oal_uint8 uc_value)
{
    if(DFX_PERFORMANCE_LOG_BUTT <= uc_performance_log_switch_type)
    {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "dfx_set_performance_log_switch_enable::log_switch type:%d.", uc_performance_log_switch_type);
        return;
    }
    g_auc_dfx_performance_log_switch[uc_performance_log_switch_type] = uc_value;
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


