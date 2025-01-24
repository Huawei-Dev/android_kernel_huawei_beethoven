

#ifndef __OAL_TYPES_H__
#define __OAL_TYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "arch/oal_types.h"
/*****************************************************************************
  2 ????????????????
*****************************************************************************/
typedef float                   oal_float32;        /* ????????:f */
typedef double                  oal_float64;        /* ????????:d */

/*******************************************************************************
    ??????????????32??????????32bits????64??????????64bits
*******************************************************************************/
typedef unsigned long           oal_uint;           /* ????????:ui */
typedef signed long             oal_int;            /* ????????:i */
typedef unsigned long           oal_ulong;           /*??????????????*/
typedef signed long             oal_long;            /*????????:i */

/*****************************************************************************
  3 ??????
*****************************************************************************/
#define OAL_PTR_NULL        (0L)                        /* ????????????????????????????????OAL_PTR_NULL???????? */

#ifndef NULL
#define NULL    OAL_PTR_NULL
#endif

#define OAL_REFERENCE(data)	((void)(data))

/* linux?????? */
#define OAL_EFAIL               1   /* ?????????????????? -1 */
#define OAL_EIO                 5   /* I/O error */
#define OAL_ENOMEM              12  /* Out of memory */
#define OAL_EFAUL               14  /* Bad address */
#define OAL_EBUSY               16  /* Device or resource busy */
#define OAL_ENODEV              19  /* No such device */
#define OAL_EINVAL              22  /* Invalid argument */
#define OAL_ETIMEDOUT           110	/* Connection timed out */

/*****************************************************************************
  4 ????????
*****************************************************************************/
typedef enum
{
    OAL_FALSE   = 0,
    OAL_TRUE    = 1,

    OAL_BUTT
}oal_bool_enum;
typedef oal_uint8 oal_bool_enum_uint8;

/* ???????????? */
typedef enum
{
    OAL_TRAFFIC_NORMAL= 0,					/*????????*/
    OAL_TRAFFIC_MULTI_USER_MULTI_AC   = 1,	/*??????????????????*/
    OAL_TRAFFIC_MULTI_USER_ONLY_BE   = 2,		/*32????????????????*/
    OAL_TRAFFIC_BUTT
}oal_traffic_type_enum;
typedef oal_uint8 oal_traffic_type_enum_uint8;

/* ???????????? */
typedef enum
{
    OAL_SWITCH_OFF  = 0,
    OAL_SWITCH_ON   = 1,

    OAL_SWITCH_BUTT
}oal_switch_enum;
typedef oal_uint8 oal_switch_enum_uint8;

typedef enum
{
    /**************************************************************************
        platform err code
    ***************************************************************************/
    ERR_CODE_PLATFORM_BASE  = 0x00000001,
    /**************************************************************************
        wifi err code
    ***************************************************************************/
    ERR_CODE_WIFI_BASE      = 0x10000000,
    /**************************************************************************
        bt device code
    ***************************************************************************/
    ERR_CODE_BT_BASE        = 0x20000000,
    /**************************************************************************
        fm err code
    ***************************************************************************/
    ERR_CODE_FM_BASE        = 0x30000000,
    /**************************************************************************
        gnss err code
    ***************************************************************************/
    ERR_CODE_GNSS_BASE      = 0x40000000,
    /**************************************************************************
        nfc err code
    ***************************************************************************/
    ERR_CODE_NFC_BASE       = 0x50000000,
    /**************************************************************************
        ir device code
    ***************************************************************************/
    ERR_CODE_IR_BASE        = 0x60000000,
}err_code_enum;
typedef oal_uint32 err_code_enum_uint32;

