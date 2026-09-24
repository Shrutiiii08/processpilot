		
#include "resource_monitor.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <sys/statvfs.h>
#include <dirent.h>
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
bool is_network_up() {
    std::ifstream file("/sys/class/net/eth0/operstate");

    if (!file) {
        return false;
    }

    std::string state;
    file >> state;

    return state == "up";
}
bool is_process_running(const std::string& process_name) {
    DIR* directory = opendir("/proc");

    if (directory == nullptr) {
        return false;
    }

    struct dirent* entry;

    while ((entry = readdir(directory)) != nullptr) {
        std::string folder_name = entry->d_name;

        // Check whether the folder name contains only digits
        bool is_pid = !folder_name.empty();

        for (char character : folder_name) {
            if (!std::isdigit(static_cast<unsigned char>(character))) {
                is_pid = false;
                break;
            }
        }

        if (!is_pid) {
            continue;
        }

        std::string command_path =
            "/proc/" + folder_name + "/comm";

        std::ifstream command_file(command_path);
        std::string command_name;

        if (command_file >> command_name) {
            if (command_name == process_name) {
                closedir(directory);
                return true;
            }
        }
    }

    closedir(directory);
    return false;
}
}
