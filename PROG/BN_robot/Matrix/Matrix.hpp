#ifndef MATRIX_H
#define MATRIX_H

//#define USE_GPU


///TODO :
///     * bool CUDA_setup()
///     * T CUDA_trace(...)
///     * T CUDA_sum(...)
///     * T CUDA_det(...)
///     * const T** CUDA_cofactormatrix
///     * const T** CUDA_transpose
///     * const T** CUDA_hadamard
///     * const T** CUDA_mult_MAT
///     * const T** CUDA_mult_T
///     * const T** CUDA_add_MAT
///     * const T** CUDA_add_T
///

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

    • size              | Returns the size of the matrix, as const std::vector<uint64_t>.
    • columns           | Returns the number of columns of the matrix.
    • rows              | Returns the number of rows of the matrix.
    • lines             | Alias for row().

    • isRow             | Returns true if the matrix is row, false otherwise.
    • isLine            | Alias for isRow.
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

    • push_back         | Alias for add.
    • push_back_line    | Alias for add_line.
    • push_back_column  | Alias for add_column.

    • remove            | Removes either a line or a column at the position specified.
    • remove_line       | Convience function to remove a line at a specified position.
    • remove_column     | Convience function to remove a column at a specified position.

    • insert            | Inserts either a line or a column at the position specified.
    • insert_line       | Convience function to insert a line at a specified position.
    • insert column     | Convience function to insert a column at a specified position.

    • swap              | Swaps two lines or two columns at the positions specified.
    • swap_line         | Convience function to swap two lines at a specified positions.
    • swap_column       | Convience function to swap two columns at a specified positions.

    • toVector          | Converts the matrix to std::vector<std::vector<T>>.
    • toVector1D        | Converts the matrix to std::vector<T>
    • toArray2D         | Converts the matrix to T** (T[][]) ********************************************* REMOVED
    • toArray1D         | Converts the matrix to T* (T[]) ************************************************ REMOVED

    • print             | Prints the contents of the matrix in stdout.
    • print_size        | Prints the size of the matrix in stdout.

    • begin_line        | Convinience function that returns 0, to provide syntax as close as the one relative to std::algorithm as possible.
    • begin_column      | Convinience function that returns 0, to provide syntax as close as the one relative to std::algorithm as possible.
    • end_line          | Convinience function that returns the number of lines, to provide syntax as close as the one relative to std::algorithm as possible.
    • end column        | Convinience function that returns the number of columns, to provide syntax as close as the one relative to std::algorithm as possible.

    • trace             | Returns the trace of the matrix, computed as T (meaning that rounding error and overflow may occur). Throws an exception (std::invalid_argument) if the matrix is not square.
    • sum               | Returns the sum of all elements of the matrix, as T (meaning that overflow may occur).
    • mean              | Returns the mean value of all elements of the matrix, as T (meaning that rounding error and overflow may occur). It is computed as sum()/(rows()*columns()).
    • average           | Alias for mean().
    • det               | Returns the determinant of the matrix. Throws an exception (std::invalid_argument) is the matrix is not square.
    • cofactor          | Returns the cofactor of the specified line and column or linear index. Throws an exception if one of them is outside the matrix.
    • comatrix          | Returns the cofactor matrix. Convinience function that returns cofactormatrix().
    • cofactormatrix    | Returns the cofactor matrix.
    • transpose         | Returns the transpose of the matrix.

    • hadamard          | Returns the Hadamard product of two matrices. Throws an exception if the sizes do not match.

    • fill              | Resizes the matrix as specified in argument and fills it with the value chosen.
    • zeroes            | Resizes the matrix as specified in argument and fills it with 0.
    • ones              | Resizes the matrix as specified in argument and fills it with the 1.
    • eye               | Creates the identity matrix of the size specified in argument.

    • operator=         | Assignment operator. Supports assignments from std::vector<std::vector<T>> and from other Matrix.
    • operator+         | Computes the addition of a Matrix with another (term by term). Also supports the addition of value in T. In that case, it is the same as adding a Matrix of the same size holding only the same value.
    • operator*         | Computes the usual matrix product of two matrices or multiplies term by term by the T speficied.
    • operator-         | Computes the substraction of two Matrix. Its implementation requires T to be able to be multiplied by (-1.).
    • operator!         | Returns the inverse of the matrix.
    • operator^         | Returns the matrix two the power specifed after the ^ (ex: a^2 returns a*a);
    • operator==        | Equality operator. Returns true only if all elements are identical and at the same position.
    • operator!=        | Returns the opposite of the result given by operator==.

