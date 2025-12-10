#include "utils.h"
#include "rdma_common.h"

#include <iostream>
#include <unordered_set>
#include <chrono>

#include <infiniband/verbs.h>
#include <arpa/inet.h>

#define INSERT_COUNT        (1 << 26)
#define FALSE_POSITIVE_RATE (double)(1.0 / 512)
#define BLOCK_SIZE          (4)
#define LOOKUP_COUNT_MAX    (1 << 26)
#define REAL_INSERT_COUNT   (1 << 26)
#define SINGLE_ROUND_COUNT  (REAL_INSERT_COUNT / 20)

void print_gids(struct ibv_context *ctx, int port) {
    struct ibv_device_attr attr;
    ibv_query_device(ctx, &attr);

    union ibv_gid gid;
    char gid_str[INET6_ADDRSTRLEN];

    for (int i = 0; i < 8; i++) {
        if (ibv_query_gid(ctx, port, i, &gid) != 0) break;
        inet_ntop(AF_INET6, &gid, gid_str, sizeof(gid_str));
        printf("Port %d GID[%d] = %s\n", port, i, gid_str);
    }
}

int main(int argc, char **argv) {

    int num_devices = 0;
    ibv_device **dev_list = ibv_get_device_list(&num_devices);
    std::cout << "Number of devices: " << num_devices << std::endl;
    for (int i = 0; dev_list[i] != nullptr; ++i) {
        ibv_device *dev = dev_list[i];
        std::cout << "Device " << i << ":\n";
        std::cout << "  name       : " << (dev->name ? dev->name : "N/A") << "\n";
        std::cout << "  dev_name   : " << (dev->dev_name ? dev->dev_name : "N/A") << "\n";
        std::cout << "  node_type  : ";
        switch (dev->node_type) {
            case IBV_NODE_CA:      std::cout << "Channel Adapter (CA)"; break;
            case IBV_NODE_SWITCH:  std::cout << "Switch"; break;
            case IBV_NODE_ROUTER:  std::cout << "Router"; break;
            case IBV_NODE_RNIC:    std::cout << "rNIC"; break;
            default:               std::cout << "Unknown (" << static_cast<int>(dev->node_type) << ")";
        }
        std::cout << "\n";
        std::cout << "  transport  : " << (dev->transport_type == IBV_TRANSPORT_IB ? "InfiniBand" :
            dev->transport_type == IBV_TRANSPORT_IWARP ? "iWARP" :
            dev->transport_type == IBV_TRANSPORT_USNIC ? "usNIC" :
            dev->transport_type == IBV_TRANSPORT_USNIC_UDP ? "usNIC UDP" :
            "Unknown") << "\n";
        std::cout << "------------------------\n";
    }

    ibv_context *ctx = ibv_open_device(dev_list[0]);
    print_gids(ctx, 1);
    print_gids(ctx, 2);

    ibv_close_device(ctx);
    ibv_free_device_list(dev_list);
    return 0;




}