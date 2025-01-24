


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "hmac_user.h"
#include "hmac_main.h"
#include "hmac_vap.h"
#include "hmac_protection.h"
#include "mac_vap.h"
#include "mac_ie.h"
#include "hmac_config.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_PROTECTION_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/


/*****************************************************************************
  3 ????????
*****************************************************************************/

OAL_STATIC oal_uint32 hmac_protection_set_mode(
                                 mac_vap_stru *pst_mac_vap,
                                 wlan_prot_mode_enum_uint8 en_prot_mode);

 oal_uint32 hmac_protection_set_autoprot(mac_vap_stru *pst_mac_vap, oal_switch_enum_uint8 en_mode)
{
    oal_uint32      ul_ret = OAL_SUCC;
    hmac_user_stru *pst_hmac_user;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_SWITCH_OFF == en_mode)
    {
        pst_mac_vap->st_protection.bit_auto_protection = OAL_SWITCH_OFF;
        ul_ret = hmac_protection_set_mode(pst_mac_vap, WLAN_PROT_NO);
    }
    else
    {
        pst_mac_vap->st_protection.bit_auto_protection = OAL_SWITCH_ON;
        /*VAP ?? AP??????*/
        if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
        {
            ul_ret = hmac_protection_update_mode_ap(pst_mac_vap);
        }
        /*VAP ?? STA??????*/
        else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
        {
            pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->uc_assoc_vap_id); /*user????????AP??????*/
            if (OAL_PTR_NULL == pst_hmac_user)
            {
                return OAL_ERR_CODE_PTR_NULL;
            }

            ul_ret = hmac_protection_update_mode_sta(pst_mac_vap, pst_hmac_user);
        }
    }

    return ul_ret;
}


OAL_STATIC oal_uint32 hmac_protection_set_rtscts_mechanism(mac_vap_stru *pst_mac_vap, oal_switch_enum_uint8 en_flag, wlan_prot_mode_enum_uint8 en_prot_mode)
{
    oal_uint32 ul_ret = OAL_SUCC;
    mac_cfg_rts_tx_param_stru st_rts_tx_param;

    mac_protection_set_rts_tx_param(pst_mac_vap,en_flag,en_prot_mode,&st_rts_tx_param);

    ul_ret = hmac_config_set_rts_param(pst_mac_vap, OAL_SIZEOF(mac_cfg_rts_tx_param_stru), (oal_uint8*)(&st_rts_tx_param));

    /*??????/????????????????????????bit_rts_cts_protect_mode????????????????????RTS/CTS enable??*/
    pst_mac_vap->st_protection.bit_rts_cts_protect_mode = en_flag;

    return ul_ret;
}


OAL_STATIC OAL_INLINE oal_uint32 hmac_protection_set_erp_protection(mac_vap_stru *pst_mac_vap, oal_switch_enum_uint8 en_flag)
{
    oal_uint32 ul_ret = OAL_SUCC;
    /*1151??????RTS-CTS???????????? ??????Self-To-CTS????*/
    ul_ret = hmac_protection_set_rtscts_mechanism(pst_mac_vap, en_flag, WLAN_PROT_ERP);

    return ul_ret;
}


OAL_STATIC oal_bool_enum hmac_protection_lsigtxop_check(mac_vap_stru *pst_mac_vap)
{
    mac_user_stru  *pst_mac_user;

    /*????????11n??????????????lsigtxop????*/
    if ((WLAN_HT_MODE != pst_mac_vap->en_protocol)
        && (WLAN_HT_ONLY_MODE != pst_mac_vap->en_protocol)
        && (WLAN_HT_11G_MODE != pst_mac_vap->en_protocol))
    {
        return OAL_FALSE;
    }

    if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id); /*user????????AP??????*/
        if (OAL_PTR_NULL == pst_mac_user)
        {
            return OAL_FALSE;
        }
    }
    /*lint -e644*/
    /*BSS ????????????????Lsig txop protection, ??????Lsig txop protection????????????, AP??STA??????????????*/
    if (((WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode) && (OAL_TRUE == mac_mib_get_LsigTxopFullProtectionActivated(pst_mac_vap)))
         ||((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode) && (OAL_TRUE == pst_mac_user->st_ht_hdl.bit_lsig_txop_protection_full_support)))
    {
        return OAL_TRUE;
    }
    else
    {
        return OAL_FALSE;
    }
    /*lint +e644*/

}


