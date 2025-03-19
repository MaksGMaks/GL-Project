#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <net/if.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct sockaddr_in getInterface(const std::string &intrName);
struct sockaddr_in getDestination(const std::string &domain);

struct icmp formICMPRequest(int ttl);

unsigned short computeChecksum(const void *buffer, int length);