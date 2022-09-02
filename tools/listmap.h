/*****************************************************************//**
 * @file   listmap.h
 * @brief  Almost certainly deprecated
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../common/string.h"
#include "../common/container.h"
#include "hashmap.h"

namespace el
{
	using sizet = size_t;
	
	template<typename T>
	struct listmap
	{
		template<typename ...Arg>
		void emplace(const string& key, Arg... arg) {
			mMap.emplace(key, mData.size());
			mData.emplace_back(arg...);
			mNames.emplace_back(key);
		}

		T& operator[](sizet index) {
			return mData[index];
		}
		T& operator[](const string& key) {
			return mData[mMap[key]];
		}

		bool has(const string& str) const {
			return mMap.contains(str);
		}

		sizet count() const { return mData.size(); }

		sizet index(const string& name) const {
			auto it = mMap.find(name);
			if (it != mMap.end())
				return it.value();
			else return -1;
		}

		void clear() {
			mData.clear();
			mNames.clear();
			mMap.clear();
		}

		T& back() {
			return mData.back();
		}

		void erase(sizet index) {
			mMap.erase(mNames[index]);
			mData.erase(mData.begin() + index);
			mNames.erase(mNames.begin() + index);
			for (sizet i = index; i < mData.size(); i++) {
				mMap[mNames[i]] = i;
			}
		}

		void swap(sizet from, sizet to) {
			el_vector::swapshift(mData, from, to);
			el_vector::swapshift(mNames, from, to);
			auto min = from < to ? from : to;
			auto max = from < to ? to : from;
			for (sizet i = min; i <= max; i++) {
				mMap[mNames[i]] = i;
			}
		}

		void rename(sizet index, const string& key) {
			if (!mMap.contains(key)) {
				auto it = mMap.find(mNames[index]);
				auto val = it.value();
				mMap.erase(it);
				mMap[key] = val;
				mNames[index] = key;
			}
		}

		vector<string> names() { return mNames; }

		template<typename T>
		void serialize(T& archive) {
			archive(mData, mNames, mMap);
		}

	private:
		vector<T> mData;
		vector<string> mNames;
		hashmap<string, sizet> mMap;
	};
}