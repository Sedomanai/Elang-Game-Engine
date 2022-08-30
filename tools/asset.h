#pragma once
#include <common/fileio.h>
#include <entt/entt.hpp>

#include "../elang_library_builder.h"
#include "hash.h"

namespace el
{
	struct AssetData
	{
		int64 id;
		fio::path filePath;
		fio::file_time_type lastWriteTime;
		sizet userCount;

		AssetData(int64 id_, const fio::path& filePath_, fio::file_time_type lastWriteTime_) : id(id_), filePath(filePath_), lastWriteTime(lastWriteTime_), userCount(0) {}
	};

	struct SubAssetData
	{
		sizet index;
		string name;
		entt::entity parent;

		SubAssetData(sizet index_, strview name_, entt::entity parent_) : index(index_), name(name_), parent(parent_) {}
	};

    struct AssetDatabase
    {
        hashmap<long long, entt::entity> registry;
        hashmap<fio::path, entt::entity> names;

		void clear() {
			registry.clear();
			names.clear();
		}
    };

	struct AssetLoaded {};
	struct AssetModified {};
	struct AssetConflict { AssetConflict(bool erased_ = false) : erased(erased_) {}; bool erased; };
	struct AssetSynced {};
	struct AssetErased {};
}
