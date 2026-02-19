#include "lru_cache.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static void print_help(const char* argv0) {
    std::cout
        << "Usage: " << argv0 << " [--capacity N]\n"
        << "\n"
        << "Interactive demo for a templated LRU cache.\n"
        << "\n"
        << "Commands:\n"
        << "  put <key> <value>\n"
        << "  get <key>\n"
        << "  erase <key>\n"
        << "  dump\n"
        << "  help\n"
        << "  exit\n";
}

int main(int argc, char** argv) {
    std::size_t capacity = 4;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            print_help(argv[0]);
            return 0;
        } else if (arg == "--capacity") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --capacity\n";
                return 2;
            }
            capacity = static_cast<std::size_t>(std::stoul(argv[++i]));
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            print_help(argv[0]);
            return 2;
        }
    }

    LRUCache<std::string, std::string> cache(capacity);

    std::cout << "LRU cache demo (capacity=" << capacity << ")\n";
    std::cout << "Type 'help' for commands.\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd.empty()) continue;

        if (cmd == "help") {
            print_help(argv[0]);
        } else if (cmd == "exit" || cmd == "quit") {
            break;
        } else if (cmd == "put") {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            if (!value.empty() && value.front() == ' ') value.erase(value.begin());
            if (key.empty() || value.empty()) {
                std::cout << "Usage: put <key> <value>\n";
                continue;
            }
            cache.put(key, value);
            std::cout << "OK\n";
        } else if (cmd == "get") {
            std::string key;
            iss >> key;
            if (key.empty()) {
                std::cout << "Usage: get <key>\n";
                continue;
            }
            auto v = cache.get(key);
            if (!v) {
                std::cout << "(miss)\n";
            } else {
                std::cout << *v << "\n";
            }
        } else if (cmd == "erase") {
            std::string key;
            iss >> key;
            if (key.empty()) {
                std::cout << "Usage: erase <key>\n";
                continue;
            }
            bool ok = cache.erase(key);
            std::cout << (ok ? "OK" : "(not found)") << "\n";
        } else if (cmd == "dump") {
            auto keys = cache.keys_mru_to_lru();
            std::cout << "MRU -> LRU: ";
            for (std::size_t i = 0; i < keys.size(); ++i) {
                std::cout << keys[i];
                if (i + 1 < keys.size()) std::cout << " ";
            }
            std::cout << "\n";
        } else {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }

    return 0;
}
