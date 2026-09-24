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
    
double disk_usage = processpilot::get_disk_usage();

if (disk_usage < 0) {
    std::cerr << "Error: Could not read disk usage."
              << std::endl;

    return 1;
}

std::cout << "Disk Usage: "
          << disk_usage
          << "%"
          << std::endl;
bool network_status = processpilot::is_network_up();

if (network_status) {
    std::cout << "Network Status: UP" << std::endl;
} else {
    std::cout << "Network Status: DOWN" << std::endl;
}
bool bash_running = processpilot::is_process_running("bash");

if (bash_running) {
    std::cout << "Bash Process: RUNNING" << std::endl;
} else {
    std::cout << "Bash Process: NOT RUNNING" << std::endl;
}
    return 0;
}
