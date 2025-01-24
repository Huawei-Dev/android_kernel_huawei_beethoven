


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)
/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_hcc_slave_if.h"
#include "hal_reset.h"
#include "wlan_spec.h"
#include "wlan_mib.h"
#include "hal_device.h"
#include "hal_soc.h"
#include "hal_mac_reg.h"
#include "hal_soc_reg.h"
#include "hal_phy_reg.h"

#include "oal_mem.h"
#include "mac_resource.h"
#include "stack_cr4.h"
#include "DMA.h"
#include "dmac_pm_sta.h"
#include "dmac_rx_data.h"
#include "frw_timer.h"
#ifdef _PRE_WLAN_FEATURE_BTCOEX
#include "dmac_btcoex.h"
#endif
#include "pm_extern.h"

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
#include "dmac_auto_adjust_freq.h"
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_PM_STA_C



/*****************************************************************************
  2 ????????????
*****************************************************************************/
extern oal_uint8  dmac_psm_is_hw_queues_empty(mac_device_stru  *pst_device);
extern oal_uint8  dmac_psm_is_tid_queues_empty(dmac_vap_stru  *pst_dmac_vap);

oal_uint32  g_ul_wait_hw = 0;
oal_uint32  g_ul_wait_tid = 0;
oal_uint32  g_ul_wait_rx_irq = 0;
oal_uint32  g_ul_coex_cnt  = 0;
oal_uint32  g_ul_wait_scanning = 0;
oal_uint32  g_ul_wait_dbac = 0;

/*****************************************************************************
  3 ????????
*****************************************************************************/

oal_void dmac_psm_sync_tsf_to_sta(oal_void)
{
    hal_to_dmac_vap_stru                 st_sta_hal_vap;

    /* ????pa */
    dmac_psm_cbb_stopwork();

    /* ??ap0??tsf??tbtt??????sta0 */
    st_sta_hal_vap.uc_vap_id = 4;

    hal_sta_tsf_restore(&st_sta_hal_vap);

    /* ??ap1??tsf??tbtt??????sta1 */
    st_sta_hal_vap.uc_vap_id = 5;

    hal_sta_tsf_restore(&st_sta_hal_vap);

    return;
}

oal_void dmac_psm_sync_tsf_to_ap(oal_void)
{
    hal_to_dmac_vap_stru    st_sta_hal_vap;

    /* ??sta0??tsf??tbtt??????ap0 */
    st_sta_hal_vap.uc_vap_id = 4;

    //????????????sta->ap????????????????????ap????>sta??????
    hal_sta_tsf_save(&st_sta_hal_vap, OAL_FALSE);

    /* ??sta1??tsf??tbtt??????ap1 */
    st_sta_hal_vap.uc_vap_id = 5;

    hal_sta_tsf_save(&st_sta_hal_vap, OAL_FALSE);

    return;

}

oal_uint8 dmac_psm_check_hw_txq_state(oal_void)
{
    mac_device_stru             *pst_device;

    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);


    /*??????????????????????TID??????????*/
    if(OAL_FALSE == dmac_psm_is_hw_queues_empty(pst_device))
    {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


oal_uint8 dmac_psm_check_txrx_state(oal_void)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    mac_device_stru             *pst_device;
    dmac_vap_stru                  *pst_dmac_vap;
    oal_uint8                       uc_vap_idx;

    pst_device      = mac_res_get_dev(0);
    if (OAL_UNLIKELY(OAL_PTR_NULL == pst_device ))
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{dmac_psm_check_txrx_state::pst_device[id:0] is NULL!}");
        return OAL_FALSE;
    }
    pst_hal_device  = pst_device->pst_device_stru;



#ifdef _PRE_WLAN_FEATURE_BTCOEX
    if(pst_hal_device->st_btcoex_btble_status.un_bt_status.st_bt_status.bit_bt_on)
    {
        g_ul_coex_cnt++;
    }
