//=======================================================================================
//
// EL is based on char strings only, provided that all files are in UTF-8, including source, 
// bin, config, and other external files. Minimal wstring interface is provided as nothing more 
// than a bridge to WinAPI. If your Windows system is based on a different locale then remember
// to change the locale to utf-8 with setlocale(LC_ALL, ".UTF-8") and use u8 string literals
// when you need to encode non-ascii characters. 
// 
// I've tested everything out under Winows 10 1909 18363.1198 and it works fine. 
// I can't say the same for lower versions as I'm not sure, but if your app is not dependent 
// on the console to print your characters then it's probably fine. 
// 
// In any case this header includes string algorithms for char based strings ONLY, 
// specifically Ascii/UTF-8 encoded strings.
//
// Most of my string algorithms construct a new string, except the ones that obviously 
// require a new size, like replace. 
// I do not actually splice strings, instead I deal directly with string indexes 
// using pseudo \0 and functors. 
// I could have used a pointer referece or the like 
// but I found that functors are surprisingly more readable.
// You CAN of course construct strings inside the functors with the provided character pointers.
// I may work on a custom find function if I feel like it. Until then I use stl for replace function.
//
// ======================================================================================
//
// EL 엔진은 소스 바이너리를 포함한 외부파일을 전부 UTF-8이라고 가정하고 코드를 짜기 때문에  
// 파일 입출력 및 스트링을 전부 char을 기반으로 하고 wbcs는 윈도우 API를 건드릴 때만 사용하도록
// 최소한의 호환 빼고는 거의 지원을 하지 않습니다. 
// 때문에 utf8를 cp949로 인코딩 된 wstring으로 불러들이거나 _T 매크로를 쓰는 등의 지금도 한국  
// 및 MS 환경에서 널리 사용되는 불편한 API를 최대한 지양하기로 결정?습니다. 
// 윈도우도 10을 시작으로 해서 UTF-8지원을 시작하는 모양에다 char string의 경우 UTF-8를 변환 없이
// 그대로 저장할 수 있고 ASCII/ANSI 호환성 등의 이점이 있어서 내린 결정입니다.
// 
// 대신 콘솔의 경우 setlocale(LC_ALL, ".UTF8")로 한글 깨짐 없이 입출력이 되게끔 해야 합니다.
// 또 파일 주소 등 입출력을 위한 문자열 리터럴의 경우는 반드시 u8리터럴을 써서 한글 및 다국어 문자가 
// 깨지지 않게 해야 합니다. 
// 그래도 wbcs를 위한 기존의 L리터럴과는 달리 똑같은 char기반 스트링이라 파일 입출력이나 스트링 
// 저장 및 변환은 다른 스트링과 똑같다는 점이 편리한 부분입니다. 게다가 ASCII랑은 완벽 호환이라
// 한국 문자를 쓰지 않는 리터럴은 기본 리터럴을 써도 될 것입니다.
// 
// UTF-8 UTF-16변환이 필요한 만약의 경우를 위해 북마크 해둔 라이브러리가 있습니다. 나중에
// WInAPI를 건드리게 되면 사용할 생각입니다. WinAPI 내장 함수도 분명 존재하지만 리눅스 및 다른 OS 환경에서
// 사용하기 힘들다는 허점이 있으며, 내장 cstring함수의 경우 윈도우 외의 환경에선 wchar_t가 
// UTF-16이 아닌 UCS-2나 다른 알 수 없는 형태로 변환되는 등의 변수가 있고, 마지막으로 codecvt의 utf8-16 
// 변환 함수 같은 경우는 현재 사용 권장이 되지 않는 관계로 후에 보다 OS환경에 영향을 덜 받는
// 외부 라이브러리로 통일 할 생각입니다. (현재 쓸 일이 없어서 보류)
// https://codingtidbit.com/2020/02/09/c17-codecvt_utf8-is-deprecated/
// 
//=======================================================================================

#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>

#include "define.h"

using std::cout;
using std::cin;
using std::endl;

namespace el {

	////////////////////// Typedef 스트링 정의 ///////////////////
	typedef std::string			string;
	typedef std::string_view	strview;
	typedef std::stringstream	stringstream;
	typedef std::istringstream	instream;
	typedef std::ostringstream	outstream;

	////////////////////// Conversions ///////////////////
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

	template<typename T>
	std::enable_if_t<is_num_v<T>, string>
		constexpr inline toString(T t) { return std::to_string(t); }

	template<typename T>
	std::enable_if_t<std::is_same_v<T, bool>, string>
		constexpr inline toString(bool b) { return b ? "true" : "false"; }

	/** Return floating points for doubles and floats **/
	// @param f: float type, floatingPoint: number of floating points printed
	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>, string>
		inline toString(T& f, int floatingPoint) {
		char buffer[64];
		std::snprintf(buffer, sizeof buffer, ("%." + std::to_string(floatingPoint) + "f").data(), f);
		return buffer;
	}

	inline sizet length(const char* s) { return std::strlen(s); }

	template<typename... Arg>
	sizet inline length(const string& t, Arg... a) {
		return length(t) + length(a...);
	}

	// Used only for concatenate
	void inline append(string& buffer, const string& lastString) {
		buffer.append(lastString);
	}
	template <typename... Arg>
	void inline append(string& buffer, const string& string, Arg... extraStrings) {
		buffer.append(string);
		append(buffer, extraStrings...);
	}
	//* Concatenate (reserves length first)   /// EFFICIENCY MAY VARY
	template <typename... Arg>
	void inline concatenate(string& buffer, const string& string, Arg... extraStrings) {
		buffer.reserve(length(buffer, string, extraStrings...));
		append(buffer, string, extraStrings...);
	}
	/**/


	////////////////// Divider /////////////////

	/// <summary>
	/// Splits a string by a delimiter and iterates the result
	/// Currently only supports single character delim
	/// </summary>
	/// <param name="T func //"> bool (*) (strview: data, sizet: iteration_number) </param>
	template<typename T>
	void iterate(strview str, const char delim, T func) {
		sizet strsize = str.size();

		if (strsize >= 3) {
			sizet i = 0, j = 0, n = 0;
			while (i < strsize) {
				if (str[i] == delim) {
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

	/// <summary>
	/// Reads two strings separated by a delimiter
	/// Currently only supports single character delim
	/// Iterates from the back because it's easier to deal with file directories
	/// </summary>
	/// <param name="T func //"> void (*) (strview: head, strview: tail) </param>
	template<typename T>
	void tokenize(strview str, const char delim, T func) {
		sizet strsize = str.size();
		if (strsize >= 3) {
			sizet i = strsize - 2;
			while (i > 0) {
				if (str[i] == delim) {
					func(str.substr(0, i), str.substr(i + 1, strsize - i - 1));
					return;
				} i--;
			}
		}

		func(str, str);
	}

	/// <summary>
	/// Traverses a string and executes a function each time it hits a delimiter
	/// Currently only supports single character delim
	/// Useful for a single specific case at the moment; to traverse a directory
	/// For details refer to <common/fileio.h> (deprected)
	/// </summary>
	/// <param name="T func //"> void (*) (strview: head) </param>
	template<typename T>
	void traverse(strview str, const char delim, T func) {
		for (auto i = 0; i < str.size(); i++) {
			if (str[i] == delim) {
				func(str.substr(0, i + 1));
			}
		}
	}

	/// <summary>
	/// Replaces words
	/// </summary>
	inline void replace(string& subject, const string& from, const string& to) {
		sizet i = 0;
		while ((i = subject.find(from, i)) != string::npos) {
			subject.replace(i, from.length(), to);
			i += to.length();
		}
	}
}