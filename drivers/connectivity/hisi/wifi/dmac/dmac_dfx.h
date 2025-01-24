

#ifndef __DMAC_DFX_H__
#define __DMAC_DFX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "oal_ext_if.h"
#include "mac_vap.h"
#include "mac_device.h"
#include "hal_ext_if.h"
#include "dmac_ext_if.h"
#include "dmac_dft.h"
/*****************************************************************************
  1 ??????????????
*****************************************************************************/

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_DFX_H

/*****************************************************************************
  2 ??????
*****************************************************************************/


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
/* ?????????????????????? */
typedef struct
{
    frw_timeout_stru            st_timer;
}dmac_exception_stat_stru;

typedef struct
{
    dmac_exception_stat_stru            st_excp_stat_info;
}dmac_dft_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32 oam_exception_stat_timeout(void *p_arg);
extern oal_uint32 dmac_dfx_init(oal_void);
extern oal_uint32 dmac_dfx_exit(oal_void);

extern oal_uint32  dmac_custom_init(oal_uint32 ul_psta_enable);

extern oal_uint32  dmac_dfr_process_mac_error(
                                    mac_device_stru                 *pst_mac_device,
                                    hal_mac_error_type_enum_uint8    en_error_id,
                                    oal_uint32     ul_error1_irq_state,
                                    oal_uint32     ul_error2_irq_state);
#ifdef _PRE_WLAN_FEATURE_DFR
extern oal_uint32 dmac_dfr_init(oal_void *p_arg);
extern oal_uint32  dmac_dfr_tx_comp_timeout_handle(oal_void *p_arg);
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
oal_uint32  dmac_pcie_err_timeout(oal_void *p_arg);
#endif

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_dfx.h */
