


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oam_config.h"
#include "oal_aes.h"

/*****************************************************************************
  2 ????????????
*****************************************************************************/
oam_customize_stru g_oam_customize;


/*****************************************************************************
  3 ????????
*****************************************************************************/
oal_void  oam_register_init_hook(oam_msg_moduleid_enum_uint8 en_moduleid,  p_oam_customize_init_func p_func)
{
    g_oam_customize.customize_init[en_moduleid] = p_func;
}


oal_int32  oam_cfg_get_one_item(
                                           oal_int8   *pc_cfg_data_buf,
                                           oal_int8   *pc_section,
                                           oal_int8   *pc_key,
                                          oal_int32   *pl_val)
{
    /*****************************????????????????*****************************/
    /*                              [section]                                 */
    /*                              key=val                                   */
    /**************************************************************************/

    oal_int8        *pc_section_addr;
    oal_int8        *pc_key_addr;
    oal_int8        *pc_val_addr;
    oal_int8        *pc_equal_sign_addr;    /* ?????????? */
    oal_int8        *pc_tmp;
    oal_uint8        uc_key_len;
    oal_int8         ac_val[OAM_CFG_VAL_MAX_LEN] = {0};  /* ?????????????? */
    oal_uint8        uc_index = 0;

    /* ????section?????????????????? */
    pc_section_addr = oal_strstr(pc_cfg_data_buf, pc_section);
    if (OAL_PTR_NULL == pc_section_addr)
    {
        OAL_IO_PRINT("oam_cfg_get_one_item::section not found!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ??section?????????????????????????????????? */
    pc_key_addr = oal_strstr(pc_section_addr, pc_key);
    if (OAL_PTR_NULL == pc_key_addr)
    {
        OAL_IO_PRINT("oam_cfg_get_one_item::key not found!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ?????????????? */
    uc_key_len  = (oal_uint8)OAL_STRLEN(pc_key);

    /* ????key??????????????'=',????????????????????????????key??????????????
       key????????????????????????
    */
    pc_equal_sign_addr = pc_key_addr + uc_key_len;
    while (*(pc_equal_sign_addr) != '=')
    {
        pc_key_addr = oal_strstr(pc_equal_sign_addr, pc_key);
        if (OAL_PTR_NULL == pc_key_addr)
        {
            OAL_IO_PRINT("oam_cfg_get_one_item::key not found!\n");
            return OAL_ERR_CODE_PTR_NULL;
        }

        pc_equal_sign_addr = pc_key_addr + uc_key_len;
    }

    /* ????val???????? */
    pc_val_addr = pc_equal_sign_addr + OAM_CFG_EQUAL_SIGN_LEN;
    if ((*(pc_val_addr) == '\n') || (*(pc_val_addr) == '\0'))
    {
        return OAL_FAIL;
    }

    for (pc_tmp = pc_val_addr; (*pc_tmp != '\n') && (*pc_tmp != '\0'); pc_tmp++)
    {
        ac_val[uc_index] = *pc_tmp;
        uc_index++;
    }

    /* ????????????????????10??????????????switch-case???? TBD */
    *pl_val = oal_atoi(ac_val);

    return OAL_SUCC;
}



oal_int32  oam_cfg_read_file_to_buf(
                                                    oal_int8   *pc_cfg_data_buf,
                                                    oal_uint32  ul_file_size)
{
    oal_file_stru     *p_file;
    oal_int32          l_ret;

    p_file = oal_file_open_readonly(OAL_CFG_FILE_PATH);
    if (OAL_PTR_NULL == p_file)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    l_ret = oal_file_read(p_file, pc_cfg_data_buf, ul_file_size);
    if (0 >= l_ret)
    {
        oal_file_close(p_file);
        return OAL_FAIL;
    }

    oal_file_close(p_file);

    return OAL_SUCC;
}


oal_uint32  oam_cfg_decrypt_all_item(
                                              oal_aes_key_stru *pst_aes_key,
                                              oal_int8         *pc_ciphertext,
                                              oal_int8         *pc_plaintext,
                                              oal_uint32        ul_cipher_len)
{
    oal_uint32      ul_loop  = 0;
    oal_uint32      ul_round = 0;
    oal_uint8      *puc_cipher_tmp;
    oal_uint8      *puc_plain_tmp;

    /* AES??????????????16??????????????????????16?????????????????????? */
    if (0 != (ul_cipher_len % OAL_AES_BLOCK_SIZE))
    {
        OAL_IO_PRINT("oam_cfg_decrypt_all_item::ciphertext length invalid!\n");
        return OAL_FAIL;
    }

    if (0 == ul_cipher_len)
    {
        OAL_IO_PRINT("oam_cfg_decrypt_all_item::ciphertext length is 0!\n");
        return OAL_FAIL;
    }

    ul_round = (ul_cipher_len >> 4);
    puc_cipher_tmp = (oal_uint8 *)pc_ciphertext;
    puc_plain_tmp  = (oal_uint8 *)pc_plaintext;

    while (ul_loop < ul_round)
    {
        oal_aes_decrypt(pst_aes_key, puc_plain_tmp, puc_cipher_tmp);

        ul_loop++;
        puc_cipher_tmp += OAL_AES_BLOCK_SIZE;
        puc_plain_tmp  += OAL_AES_BLOCK_SIZE;
    }

    return OAL_SUCC;
}


/*lint -e19*/
oal_module_symbol(oam_register_init_hook);
oal_module_symbol(oam_cfg_get_one_item);
oal_module_symbol(oam_cfg_read_file_to_buf);
oal_module_symbol(oam_cfg_decrypt_all_item);









#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