*/


#ifdef USE_GPU
#include "CUDA_src/CUDA_setup.h"
#include "CUDA_src/CUDA_matrix_operators.h"
#endif

#include <algorithm>

#include <cmath>

#include <functional>

#include <iostream>
#include <iterator>

#include <stdint.h>
#include <stdexcept>
#include <string>

#include <vector>

namespace ste{

template<class T>
class Matrix{

    protected:

        std::vector<std::vector<T>> _data;

    public:

        /***************************************************/

        enum class Orientation{
            LINE,
            COLUMN
        };

        /***************************************************/

        ///Constructor                                                                               | Inits the matrix with empty vectors.
        Matrix(const uint64_t &lines , const uint64_t&columns){

            #ifdef USE_GPU
            if(!CUDA_setup()){throw std::runtime_error("ste::Matrix::Matrix\nUnable to setup GPU.");}
            #endif


            _data.reserve(lines);
            for(uint64_t index = 0 ; index < lines ; index++){
                _data.push_back(std::vector<T>(columns));
            }

        }
        ///Constructor                                                                               | Creates an empty square matrix of dimension size.
        Matrix(const uint64_t &size) : Matrix(size , size){}

        ///Constructor                                                                               | Inits the data of the matrix using a std::vector<std::vector<T>>
        Matrix(const std::vector<std::vector<T>> &data){

            #ifdef USE_GPU
            if(!CUDA_setup()){throw std::runtime_error("ste::Matrix::Matrix\nUnable to setup GPU.");}
            #endif

            const uint64_t column_length = data.at(0).size();

            for(auto &line:data){
                if(line.size() != column_length){throw std::invalid_argument("ste::Matrix::Matrix\nCannot construct a matrix with irregular column size.");}
            }

            _data = data;
        }

    Matrix(const std::vector<T> &data , const uint64_t &rows , const uint64_t&columns){

        #ifdef USE_GPU
        if(!CUDA_setup()){throw std::runtime_error("ste::Matrix::Matrix\nUnable to setup GPU.");}
        #endif

        _data.reserve(rows);
        for(uint64_t row = 0 ; row < rows ; row++){

           std::vector<T> column_data;
           column_data.resize(columns);

            for(uint64_t column = 0 ; column < columns ; column++){
                column_data[column] = (data[row*columns + column]);
            }

            _data.push_back(column_data);

        }

    }

        /*Matrix(const T** array, const uint64_t &lines , const uint64_t &columns){


        #ifdef USE_GPU
        if(!CUDA_setup()){{throw std::invalid_argument("ste::Matrix::Matrix\nUnable to setup GPU.");}
        #endif

            _data.reserve(lines);

            for(uint64_t line = 0 ; line < lines ; line ++){

                std::vector<T> column_data;
                column_data.reserve(columns);
                for(uint64_t column = 0 ; column < columns ; column ++){
                    column_data.push_back(array[column][line]);
                }
                _data.push_back(column_data);

                std::cout << std::endl;
            }

        }*/

       /* Matrix(const T* array, const uint64_t &rows , const uint64_t &columns){


        #ifdef USE_GPU
        if(!CUDA_setup()){{throw std::invalid_argument("ste::Matrix::Matrix\nUnable to setup GPU.");}
        #endif

            //_data.resize(rows);

            for(uint64_t row = 0 ; row < rows ; row++){

               std::vector<T> column_data;
               column_data.resize(columns);

                for(uint64_t column = 0 ; column < columns ; column++){

                    //std::cout << "index: " << row*columns + column << " ";
                   // std::cout << "data: " << array[row*columns + column] << " ";
                    column_data[column] = (array[row*columns + column]);
                    //column_data.push_back(array[row*columns + column]);

                }
                //_data[row] = column_data;
               // std::cout << std::endl;
                _data.push_back(column_data);

            }

        }*/

        /***************************************************/
        ///size                                                                                      | Returns the size of the matrix as const std::vector<uint64_t>.
        const std::vector<uint64_t> size() const {
            if(_data.empty()){return {0 , 0};}

            if(_data.at(0).empty()){return std::vector<uint64_t>({_data.size() , 0});}

            return std::vector<uint64_t>({_data.size() , _data.at(0).size()});

        }

        ///columns                                                                                   | Returns the number of columns of the matrix.
        uint64_t columns() const{return _data.empty() ? 0 : _data.at(0).size();}

