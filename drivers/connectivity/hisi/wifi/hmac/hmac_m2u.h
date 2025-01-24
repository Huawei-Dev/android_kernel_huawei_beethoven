

#ifndef __HMAC_M2U_H__
#define __HMAC_M2U_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_MCAST

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "mac_frame.h"
#include "hmac_main.h"
#include "hmac_user.h"
#include "hmac_vap.h"
#include "frw_ext_if.h"


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_M2U_H
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define HMAC_M2U_GRPADDR_FILTEROUT_NUM 8
#define HMAC_M2U_ALL_GROUP 3758096385UL      /* ???????? 224.0.0.1 */
#define HMAC_M2U_DENY_GROUP 4026531585UL     /* ???????????? 239.255.255.1 */
#define HMAC_M2U_IGMPV3_GROUP 3758096406UL   /* IGMP V3  Report GROUP 224.0.0.22 */
#define HMAC_M2U_SUBNET_MASK 4294967040UL    /* ???????? 255.255.255.0 */
#define HMAC_M2U_MIN_DENY_GROUP 3758096384UL /* ?????????????? 255.255.255.0 */
#define HMAC_M2U_MAX_DENY_GROUP 4026531839UL /* ?????????????? 255.255.255.0 */


#define MAC_ETH_PROTOCOL_SUBTYPE 0x17
#define OAL_SNAP_LEN 8    /* SNAP ???????? */
#define MIN_IP_HDR_LEN 5    /* ????IP?????? */

#ifndef MAX_SNOOP_ENTRIES
#define MAX_SNOOP_ENTRIES    32    /* max number*/
#endif

#define HMAC_DEF_M2U_TIMER         30000     /* timer interval as 30 secs */
#define HMAC_DEF_M2U_TIMEOUT       120000    /* 2 minutes for timeout  */

#if 0
#define BITFIELD_LITTLE_ENDIAN      0
#define BITFIELD_BIG_ENDIAN         1
#endif
/*****************************************************************************
  3 ????????
*****************************************************************************/
typedef enum
{
    HMAC_M2U_IGMP_CMD_OTHER         = 0,
    HMAC_M2U_IGMP_CMD_ADD_EXC_LIST  = 1,
    HMAC_M2U_IGMP_CMD_ADD_INC_LIST  = 2,
    HMAC_M2U_IGMP_CMD_DISABLE       = 3,

    HMAC_M2U_IGMP_CMD_BUTT
}hmac_m2u_update_cmd_enum;
typedef oal_uint8 hmac_m2u_igmp_cmd_enum_uint8;

typedef enum
{
    HMAC_M2U_MCAST_MAITAIN         = 0,
    HMAC_M2U_MCAST_TUNNEL          = 1,
    HMAC_M2U_MCAST_TRANSLATE       = 2,

    HMAC_M2U_MCAST_BUTT
}hmac_m2u_mcast_mode_enum;
typedef oal_uint8 hmac_m2u_mcast_mode_enum_uint8;

typedef struct
{
    #if (OAL_BYTE_ORDER == OAL_LITTLE_ENDIAN)
	oal_uint32  offset	                : 11,
	            seqNum       		    : 11,
	            optHdrLen32  		    : 2,
	            frameType    		    : 2,
	            proto        		    : 6;
    #else /* big endian */
	oal_uint32  proto	                : 6,
	            frameType    		    : 2,
	            optHdrLen32  		    : 2,
	            seqNum       		    : 11,
	            offset       		    : 11;
	#endif
}mcast_tunnel_hdr_stru;

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

/* hmac_m2u_grp_list_entry????????????????????????????????snoop_list?? */
typedef struct
{
    oal_dlist_head_stru                 st_src_list;                        /* ???????????????????? */
    oal_uint8                           auc_group_addr[WLAN_MAC_ADDR_LEN];  /* ??????????????mac???? */
    oal_uint8                           auc_reserve[2];
    oal_dlist_head_stru                 st_grp_entry;
}hmac_m2u_grp_list_entry_stru;

