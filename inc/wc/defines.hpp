#pragma once

#define WC_EXCEPTION(type, ...) {										\
		auto sl = std::source_location::current();						\
		auto msg = std::format("{}: ", sl.function_name());				\
		msg += std::format(__VA_ARGS__);								\
		throw why_calculator::exception(msg, why_calculator::error_type::type);	\
	}

#define WC_STD_EXCEPTION(...) {								\
		auto sl = std::source_location::current();			\
		auto msg = std::format("{}: ", sl.function_name()); \
		msg += std::format(__VA_ARGS__);					\
		throw std::runtime_error(msg);						\
	}
