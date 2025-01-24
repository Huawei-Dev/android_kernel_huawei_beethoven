


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "hmac_dfx.h"
#include "hmac_ext_if.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DFX_C

/*****************************************************************************
  2 ????????????
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_DFR
hmac_dfr_info_stru  g_st_dfr_info;         /* DFR???????????? */
#endif

OAL_STATIC oam_cfg_data_stru  g_ast_cfg_data[OAM_CFG_TYPE_BUTT] =
{
    {OAM_CFG_TYPE_MAX_ASOC_USER,     "USER_SPEC",     "max_asoc_user",     31},
};

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
oal_uint16   g_us_wlan_assoc_user_max_num     = 32;     /* ?????????????????? Root AP????????32??,Repeater????????15?? */

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
/* ????WL_L2_DRAM??????????????????????2??????vap???????????????????????? TBD */
oal_uint32   g_ul_wlan_vap_max_num_per_device = 4 + 1;  /* 4??AP + 1??????vap */
#else
oal_uint32   g_ul_wlan_vap_max_num_per_device = 4 + 1;  /* 4??AP + 1??????vap */
#endif

#else
extern oal_uint16   g_us_wlan_assoc_user_max_num;
extern oal_uint32   g_ul_wlan_vap_max_num_per_device;
#endif

/*****************************************************************************
  3 ????????  TBD ????????????
*****************************************************************************/

oal_int32  oam_cfg_get_item_by_id(oam_cfg_type_enum_uint16  en_cfg_type)
{
    oal_uint32      ul_loop;

    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++)
    {
        if (en_cfg_type == g_ast_cfg_data[ul_loop].en_cfg_type)
        {
            break;
        }
    }

    if (OAM_CFG_TYPE_BUTT == ul_loop)
    {
        OAL_IO_PRINT("oam_cfg_get_item_by_id::get cfg item failed!\n");
        return -OAL_FAIL;
    }

    return g_ast_cfg_data[ul_loop].l_val;
}

#if ((defined(_PRE_PRODUCT_ID_HI110X_DEV)) || (defined(_PRE_PRODUCT_ID_HI110X_HOST)))

oal_uint32  hmac_custom_init(oal_uint32 ul_psta_enable)
{
    /* ????????:3??STA; 2??AP */
    /* ????????:
        1)AP ????:  2??ap + 1??????vap
        2)STA ????: 3??sta + 1??????vap
        3)STA+P2P????????:  1??sta + 1??P2P_dev + 1??P2P_GO/Client + 1??????vap
        4)STA+Proxy STA????????:  1??sta + ???proxy STA + 1??????vap
    */
    g_us_wlan_assoc_user_max_num     = 8;
    // TBD ??????WLAN_ASSOC_USER_MAX_NUM_LIMIT;
    g_ul_wlan_vap_max_num_per_device = WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE + WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE;

    return OAL_SUCC;
}

#else


oal_uint32  hmac_custom_init(oal_uint32 ul_psta_enable)
{

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    if (ul_psta_enable)
    {
        g_us_wlan_assoc_user_max_num  = 15;
        g_ul_wlan_vap_max_num_per_device = WLAN_REPEATER_SERVICE_VAP_MAX_NUM_PER_DEVICE + WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE;/* 1??AP, 1??sta??15??Proxy STA??1??????vap */
    }
    else
#endif
    {
        g_us_wlan_assoc_user_max_num  = WLAN_ASSOC_USER_MAX_NUM_LIMIT;
        g_ul_wlan_vap_max_num_per_device = WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE + WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE;
    }

    return OAL_SUCC;
}
#endif




OAL_STATIC oal_uint32  oam_cfg_restore_all_item(oal_int32 al_default_cfg_data[])
{
    oal_uint32          ul_loop;

    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++)
    {
        g_ast_cfg_data[ul_loop].l_val = al_default_cfg_data[ul_loop];
    }

    return OAL_SUCC;
}



