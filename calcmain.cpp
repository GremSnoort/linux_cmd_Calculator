#include "Processor.h"

#include <iostream>
#include <stdexcept>

static inline bool need_term(const std::string& line) {
    return !line.compare("exit") || !line.compare("quit") || !line.compare("q") || !line.compare("e");
}

int main() {
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        if (need_term(line)) break;
        else if (line.length() > 0) {
            try {
                std::cout << cmd_calc::Processor(line).run() << std::endl;
            } catch (std::exception &e) {
                std::cout << "error: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}


