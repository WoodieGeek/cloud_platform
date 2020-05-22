#include "../library/server.h"
#include "lib/server.h"
#include "lib/server_options.h"
#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>

namespace po = boost::program_options;

namespace {
    std::string ReadFromFile(const std::string& fileName) {
        std::ifstream file(fileName);
        std::stringstream result;
        if (file.is_open()) {
            result << file.rdbuf();
        }
        return result.str();
    }
}

int main(int argc, char* argv[]) {
    po::options_description description("Allowed options");
    description.add_options()
        ("help", "produce help message")
        ("port", po::value<std::string>()->required(), "set port")
        ("ip", po::value<std::string>()->required(), "set IP")
        ("connect", po::value<std::string>()->required(), "set connection string")
        ("config", po::value<std::string>()->required(), "set config file name ");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    if (!vm.count("ip") || !vm.count("port") || !vm.count("connect") || !vm.count("config")) {
        std::cout << "IP or port is missing\n";
        return 1;
    }
    std::string port, ip, connect, config;
    ip = vm["ip"].as<std::string>();
    port = vm["port"].as<std::string>();
    connect = vm["connect"].as<std::string>();
    config = vm["config"].as<std::string>();
    std::cout << ip << ":" << port << std::endl;
    std::cout << "Reading config file...\n";
    auto serverOptions = TServerOptions::Parse(ReadFromFile(config));
    try {
        std::cout << "Starting...\n";
        TMasterServer server(ip, port, connect, serverOptions);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
  return 0;
}
