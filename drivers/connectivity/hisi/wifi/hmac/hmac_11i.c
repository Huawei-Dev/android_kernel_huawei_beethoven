


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oal_types.h"
#include "oal_net.h"
#include "oal_aes.h"
#include "frw_ext_if.h"
#include "wlan_spec.h"
#include "wlan_types.h"
#include "mac_resource.h"
#include "mac_frame.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_vap.h"
//#include "mac_11i.h"
#include "hmac_11i.h"
#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif
#include "hmac_main.h"
#include "hmac_crypto_tkip.h"
#include "hmac_config.h"
#ifdef _PRE_WLAN_FEATURE_ROAM
#include "hmac_roam_main.h"
#endif //_PRE_WLAN_FEATURE_ROAM

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11i_C

#define CIPHER_SUITE_NO_ENCRYP(i) ((i & WLAN_ENCRYPT_BIT) ? 0 : 1)
#define CIPHER_SUITE_IS_WEP104(i) (((i & WLAN_WEP104_BIT) == WLAN_WEP104_BIT) ? 1 : 0)
#define CIPHER_SUITE_IS_WEP40(i)  (((i & WLAN_WEP104_BIT) == WLAN_WEP_BIT) ? 1 : 0)
#define CIPHER_SUITE_IS_WPA(i)    (((i & WLAN_WPA_BIT) == WLAN_WPA_BIT) ? 1 : 0)
#define CIPHER_SUITE_IS_WPA2(i)   (((i & WLAN_WPA2_BIT) == WLAN_WPA2_BIT) ? 1 : 0)
#define CIPHER_SUITE_IS_TKIP(i)   (((i & WLAN_TKIP_BIT) == WLAN_TKIP_BIT) ? 1 : 0)
#define CIPHER_SUITE_IS_CCMP(i)   (((i & WLAN_CCMP_BIT) == WLAN_CCMP_BIT) ? 1 : 0)



/*****************************************************************************
  2 ????????????
*****************************************************************************/


/*****************************************************************************
  3 ????????
*****************************************************************************/


OAL_STATIC wlan_priv_key_param_stru *hmac_get_key_info(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr,
                                        oal_bool_enum_uint8 en_pairwise,oal_uint8 uc_key_index)
{
    oal_uint16                  us_user_idx          = 0;
    oal_uint32                  ul_ret               = OAL_SUCC;
    mac_user_stru              *pst_mac_user = OAL_PTR_NULL;
    oal_bool_enum_uint8         en_macaddr_is_zero;

    /*1.1 ????mac addr ????????sta??????*/
    en_macaddr_is_zero = mac_addr_is_zero(puc_mac_addr);

    if(!MAC_11I_IS_PTK(en_macaddr_is_zero, en_pairwise))
    {
        /* ????????????????????????mac?????????? */

        /* ????????????????user???????? */
        us_user_idx = pst_mac_vap->us_multi_user_idx;
    }
    else  /* ???????? */
    {
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                             "{hmac_get_key_info::mac_vap_find_user_by_macaddr failed[%d]}", ul_ret);
            return OAL_PTR_NULL;
        }
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(us_user_idx);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_get_key_info::mac_res_get_mutil_mac_user null.}");
        return OAL_PTR_NULL;
    }

    /*LOG*/
    OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_get_key_info::key_index=%d,pairwise=%d.}", uc_key_index, en_pairwise);

    if (OAL_PTR_NULL != puc_mac_addr)
    {
        OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                      "{hmac_get_key_info::mac_addr[%d] = %02X:XX:XX:XX:%02X:%02X.}",
                      us_user_idx, puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]);
    }

    return mac_user_get_key(pst_mac_user, uc_key_index);
}
#ifdef _PRE_WLAN_FEATURE_WAPI

oal_uint32 hmac_config_wapi_add_key(mac_vap_stru *pst_mac_vap, mac_addkey_param_stru *pst_payload_addkey_params)
{
    oal_uint8                        uc_key_index;
    oal_bool_enum_uint8              en_pairwise;
    oal_uint8                       *puc_mac_addr;
    mac_key_params_stru             *pst_key_param;
    hmac_wapi_stru                  *pst_wapi;
    //hmac_wapi_key_stru              *pst_key;
    oal_uint32                       ul_ret;
    oal_uint16                       us_user_index = 0;
    mac_device_stru                 *pst_mac_device;

    uc_key_index = pst_payload_addkey_params->uc_key_index;
    if(HMAC_WAPI_MAX_KEYID <= uc_key_index)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_config_wapi_add_key::keyid==%u Err!.}", uc_key_index);
        return OAL_FAIL;
    }

    en_pairwise  = pst_payload_addkey_params->en_pairwise;
    puc_mac_addr = (oal_uint8*)pst_payload_addkey_params->auc_mac_addr;
    pst_key_param   = &pst_payload_addkey_params->st_key;

    if ((WAPI_KEY_LEN * 2) != pst_key_param->key_len)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_config_wapi_add_key:: key_len %u Err!.}", pst_key_param->key_len);
        return OAL_FAIL;
    }

    if(en_pairwise)
    {
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_index);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_config_wapi_add_key::mac_vap_find_user_by_macaddr failed. %u}", ul_ret);
            return OAL_PTR_NULL;
        }
    }


    pst_wapi = hmac_user_get_wapi_ptr(pst_mac_vap, en_pairwise, us_user_index);
    if (OAL_PTR_NULL == pst_wapi)
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_config_wapi_add_key:: get pst_wapi  Err!.}");
        return OAL_FAIL;
    }

    hmac_wapi_add_key(pst_wapi, uc_key_index, pst_key_param->auc_key);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_wapi_add_key::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device->uc_wapi = OAL_TRUE;

    return OAL_SUCC;
}


oal_uint32 hmac_config_wapi_add_key_and_sync(mac_vap_stru *pst_mac_vap, mac_addkey_param_stru *pst_payload_addkey_params)
{
    hmac_vap_stru                   *pst_hmac_vap;
    oal_uint32                       ul_ret;

    OAM_WARNING_LOG2(0, OAM_SF_WPA,"{hmac_config_wapi_add_key_and_sync:: key idx==%u, pairwise==%u}", pst_payload_addkey_params->uc_key_index, pst_payload_addkey_params->en_pairwise);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_config_11i_add_key::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#if 0
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac_config_wapi_add_key_and_sync::wapi==%u!}", WAPI_IS_WORK(pst_hmac_vap));
    if(!WAPI_IS_WORK(pst_hmac_vap))
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::wapi is not working!}");
        return OAL_SUCC;
    }
#endif
    ul_ret = hmac_config_wapi_add_key(&pst_hmac_vap->st_vap_base_info, pst_payload_addkey_params);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::hmac_config_wapi_add_key fail[%d].}", ul_ret);
        return ul_ret;
    }

    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_WAPI_KEY, 0, OAL_PTR_NULL);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::WLAN_CFGID_ADD_WAPI_KEY send fail[%d].}", ul_ret);
        return ul_ret;
    }

    return ul_ret;
}

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */


