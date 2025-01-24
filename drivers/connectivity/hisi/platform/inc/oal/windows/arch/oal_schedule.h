

#ifndef __OAL_WINDOWS_SCHEDULE_H__
#define __OAL_WINDOWS_SCHEDULE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include <windows.h>
#include <time.h>
#include "oal_util.h"

/*****************************************************************************
  2 ??????
*****************************************************************************/
/* typedef CRITICAL_SECTION    oal_spin_lock_stru; zouhongliang ????windows????linux?????????? */
typedef struct
{
    oal_uint8       auc_resv[4];
}oal_spin_lock_stru;

typedef LONG volatile       oal_atomic;

typedef oal_uint32          (*oal_irqlocked_func)(oal_void *);

typedef CRITICAL_SECTION    oal_rwlock_stru;

typedef oal_void            (*oal_defer_func)(oal_uint);

typedef oal_void         oal_wait_queue_stru;

typedef struct
{
    oal_uint32      resv1[4];
    oal_uint32      resv2[4];
}oal_wait_queue_head_stru;


/* Tasklet is scheduled for execution */
#define TASKLET_STATE_SCHED     (0)

/* tasklet???? */
#define OAL_DECLARE_TASK(_name, _func, _p_data) \
    struct oal_tasklet_stru _name = {NULL, 0, _func, _p_data, 0, NULL, THREAD_PRIORITY_ABOVE_NORMAL}


OAL_STATIC OAL_INLINE oal_int32  oal_wait_event_interruptible_timeout(oal_wait_queue_head_stru st_wq, oal_uint8 uc_bool, oal_int i_time)
{
    return 50;
}

OAL_STATIC OAL_INLINE oal_int32  oal_wait_event_interruptible(oal_wait_queue_head_stru st_wq, oal_uint8 uc_bool)
{
    return 10;
}

OAL_STATIC OAL_INLINE oal_int32  oal_wait_event_timeout(oal_wait_queue_head_stru st_wq, oal_uint8 uc_bool, oal_int i_time)
{
    return 50;
}


#define OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(_st_wq, _condition, _timeout)      oal_wait_event_interruptible_timeout(_st_wq, _condition, _timeout)

#define OAL_WAIT_EVENT_TIMEOUT(_st_wq, _condition, _timeout)      oal_wait_event_timeout(_st_wq, _condition, _timeout)

#define OAL_WAIT_EVENT_INTERRUPTIBLE(_st_wq, _condition)      oal_wait_event_interruptible(_st_wq, _condition)


#define OAL_WAIT_QUEUE_WAKE_UP_INTERRUPT(_pst_wq)   OAL_REFERENCE(_pst_wq)

#define OAL_WAIT_QUEUE_WAKE_UP(_pst_wq)

#define OAL_INTERRUPTIBLE_SLEEP_ON(_pst_wq)

#define OAL_WAIT_QUEUE_INIT_HEAD(_pst_wq)

/* ???????????????? */
#define OAL_TIME_GET_STAMP_MS() clock()
#define OAL_TIME_GET_HIGH_PRECISION_MS() clock()

#define OAL_ENABLE_CYCLE_COUNT()
#define OAL_DISABLE_CYCLE_COUNT()
#define OAL_GET_CYCLE_COUNT()

/* ?????????????????????????? */
#define OAL_TIME_CALC_RUNTIME(_ul_start, _ul_end)   ((OAL_TIME_US_MAX_LEN) - (_ul_start) + (_ul_end))

#define OAL_TIME_JIFFY  15

#define OAL_TIME_HZ     (1000)


#define OAL_MSECS_TO_JIFFIES(_msecs)    (_msecs)

#define OAL_JIFFIES_TO_MSECS(_jiffies)  (_jiffies)

#define OAL_INIT_COMPLETION(_my_completion) {}

#define OAL_COMPLETE(_my_completion)        {}

OAL_STATIC OAL_INLINE oal_int32  oal_in_interrupt()
{
    return 0;
}

OAL_STATIC OAL_INLINE oal_int32 oal_in_atomic()
{
    return 0;
}


typedef void (*oal_timer_func)(oal_uint);

