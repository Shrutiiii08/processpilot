	
#include "resource_monitor.hpp"

#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <sys/statvfs.h>

namespace processpilot {

double get_cpu_usage() {

    std::ifstream file("/proc/stat");

    if (!file.is_open()) {
        return -1.0;
    }

    std::string line;
    std::getline(file, line);

    std::istringstream stream(line);

    std::string cpu;
    long long user;
    long long nice;
    long long system;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;
    long long steal;

    stream >> cpu >> user >> nice >> system
           >> idle >> iowait >> irq >> softirq >> steal;

    long long idle_time = idle + iowait;

    long long total_time = user + nice + system + idle
                         + iowait + irq + softirq + steal;

    file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    file.open("/proc/stat");

    if (!file.is_open()) {
        return -1.0;
    }

    std::getline(file, line);

    std::istringstream stream2(line);

    stream2 >> cpu >> user >> nice >> system
            >> idle >> iowait >> irq >> softirq >> steal;

    long long idle_time_after = idle + iowait;

    long long total_time_after = user + nice + system + idle
                               + iowait + irq + softirq + steal;

    long long total_difference = total_time_after - total_time;
    long long idle_difference = idle_time_after - idle_time;

    if (total_difference == 0) {
        return 0.0;
    }

    double cpu_usage = 100.0 *
                       (total_difference - idle_difference)
                       / total_difference;

    return cpu_usage;
}

double get_memory_usage() {

    std::ifstream file("/proc/meminfo");

    if (!file.is_open()) {
        return -1.0;
    }

    std::string label;
    long long value;
    std::string unit;

    long long total_memory = 0;
    long long available_memory = 0;

    while (file >> label >> value >> unit) {

        if (label == "MemTotal:") {
            total_memory = value;
        }
        else if (label == "MemAvailable:") {
            available_memory = value;
        }

        if (total_memory > 0 && available_memory > 0) {
            break;
        }
    }

    file.close();

    if (total_memory == 0) {
        return -1.0;
    }

    double memory_usage =
        100.0 * (total_memory - available_memory)
        / total_memory;

    return memory_usage;
}
double get_disk_usage() {

    struct statvfs disk_info;

    if (statvfs("/", &disk_info) != 0) {
        return -1.0;
    }

    unsigned long long total_blocks = disk_info.f_blocks;
    unsigned long long available_blocks = disk_info.f_bavail;

    if (total_blocks == 0) {
        return -1.0;
    }

    unsigned long long used_blocks =
        total_blocks - available_blocks;

    double disk_usage =
        100.0 * static_cast<double>(used_blocks)
        / static_cast<double>(total_blocks);

    return disk_usage;
}
}
