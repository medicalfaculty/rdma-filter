#include "rdma_bf.h"
#include "rdma_bbf.h"
#include "rdma_ohbbf.h"
#include "utils.h"
#include "rdma_cf.h"

#include <iostream>

#define RNIC_NAME "mlx4_0"
#define RNIC_PORT (1)
#define GID_INDEX (2)
#define TCP_PORT (18515)

#ifndef TOGGLE_LOCK_FREE
#define CLIENT_COUNT (2)
#else
#define CLIENT_COUNT (1)
#endif

#define INSERT_COUNT (1 << 26)

#define FALSE_POSITIVE_RATE ((double)1.0 / 512)
#define MUTEX_GRAN (1024)     // 每个锁占 8 字节，每个锁锁了 (粒度) 字节

#define BLOCK_SIZE (4)
#define MUTEX_GRAN_BLOCK (256)

#define BITS_PER_TAG_CF (16)
#define MUTEX_GRAN_BUCKET_CF (256)

int main(int argc, char **argv) {
    char cmd[16];
    std::cout << "==== Experiment Begin ====" << std::endl;
    std::cout << "Current Time: " << get_current_time_string() << std::endl;

// ----------------------------------------------------------------------------------------------

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

    // struct RdmaOHBBF_Srv srv;
    // RdmaOHBBF_Srv_init(&srv, INSERT_COUNT, FALSE_POSITIVE_RATE, CLIENT_COUNT, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX, MUTEX_GRAN_BLOCK, BLOCK_SIZE);

    // for (int i = 0; i < CLIENT_COUNT; i++) {
    //     reliable_recv(srv.sockfd_list[i], cmd, 5);
    //     std::cout << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
    // }

    // RdmaOHBBF_Srv_destroy(&srv);

// ----------------------------------------------------------------------------------------------
    // std::cerr << "[Server] Starting RdmaCF_Srv_init..." << std::endl;
    struct RdmaCF_Srv srv;
    RdmaCF_Srv_init(&srv, INSERT_COUNT, BITS_PER_TAG_CF, MUTEX_GRAN_BUCKET_CF, CLIENT_COUNT, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX);
    // std::cerr << "[Server] RdmaCF_Srv_init completed" << std::endl;
    
    // std::cerr << "[Server] Starting sync_server (init)..." << std::endl;
    sync_server(srv.list_sockfd);
    std::cout << "[Server] Initialization successfully!" << std::endl;
    // std::cerr << "[Server] Initialization successfully!" << std::endl;

    // std::cerr << "[Server] Starting sync_server (insert)..." << std::endl;
    sync_server(srv.list_sockfd);
    // std::cerr << "[Server] Insert phase sync completed" << std::endl;
    
    // std::cerr << "[Server] Starting sync_server (lookup existing)..." << std::endl;
    sync_server(srv.list_sockfd);
    // std::cerr << "[Server] Lookup existing phase sync completed" << std::endl;
    
    // std::cerr << "[Server] Starting sync_server (lookup non-existing)..." << std::endl;
    sync_server(srv.list_sockfd);
    // std::cerr << "[Server] Lookup non-existing phase sync completed" << std::endl;
    
    // std::cerr << "[Server] Starting sync_server (delete)..." << std::endl;
    sync_server(srv.list_sockfd);
    // std::cerr << "[Server] Delete phase sync completed" << std::endl;
    
    // std::cerr << "[Server] Waiting for EXIT messages from clients..." << std::endl;
    for (int i = 0; i < CLIENT_COUNT; i++) {
        // std::cerr << "[Server] Waiting for EXIT from client " << i + 1 << "/" << CLIENT_COUNT << std::endl;
        reliable_recv(srv.list_sockfd[i], cmd, 5);
        std::cout << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
        // std::cerr << "[Server] Received close message from client: " << i + 1 << "/" << CLIENT_COUNT << std::endl;
    }
    // std::cerr << "[Server] Destroying server..." << std::endl;
    RdmaCF_Srv_destroy(&srv);
    // std::cerr << "[Server] Server destroyed" << std::endl;

// ----------------------------------------------------------------------------------------------
    std::cout << "==== Experiment End ====" << std::endl;
    std::cout << "Current Time: " << get_current_time_string() << std::endl;
    // std::cerr << "[Server] ==== Experiment End ====" << std::endl;
    // std::cerr << "[Server] Current Time: " << get_current_time_string() << std::endl;
    return 0;
}