/* ?????????????????? */
typedef enum
{
    OAL_SUCC                                  = 0,                              /* ?????????????? */

    /**************************************************************************
        plat pm exception
    **************************************************************************/
    OAL_ERR_CODE_PM_BASE                     = 10,
    OAL_ERR_CODE_ALREADY_OPEN                = (OAL_ERR_CODE_PM_BASE + 0),    /* ???????? */
    OAL_ERR_CODE_ALREADY_CLOSE               = (OAL_ERR_CODE_PM_BASE + 1),    /* ???????? */

    /**************************************************************************
        system exception
    **************************************************************************/
    OAL_ERR_CODE_SYS_BASE                     = 100,
    OAL_ERR_CODE_PTR_NULL                     = (OAL_ERR_CODE_SYS_BASE + 0),    /* ???????????? */
    OAL_ERR_CODE_ARRAY_OVERFLOW               = (OAL_ERR_CODE_SYS_BASE + 1),    /* ???????????? */
    OAL_ERR_CODE_DIV_ZERO                     = (OAL_ERR_CODE_SYS_BASE + 2),    /* ??0???? */
    OAL_ERR_CODE_ALLOC_MEM_FAIL               = (OAL_ERR_CODE_SYS_BASE + 3),    /* ???????????? */
    OAL_ERR_CODE_FREE_MEM_FAIL                = (OAL_ERR_CODE_SYS_BASE + 4),
    OAL_ERR_CODE_START_TIMRE_FAIL             = (OAL_ERR_CODE_SYS_BASE + 5),    /* ?????????????? */
    OAL_ERR_CODE_RESET_INPROGRESS             = (OAL_ERR_CODE_SYS_BASE + 6),    /* ?????????? */
    OAL_ERR_CODE_MAC_DEVICE_NULL              = (OAL_ERR_CODE_SYS_BASE + 7),    /* mac_device_struz?????? */
	OAL_ERR_CODE_MAGIC_NUM_FAIL               = (OAL_ERR_CODE_SYS_BASE + 8),    /* ???????????????? */
	OAL_ERR_CODE_NETBUF_INDEX_CHANGE          = (OAL_ERR_CODE_SYS_BASE + 9),    /*netbuf ?????? */
    OAL_ERR_CODE_CFG_REG_TIMEOUT              = (OAL_ERR_CODE_SYS_BASE + 10),   /* ?????????????? */
    OAL_ERR_CODE_CFG_REG_ERROR                = (OAL_ERR_CODE_SYS_BASE + 11),   /* ?????????????? */
    OAL_ERR_CODE_LIST_NOT_EMPTY_ERROR         = (OAL_ERR_CODE_SYS_BASE + 12),   /* ???????????????????????????????????? */

    OAL_ERR_SYS_BUTT                          = (OAL_ERR_CODE_SYS_BASE + 999),        /* system error??????*/

    /**************************************************************************
        config exception
    ***************************************************************************/
    OAL_ERR_CODE_CONFIG_BASE                  = 1000,
    OAL_ERR_CODE_INVALID_CONFIG               = (OAL_ERR_CODE_CONFIG_BASE + 0), /* ???????? */
    OAL_ERR_CODE_CONFIG_UNSUPPORT             = (OAL_ERR_CODE_CONFIG_BASE + 1), /* ?????????? */
    OAL_ERR_CODE_CONFIG_EXCEED_SPEC           = (OAL_ERR_CODE_CONFIG_BASE + 2), /* ???????????? */
    OAL_ERR_CODE_CONFIG_TIMEOUT               = (OAL_ERR_CODE_CONFIG_BASE + 3), /* ???????? */
    OAL_ERR_CODE_CONFIG_BUSY                  = (OAL_ERR_CODE_CONFIG_BASE + 4),
    OAL_ERR_CODE_ADD_VAP_INDX_UNSYNC          = (OAL_ERR_CODE_CONFIG_BASE + 5), /* HMAC??DMAC????vap????index?????? */
    OAL_ERR_CODE_ADD_MULTI_USER_INDX_UNSYNC   = (OAL_ERR_CODE_CONFIG_BASE + 6), /* HMAC??DMAC????multi user????index?????? */
    OAL_ERR_CODE_USER_RES_CNT_ZERO            = (OAL_ERR_CODE_CONFIG_BASE + 7), /* ?????????????????????????? */

    OAL_ERR_CODE_CONFIG_BUTT                  = (OAL_ERR_CODE_CONFIG_BASE + 99),/* ??????????????*/

    /**************************************************************************
        MSG exception
    **************************************************************************/
    OAL_ERR_CODE_MSG_BASE                      = 1100,
    OAL_ERR_CODE_MSG_TYPE_ERR                  = (OAL_ERR_CODE_MSG_BASE + 0),   /* ???????????????? */
    OAL_ERR_CODE_MSG_NOT_CMPTBL_WITH_STATE     = (OAL_ERR_CODE_MSG_BASE + 1),   /* ?????????????????????? */
    OAL_ERR_CODE_MSG_IE_MISS                   = (OAL_ERR_CODE_MSG_BASE + 2),   /* ????IE???? */
    OAL_ERR_CODE_MSG_IE_VALUE_ERR              = (OAL_ERR_CODE_MSG_BASE + 3),   /* ????IE???????? */
    OAL_ERR_CODE_IPC_QUEUE_FULL                = (OAL_ERR_CODE_MSG_BASE + 4),   /* ipc?????????????????????? */
    OAL_ERR_CODE_MSG_NOT_FIND_STA_TAB          = (OAL_ERR_CODE_MSG_BASE + 5),   /* ???????????????? */
    OAL_ERR_CODE_MSG_NOT_FIND_ACT_TAB          = (OAL_ERR_CODE_MSG_BASE + 6),   /* ???????????????? */
    OAL_ERR_CODE_MSG_ACT_FUN_NULL              = (OAL_ERR_CODE_MSG_BASE + 7),   /* ????????????????????NULL */
    OAL_ERR_CODE_MSG_LENGTH_ERR                = (OAL_ERR_CODE_MSG_BASE + 8),   /* ???????????? */

    OAL_ERR_CODE_MSG_BUTT                      = (OAL_ERR_CODE_MSG_BASE + 99),  /* ??????????????*/

    /**************************************************************************
        ??????????????
    **************************************************************************/
    OAL_ERR_CODE_FILE_BASE                     = 1200,
    OAL_ERR_CODE_OPEN_FILE_FAIL                = (OAL_ERR_CODE_FILE_BASE + 0),
    OAL_ERR_CODE_WRITE_FILE_FAIL               = (OAL_ERR_CODE_FILE_BASE + 1),
    OAL_ERR_CODE_READ_FILE_FAIL                = (OAL_ERR_CODE_FILE_BASE + 2),
    OAL_ERR_CODE_CLOSE_FILE_FAIL               = (OAL_ERR_CODE_FILE_BASE + 3),

    OAL_ERR_CODE_FILE_BUTT                     = (OAL_ERR_CODE_FILE_BASE + 99), /*??????????????????*/

    /**************************************************************************
        ????????????????
    **************************************************************************/
    /**************************** ?????????????? *****************************/
    OAL_ERR_CODE_DSCR_BASE                     = 10000,
    OAL_ERR_CODE_RX_DSCR_AMSDU_DISORDER        = (OAL_ERR_CODE_DSCR_BASE + 0),  /* AMSDU???????????????? */
    OAL_ERR_CODE_RX_DSCR_LOSE                  = (OAL_ERR_CODE_DSCR_BASE + 1),  /* ????????buf?????????? */

    OAL_ERR_CODE_DSCR_BUTT                     = (OAL_ERR_CODE_DSCR_BASE + 999),

    /**************************************************************************
        ????????????????,??20000??????????????????100??
    **************************************************************************/
    /**************************** AMSDU???? ***********************************/
    OAL_ERR_CODE_HMAC_AMSDU_BASE               = 20000,
    OAL_ERR_CODE_HMAC_AMSDU_DISABLE            = (OAL_ERR_CODE_HMAC_AMSDU_BASE + 0),  /* amsdu???????? */
    OAL_ERR_CODE_HMAC_MSDU_LEN_OVER            = (OAL_ERR_CODE_HMAC_AMSDU_BASE + 1),

    OAL_ERR_CODE_HMAC_AMSDU_BUTT               = (OAL_ERR_CODE_HMAC_AMSDU_BASE + 999),

    /********************************* ???? **********************************/
    /**************************** 11i ???? ***********************************/
    OAL_ERR_CODE_SECURITY_BASE               = 21000,
    OAL_ERR_CODE_SECURITY_KEY_TYPE           = (OAL_ERR_CODE_SECURITY_BASE + 0),
    OAL_ERR_CODE_SECURITY_KEY_LEN            = (OAL_ERR_CODE_SECURITY_BASE + 1),
    OAL_ERR_CODE_SECURITY_KEY_ID             = (OAL_ERR_CODE_SECURITY_BASE + 2),
    OAL_ERR_CODE_SECURITY_CHIPER_TYPE        = (OAL_ERR_CODE_SECURITY_BASE + 3),
    OAL_ERR_CODE_SECURITY_BUFF_NUM           = (OAL_ERR_CODE_SECURITY_BASE + 4),
    OAL_ERR_CODE_SECURITY_BUFF_LEN           = (OAL_ERR_CODE_SECURITY_BASE + 5),
    OAL_ERR_CODE_SECURITY_WRONG_KEY          = (OAL_ERR_CODE_SECURITY_BASE + 6),
    OAL_ERR_CODE_SECURITY_USER_INVAILD       = (OAL_ERR_CODE_SECURITY_BASE + 7),
    OAL_ERR_CODE_SECURITY_PARAMETERS         = (OAL_ERR_CODE_SECURITY_BASE + 8),
    OAL_ERR_CODE_SECURITY_AUTH_TYPE          = (OAL_ERR_CODE_SECURITY_BASE + 9),
    OAL_ERR_CODE_SECURITY_CAP                = (OAL_ERR_CODE_SECURITY_BASE + 10),
    OAL_ERR_CODE_SECURITY_CAP_MFP            = (OAL_ERR_CODE_SECURITY_BASE + 11),
    OAL_ERR_CODE_SECURITY_CAP_BSS            = (OAL_ERR_CODE_SECURITY_BASE + 12),
    OAL_ERR_CODE_SECURITY_CAP_PHY            = (OAL_ERR_CODE_SECURITY_BASE + 13),
    OAL_ERR_CODE_SECURITY_PORT_INVALID       = (OAL_ERR_CODE_SECURITY_BASE + 14),
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    OAL_ERR_CODE_SECURITY_MAC_INVALID        = (OAL_ERR_CODE_SECURITY_BASE + 15),
    OAL_ERR_CODE_SECURITY_MODE_INVALID       = (OAL_ERR_CODE_SECURITY_BASE + 16),
    OAL_ERR_CODE_SECURITY_LIST_FULL          = (OAL_ERR_CODE_SECURITY_BASE + 17),
    OAL_ERR_CODE_SECURITY_AGING_INVALID      = (OAL_ERR_CODE_SECURITY_BASE + 18),
    OAL_ERR_CODE_SECURITY_THRESHOLD_INVALID  = (OAL_ERR_CODE_SECURITY_BASE + 19),
    OAL_ERR_CODE_SECURITY_RESETIME_INVALID   = (OAL_ERR_CODE_SECURITY_BASE + 20),
#endif
    OAL_ERR_CODE_SECURITY_BUTT               = (OAL_ERR_CODE_SECURITY_BASE + 99),
    /* ???????????????????????????????????????????????????????? */
    OAL_ERR_CODE_HMAC_SECURITY_BASE               = 21100,
    OAL_ERR_CODE_HMAC_SECURITY_KEY_TYPE           = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 0),
    OAL_ERR_CODE_HMAC_SECURITY_KEY_LEN            = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 1),
    OAL_ERR_CODE_HMAC_SECURITY_KEY_ID             = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 2),
    OAL_ERR_CODE_HMAC_SECURITY_CHIPER_TYPE        = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 3),
    OAL_ERR_CODE_HMAC_SECURITY_BUFF_NUM           = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 4),
    OAL_ERR_CODE_HMAC_SECURITY_BUFF_LEN           = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 5),
    OAL_ERR_CODE_HMAC_SECURITY_WRONG_KEY          = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 6),
    OAL_ERR_CODE_HMAC_SECURITY_USER_INVAILD       = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 7),
    OAL_ERR_CODE_HMAC_SECURITY_PARAMETERS         = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 8),
    OAL_ERR_CODE_HMAC_SECURITY_AUTH_TYPE          = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 9),
    OAL_ERR_CODE_HMAC_SECURITY_CAP                = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 10),
    OAL_ERR_CODE_HMAC_SECURITY_CAP_MFP            = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 11),
    OAL_ERR_CODE_HMAC_SECURITY_CAP_BSS            = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 12),
    OAL_ERR_CODE_HMAC_SECURITY_CAP_PHY            = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 13),
    OAL_ERR_CODE_HMAC_SECURITY_PORT_INVALID       = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 14),
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    OAL_ERR_CODE_HMAC_SECURITY_MAC_INVALID        = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 15),
    OAL_ERR_CODE_HMAC_SECURITY_MODE_INVALID       = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 16),
    OAL_ERR_CODE_HMAC_SECURITY_LIST_FULL          = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 17),
    OAL_ERR_CODE_HMAC_SECURITY_AGING_INVALID      = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 18),
    OAL_ERR_CODE_HMAC_SECURITY_THRESHOLD_INVALID  = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 19),
    OAL_ERR_CODE_HMAC_SECURITY_RESETIME_INVALID   = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 20),
