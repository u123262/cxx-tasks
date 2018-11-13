/* Variant classes.
 * @file
 * @date 2018-08-12
 * @author Anonymous
 */

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>

#include <boost/variant.hpp>

//{ number
struct number: std::variant<int, float>
{
    using base = std::variant<int, float>;
	using base::base;
};
//}

//{ array
struct array: std::vector<number>
{
    using base = std::vector<number>;
	using base::base;
};
//}

//{ recursive_array
struct recursive_array: std::vector<std::variant<number, std::shared_ptr<recursive_array>>>
{
    using base = std::vector<std::variant<number, std::shared_ptr<recursive_array>>>;
	using base::base;
};
//}

//{ recursive_array2
struct recursive_array2: std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{
    using base = std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>;
	using base::base;
};
//}

//{ variant_decorator
template<typename...U>
struct variant_decorator: std::variant<U...>
{
	using base = std::variant<U...>;
	using base::base;

	template <typename T>
    const T& as() const {
		if constexpr (!std::disjunction_v<std::is_same<T, U>...>) {
			return std::get<boost::recursive_wrapper<T>>(*this).get();
		} else {
			return std::get<T>(*this);
		}
	}

	template <typename T>
	T& as() {
		return const_cast<T&>(const_cast<const variant_decorator *>(this)->as<T>());
	}
};
//}

//{ recursive_map
struct recursive_map: std::unordered_map<
		std::string,
		variant_decorator< int, bool, std::string,  boost::recursive_wrapper< recursive_map > > >
{
	using unordered_map::unordered_map;
};
//}

#endif // __VARIANT_HPP__
