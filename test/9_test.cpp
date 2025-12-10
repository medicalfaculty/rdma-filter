#include <iostream>
#include <unordered_set>
#include <chrono>
#include <unistd.h>

int main(int argc, char **argv) {

    for (int i = 0; ; i++) {
        std::cout << "Heartbeat " << i << std::endl;
        sleep(2);
    }

    return 0;
}   