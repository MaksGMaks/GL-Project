#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#define MAX_HOPS 10 

struct Arguments {
    uint max_hops = MAX_HOPS;
    std::string interface{};
    std::string domain{};
};

Arguments analyzeArgs(const int &argc, char* argv[]);
void showHelpPage();

