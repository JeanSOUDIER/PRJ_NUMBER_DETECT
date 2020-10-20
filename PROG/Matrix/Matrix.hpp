#ifndef MATRIX_H
#define MATRIX_H


#include <algorithm>

#include <functional>

#include <iostream>
#include <iterator>

#include <stdexcept>

#include <vector>

namespace ste{
template<class T>
class Matrix{

    private:

        std::vector<std::vector<T>> _data;


    public:
        /***************************************************/

        Matrix(const unsigned long &lines , const unsigned long&columns){

            _data.reserve(lines);
            for(unsigned index = 0 ; index < lines ; index++){
                _data.push_back(std::vector<T>(columns));

            }


        }
        Matrix(const std::vector<std::vector<T>> &data){_data = data;}
        ~Matrix(){}


        /***************************************************/

        const std::vector<unsigned long> size() const {
            if(_data.empty()){return {0 , 0};}

            if(_data.at(0).empty()){return std::vector<unsigned long>{_data.size() , 0};}

            return std::vector<unsigned long>{_data.size() , _data.at(0).size()};

        }

        /***************************************************/

        const T at(const unsigned &line , const unsigned &column) const {

            if(_data.empty()){throw std::out_of_range("Index out of range.");}

            if(line >= _data.size() || column >= _data.at(0).size()){throw std::out_of_range("Index out of range.");}

            return _data.at(line).at(column);

        }

        const std::vector<T> at(const unsigned &line) const {

         if(line >= _data.size()){throw std::out_of_range("Index out of range.");}
            return _data.at(line);
        }


                /***************************************************/

        void replace(const unsigned &line , const unsigned &column , const T &value){_data.at(line).at(column) = value;}
        void replace(const unsigned &line , const std::vector<T> &value){_data.at(line) = value;}
        void replace(const unsigned &line_begin, const unsigned &line_end ,
                     const unsigned &column_begin, const unsigned &column_end,
                     const T &value){

                for(unsigned line = line_begin ; line < line_end ; line++){

                    for(unsigned column = column_begin ; column < column_end ; column++){

                        _data.at(line).at(column) = value;

                    }

                }

        }


                  /***************************************************/

        std::vector<std::vector<T>> toVector() const{return _data;}

        void print() const{

            std::cout << "[";
            for(auto &line:_data){
                std::cout << "[";
                std::copy (line.begin(), line.end(), std::ostream_iterator<T>(std::cout, " "));
                std::cout << "]" << std::endl;
            }
            std::cout << "]" << std::endl;

        }

        unsigned long begin_line(){return 0;}
        unsigned long begin_column(){return 0;}

        unsigned long end_line(){return _data.size();}
        unsigned long end_column(){return _data.at(0).size();}

                /***************************************************/

        T det() const{return T(0);}

        Matrix cofactor() const{return Matrix(1,1);}

        Matrix transpose() const{return Matrix(1,1);}


                /***************************************************/

        void fill(const unsigned &size , const T &value){

            _data.clear();
            _data.reserve(size);
            for(unsigned line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }



        }

        void fill(const unsigned &length , const unsigned &width , const T &value){

            _data.clear();
            _data.reserve(length);
            for(unsigned line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }


        }



                  /***************************************************/

        void zeroes(const unsigned &size){

            _data.clear();
            _data.reserve(size);
            for(unsigned line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }
        void zeroes(const unsigned &length , const unsigned &width){

           _data.clear();
           _data.reserve(length);
            for(unsigned line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }


                    /***************************************************/

        void ones(const unsigned &size){
            _data.clear();
            _data.reserve(size);
            for(unsigned line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }
        void ones(const unsigned &length , const unsigned &width){

            _data.clear();
            _data.reserve(length);
            for(unsigned line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }






                /***************************************************/


        void operator=(const Matrix &arg){this = arg;}
        void operator=(const std::vector<std::vector<T>> &arg){_data = arg;}


        Matrix operator* (const Matrix &arg) const{

            Matrix result(_data.size() , arg.at(0).size());


            for(unsigned index_line = 0 ; index_line < _data.size() ; index_line++){

                for(unsigned index_column = 0 ; index_line < arg.at(0).size() ; index_column++){
                    T value = T(0);
                    for(unsigned index_sum = 0 ; index_sum < _data.at(0).size() ; index_sum++){
                        value += T(_data.at(index_line).at(index_sum)) * T(arg._data.at(index_sum).at(index_column));
                    }
                    result.replace(index_line , index_column , value);
                }

            }

        }
        Matrix operator* (const double &arg) const{

            if(arg == 1){return this;}

            for(auto &line:_data){

            std::transform(line.begin(), line.end(), line.begin(),
                           std::bind(std::multiplies<T>(), std::placeholders::_1, arg));

            }


        }

        void operator *=(const Matrix &arg){this = arg*this;}
        void operator *=(const double &arg){this = arg*this;}

        Matrix operator+ (const Matrix &arg) const{

            Matrix result(_data);

            for(unsigned index = 0 ; index < result._data.size() ; index++){

                std::transform (result._data.at(index).begin(),
                                result._data.at(index).end(),
                                arg._data.at(index).begin(),
                                result._data.at(index).begin(), std::plus<T>());

            }

            return result;

        }
        void operator +=(const Matrix &arg){this = this + arg;}

        Matrix operator- (const Matrix &arg) const {return this + (-1)*arg;}
        void operator -= (const Matrix &arg){this = this - arg;}

        Matrix operator! () const{                   //Inverse of matrix

            return Matrix(1,1);

        }


        Matrix operator^ (const unsigned &arg) const{ //Power operator

            if(arg <= 1){return this;}
            Matrix output(_data);

            for(unsigned index = 0 ; index < arg ; index++){output *= this;}

            return output;
        }

        bool operator== (const Matrix &arg) const{return _data == arg._data;}
        bool operator!= (const Matrix &arg) const{return !(this == arg);}

    protected:


};
}
#endif // MATRIX_H