typedef struct
{
    oal_uint32                  ul_delay;
    oal_timer_func              p_timer_func;
    oal_uint                    ui_arg;
    oal_uint32                  ul_timer_id;
}oal_timer_list_stru;

/* ???????? */
#define oal_module_init(_module_name)

#define oal_module_license(_license_name)

/* ???????? */
#define oal_module_exit(_module_name)

#define oal_module_param(_symbol, _type, _name)

/* ???????????? */
#define oal_module_symbol(_symbol)
#define OAL_MODULE_DEVICE_TABLE(_type, _name)

#define oal_smp_call_function_single(core, task, info, wait)
#define IS_ERR(x) (0)
#define IS_ERR_OR_NULL(x) (0)

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

typedef int (read_proc_t)(oal_int8 *, oal_int8 **, oal_int,
                                       oal_int32, oal_int32 *, oal_void *);
typedef int (write_proc_t)(oal_file_stru *, const oal_int8 *,
                                       oal_uint32, oal_void *);
typedef struct
{
    oal_uint32               ul_done; //??????????????????????????????????????????????????????0

    oal_wait_queue_head_stru st_wait;

}oal_completion;

#define DECLARE_COMPLETION(work) oal_completion work

typedef struct
{
    oal_int i_sec;
    oal_int i_usec;
}oal_time_us_stru;

typedef union{
    oal_int i_sec;
    oal_int i_usec;
}oal_time_t_stru;

/* ?????????? */
typedef struct
{
    LPSECURITY_ATTRIBUTES       lp_thread_attributes;       /* ????SECURITY_ATTRIBUTES???????????????????????? */
    oal_uint32                  ul_stack_size;              /* ???????????????????? */
    oal_defer_func              p_startaddress_func;        /* ?????????????????????????????? */
    oal_uint                    ui_parameter;               /* ?????????????????????? */
    oal_uint32                  ul_creationflags;           /* ?????????????????????????? */
    HANDLE                      threadhead;                 /* ???????????? */
    oal_int32                   l_npriority;                /* ?????????? */
    oal_int                     i_state;                    /* ???????????? */
}oal_tasklet_stru;

typedef struct
{
    oal_void            *data;
    oal_uint16           nlink;
    oal_uint8            resv[2];

    read_proc_t         *read_proc;
    write_proc_t        *write_proc;

}oal_proc_dir_entry_stru;

typedef struct
{
    oal_atomic            count;
    oal_spin_lock_stru    wait_lock;
}oal_mutex_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO

#define ERESTARTSYS	512

#define SCHED_NORMAL		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_BATCH		3
/* SCHED_ISO: reserved but not implemented yet */
#define SCHED_IDLE		5
struct task_struct {
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	void *stack;/*add if you use*/
};

struct semaphore {
	unsigned int		count;
};

struct sched_param {
	int sched_priority;
};

typedef struct _oal_task_lock_stru_
{
    oal_wait_queue_head_stru	wq;
    struct task_struct	*claimer;	/* task that has host claimed */
    oal_ulong			 claim_addr;
    oal_uint32           claimed;
    oal_int32			 claim_cnt;
}oal_task_lock_stru;

OAL_STATIC OAL_INLINE static char* oal_get_current_task_name()
{
	return "win32";
}


OAL_STATIC OAL_INLINE int allow_signal(int sig)
{
		return 0;
}



OAL_INLINE oal_int32  oal_atomic_read(oal_atomic *p_vector)
{
    return (oal_int32)(*p_vector);
}


OAL_INLINE oal_void  oal_atomic_set(oal_atomic *p_vector, oal_int32 l_val)
{
    *p_vector = (LONG)l_val;
}


OAL_INLINE oal_void oal_atomic_dec(oal_atomic *p_vector)
{
    InterlockedDecrement(p_vector);
}


OAL_INLINE oal_void  oal_atomic_inc(oal_atomic *p_vector)
{
    InterlockedIncrement(p_vector);
}


OAL_STATIC OAL_INLINE oal_void  oal_atomic_inc_and_test(oal_atomic *p_vector)
{
    /*InterlockedIncrementtest(p_vector);*/
}


OAL_STATIC OAL_INLINE oal_int32  oal_atomic_dec_and_test(oal_atomic *p_vector)
{
    /*InterlockedDecrementandtest(p_vector);*/
    return 0;
}


