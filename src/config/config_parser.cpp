#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace processpilot {

std::map<std::string, std::string> load_config(
    const std::string& filename) {

    std::map<std::string, std::string> config;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open configuration file."
                  << std::endl;
        return config;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::size_t separator = line.find('=');

        if (separator == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, separator);
        std::string value = line.substr(separator + 1);

        config[key] = value;
    }

    file.close();

    return config;
}

}
