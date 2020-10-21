#include <iostream>

#include <Matrix.hpp>


//C++ 14 - Tested with MinGW on Windows 10

int main(){

   const ste::Matrix<double> a({{1 , 11 ,12} , {2 , 3 ,12} , {5 , 6 ,4}});
   std::cout << "a:\n";
   a.print();
   std::cout << "\n";
   std::cout << "a.size(): ";
   a.print_size();
   std::cout << "\n";

   ste::Matrix<double> b = ste::Matrix<double>({{1 , 2 , 3}});
   std::cout << "b:\n";
   b.print();
   std::cout << "\n";
   std::cout << "b.size(): ";
   b.print_size();
   std::cout << "\n";

   b = {{1 , 2 , 3} , {4 , 5 , 6}};
   std::cout << "b:\n";
   b.print();
   std::cout << "\n";
   std::cout << "b.size(): ";
   b.print_size();
   std::cout << "\n";

   const ste::Matrix<double> c = a^2;
   std::cout << "c:\n";
   c.print();
   std::cout << "\n";
   std::cout << "c.size(): ";
   c.print_size();
   std::cout << "\n";

   const ste::Matrix<double> d = b*a;
   std::cout << "d:\n";
   d.print();
   std::cout << "\n";
   std::cout << "d.size(): ";
   d.print_size();
   std::cout << "\n";

   ste::Matrix<double> e ({{1  , 12}});
   std::cout << "e:\n";
   e.print();
   std::cout << "\n";
   e.at(1) = 11;
   std::cout << "e:\n";
   e.print();
   std::cout << "\n";


   return 0;

}


/*

    OUTPUT :

a:
[ [ 1 11 12 ] [ 2 3 12 ] [ 5 6 4 ] ]
a.size(): [3 ; 3]
b:
[ [ 1 2 3 ] ]
b.size(): [1 ; 3]
b:
[ [ 1 2 3 ] [ 4 5 6 ] ]
b.size(): [2 ; 3]
c:
[ [ 83 116 192 ] [ 68 103 108 ] [ 37 97 148 ] ]
c.size(): [3 ; 3]
d:
[ [ 20 35 48 ] [ 44 95 132 ] ]
d.size(): [2 ; 3]
e:
[ [ 1 12 ] ]
e:
[ [ 11 12 ] ]
 */

