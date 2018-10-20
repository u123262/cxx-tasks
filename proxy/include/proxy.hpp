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
        proxy (T* ptr, std::unique_lock<std::mutex> lk) :ptr_ (ptr), lk_(std::move(lk)) { }
        ~proxy() { }
		
        T* operator ->() const {
            return ptr_ ;
        }
		
    private:
        T* ptr_ ;
        std::unique_lock<std::mutex> lk_;
    };

    proxy operator -> () const
    {
        std::unique_lock<std::mutex> lck(mutex_);
        return proxy(ptr_, std::move(lck));
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