/* ????????????snoop???????? */
typedef struct
{
    oal_uint8                uc_deny_count;
    oal_uint16               us_group_list_count;
    oal_uint16               us_misc;
    oal_uint16               us_max_length;
    oal_uint32               ul_deny_group[HMAC_M2U_GRPADDR_FILTEROUT_NUM];
    oal_uint32               ul_deny_mask[HMAC_M2U_GRPADDR_FILTEROUT_NUM];
    oal_dlist_head_stru      st_grp_list;    /* ???????? */
}hmac_m2u_snoop_list_stru;

/**
 * hmac_m2u_grp_member_stru ????????????????????????????
 * ??????????????
 * ul_src_ip_addr - ??ip????
 * grp_member_address - ????????????????????
 * mode - include / exclude src_ip_address.
 **/
typedef struct
{
    oal_uint32                       ul_src_ip_addr;
    oal_uint32                       ul_timestamp;
    hmac_user_stru                  *pst_hmac_user;
    oal_uint8                        auc_grp_member_addr[WLAN_MAC_ADDR_LEN];
    oal_uint8                        en_mode;
    oal_dlist_head_stru              st_member_entry;
}hmac_m2u_grp_member_stru;



/* hmac_m2u_list_update_stru ??????????????????list update??????????????group?????????? */
typedef struct
{
    oal_uint32                       ul_src_ip_addr;                        /* ?????? */
    oal_uint32                       ul_timestamp;                          /* ?????? */
    oal_uint8                        auc_grp_addr[WLAN_MAC_ADDR_LEN];       /* ????????????????mac???? */
    oal_uint8                        auc_grp_member[WLAN_MAC_ADDR_LEN];     /* ??????????????????????mac???? */
    hmac_vap_stru                   *pst_hmac_vap;                           /* vap???? */
    hmac_user_stru                  *pst_hmac_user;                          /* user???? */
    hmac_m2u_igmp_cmd_enum_uint8     en_cmd;                                /* ?????????????? */
    oal_uint8                        auc_reserve[3];
}hmac_m2u_list_update_stru;

/*????????snoop????*/
typedef struct
{
  //  hmac_vap_stru                  *pst_hmac_vap;
    hmac_m2u_snoop_list_stru        st_m2u_snooplist;
    oal_bool_enum_uint8             en_snoop_enable;    /* ?????????????????????? */
    hmac_m2u_mcast_mode_enum_uint8  en_mcast_mode;      /* ???????????????????? */
    oal_bool_enum_uint8             en_discard_mcast;   /* ?????????????????????? */
    oal_uint8                       auc_reserve[1];
    frw_timeout_stru                st_snooplist_timer;
    oal_uint32                      ul_timeout;         /*??????????????????*/
}hmac_m2u_stru;
/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32 hmac_m2u_snoop_convert( hmac_vap_stru *pst_vap, oal_netbuf_stru *pst_buf);
extern oal_void hmac_m2u_snoop_inspecting(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_buf);
extern oal_void hmac_m2u_attach(hmac_vap_stru *pst_hmac_vap);
extern oal_void hmac_m2u_detach(hmac_vap_stru * pst_hmac_vap);
extern oal_uint32 hmac_m2u_update_snoop_list(hmac_m2u_list_update_stru *pst_list_entry);
extern oal_void hmac_m2u_add_snoop_deny_entry(hmac_vap_stru *pst_hmac_vap, oal_uint32 *ul_grpaddr);
extern oal_void hmac_m2u_clear_deny_table(hmac_vap_stru *pst_hmac_vap);
extern oal_void hmac_m2u_show_snoop_deny_table(hmac_vap_stru *pst_hmac_vap);
extern oal_void hmac_m2u_print_all_snoop_list(hmac_vap_stru *pst_hmac_vap);
extern oal_void hmac_m2u_cleanup_snoopwds_node(hmac_user_stru *pst_hmac_user);
extern oal_uint32 hmac_m2u_igmp_v1v2_update(hmac_vap_stru *pst_hmac_vap, hmac_m2u_list_update_stru *pst_list_entry, mac_igmp_header_stru *pst_igmp);


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_m2u.h */
