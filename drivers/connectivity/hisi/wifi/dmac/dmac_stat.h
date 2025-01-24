

#ifndef __DMAC_STAT_H__
#define __DMAC_STAT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_PERFORM_STAT

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "dmac_ext_if.h"
#include "hal_ext_if.h"
#include "frw_ext_if.h"
#include "dmac_user.h"
#include "dmac_main.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_STAT_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define	DMAC_STAT_ITEM_LIMIT                    100     /* ??????????????per?????????????? */

#define	DMAC_STAT_TID_DELAY_NODE_LIMIT          128		/* tid???????????????????? */
#define DMAC_STAT_TID_PER_NODE_LIMIT            128     /* tid per?????????????? */
#define	DMAC_STAT_TID_THRPT_NODE_LIMIT          128		/* tid?????????????????????? */
#define	DMAC_STAT_USER_THRPT_NODE_LIMIT         32		/* user?????????????????????? */
#define DMAC_STAT_VAP_THRPT_NODE_LIMIT          8		/* vap?????????????????????? */
#define DMAC_STAT_TIMER_CYCLE_MS                100     /* ???????????? */

#define DMAC_STAT_TIME_USEC_INT64(_pst_time) \
    ((oal_int64)((_pst_time)->i_sec) * 1000000 + (_pst_time)->i_usec)

#define DMAC_STAT_TIME_USEC_DIFF(_pst_time1, _pst_time2)    \
    (DMAC_STAT_TIME_USEC_INT64(_pst_time2)- DMAC_STAT_TIME_USEC_INT64(_pst_time1))


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


/*****************************************************************************
  8 UNION????
*****************************************************************************/
/* stat_node?????? */
typedef struct
{
    oal_dlist_head_stru	        st_entry;                               /* ????????entry */
    frw_timeout_stru            st_timer;		                        /* ?????? */

    oal_bool_enum_uint8         uc_stat_flag;                           /* ???????????? */
    oal_uint8                   uc_resv[3];                             /* ???????? */

    dmac_stat_param_stru       *pst_stat_param;                          /* ???????? */
    oal_uint32                  us_total_item;                          /* ?????????? */
    oal_uint32                  us_curr_item;                           /* ?????????? */
    oal_uint32                  aul_stat_cnt[DMAC_STAT_PER_BUTT];            /* ???????????????? */
    oal_uint32                  aul_stat_sum[DMAC_STAT_PER_BUTT];            /* ???????????????????????? */
    dmac_stat_timeout_func      p_inner_func;                           /* ?????????????????????? */

    oal_uint32                 *pul_stat_avg;                           /* ??CFG?????????????????????????????? */
}dmac_stat_node_stru;

typedef struct
{
	oal_uint32			 ul_node_num;           /* ???????????? */
	oal_dlist_head_stru	 st_stat_node_dlist;    /* ?????????????????????? */
}dmac_stat_stru;

/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32  dmac_stat_init(oal_void);
extern oal_uint32  dmac_stat_exit(oal_void);
extern oal_uint32  dmac_stat_tid_delay(dmac_tid_stru *pst_dmac_tid);
extern oal_uint32  dmac_stat_tx_thrpt(dmac_user_stru *pst_dmac_user, oal_uint8 uc_tidno, hal_tx_dscr_ctrl_one_param st_tx_dscr_param);
extern oal_uint32  dmac_stat_rx_thrpt(frw_event_hdr_stru *pst_event_hdr, mac_vap_stru *pst_vap, dmac_rx_ctl_stru *pst_rx_ctl);
extern oal_uint32  dmac_stat_display(oam_module_id_enum_uint16       en_module_id,
                                      mac_stat_type_enum_uint8       en_stat_type,
                                      oal_void                      *p_void);

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_stat.h */