        ///rows                                                                                      | Returns the number of rows of the matrix.
        uint64_t rows() const{return _data.size();}

        ///lines                                                                                     | Alias for row().
        uint64_t lines() const{return rows();}

        /***************************************************/

        /// isLine                                                                                   | Returns true if the matrix is a line, false otherwise.
        bool isLine() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (rows() == 1);

        }

        /// isRow                                                                                    | Returns true if the matrix is a line, false otherwise.
        bool isRow() const {return isLine();}

        ///isColumn                                                                                  | Returns true if the matrix is a column, false otherwise.
        bool isColumn() const{

            if(_data.empty()){return false;}

            if(_data.at(0).empty()){return false;}


            return (columns() == 1);
        }

        ///isSquare                                                                                  | Returns true if the matrix is square, false otherwise.
        bool isSquare() const{return (rows() == columns());}

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
        T& at(const uint64_t &line , const uint64_t &column){

            if(empty()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

            if(line >= rows() || column >= columns()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

            return _data.at(line).at(column);

        }

        ///at                                                                                        | Returns by reference the element at the linear index specified.
        T& at(const uint64_t &index){

         if(index >= rows()*columns()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

         return at(index / columns() , index % columns());

        }

        ///at                                                                                        | Returns the value of the element at (line , column).
        T at(const uint64_t &line , const uint64_t &column) const{

            if(empty()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

            if(line >= rows() || column >= columns()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

            return _data.at(line).at(column);

        }

        ///at                                                                                        | Returns the value of the element at the linear index specified.
        T at(const uint64_t &index) const{

         if(index >= rows()*columns()){throw std::out_of_range("ste::Matrix::at\nIndex out of range.");}

         return at(index / columns() , index % columns());


        }

        ///lineAt                                                                                    | Returns by reference the line at the specified index.
        std::vector<T>& lineAt(const uint64_t &index){

            if(empty()){throw std::out_of_range("ste::Matrix::lineAt\nIndex out of range.");}
            if(index >= rows()){throw std::out_of_range("ste::Matrix::lineAt\nIndex out of range.");}
            return _data.at(index);

        }

        ///lineAt                                                                                    | Returns the value of the line at the specified index.
        std::vector<T> lineAt(const uint64_t &index) const{

            if(empty()){throw std::out_of_range("ste::Matrix::lineAt\nIndex out of range.");}
            if(index >= rows()){throw std::out_of_range("ste::Matrix::lineAt\nIndex out of range.");}
            return _data.at(index);

        }

        ///columnAt                                                                                  | Returns the value of the column at the specified index.
        std::vector<T>& columnAt(const uint64_t &index) const {

            if(hasEmptyData()){throw std::out_of_range("ste::Matrix::columnAt\nIndex out of range.");}
            if(index >= columns()){throw std::out_of_range("ste::Matrix::columnAt\nIndex out of range.");}

            std::vector<T> result;
            result.reserve(columns());

            for(auto &line:_data){
                result.push_back(line.at(index));
            }

            return result;
        }



                /***************************************************/


        ///replace                                                                                   | Replaces the element at (line , column) by value.
        void replace(const uint64_t &line , const unsigned &column , const T &value){_data.at(line).at(column) = value;}

        ///replace                                                                                   | Replaces the element at index (linear index) by value.
        void replace(const uint64_t &index, const T &value){at(index) = value;}

        ///replace                                                                                   | Replaces the line or column specified in argument by value.
        void replace(const uint64_t &value_index ,const Orientation &orientation , const std::vector<T> &value){


            switch(orientation){

                case(Orientation::LINE):{
                    if(value_index >= rows()){throw std::invalid_argument("ste::Matrix::replace\nCannot replace a line outside the matrix.");}
                    if(value.size() != columns()){throw std::invalid_argument("ste::Matrix::replace\nCannot replace a line by another one with different length.");}

                    _data.at(value_index) = value;
                    break;
                }
                case(Orientation::COLUMN):{
                    if(value_index  >=  columns()){throw std::invalid_argument("ste::Matrix::replace\nCannot replace a column outside the matrix.");}
                    if(value.size() != rows()){throw std::invalid_argument("ste::Matrix::replace\nCannot replace a column by another one with different length.");}

                        for(uint64_t index_line = 0 ; index_line < rows() ; index_line++){
                            replace(index_line , value_index , value.at(index_line));
                        }


                    break;
                }

                default:{throw std::runtime_error("ste::Matrix::replace\nInvalid orientation provided to add.");}

            }


        }

        ///replace_line                                                                              | Replaces the line specified in argument by value.
        void replace_line(const uint64_t &value_index , const std::vector<T> &value){replace(value_index , Orientation::LINE , value);}

        ///replace_column                                                                            | Replaces the column specified in argument by value.
        void replace_column(const uint64_t &value_index , const std::vector<T> &value){replace(value_index , Orientation::COLUMN , value);}

        /// replace                                                                                  | Replaces the elements in the range specified by value.
        void replace(const uint64_t &line_begin, const uint64_t &line_end ,
                     const uint64_t &column_begin, const uint64_t &column_end,
                     const T &value){


                if(line_begin < begin_line()     ||
                   line_end > end_line()         ||
                   column_begin < begin_column() ||
                   column_end > end_column()){throw std::invalid_argument("ste::Matrix::replace\nCannot replace an element outside the matrix.");}


                for(uint64_t line = line_begin ; line < line_end ; line++){

                    for(uint64_t column = column_begin ; column < column_end ; column++){

                        _data.at(line).at(column) = value;

                    }

                }

        }


                  /***************************************************/

        ///add                                                                                       | Adds either a column or a line at the end of the matrix containing data according to orientation.
        void add(const std::vector<T> &data , const Orientation &orientation){

            switch(orientation){

                case(Orientation::LINE):{
                    if(data.size() != columns()){throw std::invalid_argument("ste::Matrix::add\nSizes must match when appending a new line to a matrix.");}

                    _data.push_back(data);
                    break;
                    }
                case(Orientation::COLUMN):{
                    if(data.size() != rows()){throw std::invalid_argument("ste::Matrix::add\nSizes must match when appending a new line to a matrix.");}

                    for(uint64_t index = 0 ; index < rows() ; index++){
                        _data.at(index).push_back(data.at(index));
                    }
                break;
                }

                default:{throw std::runtime_error("ste::Matrix::add\nInvalid orientation provided to add.");}

            }

        }

        ///add_line                                                                                  | Adds a line containing data at the end of the matrix.
        void add_line(const std::vector<T> &data){add(data , Orientation::LINE);}

        ///add_column                                                                                | Adds a column containing data at the end of the matrix.
        void add_column(const std::vector<T> &data){add(data , Orientation::COLUMN);}


                  /***************************************************/

        ///push_back                                                                                  | Alias for add.
        void push_back(const std::vector<T> &data , const Orientation &orientation){add(data , orientation);}

        ///push_back_line                                                                             | Alias for add_line.
        void push_back_line(const std::vector<T> &data){add_line(data);}

        ///push_back_column                                                                           | Alias for add_colum.
        void push_back_column(const std::vector<T> &data){add_column(data);}


                  /***************************************************/

        ///remove(const std::vector<T> &data , const Orientation &orientation)                       | Removes either a column or a line at the specified index containing data according to orientation.
        void remove(const uint64_t &element_index , const Orientation &orientation){

            switch(orientation){

                case(Orientation::LINE):{

                if(element_index >= rows()){throw std::invalid_argument("ste::Matrix::remove\nCannot remove a line outside the matrix.");}

                _data.erase(_data.begin()+element_index);

                break;
                }
                case(Orientation::COLUMN):{

                if(element_index >= columns()){throw std::invalid_argument("ste::Matrix::remove\nCannot remove a column outside the matrix.");}
                for(uint64_t index = 0 ; index < rows() ; index++){
                    _data.at(index).erase(_data.at(index).begin()+element_index);
                }
                break;
                }

                default:{throw std::runtime_error("ste::Matrix::remove\nInvalid orientation provided to remove.");}

            }


        }

        ///remove_line                                                                               | Removes a line at the specified index.
        void remove_line(const uint64_t &index){remove(index, Orientation::LINE);}

        ///remove_column                                                                             | Removes a column at the specified index.
        void remove_column(const uint64_t &index){remove(index, Orientation::COLUMN);}


                  /***************************************************/
        ///insert                                                                                    | Inserts a column or a line containing data at the specified index.
        void insert(const uint64_t &element_index , const Orientation &orientation , const std::vector<T> &data){

            switch(orientation){

                case(Orientation::LINE):{
                    if(data.size() != columns()){throw std::invalid_argument("ste::Matrix::insert\nCannot insert a line which does not have the same length as the others.");}
                    if(element_index>rows()){insert(rows() , orientation , data); return;}

                    _data.insert(_data.begin() + element_index , data);

                break;
                }
                case(Orientation::COLUMN):{
                    if(data.size() != rows()){throw std::invalid_argument("ste::Matrix::insert\nCannot insert a column which does not have the same length as the others.");}
                    if(element_index>columns()){insert(columns() , orientation , data); return;}

                    for(uint64_t index_line = 0 ; index_line < rows() ; index_line++){
                        _data.at(index_line).insert(_data.at(index_line).begin() +  element_index , data.at(index_line));
                    }



                break;
                }

                default:{throw std::runtime_error("ste::Matrix::insert\nInvalid orientation provided to insert.");}

            }


        }
        ///insert_line                                                                               | Inserts a line at index containing data.
        void insert_line(const uint64_t &index , const std::vector<T> &data){insert(index, Orientation::LINE , data);}

        ///insert_column                                                                             | Inserts a column at index containing data.
        void insert_column(const uint64_t &index , const std::vector<T> &data){insert(index, Orientation::COLUMN , data);}

        /***************************************************/
        ///swap                                                                                      | Swaps two lines or two columns at the positions specified.
        void swap(const uint64_t &element_1 , const uint64_t element_2 ,const Orientation &orientation){

            switch(orientation){

                case(Orientation::LINE):{

                    if(element_1 >= rows() || element_2 >= rows()){throw std::invalid_argument("ste::Matrix::swap\nCannot swap lines outside the matrix.");}

                    std::vector<T> temp = _data.at(element_1);
                    replace_line(element_1 , _data.at(element_2));
                    replace_line(element_2 , temp);

                break;
                }
                case(Orientation::COLUMN):{

                    if(element_1 >= columns() || element_2 >= columns()){throw std::invalid_argument("ste::Matrix::swap\nCannot swap columns outside the matrix.");}

                    std::vector<T> temp;
                    temp.reserve(columns());

                    for(auto &line:_data){
                        temp.push_back(line.at(element_1));
                    }

                    replace_column(element_1 , _data.at(element_2));
                    replace_column(element_2 , temp);


                break;
                }

                default:{throw std::runtime_error("ste::Matrix::swap\nInvalid orientation provided to insert.");}

            }


        }

        ///swap_lines                                                                                | Convience function to swap two lines at a specified positions.
        void swap_lines(const uint64_t &element_1 , const uint64_t element_2){swap(element_1, element_2 ,Orientation::LINE);}

        ///swap_columns                                                                              | Convience function to swap two columns at a specified positions.
        void swap_columns(const uint64_t &element_1 , const uint64_t element_2){swap(element_1, element_2 ,Orientation::COLUMN);}

        /***************************************************/

        ///toVector                                                                                  | Converts the matrix to std::vector<std::vector<T>>.
        std::vector<std::vector<T>> toVector() const{return _data;}

        ///toVector1D
        const std::vector<T> toVector1D() const{


            std::vector<T> result;
            result.reserve(rows() * columns());

            for(auto &column : _data){

                for(auto &value : column){

                    result.push_back(value);
                }

            }

            return result;

        }

        std::vector<T> toVector1D(){

            std::vector<T> result;
            result.reserve(rows() * columns());

            for(auto &column : _data){

                for(auto &value : column){

                    result.push_back(value);
                }

            }

            return result;

        }

        /* std::vector<T> toVector1D_CM() const{

            std::vector<T> result;
            result.resize(rows() * columns());

                for(uint64_t column = 0 ; column < columns() ; column++){

                    for(uint64_t row = 0 ; row < rows() ; row++){
                        result[row * columns() + column] = at(row , column);
                    }

                }

            return result;

        }*/



        ///print                                                                                     | Prints the contents of the matrix in stdout.
        void print() const{

            std::cout << "[ ";
            for(auto &line:_data){
                std::cout << "[ ";
                std::copy (line.begin(), line.end(), std::ostream_iterator<T>(std::cout, " "));
                std::cout << "] ";
            }
            std::cout << "]" << std::endl; //Buffer flushed to prevent user misusage

        }

        ///print_size                                                                                | Prints the size of the matrix in stdout.
        void print_size() const{

            std::cout << "[" << rows() << " ; "<< columns() << "]" << std::endl; //Buffer flushed to prevent user misusage

        }

        ///begin_line | begin_column | begin_line | end_column                                       | Convinience functions that returns the beginning or the end of a line / a column. Provided to obtain syntax as close as std::algorithm functions as possible.
        uint64_t begin_line() const{return 0;}
        uint64_t begin_column() const{return 0;}

        uint64_t end_line() const{return _data.size();}
        uint64_t end_column() const{return _data.at(0).size();}

                /***************************************************/

        ///trace                                                                                      | Returns the trace of the matrix, computed as T (meaning that rounding error and overflow may occur). Throws an exception (std::invalid_argument) if the matrix is not square.
        T trace() const{


            //Not enough calculations to justify GPU here
            if(!isSquare()){throw std::invalid_argument("ste::Matrix::trace\nMatrix is not square.");}

            T accumulator = T(0);

            for(uint64_t index = 0 ; index < rows() ; index++){
                accumulator += T(at(index , index));
            }

            return accumulator;


        }

        ///sum                                                                                        | Returns the sum of all elements of the matrix, as T (meaning that overflow may occur).
        T sum() const{

            if(hasEmptyData()){throw std::invalid_argument("ste::Matrix::sum\nMatrix is empty or has empty columns.");}

            #ifdef USE_GPU
            return CUDA_sum(toVector1D());
            #else
            T accumulator = T(0);

            for(uint64_t index_line = 0 ; index_line < rows() ; index_line++){
                for(uint64_t index_column = 0 ; index_column < columns() ; index_column++){
                    accumulator += at(index_line , index_column);
                }
            }

            return accumulator;
            #endif


        }

        ///mean                                                                                       | Returns the mean value of all elements of the matrix, as T (meaning that rounding error and overflow may occur). It is computed as sum()/(size.at(0)*size.at(1))
        T mean() const{return average();}

        ///average                                                                                    | Convinience function that returns mean().
        T average() const{
            return sum() / (rows()*columns());
        }

        ///det                                                                                        | Returns the determinant of the matrix. Throws an exception (std::invalid_argument) is the matrix is not square.
        T det() const{

            if(hasEmptyData()){throw std::invalid_argument("ste::Matrix::det\nMatrix is empty or has empty columns.");}

            if(!isSquare()){throw std::invalid_argument("ste::Matrix::det\nMatrix is not square.");}

            if(rows() == 1 && columns() == 1){return at(0 , 0);}
            if(rows() == 2 && columns() == 2){return at(0 , 0) * at(1 , 1) - at(0 , 1)*at(1 , 0);}


            //#ifdef USE_GPU
            //return T(CUDA_det(toVector1D() , rows() , columns()));
            //#else
            T determinant = T(0);

            for(uint64_t row_index = 0 ; row_index < columns() ; row_index++){

                std::vector<std::vector<T>> temp_data;

                for(uint64_t i = 1 ; i < rows() ; i++){

                    std::vector<T> temp_row;

                    for(uint64_t j = 0 ; j < columns()  ; j++){

                        if(j != row_index){temp_row.push_back(at(i , j));}

                    }

                    if(!temp_row.empty()){temp_data.push_back(temp_row);}

                }

                determinant += at(0 , row_index) * std::pow(-1 , row_index) * Matrix(temp_data).det();


            }

            return determinant;
            //#endif



        }

        ///cofactor                                                                                   | Returns the cofactor of the specified line and column.
        T cofactor(uint64_t line , uint64_t column) const{

            if(line >= rows()){throw std::invalid_argument("ste::Matrix::cofactor\nLine is outside the matrix.");}
            if(column >= columns()){throw std::invalid_argument("ste::Matrix::cofactor\nColumn is outside the matrix.");}

            Matrix temp(_data);
            temp.remove_line(line);
            temp.remove_column(column);

            return std::pow(T(-1) , line+column) * temp.det();


        }


        ///cofactor                                                                                   | Returns the cofactor of the specified linear index.
        T cofactor(uint64_t index) const{return cofactor(index / columns() , index % columns());}


        ///comatrix                                                                                   | Returns the cofactor matrix. Convinience function that returns cofactormatrix().
        Matrix comatrix() const {return cofactormatrix();}


        ///cofactormatrix                                                                             | Returns the cofactor matrix. (WIP) ********************************************
        Matrix cofactormatrix() const {


            //#ifdef USE_GPU
            //return Matrix(CUDA_cofactormatrix(toVector1D() , rows() , columns()) , rows() , columns());
            //#else
            Matrix result(_data);

            for(uint64_t index = 0 ; index < rows()*columns() ; index++){

                Matrix temp(_data);

                T value = temp.cofactor(index);
                result.replace(index , value);
                //std::cout << "index : " << index << " value: " << value << std::endl;

            }


            return result;
            //#endif




        }






        ///transpose                                                                                   | Returns the transpose of the matrix.
        Matrix transpose() const{


        //#ifdef USE_GPU
        //return Matrix(CUDA_transpose(toVector1D(), rows() , columns()) , rows() , columns());
        //#else

            Matrix result(columns() , rows());

            for(uint64_t row = 0 ; row < rows() ; row++){
                for(uint64_t column = 0 ; column < columns() ; column++){
                    result.replace(column , row , at(row  , column));
                }
            }
            return result;

       // #endif
        }


        ///hadamard                                                                                    | Returns the Hadamard product of two matrices. Throws an exception if the sizes do not match.
        Matrix hadamard(const Matrix &arg) const{

            if(size() != arg.size()){throw std::invalid_argument("Matrix::hadamard\nSizes of the two matrices must match."
                                                                 + std::string("First argument length: [") + std::to_string(rows()) + " ; "+ std::to_string(columns()) + "]\n"
                                                                 + std::string("Second argument length: [") + std::to_string(arg.rows()) + " ; "+ std::to_string(arg.columns()) + "]\n");}

           //#ifdef USE_GPU
           /*return Matrix(CUDA_hadamard(
                             toArray()     , rows()     , columns(),
                             arg.toArray() , arg.rows() , arg.columns()
                             ) ,
                         rows() , columns());*/
           //#else


            Matrix result(_data);


            for(uint64_t item = 0 ; item < rows() * columns() ; item++){
                result.at(item) = at(item) * arg.at(item);
            }

            return result;
           //#endif




        }

        static Matrix hadamard(const Matrix &arg1 , const Matrix &arg2){return arg1.hadamard(arg2);}

                /***************************************************/

        ///fill                                                                                        | Resizes the matrix to [size ; size] and fills it with value.
        void fill(const uint64_t &size , const T &value){
            fill(size , size , value);
        }

        ///fill                                                                                        | Resizes the matrix to [length ; width] and fills it with value.
        void fill(const uint64_t &length , const uint64_t &width , const T &value){

            _data.clear();
            _data.reserve(length);
            for(uint64_t line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(value));
                _data.push_back(line_data);
            }


        }


                  /***************************************************/

        ///zeroes                                                                                      | Resizes the matrix to [size ; size] and fills it with 0.
        void zeroes(const uint64_t &size){
            zeroes(size , size);
        }

        ///zeroes                                                                                      | Resizes the matrix to [length ; width] and fills it with 0.
        void zeroes(const uint64_t &length , const uint64_t &width){

           _data.clear();
           _data.reserve(length);
            for(uint64_t line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(0));
                _data.push_back(line_data);
            }
        }


                    /***************************************************/
        ///ones                                                                                        | Resizes the matrix to [size ; size] and fills it with 1.
        void ones(const uint64_t &size){
            ones(size , size);
        }

        ///ones                                                                                        | Resizes the matrix to [length ; width] and fills it with 1.
        void ones(const uint64_t &length , const uint64_t &width){

            _data.clear();
            _data.reserve(length);
            for(uint64_t line = 0 ; line < length ; line++){
                std::vector<T> line_data(width);
                std::fill(line_data.begin() , line_data.end() , T(1));
                _data.push_back(line_data);
            }
        }



                 /***************************************************/

        ///ones                                                                                         | Resizes the matrix to [size ; size] fills it to be the identity matrix.
        void eye(const uint64_t &size){

            zeroes(size);

            for(uint64_t line = 0 ; line < size ; line++){
                replace(line , line , 1);
            }

        }




                /***************************************************/

        ///operator=                                                                                 | Assignement operator.
        virtual void operator=(const Matrix &arg){_data = arg._data;}

        ///operator=                                                                                 | Assignement operator. Changes the data to the one from arg if possible, throws std::invalid_argument otherwise.
        virtual void operator=(const std::vector<std::vector<T>> &arg){

            const uint64_t column_length = arg.at(0).size();

            for(auto &line:arg){
                if(line.size() != column_length){throw std::invalid_argument("ste::Matrix::operator=\nCannot construct a matrix with irregular column size.");}
            }

            _data = arg;

        }

        ///operator*                                                                                 | Multiplies two matrices using the usual matrix product definition.
        virtual Matrix operator* (const Matrix &arg) const{

                        if(rows() != arg.columns()){throw std::invalid_argument("ste::Matrix::operator*\nDimension mismatch.\nFirst argument size: [ "
                                                                    + std::to_string(rows()) + " ; " + std::to_string(columns()) + "]\n"
                                                                    + "Second argument size: [ " + std::to_string(arg.rows()) + " ; " + std::to_string(arg.columns()) +"].") ;}

            #ifdef USE_GPU

            return Matrix(
                          CUDA_mult_MAT(toVector1D() , rows() , columns () , arg.toVector1D() , arg.rows() , arg.columns()) ,
                          rows() ,
                          arg.columns());

            #else

            Matrix result(rows() , arg.columns());

            for(uint64_t index_line = 0 ; index_line < rows() ; index_line++){

                for(uint64_t index_column = 0 ; index_column < arg.columns() ; index_column++){
                    T value = T(0);
                    for(uint64_t index_sum = 0 ; index_sum < columns(); index_sum++){
                        value += T(_data.at(index_line).at(index_sum)) * T(arg._data.at(index_sum).at(index_column));
                    }
                    result.replace(index_line , index_column , value);
                }

            }

            return result;

            #endif


        }

        ///operator*                                                                                 | Multiplies all elements of the matrix by arg.
        virtual Matrix operator* (const T &arg) const{


//            #ifdef USE_GPU
//            /*
//                CUDA_mult_T(const ste::Matrix<float> &data_1  ,
//                        const float value ,
//                        std::vector<float> &result);*/


//            std::vector<T> result(rows() * columns());
//            CUDA_mult_T(this , arg , result);
//            return Matrix(result , rows() , columns());
//            #else
            Matrix result(_data);

            if(arg == 1){return result;}

            for(auto &line:result._data){

            std::transform(line.begin(), line.end(), line.begin(),
                           std::bind(std::multiplies<T>(), std::placeholders::_1, arg));

            }

            return result;
//            #endif

        }

        ///operator+                                                                                 | Adds T two matrices.
        virtual Matrix operator+ (const Matrix &arg) const{


            //#ifdef USE_GPU
            /*return Matrix(CUDA_add_MAT(toArray() , rows() , columns() , arg.toArray() , arg.rows() , arg.columns()),
                          rows() , columns());*/
           // #else
           Matrix result(_data);

            for(uint64_t index = 0 ; index < result.rows() ; index++){

                std::transform (result._data.at(index).begin(),
                                result._data.at(index).end(),
                                arg._data.at(index).begin(),
                                result._data.at(index).begin(), std::plus<T>());

            }

            return result;
            //#endif


        }

        ///operator+                                                                                 | Adds arg to all elements. May be overrided for other purposes.
        virtual Matrix operator+ (const T &arg) const{


            //#ifdef USE_GPU
            //return Matrix(CUDA_add_T(arg , toArray() , rows() , columns()) ,  rows() , columns());
            //#else

            Matrix result(_data);
            for(auto &line:result._data){
                for(auto &item:line){item+=arg;}
            }

            return result;
            //#endif

        }

        ///operator-                                                                                 | Substracts two matrices.
        virtual Matrix operator- (const Matrix &arg) const {return Matrix(_data) + arg*T(-1.);}
        ///operator-                                                                                 | Substracts arg to all elements of the matrix. May be overrided for other purposes.
        virtual Matrix operator- (const T &arg) const {return Matrix(_data) + arg*T(-1.);}

        ///operator!                                                                                 | Returns the inverse of the matrix, or an empty one if not inversible.
        Matrix operator! () const{                   //Inverse of matrix

            const T determinant = det();
            if(determinant == 0){
                std::cerr << "Matrix is not inversible. Returned an empty matrix.\n";
                return Matrix(0,0);
            }

            return Matrix(_data).cofactormatrix().transpose()*(1/determinant);

        }

        ///operator^                                                                                 | Returns the matrix to the specified power (usual matrix product is used).
        virtual Matrix operator^ (const long long int &arg) const{ //Power operator

            Matrix output(_data);

            if(arg < 0){

                output = !output;
                return output^(-arg);
            }

            if(arg == 1){return output;}

            for(long long int power = 1 ; power < arg ; power++){output = output * Matrix(_data);}

            return output;
        }


        ///operator==                                                                               | Equality operator. Returns true only if all elements are identical and at the same position.
        bool operator== (const Matrix &arg) const{return _data == arg._data;}

        ///operator!=                                                                               | Returns the opposite of the result given by operator==.
        bool operator!= (const Matrix &arg) const{return !(this == arg);}


};//class Matrix

} //namespace ste
#endif // MATRIX_H
