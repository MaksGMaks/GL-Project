#include "interface.hpp"

Arguments analyzeArgs(const int &argc, char* argv[]) {
    Arguments args;
    if(argc > 2) {
        args.domain = argv[1]; 
        int count = 2;
        char* dump = nullptr;
        while (count < argc) {
            switch (argv[count][1]) {
            case 'i':
                count++;
                if(count == argc) {
                    std::cerr << "Argument used but not filled!" << std::endl;
                    throw std::exception{};
                }
                args.interface = argv[count];
                break;
            case 'c':
                count++;
                if(count == argc) {
                    std::cerr << "Argument used but not filled!" << std::endl;
                    throw std::exception{};
                }
                args.max_hops = strtol(argv[count], &dump, 10);

                if(argv[count] == dump || *dump != '\0') {
                    std::cerr << "Non digital value at MAX_HOPS argument!" << std::endl;
                    throw std::exception{};
                }
                break;
            default:
                std::cerr << "Wrong argument!" << std::endl;
                throw std::exception{};
                break;
            }
            count++;
        }
    } else if(argc != 2) {
        std::cerr << "Program require ip/domain for work!" << std::endl;
        throw std::exception{};
    }
    if(argv[1] == "-h")
        showHelpPage();
    else {
        args.domain = argv[1]; 
    }
    return args;
}

void showHelpPage() {
    std::cout << "\n\t\t- - - - HOW TO USE - - - -\n\n"
                 "nettrace IP/DOMAIN [-i INTERFACE] [-c NUM]\n"
                 "\n"
                 "-i INTERFACE - define, which interface use for tracing. \n"
                 "INTERFACE should be name from system {see by $(ip addr)} of device. Make sure it can reach requesting ip/domain\n"
                 "\n"
                 "-c NUM - amount of shown ips between you and destination. NUM should be number from 1 to 2^32\n"
                 "\n"
                 "-h - show this page (nettrace -h)\n"
                 "\n"
                 << std::endl;
}