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
    template<typename T>
    number(const T& x):std::variant<int, float>(x) {};
};
//}

//{ array
struct array: std::vector<number>
{
    array(std::initializer_list<number> x):std::vector<number>(x) {};
};
//}

//{ recursive_array
struct recursive_array: std::vector<std::variant<number, std::shared_ptr<recursive_array>>>
{
    recursive_array(std::initializer_list<std::variant<number, std::shared_ptr<recursive_array>>> x):
    std::vector<std::variant<number, std::shared_ptr<recursive_array>>>(x) {};
};
//}

//{ recursive_array2
struct recursive_array2: std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{
    auto convert(const std::initializer_list<std::variant<number, recursive_array2>>& input){
		std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>> tmp;

		for(auto x : input) {
			if (x.index() == 1)
				tmp.push_back(boost::recursive_wrapper<recursive_array2>(std::get<recursive_array2>(x)));
			else
				tmp.push_back(std::get<number>(x));
		}

		return tmp;
	}

	recursive_array2(std::initializer_list<std::variant<number, recursive_array2>> x):
	std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>(convert(x)) {};
};
//}

//{ variant_decorator
template<typename...T>
struct variant_decorator: std::variant<T...>
{
    using base = std::variant<T...>;
	using base::base;

	template <typename T>
	std::enable_if_t<!std::is_same<T, struct recursive_map>::value, T>
		as() { return std::get<T>(*this); }

	template <typename T>
	std::enable_if_t<std::is_same<T, struct recursive_map>::value, T>
		as() { return std::get<boost::recursive_wrapper< recursive_map >>(*this).get(); }
};
//}

//{ recursive_map
struct recursive_map: std::unordered_map<
	std::string,
	variant_decorator< int, bool, std::string,  boost::recursive_wrapper< recursive_map > > >
{
    using base = std::unordered_map<
			std::string,
			variant_decorator< int, bool, std::string,  boost::recursive_wrapper< recursive_map > > >;

	using base::base;
};
//}

#endif // __VARIANT_HPP__
