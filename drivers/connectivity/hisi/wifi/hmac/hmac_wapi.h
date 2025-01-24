


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "oal_net.h"
#include "mac_resource.h"
#include "hmac_vap.h"
#include "hmac_user.h"
//#include "mac_11i.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WAPI_H
#ifdef _PRE_WLAN_FEATURE_WAPI
/*****************************************************************************/
/*****************************************************************************
  2 ??????
*****************************************************************************/
#define WAPI_UCAST_INC                  2       /* ??????????????????,pn???????? */
#define WAPI_BCAST_INC                  1       /* ??????????????????,pn???????? */
#define WAPI_WAI_TYPE                  (oal_uint16)0x88B4 /* wapi?????????? */

#define WAPI_BCAST_KEY_TYPE             1
#define WAPI_UCAST_KEY_TYPE             0

#define SMS4_MIC_LEN                    (oal_uint8)16     /* SMS4????MIC?????? */

#define SMS4_PN_LEN                     16     /* wapi pn?????? */
#define SMS4_KEY_IDX                    1      /* WAPI???? keyidx??1?????? */
#define SMS4_WAPI_HDR_RESERVE           1      /* WAPI???????????? */
#define HMAC_WAPI_HDR_LEN               (oal_uint8)(SMS4_PN_LEN + SMS4_KEY_IDX + SMS4_WAPI_HDR_RESERVE)
#define WAPI_PDU_LEN                    2      /* wapi??????wapi pdu len?????????????? */
#define SMS4_PADDING_LEN                16     /* mic data????16???????? */

#define SMS4_MIC_PART1_QOS_LEN          48 /* ????????????????qos??????mic????????16???????????????? */
#define SMS4_MIC_PART1_NO_QOS_LEN       32 /* ??????????????????qos??????mic????????16???????????????? */
//#define WAPI_OUI                        {0x00,0x14,0x72}

#define WAPI_IE_VERSION                     1   /* wapi??version */
#define WAPI_IE_VER_SIZE                    2   /* wapi ver-ie ?????????? */
#define WAPI_IE_SUIT_TYPE_SIZE              1   /* suit type size */
#define WAPI_IE_WAPICAP_SIZE                2   /* wapi cap?????????????? */
#define WAPI_IE_BKIDCNT_SIZE                2   /* wapi bkid???????????????? */
#define WAPI_IE_BKID_SIZE                   16  /* ????bkid?????????? */
#define WAPI_IE_OUI_SIZE                    3   /* wapi oui?????? */
#define WAPI_IE_SMS4                        1   /* wapi??????????sms4 */
#define WAPI_IE_SUITCNT_SIZE                2   /* wapi suit count?????????? */
/*wapi key len*/


//typedef void (* wapi_func)(void);



#define WAPI_IS_PORT_VALID(pst_wapi) ((pst_wapi)->uc_port_valid)

#define WAPI_IS_WORK(pst_hmac_vap)   ((pst_hmac_vap)->uc_wapi)
#define PADDING(x, size)           (((x) + (size) - 1) & (~ ((size) - 1)))

#ifdef _PRE_WAPI_DEBUG
#define WAPI_TX_DROP_INC(pst_wapi)              pst_wapi->st_debug.ultx_ucast_drop++
#define WAPI_TX_WAI_INC(pst_wapi)               pst_wapi->st_debug.ultx_wai++
#define WAPI_TX_PORT_VALID(pst_wapi)            pst_wapi->st_debug.ultx_port_valid++
#define WAPI_RX_PORT_VALID(pst_wapi)            pst_wapi->st_debug.ulrx_port_valid++
#define WAPI_RX_IDX_ERR(pst_wapi)               pst_wapi->st_debug.ulrx_idx_err++
#define WAPI_RX_NETBUF_LEN_ERR(pst_wapi)        pst_wapi->st_debug.ulrx_netbuff_len_err++
#define WAPI_RX_IDX_UPDATE_ERR(pst_wapi)        pst_wapi->st_debug.ulrx_idx_update_err++
#define WAPI_RX_KEY_EN_ERR(pst_wapi)            pst_wapi->st_debug.ulrx_key_en_err++

