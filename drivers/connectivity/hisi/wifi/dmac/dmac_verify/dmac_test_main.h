

#ifndef __DMAC_TEST_MAIN_H__
#define __DMAC_TEST_MAIN_H__

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
#include "dmac_ext_if.h"
#include "dmac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_TEST_MAIN_H

/*****************************************************************************
  2 ??????
*****************************************************************************/
typedef oal_uint32 (* dmac_test_encap_frame)(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_buff, oal_uint8 *puc_param, oal_uint32 ul_param_len);

/* ???????????????????????????? */
#ifdef _PRE_WLAN_CHIP_TEST
#define DMAC_CHIP_TEST_CALL(func_call) do{\
    if(0 != dmac_test_get_chip_test())\
    {\
        (func_call);\
    }\
}while(0);
#else
#define DMAC_CHIP_TEST_CALL(func_call)
#endif


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
typedef struct dmac_lpm_txopps_tx_stub
{
    oal_uint8                       uc_stub_on;            /*0:off, 1:on*/
    oal_uint8                       auc_rsv[1];
    oal_uint16                      us_partial_aid_real;     /*??????Partial AID*/
    oal_uint16                      us_begin_num;            /* ??n?????????? */
    oal_uint16                      us_curr_num;             /* ?????????????? */
}dmac_lpm_txopps_tx_stub_stru;

typedef struct dmac_lpm_smps_tx_stub
{
    oal_uint8                       uc_stub_type;            /*0:off, 1:????,2:????*/
    oal_uint8                       uc_rts_en;             /* ??????????????RTS */
    oal_uint8                       auc_rsv[2];
    oal_uint32                      ul_rate_real[HAL_TX_RATE_MAX_NUM];         /*??????????*/
}dmac_lpm_smps_tx_stub_stru;

/*****************************************************************************
????????:????????????????????????????????????????????????????????????????????????????????
??????????????????????????????????????
*****************************************************************************/
typedef struct dmac_test_mng
{
   dmac_lpm_txopps_tx_stub_stru 
st_lpm_txop_stub;    /*TXOP PS????????????????????*/
   dmac_lpm_smps_tx_stub_stru    st_lpm_smps_stub;    /*SMPS ??????????????*/
   oal_uint32                    ul_wow_int_cnt;      /*wow????????????*/
   oal_uint8                     uc_wow_en;           /*wow????????*/
   oal_uint8                     uc_txop_ps_en;       /*txopps ????????*/
   oal_uint8                     us_txop_rx_cnt;      /*txopps ??????????????????????????*/

   frw_timeout_stru              st_sleep_timer;      /* ???????????????????? */

   /* ???????????????????? */
   dmac_test_encap_frame  ast_dmac_encap_frame[MAC_TEST_MAX_TYPE_NUM];
   oal_uint8              uc_software_retry;
   oal_uint8              uc_retry_test;
   /* add4???????? */
   oal_uint8              uc_add4_open;

   /* ?????????????????? */
   oal_uint8              uc_chip_test_open;

   /* ????????????DFX???? */
   oal_switch_enum_uint8  en_cfg_tx_cnt;

   /* ????msdu_life_time ???? */
   oal_uint8              uc_lifetime_flag;             /* 0:????off??1????on */
   oal_int16              s_ct_sch_lifetime_delay_ms;   /* ??????tx_dscp????offset */
   oal_uint32             ul_tx_mpdu_num;               /* ????mpdu???? */
   oal_uint32             ul_tx_succ_mpdu_num;          /* ??????????mpdu????*/
   oal_uint32             ul_rx_mpdu_num;               /* ??????mpdu???? */

   /* ????txop???? */
   oal_uint8              uc_long_nav_flag;             /* long nav ???????????? */
   oal_uint8              uc_long_nav_is_enabled;       /* long nav???????? */

   /* ????cca???? */
   oal_uint8              uc_cca_flag;                  /* cca???????????? */
   oal_uint32             ul_cca_rx_sta_num;            /* cca????????????STA???? */
   oal_uint32             ul_cca_rx_ap_num;             /* ????????????????????ap???? */
   oal_uint32             ul_mean_backoff;              /* ???????????? */
}dmac_test_mng_stru;
/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/
#ifdef _PRE_WLAN_CHIP_TEST
extern dmac_test_mng_stru  g_st_dmac_test_mng;
#endif

/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_void     dmac_test_init(oal_void);
extern dmac_test_encap_frame *dmac_test_get_encap_frame(oal_void);
extern oal_uint32   dmac_test_encap_init(dmac_test_encap_frame *pst_encap_fun);
extern oal_void     dmac_test_set_software_retry(oal_uint8 * puc_param);
extern oal_void     dmac_test_open_addr4(oal_uint8 *puc_add4_open);
extern oal_void     dmac_test_set_addr4(hal_tx_dscr_stru * pst_tx_dscr);

extern oal_void     dmac_test_set_dscr_software_retry(hal_to_dmac_device_stru * pst_hal_device, hal_tx_dscr_stru * pst_tx_dscr);
extern oal_uint32   dmac_test_get_chip_test(oal_void);
extern oal_void     dmac_test_set_chip_test(oal_uint8 uc_chip_test_open);
extern oal_void dmac_test_lpm_wow_init(oal_void);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of __HMAC_CHIP_TEST_H__ */