oal_uint32 hmac_config_11i_add_key(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    oal_uint8                       *puc_mac_addr;
    hmac_user_stru                  *pst_hmac_user;
    hmac_vap_stru                   *pst_hmac_vap;
    mac_key_params_stru             *pst_key;
    mac_addkey_param_stru           *pst_payload_addkey_params;
    oal_uint32                       ul_ret = OAL_SUCC;
    oal_uint16                       us_user_idx;
    oal_bool_enum_uint8              en_pairwise;
    oal_uint8                        uc_key_index;

#ifdef _PRE_WLAN_FEATURE_WAPI
    mac_device_stru                 *pst_mac_device;
#endif

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_WPA, "{hmac_config_11i_add_key::param null,pst_mac_vap=%d, puc_param=%d.}",
                       pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_11i_add_key::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*2.1 ????????*/
    pst_payload_addkey_params = (mac_addkey_param_stru *)puc_param;
    uc_key_index = pst_payload_addkey_params->uc_key_index;
    en_pairwise  = pst_payload_addkey_params->en_pairwise;
    puc_mac_addr = (oal_uint8*)pst_payload_addkey_params->auc_mac_addr;
    pst_key      = &(pst_payload_addkey_params->st_key);

#ifdef _PRE_WLAN_FEATURE_WAPI
    if (OAL_UNLIKELY(WLAN_CIPHER_SUITE_SMS4 == pst_key->cipher))
    {
        return hmac_config_wapi_add_key_and_sync(pst_mac_vap, pst_payload_addkey_params);
    }
#endif

    /*2.2 ????????????????*/
    if(uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::invalid uc_key_index[%d].}", uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }


    OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                 "{hmac_config_11i_add_key::key_index=%d, pairwise=%d.}",
                 uc_key_index, en_pairwise);
    OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                 "{hmac_config_11i_add_key::pst_params cipher=0x%08x, keylen=%d, seqlen=%d.}",
                 pst_key->cipher, pst_key->key_len, pst_key->seq_len);



	OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::mac addr=%02X:XX:XX:%02X:%02X:%02X}",
                  puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]);

    if (OAL_TRUE == en_pairwise)
    {
        /* ???????????????????????? */
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::find_user_by_macaddr fail[%d].}", ul_ret);
            return ul_ret;
        }
    }
    else
    {
        /* ???????????????????????? */
        us_user_idx = pst_mac_vap->us_multi_user_idx;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (OAL_PTR_NULL == pst_hmac_user)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::get_mac_user null.idx:%u}",us_user_idx);
        return OAL_ERR_CODE_SECURITY_USER_INVAILD;
    }

    // add_key??????????user????????????????????????????rekey????????????????
    hmac_user_clear_defrag_res(pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_WAPI
    /* 11i??????????????wapi???? */
    hmac_wapi_reset_port(&pst_hmac_user->st_wapi);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_11i_add_key::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device->uc_wapi = OAL_FALSE;
#endif


    /*3.1 ??????????????????????*/
    ul_ret = mac_vap_add_key(pst_mac_vap,  &pst_hmac_user->st_user_base_info, uc_key_index, pst_key);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_key::mac_11i_add_key fail[%d].}", ul_ret);
        return ul_ret;
    }

    /* ????????8021x?????????????????????? */
    mac_user_set_port(&pst_hmac_user->st_user_base_info, OAL_TRUE);

#ifdef _PRE_WLAN_FEATURE_ROAM
    if ((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode)  &&
        (MAC_VAP_STATE_ROAMING == pst_mac_vap->en_vap_state) &&
        (OAL_FALSE == en_pairwise))
    {
        hmac_roam_add_key_done(pst_hmac_vap);
    }
#endif //_PRE_WLAN_FEATURE_ROAM
    /***************************************************************************
    ????????DMAC??, ????DMAC????
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_KEY, us_len, puc_param);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_add_key::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return ul_ret;
}


oal_uint32 hmac_config_11i_get_key(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    wlan_priv_key_param_stru     *pst_priv_key = OAL_PTR_NULL;
    oal_key_params_stru           st_key;
    oal_uint8                     uc_key_index;
    oal_bool_enum_uint8           en_pairwise;
    oal_uint8                    *puc_mac_addr = OAL_PTR_NULL;
    void                         *cookie;
    void                        (*callback)(void*, oal_key_params_stru*);
    mac_getkey_param_stru        *pst_payload_getkey_params = OAL_PTR_NULL;


    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_WPA,
                      "{hmac_config_11i_get_key::param null, pst_mac_vap=%d, puc_param=%d.}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*2.1 ????????*/
    pst_payload_getkey_params = (mac_getkey_param_stru *)puc_param;
    uc_key_index = pst_payload_getkey_params->uc_key_index;
    en_pairwise  = pst_payload_getkey_params->en_pairwise;
    puc_mac_addr = pst_payload_getkey_params->puc_mac_addr;
    cookie       = pst_payload_getkey_params->cookie;
    callback     = pst_payload_getkey_params->callback;

    /*2.2 ????????????????*/
    if(uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_get_key::uc_key_index invalid[%d].}", uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    /*3.1 ????????*/
    pst_priv_key = hmac_get_key_info(pst_mac_vap, puc_mac_addr, en_pairwise, uc_key_index);
    if (OAL_PTR_NULL == pst_priv_key)
    {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_get_key::key is null.pairwise[%d], key_idx[%d]}",
                    en_pairwise, uc_key_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (0 == pst_priv_key->ul_key_len)
    {
        OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_get_key::key len = 0.pairwise[%d], key_idx[%d]}",
                    en_pairwise, uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }


    /*4.1 ????????????*/
    oal_memset(&st_key, 0, sizeof(st_key));
    st_key.key     = pst_priv_key->auc_key;
    st_key.key_len = (oal_int32)pst_priv_key->ul_key_len;
    st_key.seq     = pst_priv_key->auc_seq;
    st_key.seq_len = (oal_int32)pst_priv_key->ul_seq_len;
    st_key.cipher  = pst_priv_key->ul_cipher;

    /*5.1 ????????????*/
    if (callback)
    {
        callback(cookie, &st_key);
    }

    return OAL_SUCC;
}


