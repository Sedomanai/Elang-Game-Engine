/*****************************************************************//**
 * @file   string_algorithm.h
 *
 * @brief Most of my string algorithms construct a new string, except the ones that obviously
 * require a new size, like replace.
 * I do not actually splice strings, instead I deal directly with string indexes
 * using pseudo \0 and functors.
 * I could have used a pointer referece or the like
 * but I found that functors are surprisingly more readable.
 * You CAN of course construct strings inside the functors with the provided character pointers.
 *
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "string.h"
#include "define.h"
#include "type_traits.h"
#include <algorithm>

namespace el
{
	namespace el_string
	{
		inline sizet				toSizet(strview str) { return sizet(std::stoi(str.data())); }
		inline int					toInt(strview str) { return std::stoi(str.data()); }
		inline unsigned int			toUint(strview str) { return unsigned int(std::stoi(str.data())); }
		inline long					toLong(strview str) { return std::stol(str.data()); }
		inline long long			toLonglong(strview str) { return std::stoll(str.data()); }
		inline float				toFloat(strview str) { return std::stof(str.data()); }
		inline double				toDouble(strview str) { return std::stod(str.data()); }
		inline long double			toLongDouble(strview str) { return std::stold(str.data()); }
		inline unsigned long		toUlong(strview str) { return std::stoul(str.data()); }
		inline unsigned long long	toUlonglong(strview str) { return std::stoull(str.data()); }

		// std::to_string alternative
		template<typename T>
		std::enable_if_t<is_num_v<T>, string>
			constexpr inline toString(T t) { return std::to_string(t); }

		// std::to_string alternative for boolean
		template<typename T>
		std::enable_if_t<std::is_same_v<T, bool>, string>
			constexpr inline toString(bool b) { return b ? "true" : "false"; }

		/**
		 * Return floating points for doubles and floats.
		 *
		 * @param f - Float variable
		 * @param floatingPoint - Number of floating points printed (after period)
		 * @return Minced float in string
		 */
		template<typename T>
		std::enable_if_t<std::is_floating_point_v<T>, string>
			inline toString(T& f, int floatingPoint) {
			char buffer[64];
			std::snprintf(buffer, sizeof buffer, ("%." + std::to_string(floatingPoint) + "f").data(), f);
			return buffer;
		}

		// std::strlen alternative.
		inline sizet length(const char* s) { return std::strlen(s); }

		// std::strlen of all string parameters added up
		template<typename... Arg>
		sizet inline length(const string& t, Arg... a) {
			return length(t) + length(a...);
		}

		/**
		 * Append all strings. This is used only for concatenate. Do not use this instead of the std::string append method.
		 *
		 * @param buffer - String to be appended to
		 * @param lastString - String to append
		 */
		void inline append(string& buffer, const string& lastString) {
			buffer.append(lastString);
		}

		/**
		 * Append all strings in parameter.
		 *
		 * @param buffer - String to be appended to
		 * @param string - String to append
		 * @param ...extraStrings - Other strings to append, in order
		 */
		template <typename... Arg>
		void inline append(string& buffer, const string& string, Arg... extraStrings) {
			buffer.append(string);
			append(buffer, extraStrings...);
		}

		/**
		 * Concatenate i.e. append all strings in parameter.
		 * Algorithm slightly different from append, reserves length first. Efficiency may vary.
		 *
		 * @param buffer - String to be appended to
		 * @param string - String to append
		 * @param ...extraStrings - Other strings to append, in order
		 */
		template <typename... Arg>
		void inline concatenate(string& buffer, const string& string, Arg... extraStrings) {
			buffer.reserve(length(buffer, string, extraStrings...));
			append(buffer, string, extraStrings...);
		}

		/**
		 * Splits a string by a delimiter and iterates the result. Only supports single character delimiter.
		 * Iterations do not include the delimiter.
		 *
		 * @param str - String in question, all strviewable types
		 * @param delimiter - Divider consisting of a single char
		 * @param func - Producdure to perform on each divisions -- void (*) (strview: division, sizest: divisionIndexIterationOrder)
		 */
		template<typename T>
		void iterate(strview str, const char delimiter, T func) {
			sizet strsize = str.size();

			if (strsize >= 3) {
				sizet i = 0, j = 0, n = 0;
				while (i < strsize) {
					if (str[i] == delimiter) {
						if (func(str.substr(j, i - j), n++))
							return;
						j = i + 1;
					} i++;
				}
				func(str.substr(j, strsize - j), n);
				return;
			}

			func(str, 0);
			return;
		}


		/**
		 * Reads two strings separated by a delimiter. Only supports single character delim.
		 * Iterates from the back because it's easier to deal with file directories. Token does not include delimiter.
		 * (Possibly deprecated due to fio (std::filesystem))
		 *
		 * @param str - String in question, all strviewable types
		 * @param delimiter - Divider consisting of a single char
		 * @param func- Producdure to perform on each divisions -- void (*) (strview: head, strview: tail)
		 */
		template<typename T>
		void tokenize(strview str, const char delimiter, T functor) {
			sizet strsize = str.size();
			if (strsize >= 3) {
				sizet i = strsize - 2;
				while (i > 0) {
					if (str[i] == delimiter) {
						functor(str.substr(0, i), str.substr(i + 1, strsize - i - 1));
						return;
					} i--;
				}
			}

			functor(str, str);
		}


		/**
		 * (deprected) For details refer to <common/fileio.h>.
		 * Traverses a string and executes a function each time it hits a delimiter, operating from the beginning of the string to the delimter in question.
		 * Only supports single character delim. Slice does not include delimiter. Useful for a single specific case at the moment; to traverse a directory.
		 *
		 * @param str- String in question, all strviewable types
		 * @param delimiter- Divider consisting of a single char
		 * @param func- Producdure to perform on each divisions -- void (*) (strview: head)
		 */
		template<typename T>
		void traverse(strview str, const char delimiter, T functor) {
			for (auto i = 0; i < str.size(); i++) {
				if (str[i] == delimiter) {
					functor(str.substr(0, i + 1));
				}
			}
		}

		/**
		 * Replaces words.
		 *
		 * @param subject- String in question to replace words
		 * @param from- Word to be replaced by
		 * @param to- Word to replace
		 */
		inline void replace(string& subject, const string& from, const string& to) {
			sizet i = 0;
			while ((i = subject.find(from, i)) != string::npos) {
				subject.replace(i, from.length(), to);
				i += to.length();
			}
		}
	}
}