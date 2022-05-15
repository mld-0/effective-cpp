//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <new>
#include <utility>
#include <cstddef>
using namespace std;
//	{{{2
//	Ongoings:
//	{{{
//	Ongoing: 2022-05-15T16:38:39AEST overriding abstract virtual assignment operator not so easy/desirable (and if it were, it is a test on inheritance and operators beyond me) -> (a workaround, have non-virtual assignment operators which call abstract virtual functions, for derived classes to provide the implementation) -> ((is there) any way for Derived class to decide whether it wants to have an assignment operator receiving arguments by-ref/by-val?)
//	Ongoing: 2022-05-15T16:42:13AEST might (actually) need multiple classes (after all) to benchmark assignment operator by-val/by-ref?
//	}}}

template <class T>
class MyVectorInterface {
//	Ongoing: 2022-05-15T17:37:51AEST in practice, needing protected variables indicates questionable design? (this kind of half the implementation in Base, half in Derived design)
protected:
    T* begin_;
    T* end_;
    T* capacity_;

	//	LINK: https://stackoverflow.com/questions/38685940/c-assignment-operator-overloading-with-derived-classes

	//virtual void handle_copy_assignment(const MyVectorInterface&) = 0;
	//virtual void handle_move_assignment(MyVectorInterface&&) = 0;

	//template<typename W>
	//virtual void handle_copy_assignment(const W&) = 0;
	//template<typename W>
	//virtual void handle_move_assignment(W&&) = 0;

public:
    MyVectorInterface()
        : begin_(nullptr),
          end_(nullptr),
          capacity_(nullptr)
	{}

	//	Pure virtual dtor (prevent creation of class) (but provide implementation)
	virtual ~MyVectorInterface() = 0;

	//	{{{
    //virtual ~MyVectorInterface() 
	//{
    //    clear();
    //    ::operator delete(begin_);
    //}

	//	error, cannot pass abstract class by-value
	//virtual MyVectorInterface& operator=(const MyVectorInterface) = 0;		

	//	error: cannot create virtual template function
	//template<class W>
	//virtual MyVectorInterface& operator=(const W&) = 0;

	//	Ongoing: 2022-05-15T16:27:36AEST allow Derived class to implement 'operator=' ... either by-ref/by-value? -> (template? forwarding reference?)
	//virtual MyVectorInterface& operator=(const MyVectorInterface&) = 0;
	//virtual MyVectorInterface& operator=(const MyVectorInterface&&) = 0;

	//	Ongoing: 2022-05-15T16:46:15AEST leave Base assignment operators undefined, just define them for Derived classes ((in that case) still need some way to prevent clients from creating Base instances?)

	//MyVectorInterface& operator=(const MyVectorInterface& lhs) {
	//}

	//MyVectorInterface& operator=(MyVectorInterface&& lhs) {
	//}
	//	}}}

    MyVectorInterface(std::size_t N, const T& t)
        : MyVectorInterface() 
	{
        if (N > 0) {
            begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
            capacity_ = begin_ + N;
            for (; N > 0; --N, ++end_)
                ::new(end_) T(t);
        }
    }

    MyVectorInterface(const MyVectorInterface& v)
        : MyVectorInterface() 
	{
        std::size_t N = v.size();
        if (N > 0) {
            begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
            capacity_ = begin_ + N;
            for (std::size_t i = 0; i < N; ++i, ++end_)
                ::new(end_) T(v[i]);
        }
    }

    MyVectorInterface(MyVectorInterface&& v)
        : begin_(v.begin_),
          end_(v.end_),
          capacity_(v.capacity_) 
	{
        v.begin_ = nullptr;
        v.end_ = nullptr;
        v.capacity_ = nullptr;
    }

    void clear() { while (end_ > begin_) { --end_; end_->~T(); } }

    std::size_t size() const { return static_cast<std::size_t>(end_ - begin_); }

    std::size_t capacity() const { return static_cast<std::size_t>(capacity_ - begin_); }

    const T& operator[](std::size_t i) const { return begin_[i]; }

    T& operator[](std::size_t i) { return begin_[i]; }

