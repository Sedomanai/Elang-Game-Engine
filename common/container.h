/*****************************************************************//**
 * @file   container.h
 * @brief  Container types and algorithms. Also includes custom containers.
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <stack>

#include "define.h"

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

#ifndef __EL_ARRAY_
	template<typename Ty_>
	struct array { Ty_* data; size_t size; };
#define __EL_ARRAY_
#endif


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

	namespace el_vector
	{
		/**
		 * @brief * Swaps two indicies of a vector. Useful for list-like types where there can be a lot of index reordering.
		 *
		 * @brief * Unlike the "from" index which umambiguously refers to the element in question, the "to" index may seem tricky at first;
		 * instead of an item index, it refers to the gap in between.
		 *
		 * @brief * For exmaple, if "to" is 3, it refers to the space between elements in index 2 and 3.
		 * Gaps include that between -1 and 0, and also the last element and the end() of vector; therefore # of gaps is +1 of vector size.
		 * If the "from" index is n, then both n and n+1 in "to" will result in an unchanged vector.
		 *
		 * @brief * It's easy if you imagine drag-dropping them into a list gui (they're usually dropped in between gaps, not the list items).
		 * After all, this is what this function is mostly made for: to sync with list gui.
		 *
		 * @brief * Neither "from" nor "to" parameters are clamped. They must be done manually outside the function.
		 *
		 * @param v : Vector to swapshift
		 * @param from : Original index of the item to swapshift. Must be clamped between 0 and (vector size - 1)
		 * @param to : Gap between two indicies to swapshift to. If it's n, then it refers to the gap between elements n-1 and n.
		 * Must be clamped between 0 and (vector size)
		 */
		template <typename T>
		void swapshift(vector<T>& v, sizet from, sizet to) {
			if (to > from)
				std::rotate(v.begin() + from, v.begin() + from + 1, v.begin() + to);
			else
				std::rotate(v.rend() - from - 1, v.rend() - from, v.rend() - to);
		}
	}
}