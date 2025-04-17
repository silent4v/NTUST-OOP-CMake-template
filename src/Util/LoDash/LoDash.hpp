#ifndef LoDash_H
#define LoDash_H

#include "type_traits_enhance.hpp"

namespace LoDash
{
	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		std::vector<typename Container::key_type>
	> Keys [[nodiscard]] (const Container& m)
	{
		std::vector<typename Container::key_type> result;
		for (const auto& [k, v] : m)
		{
			result.push_back(k);
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		std::vector<typename Container::mapped_type>
	> Values [[nodiscard]] (const Container& m)
	{
		std::vector<typename Container::mapped_type> result;
		for (const auto& [k, v] : m)
		{
			result.push_back(v);
		}

		return result;
	}

#pragma region Map

	template <typename Func, typename V, typename K = void, typename = void>
	struct lambda_traits_map_vector_parameter : std::false_type {};

	template <typename Func, typename V, typename K>
	struct lambda_traits_map_vector_parameter<Func, V, K, void> : std::conditional_t<
		std::is_same_v<typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<0>, K>&&
		std::is_same_v<typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<1>, V>,
		std::true_type, std::false_type
	> {};
	template <typename Func, typename V>
	struct lambda_traits_map_vector_parameter<Func, V, void, void> : std::conditional_t<
		std::is_same_v<typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<0>, V>,
		std::true_type, std::false_type
	> {};

	template <typename Func, typename V, typename K = void>
	struct lambda_traits_map_vector_variadic_parameter : std::conditional_t<
		2 == lambda_traits<decltype(&Func::operator())>::arity,
		lambda_traits_map_vector_parameter<Func, V, K>,
		lambda_traits_map_vector_parameter<Func, V>
	> {};

	template <typename Type_Origin, typename MapFunc,
		typename Type_Converted = typename lambda_traits<decltype(&MapFunc::operator())>::result_type
	> std::enable_if_t<
		2 == lambda_traits<decltype(&MapFunc::operator())>::arity&&
		lambda_traits_map_vector_variadic_parameter<MapFunc, const Type_Origin&, const std::size_t&>::value,
		std::vector<Type_Converted>
	> Map [[nodiscard]] (const std::vector<Type_Origin>& datas, MapFunc&& mapFunc)
	{
		std::vector<Type_Converted> result;
		for (std::size_t i = 0; i < datas.size(); i++)
		{
			result.push_back(mapFunc(i, datas[i]));
		}
		return result;
	}

	template <typename Type_Origin, typename MapFunc,
		typename Type_Converted = typename lambda_traits<decltype(&MapFunc::operator())>::result_type
	> std::enable_if_t<
		1 == lambda_traits<decltype(&MapFunc::operator())>::arity&&
		lambda_traits_map_vector_variadic_parameter<MapFunc, const Type_Origin&>::value,
		std::vector<Type_Converted>
	> Map [[nodiscard]] (const std::vector<Type_Origin>& datas, MapFunc&& mapFunc)
	{
		std::vector<Type_Converted> result;
		for (std::size_t i = 0; i < datas.size(); i++)
		{
			result.push_back(mapFunc(datas[i]));
		}
		return result;
	}

#pragma endregion

#pragma region Where

	template <typename Func, typename K, typename V,
		typename KEY = typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<0>,
		typename VALUE = typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<1>
	> struct lambda_traits_Where_map_parameter : std::conditional_t<
		is_pass_by_reference_Ignore_pointer<KEY, K>::value&&
		is_pass_by_reference_Ignore_pointer<VALUE, V>::value,
		std::true_type, std::false_type
	> {};

	template <typename Container, typename Func>
	std::enable_if_t<
		is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value&&
		lambda_traits_Where_map_parameter<Func, typename Container::key_type, typename Container::mapped_type>::value,
		Container
	> Where [[nodiscard]] (const Container& m, Func&& filterFunc)
	{
		Container result;
		for (const auto& [k, v] : m)
		{
			if (!filterFunc(k, v))
				continue;

			result.insert(std::make_pair(k, v));
		}

		return result;
	}

	template <typename Func, typename K,
		typename KEY = typename lambda_traits<decltype(&Func::operator())>::template arg_type_at<0>
	> struct single_parameter_lambda_traits : std::conditional_t<
		is_pass_by_reference_Ignore_pointer<KEY, K>::value,
		std::true_type, std::false_type
	> {};

	template <class Container, typename Func>
	std::enable_if_t<
		is_vector_element<Container, typename Container::value_type>::value&&
		single_parameter_lambda_traits<Func, typename Container::value_type>::value,
		Container
	> Where [[nodiscard]] (const Container& m, Func&& filterFunc)
	{
		Container result;
		for (const auto& v : m)
		{
			if (!filterFunc(v))
				continue;

			result.push_back(v);
		}

		return result;
	}

	template <typename Container, typename Func>
	std::enable_if_t<is_set_element<Container, typename Container::key_type>::value&&
		single_parameter_lambda_traits<Func, typename Container::key_type>::value,
		Container
	> Where [[nodiscard]] (const Container& m, Func&& filterFunc)
	{
		Container result;
		for (const auto& v : m)
		{
			if (!filterFunc(v))
				continue;

			result.insert(v);
		}

		return result;
	}

#pragma endregion

#pragma region Sum


	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		typename Container::mapped_type
	> Sum [[nodiscard]] (const Container& m)
	{
		typename Container::mapped_type result = 0;
		for (const auto& [k, v] : m)
		{
			result += v;
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<is_vector_element<Container, typename Container::value_type>::value,
		typename Container::value_type
	> Sum [[nodiscard]] (const Container& m)
	{
		typename Container::value_type result = 0;
		for (const auto& v : m)
		{
			result += v;
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<is_set_element<Container, typename Container::key_type>::value,
		typename Container::key_type
	> Sum [[nodiscard]] (const Container& m)
	{
		typename Container::key_type result = 0;
		for (const auto& v : m)
		{
			result += v;
		}
		return result;
	}

#pragma endregion

#pragma region Max

	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		std::pair<typename Container::key_type, typename Container::mapped_type>
	> Max [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		std::pair<typename Container::key_type, typename Container::mapped_type> result;
		for (const auto& [k, v] : m)
		{
			if (!isFirst)
			{
				result = std::make_pair(k, v);
				isFirst = true;
				continue;
			}

			if (result.second < v)
				result = std::make_pair(k, v);
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<
		is_set_element<Container, typename Container::value_type>::value,
		typename Container::value_type
	> Max [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		typename Container::value_type result;
		for (const auto& v : m)
		{
			if (!isFirst)
			{
				result = v;
				isFirst = true;
				continue;
			}

			if (result < v)
				result = v;
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<
		is_vector_element<Container, typename Container::value_type>::value,
		std::pair<typename Container::size_type, typename Container::value_type>
	> Max [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		std::pair<typename Container::size_type, typename Container::value_type> result;
		for (typename Container::size_type i = 0; i < m.size(); i++)
		{
			if (!isFirst)
			{
				result = std::make_pair(i, m[i]);
				isFirst = true;
				continue;
			}

			if (result.second < m[i])
				result = std::make_pair(i, m[i]);
		}

		return result;
	}

#pragma endregion

#pragma region Min

	template <typename Container>
	std::enable_if_t < is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		std::pair<typename Container::key_type, typename Container::mapped_type>
	> Min [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		std::pair<typename Container::key_type, typename Container::mapped_type> result;
		for (const auto& [k, v] : m)
		{
			if (!isFirst)
			{
				result = std::make_pair(k, v);
				isFirst = true;
				continue;
			}

			if (result.second > v)
				result = std::make_pair(k, v);
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<is_set_element<Container, typename Container::value_type>::value,
		typename Container::value_type
	> Min [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		typename Container::value_type result;
		for (const auto& v : m)
		{
			if (!isFirst)
			{
				result = v;
				isFirst = true;
				continue;
			}

			if (result > v)
				result = v;
		}

		return result;
	}

	template <typename Container>
	std::enable_if_t<
		is_vector_element<Container, typename Container::value_type>::value,
		std::pair<typename Container::size_type, typename Container::value_type>
	> Min [[nodiscard]] (const Container& m)
	{
		bool isFirst = false;
		std::pair<typename Container::size_type, typename Container::value_type> result;
		for (typename Container::size_type i = 0; i < m.size(); i++)
		{
			if (!isFirst)
			{
				result = std::make_pair(i, m[i]);
				isFirst = true;
				continue;
			}

			if (result.second > m[i])
				result = std::make_pair(i, m[i]);
		}

		return result;
	}

#pragma endregion

#pragma region Any

	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		bool
	> Any [[nodiscard]] (const Container& m, const std::function<bool(typename Container::key_type, typename Container::mapped_type)>& filterFunc)
	{
		for (const auto& [k, v] : m)
		{
			if (filterFunc(k, v))
			{
				return true;
			}
		}

		return false;
	}

	template <typename Container>
	std::enable_if_t<is_vector_element<Container, typename Container::value_type>::value,
		bool
	> Any [[nodiscard]] (const Container& m, const std::function<bool(typename Container::value_type)>& filterFunc)
	{
		for (const auto& v : m)
		{
			if (filterFunc(v))
				return true;
		}

		return false;
	}

	template <typename Container>
	std::enable_if_t<is_set_element<Container, typename Container::key_type>::value,
		bool
	> Any [[nodiscard]] (const Container& m, const std::function<bool(typename Container::key_type)>& filterFunc)
	{
		for (const auto& v : m)
		{
			if (filterFunc(v))
				return true;
		}

		return false;
	}

#pragma endregion

#pragma region All

	template <typename Container>
	std::enable_if_t<is_map_element<Container, typename Container::key_type, typename Container::mapped_type>::value,
		bool
	> All [[nodiscard]] (const Container& m, const std::function<bool(typename Container::key_type, typename Container::mapped_type)>& filterFunc)
	{
		for (const auto& [k, v] : m)
		{
			if (!filterFunc(k, v))
			{
				return false;
			}
		}

		return true;
	}

	template <typename Container>
	std::enable_if_t<is_vector_element<Container, typename Container::value_type>::value,
		bool
	> All [[nodiscard]] (const Container& m, const std::function<bool(typename Container::value_type)>& filterFunc)
	{
		for (const auto& v : m)
		{
			if (!filterFunc(v))
				return false;
		}

		return true;
	}

	template <typename Container>
	std::enable_if_t<is_set_element<Container, typename Container::key_type>::value,
		bool
	> All [[nodiscard]] (const Container& m, const std::function<bool(typename Container::key_type)>& filterFunc)
	{
		for (const auto& v : m)
		{
			if (!filterFunc(v))
				return false;
		}

		return true;
	}

#pragma endregion

}

#endif // !LoDash_H