OAL_INLINE oal_void  oal_spin_lock_init(oal_spin_lock_stru *pst_lock)
{
    /* InitializeCriticalSection(pst_lock);*/
}


OAL_INLINE oal_void  oal_spin_lock(oal_spin_lock_stru *pst_lock)
{
    /* EnterCriticalSection(pst_lock);*/
}


OAL_INLINE oal_void  oal_spin_unlock(oal_spin_lock_stru *pst_lock)
{
    /* LeaveCriticalSection(pst_lock); */
}


OAL_INLINE oal_void oal_spin_lock_bh(oal_spin_lock_stru *pst_lock)
{
    /* EnterCriticalSection(pst_lock); */
}


OAL_INLINE oal_void oal_spin_unlock_bh(oal_spin_lock_stru *pst_lock)
{
    /* LeaveCriticalSection(pst_lock); */
}


OAL_INLINE oal_void  oal_spin_lock_irq_save(oal_spin_lock_stru *pst_lock, oal_uint *pui_flags)
{
    oal_irq_save(pui_flags, OAL_5115IRQ_OSLIS);

    oal_spin_lock(pst_lock);
}


OAL_INLINE oal_void  oal_spin_unlock_irq_restore(oal_spin_lock_stru *pst_lock, oal_uint *pui_flags)
{
    oal_spin_unlock(pst_lock);

    oal_irq_restore(pui_flags, OAL_5115IRQ_OSLIS);
}


OAL_INLINE oal_uint32  oal_spin_lock_irq_exec(oal_spin_lock_stru *pst_lock, oal_irqlocked_func p_irq_locked_func, oal_void *p_arg, oal_uint *pui_flags)
{
    oal_uint32  ul_Rslt;

    oal_spin_lock_irq_save(pst_lock, pui_flags);

    ul_Rslt = p_irq_locked_func(p_arg);

    oal_spin_unlock_irq_restore(pst_lock, pui_flags);

    return ul_Rslt;
}


OAL_INLINE oal_void  oal_rw_lock_init(oal_rwlock_stru *pst_lock)
{
    /* InitializeCriticalSection(pst_lock); */
}


OAL_INLINE oal_void  oal_rw_lock_read_lock(oal_rwlock_stru *pst_lock)
{
    /* EnterCriticalSection(pst_lock); */
}


OAL_INLINE oal_void  oal_rw_lock_read_unlock(oal_rwlock_stru *pst_lock)
{
    /*LeaveCriticalSection(pst_lock);*/
}


OAL_INLINE oal_void  oal_rw_lock_write_lock(oal_rwlock_stru *pst_lock)
{
    /*EnterCriticalSection(pst_lock);*/
}


OAL_INLINE oal_void  oal_rw_lock_write_unlock(oal_rwlock_stru *pst_lock)
{
    /*LeaveCriticalSection(pst_lock);*/
}



#if 0
OAL_STATIC DWORD WINAPI  oal_thread_process(LPVOID p_parameter)
{
    oal_tasklet_stru   *pst_task;
    oal_defer_func      p_func;
    oal_uint            ui_args;

    pst_task    = (oal_tasklet_stru *)p_parameter;
    p_func      = pst_task->p_startaddress_func;
    ui_args     = pst_task->ui_parameter;

    p_func(ui_args);

    oal_bit_atomic_clear(TASKLET_STATE_SCHED, &(pst_task->i_state));

    return 0;
}
#endif



OAL_INLINE oal_void  oal_task_init(oal_tasklet_stru *pst_task, oal_defer_func p_func, oal_uint ui_args)
{
    pst_task->lp_thread_attributes      = NULL;                                 /* ???????????????????? */
    pst_task->ul_stack_size             = 0;                                    /* ?????????????????????????????????????????? */
    pst_task->p_startaddress_func       = p_func;                               /* ?????????????????????????? */
    pst_task->ui_parameter              = ui_args;                              /* ???????????? */
    pst_task->ul_creationflags          = 0;                                    /* ?????????????????????? */
    pst_task->l_npriority               = THREAD_PRIORITY_ABOVE_NORMAL;         /* ???????????????? */
}


