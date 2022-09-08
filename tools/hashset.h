/*****************************************************************//**
 * @file   hash.h
 * @brief  Wrapper for tsl/robin hash set
 *		   Completely replaces stl set in case of Elang engine because of its objectively superior speed
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <tsl/robin_set.h>

#ifndef __ELANG_HASHSET
#define __ELANG_HASHSET
#endif

namespace el
{ 
	// For some reason cereal cannot recognize tsl::robin_map/set save/load functions but can with my wrapper

	/**
	 * @brief Hashset. Faster than stl set
	 */
	template<typename T>
	struct hashset : tsl::robin_set<T> {
		using tsl::robin_set<T>::robin_set;
		void operator=(const tsl::robin_set<T>& map) { tsl::robin_set<T>::operator=(map); }
		void operator=(tsl::robin_set<T>&& map) { tsl::robin_set<T>::operator=(map); }
	};

#ifdef __ELANG_CEREAL
#ifndef __ELANG_HASH_SERIALIZED
	template<class A>
	struct robin_cereal_saver
	{
		A& archive;
		robin_cereal_saver(A& archive_) : archive(archive_) {};
		template<typename T>
		void operator()(const T& t) { archive(t); };
	};

	// deserializer that follows the robin guidelines
	template<class A>
	struct robin_cereal_loader
	{
		A& archive;
		robin_cereal_loader(A& archive_) : archive(archive_) {};
		template<typename T>
		auto operator()() { T t; archive(t); return t; };
	};
#define __ELANG_HASH_SERIALIZED
#endif
	template <class A, class T>
	struct cereal::specialize<A, hashset<T>, cereal::specialization::non_member_load_save> {};

	// TSL robin set non-member save function
	template <class A, class T>
	void save(A& ar, const hashset<T>& set) {
		robin_cereal_saver saver(ar);
		set.serialize(saver);
	}
	// TSL robin set non-member load function
	template <class A, class T>
	void load(A& archive, hashset<T>& set) {
		robin_cereal_loader loader(archive);
		set = hashset<T>::deserialize(loader);
	}
#endif
}