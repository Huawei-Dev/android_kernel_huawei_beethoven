

#ifndef __DMAC_SCAN_TEST_H__
#define __DMAC_SCAN_TEST_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_ext_if.h"
#ifdef _PRE_WLAN_ALG_ENABLE
#include "alg_ext_if.h"
#endif
#include "mac_device.h"
#include "dmac_ext_if.h"
#include "dmac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_SCAN_TEST_H

#if defined(_PRE_WLAN_CHIP_TEST) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

/*****************************************************************************
  2 ??????
*****************************************************************************/

#define SCANNER_CREATE_PROC_ENTRY           create_proc_entry
#define SCANNER_VERIFY_PROC_NAME            "dmac_scanner"
#define PROC_SCANNER_BAND_2G                200
#define PROC_SCANNER_BAND_5G                205

#define PROC_ERROR_TYPE_ALL_OK              0
#define PROC_ERROR_TYPE_INVALID_CHAR        1
#define PROC_ERROR_TYPE_INCOMP_CMD          2
#define PROC_ERROR_TYPE_INVALID_BAND        3
#define PROC_ERROR_TYPE_INVALID_MODE        4

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

typedef struct
{
    oal_uint8                   uc_nchans;      /* ????????       */
    oal_uint8                   uc_scan_func;   /* ?????????????? */
    oal_uint16                  us_nbss;        /* BSS???? */

    oal_time_us_stru            st_time_cost;
    oal_uint32                  ul_request_id;
    oal_uint32                  ul_scan_id;

    mac_scan_chan_stats_stru    ast_channel_results[64];  /* ???????????????? */
    mac_scan_bss_stats_stru     ast_bss_results[64];      /* ????BSS???????? */

    oal_uint8                   uc_curr_status;
    oal_uint8                   uc_write_cnt;
    oal_uint8                   uc_read_cnt;
    oal_uint8                   uc_scan_cb_cnt;
    oal_uint8                   uc_bcast_probe_cnt;
    oal_uint8                   uc_enable_chan_stats_cnt;
    oal_uint8                   uc_enable_radar_det_cnt;
}dmac_scanner_procfs_stru;


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_void dmac_scan_verify_init(oal_void);
extern oal_void dmac_scan_verify_update(oal_uint8 uc_scan_func, oal_uint8 uc_band, oal_uint8  uc_scan_type);

oal_void dmac_scan_verify_cb(oal_uint8 uc_event, void *pst_scan_event_data);

#endif  /* _PRE_WLAN_CHIP_TEST */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of __DMAC_SCAN_TEST_H__ */
