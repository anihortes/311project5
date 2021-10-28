#include <iostream>
using std::cout;
using std::endl;
// fstarray_test.cpp doctest.h fstarray.h
#include "fstarray.h"

int main() {
    FSTArray<int> a(10);
    FSTArray<int> b(a);
    for(auto i=0; i< a.size(); i++){
        std::cout <<i<<std::endl;
        std::cout << a[i]<<std::endl;
        std::cout << b[i]<<std::endl;
    }

    return 0;
}
