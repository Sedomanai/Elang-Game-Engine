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

	/**
	* Rotate vectors
	* Useful for list UIs where there's a lot of mouse input reordering
	*/
	template <typename T>
	void swapshift(vector<T>& v, sizet from, sizet to) {
		if (to > from)
			std::rotate(v.begin() + from, v.begin() + from + 1, v.begin() + to + 1);
		else
			std::rotate(v.rend() - from - 1, v.rend() - from, v.rend() - to);
	}
}