#define WAPI_RX_PN_ODD_ERR(pst_wapi, pn)                                        \
           do {(pst_wapi)->st_debug.ulrx_pn_odd_err++;                          \
                oal_memcopy((pst_wapi)->st_debug.aucrx_pn, pn, WAPI_PN_LEN);    \
}while(0)

#define WAPI_RX_PN_REPLAY_ERR(pst_wapi, pn)                                    \
          do {(pst_wapi)->st_debug.ulrx_pn_replay_err++;                       \
               oal_memcopy((pst_wapi)->st_debug.aucrx_pn, pn, WAPI_PN_LEN);    \
}while(0)

#define WAPI_RX_MEMALLOC_ERR(pst_wapi)          pst_wapi->st_debug.ulrx_memalloc_err++
#define WAPI_RX_MIC_ERR(pst_wapi)               pst_wapi->st_debug.ulrx_mic_calc_fail++
#define WAPI_RX_DECRYPT_OK(pst_wapi)            pst_wapi->st_debug.ulrx_decrypt_ok++

#define WAPI_TX_MEMALLOC_ERR(pst_wapi)          pst_wapi->st_debug.ultx_memalloc_err++
#define WAPI_TX_MIC_ERR(pst_wapi)               pst_wapi->st_debug.ultx_mic_calc_fail++
//#define WAPI_TX_WAI_DROP_INC(pst_wapi)          pst_wapi->st_debug.ultx_drop_wai++
#define WAPI_TX_ENCRYPT_OK(pst_wapi)            pst_wapi->st_debug.ultx_encrypt_ok++

#else
#define WAPI_TX_DROP_INC(pst_wapi)
#define WAPI_TX_WAI_INC(pst_wapi)
#define WAPI_TX_PORT_VALID(pst_wapi)
#define WAPI_RX_PORT_VALID(pst_wapi)
#define WAPI_RX_IDX_ERR(pst_wapi)
#define WAPI_RX_NETBUF_LEN_ERR(pst_wapi)
#define WAPI_RX_IDX_UPDATE_ERR(pst_wapi)
#define WAPI_RX_KEY_EN_ERR(pst_wapi)
#define WAPI_RX_PN_ODD_ERR(pst_wapi, pn)
#define WAPI_RX_PN_REPLAY_ERR(pst_wapi, pn)
#define WAPI_RX_MEMALLOC_ERR(pst_wapi)
#define WAPI_RX_MIC_ERR(pst_wapi)
#define WAPI_RX_DECRYPT_OK(pst_wapi)
#define WAPI_TX_MEMALLOC_ERR(pst_wapi)
#define WAPI_TX_MIC_ERR(pst_wapi)
#define WAPI_TX_WAI_DROP_INC(pst_wapi)
#define WAPI_TX_ENCRYPT_OK(pst_wapi)


#endif /* #ifdef WAPI_DEBUG_MODE */

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
typedef struct
{
    oal_uint8   auc_framectl[2];        /* ?????? */
    oal_uint8   auc_adress1[OAL_MAC_ADDR_LEN];         /* ????1 */
    oal_uint8   auc_adress2[OAL_MAC_ADDR_LEN];         /* ????2 */
    oal_uint8   auc_seqctl[2];          /* ???????? */
    oal_uint8   auc_adress3[OAL_MAC_ADDR_LEN];         /* ????3 */
    oal_uint8   auc_adress4[OAL_MAC_ADDR_LEN];         /* ????4 */
}wapi_mic_hdr_stru;





/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/




/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32 hmac_wapi_deinit(hmac_wapi_stru *pst_wapi);
extern oal_uint32 hmac_wapi_init(hmac_wapi_stru *pst_wapi, oal_uint8 uc_pairwise);
//extern oal_uint32 hmac_wapi_en(hmac_vap_stru *pst_hmac_vap, oal_bool_enum_uint8 en_on);
#ifdef _PRE_WAPI_DEBUG
oal_uint32 hmac_wapi_display_info(mac_vap_stru *pst_mac_vap, oal_uint16 us_usr_idx);
#endif /* #ifdef _PRE_DEBUG_MODE */
extern oal_uint32 hmac_wapi_add_key(hmac_wapi_stru *pst_wapi, oal_uint8 uc_key_index, oal_uint8 *puc_key);
extern oal_uint32 hmac_wapi_reset_port(hmac_wapi_stru *pst_wapi);


#endif



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



