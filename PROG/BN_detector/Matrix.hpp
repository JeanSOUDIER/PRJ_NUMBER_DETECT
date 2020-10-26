#ifndef MATRIX_H
#define MATRIX_H



/**

                     Matrix class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.com)


    Provides a template class for matrix-based computing.

    Features :

    • Vector compatible.
    • Can hold any class that provies * , + and supports multiplication by (-1.) .
    • Determinant, inverse, tranpose ,cofactormatrix, trace, mean, average.
    • Dynamic resizing (possible to add lines or columns).
    • Classic fill, zeroes, ones and eye.
    • Possible to directly print contents and size to stdout.

    • enum class Orientation{...} to specified where lines should be added or removed.

    Functions :

    • Matrix            | Constructor. Accepts either a size (x , y or same for both) or a std::vector<std::vector<T>> to construct a Matrix.

    • size              | Returns the size of the matrix, as const std::vector<unsigned long long>.

    • isLine            | Returns true if the matrix is a line, false otherwise.
    • isColumn          | Returns true if the matrix is a column, false otherwise.
    • isSquare          | Returns true if the matrix is square, false otherwise.
    • isInvertible      | Returns true if the matrix is invertible, false otherwise.
    • empty             | Returns true if the matrix is empty, false otherwise.
    • hasEmptyData      | Returns true if the matrix holds empty data (column or line), false otherwise.

    • at                | Returns the element at the index specified in argument. It is passed by reference when the matrix is non-const. Linear indexes can be used.
    • lineAt            | Returns the line at the specified index. It is passed by reference when the matrix is non-const.
    • columnAt          | Returns the column at the specified index. It is always passed by value.

    • replace           | Replaces the element specified in argument first (and second if non-linear mode is chosen) by the value in last argument.

    • add               | Adds either a line or a column from a vector at the end of the matrix.
    • add_line          | Convience function to add a line at the end of the matrix.
    • add_column        | Convience function to add a column at the end of the matrix.

    • remove            | Removes either a line or a column at the position specified.
    • remove_line       | Convience function to remove a line at a specified position.
    • remove_column     | Convience function to remove a column at a specified position.

    • insert            | Inserts either a line or a column at the position specified.
    • insert_line       | Convience function to insert a line at a specified position.
    • insert column     | Convience function to insert a column at a specified position.

    • swap              | Swaps two lines or two columns at the positions specified.
    • swap_line         | Convience function to swap two lines at a specified positions.
    • swap_column       | Convience function to swap two columns at a specified positions.

    • print             | Prints the contents of the matrix in stdout.
    • print_size        | Prints the size of the matrix in stdout.

    • begin_line        | Convinience function that returns 0, to provide syntax as close as the one relative to std::algorithm as possible.
    • begin_column      | Convinience function that returns 0, to provide syntax as close as the one relative to std::algorithm as possible.
    • end_line          | Convinience function that returns the number of lines, to provide syntax as close as the one relative to std::algorithm as possible.
    • end column        | Convinience function that returns the number of columns, to provide syntax as close as the one relative to std::algorithm as possible.

    • trace             | Returns the trace of the matrix, computed as T (meaning that rounding error and overflow may occur). Throws an exception (std::invalid_argument) if the matrix is not square.
    • sum               | Returns the sum of all elements of the matrix, as T (meaning that overflow may occur).
    • mean              | Returns the mean value of all elements of the matrix, as T (meaning that rounding error and overflow may occur). It is computed as sum()/(size.at(0)*size.at(1)).
    • average           | Convinience function that returns mean().
    • det               | Returns the determinant of the matrix. Throws an exception (std::invalid_argument) is the matrix is not square.
    • comatrix          | Returns the cofactor matrix. Convinience function that returns cofactormatrix().
    • cofactormatrix    | Returns the cofactor matrix. (WIP) ********************************************
    • transpose         | Returns the transpose of the matrix.

    • fill              | Resizes the matrix as specified in argument and fills it with the value chosen.
    • zeroes            | Resizes the matrix as specified in argument and fills it with 0.
    • ones              | Resizes the matrix as specified in argument and fills it with the 1.
    • eye               | Creates the identity matrix of the size specified in argument.

    • operator=         | Assignment operator. Supports assignments from std::vector<std::vector<T>> and from other Matrix.
    • operator+         | Computes the addition of a Matrix with another (term by term). Also supports the addition of value in T. In that case, it is the same as adding a Matrix of the same size holding only the same value.
    • operator-         | Computes the substraction of two Matrix. Its implementation requires T to be able to be multiplied by (-1.).
    • operator!         | Returns the inverse of the matrix.
    • operator^         | Returns the matrix two the power specifed after the ^ (ex: a^2 returns a*a);
    • operator==        | Equality operator. Returns true only if all elements are identical and at the same position.
    • operator!=        | Returns the opposite of the result given by operator==.

*/



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

        enum class Orientation{
            LINE,
            COLUMN
        };

        /***************************************************/

        ///Constructor                                                                               | inits the matrix with empty vectors.
        Matrix(const unsigned long long &lines , const unsigned long long&columns){

            _data.reserve(lines);
            for(unsigned long long index = 0 ; index < lines ; index++){
                _data.push_back(std::vector<T>(columns));
            }

        }
        ///Constructor                                                                               | Creates an empty square matrix of dimension size.
        Matrix(const unsigned long long &size) : Matrix(size , size){}

        ///Constructor                                                                               | Inits the data of the matrix using a std::vector<std::vector<T>>
        Matrix(const std::vector<std::vector<T>> &data){

            const unsigned long long column_length = data.at(0).size();

            for(auto &line:data){
                if(line.size() != column_length){throw std::invalid_argument("Cannot construct a matrix with irregular column size.");}
            }

            _data = data;
        }


        /***************************************************/
        ///size                                                                                      | Returns the size of the matrix as const std::vector<unsigned long long>.
        const std::vector<unsigned long long> size() const {
            if(_data.empty()){return {0 , 0};}

            if(_data.at(0).empty()){return std::vector<unsigned long long>({_data.size() , 0});}

            return std::vector<unsigned long long>({_data.size() , _data.at(0).size()});

        }

        /***************************************************/

        /// isLine                                                                                   | Returns true if the matrix is a line, false otherwise.
        bool isLine() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (size().at(0) == 1);

        }

        ///isColumn                                                                                  | Returns true if the matrix is a column, false otherwise.
        bool isColumn() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (size().at(1) == 1);
        }

        ///isSquare                                                                                  | Returns true if the matrix is square, false otherwise.
        bool isSquare() const{return (size().at(0) == size().at(1));}

        ///isInvertible                                                                              | Returns true if the matrix is invertible, false otherwise.
        bool isInvertible() const{return (det() != 0);}

        ///empty                                                                                     | Returns true if the matrix is empty, false otherwise.
        bool empty() const{return _data.empty();}

        ///hasEmptyData                                                                              | Returns true if the matrix holds empty data (column or line), false otherwise.
        bool hasEmptyData() const{

            if(empty()){return true;}

            for(auto &line:_data){
                if(line.empty()){return true;}
            }

            return false;

        }



        /***************************************************/

        ///at                                                                                        | Returns by reference the element at (line , column).
        T& at(const unsigned long long &line , const unsigned long long &column){

            if(empty()){throw std::out_of_range("Index out of range.");}

            if(line >= size().at(0) || column >= size().at(1)){throw std::out_of_range("Index out of range.");}

            return _data.at(line).at(column);

        }

        ///at                                                                                        | Returns by reference the element at the linear index specified.
        T& at(const unsigned long long &index){

         if(index >= size().at(0)*size().at(1)){throw std::out_of_range("Index out of range.");}

         return at(index/size().at(1) , index %size().at(0));

        }

        ///at                                                                                        | Returns the value of the element at (line , column).
        T at(const unsigned long long &line , const unsigned long long &column) const{

            if(empty()){throw std::out_of_range("Index out of range.");}

            if(line >= size().at(0) || column >= size().at(1)){throw std::out_of_range("Index out of range.");}

            return _data.at(line).at(column);

        }

        ///at                                                                                        | Returns the value of the element at the linear index specified.
        T at(const unsigned long long &index) const{

         if(index >= size().at(0)*size().at(1)){throw std::out_of_range("Index out of range.");}

         return at(index/size().at(1) , index %size().at(0));

        }

        ///lineAt                                                                                    | Returns by reference the line at the specified index.
        std::vector<T>& lineAt(const unsigned long long &index){

            if(empty()){throw std::out_of_range("Index out of range.");}
            if(index >= size().at(0)){throw std::out_of_range("Index out of range.");}
            return _data.at(index);

        }

        ///lineAt                                                                                    | Returns the value of the line at the specified index.
        std::vector<T> lineAt(const unsigned long long & index) const{

            if(empty()){throw std::out_of_range("Index out of range.");}
            if(index >= size().at(0)){throw std::out_of_range("Index out of range.");}
            return _data.at(index);

        }

        ///columnAt                                                                                  | Returns the value of the column at the specified index.
        std::vector<T>& columnAt(const unsigned long long &index) const {

            if(hasEmptyData()){throw std::out_of_range("Index out of range.");}
            if(index >= size().at(1)){throw std::out_of_range("Index out of range.");}

            std::vector<T> result;
            result.reserve(size().at(1));

            for(auto &line:_data){
                result.push_back(line.at(index));
            }

            return result;
        }



                /***************************************************/


        ///replace                                                                                   | Replaces the element at (line , column) by value.
        void replace(const unsigned long long &line , const unsigned &column , const T &value){_data.at(line).at(column) = value;}

        ///replace                                                                                   | Replaces the element at index (linear index) by value.
        void replace(const unsigned long long &index, const T &value){at(index) = value;}

        ///replace                                                                                   | Replaces the line or column specified in argument by value.
        void replace(const unsigned long long &value_index ,const Orientation &orientation , const std::vector<T> &value){


            switch(orientation){

                case(Orientation::LINE):{
                    if(value_index >= size().at(0)){throw std::invalid_argument("Cannot replace a line outside the matrix.");}
                    if(value.size() != size().at(1)){throw std::invalid_argument("Cannot replace a line by another one with different length.");}

                    _data.at(value_index) = value;
                    break;
                }
                case(Orientation::COLUMN):{
                    if(value_index  >=  size().at(1)){throw std::invalid_argument("Cannot replace a column outside the matrix.");}
                    if(value.size() != size().at(0)){throw std::invalid_argument("Cannot replace a column by another one with different length.");}

                        for(unsigned long long index_line = 0 ; index_line < size().at(0) ; index_line++){
                            replace(index_line , value_index , value.at(index_line));
                        }


                    break;
                }

                default:{throw std::runtime_error("Invalid orientation provided to add.");}

            }


        }

        ///replace_line                                                                              | Replaces the line specified in argument by value.
        void replace_line(const unsigned long long &value_index , const std::vector<T> &value){replace(value_index , Orientation::LINE , value);}

        ///replace_column                                                                            | Replaces the column specified in argument by value.
        void replace_column(const unsigned long long &value_index , const std::vector<T> &value){replace(value_index , Orientation::COLUMN , value);}

        /// replace                                                                                  | Replaces the elements in the range specified by value.
        void replace(const unsigned long long &line_begin, const unsigned long long &line_end ,
                     const unsigned long long &column_begin, const unsigned long long &column_end,
                     const T &value){


                if(line_begin < begin_line()     ||
                   line_end > end_line()         ||
                   column_begin < begin_column() ||
                   column_end > end_column()){throw std::invalid_argument("Cannot replace an element outside the matrix.");}


                for(unsigned long long line = line_begin ; line < line_end ; line++){

                    for(unsigned long long column = column_begin ; column < column_end ; column++){

                        _data.at(line).at(column) = value;

                    }

                }

        }


                  /***************************************************/

        ///add                                                                                       | Adds either a column or a line at the end of the matrix containing data according to orientation.
        void add(const std::vector<T> &data , const Orientation &orientation){

            switch(orientation){

                case(Orientation::LINE):{
                    if(data.size() != size().at(1)){throw std::invalid_argument("Sizes must match when appending a new line to a matrix.");}

                    _data.push_back(data);
                    break;
                    }
                case(Orientation::COLUMN):{
                    if(data.size() != size().at(0)){throw std::invalid_argument("Sizes must match when appending a new line to a matrix.");}

                    for(unsigned long long index = 0 ; index < size().at(0) ; index++){
                        _data.at(index).push_back(data.at(index));
                    }
                break;
                }

                default:{throw std::runtime_error("Invalid orientation provided to add.");}

            }

        }

        ///add_line                                                                                  | Adds a line containing data at the end of the matrix.
        void add_line(const std::vector<T> &data){add(data , Orientation::LINE);}

        ///add_column                                                                                | Adds a column containing data at the end of the matrix.
        void add_column(const std::vector<T> &data){add(data , Orientation::COLUMN);}

                  /***************************************************/

        ///remove(const std::vector<T> &data , const Orientation &orientation)                       | Removes either a column or a line at the specified index containing data according to orientation.
        void remove(const unsigned long long &element_index , const Orientation &orientation){

            switch(orientation){

                case(Orientation::LINE):{

                if(element_index >= size().at(0)){throw std::invalid_argument("Cannot remove a line outside the matrix.");}

                _data.erase(_data.begin()+element_index);

                break;
                }
                case(Orientation::COLUMN):{

                if(element_index >= size().at(1)){throw std::invalid_argument("Cannot remove a column outside the matrix.");}
                for(unsigned long long index = 0 ; index < size().at(0) ; index++){
                    _data.at(index).erase(_data.at(index).begin()+element_index);
                }
                break;
                }

                default:{throw std::runtime_error("Invalid orientation provided to remove.");}

            }


        }

        ///remove_line                                                                               | Removes a line at the specified index.
        void remove_line(const unsigned long long &index){remove(index, Orientation::LINE);}

        ///remove_column                                                                             | Removes a column at the specified index.
        void remove_column(const unsigned long long &index){remove(index, Orientation::COLUMN);}


                  /***************************************************/
        ///insert                                                                                    | Inserts a column or a line containing data at the specified index.
        void insert(const unsigned long long &element_index , const Orientation &orientation , const std::vector<T> &data){

            switch(orientation){

                case(Orientation::LINE):{
                    if(data.size() != size().at(1)){throw std::invalid_argument("Cannot insert a line which does not have the same length as the others.");}
                    if(element_index>size().at(0)){insert(size().at(0) , orientation , data); return;}

                    _data.insert(_data.begin() + element_index , data);

                break;
                }
                case(Orientation::COLUMN):{
                    if(data.size() != size().at(0)){throw std::invalid_argument("Cannot insert a column which does not have the same length as the others.");}
                    if(element_index>size().at(1)){insert(size().at(1) , orientation , data); return;}

                    for(unsigned long long index_line = 0 ; index_line < size().at(0) ; index_line++){
                        _data.at(index_line).insert(_data.at(index_line).begin() +  element_index , data.at(index_line));
                    }



                break;
                }

                default:{throw std::runtime_error("Invalid orientation provided to insert.");}

            }


        }
        ///insert_line                                                                               | Inserts a line at index containing data.
        void insert_line(const unsigned long long &index , const std::vector<T> &data){insert(index, Orientation::LINE , data);}

        ///insert_column                                                                             | Inserts a column at index containing data.
        void insert_column(const unsigned long long &index , const std::vector<T> &data){insert(index, Orientation::COLUMN , data);}

        /***************************************************/

        void swap(const unsigned long long &element_1 , const unsigned long long element_2 ,const Orientation &orientation){



            switch(orientation){

                case(Orientation::LINE):{

                    if(element_1 >= size().at(0) || element_2 >= size().at(0)){throw std::invalid_argument("Cannot swap lines outside the matrix.");}

                    std::vector<T> temp = _data.at(element_1);
                    replace_line(element_1 , _data.at(element_2));
                    replace_line(element_2 , temp);

                break;
                }
                case(Orientation::COLUMN):{

                    if(element_1 >= size().at(1) || element_2 >= size().at(1)){throw std::invalid_argument("Cannot swap columns outside the matrix.");}

                    std::vector<T> temp;
                    temp.reserve(size().at(1));

                    for(auto &line:_data){
                        temp.push_back(line.at(element_1));
                    }

                    replace_column(element_1 , _data.at(element_2));
                    replace_column(element_2 , temp);


                break;
                }

                default:{throw std::runtime_error("Invalid orientation provided to insert.");}

            }


        }

        void swap_lines(const unsigned long long &element_1 , const unsigned long long element_2){swap(element_1, element_2 ,Orientation::LINE);}
        void swap_columns(const unsigned long long &element_1 , const unsigned long long element_2){swap(element_1, element_2 ,Orientation::COLUMN);}

        /***************************************************/

        ///toVector                                                                                   | Converts the matrix to std::vector<std::vector<T>>.
        std::vector<std::vector<T>> toVector() const{return _data;}

        ///print                                                                                      | Prints the contents of the matrix in stdout.
        void print() const{

            std::cout << "[ ";
            for(auto &line:_data){
                std::cout << "[ ";
                std::copy (line.begin(), line.end(), std::ostream_iterator<T>(std::cout, " "));
                std::cout << "] ";
            }
            std::cout << "]" << std::endl; //Buffer flushed to prevent user misusage

        }

        ///print_size                                                                                 | Prints the size of the matrix in stdout.
        void print_size() const{

            std::cout << "[" << size().at(0) << " ; "<< size().at(1) << "]" << std::endl; //Buffer flushed to prevent user misusage

        }

        ///begin_line | begin_column | begin_line | end_column                                        | Convinience functions that returns the beginning or the end of a line / a column. Provided to obtain syntax as close as std::algorithm functions as possible.
        unsigned long long begin_line() const{return 0;}
        unsigned long long begin_column() const{return 0;}

        unsigned long long end_line() const{return _data.size();}
        unsigned long long end_column() const{return _data.at(0).size();}

                /***************************************************/

        ///trace                                                                                       | Returns the trace of the matrix, computed as T (meaning that rounding error and overflow may occur). Throws an exception (std::invalid_argument) if the matrix is not square.
        T trace() const{

            if(!isSquare()){throw std::invalid_argument("Matrix is not square.");}

            T accumulator = T(0);


            for(unsigned long long index = 0 ; index < size().at(0) ; index++){
                accumulator += T(at(index , index));
            }

            return accumulator;

        }

        ///sum                                                                                         | Returns the sum of all elements of the matrix, as T (meaning that overflow may occur).
        T sum() const{

            if(hasEmptyData()){throw std::invalid_argument("Matrix is empty or has empty columns.");}

            T accumulator = T(0);

            for(unsigned long long index_line = 0 ; index_line < size().at(0) ; index_line++){
                for(unsigned long long index_column = 0 ; index_column < size().at(1) ; index_column++){
                    accumulator += at(index_line , index_column);
                }
            }

            return accumulator;

        }

        ///mean                                                                                        | Returns the mean value of all elements of the matrix, as T (meaning that rounding error and overflow may occur). It is computed as sum()/(size.at(0)*size.at(1))
        T mean() const{return average();}

        ///average                                                                                     | Convinience function that returns mean().
        T average() const{
            return sum() / (size().at(0)*size().at(1));
        }

        ///det                                                                                         | Returns the determinant of the matrix. Throws an exception (std::invalid_argument) is the matrix is not square.
        T det() const{

            if(hasEmptyData()){throw std::invalid_argument("Matrix is empty or has empty columns.");}

            if(!isSquare()){throw std::invalid_argument("Matrix is not square.");}

            if(size().at(0) == 1 && size().at(1) == 1){return _data.at(0).at(0);}
            if(size().at(0) == 2 && size().at(1) == 2){return _data.at(0).at(0) * _data.at(1).at(1) - _data.at(0).at(1)*_data.at(1).at(0);}


            T determinant = T(0);

            for(unsigned long long row_index = 0 ; row_index < size().at(1) ; row_index++){

                std::vector<std::vector<T>> temp_data;

                for(unsigned long long i = 1 ; i < size().at(0) ; i++){

                    std::vector<T> temp_row;

                    for(unsigned long long j = 0 ; j < size().at(1)  ; j++){

                        if(j != row_index){temp_row.push_back(at(i , j));}

                    }

                    if(!temp_row.empty()){temp_data.push_back(temp_row);}

                }

                determinant += at(0 , row_index) * std::pow(-1 , row_index) * Matrix(temp_data).det();


            }

            return determinant;

        }

        ///comatrix                                                                                    | Returns the cofactor matrix. Convinience function that returns cofactormatrix().
        Matrix comatrix() const {return cofactormatrix();}

         //TODO
        ///cofactormatrix                                                                              | Returns the cofactor matrix. (WIP) ********************************************
        Matrix cofactormatrix() const {

            return Matrix(1,1);


        }




        ///transpose                                                                                   | Returns the transpose of the matrix.
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

        ///fill                                                                                        | Resizes the matrix to [size ; size] and fills it with value.
        void fill(const unsigned long long &size , const T &value){

            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }



        }

        ///fill                                                                                        | Resizes the matrix to [length ; width] and fills it with value.
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

        ///zeroes                                                                                      | Resizes the matrix to [size ; size] and fills it with 0.
        void zeroes(const unsigned long long &size){

            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }

        ///zeroes                                                                                      | Resizes the matrix to [length ; width] and fills it with 0.
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
        ///ones                                                                                        | Resizes the matrix to [size ; size] and fills it with 1.
        void ones(const unsigned long long &size){
            _data.clear();
            _data.reserve(size);
            for(unsigned long long line = 0 ; line < size ; line++){
                std::vector<T> line_data(size);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }

        ///ones                                                                                        | Resizes the matrix to [length ; width] and fills it with 1.
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

        ///ones                                                                                         | Resizes the matrix to [size ; size] fills it to be the identity matrix.
        void eye(const unsigned long long &size){

            zeroes(size);

            for(unsigned long long line = 0 ; line < size ; line++){
                replace(line , line , 1);
            }

        }




                /***************************************************/

        ///operator=                                                                                 | Assignement operator.
        void operator=(const Matrix &arg){_data = arg._data;}

        ///operator=                                                                                 | Assignement operator. Changes the data to the one from arg if possible, throws std::invalid_argument otherwise.
        void operator=(const std::vector<std::vector<T>> &arg){

            const unsigned long long column_length = arg.at(0).size();

            for(auto &line:arg){
                if(line.size() != column_length){throw std::invalid_argument("Cannot construct a matrix with irregular column size.");}
            }

            _data = arg;

        }

        ///operator*                                                                                 | Multiplies two matrices using the usual matrix product definition.
        Matrix operator* (const Matrix &arg) const{

            Matrix result(_data.size() , arg._data.at(0).size());

            for(unsigned long long index_line = 0 ; index_line < _data.size() ; index_line++){

                for(unsigned long long index_column = 0 ; index_column < arg._data.at(0).size() ; index_column++){
                    T value = T(0);
                    for(unsigned long long index_sum = 0 ; index_sum < _data.at(0).size() ; index_sum++){
                        value += T(_data.at(index_line).at(index_sum)) * T(arg._data.at(index_sum).at(index_column));
                    }
                    result.replace(index_line , index_column , value);
                }

            }

            return result;

        }

        ///operator*                                                                                 | Multiplies all elements of the matrix by arg.
        Matrix operator* (const T &arg) const{

            Matrix result(_data);

            if(arg == 1){return result;}

            for(auto &line:result._data){

            std::transform(line.begin(), line.end(), line.begin(),
                           std::bind(std::multiplies<T>(), std::placeholders::_1, arg));

            }

            return result;

        }



        //TODO
        //Matrix& operator *=(const Matrix &arg){return (this*arg);}
        //Matrix& operator *=(const T arg){return (this*arg);}

        ///operator+                                                                                 | Adds T two matrices.
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

        ///operator+                                                                                 | Adds arg to all elements. May be overrided for other purposes.
        virtual Matrix operator+ (const T &arg) const{

            Matrix result(_data);

            for(auto &line:result._data){
                for(auto &item:line){item+=arg;}
            }

            return result;

        }

        //TODO
        //Matrix& operator +=(const Matrix &arg){}
        //Matrix& operator +=(const T &arg){}

        ///operator-                                                                                 | Substracts two matrices.
        Matrix operator- (const Matrix &arg) const {return Matrix(_data) + arg*(-1.);}
        ///operator-                                                                                 | Substracts arg to all elements of the matrix. May be overrided for other purposes.
        virtual Matrix operator- (const T &arg) const {return Matrix(_data) + arg*(-1.);}

        //TODO
        //Matrix& operator -= (const Matrix &arg){}
        //Matrix& operator -= (const Matrix &arg){}

        ///operator!                                                                                 | Returns the inverse of the matrix, or an empty one if not inversible.
        Matrix operator! () const{                   //Inverse of matrix

            const T determinant = det();
            if(determinant == 0){
                std::cerr << "Matrix is not inversible. Returned an empty matrix.\n";
                return Matrix<T>(0,0);
            }

            return (1/det())*Matrix(_data).cofactormatrix().transpose();

        }

        ///operator^                                                                                 | Returns the matrix to the specified power (usual matrix product is used).
        Matrix operator^ (const unsigned &arg) const{ //Power operator

            Matrix output(_data);
            if(arg <= 1){return output;}


            for(unsigned power = 1 ; power < arg ; power++){output = output * Matrix(_data);}

            return output;
        }



        bool operator== (const Matrix &arg) const{return _data == arg._data;}
        bool operator!= (const Matrix &arg) const{return !(this == arg);}


    protected:


};//class Matrix

} //namespace ste
#endif // MATRIX_H
