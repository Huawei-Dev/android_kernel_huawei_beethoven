

#ifndef __PLAT_EXCEPTION_RST_H__
#define __PLAT_EXCEPTION_RST_H__
/*****************************************************************************
  1 Include other Head file
*****************************************************************************/
#include "plat_type.h"
#include "oal_net.h"
/*****************************************************************************
  2 Define macro
*****************************************************************************/
#define WAIT_BFGX_READ_STACK_TIME  (5000)   /* 等待bfgx读栈操作完成的时间，5000毫秒*/
#define BFGX_BEAT_TIME             (3)      /* bfgx心跳超时时间为3秒钟*/
#define WIFI_DUMP_BCPU_TIMEOUT     (1000)   /*wifi dump bcpu数据等待时间*/

#define PLAT_EXCEPTION_RESET_IDLE  (0)      /* plat 没有在处理异常*/
#define PLAT_EXCEPTION_RESET_BUSY  (1)      /* plat 正在处理异常*/

#define BFGX_NOT_RECV_BEAT_INFO    (0)      /* host没有收到心跳消息*/
#define BFGX_RECV_BEAT_INFO        (1)      /* host收到心跳消息*/

#define RF_TEMERATURE_NORMAL       (0)      /* rf温度正常*/
#define RF_TEMERATURE_OVERHEAT     (1)      /* rf温度过热*/

#define EXCEPTION_SUCCESS          (0)
#define EXCEPTION_FAIL             (1)

#define BEAT_TIMER_DELETE          (0)
#define BEAT_TIMER_RESET           (1)

#define BFGX_MEM_DUMP_BLOCK_COUNT  (3)       /* BFGX 上报的内存块个数 */
#define UART_READ_WIFI_MEM_TIMEOUT (10000)
#define UART_HALT_WCPU_TIMEOUT     (500)

#define BFGX_MAX_RESET_CMD_LEN     (5)
#define BT_RESET_CMD_LEN           (4)
#define FM_RESET_CMD_LEN           (1)
#define GNSS_RESET_CMD_LEN         (5)
#define IR_RESET_CMD_LEN           (4)
#define NFC_RESET_CMD_LEN          (4)

#define SDIO_STORE_BFGX_REGMEM     "readm_bfgx_sdio"
#define UART_STORE_BFGX_STACK      "readm_bfgx_uart"
#define UART_STORE_WIFI_MEM        "readm_wifi_uart"
#define SDIO_STORE_WIFI_MEM        "readm_wifi_sdio"

/*nfc buffer参数*/
#define OML_STATUS_ADD_LENGTH       13
#define NFCLOGLEN                  (4 * sizeof(unsigned int)+ OML_STATUS_ADD_LENGTH)
#define NFCLOGNUM                  (100)
#define OMLNFCDATABUFFLEN          (NFCLOGNUM * NFCLOGLEN)
#define NFC_SEND_LEN_LIMIT         (NFCLOGNUM*NFCLOGLEN)

#define MEMDUMP_ROTATE_QUEUE_MAX_LEN (10)
/*****************************************************************************
  3 STRUCT DEFINE
*****************************************************************************/
enum SUBSYSTEM_ENUM
{
	SUBSYS_WIFI  = 0,
	SUBSYS_BFGX  = 1,

	SUBSYS_BOTTOM,
};

enum WIFI_THREAD_ENUM
{ 
	THREAD_WIFI   = 0,

	WIFI_THREAD_BOTTOM,
};

enum BFGX_THREAD_ENUM
{ 
	THREAD_BT   = 0,
	THREAD_FM   = 1,
	THREAD_GNSS = 2,
	THREAD_IR   = 3,
	THREAD_NFC  = 4,

	BFGX_THREAD_BOTTOM,
};

enum EXCEPTION_TYPE_ENUM
{
	BEAT_HEART_TIMEOUT     = 0,  /*心跳超时*/
	LAST_WORD              = 1,  /*CPU挂死，临终遗言*/
	WATCHDOG_TIMEOUT       = 2,  /*看门狗超时*/
	TIMER_TIMEOUT          = 3,  /*定时器超时*/
	ARP_TIMEOUT            = 4,  /*arp verifier超时，有发无收*/
	//RF_OVERHEAT_EXCEPTION  = 5,  /*RF过热异常*/
    BFGX_POWER_ON_FAIL     = 6,
    BFGX_WAKEUP_FAIL       = 7,
    WIFI_POWER_ON_FAIL     = 8,
    WIFI_WAKEUP_FAIL       = 9,
    WIFI_DEVICE_PANIC      =10,/*wcpu arm exception*/
    SDIO_TRANS_FAIL        =11,/*sdio read or write failed*/

    SDIO_DUMP_BCPU         = 12,

    EXCEPTION_TYPE_BOTTOM,
};

enum UART_WIFI_MEM_DUMP
{
	WIFI_PUB_REG   = 0,
	WIFI_PRIV_REG  = 1,
	WIFI_MEM       = 2,