#endif
    OAL_ERR_CODE_HMAC_SECURITY_BUTT               = (OAL_ERR_CODE_HMAC_SECURITY_BASE + 499),

/**************************** wapi ???? ***********************************/
    OAL_ERR_CODE_WAPI_BASE                          = 21600,
    OAL_ERR_CODE_WAPI_NETBUFF_LEN_ERR               = (OAL_ERR_CODE_WAPI_BASE + 0),
    OAL_ERR_CODE_WAPI_DECRYPT_FAIL                  = (OAL_ERR_CODE_WAPI_BASE + 1),
    OAL_ERR_CODE_WAPI_MIC_CALC_FAIL                 = (OAL_ERR_CODE_WAPI_BASE + 2),
    OAL_ERR_CODE_WAPI_ENRYPT_FAIL                   = (OAL_ERR_CODE_WAPI_BASE + 3),
    OAL_ERR_CODE_WAPI_MIC_CMP_FAIL                  = (OAL_ERR_CODE_WAPI_BASE + 4),

    OAL_ERR_CODE_WAPI_BUTT                          = (OAL_ERR_CODE_WAPI_BASE + 99),
    /********************************* ???? **********************************/
    /**************************** 11w ???? ***********************************/
    OAL_ERR_CODE_PMF_BASE                         = 22000,
    OAL_ERR_CODE_PMF_ACTIVE_DOWN                  = (OAL_ERR_CODE_PMF_BASE + 0), /* user??bit_pmf_active???????????????? */
    OAL_ERR_CODE_PMF_SA_QUERY_REQ_SEND_FAIL       = (OAL_ERR_CODE_PMF_BASE + 1), /* hmac_send_sa_query_req????????sa query req???????? */
    OAL_ERR_CODE_PMF_DISABLED                     = (OAL_ERR_CODE_PMF_BASE + 2), /* dot11RSNAProtectedManagementFramesActivated ????0 */
    OAL_ERR_CODE_PMF_SA_QUERY_START_FAIL          = (OAL_ERR_CODE_PMF_BASE + 3), /* hmac_start_sa_query???????????????? */
    OAL_ERR_CODE_PMF_SA_QUERY_DEL_USER_FAIL       = (OAL_ERR_CODE_PMF_BASE + 4), /* hmac_sa_query_del_user????,SA query???????????????? */
    OAL_ERR_CODE_PMF_BIP_AES_CMAC_ENCRYPT_FAIL    = (OAL_ERR_CODE_PMF_BASE + 5), /* oal_crypto_aes_cmac_encrypt??????AES_CMAC???????? */
    OAL_ERR_CODE_PMF_BIP_CRIPTO_FAIL              = (OAL_ERR_CODE_PMF_BASE + 6), /* dmac_bip_crypto??????bip???????? */
    OAL_ERR_CODE_PMF_BIP_DECRIPTO_FAIL            = (OAL_ERR_CODE_PMF_BASE + 7), /* oal_crypto_bip_demic??????bip???????? */
    OAL_ERR_CODE_PMF_IGTK_INDX_FAIL               = (OAL_ERR_CODE_PMF_BASE + 8), /* ????igtk_index ???? */
    OAL_ERR_CODE_PMF_VAP_CAP_FAIL                 = (OAL_ERR_CODE_PMF_BASE + 9), /* VAP??mfpc&mfpr???????? */
    OAL_ERR_CODE_PMF_VAP_ACTIVE_DOWN              = (OAL_ERR_CODE_PMF_BASE + 10), /* VAP??mib dot11RSNAActived??????OAL_FALES */
    OAL_ERR_CODE_PMF_IGTK_NOT_EXIST               = (OAL_ERR_CODE_PMF_BASE + 11), /* igtk??????????igtk_id?????? */
    OAL_ERR_CODE_PMF_ALIGN_ERR                    = (OAL_ERR_CODE_PMF_BASE + 12), /* bip?????????????? */
    OAL_ERR_CODE_PMF_REPLAY_ATTAC                 = (OAL_ERR_CODE_PMF_BASE + 13), /* bip???????? */
    OAL_ERR_CODE_PMF_MMIE_ERR                     = (OAL_ERR_CODE_PMF_BASE + 14), /* bip?????????????????? */
    OAL_ERR_CODE_PMF_NO_PROTECTED_ERROR           = (OAL_ERR_CODE_PMF_BASE + 15), /* PMF?????????????????????????????? */

    OAL_ERR_CODE_PMF_BUTT                         = (OAL_ERR_CODE_PMF_BASE + 999),
    /********************************* ???? **********************************/
    /***************hostapd/wpa_supplicant?????????????????? *****************/
    OAL_ERR_CODE_CFG80211_BASE               = 23000,
    OAL_ERR_CODE_CFG80211_SKB_MEM_FAIL       = (OAL_ERR_CODE_CFG80211_BASE + 0),  /* skb?????????????????????? */
    OAL_ERR_CODE_CFG80211_EMSGSIZE           = (OAL_ERR_CODE_CFG80211_BASE + 1),  /* ????????,???????????? */
    OAL_ERR_CODE_CFG80211_MCS_EXCEED         = (OAL_ERR_CODE_CFG80211_BASE + 2),  /* MCS????32 */
    OAL_ERR_CODE_CFG80211_ENOBUFS            = (OAL_ERR_CODE_CFG80211_BASE + 3),

    OAL_ERR_CODE_CFG80211_BUTT               = (OAL_ERR_CODE_CFG80211_BASE + 999),

    /********************************* OAL **********************************/
    OAL_ERR_CODE_OAL_BASE                    = 24000,

    /**************************** OAL --- ?????? ****************************/
    OAL_ERR_CODE_OAL_MEM_BASE                = (OAL_ERR_CODE_OAL_BASE + 0),
    OAL_ERR_CODE_OAL_MEM_GET_POOL_FAIL       = (OAL_ERR_CODE_OAL_MEM_BASE + 0),   /* ?????????????????????? */
    OAL_ERR_CODE_OAL_MEM_ALLOC_CTRL_BLK_FAIL = (OAL_ERR_CODE_OAL_MEM_BASE + 1),   /* ?????????????? */
    OAL_ERR_CODE_OAL_MEM_SKB_SUBPOOL_ID_ERR  = (OAL_ERR_CODE_OAL_MEM_BASE + 2),   /* ????netbuf subpool id???? */
    OAL_ERR_CODE_OAL_MEM_GET_CFG_TBL_FAIL    = (OAL_ERR_CODE_OAL_MEM_BASE + 3),   /* ?????????????????????? */
    OAL_ERR_CODE_OAL_MEM_EXCEED_MAX_LEN      = (OAL_ERR_CODE_OAL_MEM_BASE + 4),   /* ?????????????????????? */
    OAL_ERR_CODE_OAL_MEM_EXCEED_SUBPOOL_CNT  = (OAL_ERR_CODE_OAL_MEM_BASE + 5),   /* ???????????????????? */
    OAL_ERR_CODE_OAL_MEM_DOG_TAG             = (OAL_ERR_CODE_OAL_MEM_BASE + 6),   /* ???????? */
    OAL_ERR_CODE_OAL_MEM_ALREADY_FREE        = (OAL_ERR_CODE_OAL_MEM_BASE + 7),   /* ?????????????????????????? */
    OAL_ERR_CODE_OAL_MEM_USER_CNT_ERR        = (OAL_ERR_CODE_OAL_MEM_BASE + 8),   /* ??????????????????0?????? */
    OAL_ERR_CODE_OAL_MEM_EXCEED_TOTAL_CNT    = (OAL_ERR_CODE_OAL_MEM_BASE + 9),   /* ???????????????????????????????????????? */