OAL_STATIC oal_uint32 hmac_protection_update_ht_protection(mac_vap_stru *pst_mac_vap)
{
    oal_uint32    ul_ret      = OAL_SUCC;
    oal_bool_enum en_lsigtxop = OAL_FALSE;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*????vap??????????ht ??????????????*/
    if (WLAN_PROT_HT != pst_mac_vap->st_protection.en_protection_mode)
    {
        return OAL_SUCC;
    }

    en_lsigtxop = hmac_protection_lsigtxop_check(pst_mac_vap);

    /*????????????lsigtxop?????? ??????????lsigtxop????*/
    if (OAL_TRUE == en_lsigtxop)
    {
        /*??????????rts cts protection?????? ????????lsig txop protection????*/
        if (OAL_SWITCH_ON == pst_mac_vap->st_protection.bit_rts_cts_protect_mode)
        {
            ul_ret = hmac_protection_set_rtscts_mechanism(pst_mac_vap, OAL_SWITCH_OFF, WLAN_PROT_HT);
            if(OAL_SUCC != ul_ret)
            {
                return ul_ret;
            }

            mac_protection_set_lsig_txop_mechanism(pst_mac_vap, OAL_SWITCH_ON);
        }
    }
    else/*????????????????ht??????????rts cts protection ????*/
    {
        /*??????????rts cts protection?????? ????????lsig txop protection????*/
        if (OAL_SWITCH_ON == pst_mac_vap->st_protection.bit_lsig_txop_protect_mode)
        {
            mac_protection_set_lsig_txop_mechanism(pst_mac_vap, OAL_SWITCH_OFF);
            ul_ret = hmac_protection_set_rtscts_mechanism(pst_mac_vap, OAL_SWITCH_ON, WLAN_PROT_HT);
            if(OAL_SUCC != ul_ret)
	     {
                return ul_ret;
	     }
        }
    }

    return ul_ret;
}


