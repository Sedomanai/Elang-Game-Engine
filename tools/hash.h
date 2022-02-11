#pragma once

#include "../common/container.h"

#include <tsl/robin_map.h>
#include <tsl/robin_set.h>

namespace el
{ 
	// For SOME reason cereal cannot recognize tsl::robin_map/set save/load functions but can with my wrapper
	// In other words, the following does not work. I seriously do not know why but hey.
	// **
	//template<typename T, typename U>
	//using hashmap = tsl::robin_map<T, U>;
	//template<typename T>
	//using hashset = tsl::robin_set<T>;

	template<typename T, typename U>
	struct hashmap : tsl::robin_map<T, U> {
		using tsl::robin_map<T, U>::robin_map;
		void operator=(const tsl::robin_map<T, U>& map) { tsl::robin_map<T, U>::operator=(map); }
		void operator=(tsl::robin_map<T, U>&& map) { tsl::robin_map<T, U>::operator=(map); }
	};
	template<typename T>
	struct hashset : tsl::robin_set<T> {
		using tsl::robin_set<T>::robin_set;
		void operator=(const tsl::robin_set<T>& map) { tsl::robin_map<T>::operator=(map); }
		void operator=(tsl::robin_set<T>&& map) { tsl::robin_map<T>::operator=(map); }
	};

	// Bi-directional hash map (or multimap) using two robin hashmaps
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

	//인덱스 변동이 필요할 때 리스트처럼 사용할 수 있는 맵
	//이름이 필요한 GUI 엔진이랑 필요없는 게임 클래스와 연동할 때 쓴다
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
			swapshift(mData, from, to);
			swapshift(mNames, from, to);
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