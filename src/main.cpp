#include <iostream>
#include <iomanip>

#include "monitor/resource_monitor.hpp"

int main() {

    std::cout << "ProcessPilot started successfully!"
              << std::endl;

    double cpu_usage = processpilot::get_cpu_usage();

    if (cpu_usage < 0) {
        std::cerr << "Error: Could not read CPU usage."
                  << std::endl;

        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "CPU Usage: "
              << cpu_usage
              << "%"
              << std::endl;
double memory_usage = processpilot::get_memory_usage();

if (memory_usage < 0) {
    std::cerr << "Error: Could not read memory usage."
              << std::endl;

    return 1;
}

std::cout << "RAM Usage: "
          << memory_usage
          << "%"
          << std::endl;
    return 0;
}