OAL_STATIC oal_uint32 hmac_protection_set_ht_protection(mac_vap_stru *pst_mac_vap, oal_switch_enum_uint8 en_flag)
{
    oal_uint32      ul_ret = OAL_SUCC;
    oal_bool_enum   en_lsigtxop = OAL_FALSE;

    en_lsigtxop = mac_protection_lsigtxop_check(pst_mac_vap);
    /*????????lsigtxop????????????*/
    if (OAL_TRUE == en_lsigtxop)
    {
        mac_protection_set_lsig_txop_mechanism(pst_mac_vap, en_flag);
    }
    else
    {
        ul_ret = hmac_protection_set_rtscts_mechanism(pst_mac_vap, en_flag, WLAN_PROT_HT);
        if(OAL_SUCC != ul_ret)
        {
	     return ul_ret;
	 }
    }

    return ul_ret;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

oal_bool_enum_uint8 hmac_protection_need_sync(mac_vap_stru *pst_mac_vap,
                                mac_h2d_protection_stru *pst_h2d_prot)
{
    mac_h2d_protection_stru *pst_prot_old;
    hmac_vap_stru       *pst_hmac_vap;
    mac_protection_stru *pst_old;
    mac_protection_stru *pst_new;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                    "{hmac_protection_need_sync::null hmac_vap}");
        return OAL_FALSE;
    }
    pst_prot_old = &pst_hmac_vap->st_prot;

    if ( (pst_prot_old->en_dot11HTProtection != pst_h2d_prot->en_dot11HTProtection) ||
         (pst_prot_old->en_dot11RIFSMode != pst_h2d_prot->en_dot11RIFSMode) ||
         (pst_prot_old->en_dot11LSIGTXOPFullProtectionActivated != pst_h2d_prot->en_dot11LSIGTXOPFullProtectionActivated) ||
         (pst_prot_old->en_dot11NonGFEntitiesPresent != pst_h2d_prot->en_dot11NonGFEntitiesPresent)
         )
    {
        oal_memcopy(pst_prot_old, pst_h2d_prot, OAL_SIZEOF(mac_h2d_protection_stru));
        return OAL_TRUE;
    }

    pst_old = &pst_prot_old->st_protection;
    pst_new = &pst_h2d_prot->st_protection;

    if ( (pst_old->en_protection_mode != pst_new->en_protection_mode) ||
         (pst_old->bit_auto_protection != pst_new->bit_auto_protection) ||
         (pst_old->bit_obss_non_erp_present != pst_new->bit_obss_non_erp_present) ||
         (pst_old->bit_obss_non_ht_present != pst_new->bit_obss_non_ht_present) ||
         (pst_old->bit_rts_cts_protect_mode != pst_new->bit_rts_cts_protect_mode) ||
         (pst_old->bit_lsig_txop_protect_mode != pst_new->bit_lsig_txop_protect_mode) ||
         (pst_old->uc_sta_non_ht_num != pst_new->uc_sta_non_ht_num)
      )
    {
        oal_memcopy(pst_prot_old, pst_h2d_prot, OAL_SIZEOF(mac_h2d_protection_stru));
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


OAL_STATIC oal_uint32 hmac_protection_sync_data(mac_vap_stru *pst_mac_vap)
{
    mac_h2d_protection_stru           st_h2d_prot;
    wlan_mib_Dot11OperationEntry_stru *pst_mib;
    oal_uint32                        ul_ret = OAL_SUCC;

    OAL_MEMZERO(&st_h2d_prot, OAL_SIZEOF(st_h2d_prot));

    st_h2d_prot.ul_sync_mask |= H2D_SYNC_MASK_MIB;
    st_h2d_prot.ul_sync_mask |= H2D_SYNC_MASK_PROT;

    oal_memcopy((oal_uint8*)&st_h2d_prot.st_protection, (oal_uint8*)&pst_mac_vap->st_protection,
                OAL_SIZEOF(mac_protection_stru));

    pst_mib = &pst_mac_vap->pst_mib_info->st_wlan_mib_operation;
    st_h2d_prot.en_dot11HTProtection         = pst_mib->en_dot11HTProtection;
    st_h2d_prot.en_dot11RIFSMode             = pst_mib->en_dot11RIFSMode;
    st_h2d_prot.en_dot11LSIGTXOPFullProtectionActivated =
                    pst_mib->en_dot11LSIGTXOPFullProtectionActivated;
    st_h2d_prot.en_dot11NonGFEntitiesPresent = pst_mib->en_dot11NonGFEntitiesPresent;

    if (OAL_TRUE == hmac_protection_need_sync(pst_mac_vap, &st_h2d_prot))
    {
        ul_ret = hmac_config_set_protection(pst_mac_vap, OAL_SIZEOF(st_h2d_prot),
                                        (oal_uint8*)&st_h2d_prot);
    }

    return ul_ret;
}
#endif


OAL_STATIC oal_uint32 hmac_protection_set_mode(
                                 mac_vap_stru *pst_mac_vap,
                                 wlan_prot_mode_enum_uint8 en_prot_mode)
{
    oal_uint32 ul_ret = OAL_SUCC;

    /*??????????????????????????????????*/
    if (en_prot_mode == pst_mac_vap->st_protection.en_protection_mode)
    {
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        ul_ret = hmac_protection_sync_data(pst_mac_vap);
#endif
        return ul_ret;
    }

    /*??????????????????*/
    if (WLAN_PROT_ERP == pst_mac_vap->st_protection.en_protection_mode)
    {
        ul_ret = hmac_protection_set_erp_protection(pst_mac_vap, OAL_SWITCH_OFF);
        if(OAL_SUCC != ul_ret)
        {
		return ul_ret;
	 }
    }
    else if (WLAN_PROT_HT == pst_mac_vap->st_protection.en_protection_mode)
    {
        ul_ret = hmac_protection_set_ht_protection(pst_mac_vap, OAL_SWITCH_OFF);
        if(OAL_SUCC != ul_ret)
        {
		return ul_ret;
	 }
    }
    else
    {
        /*GF????????????????????????*/
    }

    pst_mac_vap->st_protection.en_protection_mode = en_prot_mode;

    /*????????????????*/
    if (WLAN_PROT_ERP == en_prot_mode)
    {
        ul_ret = hmac_protection_set_erp_protection(pst_mac_vap, OAL_SWITCH_ON);
        if(OAL_SUCC != ul_ret)
        {
		return ul_ret;
	 }
    }
    else if (WLAN_PROT_HT == en_prot_mode)
    {
        ul_ret = hmac_protection_set_ht_protection(pst_mac_vap, OAL_SWITCH_ON);
        if(OAL_SUCC != ul_ret)
        {
		return ul_ret;
	 }
    }
    else
    {
        /*GF????????????????????????*/
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    ul_ret = hmac_protection_sync_data(pst_mac_vap);
#else
    /*??????????????????????????????????????????*/
    hmac_config_update_protection_tx_param(pst_mac_vap, OAL_SIZEOF(ul_ret), (oal_uint8*)(&ul_ret)); /*??????????????????*/
#endif

    return ul_ret;
}



oal_uint32 hmac_protection_update_mib_ap(mac_vap_stru *pst_mac_vap)
{
    oal_uint32           ul_ret         = OAL_SUCC;
    mac_protection_stru *pst_protection = OAL_PTR_NULL;
    oal_bool_enum_uint8  en_lsig_txop_full_protection_activated;
    oal_bool_enum_uint8  en_non_gf_entities_present;
    oal_bool_enum_uint8  en_rifs_mode;
    oal_bool_enum_uint8  en_ht_protection;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_protection = &(pst_mac_vap->st_protection);

    /*????vap??en_dot11NonGFEntitiesPresent????*/
    en_non_gf_entities_present = (0 != pst_protection->uc_sta_non_gf_num) ? OAL_TRUE : OAL_FALSE;
    mac_mib_set_NonGFEntitiesPresent(pst_mac_vap, en_non_gf_entities_present);

    /*????vap??en_dot11LSIGTXOPFullProtectionActivated????*/
    en_lsig_txop_full_protection_activated = (0 == pst_protection->uc_sta_no_lsig_txop_num) ? OAL_TRUE : OAL_FALSE;

    mac_mib_set_LsigTxopFullProtectionActivated(pst_mac_vap, en_lsig_txop_full_protection_activated);

    ul_ret = hmac_protection_update_ht_protection(pst_mac_vap);
    if(OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_protection_update_mib_ap::hmac_protection_update_ht_protection fail.err code %u}",ul_ret);
        return ul_ret;
    }

    /*????vap??en_dot11HTProtection??en_dot11RIFSMode????*/
    if (0 != pst_protection->uc_sta_non_ht_num)
    {
        en_ht_protection = WLAN_MIB_HT_NON_HT_MIXED;
        en_rifs_mode     = OAL_FALSE;
    }
    else if (OAL_TRUE == pst_protection->bit_obss_non_ht_present)
    {
        en_ht_protection = WLAN_MIB_HT_NONMEMBER_PROTECTION;
        en_rifs_mode     = OAL_FALSE;
    }
    else if ((WLAN_BAND_WIDTH_20M != pst_mac_vap->st_channel.en_bandwidth)
               && (0 != pst_protection->uc_sta_20M_only_num))
    {
        en_ht_protection = WLAN_MIB_HT_20MHZ_PROTECTION;
        en_rifs_mode     = OAL_TRUE;
    }
    else
    {
        en_ht_protection = WLAN_MIB_HT_NO_PROTECTION;
        en_rifs_mode     = OAL_TRUE;
    }

    mac_mib_set_HtProtection(pst_mac_vap, en_ht_protection);
    mac_mib_set_RifsMode(pst_mac_vap, en_rifs_mode);

    ul_ret = hmac_protection_update_mode_ap(pst_mac_vap);
    return ul_ret;
}


oal_uint32 hmac_protection_update_mode_ap(mac_vap_stru *pst_mac_vap)
{
    oal_uint32                ul_ret             = OAL_SUCC;
    wlan_prot_mode_enum_uint8 en_protection_mode = WLAN_PROT_NO;
    mac_protection_stru      *pst_protection     = OAL_PTR_NULL;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_protection = &(pst_mac_vap->st_protection);

    /*???????????????????? ????????*/
    if (OAL_SWITCH_OFF == mac_vap_protection_autoprot_is_enabled(pst_mac_vap))
    {
        return OAL_SUCC;
    }

    /*??2G??????????????non erp??????AP?????? ????OBSS??????non erp?????? ??????erp????*/
    if ((WLAN_BAND_2G == pst_mac_vap->st_channel.en_band)
         && ((0 != pst_protection->uc_sta_non_erp_num) || (OAL_TRUE == pst_protection->bit_obss_non_erp_present)))
    {
        en_protection_mode = WLAN_PROT_ERP;
    }
    /*??????non ht??????AP?????? ????OBSS??????non ht?????? ??????ht????*/
    else if ((0 != pst_protection->uc_sta_non_ht_num) || (OAL_TRUE == pst_protection->bit_obss_non_ht_present))
    {
        en_protection_mode = WLAN_PROT_HT;
    }
    /*??????non gf??????AP?????? ??????gf????*/
    else if (0 != pst_protection->uc_sta_non_gf_num)
    {
        en_protection_mode = WLAN_PROT_GF;
    }
    /*??????????????????*/
    else
    {
        en_protection_mode = WLAN_PROT_NO;
    }

    /*????????????????*/
    ul_ret = hmac_protection_set_mode(pst_mac_vap, en_protection_mode);

    return ul_ret;
}


oal_uint32 hmac_protection_update_mode_sta(mac_vap_stru *pst_mac_vap_sta, hmac_user_stru *pst_hmac_user)
{
    wlan_prot_mode_enum_uint8 en_protection_mode = WLAN_PROT_NO;

    if ((OAL_PTR_NULL == pst_mac_vap_sta) || (OAL_PTR_NULL == pst_hmac_user))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*???????????????????? ????????*/
    if (OAL_SWITCH_OFF == mac_vap_protection_autoprot_is_enabled(pst_mac_vap_sta))
    {
        return OAL_SUCC;
    }

    en_protection_mode = mac_vap_get_user_protection_mode(pst_mac_vap_sta, &(pst_hmac_user->st_user_base_info));

    /*????????????????*/
    return hmac_protection_set_mode(pst_mac_vap_sta, en_protection_mode);
}


oal_uint32 hmac_protection_obss_aging_ap(mac_vap_stru *pst_mac_vap)
{
    oal_uint32    ul_ret               = OAL_SUCC;
    oal_bool_enum em_update_protection = OAL_FALSE; /*????????????????vap??protection*/

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*????ERP????????*/
    if (OAL_TRUE == pst_mac_vap->st_protection.bit_obss_non_erp_present)
    {
        pst_mac_vap->st_protection.uc_obss_non_erp_aging_cnt++;
        if (pst_mac_vap->st_protection.uc_obss_non_erp_aging_cnt >= WLAN_PROTECTION_NON_ERP_AGING_THRESHOLD)
        {
            pst_mac_vap->st_protection.bit_obss_non_erp_present = OAL_FALSE;
            em_update_protection = OAL_TRUE;
            pst_mac_vap->st_protection.uc_obss_non_erp_aging_cnt = 0;
        }
    }

    /*????HT????????*/
    if (OAL_TRUE == pst_mac_vap->st_protection.bit_obss_non_ht_present)
    {
        pst_mac_vap->st_protection.uc_obss_non_ht_aging_cnt++;

        if (pst_mac_vap->st_protection.uc_obss_non_ht_aging_cnt >= WLAN_PROTECTION_NON_HT_AGING_THRESHOLD)
        {
            pst_mac_vap->st_protection.bit_obss_non_ht_present = OAL_FALSE;
            em_update_protection = OAL_TRUE;
            pst_mac_vap->st_protection.uc_obss_non_ht_aging_cnt = 0;
        }
    }

    /*????????????????*/
    if(OAL_TRUE == em_update_protection)
    {
        ul_ret = hmac_protection_update_mib_ap(pst_mac_vap);
    }

    return ul_ret;
}


OAL_STATIC oal_uint32  hmac_protection_del_user_stat_legacy_ap(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    mac_protection_stru    *pst_protection = &(pst_mac_vap->st_protection);
    hmac_user_stru         *pst_hmac_user;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_user->us_assoc_id);
    if (OAL_PTR_NULL == pst_hmac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                        "hmac_protection_del_user_stat_legacy_ap::Get Hmac_user(idx=%d) NULL POINT!", pst_mac_user->us_assoc_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*??????????????????????ERP*/
    if ((OAL_FALSE == pst_hmac_user->st_hmac_cap_info.bit_erp)
        && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_erp_stats_flag)
        && (0 != pst_protection->uc_sta_non_erp_num))
    {
        pst_protection->uc_sta_non_erp_num--;
    }

    /*??????????????????????short preamble*/
    if ((OAL_FALSE == pst_hmac_user->st_hmac_cap_info.bit_short_preamble)
        && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_short_preamble_stats_flag)
        && (0 != pst_protection->uc_sta_no_short_preamble_num))
    {
        pst_protection->uc_sta_no_short_preamble_num--;
    }

    /*??????????????????????short slot*/
    if ((OAL_FALSE == pst_hmac_user->st_hmac_cap_info.bit_short_slot_time)
        && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_short_slot_stats_flag)
        && (0 != pst_protection->uc_sta_no_short_slot_num))
    {
        pst_protection->uc_sta_no_short_slot_num--;
    }

    pst_hmac_user->st_user_stats_flag.bit_no_short_slot_stats_flag     = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_no_short_preamble_stats_flag = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_no_erp_stats_flag            = OAL_FALSE;

    return OAL_SUCC;
}