oal_uint32 hmac_config_11i_remove_key(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    wlan_priv_key_param_stru  *pst_priv_key = OAL_PTR_NULL;
    oal_uint32                 ul_ret       = OAL_SUCC;
    oal_uint8                  uc_key_index;
    oal_bool_enum_uint8        en_pairwise;
    oal_uint8                 *puc_mac_addr;
    mac_removekey_param_stru  *pst_payload_removekey_params = OAL_PTR_NULL;
    wlan_cfgid_enum_uint16     en_cfgid;
	mac_user_stru             *pst_mac_user;
    oal_bool_enum_uint8        en_macaddr_is_zero;

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_WPA, "{hmac_config_11i_remove_key::param null,pst_mac_vap=%d, puc_param=%d.}", pst_mac_vap, puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*2.1 ????????*/
    pst_payload_removekey_params = (mac_removekey_param_stru *)puc_param;
    uc_key_index = pst_payload_removekey_params->uc_key_index;
    en_pairwise  = pst_payload_removekey_params->en_pairwise;
    puc_mac_addr = pst_payload_removekey_params->auc_mac_addr;

    OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_remove_key::uc_key_index=%d, en_pairwise=%d.}",
                  uc_key_index, en_pairwise);

    /*2.2 ????????????????*/
    if(uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK)
    {
        /* ??????????????6 ????????????????????6?????????????????? */
        /* ??????????key idx > ???????????????????? */
        OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_remove_key::invalid uc_key_index[%d].}", uc_key_index);
        return OAL_SUCC;
    }

    /*3.1 ????????????????*/
    pst_priv_key = hmac_get_key_info(pst_mac_vap, puc_mac_addr, en_pairwise, uc_key_index);
    if (OAL_PTR_NULL == pst_priv_key)
    {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_remove_key::pst_priv_key null.}");
        return OAL_ERR_CODE_SECURITY_USER_INVAILD;
    }

    if (0 == pst_priv_key->ul_key_len)
    {
        /* ???????????????????????? ?????????????? */
        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_remove_key::ul_key_len=0.}");
        return OAL_SUCC;
    }

    /*4.1 ??????wep????wpa*/
    if ((WLAN_CIPHER_SUITE_WEP40 == pst_priv_key->ul_cipher) || (WLAN_CIPHER_SUITE_WEP104 == pst_priv_key->ul_cipher))
    {
		mac_mib_set_wep(pst_mac_vap, uc_key_index);
        en_cfgid = WLAN_CFGID_REMOVE_WEP_KEY;
    }
    else
    {
		en_macaddr_is_zero = mac_addr_is_zero(puc_mac_addr);
        if(MAC_11I_IS_PTK(en_macaddr_is_zero, en_pairwise))
        {
            pst_mac_user = mac_vap_get_user_by_addr(pst_mac_vap, puc_mac_addr);
            if (OAL_PTR_NULL == pst_mac_user)
            {
                return OAL_ERR_CODE_SECURITY_USER_INVAILD;
            }
            pst_mac_user->st_user_tx_info.st_security.en_cipher_key_type = HAL_KEY_TYPE_BUTT;
        }
        else
        {
            pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->us_multi_user_idx);
            if (OAL_PTR_NULL == pst_mac_user)
            {
                return OAL_ERR_CODE_SECURITY_USER_INVAILD;
            }
        }
        en_cfgid = WLAN_CFGID_REMOVE_KEY;
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
        if (!mac_vap_is_vsta(pst_mac_vap))
#endif
        {
            mac_user_set_port(pst_mac_user, OAL_FALSE);
            mac_user_init_key(pst_mac_user);
        }
    }

    /*4.2 ????????dmac??????*/
    ul_ret = hmac_config_send_event(pst_mac_vap, en_cfgid, us_len, puc_param);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                      "{hmac_config_11i_remove_key::hmac_config_send_event failed[%d], en_cfgid=%d .}", ul_ret, en_cfgid);
        return ul_ret;
    }

    /* 5.1 ????????????????????????????0 */
    pst_priv_key->ul_key_len = 0;

    return ul_ret;
}


oal_uint32 hmac_config_11i_set_default_key(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    oal_uint32                    ul_ret       = OAL_SUCC;
    oal_uint8                     uc_key_index = 0;
    oal_bool_enum_uint8           en_unicast   = OAL_FALSE;
    oal_bool_enum_uint8           en_multicast = OAL_FALSE;
    mac_setdefaultkey_param_stru *pst_payload_setdefaultkey_params;

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == puc_param))
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_config_11i_set_default_key::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*2.1 ????????*/
    pst_payload_setdefaultkey_params = (mac_setdefaultkey_param_stru *)puc_param;
    uc_key_index = pst_payload_setdefaultkey_params->uc_key_index;
    en_unicast   = pst_payload_setdefaultkey_params->en_unicast;
    en_multicast = pst_payload_setdefaultkey_params->en_multicast;

    /*2.2 ????????????????*/
    if(uc_key_index >= (WLAN_NUM_TK + WLAN_NUM_IGTK))
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_set_default_key::invalid uc_key_index[%d].}",
                       uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    /*2.3 ??????????????*/
    if ((OAL_FALSE == en_multicast) && (OAL_FALSE == en_unicast))
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_set_default_key::not ptk or gtk,invalid mode.}");
        return OAL_ERR_CODE_SECURITY_PARAMETERS;
    }

    if (uc_key_index >= WLAN_NUM_TK)
    {
        /*3.1 ????default mgmt key????*/
        ul_ret = mac_vap_set_default_mgmt_key(pst_mac_vap, uc_key_index);
    }
    else
    {
        ul_ret = mac_vap_set_default_key(pst_mac_vap, uc_key_index);
    }

    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_set_default_key::set key[%d] failed[%d].}", uc_key_index, ul_ret);
        return ul_ret;
    }

    /***************************************************************************
    ????????DMAC??, ????DMAC????
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_DEFAULT_KEY, us_len, puc_param);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_set_default_key::hmac_config_send_event failed[%d].}", ul_ret);
    }
    OAM_INFO_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_set_default_key::key_id[%d] un[%d] mu[%d] OK}",
                  uc_key_index, en_unicast, en_multicast);

    return ul_ret;
}


oal_uint32 hmac_config_11i_add_wep_entry(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param)
{
    mac_user_stru                    *pst_mac_user;
    oal_uint32                        ul_ret;

    if (OAL_PTR_NULL == pst_mac_vap || OAL_PTR_NULL == puc_param)
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_config_11i_add_wep_entry::PARMA NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user =  (mac_user_stru *)mac_vap_get_user_by_addr(pst_mac_vap, puc_param);
    if (OAL_PTR_NULL == pst_mac_user)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_wep_entry::mac_user NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_user_update_wep_key(pst_mac_user, pst_mac_vap->us_multi_user_idx);
    if (OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_add_wep_entry::mac_wep_add_usr_key failed[%d].}", ul_ret);
        return ul_ret;
    }
    /***************************************************************************
    ????????DMAC??, ????DMAC????
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_WEP_ENTRY, us_len, puc_param);
    if (OAL_UNLIKELY(OAL_SUCC != ul_ret))
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_add_wep_entry::hmac_config_send_event failed[%d].}", ul_ret);
    }

    /* ??????????????????????*/
    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                   "{hmac_config_11i_add_wep_entry:: usridx[%d] OK.}", pst_mac_user->us_assoc_id);

    return ul_ret;
}


oal_uint32 hmac_init_security(mac_vap_stru *pst_mac_vap,oal_uint8 *puc_addr)
{
    oal_uint32 ul_ret = OAL_SUCC;
    oal_uint16       us_len;
    oal_uint8       *puc_param;

    if (OAL_PTR_NULL == pst_mac_vap)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_TRUE == mac_is_wep_enabled(pst_mac_vap))
    {
        puc_param = puc_addr;
        us_len = WLAN_MAC_ADDR_LEN;
        ul_ret = hmac_config_11i_add_wep_entry(pst_mac_vap, us_len, puc_param);
    }
    return ul_ret;
}



