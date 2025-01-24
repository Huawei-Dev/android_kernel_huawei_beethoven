

#ifndef __OAL_WINDOWS_MEM_H__
#define __OAL_WINDOWS_MEM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/


/*****************************************************************************
  2 ??????
*****************************************************************************/
#define OAL_HCC_HDR_LEN 8
#define OAL_MAX_CB_LEN  19
#define OAL_TX_CB_LEN   19

#define MAX_MAC_HEAD_LEN            WLAN_MAX_MAC_HDR_LEN              //??????4??????????MAC??????30 4??????????32
#define MAC_HEAD_OFFSET   (OAL_NETBUF_MACHDR_BYTE_LEN - MAX_MAC_HEAD_LEN)
#define CB_OFFSET         (OAL_NETBUF_MACHDR_BYTE_LEN - MAX_MAC_HEAD_LEN - OAL_MAX_CB_LEN)

/* ????enhanced????????????????????????????????????????????????4???????????? */
/* ????????????????????????oal_mem_struc????????????????????????????           */
/*                                                                           */
/* +-------------------+------------------------------------------+---------+ */
/* | oal_mem_stru addr |                    payload               | dog tag | */
/* +-------------------+------------------------------------------+---------+ */
/* |      4 byte       |                                          | 4 byte  | */
/* +-------------------+------------------------------------------+---------+ */
#define OAL_MEM_INFO_SIZE            4
/*****************************************************************************
  3 ????????
*****************************************************************************/

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

/*****************************************************************************
  ??????  : oal_mem_netbuf_pool_id_enum_uint8
  ????????:
  ????????: netbuf??????ID
*****************************************************************************/
typedef enum
{
    OAL_MEM_NETBUF_POOL_ID_SHORT_PKT = 0,                /* ?????????? */
    OAL_MEM_NETBUF_POOL_ID_MGMT_PKT,                     /* ?????? */
    OAL_MEM_NETBUF_POOL_ID_LARGE_PKT,                    /* ?????????? */
    OAL_MEM_NETBUF_POOL_ID_OAM_PKT,                      /* SDT?????? */

    OAL_MEM_NETBUF_POOL_ID_BUTT
}oal_mem_netbuf_pool_id_enum;
typedef oal_uint8 oal_mem_netbuf_pool_id_enum_uint8;

/*****************************************************************************
  ??????  : oal_netbuf_subpool_stru
  ????????: ????????netbuf??????????????
*****************************************************************************/
typedef struct
{
    oal_uint16            us_free_cnt;         /* ?????????????????????? */
    oal_uint16            us_total_cnt;        /* ???????????????????? */
    oal_uint16            us_start_index;      /* ??????????paylaod??????index */
    oal_uint16            us_len;              /* ?????????????????????? */

    /* ????oal_netbuf_stru??????????????????????????????????oal_netbuf_stru???? */
    oal_netbuf_stru       *pst_free_stack;
} oal_netbuf_subpool_stru;

/*****************************************************************************
  ??????  : oal_netbuf_pool_stru
  ????????: ????????netbuf????????????????
*****************************************************************************/
typedef struct oal_netbuf_pool
{
    oal_uint16              us_max_byte_len;        /* ???????????????????????????? */
    oal_uint16              us_mem_total_cnt;       /* ???????????????????????? */
    oal_uint16              us_mem_used_cnt;        /* ?????????????????? */
    oal_uint8               uc_subpool_cnt;         /* ?????????????????????????? */
    oal_uint8               uc_reserved;
    /* ?????????????????? */
    oal_netbuf_subpool_stru    ast_subpool_table[OAL_MEM_NETBUF_POOL_ID_BUTT];

}oal_netbuf_pool_stru;
 //typedef oal_netbuf_pool oal_netbuf_pool_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

/*****************************************************************************
  10 ????????
*****************************************************************************/
/* device ?????????? host???????????? */
extern oal_uint8 *oal_netbuf_cb(oal_netbuf_stru *pst_netbuf);
extern oal_uint8 *oal_netbuf_data(oal_netbuf_stru *pst_netbuf);
extern oal_uint8* oal_netbuf_header(oal_netbuf_stru *pst_netbuf);
extern oal_uint8* oal_netbuf_payload(oal_netbuf_stru *pst_netbuf);

extern oal_uint32  oal_netbuf_get_len(oal_netbuf_stru *pst_netbuf);
extern oal_netbuf_stru  *oal_netbuf_alloc(oal_uint32 ul_size, oal_int32 l_reserve, oal_int32 l_align);
extern oal_uint32  oal_mem_netbuf_free(oal_uint32  ul_file_id,
                                                oal_uint32      ul_line_num,
                                                oal_netbuf_stru   *pst_netbuf,
                                                oal_uint8          uc_lock);
extern oal_uint32  oal_netbuf_free(oal_netbuf_stru *pst_netbuf);
extern oal_uint8   *oal_netbuf_get_mac_hdr_addr(oal_netbuf_stru *pst_netbuf);
extern oal_void  oal_netbuf_queue_purge(oal_netbuf_head_stru  *pst_netbuf_head);
extern oal_void  oal_netbuf_set_len(oal_netbuf_stru *pst_netbuf, oal_uint32 ul_len);
extern oal_uint8   *oal_netbuf_get_hcc_hdr_addr(oal_netbuf_stru *pst_netbuf);
extern oal_uint8   *oal_netbuf_get_payload_addr(oal_netbuf_stru * pst_netbuf);
extern oal_netbuf_stru* oal_mem_multi_netbuf_alloc(oal_uint32 ul_file_id, oal_uint32 ul_line_num, oal_uint32 ul_len);
extern oal_netbuf_stru *oal_netbuf_get_ctrl_blk(oal_uint16  us_index);
extern oal_uint32 oal_mem_multi_netbuf_free(oal_uint32  ul_line_num, oal_netbuf_stru   *pst_netbuf);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_mm.h */

