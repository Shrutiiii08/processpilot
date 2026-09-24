
#ifndef RESOURCE_MONITOR_HPP
#define RESOURCE_MONITOR_HPP
#include <string>
namespace processpilot {

double get_cpu_usage();
double get_memory_usage();
double get_disk_usage();
bool is_network_up();
bool is_process_running(const std::string& process_name);
}

#endif


