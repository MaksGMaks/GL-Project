#include "socket.hpp"

struct sockaddr_in getInterface(const std::string &intrName) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Socket creation failed");
        return {};
    }

    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));
    std::strncpy(ifr.ifr_name, intrName.c_str(), IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
        std::cerr << "Interface '" << intrName << "' not found. Error: " << strerror(errno) << std::endl;
        close(fd);
        return {};
    }

    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        std::cerr << "Failed to get IP address for interface '" << intrName << "'. Error: " << strerror(errno) << std::endl;
        close(fd);
        return {};
    }

    close(fd);

    struct sockaddr_in interfaceAddress = {};
    std::memcpy(&interfaceAddress, &ifr.ifr_addr, sizeof(struct sockaddr_in));
    interfaceAddress.sin_family = AF_INET;
    interfaceAddress.sin_port = 0;
    return interfaceAddress;
}

struct sockaddr_in getDestination(const std::string &domain) {
    struct addrinfo hints, *res;
    char ip_str[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(domain.c_str(), NULL, &hints, &res);
    if (status != 0) {
        std::cerr << "Getaddrinfo error: " << gai_strerror(status) << std::endl;
        return {};
    }

    struct sockaddr_in destAddress = {};
    if (res) {
        std::memcpy(&destAddress, res->ai_addr, sizeof(struct sockaddr_in));
        destAddress.sin_family = AF_INET;
    }
    freeaddrinfo(res);

    return destAddress;
}

struct icmp formICMPRequest(int ttl) {
    struct icmp icmpPacket;
    memset(&icmpPacket, 0, sizeof(icmpPacket));

    icmpPacket.icmp_type = ICMP_ECHO;
    icmpPacket.icmp_code = 0;
    icmpPacket.icmp_id = getpid();
    icmpPacket.icmp_seq = ttl;
    icmpPacket.icmp_cksum = 0;

    icmpPacket.icmp_cksum = computeChecksum(&icmpPacket, sizeof(icmpPacket));

    return icmpPacket;
}

unsigned short computeChecksum(const void *buffer, int length) {
    unsigned int sum = 0;
    const unsigned short *data = static_cast<const unsigned short *>(buffer);

    while (length > 1) {
        sum += *data++;
        length -= 2;
    }

    if (length == 1) {
        sum += *(const unsigned char *)data;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return static_cast<unsigned short>(~sum);
}