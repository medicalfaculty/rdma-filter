#ifndef __RDMA_COMMON_H__
#define __RDMA_COMMON_H__

#include <stdint.h>
#include <stdlib.h>

#include <infiniband/verbs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

struct rdma_conn_info {
    uint32_t qp_num;
    uint32_t psn;           // initial packet sequence number
    union ibv_gid gid;
    uint64_t remote_addr;   // server端位图地址
    uint32_t rkey;          // memory region key
    uint64_t mutex_addr;    // server端mutex地址
    uint32_t mutex_rkey;    // mutex memory region key
} __attribute__((packed));

ibv_qp_init_attr DEFAULT_QP_INIT_ATTR = {
    .send_cq = NULL,
    .recv_cq = NULL,
    .qp_type = IBV_QPT_RC,
    .cap.max_send_wr = 10,
    .cap.max_recv_wr = 10,
    .cap.max_send_sge = 1,
    .cap.max_recv_sge = 1,
};

ibv_qp_attr DEFAULT_QP_ATTR = {
    .qp_state = IBV_QPS_INIT,
    .port_num = 1,
    .pkey_index = 0,
    .qp_access_flags = IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ,
};

ibv_qp_attr DEFAULT_RTR_QP_ATTR = {
    .qp_state = IBV_QPS_RTR,
    .path_mtu = IBV_MTU_1024,
    .dest_qp_num = 0,
    .rq_psn = 0,
    .max_dest_rd_atomic = 1,
    .min_rnr_timer = 12,
};

ibv_qp_attr DEFAULT_RTS_QP_ATTR = {
    .qp_state = IBV_QPS_RTS,
    .timeout = 14,
    .retry_cnt = 7,
    .rnr_retry = 7,
    .sq_psn = 0,
    .max_rd_atomic = 1,
};

#endif /* __RDMA_COMMON_H__ */