#ifdef _PRE_WLAN_FEATURE_SMP_SUPPORT
    /**************************** OAL --- ???? ****************************/
    OAL_ERR_CODE_OAL_EVENT_BASE              = (OAL_ERR_CODE_OAL_BASE + 100),
    OAL_ERR_CODE_OAL_EVENT_Q_EMPTY           = (OAL_ERR_CODE_OAL_EVENT_BASE + 0),
#endif
    OAL_ERR_CODE_OAL_BUTT                    = (OAL_ERR_CODE_OAL_BASE + 999),

    /********************************* OAM **********************************/
    OAL_ERR_CODE_OAM_BASE                    = 25000,

    /**************************** OAM --- event *****************************/
    OAL_ERR_CODE_OAM_EVT_BASE               = (OAL_ERR_CODE_OAM_BASE + 0),
    OAL_ERR_CODE_OAM_EVT_USER_IDX_EXCEED    = (OAL_ERR_CODE_OAM_EVT_BASE + 0),    /* ?????????????????? */
    OAL_ERR_CODE_OAM_EVT_FRAME_DIR_INVALID  = (OAL_ERR_CODE_OAM_EVT_BASE + 1),    /* ?????????????????????????????? */
    OAL_ERR_CODE_OAM_EVT_FR_HDR_LEN_INVALID = (OAL_ERR_CODE_OAM_EVT_BASE + 2),    /* ???????????? */
    OAL_ERR_CODE_OAM_EVT_FR_LEN_INVALID     = (OAL_ERR_CODE_OAM_EVT_BASE + 3),    /* ????(????????)???????? */
    OAL_ERR_CODE_OAM_EVT_DSCR_LEN_INVALID   = (OAL_ERR_CODE_OAM_EVT_BASE + 4),    /* ?????????????? */

    OAL_ERR_CODE_OAM_BUTT                    = (OAL_ERR_CODE_OAM_BASE + 999),

    /********************************* KeepAlive **********************************/
    OAL_ERR_CODE_KEEPALIVE_BASE             = 26000,
    /**************************** KeepAlive --- event *****************************/
    OAL_ERR_CODE_KEEPALIVE_CONFIG_VAP       = (OAL_ERR_CODE_KEEPALIVE_BASE + 1),
    OAL_ERR_CODE_KEEPALIVE_BIG_INTERVAL     = (OAL_ERR_CODE_KEEPALIVE_BASE + 2),
    OAL_ERR_CODE_KEEPALIVE_SMALL_LIMIT      = (OAL_ERR_CODE_KEEPALIVE_BASE + 3),
    OAL_ERR_CODE_KEEPALIVE_INVALID_VAP      = (OAL_ERR_CODE_KEEPALIVE_BASE + 4),
    OAL_ERR_CODE_KEEPALIVE_PTR_NULL         = (OAL_ERR_CODE_KEEPALIVE_BASE + 5),

    OAL_ERR_CODE_KEEPALIVE_BUTT             = (OAL_ERR_CODE_KEEPALIVE_BASE + 999),


    /******* PROXY ARP???? COMP--skb????????; INCOMP--skb??????????????????????????******/
    OAL_ERR_CODE_PROXY_ARP_BASE                     = 27000,
    OAL_ERR_CODE_PROXY_ARP_INVLD_SKB_INCOMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 0), /* ????????SKB???? */
    OAL_ERR_CODE_PROXY_ARP_LEARN_USR_NOTEXIST_COMP  = (OAL_ERR_CODE_PROXY_ARP_BASE + 1), /* GARP??????????BSS */
    OAL_ERR_CODE_PROXY_ARP_LEARN_USR_COMP           = (OAL_ERR_CODE_PROXY_ARP_BASE + 2), /* GARP?????????????? */
    OAL_ERR_CODE_PROXY_ARP_REPLY2BSS_COMP           = (OAL_ERR_CODE_PROXY_ARP_BASE + 3), /* ?? arp reply????BSS */
    OAL_ERR_CODE_PROXY_ARP_REPLY2ETH_COMP           = (OAL_ERR_CODE_PROXY_ARP_BASE + 4), /* ?? arp reply??ETH */
    OAL_ERR_CODE_PROXY_ARP_CREATE_FAIL_COMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 5), /* ????SKB???? */
    OAL_ERR_CODE_PROXY_ND_INVLD_SKB1_INCOMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 6), /* ????????SKB???? */
    OAL_ERR_CODE_PROXY_ND_INVLD_SKB2_INCOMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 7), /* ????????SKB???? */
    OAL_ERR_CODE_PROXY_ARP_REPLY_MCAST_COMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 8), /* ????????????arp reply */
    OAL_ERR_CODE_PROXY_ARP_REPLY_INCOMP             = (OAL_ERR_CODE_PROXY_ARP_BASE + 9), /* ??????arp reply?????? */
    OAL_ERR_CODE_PROXY_ARP_NOT_REQ_REPLY_INCOMP     = (OAL_ERR_CODE_PROXY_ARP_BASE + 10),/* ????arp req????reply */

    OAL_ERR_CODE_PROXY_ND_LEARN_USR_NOTEXIST_COMP   = (OAL_ERR_CODE_PROXY_ARP_BASE + 11),/* ????NS??????????ap??????ns????mac?????? */
    OAL_ERR_CODE_PROXY_ND_LEARN_USR_ALREADY_EXIST_INCOMP   = (OAL_ERR_CODE_PROXY_ARP_BASE + 12),/* ????NS??????????ap??????ipv6??????????????hash?? */
    OAL_ERR_CODE_PROXY_ND_LEARN_USR_SUCC_COMP       = (OAL_ERR_CODE_PROXY_ARP_BASE + 13),/* ????NS???????????? */
    OAL_ERR_CODE_PROXY_ND_LEARN_USR_FAIL_INCOMP     = (OAL_ERR_CODE_PROXY_ARP_BASE + 14),/* ????NS???????????? */
    OAL_ERR_CODE_PROXY_ND_NS_OPT_INVLD_COMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 15),/* ??NS??icmpv6 opt??????ll???? */
    OAL_ERR_CODE_PROXY_ND_NS_FINDUSR_ERR_COMP       = (OAL_ERR_CODE_PROXY_ARP_BASE + 16),/* NS icmpv6????target ipv6????????hash????*/
    OAL_ERR_CODE_PROXY_ND_NS_CREATE_NA_FAIL_COMP    = (OAL_ERR_CODE_PROXY_ARP_BASE + 17),/* ????NA???? */
    OAL_ERR_CODE_PROXY_ND_NS_REPLY_NA2BSS_COMP      = (OAL_ERR_CODE_PROXY_ARP_BASE + 18),/* ????NS????AP????????NA??BSS */
    OAL_ERR_CODE_PROXY_ND_NA_INVLD_COMP             = (OAL_ERR_CODE_PROXY_ARP_BASE + 19),/* ??????NA*/
    OAL_ERR_CODE_PROXY_ND_NA_MCAST_NOT_LLA_COMP     = (OAL_ERR_CODE_PROXY_ARP_BASE + 20),/* ????????????NA?? icmpv6 opt????ll????*/
    OAL_ERR_CODE_PROXY_ND_NA_UCAST_NOT_LLA_INCOMP   = (OAL_ERR_CODE_PROXY_ARP_BASE + 21),/* ????????????NA?? icmpv6 opt????ll????*/
    OAL_ERR_CODE_PROXY_ND_NA_DUP_ADDR_INCOMP        = (OAL_ERR_CODE_PROXY_ARP_BASE + 22),/* NA????????ipv6???????? */
    OAL_ERR_CODE_PROXY_ND_NA_UNSOLICITED_COMP       = (OAL_ERR_CODE_PROXY_ARP_BASE + 23),/* NA??S??????0 */
    OAL_ERR_CODE_PROXY_ND_NA_SOLICITED_INCOMP       = (OAL_ERR_CODE_PROXY_ARP_BASE + 24),/* NA??S??????1 */
    OAL_ERR_CODE_PROXY_ND_NOT_ICMPV6_INCOMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 25),/* ????????icmpv6 */
    OAL_ERR_CODE_PROXY_ND_ICMPV6_NOT_NSNA_INCOMP    = (OAL_ERR_CODE_PROXY_ARP_BASE + 26),/* ????NS????NA */
    OAL_ERR_CODE_PROXY_ARP_FINDUSR_ERR_COMP         = (OAL_ERR_CODE_PROXY_ARP_BASE + 27),/* arp????target ipv4????????hash????*/
    OAL_ERR_CODE_PROXY_OTHER_INCOMP                 = (OAL_ERR_CODE_PROXY_ARP_BASE + 28),/* ??????proxy ???????? */
    OAL_ERR_CODE_PROXY_ND_NS_REPLY_NA2ETH_COMP      = (OAL_ERR_CODE_PROXY_ARP_BASE + 29),/* ????NS????AP????????NA??ETH */
    OAL_ERR_CODE_PROXY_ARP_BUTT                     = (OAL_ERR_CODE_PROXY_ARP_BASE + 499),


    /********************************* ???????? **********************************/
    OAL_ERR_CODE_QUEUE_BASE                             = 28000,
    OAL_ERR_CODE_QUEUE_CNT_ZERO                         = (OAL_ERR_CODE_QUEUE_BASE + 0),    /* ???????? */

    /********************************* SWP CBB???? *******************************/
    OAL_ERR_CODE_SWP_CBB_BASE                           = 28100,
    OAL_ERR_CODE_SWP_CBB_ALREADY_ACTIVE                 = (OAL_ERR_CODE_SWP_CBB_BASE + 0),  /* ????CBB???????????? */
    OAL_ERR_CODE_SWP_CBB_INT_REGISTER_FAIL              = (OAL_ERR_CODE_SWP_CBB_BASE + 1),  /* ???????????????????? */
    OAL_ERR_CODE_SWP_CBB_LENGTH_INVALID                 = (OAL_ERR_CODE_SWP_CBB_BASE + 2),  /* ???????????? */
    OAL_ERR_CODE_SWP_CBB_BUFFUR_FULL                    = (OAL_ERR_CODE_SWP_CBB_BASE + 3),  /* SWP CBB RX??TX???????? */

    /********************************* Type A???? ********************************/
    OAL_ERR_CODE_TYPE_A_BASE                            = 28200,
    OAL_ERR_CODE_UID_ERR                                = (OAL_ERR_CODE_TYPE_A_BASE  + 0),  /* UID ????  */
    OAL_ERR_TIME_OUT_TIMES_BEYOND                       = (OAL_ERR_CODE_TYPE_A_BASE  + 1),  /* ???????????? */
    OAL_ERR_LEVEL_BEYOND                                = (OAL_ERR_CODE_TYPE_A_BASE  + 2),  /* ???????????? */

    /********************************* Type A LISTEN NFC-DEP???? ********************************/
    OAL_ERR_CODE_NFC_DEP_LISTEN_BASE                    = 28300,

    /********************************* Type A POLL NFC-DEP???? ********************************/
    OAL_ERR_CODE_NFC_DEP_POLL_BASE                      = 28400,

    /********************************* NFC-DEP???????? ********************************/
    OAL_ERR_CODE_NFC_DEP_BASE                           = 28500,
    OAL_ERR_CODE_NFC_DEP_FRAME_TYPE_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 0),  /* ?????????? */
    OAL_ERR_CODE_NFC_DEP_FRAME_OPCODE_ERR               = (OAL_ERR_CODE_NFC_DEP_BASE + 1),  /* ???????????? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_DID_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 2),  /* DID ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_GEN_INFO_FLAG_ERR       = (OAL_ERR_CODE_NFC_DEP_BASE + 3),  /* GEN INFO flag ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_DSI_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 4),  /* DSI ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_DRI_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 5),  /* DRI ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_FSL_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 6),  /* FSL ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_MI_ERR                  = (OAL_ERR_CODE_NFC_DEP_BASE + 7),  /* MI ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_NAD_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 8),  /* NAD ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_PNI_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 9),  /* PNI ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_PAYLOAD_ERR             = (OAL_ERR_CODE_NFC_DEP_BASE + 10), /* PAYLOAD ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_SENS_RES_ERR            = (OAL_ERR_CODE_NFC_DEP_BASE + 11), /* sens_res  ???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_TAG1_PLT_SUCC           = (OAL_ERR_CODE_NFC_DEP_BASE + 12), /* sens_res ????????tag1?????????????? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_CL_ERR                  = (OAL_ERR_CODE_NFC_DEP_BASE + 13), /* SDD_REQ ?????????? */
    OAL_ERR_CODE_NFC_DEP_NFCID_ERR                      = (OAL_ERR_CODE_NFC_DEP_BASE + 14), /* NFCID???? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_CASCADE_ERR             = (OAL_ERR_CODE_NFC_DEP_BASE + 15), /* Cascade???????? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_BCC_ERR                 = (OAL_ERR_CODE_NFC_DEP_BASE + 16), /* BCC???????? */
    OAL_ERR_CODE_NFC_DEP_TYPE_A_CT_ERR                  = (OAL_ERR_CODE_NFC_DEP_BASE + 17), /* CT?????? */

    /********************************* NFC CBB ????**********************************/
    OAL_ERR_CODE_NFC_CBB_BASE                           = 28600,
    OAL_ERR_CODE_NFC_RX_CRC_ERR                         = (OAL_ERR_CODE_NFC_CBB_BASE + 0),  /* CRC ???? */
    OAL_ERR_CODE_NFC_RX_PTY_ERR                         = (OAL_ERR_CODE_NFC_CBB_BASE + 1),  /* PTY ???? */
    OAL_ERR_CODE_NFC_RX_BCC_ERR                         = (OAL_ERR_CODE_NFC_CBB_BASE + 2),  /* BCC ???? */
    OAL_ERR_CODE_NFC_CRPLL_UNLOCK_FLAG_ERR              = (OAL_ERR_CODE_NFC_CBB_BASE + 3),  /* CRPLL ???? ???? */
    OAL_ERR_CODE_NFC_LSTNA_FALLING_FALL_ERR             = (OAL_ERR_CODE_NFC_CBB_BASE + 4),  /* FAILING EDGE ????*/
    OAL_ERR_CODE_NFC_RX_BUFF_ERR                        = (OAL_ERR_CODE_NFC_CBB_BASE + 5),  /* BUFF ???? */
    OAL_ERR_CODE_NFC_RX_BUFF_FRAME_TYPE_ERR             = (OAL_ERR_CODE_NFC_CBB_BASE + 6),  /* FRAME TYPE ???? */
    OAL_ERR_CODE_CBB_INT_REGISTER_FAIL                  = (OAL_ERR_CODE_NFC_CBB_BASE + 7),  /* INT_REGISTER_FAIL ???? */
    OAL_ERR_CODE_CBB_LSTN_RX2TX_TO                      = (OAL_ERR_CODE_NFC_CBB_BASE + 8),  /* Listen?????????????????? */
    OAL_ERR_CODE_NFC_RX_LSTN_RATE_ERR                   = (OAL_ERR_CODE_NFC_CBB_BASE + 9),  /* type f Listen???????????????????????????????? */

    /********************************* ???????? **********************************/
    OAL_ERR_CODE_SCHED_BASE                             = 28700,
    OAL_ERR_CODE_SCHED_FSM_EXCEPT_FUN_NULL              = (OAL_ERR_CODE_SCHED_BASE + 0),    /* ????????????????????NULL */
    OAL_ERR_CODE_SCHED_FSM_STA_TAB_NULL                 = (OAL_ERR_CODE_SCHED_BASE + 1),    /* ??????????????NULL???????? */
    OAL_ERR_CODE_SCHED_PUSH_QUEUE_ID_INVALID            = (OAL_ERR_CODE_SCHED_BASE + 2),    /* ????????ID???? */

    /********************************* Tag4B???? **********************************/
    OAL_ERR_CODE_TAG4B_BASE                             = 28800,
    OAL_ERR_CODE_TAG4B_NOT_COMPLIANT_14443              = (OAL_ERR_CODE_TAG4B_BASE + 0),    /* ??????14443???????? */
    OAL_ERR_CODE_TAG4B_OPCODE_ERR                       = (OAL_ERR_CODE_TAG4B_BASE + 1),    /* ATTRIB?????????? */
    OAL_ERR_CODE_TAG4B_TYPE_B_DID_ERR                   = (OAL_ERR_CODE_TAG4B_BASE + 2),    /* DID???? */
    OAL_ERR_CODE_TAG4B_NFCID_ERR                        = (OAL_ERR_CODE_TAG4B_BASE + 3),    /* NFCID???? */
    OAL_ERR_CODE_TAG4B_BR_ERR                           = (OAL_ERR_CODE_TAG4B_BASE + 4),    /* ???????? */
    OAL_ERR_CODE_TAG4B_PARAM3_MSB_ERR                   = (OAL_ERR_CODE_TAG4B_BASE + 5),    /* PARAM3 b8-b4????0 */

    /********************************* ISO-DEP???????? **********************************/
    OAL_ERR_CODE_ISO_DEP_BASE                           = 28900,
    OAL_ERR_CODE_ISO_DEP_IBLOCK_RETRY_ERR               = (OAL_ERR_CODE_ISO_DEP_BASE + 0),  /* IBLOCK?????????????????? */
    OAL_ERR_CODE_ISO_DEP_OVER_FSC_ERR                   = (OAL_ERR_CODE_ISO_DEP_BASE + 1),  /* ????????block????????FSC???? */
    OAL_ERR_CODE_ISO_DEP_OVER_FSD_ERR                   = (OAL_ERR_CODE_ISO_DEP_BASE + 2),  /* ????????block????????FSD???? */
    OAL_ERR_CODE_ISO_DEP_BLOCK_TYPE_ERR                 = (OAL_ERR_CODE_ISO_DEP_BASE + 3),  /* BLOCK???????? */
    OAL_ERR_CODE_ISO_DEP_DID_ERR                        = (OAL_ERR_CODE_ISO_DEP_BASE + 4),  /* DID???? */
    OAL_ERR_CODE_ISO_DEP_NAD_ERR                        = (OAL_ERR_CODE_ISO_DEP_BASE + 5),  /* NAD???? */
    OAL_ERR_CODE_ISO_DEP_BN_ERR                         = (OAL_ERR_CODE_ISO_DEP_BASE + 6),  /* BLOCK NUM???? */
    OAL_ERR_CODE_ISO_DEP_ACK_RETRY_ERR                  = (OAL_ERR_CODE_ISO_DEP_BASE + 7),  /* R_ACK??????????????????*/
    OAL_ERR_CODE_ISO_DEP_NAK_RETRY_ERR                  = (OAL_ERR_CODE_ISO_DEP_BASE + 8),  /* R_NAK??????????????????*/
    OAL_ERR_CODE_ISO_DEP_WTX_RETRY_ERR                  = (OAL_ERR_CODE_ISO_DEP_BASE + 9),  /* S_WTX??????????????????*/
    OAL_ERR_CODE_ISO_DEP_DSL_RETRY_ERR                  = (OAL_ERR_CODE_ISO_DEP_BASE + 10), /* S_DSL??????????????????*/
    OAL_ERR_CODE_ISO_DEP_PCB_FIX_NUM_ERR                = (OAL_ERR_CODE_ISO_DEP_BASE + 11), /* PBC??fix num???? */
    OAL_ERR_CODE_ISO_DEP_WTXM_ERR                       = (OAL_ERR_CODE_ISO_DEP_BASE + 12), /* WTXM???? */
    OAL_ERR_CODE_ISO_DEP_PROTOCOL_ERR                   = (OAL_ERR_CODE_ISO_DEP_BASE + 13), /* ???????? */
    OAL_ERR_CODE_ISO_DEP_UNRECOVERABLE_EXCEPTIOM        = (OAL_ERR_CODE_ISO_DEP_BASE + 14), /* ???????????? */

    /********************************* TYPE B???????? **********************************/
    OAL_ERR_CODE_TYPE_B_BASE                            = 29000,
    OAL_ERR_CODE_CUR_SLOT_NUM_ERR                       = (OAL_ERR_CODE_TYPE_B_BASE + 1),   /* ???????????? */
    OAL_ERR_CODE_SLOT_NUM_ERR                           = (OAL_ERR_CODE_TYPE_B_BASE + 2),   /* ?????????????? */
    OAL_ERR_CODE_TYPE_B_SENSB_RES_OPCODE_ERR            = (OAL_ERR_CODE_TYPE_B_BASE + 3),   /* SENSB_RES?????????? */
    OAL_ERR_CODE_TYPE_B_CR_AFI_ERR                      = (OAL_ERR_CODE_TYPE_B_BASE + 4),   /* AFI???????????? */
    OAL_ERR_CODE_DID_OVER_ERR                           = (OAL_ERR_CODE_TYPE_B_BASE + 5),   /* did?????????? */
    OAL_ERR_CODE_FSD_FSC_TR0_TR1_TR2_VALUE_ERR          = (OAL_ERR_CODE_TYPE_B_BASE + 6),   /* FSD????FSC???????? */
    OAL_ERR_CODE_MBL_ERR                                = (OAL_ERR_CODE_TYPE_B_BASE + 7),   /* MBL?????????? */
    /********************************* TAG4A???? **********************************/
    OAL_ERR_CODE_TAG4A_BASE                             = 29100,
    OAL_ERR_CODE_TAG4A_ATS_TL_ERR                       = (OAL_ERR_CODE_SCHED_BASE + 0),    /* ATS TL???? */
    OAL_ERR_CODE_TAG4A_PPS_RES_ERR                      = (OAL_ERR_CODE_SCHED_BASE + 1),    /* PPS_RES???? */
    OAL_ERR_CODE_TAG4A_PPS_DID_ERR                      = (OAL_ERR_CODE_SCHED_BASE + 2),    /* PPS_RES DID???? */
    OAL_ERR_CODE_TAG4A_RATS_OPCODE_ERR                  = (OAL_ERR_CODE_SCHED_BASE + 3),    /* RATS ???????? */
    OAL_ERR_CODE_TAG4A_RATS_DID_ERR                     = (OAL_ERR_CODE_SCHED_BASE + 4),    /* RATS DID???? */
    /********************************* TYPE F???????? **********************************/
    OAL_ERR_CODE_TYPE_F_BASE                            = 29200,
    OAL_ERR_CODE_TYPE_F_SENSF_RES_OPCODE_ERR            = (OAL_ERR_CODE_TYPE_F_BASE + 1),   /* SENSF_RES?????????? */
    OAL_ERR_CODE_TYPE_F_SENSF_REQ_OPCODE_ERR            = (OAL_ERR_CODE_TYPE_F_BASE + 2),   /* SENSF_REQ?????????? */
    OAL_ERR_CODE_TYPE_F_SENSF_RES_WITH_RD_ERR           = (OAL_ERR_CODE_TYPE_F_BASE + 3),   /* SENSF_RES????RD???? */
    /********************************* TAG3???????? **********************************/
    OAL_ERR_CODE_TAG3_BASE                              = 29300,
    OAL_ERR_CODE_TAG3_CUP_CMD_OPCODE_ERR                = (OAL_ERR_CODE_TAG3_BASE + 1),     /* CUP_CMD???????? */
    OAL_ERR_CODE_TAG3_CUP_RES_OPCODE_ERR                = (OAL_ERR_CODE_TAG3_BASE + 2),     /* CUP_RES???????? */
    OAL_ERR_CODE_TAG3_PAYLOAD_ERR                       = (OAL_ERR_CODE_TAG3_BASE + 3),     /* PAYLOAD???? */

    /********************************* NCI????RF DISCOVERY???? **********************************/
    OAL_ERR_CODE_RF_DISCOVERY_BASE                      = 29400,
    OAL_ERR_CODE_RF_DISCOVERY_TECH_TYPE_ERR             = (OAL_ERR_CODE_RF_DISCOVERY_BASE + 1), /* ???????????? */
    OAL_ERR_CODE_RF_DISCOVERY_MODE_ERR                  = (OAL_ERR_CODE_RF_DISCOVERY_BASE + 2), /* ?????????????? */

    /********************************* TECH DETECT ACT???? **********************************/
    OAL_ERR_CODE_TECH_DETECT_ACT_BASE                   = 29500,
    OAL_ERR_CODE_TECH_DETECT_ACT_TECH_TYPE_ERR          = (OAL_ERR_CODE_TECH_DETECT_ACT_BASE + 1), /* ?????????????????????????? */

    /********************************* NCI????????**********************************/
    OAL_ERR_CODE_NCI_BASE                               = 29600,
    OAL_ERR_CODE_NCI_CONFIG_PARAM_INVALID               = (OAL_ERR_CODE_NCI_BASE + 1),      /* ?????????? */
    OAL_ERR_CODE_NCI_UNKNOWN_MSG                        = (OAL_ERR_CODE_NCI_BASE + 2),      /* ?????????????? */
    OAL_ERR_CORE_NCI_PAYLOAD_ERR                        = (OAL_ERR_CODE_NCI_BASE + 3),      /* PAYLOAD???? */
    OAL_ERR_CODE_NCI_DISPATCH_FUN_NULL                  = (OAL_ERR_CODE_NCI_BASE + 4),      /* Dispatch??????????NULL */
    OAL_ERR_CODE_NCI_VAL_LEN_TOO_SHORT                  = (OAL_ERR_CODE_NCI_BASE + 5),      /* ?????????????????? */
    OAL_ERR_CODE_NCI_RECV_MSG_TOO_BIG                   = (OAL_ERR_CODE_NCI_BASE + 6),      /* ?????????????????????? */
    OAL_ERR_CODE_NCI_PARAM_ID_TOO_BIG                   = (OAL_ERR_CODE_NCI_BASE + 7),      /* ??????ID???????? */
    OAL_ERR_CODE_NCI_GID_OID_INVALID                    = (OAL_ERR_CODE_NCI_BASE + 8),      /* NCI??????GID??OID???? */
    OAL_ERR_CODE_NCI_PACKET_INVALID                     = (OAL_ERR_CODE_NCI_BASE + 9),      /* ????????NCI Packet???????? */

    /********************************* SHDLC????????**********************************/
    OAL_ERR_CODE_SHDLC_BASE                             = 29700,
    OAL_ERR_RECV_FRAME_TYPE_DIF_FSM                     = (OAL_ERR_CODE_SHDLC_BASE + 1),    /* ???????????????????????????? */
    OAL_ERR_RECV_RSET_LENGTH                            = (OAL_ERR_CODE_SHDLC_BASE + 2),    /* ????????RSET????payload???????????????? */
    OAL_ERR_RECV_FRAME_TYPE_UNKNOWN                     = (OAL_ERR_CODE_SHDLC_BASE + 3),    /* ?????????????????? */
    OAL_ERR_RECV_I_FRAME_LENGTH                         = (OAL_ERR_CODE_SHDLC_BASE + 4),    /* ??????????I????payload???????????????? */