    void swap(MyVectorInterface& v) {
        std::swap(begin_, v.begin_);
        std::swap(end_, v.end_);
        std::swap(capacity_, v.capacity_);
    }
};

template<typename T>
MyVectorInterface<T>::~MyVectorInterface() 
{
    clear();
    ::operator delete(begin_);
}

//	Ongoing: 2022-05-15T16:24:19AEST necessity to provide 'swap' for classes derived from 'MyVectorInterface'?
template<class T>
inline void swap(MyVectorInterface<T>& x, MyVectorInterface<T>& y) 
{
    x.swap(y);
}

//	Ongoing: 2022-05-15T16:58:44AEST Derived 'MyVectorA' -> need to provide ctors(/operators) which call Base counterparts?
template<class T>
class MyVectorA: public MyVectorInterface<T> {
public:

	MyVectorA() 
		: MyVectorInterface<T>()
	{}

	virtual ~MyVectorA() = default;

	MyVectorA(std::size_t N, const T& t)
		: MyVectorInterface<T>(N, t)
	{}

	//	Ongoing: 2022-05-15T17:02:40AEST (it works because it 'is-a'?) passing a Derived object (by-const-ref) to Base ctor
	MyVectorA(const MyVectorA& v)
		: MyVectorInterface<T>(v)
	{}

	MyVectorA(MyVectorA&& v) 
		: MyVectorInterface<T>(move(v))
	{}

	MyVectorA& operator=(const MyVectorA& v) 
	{
		auto tmp = v;
	    swap(tmp);
	    return *this;
	}

	MyVectorA& operator=(MyVectorA&& v) 
	{
		this->clear();
		swap(v);
		return *this;
	}

	//	Ongoing: 2022-05-15T17:08:14AEST (unfortunate that one also needs to provide 'swap' for Derived) (is passing a Derived by-ref where a Base by-ref is expected valid / works as one might expect (is-a)?)
	void swap(MyVectorA& v) {
		MyVectorInterface<T>::swap(v);
	}

};

template<class T>
class MyVectorB: public MyVectorInterface<T> {
public:
	MyVectorB() 
		: MyVectorInterface<T>()
	{}

	virtual ~MyVectorB() = default;

	MyVectorB(std::size_t N, const T& t)
		: MyVectorInterface<T>(N, t)
	{}

	//	Ongoing: 2022-05-15T17:02:40AEST (it works because it 'is-a'?) passing a Derived object (by-const-ref) to Base ctor
	MyVectorB(const MyVectorB& v)
		: MyVectorInterface<T>(v)
	{}

	MyVectorB(MyVectorB&& v) 
		: MyVectorInterface<T>(move(v))
	{}

	//	Ongoing: 2022-05-15T17:14:41AEST need to use 'this->' because Base class is a template class(?) (and is our use of it correct here?)
	//	LINK: https://stackoverflow.com/questions/9289859/calling-template-function-of-template-base-class
	MyVectorB& operator=(const MyVectorB& v) 
	{
		if (this != &v) {
			std::size_t N = v.size();
			if (this->capacity() < N) {
				this->clear();
				::operator delete(this->begin_);
				this->begin_ = this->end_ = static_cast<T*>(::operator new(N*sizeof(T)));
				this->capacity_ = this->begin_ + N;
			}
			T* p = this->begin_;
			const T* q = v.begin_;
			for (; p < this->end_ && q < v.end_; ++p, ++q)
				*p = *q;
			if (q < v.end_) {
				for (; q < v.end_; ++q, ++this->end_)
					::new(this->end_) T(*q);
			} else {
				while (this->end_ > p) {
					--this->end_;
					this->end_->~T();
				}
			}
		}
		return *this;
	}

	MyVectorB& operator=(MyVectorB&& v) {
		this->clear();
		swap(v);
		return *this;
	}

	//	Ongoing: 2022-05-15T17:30:19AEST we are <overriding/hiding> a non-virtual function (bad?)
	//	Ongoing: 2022-05-15T17:08:14AEST (unfortunate that one also needs to provide 'swap' for Derived) (is passing a Derived by-ref where a Base by-ref is expected valid / works as one might expect (is-a)?)
	void swap(MyVectorB& v) {
		MyVectorInterface<T>::swap(v);
	}

};

