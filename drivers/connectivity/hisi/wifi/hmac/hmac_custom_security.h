

#ifndef __HMAC_CRYPTO_FRAME_H__
#define __HMAC_CRYPTO_FRAME_H__

#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "mac_vap.h"

/*****************************************************************************
  2 ??????
*****************************************************************************/
#define CS_INVALID_AGING_TIME         0
#define CS_DEFAULT_AGING_TIME         3600
#define CS_DEFAULT_RESET_TIME         3600
#define CS_DEFAULT_THRESHOLD          100



/*****************************************************************************
  3 ????????
*****************************************************************************/

/* ???????? mode*/
typedef enum
{
    CS_ISOLATION_MODE_BROADCAST=0x01,        /* ????     */
    CS_ISOLATION_MODE_MULTICAST=0x02,        /* ????     */
    CS_ISOLATION_MODE_UNICAST=0x04,          /* ????     */
}cs_isolation_mode_enum;

/* ???????? */
typedef enum
{
    CS_ISOLATION_TYPE_NONE,             /* ????????        */
    CS_ISOLATION_TYPE_MULTI_BSS,        /* ??BSS????       */
    CS_ISOLATION_TYPE_SINGLE_BSS,       /* ??BSS????       */

    CS_ISOLATION_TYPE_BUTT
}cs_isolation_type_enum;
typedef oal_uint8 cs_isolation_type_enum_uint8;

/* ????forwording???? */
typedef enum
{
    CS_ISOLATION_FORWORD_NONE,         /* ????forword????????       */
    CS_ISOLATION_FORWORD_TOLAN,        /* ????forword????????lan  */
    CS_ISOLATION_FORWORD_DROP,         /* ????forword??????       */

    CS_ISOLATION_FORWORD_BUTT
}cs_isolation_forward_enum;
typedef oal_uint8 cs_isolation_forward_enum_uint8;


/* ?????????? */
typedef enum
{
    CS_BLACKLIST_MODE_NONE,            /* ????         */
    CS_BLACKLIST_MODE_BLACK,           /* ??????       */
    CS_BLACKLIST_MODE_WHITE,           /* ??????       */

    CS_BLACKLIST_MODE_BUTT
}cs_blacklist_mode_enum;
typedef oal_uint8 cs_blacklist_mode_enum_uint8;

/* ?????????? */
typedef enum
{
    CS_BLACKLIST_TYPE_ADD,           /* ????       */
    CS_BLACKLIST_TYPE_DEL,           /* ????       */

    CS_BLACKLIST_TYPE_BUTT
}cs_blacklist_type_enum;
typedef oal_uint8 cs_blacklist_type_enum_uint8;




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
/* ?????????????????? */
typedef struct
{
    oal_uint8                       uc_enabled;               /* ???????? 0:??????  1:???? */
    oal_uint8                       auc_reserved[3];          /* ????????                  */
    oal_uint32                      ul_threshold;             /* ????                      */
    oal_uint32                      ul_reset_time;            /* ????????                  */
    oal_uint32                      ul_aging_time;            /* ????????                  */
} hmac_autoblacklist_cfg_stru;

/* ???????????? */
typedef struct
{
    oal_uint8                       uc_type;                  /* ????????    */
    oal_uint8                       uc_mode;                  /* ????????    */
    oal_uint8                       auc_sa[6];                /* mac????     */
} hmac_blacklist_cfg_stru;


/* ???????????? */
typedef struct
{
    oal_uint8                      uc_isolation_type;        /* ????????  */
    oal_uint8                      uc_isolation_mode;        /* ????????  */
    oal_uint8                      auc_reserved[2];          /* ????????  */
}hmac_isolation_cfg_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32 hmac_isolation_set_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_mode);
extern oal_uint32 hmac_isolation_set_type(mac_vap_stru *pst_mac_vap, oal_uint8 uc_type);
extern oal_uint32 hmac_isolation_set_forward(mac_vap_stru *pst_mac_vap, oal_uint8 uc_forward);
extern cs_isolation_forward_enum hmac_isolation_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr);
extern oal_uint32 hmac_autoblacklist_enable(mac_vap_stru *pst_mac_vap, oal_uint8 uc_enabled);
extern oal_uint32 hmac_autoblacklist_set_aging(mac_vap_stru *pst_mac_vap, oal_uint32 ul_aging_time);
extern oal_uint32 hmac_autoblacklist_set_threshold(mac_vap_stru *pst_mac_vap, oal_uint32 ul_threshold);
extern oal_uint32 hmac_autoblacklist_set_reset_time(mac_vap_stru *pst_mac_vap, oal_uint32 ul_reset_time);
extern oal_void hmac_autoblacklist_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr);
extern oal_uint32 hmac_blacklist_set_mode(mac_vap_stru *pst_mac_vap, oal_uint8 uc_mode);
extern oal_uint32 hmac_blacklist_add(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time);
extern oal_uint32 hmac_blacklist_add_only(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr, oal_uint32 ul_aging_time);
extern oal_uint32 hmac_blacklist_del(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr);

extern oal_uint32 hmac_blacklist_update(mac_vap_stru *pst_mac_vap, hmac_blacklist_cfg_stru *pst_blacklist_cfg);
extern oal_bool_enum_uint8 hmac_blacklist_filter(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr);
extern oal_void hmac_show_blacklist_info(mac_vap_stru *pst_mac_vap);
extern oal_uint32 hmac_backlist_get_drop(mac_vap_stru *pst_mac_vap, oal_uint8 *puc_mac_addr);
extern oal_uint32 hmac_isolation_clear_counter(mac_vap_stru *pst_mac_vap);
extern oal_uint32 hmac_isolation_get_bcast_counter(mac_vap_stru *pst_mac_vap);
extern oal_uint32 hmac_isolation_get_mcast_counter(mac_vap_stru *pst_mac_vap);
extern oal_uint32 hmac_isolation_get_ucast_counter(mac_vap_stru *pst_mac_vap);
extern oal_void hmac_show_isolation_info(mac_vap_stru *pst_mac_vap);
extern oal_uint8 hmac_backlist_get_list_num(mac_vap_stru *pst_mac_vap);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif  /* #ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY */

#endif /* end of hmac_crypto_tkip.h */

