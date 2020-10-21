#ifndef MATRIX_H
#define MATRIX_H



#include <algorithm>

#include <cmath>

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

        Matrix(const unsigned long long &lines , const unsigned long long&columns){

            _data.reserve(lines);
            for(unsigned long long index = 0 ; index < lines ; index++){
                _data.push_back(std::vector<T>(columns));

            }


        }

        Matrix(const unsigned long long &size) : Matrix(size , size){}

        Matrix(const std::vector<std::vector<T>> &data){_data = data;}
        ~Matrix(){}


        /***************************************************/

        const std::vector<unsigned long long> size() const {
            if(_data.empty()){return {0 , 0};}

            if(_data.at(0).empty()){return std::vector<unsigned long long>({_data.size() , 0});}

            return std::vector<unsigned long long>({_data.size() , _data.at(0).size()});

        }

        /***************************************************/


        bool isLine() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (size().at(0) == 1);

        }

        bool isColumn() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (size().at(1) == 1);
        }

        bool isSquare() const{return (size().at(0) == size().at(1));}

        bool isInvertible() const{return (det() != 0);}

        bool empty() const{return _data.empty();}

        bool hasEmptyData() const{

            if(empty()){return true;}

            for(auto &line:_data){
                if(line.empty()){return true;}
            }

            return false;

        }


        /***************************************************/

        T& at(const unsigned long long &line , const unsigned long long &column) {

            if(_data.empty()){throw std::out_of_range("Index out of range.");}

            if(line >= _data.size() || column >= _data.at(0).size()){throw std::out_of_range("Index out of range.");}

            return _data.at(line).at(column);

        }

        T& at(const unsigned long long &index) {

         if(index >= size().at(0)*size().at(1)){throw std::out_of_range("Index out of range.");}

         return at(index/size().at(1) , index %size().at(0));

        }

        T at(const unsigned long long &line , const unsigned long long &column) const{

            if(_data.empty()){throw std::out_of_range("Index out of range.");}

            if(line >= _data.size() || column >= _data.at(0).size()){throw std::out_of_range("Index out of range.");}

            return _data.at(line).at(column);

        }

        T at(const unsigned long long &index) const{

         if(index >= size().at(0)*size().at(1)){throw std::out_of_range("Index out of range.");}

         return at(index/size().at(1) , index %size().at(0));

        }


                /***************************************************/

        void replace(const unsigned long long &line , const unsigned &column , const T &value){_data.at(line).at(column) = value;}
        void replace(const unsigned long long &index, const T &value){at(index) = value;}
        void replace(const unsigned long long &line , const std::vector<T> &value){_data.at(line) = value;}
        void replace(const unsigned long long &line_begin, const unsigned long long &line_end ,
                     const unsigned long long &column_begin, const unsigned long long &column_end,
                     const T &value){

                for(unsigned long long line = line_begin ; line < line_end ; line++){

                    for(unsigned long long column = column_begin ; column < column_end ; column++){

                        _data.at(line).at(column) = value;

                    }

                }

        }


                  /***************************************************/

        std::vector<std::vector<T>> toVector() const{return _data;}

        void print() const{

            std::cout << "[ ";
            for(auto &line:_data){
                std::cout << "[ ";
                std::copy (line.begin(), line.end(), std::ostream_iterator<T>(std::cout, " "));
                std::cout << "] ";
            }
            std::cout << "]" << std::endl; //Buffer flushed to prevent user misusage

        }

        void print_size() const{

            std::cout << "[" << size().at(0) << " ; "<< size().at(1) << "]" << std::endl; //Buffer flushed to prevent user misusage

        }


        unsigned long long begin_line(){return 0;}
        unsigned long long begin_column(){return 0;}

        unsigned long long end_line(){return _data.size();}
        unsigned long long end_column(){return _data.at(0).size();}

                /***************************************************/


        const T trace() const{

            if(!isSquare()){throw std::invalid_argument("Matrix is not square.");}

            T accumulator = T(0);


            for(unsigned long long index = 0 ; index < size().at(0) ; index++){
                accumulator += T(at(index , index));
            }

            return accumulator;

        }

        const T sum() const{

            if(_data.empty()){throw std::invalid_argument("Matrix is empty.");}

            for(auto &line:_data){
                if(line.empty()){throw std::invalid_argument("A column is empty.");}
            }


            T accumulator = T(0);

            for(unsigned long long index_line = 0 ; index_line < size().at(0) ; index_line++){
                for(unsigned long long index_column = 0 ; index_column < size().at(1) ; index_column++){
                    accumulator += at(index_line , index_column);
                }
            }

            return accumulator;

        }

        const T mean() const{return average();}

        const T average() const{
            return sum() / (size().at(0)*size().at(1));
        }

        T det() const{

            if(empty() || hasEmptyData()){throw std::invalid_argument("Matrix is empty or has empty columns.");}

            if(!isSquare()){throw std::invalid_argument("Matrix is not square.");}

            if(_data.size() == 1 && _data.at(0).size() == 1){return _data.at(0).at(0);}
            if(_data.size() == 2 && _data.at(0).size() == 2){return _data.at(0).at(0) * _data.at(1).at(1) - _data.at(0).at(1)*_data.at(1).at(0);}


            T determinant = T(0);

            for(unsigned long long row_index = 0 ; row_index < _data.at(0).size() ; row_index++){

                std::vector<std::vector<T>> temp_data;

                for(unsigned long long i = 1 ; i < _data.size() ; i++){

                    std::vector<T> temp_row;

                    for(unsigned long long j = 0 ; j < _data.at(i).size() ; j++){

                        if(j != row_index){temp_row.push_back(at(i , j));}

                    }

                    if(!temp_row.empty()){temp_data.push_back(temp_row);}

                }

                determinant += at(0 , row_index) * std::pow(-1 , row_index) * Matrix(temp_data).det();


            }

            return determinant;

        }

        Matrix comatrix() const {return cofactormatrix();}


        Matrix cofactormatrix() const {


            //TODO
            return Matrix(1,1);


        }





        Matrix transpose() const{

            Matrix result(_data);

            if(result.isLine()){

                std::vector<std::vector<T>> data (result.size().at(1));

                for(unsigned long long index = 0 ; index < result.size().at(1) ; index++){
                    data.at(index) = std::vector<T>({result.at(0 , index)});
                }

                result = data;

            }
            else if(result.isColumn()){

                std::vector<T> data (result.size().at(0));

                for(unsigned long long index = 0 ; index < result.size().at(0) ; index++){
                    data.at(index) = (result.at(index , 0));
                }

                result = std::vector<std::vector<T>>{data};

            }

            else{

                for(unsigned long long index = 0 ; index < size().at(0) * size().at(1) ; index++){
                    result.replace(index , at(index % size().at(0) , index/size().at(1)));
                }

            }



            return result;

        }


                /***************************************************/

        void fill(const unsigned long long &size , const T &value){

            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }



        }

        void fill(const unsigned long long &length , const unsigned long long &width , const T &value){

            _data.clear();
            _data.reserve(length);
            for(unsigned long long line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }


        }



                  /***************************************************/

        void zeroes(const unsigned long long &size){

            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }
        void zeroes(const unsigned long long &length , const unsigned long long &width){

           _data.clear();
           _data.reserve(length);
            for(unsigned long long line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }


                    /***************************************************/

        void ones(const unsigned long long &size){
            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }
        void ones(const unsigned long long &length , const unsigned long long &width){

            _data.clear();
            _data.reserve(length);
            for(unsigned long long line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }






                /***************************************************/


        void operator=(const Matrix &arg){_data = arg._data;}
        void operator=(const std::vector<std::vector<T>> &arg){_data = arg;}


        Matrix operator* (const Matrix &arg) const{

            Matrix result(_data.size() , arg.at(0).size());

            for(unsigned long long index_line = 0 ; index_line < _data.size() ; index_line++){

                for(unsigned long long index_column = 0 ; index_column < arg.at(0).size() ; index_column++){
                    T value = T(0);
                    for(unsigned long long index_sum = 0 ; index_sum < _data.at(0).size() ; index_sum++){
                        value += T(_data.at(index_line).at(index_sum)) * T(arg._data.at(index_sum).at(index_column));
                    }
                    result.replace(index_line , index_column , value);
                }

            }

            return result;

        }

        Matrix operator* (const T &arg) const{


            Matrix result(_data);

            if(arg == 1){return result;}

            for(auto &line:result._data){

            std::transform(line.begin(), line.end(), line.begin(),
                           std::bind(std::multiplies<T>(), std::placeholders::_1, arg));

            }

            return result;


        }

        Matrix operator* (const T &arg){

            Matrix result(_data);

            if(arg == 1){return result;}

            for(auto &line:result._data){

            std::transform(line.begin(), line.end(), line.begin(),
                           std::bind(std::multiplies<T>(), std::placeholders::_1, arg));

            }

            return result;


        }

        Matrix operator* (const Matrix &arg){

            //std::cout << "this.size() : [" << size().at(0) << " ; " << size().at(1) << "]\n";
            //std::cout << "arg.size() : [" << arg.size().at(0) << " ; " << arg.size().at(1) << "]\n";

            Matrix result(_data.size() , arg.size().at(0));
            //std::cout << "result.size() : [" << result.size().at(0) << " ; " << result.size().at(1) << "]\n";


            for(unsigned long long index_line = 0 ; index_line < _data.size() ; index_line++){

                for(unsigned long long index_column = 0 ; index_column < arg.size().at(0) ; index_column++){
                    T value = T(0);
                    for(unsigned long long index_sum = 0 ; index_sum < _data.at(0).size() ; index_sum++){
                        value += T(_data.at(index_line).at(index_sum)) * T(arg._data.at(index_sum).at(index_column));
                    }
                    result.replace(index_line , index_column , value);
                }

            }

            return result;
        }


        //TODO
        //Matrix& operator *=(const Matrix &arg){return (this*arg);}
        //Matrix& operator *=(const T arg){return (this*arg);}

        Matrix operator+ (const Matrix &arg) const{

            Matrix result(_data);

            for(unsigned long long index = 0 ; index < result._data.size() ; index++){

                std::transform (result._data.at(index).begin(),
                                result._data.at(index).end(),
                                arg._data.at(index).begin(),
                                result._data.at(index).begin(), std::plus<T>());

            }

            return result;

        }


        Matrix operator+ (const Matrix &arg){

            Matrix result(_data);

            for(unsigned long long index = 0 ; index < result._data.size() ; index++){

                std::transform (result._data.at(index).begin(),
                                result._data.at(index).end(),
                                arg._data.at(index).begin(),
                                result._data.at(index).begin(), std::plus<T>());

            }

            return result;

        }

        //TODO
        //Matrix& operator +=(const Matrix &arg){}
        //Matrix& operator +=(const T &arg){}

        Matrix operator- (const Matrix &arg) const {return Matrix(_data) + arg*(-1.);}
        Matrix operator- (const Matrix &arg) {return Matrix(_data) + arg*(-1.);}
        Matrix operator- (const T &arg) const {return Matrix(_data) + arg*(-1.);}
        Matrix operator- (const T &arg) {return Matrix(_data) + arg*(-1.);}

        //TODO
        //Matrix& operator -= (const Matrix &arg){}
        //Matrix& operator -= (const Matrix &arg){}

        Matrix operator! () const{                   //Inverse of matrix

            const T determinant = det();
            if(determinant == 0){
                std::cout << "Matrix is not inversible.\n";
                return Matrix<T>(0,0);
            }

            return (1/det())*Matrix(_data).cofactormatrix().transpose();

        }

        Matrix operator! (){                   //Inverse of matrix

            const T determinant = det();
            if(determinant == 0){
                std::cout << "Matrix is not inversible.\n";
                return Matrix<T>(0,0);
            }

            return (1/det())*Matrix(_data).cofactormatrix().transpose();

        }


        Matrix operator^ (const unsigned &arg) const{ //Power operator

            Matrix output(_data);
            if(arg <= 1){return output;}


            for(unsigned power = 1 ; power < arg ; power++){output = output * Matrix(_data);}

            return output;
        }

        Matrix operator^ (const unsigned &arg){ //Power operator

            Matrix output(_data);
            if(arg <= 1){return output;}

            for(unsigned long long power = 1 ; power < arg ; power++){output = output * Matrix(_data);}

            return output;
        }

        bool operator== (const Matrix &arg) const{return _data == arg._data;}
        bool operator== (const Matrix &arg) {return _data == arg._data;}

        bool operator!= (const Matrix &arg) const{return !(this == arg);}
        bool operator!= (const Matrix &arg){return !(this == arg);}


    protected:


};
}
#endif // MATRIX_H