OAL_STATIC oal_uint32  hmac_protection_del_user_stat_ht_ap(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    mac_user_ht_hdl_stru   *pst_ht_hdl     = &(pst_mac_user->st_ht_hdl);
    mac_protection_stru    *pst_protection = &(pst_mac_vap->st_protection);
    hmac_user_stru         *pst_hmac_user;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_user->us_assoc_id);
    if (OAL_PTR_NULL == pst_hmac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                        "hmac_protection_del_user_stat_ht_ap::Get Hmac_user(idx=%d) NULL POINT!", pst_mac_user->us_assoc_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*??????????????????????HT*/
    if ((OAL_FALSE == pst_ht_hdl->en_ht_capable)
        && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_ht_stats_flag)
        && (0 != pst_protection->uc_sta_non_ht_num))
    {
        pst_protection->uc_sta_non_ht_num--;
    }
    else /*????HT*/
    {
        /*??????????????????????20/40Mhz????*/
        if ((OAL_FALSE == pst_ht_hdl->bit_supported_channel_width)
            && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_20M_only_stats_flag)
            && (0 != pst_protection->uc_sta_20M_only_num))
        {
            pst_protection->uc_sta_20M_only_num--;
        }

        /*??????????????????????GF*/
        if ((OAL_FALSE == pst_ht_hdl->bit_ht_green_field)
            && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_gf_stats_flag)
            && (0 != pst_protection->uc_sta_non_gf_num))
        {
            pst_protection->uc_sta_non_gf_num--;
        }

        /*??????????????????????L-SIG TXOP Protection*/
        if ((OAL_FALSE == pst_ht_hdl->bit_lsig_txop_protection)
            && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_lsig_txop_stats_flag)
            && (0 != pst_protection->uc_sta_no_lsig_txop_num))
        {
            pst_protection->uc_sta_no_lsig_txop_num--;
        }

        /*??????????????????????40Mhz cck*/
        if ((OAL_FALSE == pst_ht_hdl->bit_dsss_cck_mode_40mhz)
             && (OAL_TRUE == pst_ht_hdl->bit_supported_channel_width)
             && (OAL_TRUE == pst_hmac_user->st_user_stats_flag.bit_no_40dsss_stats_flag)
             && (0 != pst_protection->uc_sta_no_40dsss_cck_num))
        {
            pst_protection->uc_sta_no_40dsss_cck_num--;
        }
    }

    pst_hmac_user->st_user_stats_flag.bit_no_ht_stats_flag             = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_no_gf_stats_flag             = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_20M_only_stats_flag          = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_no_40dsss_stats_flag         = OAL_FALSE;
    pst_hmac_user->st_user_stats_flag.bit_no_lsig_txop_stats_flag      = OAL_FALSE;

    return OAL_SUCC;
}