/* #ifdef _PRE_WLAN_FEATURE_ROAM */
    /********************************* ????????**********************************/
    OAL_ERR_CODE_ROAM_BASE                              = 29800,
    OAL_ERR_CODE_ROAM_DISABLED                          = (OAL_ERR_CODE_ROAM_BASE + 0),      /* ??????????   */
    OAL_ERR_CODE_ROAM_INVALID_VAP                       = (OAL_ERR_CODE_ROAM_BASE + 1),      /* ??????VAP    */
    OAL_ERR_CODE_ROAM_INVALID_USER                      = (OAL_ERR_CODE_ROAM_BASE + 2),      /* ??????USER   */
    OAL_ERR_CODE_ROAM_NO_VALID_BSS                      = (OAL_ERR_CODE_ROAM_BASE + 3),      /* ????????BSS  */
    OAL_ERR_CODE_ROAM_STATE_UNEXPECT                    = (OAL_ERR_CODE_ROAM_BASE + 4),      /* ????????     */
    OAL_ERR_CODE_ROAM_EVENT_UXEXPECT                    = (OAL_ERR_CODE_ROAM_BASE + 5),      /* ????????     */
    OAL_ERR_CODE_ROAM_FRAMER_SUB_TYPE                   = (OAL_ERR_CODE_ROAM_BASE + 6),      /* ??????????   */
    OAL_ERR_CODE_ROAM_FRAMER_LEN                        = (OAL_ERR_CODE_ROAM_BASE + 7),      /* ??????????   */
    OAL_ERR_CODE_ROAM_INVALID_VAP_STATUS                = (OAL_ERR_CODE_ROAM_BASE + 8),      /* ??????VAP????*/
    OAL_ERR_CODE_ROAM_HANDSHAKE_FAIL                    = (OAL_ERR_CODE_ROAM_BASE + 9),      /* ????????????*/
