

#ifndef __OAM_MAIN_H__
#define __OAM_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#ifdef _PRE_WIFI_DMT
#include "dmt_stub.h"
#endif

/*****************************************************************************
  2 ??????
*****************************************************************************/
#define OAM_BEACON_HDR_LEN              24
#define OAM_TIMER_MAX_LEN               36
#define OAM_PRINT_CRLF_NUM              20              /* ???????????????? */

#define OAM_FEATURE_NAME_ABBR_LEN            (12)           /* ???????????????????? */

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1151)
#define OAM_DATA2SDT_FAIL_COUNT     1000
#endif

/* ???????????? */
typedef oal_uint32  (* oal_print_func)(oal_int8 *pc_string);

/* ???????????????? */
typedef oal_int8*   oam_va_list;

/*****************************************************************************
  3 ????????
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
/* ?????????????????????? */
typedef struct
{
    oam_feature_enum_uint8              en_feature_id;
    oal_uint8                           auc_feature_name_abbr[OAM_FEATURE_NAME_ABBR_LEN]; /* ??0???? */
}oam_software_feature_stru;

/* ????ul_interval??????ul_burst?????????? */
typedef enum
{
    OAM_RATELIMIT_OUTPUT        = 0,
    OAM_RATELIMIT_NOT_OUTPUT,
}oam_ratelimit_output_enum;
typedef oal_uint8   oam_ratelimit_output_enum_uint8;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

/*****************************************************************************
  4 ????????????
*****************************************************************************/
//extern oam_mng_ctx_stru             g_st_oam_mng_ctx;
extern oam_software_feature_stru    gst_oam_feature_list[];

/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_int32  oam_main_init(oal_void);
extern oal_void   oam_main_exit(oal_void);
extern oal_uint32   oam_send_device_data2sdt(oal_uint8* pc_string, oal_uint16 len);
extern oal_uint32  oam_set_file_path(oal_int8 *pc_file_path, oal_uint32 ul_length);
extern oal_uint32  oam_set_output_type(oam_output_type_enum_uint8 en_output_type);
extern oal_uint32  oam_print(oal_int8 *pc_string);
extern oal_uint32  oam_print_to_file(oal_int8 *pc_string);
extern oal_uint32  oam_print_to_sdt(oal_int8 *pc_string);
extern oal_uint32  oam_print_to_console(oal_int8 *pc_string);
extern oal_uint32  oam_upload_log_to_sdt(oal_int8 *pc_string);
extern oal_void oam_dump_buff_by_hex(oal_uint8 *puc_buff, oal_int32 l_len, oal_int32 l_num);
extern oam_ratelimit_output_enum_uint8 oam_log_ratelimit(oam_ratelimit_type_enum_uint8 en_ratelimit_type );


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oam_main */