#endif

    /* ?????????????? */
    if (MAC_SCAN_STATE_RUNNING == pst_device->en_curr_scan_state)
    {
        /* ????host??????????????????++,???????????????????????????????????????? */
        if (OAL_TRUE == PM_WLAN_IsHostSleep())
        {
            g_ul_wait_scanning++;
        }
        return OAL_FALSE;
    }

    /* DBAC running ?????? */
    if(OAL_TRUE == mac_is_dbac_running(pst_device))
    {
        g_ul_wait_dbac++;
        return OAL_FALSE;
    }

    /*??????????????????????TID??????????*/
    if(OAL_FALSE == dmac_psm_is_hw_queues_empty(pst_device))
    {
        //OAL_IO_PRINT("pm check:tx hw queues not empty\r\n");
        g_ul_wait_hw++;
        return OAL_FALSE;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_device->uc_vap_num; uc_vap_idx++)
    {
        pst_dmac_vap = mac_res_get_dmac_vap(pst_device->auc_vap_id[uc_vap_idx]);

        if (WLAN_VAP_MODE_BSS_STA == pst_dmac_vap->st_vap_base_info.en_vap_mode)
        {

            if(OAL_FALSE == dmac_psm_is_tid_queues_empty(pst_dmac_vap))
            {
                //OAL_IO_PRINT("pm check:tx tid queues not empty\r\n");
                g_ul_wait_tid++;
                 return OAL_FALSE;
            }
        }
    }

    /*??????????????????????*/
    if (!oal_dlist_is_empty(&(pst_hal_device->ast_rx_isr_info_list[0])))
    {
        //OAL_IO_PRINT("ast_rx_isr_info_list[0] IS NOT EMPTY\r\n");
        g_ul_wait_rx_irq++;
        return OAL_FALSE;

    }

    if (!oal_dlist_is_empty(&(pst_hal_device->ast_rx_isr_info_list[1])))
    {
        //OAL_IO_PRINT("ast_rx_isr_info_list[1] IS NOT EMPTY\r\n");
        g_ul_wait_rx_irq++;
         return OAL_FALSE;
    }

    return OAL_TRUE;

}

oal_uint8  dmac_psm_is_fake_queues_empty(oal_void)
{
    mac_device_stru                     *pst_device;
    hal_to_dmac_device_stru             *pst_hal_device;
    oal_uint8                           uc_fake_q;
    oal_uint8                           uc_ret = OAL_TRUE;

    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);
    pst_hal_device  = pst_device->pst_device_stru;

    /* ????????????????fake_q,????????false????break,?????? */
    for (uc_fake_q = 0; uc_fake_q <= HAL_TX_FAKE_QUEUE_BGSCAN_ID; uc_fake_q++)
    {
        if (dmac_tx_fake_queue_empty_assert(pst_hal_device, uc_fake_q, THIS_FILE_ID) != OAL_TRUE)
        {
            uc_ret = OAL_FALSE;
        }
    }

    return uc_ret;
}


oal_void dmac_psm_save_tcm (hal_to_dmac_device_stru  *pst_hal_device)
{
    hal_reset_reg_save(pst_hal_device, HAL_RESET_HW_TYPE_TCM);

}

oal_void dmac_psm_save_start_dma (oal_uint8* puc_ch0,oal_uint8* puc_ch1,oal_uint8* puc_ch2)
{
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_device;

    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);
    pst_hal_device  = pst_device->pst_device_stru;

    hal_reset_reg_dma_save(pst_hal_device,puc_ch0,puc_ch1,puc_ch2);
}

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT

oal_void dmac_psm_save_dutycycle (mac_device_stru  *pst_device)
{
    oal_uint32 aul_reg_value[3];

    /* ????mac?????????? */
    hal_get_txrx_frame_time(pst_device->pst_device_stru, aul_reg_value);
   /*??????????????????tx rx????*/
    pst_device->ul_rx_dir_duty_lp += aul_reg_value[0];
    pst_device->ul_rx_nondir_duty_lp += aul_reg_value[1];
    pst_device->ul_duty_ratio_lp += aul_reg_value[2];
}
#endif