oal_int32  oam_cfg_get_all_item(oal_void)
{
    oal_int32     al_default_cfg_data[OAM_CFG_TYPE_BUTT] = {0};
    oal_uint8    *puc_plaintext;
    oal_uint8    *puc_ciphertext;
    oal_uint32    ul_file_size = 0;
    oal_uint32    ul_loop;
    oal_int32     l_ret;
    oal_int       i_key[OAL_AES_KEYSIZE_256] = {0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12,
                                       		    0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12,
                                       		    0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12,
                                       		    0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12};

	oal_aes_key_stru    st_aes_key;

    /* ??????????????????????????????????????????????????????????????????????
       ????????????????????????????????????????????????????????
    */
    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++)
    {
        al_default_cfg_data[ul_loop] = g_ast_cfg_data[ul_loop].l_val;
    }

    /* ?????????????????????????? */
    l_ret = oal_file_size(&ul_file_size);
    if (OAL_SUCC != l_ret)
    {
        OAL_IO_PRINT("oam_cfg_get_all_item::get file size failed!\n");
        return l_ret;
    }

    /* ???????????????????????????????????????????????????????? */
    puc_ciphertext = oal_memalloc(ul_file_size + OAM_CFG_STR_END_SIGN_LEN);
    if (OAL_PTR_NULL == puc_ciphertext)
    {
        OAL_IO_PRINT("oam_cfg_get_all_item::alloc ciphertext buf failed! load ko with default cfg!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    OAL_MEMZERO(puc_ciphertext, ul_file_size + OAM_CFG_STR_END_SIGN_LEN);

    l_ret = oam_cfg_read_file_to_buf((oal_int8 *)puc_ciphertext, ul_file_size);
    if (OAL_SUCC != l_ret)
    {
        OAL_IO_PRINT("oam_cfg_get_all_item::get cfg data from file failed! fail id-->%d\n", l_ret);
        oal_free(puc_ciphertext);
        return l_ret;
    }

    /* ?????????????????????????? */
    puc_plaintext = oal_memalloc(ul_file_size + OAM_CFG_STR_END_SIGN_LEN);
    if (OAL_PTR_NULL == puc_plaintext)
    {
        OAL_IO_PRINT("oam_cfg_get_all_item::alloc pc_plaintext buf failed! load ko with default cfg!\n");
        oal_free(puc_ciphertext);

        return OAL_ERR_CODE_PTR_NULL;
    }
    OAL_MEMZERO(puc_plaintext, ul_file_size + OAM_CFG_STR_END_SIGN_LEN);

    /* ???? */
    l_ret = (oal_int32)oal_aes_expand_key(&st_aes_key,(oal_uint8 *)i_key,OAL_AES_KEYSIZE_256);
    if (OAL_SUCC != l_ret)
    {
        oal_free(puc_plaintext);
        oal_free(puc_ciphertext);

        return l_ret;
    }

    oam_cfg_decrypt_all_item(&st_aes_key, (oal_int8 *)puc_ciphertext,
                            (oal_int8 *)puc_plaintext, ul_file_size);

    /* ??????????????????????????????????OAM?????????? */
    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++)
    {
        l_ret = oam_cfg_get_one_item((oal_int8 *)puc_plaintext,
                                     g_ast_cfg_data[ul_loop].pc_section,
                                     g_ast_cfg_data[ul_loop].pc_key,
                                     &g_ast_cfg_data[ul_loop].l_val);

        /* ?????????????????????????????????????????????? */
        if (OAL_SUCC != l_ret)
        {
            OAL_IO_PRINT("oam_cfg_get_all_item::get cfg item fail! ul_loop=%d\n", ul_loop);

            oam_cfg_restore_all_item(al_default_cfg_data);
            oal_free(puc_plaintext);
            oal_free(puc_ciphertext);

            return l_ret;
        }
    }

    /* ?????????? */
    oal_free(puc_plaintext);
    oal_free(puc_ciphertext);

    return OAL_SUCC;
}



oal_uint32  oam_cfg_init(oal_void)
{
    oal_int32      l_ret = OAL_SUCC;

    l_ret = oam_cfg_get_all_item();

    return (oal_uint32)l_ret;
}

oal_uint32 hmac_dfx_init(void)
{
    oam_register_init_hook(OM_WIFI, oam_cfg_init);
    return hmac_custom_init(OAL_FALSE);
}

oal_uint32 hmac_dfx_exit(void)
{
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_DFR
oal_module_symbol(g_st_dfr_info);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
