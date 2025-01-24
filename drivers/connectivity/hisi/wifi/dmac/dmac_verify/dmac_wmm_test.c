

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ?????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "dmac_ext_if.h"
#include "dmac_vap.h"
#include "dmac_main.h"
#include "dmac_wmm_test.h"
#include "dmac_config.h"
#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_WMM_TEST_C

#ifdef _PRE_WLAN_CHIP_TEST
/*****************************************************************************
  2 ??????
*****************************************************************************/


/*****************************************************************************
  3 ????????????
*****************************************************************************/
/*****************************************************************************
  2 ????????????
*****************************************************************************/
/* WME??????????????????OFDM?????? */

/*****************************************************************************
  3 ????????
*****************************************************************************/



oal_uint32  dmac_test_open_wmm_test(mac_vap_stru *pst_mac_vap, oal_uint8 uc_test_type)
{
    hal_to_dmac_device_stru *pst_hal_device;
    mac_device_stru         *pst_mac_device;
    dmac_vap_stru           *pst_dmac_vap;
    oal_uint32               ul_tsf = 0;
    oal_uint32               ul_tsf_passed = 0;
    mac_wme_param_stru      *pst_wmm;
    hal_to_dmac_vap_stru    *pst_hal_vap;
    oal_uint                 ul_irq_flag;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hal_device = pst_mac_device->pst_device_stru;
    if (OAL_PTR_NULL == pst_hal_device)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_vap = mac_res_get_dmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_dmac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hal_vap  = pst_dmac_vap->pst_hal_vap;
    pst_wmm = mac_get_wmm_cfg(pst_mac_vap->en_vap_mode);

    /* ?????????????????????????????? */
    oal_irq_save(&ul_irq_flag, OAL_5115IRQ_DTOWT);

    if (DMAC_TEST_WMM_SUSPEND == (uc_test_type & DMAC_TEST_WMM_SUSPEND))
    {
        /* ???????????? */
        hal_set_machw_tx_suspend(pst_hal_device);
    }

    /* ?????????? */
    hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf);

    /* ????????abort */
    hal_set_tx_abort_en(pst_hal_device, 1);

    if (DMAC_TEST_WMM_ENABLE == (uc_test_type & DMAC_TEST_WMM_ENABLE))
    {
        /* ????WMM */
        hal_enable_machw_edca(pst_hal_device);
        /* ????????WMM???? */
        dmac_config_set_wmm_open_cfg(pst_hal_vap, pst_wmm);
    }
    else
    {
        /* ????WMM */
        hal_disable_machw_edca(pst_hal_device);
        /* ????????WMM???? */
        dmac_config_set_wmm_close_cfg(pst_hal_vap, pst_wmm);
    }

    /* ????abort */
    hal_set_tx_abort_en(pst_hal_device, 0);

    if (DMAC_TEST_WMM_SUSPEND == (uc_test_type & DMAC_TEST_WMM_SUSPEND))
    {
        /* ???????????????? */
        hal_set_machw_tx_resume(pst_hal_device);
    }

    /* ?????????????? */
    hal_vap_tsf_get_32bit(pst_hal_vap, &ul_tsf_passed);

    /* ?????? */
    oal_irq_restore(&ul_irq_flag, OAL_5115IRQ_DTOWT);

    /*??????????????????????us*/
    OAM_WARNING_LOG1(0, OAM_SF_WMM, "{dmac_test_open_wmm_test::wmm time passed=%u}\r\n",
                                     (ul_tsf_passed - ul_tsf));
    return OAL_SUCC;
}


#endif /*_PRE_WLAN_CHIP_TEST*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