oal_void dmac_psm_save_mac_statistics_data(mac_device_stru  *pst_device)
{
    hal_mac_key_statis_info_stru   st_mac_key_statis_info;

    hal_get_mac_statistics_data(pst_device->pst_device_stru, &st_mac_key_statis_info);

    pst_device->st_mac_key_statis_info.ul_tkipccmp_rep_fail_cnt += st_mac_key_statis_info.ul_tkipccmp_rep_fail_cnt;
    pst_device->st_mac_key_statis_info.ul_tx_mpdu_cnt  += st_mac_key_statis_info.ul_tx_mpdu_cnt;
    pst_device->st_mac_key_statis_info.ul_rx_passed_mpdu_cnt  += st_mac_key_statis_info.ul_rx_passed_mpdu_cnt;
    pst_device->st_mac_key_statis_info.ul_rx_failed_mpdu_cnt  += st_mac_key_statis_info.ul_rx_failed_mpdu_cnt;
    pst_device->st_mac_key_statis_info.ul_rx_tkipccmp_mic_fail_cnt  += st_mac_key_statis_info.ul_rx_tkipccmp_mic_fail_cnt;
    pst_device->st_mac_key_statis_info.ul_key_search_fail_cnt += st_mac_key_statis_info.ul_key_search_fail_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_dotb_ok_frm_cnt += st_mac_key_statis_info.ul_phy_rx_dotb_ok_frm_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_htvht_ok_frm_cnt  += st_mac_key_statis_info.ul_phy_rx_htvht_ok_frm_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_lega_ok_frm_cnt  += st_mac_key_statis_info.ul_phy_rx_lega_ok_frm_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_dotb_err_frm_cnt += st_mac_key_statis_info.ul_phy_rx_dotb_err_frm_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_htvht_err_frm_cnt  += st_mac_key_statis_info.ul_phy_rx_htvht_err_frm_cnt;
    pst_device->st_mac_key_statis_info.ul_phy_rx_lega_err_frm_cnt  += st_mac_key_statis_info.ul_phy_rx_lega_err_frm_cnt;
}



oal_uint8 dmac_psm_clean_state (oal_void)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    mac_device_stru             *pst_device;
    oal_uint32                   ul_is_netbuf_empty;
    extern frw_event_mem_stru   *g_pst_first_rx_event;

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    dmac_freq_control_stru      *pst_dmac_freq;
    pst_dmac_freq = dmac_get_auto_freq_handle();
    pst_dmac_freq->uc_pm_enable = OAL_FALSE;
#endif

    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);

    pst_hal_device  = pst_device->pst_device_stru;

    if(g_pst_first_rx_event)
    {
        FRW_EVENT_FREE(g_pst_first_rx_event);
        g_pst_first_rx_event = OAL_PTR_NULL;
    }




    hcc_slave_clean();

    hal_rx_destroy_dscr_queue(pst_hal_device,OAL_TRUE);

    /* ??????oam????????????netbuf????,??????????????????????netbuf?????????????? */
    ul_is_netbuf_empty = oal_mem_is_netbuf_empty();

    if (ul_is_netbuf_empty != OAL_TRUE)
    {
        OAM_ERROR_LOG0(0,OAM_SF_PWR,"{dmac_psm_clean_state::before deep sleep netbuf is not empty.}");
        OAL_MEM_INFO_PRINT(OAL_MEM_POOL_ID_NETBUF);
    }

    return OAL_SUCC;
}


oal_uint8 dmac_psm_save_ps_state (oal_void)
{
    hal_to_dmac_device_stru *pst_hal_device;
    mac_device_stru         *pst_device;

    /*??????????????????????????????????????????????*/
    pst_device  = mac_res_get_dev(0);

    pst_hal_device  = pst_device->pst_device_stru;

    dmac_psm_save_tcm(pst_hal_device);

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT
    dmac_psm_save_dutycycle(pst_device);
#endif
    dmac_psm_save_mac_statistics_data(pst_device);

    /* ????PHY???????????? */
    //dmac_psm_save_phy_hw_reg(pst_hal_device);

   /* ????MAC???????????? */
    //dmac_psm_save_mac_hw_reg(pst_hal_device);

    return OAL_TRUE;


}


oal_void dmac_psm_init_netbuf_pool(oal_void)
{
    oal_uint32 ul_atcm_save = 0;

    ul_atcm_save = oal_mem_is_atcm_need_save();

    if(OAL_TRUE == ul_atcm_save)
    {
        oal_mem_do_create_netbuf_subpool(OAL_MEM_NETBUF_POOL_ID_LARGE_PKT);

        oal_mem_clear_atcm_need_save_flag();
    }
}