	UART_WIFI_MEM_DUMP_BOTTOM,
};

enum DUMP_CMD_TYPE
{
    CMD_READM_WIFI_SDIO = 0,
    CMD_READM_WIFI_UART = 1,
    CMD_READM_BFGX_UART = 2,
    CMD_READM_BFGX_SDIO = 3,

    CMD_DUMP_BUFF,
};

struct st_uart_dump_wifi_mem_info
{
    unsigned char *file_name;
    unsigned int size;
};

struct st_uart_dump_wifi_info
{
    unsigned int cmd;
    unsigned int total_size;
    unsigned int block_count;
    struct st_uart_dump_wifi_mem_info *block_info;
};

struct st_exception_mem_info
{
    unsigned char *exception_mem_addr;
    unsigned int total_size;
    unsigned int recved_size;
    unsigned char  *file_name;
};

struct st_wifi_dump_mem_info
{
    ulong  mem_addr;
    unsigned int size;
    unsigned char  *file_name;
};

struct st_wifi_dfr_callback
{
    void  (*wifi_recovery_complete)(void);
    void  (*notify_wifi_to_recovery)(void);
};

struct st_bfgx_reset_cmd
{
    unsigned int len;
    unsigned char  cmd[BFGX_MAX_RESET_CMD_LEN];
};

struct st_exception_info
{
    unsigned int   exception_reset_enable;
	unsigned int   subsys_type;
	unsigned int   thread_type;
	unsigned int   excetion_type;
	
	atomic_t bfgx_beat_flag;
	atomic_t is_reseting_device;
	
	struct workqueue_struct *plat_exception_rst_workqueue;
	struct work_struct plat_exception_rst_work;
	struct work_struct uart_store_wifi_mem_to_file_work;
	struct timer_list bfgx_beat_timer;

	struct completion wait_read_bfgx_stack;
	struct completion wait_sdio_d2h_dump_ack;

	struct ps_plat_s *ps_plat_d;
	struct st_wifi_dfr_callback *wifi_callback;

	spinlock_t exception_spin_lock;

    unsigned int wifi_exception_cnt;
    unsigned int bfgx_exception_cnt;

	/*下边的变量调试使用*/
	unsigned int debug_beat_flag;

    /*wifi打开BCPU for sdio mem dump，该功能仅在调试时使用，默认关闭*/
    unsigned int wifi_open_bcpu_enable;

    struct completion wait_uart_read_wifi_mem;
    struct completion wait_uart_halt_wcpu;

    /* wifi异常触发 */
    struct work_struct          wifi_excp_worker;
    struct work_struct          wifi_excp_recovery_worker;
    struct workqueue_struct    *wifi_exception_workqueue;
    unsigned int                      wifi_excp_type;

};

struct sdio_dump_bcpu_buff
{
    unsigned char *mem_addr;
    unsigned int data_limit;
    unsigned int data_len;
};
/*****************************************************************************
  4 EXTERN VARIABLE
*****************************************************************************/

/*****************************************************************************
  5 全局变量定义
*****************************************************************************/
extern struct sdio_dump_bcpu_buff st_bcpu_dump_buff;
extern oal_netbuf_stru*       st_bcpu_dump_netbuf;

/*****************************************************************************
  6 EXTERN FUNCTION
*****************************************************************************/
extern int mod_beat_timer(unsigned char on);
extern int is_bfgx_exception(void);
extern int get_exception_info_reference(struct st_exception_info **exception_data);
extern int plat_exception_handler(unsigned int subsys_type, unsigned int thread_type, unsigned int exception_type);
extern int plat_bfgx_exception_rst_register(struct ps_plat_s *data);
extern int plat_wifi_exception_rst_register(void *data);
extern int wifi_exception_mem_dump(struct st_wifi_dump_mem_info *pst_mem_dump_info, unsigned int count, int excep_type);
extern int wifi_open_bcpu_set(unsigned char enable);
extern int prepare_to_recv_bfgx_stack(unsigned int len);
extern int bfgx_recv_dev_mem(unsigned char *buf_ptr, unsigned short count);
extern void store_wifi_mem_to_file(void);
extern int uart_recv_wifi_mem(unsigned char *buf_ptr, unsigned short count);
extern int uart_halt_wcpu(void);
extern int uart_read_wifi_mem(unsigned int which_mem);
extern int debug_uart_read_wifi_mem(unsigned int ul_lock);
extern int plat_exception_reset_init(void);
extern int plat_exception_reset_exit(void);
extern int wifi_exception_work_submit(unsigned int wifi_excp_type);
extern int plat_power_fail_exception_info_set(unsigned int subsys_type, unsigned int thread_type, unsigned int exception_type);
extern void plat_power_fail_process_done(void);
extern int bfgx_subsystem_reset(void);
extern int bfgx_system_reset(void);
extern int debug_sdio_read_bfgx_reg_and_mem(unsigned int which_mem);
extern int exception_bcpu_dump_recv(unsigned char* str,oal_netbuf_stru* netbuf);
#endif

