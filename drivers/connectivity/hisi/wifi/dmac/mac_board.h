

#ifndef __MAC_BOARD_H__
#define __MAC_BOARD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_vap.h"
#include "dmac_ext_if.h"


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_H

/*****************************************************************************
  2 ??????
*****************************************************************************/
/*****************************************************************************
  DFX ??????
*****************************************************************************/
#if ((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION) || defined(_PRE_WIFI_DMT))&&(!defined(_PRE_PC_LINT))
#define DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type) (0)
#define DFX_SET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type, _uc_value)
#else
#define DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type) dfx_get_performance_log_switch_enable(_uc_type)
#define DFX_SET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type, _uc_value) dfx_set_performance_log_switch_enable(_uc_type, _uc_value)
#endif
    

/*****************************************************************************
  3 ????????
*****************************************************************************/
/*****************************************************************************
  DFX????????????
*****************************************************************************/
#ifdef _PRE_WLAN_DFT_STAT
typedef enum
{
    DFX_PERFORMANCE_TX_LOG,
    DFX_PERFORMANCE_DUMP,
    DFX_PERFORMANCE_LOG_BUTT,
}dfx_performance_log_switch_enum;
typedef oal_uint8 dfx_performance_log_switch_enum_uint8;
#endif

/*****************************************************************************
  4 ????????????
*****************************************************************************/
/* HOST CRX???? */
extern frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT];

/* DMAC??????HOST_DRX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT];

/* DMAC??????WLAN_DTX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT];

/* DMAC??????WLAN_CTX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];

/* DMAC????,WLAN_DRX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* DMAC???????????????????????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table[HAL_EVENT_ERROR_IRQ_SUB_TYPE_BUTT];

/* DMAC????,WLAN_CRX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMAC??????TX_COMP???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_BUTT];

/* DMAC????, TBTT?????????????????? */
extern frw_event_sub_table_item_stru g_ast_dmac_tbtt_event_sub_table[HAL_EVENT_TBTT_SUB_TYPE_BUTT];

/*DMAC????, MISC?????????????????? */
extern  frw_event_sub_table_item_stru g_ast_dmac_misc_event_sub_table[HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT];

/* WLAN_DTX ???????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX_BUTT];

/* HMAC???? WLAN_DRX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? WLAN_CRX???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? TBTT???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_tbtt_event_sub_table[DMAC_TBTT_EVENT_SUB_TYPE_BUTT];

/* HMAC???? ????HOST???????????????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT];

/* HMAC???? MISC???????????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT];

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
/* HMAC???? IPC???????????????????????? */
extern frw_event_sub_table_item_stru g_ast_hmac_host_drx_event_sub_table[DMAC_TX_HOST_DRX_BUTT];
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
extern frw_event_sub_table_item_stru g_ast_hmac_hcc_rx_event_sub_table[DMAC_HCC_RX_EVENT_SUB_TYPE_BUTT];
#endif
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
    
/*****************************************************************************
  DFX????????????
*****************************************************************************/
#ifdef _PRE_WLAN_DFT_STAT
extern oal_uint32 dfx_get_performance_log_switch_enable(dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type);
extern oal_void dfx_set_performance_log_switch_enable(dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type, oal_uint8 uc_value);
#endif    
    

extern oal_void  event_fsm_unregister(oal_void);

extern oal_void  event_fsm_table_register(oal_void);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of mac_board */



