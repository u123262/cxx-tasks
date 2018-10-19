/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>

//{ endm manipulator
std::ostream& endm(std::ostream& os)
{
    os << "[eol]\n";
    return os;
}
//}

//{ squares manipulator
struct squares_proxy {
public:
    explicit squares_proxy(std::ostream& os):os(os){}

    template<typename T>
    friend std::ostream & operator<<(squares_proxy const& q, T const& word) {
        return q.os << "[" << word << "]";
    }

private:
    std::ostream & os;
};

struct squares_creator { } squares;
squares_proxy operator<<(std::ostream& os, squares_creator) {
    return squares_proxy(os);
}
//}

//{ add manipulator
struct add_proxy_next {
    explicit add_proxy_next(std::ostream & os, long const& num): os(os), previous_num(num) {}

    std::ostream& operator<<(long const& current_num) {
        return os << previous_num + current_num;
    }

private:
    std::ostream & os;
    long previous_num;
};

struct add_proxy {
    explicit add_proxy(std::ostream & os): os(os) {}

    struct add_proxy_next operator<<(long const& current_num) {
        return add_proxy_next(os, current_num);
    }

private:
    std::ostream & os;
};

struct add_creator { } add;
add_proxy operator<<(std::ostream & os, add_creator) {
    return add_proxy(os);
}
//}

#endif // __IOMANIP_HPP__
