#include <iostream>
#include "interface.hpp"

int main(int argc, char* argv[]) {
    Arguments allArgs;
    try {
        allArgs = analyzeArgs(argc, argv);
        std::cout << "Max hops: " << allArgs.max_hops << std::endl;
        std::cout << "Interface: " << allArgs.interface << std::endl;
        std::cout << "Domain: " << allArgs.domain << std::endl;
    } catch(std::exception) {
        showHelpPage();
    }
    return 0;
}