OAL_STATIC oal_void  hmac_protection_del_user_stat_ap(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    hmac_protection_del_user_stat_legacy_ap(pst_mac_vap, pst_mac_user);
    hmac_protection_del_user_stat_ht_ap(pst_mac_vap, pst_mac_user);


}


oal_uint32 hmac_protection_del_user(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    oal_uint32 ul_ret = OAL_SUCC;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == pst_mac_user))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*AP ????VAP??????????????????????????*/
    if (WLAN_VAP_MODE_BSS_AP == pst_mac_vap->en_vap_mode)
    {
        /*????????????????user????*/
        hmac_protection_del_user_stat_ap(pst_mac_vap, pst_mac_user);

        /*????AP??????????mib??*/
        ul_ret = hmac_protection_update_mib_ap(pst_mac_vap);
        if(OAL_SUCC != ul_ret)
        {
		return ul_ret;
	 }
    }
    /*????STA????????????*/
    else if (WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)
    {
        ul_ret = hmac_protection_set_mode(pst_mac_vap, WLAN_PROT_NO);
    }

    return ul_ret;
}


oal_uint32  hmac_protection_obss_update_timer(void *p_arg)
{
    mac_device_stru     *pst_mac_device;
    oal_uint8            uc_vap_idx;
    mac_vap_stru        *pst_mac_vap;

    if (OAL_PTR_NULL == p_arg)
    {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_protection_obss_update_timer::p_arg null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = (mac_device_stru *)p_arg;

    /* ????device??????VAP, ????????OBSS ???????? */
    /* ????vap??1???? */
    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++)
    {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (OAL_UNLIKELY(OAL_PTR_NULL == pst_mac_vap))
        {
            OAM_WARNING_LOG0(uc_vap_idx, OAM_SF_ANY, "{hmac_protection_obss_update_timer::pst_mac_vap null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* OBSS??????????AP????????AP?????????? */
        if (WLAN_VAP_MODE_BSS_AP != pst_mac_vap->en_vap_mode)
        {
            continue;
        }

        hmac_protection_obss_aging_ap(pst_mac_vap);
    }

    return OAL_SUCC;
}


oal_uint32 hmac_protection_start_timer(hmac_vap_stru  *pst_hmac_vap)
{
    mac_device_stru                *pst_mac_device;

    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_protection_start_timer::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????OBSS?????????????? ?????????????????????????? */
    if (OAL_FALSE == pst_mac_device->st_obss_aging_timer.en_is_registerd)
    {
        FRW_TIMER_CREATE_TIMER(&(pst_mac_device->st_obss_aging_timer),
                               hmac_protection_obss_update_timer,
                               WLAN_USER_AGING_TRIGGER_TIME,                    /* 5000ms???????? */
                               pst_mac_device,
                               OAL_TRUE,
                               OAM_MODULE_ID_HMAC,
                               pst_mac_device->ul_core_id);
    }

    return OAL_SUCC;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