oal_bool_enum_uint8 hmac_check_pcip_wpa_policy(wlan_mib_ieee802dot11_stru *pst_mib_info,
                                            oal_uint8 uc_policy)
{
    oal_uint8 uc_loop = 0;
    for (uc_loop = 0; uc_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; uc_loop++)
    {
        /* ?????????????????????????? */
        if ((pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[uc_loop].en_dot11RSNAConfigPairwiseCipherActivated == OAL_TRUE)
            && (pst_mib_info->ast_wlan_mib_rsna_cfg_wpa_pairwise_cipher[uc_loop].uc_dot11RSNAConfigPairwiseCipherImplemented == uc_policy))
        {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}


oal_bool_enum_uint8 hmac_check_pcip_wpa2_policy(wlan_mib_ieee802dot11_stru *pst_mib_info,
                                            oal_uint8 uc_policy)
{
    oal_uint8 uc_loop = 0;
    for (uc_loop = 0; uc_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; uc_loop++)
    {
        /* ?????????????????????????? */
        if ((pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[uc_loop].en_dot11RSNAConfigPairwiseCipherActivated == OAL_TRUE)
            && (pst_mib_info->ast_wlan_mib_rsna_cfg_wpa2_pairwise_cipher[uc_loop].uc_dot11RSNAConfigPairwiseCipherImplemented == uc_policy))
        {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}



oal_uint32 hmac_check_pcip_policy(wlan_mib_ieee802dot11_stru *pst_mib_info,
                                            oal_uint8 uc_policy,
                                            oal_uint8 uc_80211i_mode)
{
    oal_bool_enum_uint8 en_ret = OAL_FALSE;

    if (DMAC_WPA_802_11I == uc_80211i_mode)
    {
        en_ret = hmac_check_pcip_wpa_policy(pst_mib_info, uc_policy);
    }
    else if (DMAC_RSNA_802_11I == uc_80211i_mode)
    {
        en_ret = hmac_check_pcip_wpa2_policy(pst_mib_info, uc_policy);
    }
    else
    {
        en_ret = OAL_FALSE;
    }

    if (OAL_TRUE == en_ret)
    {
        return OAL_SUCC;
    }
    else
    {
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }
}



oal_uint32 hmac_check_rsn_capability(mac_vap_stru *pst_mac_vap, const oal_uint8 *puc_rsn_ie, mac_status_code_enum_uint16 *pen_status_code)
{
    wlan_mib_ieee802dot11_stru         *pst_mib_info;
    oal_uint16                          us_rsn_capability;
    oal_bool_enum_uint8                 en_PreauthActivated;
    oal_bool_enum_uint8                 en_dot11RSNAMFPR;
    oal_bool_enum_uint8                 en_dot11RSNAMFPC;


    /*1.1 ????????*/
    if (OAL_PTR_NULL == pst_mac_vap)
    {
        *pen_status_code = MAC_INVALID_RSN_INFO_CAP;
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_check_rsn_capability::pst_mac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mib_info = pst_mac_vap->pst_mib_info;
    if (OAL_PTR_NULL == pst_mib_info)
    {
        *pen_status_code = MAC_INVALID_RSN_INFO_CAP;
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_check_rsn_capability::pst_mib_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }


    us_rsn_capability = mac_get_rsn_capability(puc_rsn_ie);

    /*2.1 ??????????????*/
    en_PreauthActivated = us_rsn_capability & BIT0;
    if (pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAPreauthenticationActivated != en_PreauthActivated)
    {
        *pen_status_code = MAC_INVALID_RSN_INFO_CAP;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_check_rsn_capability::VAP not supported PreauthActivated[%d].}", en_PreauthActivated);
        return OAL_ERR_CODE_SECURITY_AUTH_TYPE;
    }

    /*3.1 ??????????(80211w)????????*/
    en_dot11RSNAMFPR = (us_rsn_capability & BIT6) ? OAL_TRUE : OAL_FALSE;
    en_dot11RSNAMFPC = (us_rsn_capability & BIT7) ? OAL_TRUE : OAL_FALSE;
    /*3.1.1 ??????????????????MFP????*/
    if ((OAL_TRUE == pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPR) && (OAL_FALSE == en_dot11RSNAMFPC))
    {
        *pen_status_code = MAC_MFP_VIOLATION;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_check_rsn_capability::refuse with NON MFP[%d].}", en_PreauthActivated);
        return OAL_ERR_CODE_SECURITY_CAP_MFP;
    }
    /*3.1.2 ??????????????????MFP????*/
    if ((OAL_FALSE == pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAMFPC) && (OAL_TRUE == en_dot11RSNAMFPR))
    {
        *pen_status_code = MAC_MFP_VIOLATION;
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_check_rsn_capability::VAP not supported RSNA MFP[%d].}", en_PreauthActivated);
        return OAL_ERR_CODE_SECURITY_CAP_MFP;
    }

    return OAL_SUCC;
}


oal_uint8 * hmac_get_security_oui(oal_uint8 uc_80211i_mode)
{
    oal_uint8 * puc_oui;

    if (DMAC_WPA_802_11I == uc_80211i_mode)
    {
        puc_oui = g_auc_wpa_oui;
    }
    else if (DMAC_RSNA_802_11I == uc_80211i_mode)
    {
        puc_oui = g_auc_rsn_oui;
    }
    else
    {
        puc_oui = OAL_PTR_NULL;
    }

    return puc_oui;
}


oal_uint8 hmac_get_pcip_policy_auth(oal_uint8 *puc_frame, oal_uint8 *puc_len, oal_uint8 *puc_oui)
{
    oal_uint8 uc_indext         = 0;
    oal_uint8 uc_pcip_policy    = 0;

    /*************************************************************************/
    /*                  RSN Element Pairwise Ciper Format                    */
    /* --------------------------------------------------------------------- */
    /* | Pairwise Cipher Count | Pairwise Cipher Suite List |                */
    /* --------------------------------------------------------------------- */
    /* |         2             |           4*m              |                */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/

    /* ???????????????????????????????????? */
    *puc_len = (oal_uint8)(puc_frame[0] * 4)+ 2;

    /* ASSOC REQ ??????????????????????1?? */
    if ((puc_frame[0] > 1) || (0 == puc_frame[0]))
    {
        uc_pcip_policy = 0xFF;
    }
    else
    {
        uc_indext = 2; /* ??????????????2 ???? */

        uc_indext += MAC_OUI_LEN; /* ????OUI ???? */

        uc_pcip_policy = puc_frame[uc_indext];/* ???????????? */
    }

    return uc_pcip_policy;
}



oal_uint8 hmac_get_auth_policy_auth(oal_uint8 * puc_frame, oal_uint8 *uc_len, oal_uint8 *puc_oui)
{
    oal_uint8 uc_index        = 0;
    oal_uint8 uc_auth_policy  = 0;

    /*************************************************************************/
    /*                  RSN Element AKM Suite Format                         */
    /* --------------------------------------------------------------------- */
    /* |    AKM Cipher Count   |   AKM Cipher Suite List    |                */
    /* --------------------------------------------------------------------- */
    /* |         2             |           4*s              |                */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/

    /* ???????????????????????????????? */
    *uc_len = (oal_uint8)(puc_frame[0] * 4) + 2;

    if ((puc_frame[0] > 1) || (0 == puc_frame[0]))
    {
        uc_auth_policy = 0xFF;
    }
    else
    {
        uc_index = 2; /* ????AKM ????????2???? */

        uc_index += MAC_OUI_LEN; /* ????OUI ???? */

        uc_auth_policy = puc_frame[uc_index];/* ???????????? */
    }

    return uc_auth_policy;
}




oal_uint32 hmac_check_join_req_parewise_cipher_supplicant(
                                wlan_mib_ieee802dot11_stru  *pst_mib_info,
                                oal_uint8                   *puc_pcip_policy_match,
                                oal_uint8                    uc_802_11i_mode,
                                oal_uint8                   *puc_pcip_policy)
{
    oal_uint8                    uc_loop = 0;
    oal_bool_enum_uint8          en_ret  = OAL_FALSE;


    /* ?????????????????????????? */
    *puc_pcip_policy_match = 0xFF;

    /* ??STA mib ???????? AP ?????????????????????? */
    for (uc_loop = 0; uc_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; uc_loop++)
    {
        if (WLAN_80211_CIPHER_SUITE_GROUP_CIPHER == puc_pcip_policy[uc_loop])
        {
            /* ???????????????????????????? */
            *puc_pcip_policy_match = WLAN_80211_CIPHER_SUITE_GROUP_CIPHER;
            break;
        }
        else if (0xFF == puc_pcip_policy[uc_loop])
        {
            /* ?????????????????????????????????? */
            continue;
        }

        /* ???????????????? */
        /* ???????????????? */
        if (uc_802_11i_mode == DMAC_WPA_802_11I)
        {
            en_ret = hmac_check_pcip_wpa_policy(pst_mib_info, puc_pcip_policy[uc_loop]);
        }
        else
        {
            en_ret = hmac_check_pcip_wpa2_policy(pst_mib_info, puc_pcip_policy[uc_loop]);
        }
        if (OAL_TRUE == en_ret)
        {
            *puc_pcip_policy_match = puc_pcip_policy[uc_loop];
            if (WLAN_80211_CIPHER_SUITE_CCMP == puc_pcip_policy[uc_loop])
            {
                break;
            }
        }
    }

    /* ???????????????????????? */
    if (0xFF == *puc_pcip_policy_match)
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_check_join_req_parewise_cipher_supplicant::pariwise not match.}");
        for (uc_loop = 0; uc_loop < MAC_PAIRWISE_CIPHER_SUITES_NUM; uc_loop++)
        {
            OAM_ERROR_LOG2(0, OAM_SF_WPA, "{hmac_check_join_req_parewise_cipher_supplicant::user pairwise[%d]=%d.}",
                           uc_loop, puc_pcip_policy[uc_loop]);
        }

        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    return OAL_SUCC;
}

#if defined(_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2)

OAL_STATIC oal_uint32 hmac_check_join_req_auth_suite_supplicant(
                                wlan_mib_ieee802dot11_stru  *pst_mib_info,
                                oal_uint8                   *puc_auth_policy_match,
                                oal_uint8                   *puc_auth_policy)
{
    oal_uint8 uc_loop = 0;

    /* ?????????????????????? */
    *puc_auth_policy_match = 0xFF;

    /* ????STA ?? AP ?????????????????? */
    for (uc_loop = 0; uc_loop < MAC_AUTHENTICATION_SUITE_NUM; uc_loop++)
    {
        /* ?????????????????????????????????? */
        if (0xFF == puc_auth_policy[uc_loop])
        {
            continue;
        }

        /* ?????????????????????????????????????????? */
        if (OAL_TRUE == mac_check_auth_policy(pst_mib_info, puc_auth_policy[uc_loop]))
        {
            *puc_auth_policy_match = puc_auth_policy[uc_loop];
        }
    }

    if (0xFF == *puc_auth_policy_match)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_check_join_req_security_cap_supplicant::user auth=%d.}", puc_auth_policy[0]);
        return OAL_ERR_CODE_SECURITY_AUTH_TYPE;
    }

    return OAL_SUCC;
}



