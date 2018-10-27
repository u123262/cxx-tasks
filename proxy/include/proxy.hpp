/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>

template<class T>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object
    class proxy: private T
    {
    public:
        proxy (T* ptr, std::mutex* mutex) :ptr_ (ptr), mutex_(mutex) { }
        ~proxy(){ mutex_->unlock(); }
		
        T* operator ->() const {
            return ptr_ ;
        }
		
    private:
        T* ptr_ ;
        std::mutex* mutex_;
    };

    proxy operator -> () const
    {
        mutex_.lock();
        return proxy(ptr_, &mutex_);
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
