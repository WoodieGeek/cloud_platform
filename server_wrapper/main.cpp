#include "../library/server.h"
#include "lib/server.h"
#include <boost/program_options.hpp>

#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description description("Allowed options");
    description.add_options()
        ("help", "produce help message")
        ("port", po::value<std::string>()->required(), "set port")
        ("ip", po::value<std::string>()->required(), "set IP");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    if (!vm.count("ip") || !vm.count("port")) {
        std::cout << "IP or port is missing\n";
        return 1;
    }
    std::string port, ip;
    ip = vm["ip"].as<std::string>();
    port = vm["port"].as<std::string>();
    std::cout << ip << ":" << port << std::endl;
    try {
        std::cout << "Starting...\n";
        TServerWrapper server(ip, port);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
