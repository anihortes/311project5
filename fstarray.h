// FSTArray.h
// A. Harrison Owen
// Started: 2021-10-15
// Updated: 2021-10-28
//
// For CS 311 Fall 2021
// Frightfully smart array of int

#ifndef FILE_FSTArray_H_INCLUDED
#define FILE_FSTArray_H_INCLUDED

#include <cstddef>
// For std::size_t
#include <algorithm>
// For std::copy
// For std::swap
#include <stdexcept>
// For std::out_of_range

// *********************************************************************
// class FSTArray - Class definition
// *********************************************************************


// class FSTArray
// Frightfully Smart Array of int.
// Resizable, copyable/movable, exception-safe.
// Invariants:
//     0 <= _size <= _capacity.
//     _data points to an array of value_type, allocated with new [],
//      owned by *this, holding _capacity value_type values -- UNLESS
//      _capacity == 0, in which case _data may be nullptr.
//
// value_type = value type of array elements
template <typename valType>
class FSTArray {

// ***** FSTArray: types *****
public:

    // value_type: type of data items
    using value_type = valType;
    // size_type: type of sizes & indices
    using size_type = std::size_t;

    // iterator, const_iterator: random-access iterator types
    using iterator = value_type *;
    using const_iterator = const value_type *;

// ***** FSTArray: internal-use constants *****
private:

    // Capacity of default-constructed object
    enum { DEFAULT_CAP = 16 };

// ***** FSTArray: ctors, op=, dctor *****
public:

    // Default ctor & ctor from size
    // Strong Guarantee
    explicit FSTArray(size_type size=0)
        :_capacity(std::max(size, size_type(DEFAULT_CAP))),
            // _capacity must be declared before _data
         _size(size),
         _data(_capacity == 0 ? nullptr
                              : new value_type[_capacity])
    {}

    // Copy ctor
    // Strong Guarantee
    FSTArray(const FSTArray & other):
        _capacity(other._capacity),
        _size(other._size),
        _data(other._capacity == 0 ? nullptr
            : new value_type[other._capacity])
    {
        try {
            std::copy(other.begin(), other.end(), begin());
        }
        catch(...){
            //if it fails and exits...delete this pointer?
            delete [] _data;
            throw;
        }
    }

    // Move ctor
    // No-Throw Guarantee
    FSTArray(FSTArray && other) noexcept
            :_capacity(other._capacity),
             _size(other._size),
             _data(other._data)
    {
        other._capacity = 0;
        other._size = 0;
        other._data = nullptr;
    }

    // Copy assignment operator
    // No-throw Guarantee
    FSTArray & operator=(const FSTArray & other)
    {
        FSTArray copyRhs(other);
        swap(copyRhs);
        return *this;
    }

    // Move assignment operator
    // No-Throw Guarantee
    FSTArray & operator=(FSTArray && other) noexcept
    {
        swap(other);
        return *this; // DUMMY
    }

    // Dctor
    // No-Throw Guarantee
    ~FSTArray()
    {
        delete [] _data;
    }

// ***** FSTArray: general public operators *****
public:

    // operator[] - non-const & const
    // Pre:
    //     index must be non-zero
    //     index must not be greater than size of array
    // No-Throw Guarantee
    // Exception Neutral
    value_type & operator[](size_type index)
    {
        return _data[index];
    }

    const value_type & operator[](size_type index) const
    {
        return _data[index];
    }

// ***** FSTArray: general public functions *****
public:

    // size
    // No-Throw Guarantee
    // Exception neutral
    [[nodiscard]] size_type size() const noexcept
    {
        return _size;
    }

    // empty
    // No-Throw Guarantee
    // Exception neutral
    [[maybe_unused]] [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0;
    }

    // begin - non-const & const
    // No-Throw Guarantee
    // Exception neutral
    iterator begin() noexcept
    {
        return _data;
    }
    const_iterator begin() const noexcept
    {
        return _data;
    }

    // end - non-const & const
    // No-Throw Guarantee
    // Exception neutral
    iterator end() noexcept
    {
        return begin() + size();
    }
    const_iterator end() const noexcept
    {
        return begin() + size();
    }


// resize
// Strong Guarantee
// Exception neutral
// Pre:
//     newsize must be non-zero
    void resize(size_type newsize)
    {
        if(newsize >= _capacity) {
            _capacity = 2*newsize;
            auto *newArray = new value_type[_capacity];
            try {
                std::copy(_data, _data+size(), newArray);
                //clean up _data ptr
                delete[] _data;
                _data = newArray;
            }
            catch(...){
                // if copy fails, delete newArray pointer and exit
                // copy should not destroy original data
                delete[] newArray;
                throw;
            }
        }
        _size = newsize;

    }


// insert
// Strong Guarantee
// Exception neutral
// Pre:
//     iterator must be non-zero and smaller than size
    iterator insert(iterator pos, const value_type & item)
    {
        // if data is reallocated, then we will have to
        // save distance from iterator to begin()
        auto tempPosition = pos - begin();
        resize(size()+1);
        *(end()-1) = item;

        if(tempPosition != size()){
            std::rotate(begin()+tempPosition, end()-1, end());
        }

        return begin()+tempPosition;
    }


// erase
// Strong Guarantee
// Exception neutral
    iterator erase(iterator pos)
    {
        try {
            if (pos != end()) {
                std::rotate(pos, pos + 1, end());
            }
        }
        catch(...){
            throw;
        }
        _size--;
        return pos;
    }


// swap
// No-throw Guarantee
// Exception neutral
    void swap(FSTArray & other) noexcept
    {
            std::swap(_data, other._data);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
    }

    // push_back
    // Strong Guarantee
    // Exception neutral
    void push_back(const value_type & item)
    {
        insert(end(), item);
    }

    // pop_back
    // Strong Guarantee
    // Exception neutral
    void pop_back()
    {
        erase(end()-1);
    }

// ***** FSTArray: data members *****
private:

    // Below, _capacity must be declared before _data
    size_type    _capacity;  // Size of our allocated array
    size_type    _size;      // Size of client's data
    value_type * _data;      // Pointer to our array

};  // End class FSTArray


#endif  //#ifndef FILE_FSTArray_H_INCLUDED