OAL_STATIC oal_uint32 hmac_check_join_req_security_cap_supplicant(
                                mac_bss_dscr_stru           *pst_bss_dscr,
                                wlan_mib_ieee802dot11_stru  *pst_mib_info,
                                oal_uint8                   *puc_grp_policy_match,
                                oal_uint8                   *puc_pcip_policy_match,
                                oal_uint8                   *puc_auth_policy_match,
                                oal_uint8                    uc_802_11i_mode)
{
    oal_uint8 *puc_pcip_policy = OAL_PTR_NULL;
    oal_uint8 *puc_auth_policy = OAL_PTR_NULL;
    oal_uint8  uc_grp_policy   = 0;

    if ((pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode & uc_802_11i_mode)
        != uc_802_11i_mode )
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_check_join_req_security_cap_supplicant::80211i modeh=%d.}", uc_802_11i_mode);
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    if (DMAC_RSNA_802_11I == uc_802_11i_mode)
    {
        puc_pcip_policy = pst_bss_dscr->st_bss_sec_info.auc_rsn_pairwise_policy;
        puc_auth_policy = pst_bss_dscr->st_bss_sec_info.auc_rsn_auth_policy;
        uc_grp_policy   = pst_bss_dscr->st_bss_sec_info.uc_rsn_grp_policy;
    }
    else
    {
        puc_pcip_policy = pst_bss_dscr->st_bss_sec_info.auc_wpa_pairwise_policy;
        puc_auth_policy = pst_bss_dscr->st_bss_sec_info.auc_wpa_auth_policy;
        uc_grp_policy   = pst_bss_dscr->st_bss_sec_info.uc_wpa_grp_policy;
    }

    /* ???????? */
    *puc_grp_policy_match = uc_grp_policy;

    /* ???????????????? */
    if(OAL_SUCC != hmac_check_join_req_parewise_cipher_supplicant(pst_mib_info, puc_pcip_policy_match, uc_802_11i_mode, puc_pcip_policy))
    {
        return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
    }

    /* ???????????????? */
    if(OAL_SUCC != hmac_check_join_req_auth_suite_supplicant(pst_mib_info, puc_auth_policy_match, puc_auth_policy))
    {
        return OAL_ERR_CODE_SECURITY_AUTH_TYPE;
    }

    /* ????RSN, ????RSN capability */
    if (DMAC_RSNA_802_11I == uc_802_11i_mode)
    {
        /* RSN Capabilities Information                                         */
        /************************************************************************/
        /* +-------------+---------------+-------------+------------+---------+ */
        /* | B15 - B6    |  B5 - B4      | B3 - B2     |      B1    |    B0   | */
        /* +-------------+---------------+-------------+------------+---------+ */
        /* | other filed |  GTSKA Replay | PTSKA Replay| NoPairwise | Pre-Auth| */
        /* |             |    Counter    |   Counter   |            |         | */
        /* +-------------+---------------+-------------+------------+---------+ */
        /************************************************************************/

    }

    OAM_INFO_LOG3(0, OAM_SF_WPA, "{hmac_check_join_req_security_cap_supplicant::group=%d, pairwise=%d, auth=%d.}",
                   *puc_grp_policy_match, *puc_pcip_policy_match, *puc_auth_policy_match);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 hmac_check_security_capability_supplicant(mac_vap_stru *pst_mac_vap,
                                                                mac_bss_dscr_stru *pst_bss_dscr)
{
    wlan_mib_ieee802dot11_stru  *pst_mib_info;
    hmac_vap_stru               *pst_hmac_vap;
    oal_uint8                    uc_grp_policy_match  = 0xFF;
    oal_uint8                    uc_pcip_policy_match = 0xFF;
    oal_uint8                    uc_auth_policy_match = 0xFF;
    oal_uint8                    uc_80211i_mode       = 0x00;
    oal_uint32                   ul_ret = OAL_FAIL;

    pst_mib_info = pst_mac_vap->pst_mib_info;    /* ?????? MIB ?? */

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_check_security_capability_supplicant::bss_11i_mode[%d], vap rsn_active[%d], wpa[%d], wpa2[%d].}",
                    pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode,
                    pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated,
                    pst_mac_vap->st_cap_flag.bit_wpa,
                    pst_mac_vap->st_cap_flag.bit_wpa2);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_check_security_capability_supplicant::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_TRUE == pst_hmac_vap->en_wps_active)
    {
        OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_check_security_capability_supplicant::WPS enable.}");
        return OAL_SUCC;
    }

    if (OAL_TRUE == pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated)
    {
        /* ????STA ?? AP ????????????????, ??????????????match ?? */
        /* ??WPA/WPA2 ????????????????????WPA2 */
        if ((pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode & DMAC_RSNA_802_11I)
            && (OAL_TRUE == pst_mac_vap->st_cap_flag.bit_wpa2))
        {
            ul_ret = hmac_check_join_req_security_cap_supplicant(pst_bss_dscr,
                                    pst_mib_info,
                                    &uc_grp_policy_match,
                                    &uc_pcip_policy_match,
                                    &uc_auth_policy_match,
                                    DMAC_RSNA_802_11I);
            if (OAL_SUCC == ul_ret)
            {
                uc_80211i_mode = DMAC_RSNA_802_11I;
            }
        }

        if ((OAL_FAIL == ul_ret)
            && (pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode & DMAC_WPA_802_11I)
            && (OAL_TRUE == pst_mac_vap->st_cap_flag.bit_wpa))
        {
            ul_ret = hmac_check_join_req_security_cap_supplicant(pst_bss_dscr,
                                    pst_mib_info,
                                    &uc_grp_policy_match,
                                    &uc_pcip_policy_match,
                                    &uc_auth_policy_match,
                                    DMAC_WPA_802_11I);
            if (OAL_SUCC == ul_ret)
            {
                uc_80211i_mode = DMAC_WPA_802_11I;
            }
        }
        if (OAL_SUCC != ul_ret)
        {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_check_security_capability_supplicant::WPA & WPA2 not match[%d].}", ul_ret);
            return OAL_ERR_CODE_SECURITY_CHIPER_TYPE;
        }

        pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode       = uc_80211i_mode;
        pst_bss_dscr->st_bss_sec_info.uc_grp_policy_match      = uc_grp_policy_match;
        pst_bss_dscr->st_bss_sec_info.uc_pairwise_policy_match = uc_pcip_policy_match;
        pst_bss_dscr->st_bss_sec_info.uc_auth_policy_match     = uc_auth_policy_match;
        mac_mib_set_RSNAAuthenticationSuiteSelected(pst_mac_vap, uc_auth_policy_match);
    }
    /* ??vap??????rsn,????????rsn, ???????? */
    else
    {
        if (pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode & (DMAC_RSNA_802_11I | DMAC_WPA_802_11I))
        {
            OAM_ERROR_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "hmac_check_security_capability_supplicant::WPA or WPA2 not support.uc_bss_80211i_mode: %d, bss mac addr: XX:XX:XX:%02X:%02X:%02X",
                                                                pst_bss_dscr->st_bss_sec_info.uc_bss_80211i_mode,
                                                                pst_bss_dscr->auc_mac_addr[3],
                                                                pst_bss_dscr->auc_mac_addr[4],
                                                                pst_bss_dscr->auc_mac_addr[5]);

            /* ????Beacon/Probe rsp?????? */
            /*lint -e416*/
            oam_report_80211_frame(BROADCAST_MACADDR,
                                   pst_bss_dscr->auc_mgmt_buff,
                                   MAC_80211_FRAME_LEN,
                                   pst_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN,
                                   (oal_uint16)pst_bss_dscr->ul_mgmt_len,
                                   OAM_OTA_FRAME_DIRECTION_TYPE_RX);
            /*lint +e416*/

            return OAL_SUCC;
        }
    }

    return OAL_SUCC;
}
#endif /* (_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2) */


