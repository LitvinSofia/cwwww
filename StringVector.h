#ifndef STRING_VECTOR_HPP
#define STRING_VECTOR_HPP
#include "VectorIterator.h"
#include <iostream>
#include <iosfwd>
class StringVector {
private:
    std::string* array_;
    size_t capacity_;
    size_t size_;
public:
    void setSize(int n) {
        size_ = size_+ n;
    }
    class VectorIterator : public std::iterator < std::forward_iterator_tag, std::string, std::ptrdiff_t, std::string*, std::string& > {
 private:
        std::string* ptr_;
    public:
        explicit VectorIterator(std::string* ptr) :
            ptr_(ptr)
        {}

        VectorIterator operator++() {
            ptr_++;
            return *this;
        }
        VectorIterator operator++(int) {
            VectorIterator tmp = *this;
            ++ptr_;
            return tmp;
        }

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        bool operator==(const VectorIterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const VectorIterator& other) const {
            return !(*this == other);
        }
        friend void erase(VectorIterator& pos, StringVector& v);
    };

    friend void erase(VectorIterator& pos, StringVector& v) {
        if (pos == v.end()) {
            return;
        }
        std::string* target = pos.ptr_;
        for (std::string* p = target + 1; p != v.array_ + v.size_; ++p) {
            *(p - 1) = *p;
        }
        --v.size_;
        if (v.size_ < 0) {
            v.size_ = 0;
        }
    }
    VectorIterator begin() {
        return VectorIterator(array_);
    }
    VectorIterator end() {
        return VectorIterator(array_+size_);
    }
    
    StringVector() : 
        array_(nullptr), 
        capacity_(0),
        size_(0)
    {}
    StringVector(size_t capacity) :
        array_(new std::string[capacity]),
        capacity_(capacity),
        size_(0)
    {}
    ~StringVector()
    { 
        delete[] array_;
    }
    void push_back(const std::string& value) 
    {
        if (size_ == capacity_) {
            reserve((capacity_ == 0) ? 1 : capacity_ * 2);
        }
        array_[size_++] = value;
    }
    void pop_back()
    {
        if (size_ > 0) {
            --size_;
        }
    }
    std::string& operator[](size_t index) 
    {
        return array_[index];
    }
    const std::string& operator[](size_t index) const 
    {
        return array_[index];
    }
    bool empty() const
    {
        return size_ == 0;
    }
    size_t size() const
    {
        return size_;
    }
    size_t capacity() const 
    {
        return capacity_;
    }
    void reserve(size_t new_capacity) 
    {
        std::string* new_array = new std::string[new_capacity];
        for (size_t i = 0; i < size_; i++) {
            new_array[i] = array_[i];
        }
        delete[] array_;
        array_ = new_array;
        capacity_ = new_capacity;
    }
    friend std::ostream& operator<<(std::ostream& out, const StringVector& v)
    {
        for (size_t i = 0; i < v.size(); ++i) {
            out << v[i] << ' ';
        }
        return out;
    }
};
#endif