#ifndef UTILITY_H
#define UTILITY_H

#include "stdint.h"

#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <string>

#define EU_FORMAT //If this is defined, replaces all "." by "," in doubles when converting them to strings. This is to compensate for Excel using strange conventions

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

    std::vector<std::vector<std::string>> readCSV_STR(const std::string &path, const std::string &delimiter);
    std::vector<std::vector<double>> readCSV_DOUBLE(const std::string &path, const std::string &delimiter);
    std::vector<std::vector<float>> readCSV_FLOAT(const std::string &path, const std::string &delimiter);

    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T> long long int indexOf(const std::vector<T> &vector , const T &data){

        return (std::find(vector.begin(), vector.end(), data) != vector.end())
           ?
           std::distance(vector.begin(), std::find(vector.begin(), vector.end(), data))
           : -1;
    }


}
#endif // UTILITY_H