oal_uint32 hmac_check_capability_mac_phy_supplicant(mac_vap_stru      *pst_mac_vap,
                                                               mac_bss_dscr_stru  *pst_bss_dscr)
{
    oal_uint32 ul_ret = OAL_FAIL;

    if ((OAL_PTR_NULL == pst_mac_vap) || (OAL_PTR_NULL == pst_bss_dscr))
    {
        OAM_WARNING_LOG2(0, OAM_SF_WPA, "{hmac_check_capability_mac_phy_supplicant::input null %x %x.}", pst_mac_vap, pst_bss_dscr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??????????????????????STA HT/VHT mib?? */
    mac_vap_config_vht_ht_mib_by_protocol(pst_mac_vap);

    ul_ret = hmac_check_bss_cap_info(pst_bss_dscr->us_cap_info, pst_mac_vap);
    if (ul_ret != OAL_TRUE)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_check_capability_mac_phy_supplicant::hmac_check_bss_cap_info failed[%d].}", ul_ret);
    }

    /* check bss capability info PHY,????PHY????????????AP */
    mac_vap_check_bss_cap_info_phy_ap(pst_bss_dscr->us_cap_info, pst_mac_vap);

#if defined(_PRE_WLAN_FEATURE_WPA) || defined(_PRE_WLAN_FEATURE_WPA2)
    ul_ret = hmac_check_security_capability_supplicant(pst_mac_vap, pst_bss_dscr);
    if (OAL_SUCC != ul_ret)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_check_capability_mac_phy_supplicant::hmac_check_security_capability_supplicant failed[%d].}", ul_ret);
    }
#endif

    return OAL_SUCC;
}


oal_void hmac_update_pcip_policy_prot_supplicant(mac_vap_stru *pst_mac_vap,
                                                            oal_uint8 uc_pcip_policy_match)
{
    hmac_vap_stru                  *pst_hmac_vap;
    mac_cfg_mode_param_stru         st_cfg_mode;
    oal_uint8                       uc_protocol_fall_flag = OAL_FALSE;

    st_cfg_mode.en_protocol = pst_mac_vap->en_protocol;

    if ((WLAN_80211_CIPHER_SUITE_TKIP       == uc_pcip_policy_match)
        || (WLAN_80211_CIPHER_SUITE_WEP_104 == uc_pcip_policy_match)
        || (WLAN_80211_CIPHER_SUITE_WEP_40  == uc_pcip_policy_match)
        || (WLAN_80211_CIPHER_SUITE_WAPI == uc_pcip_policy_match) )
    {
        if ((pst_mac_vap->en_protocol >= WLAN_HT_MODE) && (pst_mac_vap->en_protocol < WLAN_PROTOCOL_BUTT))
        {
            if (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band)
            {
                st_cfg_mode.en_protocol = WLAN_MIXED_ONE_11G_MODE;
                pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
                uc_protocol_fall_flag = OAL_TRUE;
            }
            if (WLAN_BAND_5G == pst_mac_vap->st_channel.en_band)
            {
                st_cfg_mode.en_protocol = WLAN_LEGACY_11A_MODE;
                pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
                uc_protocol_fall_flag = OAL_TRUE;

            }
        }
    }
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_update_pcip_policy_prot_supplicant::pst_hmac_vap null.}");
        return;
    }
    pst_hmac_vap->bit_protocol_fall = uc_protocol_fall_flag;

    if (st_cfg_mode.en_protocol >= WLAN_HT_MODE)
    {
        pst_hmac_vap->en_tx_aggr_on   = OAL_TRUE;
        pst_hmac_vap->en_amsdu_active = OAL_TRUE;
    }
    else
    {
        pst_hmac_vap->en_tx_aggr_on   = OAL_FALSE;
        pst_hmac_vap->en_amsdu_active = OAL_FALSE;
    }

