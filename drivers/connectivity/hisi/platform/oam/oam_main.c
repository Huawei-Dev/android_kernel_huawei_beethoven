


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oam_main.h"
#include "oam_log.h"
#include "oam_event.h"
#include "oam_alarm.h"
#include "oam_trace.h"
#include "oam_statistics.h"
#if (!defined(_PRE_PRODUCT_ID_HI110X_DEV))
#include "oam_config.h"
#endif
#include "oam_linux_netlink.h"
#include "oam_ext_if.h"

#include "oal_hcc_host_if.h"


/*****************************************************************************
  2 ????????????
*****************************************************************************/
/* OAM??????????????????????????????????????OAM???????????????????? */
oam_mng_ctx_stru    g_st_oam_mng_ctx;

/* ???????????????? */
OAL_STATIC oal_print_func g_pa_oam_print_type_func[OAM_OUTPUT_TYPE_BUTT]=
{
    oam_print_to_console,   /* OAM_OUTPUT_TYPE_CONSOLE ?????????? */
    oam_print_to_file,      /* OAM_OUTPUT_TYPE_FS ???????????? */
    oam_print_to_sdt,       /* OAM_OUTPUT_TYPE_SDT ??????SDT,??????????????????2048 */
};

/* ??????SDT?????????????????? */
oam_sdt_func_hook_stru          g_st_oam_sdt_func_hook;
oam_wal_func_hook_stru          g_st_oam_wal_func_hook;
oam_sdt_stat_info_stru          g_st_sdt_stat_info;

