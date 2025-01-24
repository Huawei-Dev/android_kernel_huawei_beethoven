

#ifndef __DMAC_MAIN_H__
#define __DMAC_MAIN_H__

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
#include "dmac_vap.h"
#include "mac_resource.h"
#include "mac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_MAIN_H

/*****************************************************************************
  2 ??????
*****************************************************************************/
#ifdef _PRE_WLAN_DFT_EVENT
#define DMAC_EVENT_PAUSE_OR_RESUME_TID(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string)  oam_event_report(_puc_macaddr, _uc_vap_id, OAM_MODULE_ID_DMAC, en_event_type, _puc_string)
#define DMAC_EVENT_PAUSE_OR_RESUME_USER(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string) oam_event_report(_puc_macaddr, _uc_vap_id, OAM_MODULE_ID_DMAC, en_event_type, _puc_string)
#else
#define DMAC_EVENT_PAUSE_OR_RESUME_TID(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string)
#define DMAC_EVENT_PAUSE_OR_RESUME_USER(_puc_macaddr, _uc_vap_id, en_event_type, _puc_string) ((void)(_puc_string))
#endif
typedef oal_void  (*dmac_set_dscr_func)(oal_int32,oal_uint8,dmac_vap_stru*);

/*????????????????????OAL_SUCC*/
/*lint -e607*/
#define DMAC_CHECK_RET(ul_ret)        \
    if (OAL_SUCC != ul_ret)                      \
    {                                            \
        return ul_ret;                           \
    }

#define DMAC_CHECK_NULL(ptr)        \
    if (OAL_PTR_NULL == ptr)                     \
    {                                            \
        return OAL_ERR_CODE_PTR_NULL;            \
    }

/*lint +e607*/

/* ????DMAC??HMAC?????????????? */
#define DMAC_INIT_SYN_MSG_HDR(_pst_syn_msg, _en_syn_id, _us_len) \
    do {                                            \
        (_pst_syn_msg)->en_syn_id = (_en_syn_id);   \
        (_pst_syn_msg)->us_len = (_us_len);   \
    } while(0)

/*****************************************************************************
  3 ????????
*****************************************************************************/
typedef enum
{
    FRW_DMAC_TO_HMAC_UPDATE_USER_TIMESTAMP,  /* ????USER?????? */

    FRW_DMAC_TO_HMAC_BUTT
}frw_dmac_to_hmac_syn_enum;
typedef oal_uint16 frw_dmac_to_hmac_syn_enum_uint16;

/*dmac????????????????*/
typedef struct
{
    oal_time_us_stru  st_timestamp_us;        /* ???????????????????? */
    oal_uint32        ul_last_timestamp;      /* ????????????????????????*/
    frw_timeout_stru  st_timer;               /* ?????? */

}dmac_timeStamp_stru;

/*****************************************************************************
  4 ????????????
*****************************************************************************/
/* DMAC????, MISC?????????????????? */
#ifdef _PRE_WLAN_FEATURE_DBAC
extern frw_event_sub_table_item_stru g_ast_dmac_misc_event_sub_table[HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT];
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


/* ???????????????????????? */
typedef struct
{
    oal_uint32      ul_addr;
    oal_uint16      us_len;
    oal_uint8       uc_mode;
    oal_uint8       auc_resv[1];
    oal_uint32      ul_reg_val;
}dmac_sdt_reg_frame_stru;

#ifdef _PRE_WLAN_DFT_REG
oal_uint32 dmac_reg_report(frw_event_mem_stru *pst_event_mem);
#endif

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

OAL_STATIC OAL_INLINE oal_void  dmac_excp_free_mgmt_frame(oal_netbuf_stru *pst_netbuf)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_netbuf_free(pst_netbuf);
#endif
}

/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_void  dmac_main_exit(oal_void);
extern oal_int32  dmac_main_init(oal_void);
extern oal_void dmac_timestamp_get(oal_time_us_stru *pst_usec);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
extern oal_uint32 dmac_init_event_process(frw_event_mem_stru *pst_event_mem);
#endif
#ifdef _PRE_WLAN_REALTIME_CALI
extern oal_uint32  dmac_rf_realtime_cali_timeout(oal_void * p_arg);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_main */
