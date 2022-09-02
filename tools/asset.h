/*****************************************************************//**
 * @file   asset.h
 * @brief  All Asset related types.
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <entt/entt.hpp>
#include <filesystem>
#include "hashmap.h"
#include "../common/define.h"
#include "../common/string.h"

namespace el
{
	using Entity = entt::entity;
	namespace fio = std::filesystem;
	using string = std::string;

	/**
	 * @brief All user-based assets must hold this element.
	 * @brief It is used to hold original file data in memory and to sync with it when necessary
	 */
	struct AssetData
	{
		// This is not user given but the INODE (unique identifier) of the file which ways of getting it is platform dependent
		// It's used to detect name changes and/or deletion of original file
		int64 inode;
		// The filePath should be relative (to the project folder)
		fio::path filePath;
		// Last write time of the file this asset is synced to. 
		fio::file_time_type lastWriteTime;
		sizet userCount;

		AssetData(int64 inode_, const fio::path& filePath_, fio::file_time_type lastWriteTime_) : inode(inode_), filePath(filePath_), lastWriteTime(lastWriteTime_), userCount(0) {}
	};

	/**
	 * @brief All sub assets of an asset (such as a cell of an atlas) must hold this element.
	 */
	struct SubAssetData
	{
		// Identifiable unique index that separates itself from other sub assets of the same parent
		sizet index;
		// Identifiable unique name that separates itself from other sub assets of the same parent
		string name;
		// The parent Asset that this Sub Asset belongs to
		entt::entity parent;

		SubAssetData(sizet index_, strview name_, entt::entity parent_) : index(index_), name(name_), parent(parent_) {}
	};

	/**
	 * @brief Asset Database. Can get any Asset via its relative file path and/or inode
	 * @brief Can also be used
	 */
	struct AssetDatabase
	{
		hashmap<long long, Entity> registry;
		hashmap<fio::path, Entity> names;

		void clear() {
			registry.clear();
			names.clear();
		}
	};

	// This is attached to all opened/loaded/imported Assets
	struct AssetLoaded {};
	// All AssetModified Assets should have an AssetLoaded attached (if not, it's not designed right)
	struct AssetModified {};
	// This is attached when a file that is internally modified is also externally modified.
	// All AssetConflict Assets should have an AssetModified/AssetLoaded attached.
	struct AssetConflict { AssetConflict(bool erased_ = false) : erased(erased_) {}; bool erased; };
	// This is attached during recursive file navigation, checking whether file exists. 
	// All existing ones have AssetSynced attached.
	struct AssetSynced {};
	// This is used internally during the loop that checks whether Asset is synced. 
	// If not, AssetErased is attached.
	struct AssetErased {};
	// Any Asset type that does not require a meta. It's unfortuantely necessary for template reasons
	struct NullMeta { sizet meta; };
}
