#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#include <utility>
#include <sstream>

// Define macros for logging
#define INFO(...) spdlog::info(__VA_ARGS__)
#define WARN(...) spdlog::warn(__VA_ARGS__)
#define ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG(level, ...) spdlog::level(__VA_ARGS__)

namespace Lighter {

    // File Handling Utilities
    std::string getFileExtension(const std::string& filePath);

    // String Manipulation
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);

    // Time Management
    std::string formatDuration(int seconds);

} // namespace utils

#endif // UTILS_H
