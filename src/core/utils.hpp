#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#define INFO(...) spdlog::info(__VA_ARGS__)
#define WARN(...) spdlog::warn(__VA_ARGS__)
#define ERROR(...) spdlog::error(__VA_ARGS__)
#define DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG(level, ...) spdlog::level(__VA_ARGS__)

namespace Lighter {

// File Handling Utilities
std::string getFileExtension(const std::string &filePath);

// String Manipulation
std::string trim(const std::string &str);
std::vector<std::string> split(const std::string &str, char delimiter);

// Time Management
std::string formatDuration(int seconds);

} // namespace Lighter

#endif // UTILS_H