#ifdef _PRE_WIFI_DMT
    hmac_config_sta_update_rates(pst_mac_vap, &st_cfg_mode, OAL_PTR_NULL);
#endif

    mac_vap_init_by_protocol(pst_mac_vap, st_cfg_mode.en_protocol);
    //mac_vap_init_rates(pst_mac_vap);

    OAM_INFO_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_update_pcip_policy_prot_supplicant::en_protocol=%d, bandwidth=%d.}", pst_mac_vap->en_protocol, pst_mac_vap->st_channel.en_bandwidth);
}


oal_uint32  hmac_update_current_join_req_parms_11i(
                    mac_vap_stru           *pst_mac_vap,
                    mac_bss_80211i_info_stru     *pst_11i)
{
    hmac_vap_stru                  *pst_hmac_vap;
    oal_uint8  uc_grp_policy_match  = 0;    /* STA ?? AP ?????????????????????? */
    oal_uint8  uc_pcip_policy_match = 0;    /* STA ?? AP ?????????????????????? */
    oal_uint8  uc_auth_policy_match = 0;    /* STA ?? AP ?????????????????? */
    oal_uint8  uc_80211i_mode       = 0;    /* STA ?????????????????? */
    oal_uint16 ul_ciphersize        = 0;

    /* ????????????????jion ????STA ??mib */
    uc_grp_policy_match  = pst_11i->uc_grp_policy_match;
    uc_pcip_policy_match = pst_11i->uc_pairwise_policy_match;
    uc_auth_policy_match = pst_11i->uc_auth_policy_match;
    uc_80211i_mode       = pst_11i->uc_bss_80211i_mode;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_update_current_join_req_parms_11i::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_TRUE != pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11PrivacyInvoked)
    {
        if(OAL_TRUE == pst_hmac_vap->bit_protocol_fall)
        {
            hmac_update_pcip_policy_prot_supplicant(pst_mac_vap, WLAN_80211_CIPHER_SUITE_NO_ENCRYP);
        }
        return OAL_SUCC;
    }

    /* ????RSN */
    if (OAL_TRUE != pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated)
    {
        /* ??WEP / TKIP ??????????????????????HT MODE */
        hmac_update_pcip_policy_prot_supplicant(pst_mac_vap, WLAN_80211_CIPHER_SUITE_WEP_40);
        return OAL_SUCC;
    }

    /* ??WEP / TKIP ??????????????????????HT MODE */
    hmac_update_pcip_policy_prot_supplicant(pst_mac_vap, uc_pcip_policy_match);

    /* ????STA ??MIB ???? */
    /* ????????mib ??????????????????AP */
    if (pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGroupCipher
        != uc_grp_policy_match)
    {
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAConfigGroupCipher
            = uc_grp_policy_match;
        if (WLAN_80211_CIPHER_SUITE_CCMP == uc_grp_policy_match)
        {
            ul_ciphersize = WLAN_CCMP_KEY_LEN * 8;    /* CCMP ??????????????????(BITS) */
        }
        else if (WLAN_80211_CIPHER_SUITE_TKIP == uc_grp_policy_match)
        {
            ul_ciphersize = WLAN_TKIP_KEY_LEN * 8;    /* TKIP ????????????????????(BITS) */
        }
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.ul_dot11RSNAConfigGroupCipherSize
            = ul_ciphersize;
        pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAGroupCipherSelected
            = uc_grp_policy_match;
    }

    /* ????MIB ?? */
    pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAAuthenticationSuiteRequested
            = uc_auth_policy_match;
    pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAPairwiseCipherRequested
            = uc_pcip_policy_match;
    pst_mac_vap->pst_mib_info->st_wlan_mib_rsna_cfg.uc_dot11RSNAGroupCipherRequested
            = uc_grp_policy_match;

    /* STA ???????? 80211i_mode ??????WPA ???? WPA2 */
    pst_hmac_vap->uc_80211i_mode = uc_80211i_mode;

    OAM_INFO_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_update_current_join_req_parms_11i::mode=%d group=%d pairwise=%d auth=%d.}",
                  uc_80211i_mode, uc_grp_policy_match, uc_pcip_policy_match, uc_auth_policy_match);
    return OAL_SUCC;
}
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

oal_uint32 hmac_en_mic(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf, oal_uint8 *puc_iv_len)
{
    wlan_priv_key_param_stru             *pst_key        = OAL_PTR_NULL;
    oal_uint32                            ul_ret         = OAL_SUCC;
    wlan_ciper_protocol_type_enum_uint8   en_cipher_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    wlan_cipher_key_type_enum_uint8       en_key_type    = 0;
    mac_tx_ctl_stru                      *pst_tx_ctl;

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_hmac_vap) ||
        (OAL_PTR_NULL == pst_hmac_user) ||
        (OAL_PTR_NULL == pst_netbuf) ||
        (OAL_PTR_NULL == puc_iv_len))
    {
        OAM_ERROR_LOG4(0, OAM_SF_WPA, "{hmac_en_mic::pst_hmac_vap=%d pst_hmac_user=%d pst_netbuf=%d puc_iv_len=%d.}",
                       pst_hmac_vap, pst_hmac_user, pst_netbuf, puc_iv_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *puc_iv_len    = 0;
    en_key_type    = pst_hmac_user->st_user_base_info.st_user_tx_info.st_security.en_cipher_key_type;
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_key = mac_user_get_key(&pst_hmac_user->st_user_base_info, en_key_type - 1);
    if (OAL_PTR_NULL == pst_key)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_en_mic::mac_user_get_key FAIL. en_key_type[%d]}", en_key_type);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    switch (en_cipher_type)
    {
        case WLAN_80211_CIPHER_SUITE_TKIP:
            if (en_key_type  == 0 || en_key_type > 5)
            {
                return OAL_ERR_CODE_SECURITY_KEY_TYPE;
            }

            pst_tx_ctl = (mac_tx_ctl_stru *)OAL_NETBUF_CB(pst_netbuf);
            /* TIKPmic????data????????????????????????  */
            if (pst_tx_ctl->bit_80211_mac_head_type == 1)
            {
                oal_netbuf_pull(pst_netbuf, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
            }
            ul_ret = hmac_crypto_tkip_enmic(pst_key, pst_netbuf);
            if (OAL_SUCC != ul_ret)
            {
                OAM_ERROR_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WPA,
                              "{hmac_en_mic::hmac_crypto_tkip_enmic failed[%d].}", ul_ret);
                return ul_ret;
            }
            /* data????????????????????dmac offload??????????&??????????????  */
            if (pst_tx_ctl->bit_80211_mac_head_type == 1)
            {
                oal_netbuf_push(pst_netbuf, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN);
            }

            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        case WLAN_80211_CIPHER_SUITE_CCMP:
            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        default:
            break;
    }

    return OAL_SUCC;
}

#else


