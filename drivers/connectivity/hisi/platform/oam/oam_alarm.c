


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oam_alarm.h"
#include "wlan_spec.h"
#include "oam_main.h"

/*****************************************************************************
  2 ????????????
*****************************************************************************/


/*****************************************************************************
  3 ????????
*****************************************************************************/

oal_uint32  oam_alarm_get_switch(
                oal_uint8              uc_vap_id,
                oal_switch_enum_uint8 *pen_switch_type)
{
    if (OAL_PTR_NULL == pen_switch_type)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_vap_id >= WLAN_VAP_SUPPOTR_MAX_NUM_SPEC)
    {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    *pen_switch_type = g_st_oam_mng_ctx.ast_alarm_ctx[uc_vap_id].en_alarm_switch;

    return OAL_SUCC;
}


oal_uint32  oam_alarm_set_switch(
                oal_uint8              uc_vap_id,
                oal_switch_enum_uint8  en_switch_type)
{
    if (en_switch_type >= OAL_SWITCH_BUTT)
    {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (uc_vap_id >= WLAN_VAP_SUPPOTR_MAX_NUM_SPEC)
    {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    g_st_oam_mng_ctx.ast_alarm_ctx[uc_vap_id].en_alarm_switch = en_switch_type;

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  oam_alarm_format_string(
                oal_int8                       *pac_output_data,
                oal_uint16                      ul_data_len,
                oal_uint8                       uc_vap_id,
                oal_uint16                      us_file_no,
                oal_uint32                      ul_file_line_no,
                oam_module_id_enum_uint16       en_mod,
                oam_alarm_type_enum_uint16      en_alarm_type)
{
    oal_uint32 ul_tick;

    if (OAL_PTR_NULL == pac_output_data)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????????TICK?? */
    ul_tick = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    /* ?????????????? */
    OAL_SPRINTF(pac_output_data,
                ul_data_len,
                "??ALARM??:Tick=%u, FileId=%d, LineNo=%u, VAP =%d, ModId=%d,ALARM TYPE = %u\r\n",
                ul_tick,
                us_file_no,
                ul_file_line_no,
                uc_vap_id,
                en_mod,
                en_alarm_type);

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  oam_alarm_print_to_std(
                oal_uint8                   uc_vap_id,
                oal_uint16                  us_file_no,
                oal_uint32                  ul_file_line_no,
                oam_module_id_enum_uint16   en_mod,
                oam_alarm_type_enum_uint16  en_alarm_type)
{
    oal_int8   ac_output_data[OAM_PRINT_FORMAT_LENGTH];  /* ?????????????????????????? */
    oal_uint32 ul_rslt;

    ul_rslt = oam_alarm_format_string(ac_output_data,
                                      OAM_PRINT_FORMAT_LENGTH,
                                      uc_vap_id,
                                      us_file_no,
                                      ul_file_line_no,
                                      en_mod,
                                      en_alarm_type);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    ul_rslt = oam_print_to_console(ac_output_data);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  oam_alarm_print_to_file(
                oal_uint8                   uc_vap_id,
                oal_uint16                  us_file_no,
                oal_uint32                  ul_file_line_no,
                oam_module_id_enum_uint16   en_mod,
                oam_alarm_type_enum_uint16  en_alarm_type)
{
    oal_int8   ac_output_data[OAM_PRINT_FORMAT_LENGTH]; /* ?????????????????????????? */
    oal_uint32 ul_rslt;

    ul_rslt = oam_alarm_format_string(ac_output_data,
                                      OAM_PRINT_FORMAT_LENGTH,
                                      uc_vap_id,
                                      us_file_no,
                                      ul_file_line_no,
                                      en_mod,
                                      en_alarm_type);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    ul_rslt = oam_print_to_file(ac_output_data);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32  oam_alarm_print_to_sdt(
                oal_uint8                   uc_vap_id,
                oal_uint16                  us_file_no,
                oal_uint32                  ul_file_line_no,
                oam_module_id_enum_uint16   en_mod,
                oam_alarm_type_enum_uint16  en_alarm_type)
{
    oal_int8   ac_output_data[OAM_PRINT_FORMAT_LENGTH]; /* ?????????????????????????? */
    oal_uint32 ul_rslt;

    ul_rslt = oam_alarm_format_string(ac_output_data,
                                      OAM_PRINT_FORMAT_LENGTH,
                                      uc_vap_id,
                                      us_file_no,
                                      ul_file_line_no,
                                      en_mod,
                                      en_alarm_type);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    ul_rslt = oam_upload_log_to_sdt(ac_output_data);

    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    return OAL_SUCC;
}


oal_uint32  oam_alarm_report(
                oal_uint8                   uc_vap_id,
                oal_uint16                  us_file_no,
                oal_uint32                  ul_file_line_no,
                oam_module_id_enum_uint16   en_mod,
                oam_alarm_type_enum_uint16  en_alarm_type)
{
    oal_uint32 ul_rslt;

    if (en_alarm_type >= OAM_ALARM_TYPE_BUTT)
    {
        return OAL_FAIL;
    }

    if (uc_vap_id >= WLAN_VAP_SUPPOTR_MAX_NUM_SPEC)
    {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ????VAP????????????ALARM???? */
    if (OAL_SWITCH_OFF == g_st_oam_mng_ctx.ast_alarm_ctx[uc_vap_id].en_alarm_switch)
    {
        return OAL_SUCC;
    }

    switch (g_st_oam_mng_ctx.en_output_type)
    {
        /* ???????????? */
        case OAM_OUTPUT_TYPE_CONSOLE:
            ul_rslt = oam_alarm_print_to_std(uc_vap_id, us_file_no, ul_file_line_no, en_mod, en_alarm_type);

            break;

        /* ???????????????? */
        case OAM_OUTPUT_TYPE_FS:
            ul_rslt = oam_alarm_print_to_file(uc_vap_id, us_file_no, ul_file_line_no, en_mod, en_alarm_type);
            break;

        /* ??????PC?????????????? */
        case OAM_OUTPUT_TYPE_SDT:
            ul_rslt = oam_alarm_print_to_sdt(uc_vap_id, us_file_no, ul_file_line_no, en_mod, en_alarm_type);

            break;

        /* ???????? */
        default:
            ul_rslt = OAL_ERR_CODE_INVALID_CONFIG;

            break;
    }

    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    return OAL_SUCC;
}


oal_uint32  oam_alarm_init(oal_void)
{
    oal_uint32 ul_rslt;
    oal_uint32 ul_loop;

    /* ??????????VAP????ALARM???????? */
    for (ul_loop = 0; ul_loop < WLAN_VAP_SUPPOTR_MAX_NUM_SPEC; ul_loop++)
    {
        ul_rslt = oam_alarm_set_switch((oal_uint8)ul_loop, OAL_SWITCH_ON);
        if (OAL_SUCC != ul_rslt)
        {
            return ul_rslt;
        }
    }

    return OAL_SUCC;
}


/*lint -e19*/
oal_module_symbol(oam_alarm_set_switch);
oal_module_symbol(oam_alarm_get_switch);
oal_module_symbol(oam_alarm_report);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

