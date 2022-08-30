/*****************************************************************//**
 * @file   container.h
 * @brief  Container types and algorithms. Also includes custom containers.
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include <vector>
#include <map>
#include <list>
#include <queue>
#include <stack>

#include <tsl/robin_map.h>
#include <tsl/robin_set.h>

#include "define.h"
#include "string.h"
#include "algorithm.h"

namespace el
{
	template <typename T>
	using vector = std::vector<T>;

	template <typename T, typename U>
	using map = std::map<T, U>;

	template <typename T>
	using list = std::list<T>;

	template <typename T>
	using queue = std::queue<T>;

	template <typename T>
	using stack = std::stack<T>;

	template <typename TyA_, typename TyB_>
	struct pair { TyA_ first; TyB_ second; };

	template<typename Ty_>
	struct array { Ty_* data; size_t size; };


	// cout vector
	template<class T>
	inline std::ostream& operator<<(std::ostream& out, const vector<T>& v) {
		cout << "{ ";

		for (int i = 0; i < v.size(); i++) {
			cout << v[i] << (i == (v.size() - 1) ? " " : ", ");
		}

		cout << "}";
		return out;
	}

	/**
	 * Rotate vetors. Useful for list-like types where there can be a lot of index reordering.
	 * 
	 * @param v- Vector to swapshift
	 * @param from- Original index of item to swapshift
	 * @param to- 
	 */
	template <typename T>
	void swapshift(vector<T>& v, sizet from, sizet to) {
		if (to > from)
			std::rotate(v.begin() + from, v.begin() + from + 1, v.begin() + to);
		else
			std::rotate(v.rend() - from - 1, v.rend() - from, v.rend() - to);
	}
}