
#ifndef    __DMAC_GREEN_AP_H__
#define    __DMAC_GREEN_AP_H__

#ifdef  __cplusplus
#if     __cplusplus
extern  "C" {
#endif
#endif

#ifdef    _PRE_WLAN_FEATURE_GREEN_AP
/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include    "oal_types.h"
#include    "oal_hardware.h"
#include    "frw_ext_if.h"
#include    "mac_device.h"


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_GREEN_AP_H

/*****************************************************************************
  2 ??????
*****************************************************************************/
#define  DMAC_DEFAULT_GAP_WORK_TIME     10
#define  DMAC_DEFAULT_GAP_PAUSE_TIME    10
#define  DMAC_DEFAULT_GAP_SLOT_CNT      2*100/(DMAC_DEFAULT_GAP_WORK_TIME + DMAC_DEFAULT_GAP_PAUSE_TIME)
#define  DMAC_DEFAULT_GAP_RESUME_OFFSET 1

#define  DMAC_GAP_EN_THRPT_HIGH_TH        50                        /* ??????????????????green ap?????? */
#define  DMAC_GAP_PPS_TO_THRPT_MBPS(pps)  ((pps * 8 * 1460) >> 20)  /* pps????????(Mbps) */
/*****************************************************************************
  3 ????????
*****************************************************************************/
typedef enum
{
    DMAC_GREEN_AP_STATE_NOT_INITED   = 0,
    DMAC_GREEN_AP_STATE_INITED       = 1,
    DMAC_GREEN_AP_STATE_WORK         = 2,
    DMAC_GREEN_AP_STATE_PAUSE        = 3,

    DMAC_GREEN_AP_STATE_BUTT
}dmac_green_ap_state_enum;
typedef oal_uint8 dmac_green_ap_state_enum_uint8;


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
typedef struct tag_dmac_green_ap_mgr_stru
{
    oal_uint8                      uc_green_ap_enable;       /* green ap???? */
    oal_uint8                      uc_green_ap_suspend;      /* green ap????suspend */
    oal_bool_enum_uint8            en_green_ap_dyn_en;       /* green ap ????????????(??????????) */
    oal_bool_enum_uint8            en_green_ap_dyn_en_old;   /* ??????????green ap ???????????? */
    dmac_green_ap_state_enum_uint8 uc_state;                 /* green ap???? */
    oal_uint8                      resv1[3];

    oal_uint8 uc_device_id;         /* ??????????device id */
    oal_uint8 uc_hal_vap_id;
    oal_uint8 uc_vap_id;
    oal_uint8 uc_work_time;         /* ??????????????ms*/
    oal_uint8 uc_pause_time;        /* ??????????????ms */
    oal_uint8 uc_max_slot_cnt;      /* beacon?????????????????? = 2*beacon period/(work+pause) */
    oal_uint8 uc_cur_slot;          /* ???????? */
    oal_uint8 uc_resume_offset;     /* resume????????????????????????????????????resume????RF */

#if (!defined(_PRE_PRODUCT_ID_HI110X_DEV))
    struct hrtimer st_gap_timer;
#endif

    hal_one_packet_cfg_stru         st_one_packet_cfg;
    oal_uint8                       uc_one_packet_done;
    oal_uint8                       resv2[3];

    oal_uint32                      ul_pause_count;
    oal_uint32                      ul_resume_count;
    oal_uint32                      ul_total_count;
}dmac_green_ap_mgr_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32  dmac_green_ap_init(mac_device_stru *pst_device);
extern oal_uint32  dmac_green_ap_exit(mac_device_stru *pst_device);
extern oal_uint32  dmac_green_ap_timer_event_handler(frw_event_mem_stru *pst_event_mem);
extern oal_uint32  dmac_green_ap_start(oal_uint8 uc_device_id);
extern oal_uint32  dmac_green_ap_stop(oal_uint8 uc_device_id);
extern oal_uint32  dmac_green_ap_suspend(mac_device_stru *pst_device);
extern oal_uint32  dmac_green_ap_resume(mac_device_stru *pst_device);
extern oal_uint32  dmac_green_ap_dump_info(oal_uint8 uc_device_id);
extern oal_void  dmac_green_ap_pps_process(oal_uint32 ul_pps_rate);

#endif /* _PRE_WLAN_FEATURE_GREEN_AP */

#ifdef  __cplusplus
#if     __cplusplus
    }
#endif
#endif

#endif
