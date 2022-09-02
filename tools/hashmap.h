/*****************************************************************//**
 * @file   hash.h
 * @brief  Wrapper for tsl/robin hash map
 *		   Completely replaces stl map in case of Elang engine because of its objectively superior speed
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <tsl/robin_map.h>

#ifndef __ELANG_HASHMAP
#define __ELANG_HASHMAP
#endif

namespace el
{ 
	// For some reason cereal cannot recognize tsl::robin_map/set save/load functions but can with my wrapper

	/**
	 * @brief Hashmap. Faster than stl map
	 */
	template<typename T, typename U>
	struct hashmap : tsl::robin_map<T, U> {
		using tsl::robin_map<T, U>::robin_map;
		void operator=(const tsl::robin_map<T, U>& map) { tsl::robin_map<T, U>::operator=(map); }
		void operator=(tsl::robin_map<T, U>&& map) { tsl::robin_map<T, U>::operator=(map); }
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
	template <class A, class T, class U>
	struct cereal::specialize<A, hashmap<T, U>, cereal::specialization::non_member_load_save> {};

	// TSL robin map non-member save function
	template <class A, class T, class U>
	void save(A& archive, const hashmap<T, U>& map) {
		robin_cereal_saver saver(archive);
		map.serialize(saver);
	}
	// TSL robin map non-member load function
	template <class A, class T, class U>
	void load(A& archive, hashmap<T, U>& map) {
		robin_cereal_loader loader(archive);
		map = hashmap<T, U>::deserialize(loader, true);
	}
#endif
}