oal_uint8 g_auc_bcast_addr[WLAN_MAC_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#ifdef _PRE_DEBUG_MODE  /* ???????????????????? */
oal_uint32          g_aul_debug_feature_switch[OAM_DEBUG_TYPE_BUTT] =
{
    OAL_SWITCH_OFF,   /* OAM_DEBUG_TYPE_ECHO_REG */
};
#endif

/* ???????? */
oam_software_feature_stru   gst_oam_feature_list[OAM_SOFTWARE_FEATURE_BUTT] =
{
    /*??????ID                  ??????????*/
    /* 0 */
    {OAM_SF_SCAN,               "scan"},
    {OAM_SF_AUTH,               "auth"},
    {OAM_SF_ASSOC,              "assoc"},
    {OAM_SF_FRAME_FILTER,       "ff"},
    {OAM_SF_WMM,                "wmm"},

    /* 5 */
    {OAM_SF_DFS,                "dfs"},
    {OAM_SF_NETWORK_MEASURE,    "nm"},
    {OAM_SF_ENTERPRISE_VO,      "ev"},
    {OAM_SF_HOTSPOTROAM,        "roam"},
    {OAM_SF_NETWROK_ANNOUNCE,   "11u"},

    /* 10 */
    {OAM_SF_NETWORK_MGMT,       "11k"},
    {OAM_SF_NETWORK_PWS,        "pws"},
    {OAM_SF_PROXYARP,           "proxyarp"},
    {OAM_SF_TDLS,               "tdls"},
    {OAM_SF_CALIBRATE,          "cali"},

    /* 15 */
    {OAM_SF_EQUIP_TEST,         "equip"},
    {OAM_SF_CRYPTO,             "crypto"},
    {OAM_SF_WPA,                "wpa"},
    {OAM_SF_WEP,                "wep"},
    {OAM_SF_WPS,                "wps"},

    /* 20 */
    {OAM_SF_PMF,                "pmf"},
    {OAM_SF_WAPI,               "wapi"},
    {OAM_SF_BA,                 "ba"},
    {OAM_SF_AMPDU,              "ampdu"},
    {OAM_SF_AMSDU,              "amsdu"},

    /* 25 */
    {OAM_SF_STABILITY,          "dfr"},
    {OAM_SF_TCP_OPT,            "tcp"},
    {OAM_SF_ACS,                "acs"},
    {OAM_SF_AUTORATE,           "autorate"},
    {OAM_SF_TXBF,               "txbf"},

    /* 30 */
    {OAM_SF_DYN_RECV,           "weak"},
    {OAM_SF_VIVO,               "vivo"},
    {OAM_SF_MULTI_USER,         "muser"},
    {OAM_SF_MULTI_TRAFFIC,      "mtraff"},
    {OAM_SF_ANTI_INTF,          "anti_intf"},

    /* 35 */
    {OAM_SF_EDCA,               "edca"},
    {OAM_SF_SMART_ANTENNA,      "ani"},
    {OAM_SF_TPC,                "tpc"},
    {OAM_SF_TX_CHAIN,           "txchain"},
    {OAM_SF_RSSI,               "rssi"},

    /* 40 */
    {OAM_SF_WOW,                "wow"},
    {OAM_SF_GREEN_AP,           "green"},
    {OAM_SF_PWR,                "pwr"},
    {OAM_SF_SMPS,               "smps"},
    {OAM_SF_TXOP,               "txop"},

    /* 45 */
    {OAM_SF_WIFI_BEACON,        "beacon"},
    {OAM_SF_KA_AP,              "alive"},
    {OAM_SF_MULTI_VAP,          "mvap"},
    {OAM_SF_2040,               "2040"},
    {OAM_SF_DBAC,               "dbac"},

    /* 50 */
    {OAM_SF_PROXYSTA,           "proxysta"},
    {OAM_SF_UM,                 "um"},
    {OAM_SF_P2P,                "p2p"},
    {OAM_SF_M2U,                "m2u"},
    {OAM_SF_IRQ,                "irq"},

    /* 55 */
    {OAM_SF_TX,                 "tx"},
    {OAM_SF_RX,                 "rx"},
    {OAM_SF_DUG_COEX,           "dugcoex"},
    {OAM_SF_CFG,                "cfg"},
    {OAM_SF_FRW,                "frw"},

    /* 60 */
    {OAM_SF_KEEPALIVE,          "keepalive"},
    {OAM_SF_COEX,               "coex"},
    {OAM_SF_HS20,               "hs20"},
    {OAM_SF_MWO_DET,            "mwodet"},
    {OAM_SF_CCA_OPT,            "cca"},

    /* 65 */
    {OAM_SF_ROAM,               "roam"},
    {OAM_SF_DFT,                "dft"},
    {OAM_SF_DFR,                "dfr"},
    {OAM_SF_VOWIFI,             "vowifi"},
    {OAM_SF_WMMAC,              "wmmac"},

    {OAM_SF_ANY,                "any"},
    {OAM_SF_HILINK,             "hilink"},

};

/*****************************************************************************
  3 ????????
*****************************************************************************/

oal_uint32  oam_print(oal_int8 *pc_string)
{
    oam_output_type_enum_uint8 en_output_type;
    oal_uint32                 ul_rslt;

    ul_rslt = oam_get_output_type(&en_output_type);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    ul_rslt = g_pa_oam_print_type_func[en_output_type](pc_string);
    if (OAL_SUCC != ul_rslt)
    {
        return ul_rslt;
    }

    return OAL_SUCC;
}


oal_uint32 oam_print_to_console(oal_int8 *pc_string)
{
    if (OAL_UNLIKELY(OAL_PTR_NULL == pc_string))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    OAL_IO_PRINT("%s\r\n", pc_string);

    return OAL_SUCC;
}


oal_uint32   oam_print_to_file(oal_int8 *pc_string)
{
#ifdef _PRE_WIFI_DMT

    oal_file_stru            *f_file_ret;                                 /* ???????????????????????? */
    oal_file_stru            *f_event_file;
    oal_int32                 l_rslt;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pc_string))
    {
        OAM_IO_PRINTK("null param. \r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    f_event_file = oal_file_open_append(g_st_oam_mng_ctx.ac_file_path);
    if (OAL_UNLIKELY(OAL_FILE_FAIL == f_event_file))
    {
        OAM_IO_PRINTK("open file failed. \r\n");
        return OAL_ERR_CODE_OPEN_FILE_FAIL;
    }

    f_file_ret = oal_file_write(f_event_file, pc_string, (OAL_STRLEN(pc_string) + 1));
    if (OAL_FILE_FAIL == f_file_ret)
    {
        l_rslt = oal_file_close(f_event_file);
        if (0 != l_rslt)
        {
            OAM_IO_PRINTK("close file failed. \r\n");
            return OAL_ERR_CODE_CLOSE_FILE_FAIL;
        }

        OAM_IO_PRINTK("write file failed. \r\n");
        return OAL_ERR_CODE_WRITE_FILE_FAIL;
    }

    l_rslt = oal_file_close(f_event_file);

    if (0 != l_rslt)
    {
        OAM_IO_PRINTK("close file failed. \r\n");
        return OAL_ERR_CODE_CLOSE_FILE_FAIL;
    }
#endif
    return OAL_SUCC;
}


oal_uint32 oam_print_to_sdt(oal_int8 *pc_string)
{
    oal_netbuf_stru                *pst_skb;
    oal_uint32                      ul_ret       = OAL_SUCC;
    oal_uint16                      us_strlen;

    if (OAL_UNLIKELY(OAL_PTR_NULL == pc_string))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_UNLIKELY(OAL_PTR_NULL == g_st_oam_sdt_func_hook.p_sdt_report_data_func))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ????sdt????????'0'???? */
    us_strlen = (oal_uint16)OAL_STRLEN(pc_string);

    us_strlen = (us_strlen > OAM_REPORT_MAX_STRING_LEN) ? OAM_REPORT_MAX_STRING_LEN : us_strlen;

    pst_skb = oam_alloc_data2sdt(us_strlen);
    if (OAL_PTR_NULL == pst_skb)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* COPY?????????? */
    oal_memset(oal_netbuf_data(pst_skb), 0, us_strlen);
    oal_memcopy(oal_netbuf_data(pst_skb), pc_string, (oal_uint32)us_strlen);

    /* ??????sdt???????????????????????????? */
    ul_ret = oam_report_data2sdt(pst_skb, OAM_DATA_TYPE_STRING, OAM_PRIMID_TYPE_OUTPUT_CONTENT);

    return ul_ret;
}