oal_void  dmac_psm_recover_mac_reg(hal_to_dmac_device_stru  *pst_hal_device)
{
    hal_reset_reg_restore(pst_hal_device, HAL_RESET_HW_TYPE_MAC);
}


oal_void dmac_psm_recover_tcm (hal_to_dmac_device_stru  *pst_hal_device)
{
    hal_reset_reg_restore(pst_hal_device, HAL_RESET_HW_TYPE_TCM);
}


oal_void dmac_psm_recover_primary_channel(mac_device_stru *pst_device)
{
    hal_to_dmac_device_stru         *pst_h2d_device;
    hal_wifi_channel_status_stru    *pst_wifi_channel_status;

    pst_h2d_device = pst_device->pst_device_stru;

    pst_wifi_channel_status = &(pst_h2d_device->st_wifi_channel_status);

    /* ???????? */
    hal_set_bandwidth_mode(pst_device->pst_device_stru, pst_wifi_channel_status->uc_bandwidth);

    hal_set_primary_channel(pst_device->pst_device_stru,
                   pst_wifi_channel_status->uc_chan_number,
                   pst_wifi_channel_status->uc_band,
                   pst_wifi_channel_status->uc_idx,
                   pst_wifi_channel_status->uc_bandwidth);


}


oal_uint8  dmac_psm_recover_powerdown(oal_uint8 uc_dmach0,oal_uint8 uc_dmach1,oal_uint8 uc_dmach2)
{
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_device;
    //oal_uint32                      ul_ret  = OAL_SUCC;
    hal_error_state_stru            st_error_state;
#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    dmac_freq_control_stru      *pst_dmac_freq;
#endif
    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);//????????????????????????device id????????????????????
    pst_hal_device  = pst_device->pst_device_stru;


    /*??TSF??????????BANK1??????????*/
    dmac_psm_recover_mac_reg(pst_hal_device);

    /*tsf????????*/
    PM_WLAN_Tsf_Aon_to_Inner_start();

    //dmac_psm_recover_phy_reg(pst_hal_device);

    /* TCM ????*/
    dmac_psm_recover_tcm(pst_hal_device);

    //restart??????paldo ??????
    frw_timer_time_fix();

    DMAWaitChannelDone(uc_dmach0);

    dmac_psm_init_netbuf_pool();

    hal_rx_init_dscr_queue(pst_hal_device,OAL_FALSE);

     /*????tsf????????*/
    PM_WLAN_Aon_Tsf_Sync();

    DMAWaitChannelDone(uc_dmach1);

    //DMAWaitChannelDone(uc_dmach2);


    /*??????????????????????????????????????????????????????????*/
    //hal_rx_destroy_dscr_queue(pst_hal_device,OAL_FALSE);

    dmac_psm_sync_tsf_to_ap();

    hal_irq_init((hal_device_stru*)pst_hal_device);


    hal_set_machw_rx_buff_addr(pst_hal_device, (oal_uint32)HAL_RX_DSCR_GET_REAL_ADDR(pst_hal_device->ast_rx_dscr_queue[HAL_RX_DSCR_NORMAL_PRI_QUEUE].pul_element_head), HAL_RX_DSCR_NORMAL_PRI_QUEUE);
    hal_set_machw_rx_buff_addr(pst_hal_device, (oal_uint32)HAL_RX_DSCR_GET_REAL_ADDR(pst_hal_device->ast_rx_dscr_queue[HAL_RX_DSCR_HIGH_PRI_QUEUE].pul_element_head), HAL_RX_DSCR_HIGH_PRI_QUEUE);
    hal_set_machw_rx_buff_addr(pst_hal_device, (oal_uint32)HAL_RX_DSCR_GET_REAL_ADDR(pst_hal_device->ast_rx_dscr_queue[HAL_RX_DSCR_SMALL_QUEUE].pul_element_head), HAL_RX_DSCR_SMALL_QUEUE);

    /* ?????????? */
    st_error_state.ul_error1_val = 0xFFFFFFFF;
    st_error_state.ul_error2_val = 0xFFFFFFFF;
    hal_clear_mac_error_int_status(pst_hal_device, &st_error_state);
    hal_clear_mac_int_status(pst_hal_device, 0xffffffff);

    /* SDIO ?????????? */
    hcc_slave_reinit();

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
        pst_dmac_freq = dmac_get_auto_freq_handle();
        pst_dmac_freq->uc_pm_enable = OAL_TRUE;
