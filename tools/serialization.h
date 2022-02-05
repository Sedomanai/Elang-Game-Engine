#pragma once

#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/common.hpp>

#include "hash.h"


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

	template <class A, class T, class U>
	struct cereal::specialize<A, hashmap<T, U>, cereal::specialization::non_member_load_save> {};
	template <class A, class T>
	struct cereal::specialize<A, hashset<T>, cereal::specialization::non_member_load_save> {};

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

}