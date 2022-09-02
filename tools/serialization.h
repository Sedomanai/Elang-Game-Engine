/*****************************************************************//**
 * @file   serialization.h
 * @brief  This is for most serializations between c++ programs
 *		   Internally uses Cereal
 * 
 *	       Since it's hard to find the exact parsing methods of Cereal,
 *		   It is only used for serializing between c++ programs (which is most of the time)
 *		   To export to other languages/platforms, try using el::stream instead
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/common.hpp>
#include <fstream>

#include "../common/define.h"
#include "../common/string.h"

#ifndef __ELANG_CEREAL
#define __ELANG_CEREAL
#endif

namespace el
{
	struct OutArchive
	{
		OutArchive(const string& path) : ofs(path, std::ios::binary), cereal(ofs) {}
		void flush() { ofs.flush(); }
		bool valid() { return ofs.good(); }
		sizet size() { return ofs.tellp(); }

		template<typename T>
		void operator()(const T& t) { cereal(t); }
		template<typename T, typename ...Arg>
		void operator()(const T& t, Arg&... arg) { cereal(t); operator()(arg...); }

	protected:
		cereal::BinaryOutputArchive cereal;
		std::ofstream ofs;
	};

	struct InArchive
	{
		InArchive(const std::string& path) : ifs(path, std::ios::binary), cereal(ifs) {}
		bool valid() { return ifs.good(); }
		sizet size() { return ifs.tellg(); }

		template<typename T>
		void operator()(T& t) { cereal(t); }
		template<typename T, typename ...Arg>
		void operator()(T& t, Arg&... arg) { cereal(t); operator()(arg...); }

	protected:
		cereal::BinaryInputArchive cereal;
		std::ifstream ifs;

	};

	/////////////// TSL ROBIN //////////////

	// TSL robin map/set has a serialize() method which would clash with the following non-member load/save functions
	// This will tell cereal to use our custom functions instead

#if (defined(__ELANG_HASHMAP) || defined(__ELANG_HASHSET))
#ifndef __ELANG_HASH_SERIALIZED
	// serializer that follows the robin guidelines
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

#ifdef __ELANG_HASHMAP
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

#ifdef __ELANG_HASHSET
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
#endif
}