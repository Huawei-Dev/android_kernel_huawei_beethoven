#pragma once

#define RIL_NETLINK_INIT 1
#define RIL_NETLINK_EXIT 0
#define MAX_IF_NAME 16

enum {
    NETLINK_TCP_REG = 0,
    NETLINK_TCP_RST_MSG,
    NETLINK_KNL_RLT_MSG,
    NETLINK_TCP_UNREG
};

struct Ril2KnlMsg
{
    char if_name[MAX_IF_NAME];
};

void notify_chr_thread_to_send_msg(unsigned int dst_addr, unsigned int src_addr);
int chr_notify_event(int event, int pid, unsigned int src_addr, struct http_return *prtn);
extern unsigned int g_user_space_pid;