#endif

    return OAL_TRUE;

}



oal_void  dmac_psm_recover_start_dma(oal_uint8 *puc_ch0,oal_uint8 *puc_ch1,oal_uint8 *puc_ch2)
{
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_device;

    pst_device      = mac_res_get_dev(0);
    pst_hal_device  = pst_device->pst_device_stru;

    hal_reset_reg_dma_restore(pst_hal_device,puc_ch0,puc_ch1,puc_ch2);

}


oal_uint8 dmac_psm_recover_no_powerdown (oal_void)
{
    hal_to_dmac_device_stru        *pst_hal_device;
    mac_device_stru                *pst_device;
    hal_error_state_stru            st_error_state;
    //oal_uint32                      ul_ret;
#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    dmac_freq_control_stru      *pst_dmac_freq;
#endif

    pst_device      = mac_res_get_dev(0);//????????????????????????device id????????????????????
    pst_hal_device  = pst_device->pst_device_stru;


    PM_WLAN_Tsf_Aon_to_Inner_start();

    //restart??????paldo ??????
    frw_timer_time_fix();

    /* g_st_netbuf_pool??????, ???????????????????????? */
    //OAL_MEMZERO(&g_st_netbuf_pool, OAL_SIZEOF(g_st_netbuf_pool));

    //dmac_psm_init_netbuf_pool();

    /*??????????????????????????????????????????????????????????*/
    //hal_rx_destroy_dscr_queue(pst_hal_device,OAL_FALSE);

    /* ??????????????????????rx????,????rx????????????????????????????rx new */
    hal_psm_clear_mac_rx_isr(pst_hal_device);

    hal_rx_init_dscr_queue(pst_hal_device,OAL_TRUE);


     /* ?????????? */
    st_error_state.ul_error1_val = 0xFFFFFFFF;
    st_error_state.ul_error2_val = 0xFFFFFFFF;
    hal_clear_mac_error_int_status(pst_hal_device, &st_error_state);
    hal_clear_mac_int_status(pst_hal_device, 0xffffffff);

    /* SDIO ?????????? */
    hcc_slave_reinit();

    /*????tsf????????*/
    PM_WLAN_Aon_Tsf_Sync();

    dmac_psm_sync_tsf_to_ap();

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    pst_dmac_freq = dmac_get_auto_freq_handle();
    pst_dmac_freq->uc_pm_enable = OAL_TRUE;
#endif

    return OAL_TRUE;

}


oal_uint8 dmac_psm_cbb_stopwork (oal_void)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    mac_device_stru             *pst_device;

    pst_device      = mac_res_get_dev(0);

    pst_hal_device  = pst_device->pst_device_stru;

    /* ???????????? ???????????? */
    //hal_set_machw_tx_suspend(pst_hal_device);

    /* ????pa */
    hal_disable_machw_phy_and_pa(pst_hal_device);

    return OAL_TRUE;
}


oal_void dmac_psm_rf_sleep(oal_uint8 uc_restore_reg)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    mac_device_stru             *pst_device;

    pst_device      = mac_res_get_dev(0);

    pst_hal_device  = pst_device->pst_device_stru;

    hal_psm_rf_sleep(pst_hal_device, uc_restore_reg);

    return;
}

oal_void dmac_psm_rf_awake (oal_uint8 uc_restore_reg)
{
    hal_to_dmac_device_stru     *pst_hal_device;
    mac_device_stru             *pst_device;

    /*??????????????????????????????????????????????*/
    pst_device      = mac_res_get_dev(0);

    pst_hal_device  = pst_device->pst_device_stru;


    hal_psm_rf_awake(pst_hal_device,uc_restore_reg);

    dmac_psm_recover_primary_channel(pst_device);

    return;
}




#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