oal_uint32 oam_upload_log_to_sdt(oal_int8 *pc_string)
{
    oal_netbuf_stru        *pst_skb;
    oal_uint32              ul_ret      = OAL_SUCC;

    if (OAL_UNLIKELY(OAL_PTR_NULL == g_st_oam_sdt_func_hook.p_sdt_report_data_func))
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_PTR_NULL == pc_string)
    {
        OAL_IO_PRINT("oam_upload_log_to_sdt::pc_string is null!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_skb = oam_alloc_data2sdt(OAL_SIZEOF(oam_log_info_stru));
    if (OAL_PTR_NULL == pst_skb)
    {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* COPY?????????? */
    oal_memcopy(oal_netbuf_data(pst_skb), pc_string, OAL_SIZEOF(oam_log_info_stru));

    /* ??????sdt???????????????????????????? */
    ul_ret = oam_report_data2sdt(pst_skb, OAM_DATA_TYPE_LOG, OAM_PRIMID_TYPE_OUTPUT_CONTENT);

    return ul_ret;
}

oal_uint32 oam_upload_device_log_to_sdt(oal_uint8 *pc_string, oal_uint16 len)
{
    oal_netbuf_stru        *pst_skb;
    oal_uint32              ul_ret      = OAL_SUCC;

    if (OAL_PTR_NULL == pc_string)
    {
        OAL_IO_PRINT("oam_upload_log_to_sdt::pc_string is null!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_skb = oam_alloc_data2sdt(len);
    if (OAL_PTR_NULL == pst_skb)
    {
        OAL_IO_PRINT("alloc netbuf stru failed!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* COPY?????????? */
    oal_memcopy(oal_netbuf_data(pst_skb), pc_string, len);

    /* ??????sdt???????????????????????????? */
    ul_ret = oam_report_data2sdt(pst_skb, OAM_DATA_TYPE_DEVICE_LOG, OAM_PRIMID_TYPE_OUTPUT_CONTENT);

    return ul_ret;
}


oal_uint32 oam_send_device_data2sdt(oal_uint8* pc_string, oal_uint16 len)
{
    oal_uint32 ul_ret = OAL_SUCC;
    if (NULL == pc_string)
    {
        return OAL_EFAIL;
    }

    ul_ret = oam_upload_device_log_to_sdt(pc_string, len);

    return ul_ret;
}

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
oal_int32 oam_rx_post_action_function(oal_uint8 stype,
                                             hcc_netbuf_stru* pst_hcc_netbuf, oal_uint8 *pst_context)
{
    oal_uint8   *puc_data;
    OAL_REFERENCE(pst_context);
    OAL_BUG_ON(NULL == pst_hcc_netbuf);

    puc_data = oal_netbuf_data(pst_hcc_netbuf->pst_netbuf);
    if((DUMP_REG == stype) || (DUMP_MEM == stype))
    {
       //AL_IO_PRINT("receive dump msg  = %d\n", stype);
        exception_bcpu_dump_recv(puc_data,pst_hcc_netbuf->pst_netbuf);
        oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
        return OAL_SUCC;
    }

    /* ????OAM????*/
    oam_send_device_data2sdt(puc_data, (oal_uint16)pst_hcc_netbuf->len);

    oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
    return OAL_SUCC;
}
oal_int32 chr_rx_post_action_function(oal_uint8 stype,
                                             hcc_netbuf_stru* pst_hcc_netbuf, oal_uint8 *pst_context)
{
    oal_uint8 *puc_data;
    OAL_REFERENCE(pst_context);
    OAL_BUG_ON(NULL == pst_hcc_netbuf);

    puc_data = oal_netbuf_data(pst_hcc_netbuf->pst_netbuf);
    chr_dev_exception_callback(puc_data, (oal_uint16)pst_hcc_netbuf->len);
    oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
    return OAL_SUCC;
}

#endif


oal_uint32  oam_get_output_type(oam_output_type_enum_uint8 *pen_output_type)
{

    *pen_output_type = g_st_oam_mng_ctx.en_output_type;

    return OAL_SUCC;
}


oal_uint32  oam_set_output_type(oam_output_type_enum_uint8 en_output_type)
{
    if (en_output_type >= OAM_OUTPUT_TYPE_BUTT)
    {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    g_st_oam_mng_ctx.en_output_type = en_output_type;

    return OAL_SUCC;
}


oal_uint32  oam_set_file_path(oal_int8 *pc_file_path, oal_uint32 ul_length)
{
#ifdef _PRE_WIFI_DMT
    oal_file_stru         *f_event_file;
	oal_uint8             *puc_file_path;

    if (OAL_PTR_NULL == pc_file_path)
	{
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (ul_length > OAM_FILE_PATH_LENGTH)
    {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    puc_file_path = DmtStub_GetDebugFilePath();
    oal_memcopy(g_st_oam_mng_ctx.ac_file_path, puc_file_path, strlen(puc_file_path));

    /* ???????????????????????????????????? */
    f_event_file = oal_file_open_rw(g_st_oam_mng_ctx.ac_file_path);
	if (OAL_FILE_FAIL == f_event_file)
    {
        return OAL_ERR_CODE_WRITE_FILE_FAIL;
    }

    if (0 != oal_file_close(f_event_file))
    {
        return OAL_ERR_CODE_CLOSE_FILE_FAIL;
    }

#else

    if (OAL_PTR_NULL == pc_file_path)
	{
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (ul_length > OAM_FILE_PATH_LENGTH)
    {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    oal_memcopy(g_st_oam_mng_ctx.ac_file_path, pc_file_path, ul_length);

#endif

    return OAL_SUCC;
}


oal_void  oam_dump_buff_by_hex(oal_uint8 *puc_buff, oal_int32 l_len, oal_int32 l_num)
{
    oal_int32       l_loop;

    for (l_loop = 0; l_loop < l_len; l_loop++)
    {
        OAL_IO_PRINT("%02x ", puc_buff[l_loop]);

        if (0 == (l_loop + 1) % l_num)
        {
            OAL_IO_PRINT("\n");
        }
    }

    OAL_IO_PRINT("\n");
}


OAL_STATIC oal_void oam_drv_func_hook_init(oal_void)
{
    /* sdt???????????????????? */
    g_st_oam_sdt_func_hook.p_sdt_report_data_func       = OAL_PTR_NULL;
    g_st_oam_sdt_func_hook.p_sdt_get_wq_len_func        = OAL_PTR_NULL;

    /* wal???????????????????? */
    g_st_oam_wal_func_hook.p_wal_recv_cfg_data_func     = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_mem_data_func     = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_reg_data_func     = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_global_var_func   = OAL_PTR_NULL;
}


oal_void oam_sdt_func_fook_register(oam_sdt_func_hook_stru *pfun_st_oam_sdt_hook)
{
    g_st_oam_sdt_func_hook.p_sdt_report_data_func = pfun_st_oam_sdt_hook->p_sdt_report_data_func;
    g_st_oam_sdt_func_hook.p_sdt_get_wq_len_func  = pfun_st_oam_sdt_hook->p_sdt_get_wq_len_func;
}


oal_void oam_wal_func_fook_register(oam_wal_func_hook_stru *pfun_st_oam_wal_hook)
{
    g_st_oam_wal_func_hook.p_wal_recv_cfg_data_func     = pfun_st_oam_wal_hook->p_wal_recv_cfg_data_func;
    g_st_oam_wal_func_hook.p_wal_recv_mem_data_func     = pfun_st_oam_wal_hook->p_wal_recv_mem_data_func;
    g_st_oam_wal_func_hook.p_wal_recv_reg_data_func     = pfun_st_oam_wal_hook->p_wal_recv_reg_data_func;
    g_st_oam_wal_func_hook.p_wal_recv_global_var_func   = pfun_st_oam_wal_hook->p_wal_recv_global_var_func;
}


oal_uint32  oam_filter_data2sdt(oam_data_type_enum_uint8 en_type)
{
    if (g_st_sdt_stat_info.ul_wq_len < WLAN_SDT_MSG_FLT_HIGH_THD)
    {
        g_st_sdt_stat_info.en_filter_switch = OAL_FALSE;
        return OAM_FLT_PASS;
    }
    else if ((g_st_sdt_stat_info.ul_wq_len >= WLAN_SDT_MSG_FLT_HIGH_THD)
              && (g_st_sdt_stat_info.ul_wq_len < WLAN_SDT_MSG_QUEUE_MAX_LEN))
    {
        /* ???????????????????????????????????? */
        g_st_sdt_stat_info.en_filter_switch = OAL_TRUE;
        return ((oal_uint8)OAM_DATA_TYPE_LOG == en_type) ? OAM_FLT_PASS : OAM_FLT_DROP;/* [false alarm]:??????????????0????1????????*/
    }

    /* ?????????????????? */
    return OAM_FLT_DROP;
}


oal_netbuf_stru *oam_alloc_data2sdt(oal_uint16  us_data_len)
{
    oal_netbuf_stru    *pst_netbuf = OAL_PTR_NULL;
#if ((_PRE_OS_VERSION_RAW != _PRE_OS_VERSION) && (_PRE_OS_VERSION_WIN32_RAW != _PRE_OS_VERSION))
    pst_netbuf = oal_mem_sdt_netbuf_alloc(us_data_len+WLAN_SDT_SKB_RESERVE_LEN, OAL_TRUE);
    if (OAL_PTR_NULL == pst_netbuf)
    {
        return OAL_PTR_NULL;
    }

    oal_netbuf_reserve(pst_netbuf, WLAN_SDT_SKB_HEADROOM_LEN);

    oal_netbuf_put(pst_netbuf, us_data_len);
#endif
    return pst_netbuf;
}


oal_uint32 oam_report_data2sdt(oal_netbuf_stru *pst_netbuf,
                               oam_data_type_enum_uint8 en_type,
                               oam_primid_type_enum_uint8 en_prim)
{
    /* ????sdt???????????????????????????????????? */
    if (OAL_LIKELY(OAL_PTR_NULL != g_st_oam_sdt_func_hook.p_sdt_get_wq_len_func))
    {
        g_st_sdt_stat_info.ul_wq_len = (oal_uint32)g_st_oam_sdt_func_hook.p_sdt_get_wq_len_func();
    }

    if (OAM_FLT_PASS != oam_filter_data2sdt(en_type))
    {
        OAM_SDT_STAT_INCR(ul_filter_cnt);
        oal_mem_sdt_netbuf_free(pst_netbuf, OAL_TRUE);
        //oal_netbuf_free(pst_netbuf);

        /* Note: ????????????????????????????????warning????????*/
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
        if(0 != ((g_st_sdt_stat_info.ul_filter_cnt) % OAM_DATA2SDT_FAIL_COUNT))
        {
            return OAL_SUCC;
        }
#endif
        return OAL_FAIL;
    }

    if (OAL_UNLIKELY(OAL_PTR_NULL == g_st_oam_sdt_func_hook.p_sdt_report_data_func))
    {
        OAL_IO_PRINT("oam_report_data2sdt p_sdt_report_data_func is NULL. \n");
        return OAL_FAIL;
    }

    g_st_oam_sdt_func_hook.p_sdt_report_data_func(pst_netbuf, en_type, en_prim);

    return OAL_SUCC;
}


oal_void oam_sdt_func_fook_unregister(oal_void)
{
    /* ???????????? */
    g_st_oam_sdt_func_hook.p_sdt_report_data_func           = OAL_PTR_NULL;
    g_st_oam_sdt_func_hook.p_sdt_get_wq_len_func            = OAL_PTR_NULL;
}


oal_void oam_wal_func_fook_unregister(oal_void)
{
    /* ???????????? */
    g_st_oam_wal_func_hook.p_wal_recv_cfg_data_func         = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_global_var_func       = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_mem_data_func         = OAL_PTR_NULL;
    g_st_oam_wal_func_hook.p_wal_recv_reg_data_func         = OAL_PTR_NULL;
}

oal_int32  oam_main_init(oal_void)
{
    oal_uint32 ul_rslt;

    /* ????????????????FILE???? */
    ul_rslt = oam_set_file_path(WLAN_OAM_FILE_PATH, (OAL_STRLEN(WLAN_OAM_FILE_PATH) + 1));
    if (OAL_SUCC != ul_rslt)
    {
        OAL_IO_PRINT("oam_main_init call oam_set_file_path fail %d\n",ul_rslt);
        OAL_BUG_ON(1);
        return -OAL_EFAIL;
    }

    /* ?????????????????????? */
    ul_rslt = oam_set_output_type(OAM_OUTPUT_TYPE_SDT);
    if (OAL_SUCC != ul_rslt)
    {
        OAL_IO_PRINT("oam_main_init call oam_set_output_type fail %d\n",ul_rslt);
        OAL_BUG_ON(1);
        return -OAL_EFAIL;
    }

    /* ????LOG???????????????? */
    ul_rslt = oam_log_init();
    if (OAL_SUCC != ul_rslt)
    {
        OAL_IO_PRINT("oam_main_init call oam_log_init fail %d\n",ul_rslt);
        OAL_BUG_ON(1);
        return -OAL_EFAIL;
    }

    /* ????EVENT???????????????? */
    ul_rslt = oam_event_init();
    if (OAL_SUCC != ul_rslt)
    {
        OAL_IO_PRINT("oam_main_init call oam_event_init fail %d\n",ul_rslt);
        OAL_BUG_ON(1);
        return -OAL_EFAIL;
    }

    /* ??????5115timer???????????????????????????? */
    oal_5115timer_init();

#ifdef _PRE_PROFILING_MODE
    /* ????PROFILING???????????????? */
    ul_rslt = oam_profiling_init();
    if (OAL_SUCC != ul_rslt)
    {
        OAL_IO_PRINT("oam_main_init call oam_profiling_init fail %d\n",ul_rslt);
        OAL_BUG_ON(1);
        return -OAL_EFAIL;
    }
#endif

    /* ??????oam?????????????? */
    oam_drv_func_hook_init();

    /* ?????????????? */
    oam_statistics_init();

/*TBD??????????????????????????*/
#if 0
    /* ????????????????????????????????OAM?????????? */
    for (ul_counter= 0; ul_counter < OM_MODULEID_BUTT; ul_counter++)
    {
        if (OAL_PTR_NULL != g_oam_customize.customize_init[ul_counter])
        {
            g_oam_customize.customize_init[ul_counter]();
        }
    }
#endif

#ifdef _PRE_WLAN_DFT_REG
    oam_reg_init();
#endif

#if ((_PRE_OS_VERSION_RAW != _PRE_OS_VERSION) && (_PRE_OS_VERSION_WIN32_RAW != _PRE_OS_VERSION))
        /* ??netlink */
 #if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
    /* ??????????????????????????????02?????????????????????????? TBD */
        ul_rslt = oam_netlink_kernel_create();
        if (OAL_SUCC != ul_rslt)
        {
            OAL_IO_PRINT("oam_main_init call oam_netlink_kernel_create fail %d\n",ul_rslt);
            OAL_BUG_ON(1);
            return -OAL_EFAIL;
        }
 #endif
#endif
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    hcc_rx_register(hcc_get_default_handler(),HCC_ACTION_TYPE_OAM,oam_rx_post_action_function,NULL);
    hcc_rx_register(hcc_get_default_handler(),HCC_ACTION_TYPE_CHR,chr_rx_post_action_function,NULL);
#endif
    return OAL_SUCC;
}


oal_void  oam_main_exit(oal_void)
{

    /* ??????5115timer???????????????????????????? */
    oal_5115timer_exit();

#ifdef _PRE_WLAN_DFT_REG
    oam_reg_exit();
#endif
    /* ???????????????????????? */

    /* ???????????????????? */
#if ((_PRE_OS_VERSION_RAW != _PRE_OS_VERSION) && (_PRE_OS_VERSION_WIN32_RAW != _PRE_OS_VERSION))
    #if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
       /* ??????????????????????????????02?????????????????????????? TBD */

        oam_netlink_kernel_release();
    #endif
#endif

    return ;
}

/*lint -e578*//*lint -e19*/
#if (_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID)
oal_module_init(oam_main_init);
oal_module_exit(oam_main_exit);
#endif
/*lint -e19*/
oal_module_symbol(oam_main_init);
oal_module_symbol(oam_main_exit);
oal_module_symbol(oam_send_device_data2sdt);
oal_module_symbol(oam_set_file_path);
oal_module_symbol(oam_set_output_type);
oal_module_symbol(oam_get_output_type);
oal_module_symbol(oam_print);
oal_module_symbol(g_st_oam_mng_ctx);
oal_module_symbol(oam_dump_buff_by_hex);
oal_module_symbol(g_st_oam_sdt_func_hook);
oal_module_symbol(g_st_oam_wal_func_hook);
oal_module_symbol(oam_sdt_func_fook_register);
oal_module_symbol(oam_sdt_func_fook_unregister);
oal_module_symbol(oam_wal_func_fook_register);
oal_module_symbol(oam_wal_func_fook_unregister);
oal_module_symbol(oam_report_data2sdt);
oal_module_symbol(g_st_sdt_stat_info);
oal_module_symbol(oam_alloc_data2sdt);
oal_module_symbol(gst_oam_feature_list);
oal_module_symbol(g_auc_bcast_addr);

#ifdef _PRE_DEBUG_MODE
oal_module_symbol(g_aul_debug_feature_switch);
#endif

oal_module_license("GPL");







#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