OAL_STATIC OAL_INLINE oal_void oal_task_kill(oal_tasklet_stru *pst_task)
{
}


OAL_INLINE oal_void  oal_task_sched(oal_tasklet_stru *pst_task)
{

}


OAL_STATIC OAL_INLINE oal_uint oal_task_is_scheduled(oal_tasklet_stru *pst_task)
{
    return (pst_task->i_state & (1 << (TASKLET_STATE_SCHED + 1)));
}


OAL_INLINE oal_void  oal_time_get_stamp_us(oal_time_us_stru *pst_usec)
{
    pst_usec->i_sec     = 0;

    pst_usec->i_usec    = 0;
}


OAL_STATIC OAL_INLINE oal_time_t_stru oal_ktime_get(oal_void)
{
    oal_time_t_stru st_time = {0};
    return st_time;
}


OAL_STATIC OAL_INLINE oal_time_t_stru oal_ktime_sub(const oal_time_t_stru lhs, const oal_time_t_stru rhs)
{
    oal_time_t_stru st_time = {0};
    return st_time;
}

extern oal_void  oal_timer_init(oal_timer_list_stru *pst_timer, oal_uint32 ul_delay, oal_timer_func p_func, oal_uint ui_arg);
extern oal_int32  oal_timer_delete(oal_timer_list_stru *pst_timer);
extern oal_int32  oal_timer_delete_sync(oal_timer_list_stru *pst_timer);
extern oal_int32  oal_timer_start(oal_timer_list_stru *pst_timer, oal_uint ui_expires);
extern oal_void  oal_timer_add(oal_timer_list_stru *pst_timer);


OAL_STATIC OAL_INLINE oal_void  oal_timer_start_on(oal_timer_list_stru *pst_timer, oal_uint ui_delay, oal_int32 cpu)
{
    OAL_REFERENCE(pst_timer);
    OAL_REFERENCE(ui_delay);
    OAL_REFERENCE(cpu);
}


OAL_INLINE oal_uint32  oal_copy_from_user(oal_void *p_to, const oal_void *p_from, oal_uint32 ul_size)
{
    oal_memcopy(p_to, p_from, ul_size);

    return 0;
}


OAL_INLINE oal_uint32  oal_copy_to_user(oal_void *p_to, const oal_void *p_from, oal_uint32 ul_size)
{
    oal_memcopy(p_to, p_from, ul_size);

    return 0;
}


OAL_INLINE oal_proc_dir_entry_stru* oal_create_proc_entry(const oal_int8 *pc_name, oal_uint16 us_mode, oal_proc_dir_entry_stru *pst_parent)
{
    oal_proc_dir_entry_stru     *pst_proc_entry;

    pst_proc_entry = (oal_proc_dir_entry_stru *)oal_memalloc(sizeof(oal_proc_dir_entry_stru));

    return pst_proc_entry;
}


OAL_STATIC OAL_INLINE void oal_remove_proc_entry(const oal_int8 *pc_name, oal_proc_dir_entry_stru *pst_parent)
{
    oal_free(pst_parent);
    return;
}


OAL_STATIC OAL_INLINE oal_uint32 oal_time_is_before(oal_uint ui_time)
{
    return 1;
}


OAL_STATIC OAL_INLINE oal_uint32 oal_time_after(oal_uint32 ul_time_a, oal_uint32 ul_time_b)
{
    return 1;
}


OAL_STATIC OAL_INLINE oal_uint32  oal_wait_for_completion_timeout(oal_completion *pst_completion, oal_uint32 ul_timeout)
{
    return 1;
}


OAL_STATIC OAL_INLINE oal_void oal_smp_task_lock(oal_task_lock_stru* pst_lock)
{
    OAL_REFERENCE(pst_lock);
}


OAL_STATIC OAL_INLINE oal_void oal_smp_task_unlock(oal_task_lock_stru* pst_lock)
{
    OAL_REFERENCE(pst_lock);
}


OAL_STATIC OAL_INLINE oal_void oal_smp_task_lock_init(oal_task_lock_stru* pst_lock)
{
    OAL_REFERENCE(pst_lock);
}
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_schedule.h */
