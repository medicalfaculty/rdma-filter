#include "rdma_bf.h"
#include "rdma_bbf.h"
#include "rdma_ohbbf.h"
#include "utils.h"

#include <iostream>

#define RNIC_NAME "mlx4_0"
#define RNIC_PORT (2)
#define GID_INDEX (2)
#define TCP_PORT (18515)

#define INSERT_COUNT (1 << 26)
#define FALSE_POSITIVE_RATE ((double)1.0 / 512)
#define CLIENT_COUNT (2)
#define MUTEX_GRAN (1024)     // 每个锁占 8 字节，每个锁锁了 (粒度) 字节

#define BLOCK_SIZE (4)
#define MUTEX_GRAN_BLOCK (256)

int main(int argc, char **argv) {
    char cmd[16];

    // struct RdmaBF_Srv srv;
    // RdmaBF_Srv_init(&srv, INSERT_COUNT, FALSE_POSITIVE_RATE, CLIENT_COUNT, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX, MUTEX_GRAN);

    // for (int i = 0; i < CLIENT_COUNT; i++) {
    //     reliable_recv(srv.sockfd_list[i], cmd, 5);
    //     std::cout << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
    // }

    // RdmaBF_Srv_destroy(&srv);

// ----------------------------------------------------------------------------------------------

            // struct RdmaBBF_Srv srv;
            // RdmaBBF_Srv_init(&srv, INSERT_COUNT, FALSE_POSITIVE_RATE, CLIENT_COUNT, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX, MUTEX_GRAN_BLOCK, BLOCK_SIZE);

            // for (int i = 0; i < CLIENT_COUNT; i++) {
            //     reliable_recv(srv.sockfd_list[i], cmd, 5);
            //     std::cout << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
            // }

            // RdmaBBF_Srv_destroy(&srv);

// ----------------------------------------------------------------------------------------------

    struct RdmaOHBBF_Srv srv;
    RdmaOHBBF_Srv_init(&srv, INSERT_COUNT, FALSE_POSITIVE_RATE, CLIENT_COUNT, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX, MUTEX_GRAN_BLOCK, BLOCK_SIZE);

    for (int i = 0; i < CLIENT_COUNT; i++) {
        reliable_recv(srv.sockfd_list[i], cmd, 5);
        std::cout << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
    }

    RdmaOHBBF_Srv_destroy(&srv);

    return 0;
}