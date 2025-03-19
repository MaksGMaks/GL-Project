#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#define MAX_HOPS 10 
#define MAX_TIMEOUT 3

struct Arguments {
    uint16_t max_hops = MAX_HOPS;
    std::string interface{};
    std::string domain{};
    uint8_t timeout = MAX_TIMEOUT;
    bool showFQDN = false;
};

Arguments analyzeArgs(const int &argc, char* argv[]);
void showHelpPage();

double getTimeDiff(const struct timespec &start, const struct timespec &end);