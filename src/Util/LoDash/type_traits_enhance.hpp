#ifndef type_traits_enhance_H
#define type_traits_enhance_H

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace LoDash
{
	template <typename T, typename U, typename... Rest>
	struct is_one_of : std::conditional_t<
		std::is_same_v<T, U>, std::true_type, is_one_of<T, Rest...>> {};

	template <typename T, typename U>
	struct is_one_of<T, U> : std::conditional_t<
		std::is_same_v<T, U>, std::true_type, std::false_type
	> {};

	template <typename T, typename K, typename V, typename = void>
	struct is_map_element : std::false_type {};
	template <typename T, typename K, typename V>
	struct is_map_element<T, K, V, std::void_t<typename T::key_type, typename T::mapped_type>> : is_one_of<T, std::map<K, V>, std::unordered_map<K, V>> {};

	template <typename T, typename V, typename = void>
	struct is_vector_element : std::false_type {};
	template <typename T, typename V>
	struct is_vector_element<T, V, std::void_t<typename T::value_type>> : is_one_of<T, std::vector<V>> {};

	template <typename T, typename V, typename = void>
	struct is_set_element : std::false_type {};
	template <typename T, typename K>
	struct is_set_element<T, K, std::void_t<typename T::key_type>> : is_one_of<T, std::set<K>, std::unordered_set<K>> {};

	template <typename T, typename V, typename = void>
	struct is_single_element : std::false_type {};
	template <typename T, typename V>
	struct is_single_element<T, V, std::void_t<typename T::value_type>> : is_one_of<T, std::set<V>, std::unordered_set<V>, std::vector<V>> {};

	template<typename T> struct lambda_traits;
	template<typename R, typename ClassType, typename ...Args>
	struct lambda_traits<R(ClassType::*)(Args...) const> {
		using result_type = R;
		using args_type = std::tuple<Args...>;
		template<size_t index>
		using arg_type_at = std::tuple_element_t<index, args_type>;
		static constexpr size_t arity = sizeof...(Args);
	};

	template <typename T>
	struct is_pointer : std::false_type {};
	template <typename T>
	struct is_pointer<T*> : std::true_type {};
	template <typename T>
	struct is_pointer<std::shared_ptr<T>> : std::true_type {};

	template <typename Expected, typename Actual>
	struct is_pass_by_reference_Ignore_pointer : std::conditional_t<
		std::is_same_v<Expected, const Actual&> ||
		is_pointer<Actual>::value,
		std::true_type, std::false_type
	> {};
}

#endif // !type_traits_enhance_H