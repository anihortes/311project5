#include <iostream>
#include <algorithm>

int main() {
    int *ptr = new int [10];
    for(auto i=0;i<10;i++){
        ptr[i]=i;
    }

    int *newPtr = new int [500000000];
    try {
        std::copy(ptr, ptr + 10, newPtr);
        delete[] ptr;
        ptr = newPtr;
    }
    catch(std::bad_alloc &e){
        delete [] newPtr;
    }

    return 0;
}
