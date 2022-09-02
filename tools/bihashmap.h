/*****************************************************************//**
 * @file   bihashmap.h
 * @brief  Bi-directional hash map (or multimap) using two robin hashmaps
 *		   Most probably deprecated
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "hashmap.h"

namespace el
{ 
	using sizet = size_t;
	template<typename Key, typename Data>
	struct bihashmap
	{
		template<typename ...Arg>
		void emplace(const Key& key, Arg ...args) {
			mData.emplace(key, args...);
			mKeys.emplace(args..., key);
		}
		bool contains(const Key& key) {
			return mData.contains(key);
		}
		bool contains(const Data& data) {
			return mKeys.contains(data);
		}

		void erase(const Key& key) {
			mKeys.erase(mData[key]);
			mData.erase(key);
		}
		void erase(const Data& data) {
			mData.erase(mKeys[data]);
			mKeys.erase(data);
		}
		void safe_erase(const Key& key) {
			auto it = mData.find(key);
			if (it != mData.end()) {
				mKeys.erase(it.value());
				mData.erase(key);
			}
		}
		void safe_erase(const Data& data) {
			auto it = mKeys.find(data);
			if (it != mKeys.end()) {
				mData.erase(it.value());
				mKeys.erase(data);
			}
		}
		Key operator[](const Data& data) { return mKeys[data]; }
		Data operator[](const Key& key) { return mData[key]; }

		Key at(const Data& data) { return mKeys.at(data); }
		Data at(const Key& key) { return mData.at(key); }

		Key safe_key(const Data& data) {
			auto it = mKeys.find(data);
			if (it != mKeys.end()) {
				return it.value();
			}
			else return Key();
		}

		Data safe_data(const Key& key) {
			auto it = mData.find(key);
			if (it != mData.end()) {
				return it.value();
			}
			else return Data();
		}

		sizet count() { return mData.size(); }

		template<typename T>
		void serialize(T& archive) {
			archive(mData, mKeys);
		}

		auto begin() { return mData.begin(); }
		auto end() { return mData.end(); }
		void clear() { mData.clear(); mKeys.clear(); }

	private:
		hashmap<Key, Data> mData;
		hashmap<Data, Key> mKeys;
	};
}