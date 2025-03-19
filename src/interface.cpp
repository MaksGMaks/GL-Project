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
            case 't':
                count++;
                if(count == argc) {
                    std::cerr << "Argument used but not filled!" << std::endl;
                    throw std::exception{};
                }
                args.timeout = strtol(argv[count], &dump, 10);

                if(argv[count] == dump || *dump != '\0') {
                    std::cerr << "Non digital value at MAX_HOPS argument!" << std::endl;
                    throw std::exception{};
                }
                break;
            case 'd':
                args.showFQDN = true;
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
    if(std::string(argv[1]) == "-h")
        throw std::exception{};
    else {
        args.domain = argv[1]; 
    }
    return args;
}

void showHelpPage() {
    std::cout << "\n\t\t- - - - HOW TO USE - - - -\n\n"
                 "nettrace IP/DOMAIN [-i INTERFACE] [-c NUM] [-t TIME] [-d]\n"
                 "\n"
                 "-i INTERFACE - define, which interface use for tracing. \n"
                 "INTERFACE should be name from system {see by $(ip addr)} of device. Make sure it can reach requesting ip/domain\n"
                 "\n"
                 "-c NUM - amount of shown ips between you and destination, by default is 10. NUM should be number from 1 to 65535\n"
                 "\n"
                 "-t TIME - timeout in seconds, by default is 3. TIME should be number from 1 to 255\n"
                 "\n"
                 "-d - option to show FQDN of ip if it's available\n"
                 "\n"
                 "-h - show this page (nettrace -h)\n"
                 "\n"
                 << std::endl;
}

double getTimeDiff(const struct timespec &start, const struct timespec &end) {
    return (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
}