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

    struct timeval timeout;
    timeout.tv_sec = allArgs.timeout;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char receiveBuffer[1024];
    int currentTTL, timeoutCount = 0;
    struct timespec startTime{}, endTime{};

    for(int ttl = 1; ttl <= allArgs.max_hops; ttl++) {
        setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        struct sockaddr_in responseAddress;
        socklen_t addressLength = sizeof(responseAddress);
        struct icmp icmpPacket = formICMPRequest(ttl);

        clock_gettime(CLOCK_MONOTONIC, &startTime);
        if (sendto(sock, &icmpPacket, sizeof(icmpPacket), 0 
                   , (struct sockaddr *)&destAddress, sizeof(destAddress)) < 0) {
            perror("Failed to send ICMP request");
            continue;
        }

        if (recvfrom(sock, receiveBuffer, sizeof(receiveBuffer), 0 
                     , (struct sockaddr *)&responseAddress, &addressLength) < 0) {
            std::cout << "\n*\t*\t*\t\n" << std::endl;
            ttl--;
            timeoutCount++;
            if(currentTTL == ttl && timeoutCount > 4) {
                std::cout << "Request timeout, can't reach destination" << std::endl;
                ttl = allArgs.max_hops;
            }
            continue;
        }

        clock_gettime(CLOCK_MONOTONIC, &endTime);
        double timeTaken = getTimeDiff(startTime, endTime) / 1000.0;

        currentTTL = ttl;
        timeoutCount = 0;

        char routerIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &responseAddress.sin_addr, routerIp, sizeof(routerIp));

        std::cout << ttl << "\t| " << routerIp << ((std::string(routerIp).length() < 14) ? "\t\t| " : "\t| ") << timeTaken << "ms";
        if(allArgs.showFQDN) {
            char host[NI_MAXHOST];
        
            int domainStatus = getnameinfo((struct sockaddr *)&responseAddress, sizeof(responseAddress)
                                     , host, sizeof(host), nullptr, 0, NI_NAMEREQD);
            std::string domain = (domainStatus < 0) ? "" : std::string(host);
            std::cout << "\t| " << domain;
        }
        std::cout << std::endl;

        if (responseAddress.sin_addr.s_addr == destAddress.sin_addr.s_addr) {
            break;
        }
    }

    close(sock);
    
    return 0;
}