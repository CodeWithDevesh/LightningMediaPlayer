#include "lightning_utils.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <filesystem>
extern "C"
{
}

namespace lighter
{

    std::string getFileExtension(const std::string &filePath)
    {
        auto ext = std::filesystem::path(filePath).extension();
        return ext.empty() ? "" : ext.string().substr(1);
    }

    std::string trim(const std::string &str)
    {
        auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
        auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
        return (start < end) ? std::string(start, end) : "";
    }

    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string getFileName(const std::string &filePath)
    {
        auto name = std::filesystem::path(filePath).filename();
        return name.empty() ? "" : name.string();
    }

    std::string formatDuration(int seconds)
    {
        int hours = seconds / 3600;
        seconds %= 3600;
        int minutes = seconds / 60;
        seconds %= 60;
        constexpr int BUFFER_SIZE = 9;
        char buffer[BUFFER_SIZE];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
        return std::string(buffer);
    }

} // namespace utils