/* #endif //_PRE_WLAN_FEATURE_ROAM */

    /********************************* VoWiFi????**********************************/
    OAL_ERR_CODE_VOWIFI_BASE                            = 29900,
    OAL_ERR_CODE_VOWIFI_SET_INVALID                     = (OAL_ERR_CODE_VOWIFI_BASE + 0),      /* ????????????????   */


    /********************************* HW RESET ????*************************************/
    OAL_ERR_CODE_HW_RESET_BASE                          = 30600,
    OAL_ERR_CODE_HW_RESET_PHY_SAVE_MEMALLOC             = (OAL_ERR_CODE_HW_RESET_BASE + 0),
    OAL_ERR_CODE_HW_RESET_MAC_SAVE_MEMALLOC             = (OAL_ERR_CODE_HW_RESET_BASE + 1),
    OAL_ERR_CODE_HW_RESET_MAC_SAVE_SIZELIMIT            = (OAL_ERR_CODE_HW_RESET_BASE + 2),
    OAL_ERR_CODE_HW_RESET_PHY_SAVE_SIZELIMIT            = (OAL_ERR_CODE_HW_RESET_BASE + 3),
    OAL_ERR_CODE_HW_RESET_TX_QUEUE_MEMALLOC             = (OAL_ERR_CODE_HW_RESET_BASE + 4),    /* reset??????????tx fake queue???? */

    OAL_FAIL,                                                                   /* ?????????????????????????????????????? */

    OAL_ERR_CODE_BUTT
}oal_err_code_enum;
typedef oal_uint32 oal_err_code_enum_uint32;

/*****************************************************************************
  5 ????????????
*****************************************************************************/


/*****************************************************************************
  6 ??????????
*****************************************************************************/


/*****************************************************************************
  7 ????????
*****************************************************************************/


/*****************************************************************************
  8 STRUCT????
*****************************************************************************/


/*****************************************************************************
  9 UNION????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_types.h */

