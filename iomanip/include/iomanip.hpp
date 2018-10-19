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
inline int geti() {
    static int i = (int)std::ios_base::xalloc();
    return i;
}

struct sum_put : std::num_put<char> {
    iter_type do_put(iter_type s, std::ios_base& f, char_type fill, long v) const {
        static int i = geti();

        if (f.iword(i) == '+') {
            f.iword(i) = v;
            return nullptr;
        } else {
            long sum = v + f.iword(i);
            f.iword(i) = NULL;
            return std::num_put<char>::do_put(s, f, fill, sum);
        }
    }
};

std::ostream& add(std::ostream& os) {
    os.imbue(std::locale(std::locale(), new sum_put));
    os.iword(geti()) = '+';
    return os;
}
//}

#endif // __IOMANIP_HPP__
