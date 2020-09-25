#include "utility.h"

namespace Utility {


std::vector<std::string> split(std::string str, const std::string &delimiter){


    std::vector<std::string> result;

    size_t pos = 0;
    while((pos = str.find(delimiter)) != std::string::npos){
        const std::string token = str.substr(0 , pos);
        str.erase(0, pos + delimiter.length());
        result.push_back(token);
    }

    if(!str.empty() && (str.find_first_not_of( "0123456789" ) == std::string::npos)){result.push_back(str);}


    return result;


}

std::vector<std::string> split(const std::string &str , const char &delimiter){return split(str , std::to_string(delimiter));}







std::string readAllFile(const std::string &path){

    const uint64_t size = fileSize(path);
    if(size <=0){return "";}

    std::vector<char> buffer(size);
    std::ifstream file(path);

    if(file.is_open()){
        file.read(&buffer[0] , size);
    }
    file.close();

    return std::string(&buffer[0] , size);

}

uint64_t fileSize(const std::string &path){

    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}



bool exists(const std::string &path){return std::experimental::filesystem::exists(path);}

}
