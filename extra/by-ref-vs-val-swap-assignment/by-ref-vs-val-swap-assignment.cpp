#include <iostream>
#include <cstddef>
#include <new>
#include <utility>
using namespace std;

//	Taken from (see below):
//	LINK: http://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
//	Benchmark by-value vs by-reference swap-implemented assignment for custom 'MyVector' -> (what am I even looking at with 'MyVector'?)


//	Results:
//	by-value
//	clang++ -stdlib=libc++ -std=c++11 -O3 -DUSE_SWAP_ASSIGNMENT by-ref-vs-val-swap-assignment.cpp
//			120 microseconds
//	by-ref
//	clang++ -stdlib=libc++ -std=c++11 -O3  by-ref-vs-val-swap-assignment.cpp
//			18 microseconds

template <class T>
class MyVector
{
    T* begin_;
    T* end_;
    T* capacity_;

public:
    MyVector()
        : begin_(nullptr),
          end_(nullptr),
          capacity_(nullptr)
        {}

    ~MyVector()
    {
        clear();
        ::operator delete(begin_);
    }

    MyVector(std::size_t N, const T& t)
        : MyVector()
    {
        if (N > 0)
        {
            begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
            capacity_ = begin_ + N;
            for (; N > 0; --N, ++end_)
                ::new(end_) T(t);
        }
    }

    MyVector(const MyVector& v)
        : MyVector()
    {
        std::size_t N = v.size();
        if (N > 0)
        {
            begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
            capacity_ = begin_ + N;
            for (std::size_t i = 0; i < N; ++i, ++end_)
                ::new(end_) T(v[i]);
        }
    }

    MyVector(MyVector&& v)
        : begin_(v.begin_),
          end_(v.end_),
          capacity_(v.capacity_)
    {
        v.begin_ = nullptr;
        v.end_ = nullptr;
        v.capacity_ = nullptr;
    }

#ifndef USE_SWAP_ASSIGNMENT

    //MyVector& operator=(const MyVector& v)
    //{
    //    if (this != &v)
    //    {
    //        std::size_t N = v.size();
    //        if (capacity() < N)
    //        {
    //            clear();
    //            ::operator delete(begin_);
    //            begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
    //            capacity_ = begin_ + N;
    //        }
    //        T* p = begin_;
    //        const T* q = v.begin_;
    //        for (; p < end_ && q < v.end_; ++p, ++q)
    //            *p = *q;
    //        if (q < v.end_)
    //        {
    //            for (; q < v.end_; ++q, ++end_)
    //                ::new(end_) T(*q);
    //        }
    //        else
    //        {
    //            while (end_ > p)
    //            {
    //                --end_;
    //                end_->~T();
    //            }
    //        }
    //    }
    //    return *this;
    //}

    MyVector& operator=(const MyVector& v)
    {
		auto tmp = v;
        swap(tmp);
        return *this;
    }
    //MyVector& operator=(MyVector&& v)
    //{
    //    clear();
    //    swap(v);
    //    return *this;
    //}

    MyVector& operator=(MyVector&& v)
    {
        clear();
        swap(v);
        return *this;
    }

#else

    //MyVector& operator=(const MyVector& v)
    //{
	//		auto tmp = v;
    //    swap(tmp);
    //    return *this;
    //}
    //MyVector& operator=(MyVector&& v)
    //{
    //    clear();
    //    swap(v);
    //    return *this;
    //}

    MyVector& operator=(MyVector v)
    {
        swap(v);
        return *this;
    }

#endif

    void clear()
    {
        while (end_ > begin_)
        {
            --end_;
            end_->~T();
        }
    }

    std::size_t size() const
        {return static_cast<std::size_t>(end_ - begin_);}
    std::size_t capacity() const
        {return static_cast<std::size_t>(capacity_ - begin_);}
    const T& operator[](std::size_t i) const
        {return begin_[i];}
    T& operator[](std::size_t i)
        {return begin_[i];}
    void swap(MyVector& v)
    {
        std::swap(begin_, v.begin_);
        std::swap(end_, v.end_);
        std::swap(capacity_, v.capacity_);
    }
};

template <class T>
inline
void
swap(MyVector<T>& x, MyVector<T>& y)
{
    x.swap(y);
}

#include <iostream>
#include <string>
#include <chrono>

int main()
{
    MyVector<std::string> v1(1000, "1234567890123456789012345678901234567890");
    MyVector<std::string> v2(1000, "1234567890123456789012345678901234567890123456789");
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double, std::micro> US;
    auto t0 = Clock::now();
    v2 = v1;
    auto t1 = Clock::now();
    std::cout << US(t1-t0).count() << " microseconds\n";
}

