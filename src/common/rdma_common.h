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

ibv_qp *create_rc_qp(ibv_pd *pd, ibv_cq *cq);

int modify_init_qp(ibv_qp *qp, uint8_t port);

int modify_rtr_qp(ibv_qp* qp, uint32_t remote_qpn, uint32_t remote_psn, const union ibv_gid& remote_gid, uint8_t sgid_index, uint8_t port);

int modify_rts_qp(ibv_qp* qp, uint32_t sq_psn);

ibv_sge* create_sge(ibv_mr* mr);

int rdma_one_side(ibv_qp *qp, int wr_id, ibv_sge *sge, uint64_t remote_addr, uint32_t rkey, ibv_wr_opcode opcode);

int check_cq(ibv_cq *cq, int wr_id);

int rdma_atomic_cas(ibv_qp *qp, int wr_id, ibv_sge *sge, ibv_cq *cq, uint64_t remote_addr, uint32_t rkey, uint64_t compare_add, uint64_t swap);

ibv_context *open_rdma_ctx(const char* name_dev);

rdma_conn_info *create_local_info(ibv_context *ctx, uint8_t port, uint8_t gid_index);

#endif /* __RDMA_COMMON_H__ */