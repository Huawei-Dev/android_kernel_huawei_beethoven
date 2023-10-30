

#ifndef __BFGX_LOW_POWER_H__
#define __BFGX_LOW_POWER_H__
/*****************************************************************************
  1 Include other Head file
*****************************************************************************/
#include "plat_type.h"
#include "hw_bfg_ps.h"
/*****************************************************************************
  2 Define macro
*****************************************************************************/

/*****************************************************************************
  3 STRUCT DEFINE
*****************************************************************************/

/*****************************************************************************
  4 EXTERN VARIABLE
*****************************************************************************/

/*****************************************************************************
  5 EXTERN FUNCTION
*****************************************************************************/
extern unsigned char check_bfg_not_booting(struct ps_core_s *ps_core_d);
extern void host_allow_bfg_sleep(struct ps_core_s *ps_core_d);
extern void bfg_timer_expire(unsigned long data);
extern unsigned char bfgx_wakeup_device(struct ps_core_s *ps_core_d, unsigned char type);
#endif

