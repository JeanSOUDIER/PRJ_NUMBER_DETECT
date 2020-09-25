#ifndef UTILITY_H
#define UTILITY_H

#include "stdint.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <string>

#if __cplusplus >= 201402L
#include <experimental/filesystem>
#endif

namespace Utility {

    std::vector<std::string> split(std::string str, const std::string &delimiter);
    std::vector<std::string> split(const std::string &str , const char &delimiter);

    std::string readAllFile(const std::string &path); //DO NOT USE FOR LARGE FILES, OR IT IS VERY LIKELY TO CAUSE AN OOM (Out Of Memory) ERROR
    uint64_t fileSize(const std::string &path);

    bool exists(const std::string &path);


}
#endif // UTILITY_H
