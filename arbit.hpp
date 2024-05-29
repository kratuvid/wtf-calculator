#pragma once

#include <cstdint>
#include <cstring>
#include <cmath>
#include <list>
#include <print>
#include <limits>
#include <string_view>
#include <cstdlib>
#include <cctype>
#include <exception>
#include <format>
#include <array>
#include <string>
#include <algorithm>
#include <initializer_list>

#include "utility.hpp"

#define WC_ARBIT_EXCEPTION(type, ...) {									\
		auto msg = std::format(__VA_ARGS__);							\
		throw wc::arbit::exception(msg, wc::arbit::error_type::type);	\
	}

namespace wc
{
	class arbit
	{
	public:
		enum class error_type { parse, calc };
		static constexpr std::array<std::string_view, 2> error_type_str { "parse", "calc" };
		class exception;

		using base_t = uint32_t;
		using sbase_t = int32_t;
		using base_double_t = uint64_t;

	private:
		static const base_t default_precision = 50, base_max = ~base_t(0);
		static const size_t base_bits = sizeof(base_t) * 8;

		base_t precision = default_precision;
		base_t *fixed_ptr=nullptr, *decimal_ptr=nullptr;
		size_t fixed_len=0, decimal_len=0;
		size_t actual_fixed_len=0, actual_decimal_len=0;

	private:
		template<typename T> void is_valid_integer()
		{
			static_assert(std::numeric_limits<T>::is_integer);
			static_assert(std::numeric_limits<T>::is_signed);
		}

		void parse(std::string_view both);
		void parse(std::string_view fixed, std::string_view decimal, bool neg);

		void grow(size_t by);
		void grow(size_t by, bool neg);
		void shrink(size_t by);

	public:
		arbit(const arbit& other);
		arbit(arbit&& other);
		arbit(std::string_view both, base_t precision=default_precision);
		arbit(base_t fixed=0, base_t decimal=0, base_t precision=default_precision);
		arbit(std::initializer_list<base_t> fixed, std::initializer_list<base_t> decimal, base_t precision=default_precision);
		~arbit();

		base_t get_precision() const;
		base_t set_precision(base_t precision);

		bool is_negative() const;
		size_t bytes() const { return sizeof(base_t) * fixed_len; }
		static bool is_base_t_negative(base_t n) { return n >> ((sizeof(base_t) * 8) - 1); }

		arbit& negate();
		arbit operator-() const;

		template<typename T> arbit& operator-=(T rhs) { *this += -rhs; return *this; }
		arbit& operator-=(const arbit& rhs) { *this += -rhs; return *this; }

		template<typename T> arbit& operator+=(T rhs);
		arbit& operator+=(const arbit& rhs);

		arbit& operator=(const arbit& rhs);

		void raw_print(bool hex) const;
		void print() const;
	};

	class arbit::exception : public std::runtime_error
	{
	public:
		arbit::error_type type;

		exception(const std::string_view& what, arbit::error_type type)
			:std::runtime_error(what.data()), type(type)
		{}
	};
};

#include "arbit.inl"

/* Test code

   	try
	{
		const auto max_i = std::numeric_limits<int>::max();
		const auto max_ll = std::numeric_limits<long long>::max();
		const auto max_u = std::numeric_limits<unsigned>::max();

		std::random_device rd;
		std::mt19937 engine(rd());
		std::uniform_int_distribution<int> dist(-1000, 1000);

		// wc::arbit n100("109.8442");
		const auto begin = max_i - 100;
		std::println("Begin: {}", begin);

		wc::arbit n0({begin, begin - 200, begin / 2}, {});
		n0.negate();
		for (int i=0; i < 10; i++)
		{
			auto by = dist(engine);
			n0 += by;
			std::print("+= {}: ", by); n0.print();
			std::print(" <> ");
			n0.raw_print(true);
		}
	}
	catch (wc::arbit::exception& e)
	{
		std::println("Fatal arbit exception: {}: {}",
					 wc::arbit::error_type_str[static_cast<int>(e.type)],
					 e.what());
		return 12;
	}
	catch (std::exception& e)
	{
		std::println("Fatal standard exception: {}", e.what());
		return 11;
	}
*/