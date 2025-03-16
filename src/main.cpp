#include <iostream>
#include "interface.hpp"
#include "socket.hpp"

int main(int argc, char* argv[]) {
    Arguments allArgs;
    try {
        allArgs = analyzeArgs(argc, argv);
    } catch(std::exception) {
        showHelpPage();
        return -1;
    }
    
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock < 0) {
        std::cerr << "Socket creating failed. Please try again." << std::endl;
        return -1;
    }
    if(allArgs.interface != "") {
        struct sockaddr_in interfaceAddress = getInterface(allArgs.interface);
        if(interfaceAddress.sin_addr.s_addr == 0) {
            return -1;
        }
        
        if(bind(sock, (struct sockaddr*)&interfaceAddress, sizeof(interfaceAddress)) < 0) {
            std::cerr << "Bind failed." << std::endl;
            return -1;
        }
    }

    struct sockaddr_in destAddress = getDestination(allArgs.domain);
    if(destAddress.sin_addr.s_addr == 0) {
        return -1;
    }

    char receiveBuffer[1024];

    for(int ttl = 1; ttl <= allArgs.max_hops; ttl++) {
        setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

        struct icmp icmpPacket = formICMPRequest(ttl);

        if (sendto(sock, &icmpPacket, sizeof(icmpPacket), 0, 
                   (struct sockaddr *)&destAddress, sizeof(destAddress)) < 0) {
            perror("Failed to send ICMP request");
            continue;
        }

        struct sockaddr_in responseAddress;
        socklen_t addressLength = sizeof(responseAddress);

        if (recvfrom(sock, receiveBuffer, sizeof(receiveBuffer), 0, 
                     (struct sockaddr *)&responseAddress, &addressLength) < 0) {
            std::cout << ttl << ": * (timeout)" << std::endl;
            continue;
        }

        char routerIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &responseAddress.sin_addr, routerIp, sizeof(routerIp));
        std::cout << ttl << ": " << routerIp << std::endl;

        if (responseAddress.sin_addr.s_addr == destAddress.sin_addr.s_addr) {
            break;
        }
    }

    close(sock);
    
    return 0;
}