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

    if(!str.empty() && (str.find_first_not_of( "0123456789" ) == std::string::npos)){result.push_back(str);} //Second condition for this particular application
    //if(!str.empty()){result.push_back(str);}


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



bool exists(const std::string &path){

    return static_cast<bool>(std::ifstream (path));

}

bool writeCSV(const std::string &path, const std::vector<std::vector<double> > &data, const std::string &delimiter){

    std::vector<std::vector<std::string>> data_str;
    data_str.reserve(data.size());
    for(auto &vect:data){
        std::vector<std::string> str_vect;


    #ifdef EU_FORMAT //If this is defined, replaces all "." by "," in doubles when converting them to strings. This is to compensate for Excel using strange conventions
    std::for_each(str_vect.begin() , str_vect.end() , [](std::string str){

       std::replace(str.begin() , str.end() , '.' , ',');

    });

    std::transform(vect.begin(), vect.end() , std::back_inserter(str_vect), [](double d) {

        std::string double_str = std::to_string(d);
        std::replace(double_str.begin() , double_str.end() , '.' , ',');
        return double_str;
    });

    #else
    std::transform(vect.begin(), vect.end() , std::back_inserter(str_vect), [](double d) {return std::to_string(d);});
    #endif


        data_str.push_back(str_vect);
    }


    return writeCSV(path , data_str , delimiter);

}

bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data , const std::string &delimiter){

    if(Utility::exists(path)){std::remove(&path[0]);}

    std::ofstream file(path + ".csv");

    if(file.is_open()){

        for(auto &line:data){
            for(auto &item:line){file << item << delimiter;}
            file << "\n";
        }

    }
    else{return false;}

    file.close();
    return true;

}

bool writeCSV(const std::string &path , const std::vector<std::vector<double>> &data , const char &delimiter){return Utility::writeCSV(path , data , std::to_string(delimiter));}
bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data , const char &delimiter){return Utility::writeCSV(path , data , std::to_string(delimiter));}

bool writeCSV(const std::string &path , const std::vector<std::vector<double>> &data){return Utility::writeCSV(path , data , ";");}
bool writeCSV(const std::string &path, const std::vector<std::vector<std::string>> &data){return Utility::writeCSV(path , data , ";");}


}
