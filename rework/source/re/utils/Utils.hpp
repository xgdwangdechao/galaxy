///
///  Utils.hpp
///  rework
///
///  Created by reworks on 26/06/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_UTILS_HPP_
#define REWORK_UTILS_HPP_

#include <string>
#include <sstream>

#include "loguru/loguru.hpp"

namespace re
{
	namespace utils
	{
		///
		/// \brief Do something to each item in tuple.
		///
		/// Different overload.
		/// Credits: https://stackoverflow.com/a/26902803
		/// 
		template<class F, class...Ts, std::size_t...Is>
		inline void for_each_in_tuple(std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>)
		{
			using expander = int[];
			(void)expander {
				0, ((void)func(std::get<Is>(tuple)), 0)...
			};
		}

		///
		/// \brief Do something to each item in tuple.
		///
		/// E.g. for_each_in_tuple(some, [](const auto &x) { std::cout << x << std::endl; });
		/// Credits: https://stackoverflow.com/a/26902803
		/// 
		template<class F, class...Ts>
		inline void for_each_in_tuple(std::tuple<Ts...> & tuple, F func)
		{
			for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
		}

		///
		/// \brief Get an extension.
		///
		/// Thanks to: https://stackoverflow.com/a/4505931
		///
		/// \param filename Name of file with an extension to retrieve. ONLY WORKS FOR FILES. DO NOT USE FOR PATHS! ESPECIALLY PATHS WITH DOTS IN THEM!
		///
		/// \return string containing extension.
		///
		inline std::string getExtension(const std::string& filename)
		{
			if (filename.find_last_of(".") != std::string::npos)
			{
				return filename.substr(filename.find_last_of(".") + 1);
			}
			else
			{
				return "";
			}
		}

		///
		/// \brief Remove an extension.
		///
		/// Thanks to: http://stackoverflow.com/a/6417908
		///
		/// \param filename Name of the file with the extension to remove. ONLY WORKS FOR FILES. DO NOT USE FOR PATHS! ESPECIALLY PATHS WITH DOTS IN THEM!
		///
		/// \return filename without extension.
		///
		inline std::string removeExtension(const std::string& filename)
		{
			size_t lastdot = filename.find_last_of(".");
			if (lastdot == std::string::npos) return filename;
			return filename.substr(0, lastdot);
		}

		///
		/// Converts a boolean to a std::string.
		///
		/// \param value Boolean value to convert.
		///
		/// \return Returns either "true" or "false".
		///
		inline std::string boolToString(bool value)
		{
			std::string out = (value == true) ? "true" : "false";
			return out;
			
		}

		///
		/// Converts a std::string to a boolean.
		///
		/// \param value std::string value to convert.
		///
		/// \return Returns either true or false.
		///
		inline constexpr bool stringToBool(std::string_view str)
		{
			bool out = (str == "true") ? true : false;
			return out;
		}

		///
		/// \brief Convert a std::string to any type.
		///
		/// Thanks to: https://gist.github.com/timofurrer/2725779
		/// Some adjustments have been made.
		///
		/// \param data std::string containing type to convert.
		///
		/// \return Returns data as T type.
		///
		template <typename T>
		inline T convertString(const std::string &data)
		{
			if (!data.empty())
			{
				T ret;

				std::istringstream iss(data);
				if (data.find("0x") != std::string::npos)
				{
					iss >> std::hex >> ret;
				}
				else
				{
					iss >> std::dec >> ret;
				}

				if (iss.fail())
				{
					LOG_S(ERROR) << "Convert error: cannot convert string '" << data << "' to value";
					return T();
				}

				return ret;
			}

			return T();
		}

		///
		/// Template specialization for convertString for std::string.
		///
		/// \see convertString
		///
		template<>
		inline std::string convertString<std::string>(const std::string& data)
		{
			return data;
		}

		///
		/// Template specialization for convertString for bool.
		///
		/// \see convertString
		///
		template<>
		inline bool convertString<bool>(const std::string& data)
		{
			return utils::stringToBool(data);
		}

		///
		/// \brief Ensures any null c-strings are converted to empty ones.
		///
		/// Credits: https://stackoverflow.com/a/1724514
		///
		/// \param s String to check.
		///
		/// \return Returns empty string if input is null.
		///
		///
		inline constexpr const char* nullToEmpty(char const* s)
		{
			return (s ? s : "");
		}
	}
}

#endif