template<class T>
class MyVectorC: public MyVectorInterface<T> {
public:

	MyVectorC() 
		: MyVectorInterface<T>()
	{}

	virtual ~MyVectorC() = default;

	MyVectorC(std::size_t N, const T& t)
		: MyVectorInterface<T>(N, t)
	{}

	MyVectorC(const MyVectorC& v)
		: MyVectorInterface<T>(v)
	{}

	MyVectorC(MyVectorC&& v) 
		: MyVectorInterface<T>(move(v))
	{}

	MyVectorC& operator=(MyVectorC v) 
	{
	    swap(v);
	    return *this;
	}

	//	necessary? (is not the point of by-val assignment, that it can handle everything (and in theory, optimally)?)
	//MyVectorC& operator=(MyVectorC&& v) {
	//	this->clear();
	//	swap(v);
	//	return *this;
	//}

	void swap(MyVectorC& v) {
		MyVectorInterface<T>::swap(v);
	}
};


//	{{{
//#ifndef USE_SWAP_ASSIGNMENT
//    MyVectorInterface& operator=(const MyVectorInterface& v) {
//        if (this != &v) {
//            std::size_t N = v.size();
//            if (capacity() < N) {
//                clear();
//                ::operator delete(begin_);
//                begin_ = end_ = static_cast<T*>(::operator new(N*sizeof(T)));
//                capacity_ = begin_ + N;
//            }
//            T* p = begin_;
//            const T* q = v.begin_;
//            for (; p < end_ && q < v.end_; ++p, ++q)
//                *p = *q;
//            if (q < v.end_) {
//                for (; q < v.end_; ++q, ++end_)
//                    ::new(end_) T(*q);
//            } else {
//                while (end_ > p) {
//                    --end_;
//                    end_->~T();
//                }
//            }
//        }
//        return *this;
//    }
//    //MyVectorInterface& operator=(const MyVectorInterface& v) {
//	//	auto tmp = v;
//    //    swap(tmp);
//    //    return *this;
//    //}
//    MyVectorInterface& operator=(MyVectorInterface&& v) {
//        clear();
//        swap(v);
//        return *this;
//    }
//#else
//    //MyVectorInterface& operator=(const MyVectorInterface& v) {
//	//		auto tmp = v;
//    //    swap(tmp);
//    //    return *this;
//    //}
//    //MyVectorInterface& operator=(MyVectorInterface&& v) {
//    //    clear();
//    //    swap(v);
//    //    return *this;
//    //}
//    MyVectorInterface& operator=(MyVectorInterface v) {
//        swap(v);
//        return *this;
//    }
//#endif
//	}}}

using micro_s_t = std::chrono::duration<double, std::micro>;
using hi_res_clock_t = std::chrono::high_resolution_clock;

int main()
{
	cout << "by-ref swap assignment\n";
    MyVectorA<string> v1(1000, "1234567890123456789012345678901234567890");
    MyVectorA<string> v2(1000, "1234567890123456789012345678901234567890123456789");
    auto t0 = hi_res_clock_t::now();
    v2 = v1;
    auto t1 = hi_res_clock_t::now();
    cout << micro_s_t(t1-t0).count() << " microseconds\n";
	cout << "\n";

	cout << "custom assignment\n";
    MyVectorB<string> v3(1000, "1234567890123456789012345678901234567890");
    MyVectorB<string> v4(1000, "1234567890123456789012345678901234567890123456789");
    t0 = hi_res_clock_t::now();
    v4 = v3;
    t1 = hi_res_clock_t::now();
    cout << micro_s_t(t1-t0).count() << " microseconds\n";
	cout << "\n";

	cout << "by-val swap assignment\n";
    MyVectorC<string> v5(1000, "1234567890123456789012345678901234567890");
    MyVectorC<string> v6(1000, "1234567890123456789012345678901234567890123456789");
    t0 = hi_res_clock_t::now();
    v6 = v5;
    t1 = hi_res_clock_t::now();
    cout << micro_s_t(t1-t0).count() << " microseconds\n";
	cout << "\n";

	return 0;
}