oal_uint32 hmac_en_mic(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf, oal_uint8 *puc_iv_len)
{
    wlan_priv_key_param_stru             *pst_key        = OAL_PTR_NULL;
    oal_uint32                            ul_ret         = OAL_SUCC;
    wlan_ciper_protocol_type_enum_uint8   en_cipher_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    wlan_cipher_key_type_enum_uint8       en_key_type    = 0;

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_hmac_vap) ||
        (OAL_PTR_NULL == pst_hmac_user) ||
        (OAL_PTR_NULL == pst_netbuf) ||
        (OAL_PTR_NULL == puc_iv_len))
    {
        OAM_ERROR_LOG4(0, OAM_SF_WPA, "{hmac_en_mic::pst_hmac_vap=%d pst_hmac_user=%d pst_netbuf=%d puc_iv_len=%d.}",
                       pst_hmac_vap, pst_hmac_user, pst_netbuf, puc_iv_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *puc_iv_len    = 0;
    en_key_type    = pst_hmac_user->st_user_base_info.st_user_tx_info.st_security.en_cipher_key_type;
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_key = mac_user_get_key(&pst_hmac_user->st_user_base_info, en_key_type - 1);
    if (OAL_PTR_NULL == pst_key)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_en_mic::mac_user_get_key FAIL. en_key_type[%d]}", en_key_type);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    switch (en_cipher_type)
    {
        case WLAN_80211_CIPHER_SUITE_TKIP:
            if (en_key_type  == 0 || en_key_type > 5)
            {
                return OAL_ERR_CODE_SECURITY_KEY_TYPE;
            }
            ul_ret = hmac_crypto_tkip_enmic(pst_key, pst_netbuf);
            if (OAL_SUCC != ul_ret)
            {
                OAM_ERROR_LOG1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WPA,
                              "{hmac_en_mic::hmac_crypto_tkip_enmic failed[%d].}", ul_ret);
                return ul_ret;
            }
            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        case WLAN_80211_CIPHER_SUITE_CCMP:
            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        default:
            break;
    }

    return OAL_SUCC;
}
#endif


oal_uint32 hmac_de_mic(hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf)
{
    wlan_priv_key_param_stru             *pst_key        = OAL_PTR_NULL;
    oal_uint32                            ul_ret         = OAL_SUCC;
    wlan_ciper_protocol_type_enum_uint8   en_cipher_type = WLAN_80211_CIPHER_SUITE_NO_ENCRYP;
    wlan_cipher_key_type_enum_uint8       en_key_type    = 0;

    /*1.1 ????????*/
    if ((OAL_PTR_NULL == pst_hmac_user) ||
        (OAL_PTR_NULL == pst_netbuf))
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_de_mic::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_key_type    = pst_hmac_user->st_user_base_info.st_user_tx_info.st_security.en_cipher_key_type;
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_key = mac_user_get_key(&pst_hmac_user->st_user_base_info, en_key_type - 1);
    if (OAL_PTR_NULL == pst_key)
    {
        OAM_ERROR_LOG1(0, OAM_SF_WPA, "{hmac_de_mic::mac_user_get_key FAIL. en_key_type[%d]}", en_key_type);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    switch (en_cipher_type)
    {
        case WLAN_80211_CIPHER_SUITE_TKIP:
            if (en_key_type  == 0 || en_key_type > 5)
            {
                return OAL_ERR_CODE_SECURITY_KEY_TYPE;
            }
            ul_ret = hmac_crypto_tkip_demic(pst_key, pst_netbuf);
            if (OAL_SUCC != ul_ret)
            {
                OAM_ERROR_LOG1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_WPA,
                               "{hmac_de_mic::hmac_crypto_tkip_demic failed[%d].}", ul_ret);
                return ul_ret;
            }
            break;
        default:
            break;
    }

    return OAL_SUCC;
}



oal_uint32 hmac_rx_tkip_mic_failure_process(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru                     *pst_event;
    frw_event_mem_stru                 *pst_hmac_event_mem;
    frw_event_hdr_stru                 *pst_event_hdr;
    dmac_to_hmac_mic_event_stru        *pst_mic_event;

    if (OAL_PTR_NULL == pst_event_mem)
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_rx_tkip_mic_failure_process::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????????????????? */
    pst_event           = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event_hdr       = &(pst_event->st_event_hdr);
    pst_mic_event       = (dmac_to_hmac_mic_event_stru *)&(pst_event->auc_event_data);

    /* ??mic????????WAL */
    pst_hmac_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_to_hmac_mic_event_stru));
    if (OAL_PTR_NULL == pst_hmac_event_mem)
    {
        OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_WPA, "{hmac_rx_tkip_mic_failure_process::pst_hmac_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ???????? */
    pst_event = (frw_event_stru *)pst_hmac_event_mem->puc_data;

    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_MIC_FAILURE,
                       OAL_SIZEOF(dmac_to_hmac_mic_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_event_hdr->uc_chip_id,
                       pst_event_hdr->uc_device_id,
                       pst_event_hdr->uc_vap_id);

    /* ????????STA mac???? */
    oal_memcopy((oal_uint8 *)frw_get_event_payload(pst_event_mem),(oal_uint8 *)pst_mic_event, sizeof(dmac_to_hmac_mic_event_stru));

    /* ???????? */
    frw_event_dispatch_event(pst_hmac_event_mem);
    FRW_EVENT_FREE(pst_hmac_event_mem);
    return OAL_SUCC;
}


oal_uint32 hmac_11i_ether_type_filter(hmac_vap_stru *pst_vap, oal_uint8 *puc_mac_addr, oal_uint16 us_ether_type)
{
    mac_user_stru               *pst_mac_user;
    mac_vap_stru                *pst_mac_vap = OAL_PTR_NULL;
    oal_uint32                   ul_ret = OAL_SUCC;


    if (OAL_PTR_NULL == puc_mac_addr)
    {
        OAM_ERROR_LOG0(0, OAM_SF_WPA, "{hmac_11i_ether_type_filter::puc_mac_addr null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_vap->st_vap_base_info);

    if (OAL_TRUE == pst_mac_vap->pst_mib_info->st_wlan_mib_privacy.en_dot11RSNAActivated)/* ????????????WPA/WPA2 */
    {
        pst_mac_user = mac_vap_get_user_by_addr(&(pst_vap->st_vap_base_info), puc_mac_addr);
        if (OAL_PTR_NULL == pst_mac_user)
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_11i_ether_type_filter:: user filterd.}");
            return OAL_ERR_CODE_SECURITY_USER_INVAILD;
        }

        if (pst_mac_user->en_port_valid != OAL_TRUE)/* ???????????????? */
        {
            /* ??????????????????EAPOL ?????????????? */
            if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_PAE) != us_ether_type)
            {
                OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                               "{hmac_11i_ether_type_filter::TYPE 0x%04x not permission.}", us_ether_type);
                ul_ret = OAL_ERR_CODE_SECURITY_PORT_INVALID;
            }
        }
        /* EAPOL???????????? */
        else if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_PAE) == us_ether_type)
        {
            OAM_INFO_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                               "{hmac_11i_ether_type_filter::rx EAPOL.}");
        }
    }
    return ul_ret;
}



/*lint -e578*//*lint -e19*/
oal_module_symbol(hmac_config_11i_set_default_key);
oal_module_symbol(hmac_config_11i_remove_key);
oal_module_symbol(hmac_config_11i_get_key);
oal_module_symbol(hmac_config_11i_add_key);
oal_module_symbol(hmac_config_11i_add_wep_entry);
/*oal_module_symbol(hmac_config_rssi_switch);*/
/*lint +e578*//*lint +e19*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

