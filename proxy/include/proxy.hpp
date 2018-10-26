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
    class proxy
    {
    public:
        proxy (T* ptr, std::mutex& mutex_): lock_(mutex_), ptr_ (ptr) { }

        T* operator ->() const {
            return ptr_ ;
        }
		
    private:
        T* ptr_ ;
        std::lock_guard<std::mutex> lock_;
    };

    proxy operator -> () const
    {
        return proxy(ptr_, std::ref(mutex_));
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
