#ifndef UTILITY_H
#define UTILITY_H

#include "stdint.h"

#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <string>


namespace Utility {

    std::vector<std::string> split(std::string str, const std::string &delimiter);
    std::vector<std::string> split(const std::string &str , const char &delimiter);

    std::string readAllFile(const std::string &path); //DO NOT USE FOR LARGE FILES, OR IT IS VERY LIKELY TO CAUSE AN OOM (Out Of Memory) ERROR
    uint64_t fileSize(const std::string &path);
    bool exists(const std::string &path);

    bool writeCSV(const std::string &path , const std::vector<std::vector<double>> &data , const std::string &delimiter);
    bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data , const std::string &delimiter);

    bool writeCSV(const std::string &path , const std::vector<std::vector<double>> &data , const char &delimiter);
    bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data , const char &delimiter);

    bool writeCSV(const std::string &path , const std::vector<std::vector<double>> &data);
    bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data);

}
#endif // UTILITY_H
