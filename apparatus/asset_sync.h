/*****************************************************************//**
 * @file   asset_sync.h
 * @brief  Used to sync assets with their actual files. 
 *         Listens to file creation, modification, and erasure and deals accordingly.
 *		   For specific behavior, see the little footnote below or check the method descriptions. 
 * 
 *		   While it has the functionality to import, save, and export, it does not do it automatically.
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/tool_declarer.h"
#include "../tools/project.h"
#include "../tools/asset_database.h"

namespace el
{
	struct AssetLoader;

	/**
	 * wanted behavior during sync

		explorer (syncAsset method)
		create file - sync in editor 
		erase file - sync in editor, unless already loaded in editor (has AssetLoaded), then don't sync (+ AssetConflict, erased true)
		modify file - sync in editor, unless already modified in editor (has AssetModified), then don't sync (+ AssetConflict)
		rename file - sync in editor, keep the entity index
		create file to erased file's name - sync created file to erased file's entity index (if AssetLoaded but not AssetModified, reload, else if AssetLoaded, +AssetConflict)
		rename file to erased file's name - sync renamed file to erased file's entity index (if AssetLoaded but not AssetModified, reload, else if AssetLoaded, +AssetConflict)

		editor
		create file - immediately sync in explorer
		erase file - immediately sync in explorer
		modify file - wait until save, enable only when loaded (AssetModified) (user-based, use with care)
		open file - if unloaded, load (AssetLoaded)
		close file = if loaded, unload (-AssetLoaded, - AssetModified, -AssetConflict), make sure to double check to save before closing
		save file - if modified, sync in editor (-AssetModified, -AssetConflict)
		revert file - if conflicting (has AssetConflict), then call 

		how to resolve asset conflict
		 1. ask immediately upon sync (recommended, for every file, choice to save, load, ignore) (chosen)
		 2. manual revert (user choice)

		sync when ? 
		 1. upon program window focus (recommended, may have a miniscule chance of slowing down) (chosen)
		 2. upon ctrl + r (may be buggy)
	 */

	/**
	 * @brief Used to sync any asset. For more specific behaviors, check its methods.
	 *		  Because this uses ECS style registry, syncing the Asset will automatically and immediately register 
	 *		  in every element using the Asset
	 * 
	 * @brief 
	 */
	struct ELANG_DLL AssetSync
	{
		friend struct AssetLoader;

		// Sync asset. If the original file is new, enlist the new file data. 
		// If the original file is erased, internal file data is also erased (and of course unloaded)
		// If the original file was modified/renamed, the internal file is also reloaded automatically
		// If the original file was erased but a new file took its place with the same name, that asset will sync with internal data
		// When the file is both internally and externally modified, this will attach AssetConflict
		// 
		// ** Consider calling this via AssetLoader with the syncAllAssets method
		template<typename T, typename M>
		asset<T> syncAsset(const fio::path& path, AssetDatabase& reg) {
			auto id = el_file::identifier(path);
			auto key = fio::relative(path, gProject.directory);

			auto& bish = reg.names;
			auto& bishi = reg.registry;

			if (bishi.contains(id)) {
				auto data = asset<AssetData>(bishi[id]);
				if (bish.contains(key)) {
					asset<AssetData> named = bish[key];
					if (named == data) { // file exists, name unchanged
						checkModification<T, M>(data, path);
						return data;
					} else { // when name changed to the same as an erased file, reimport
						// erase previous name
						auto erased = named;
						if (erased) { // double check
							auto eid = erased->inode;
							bishi[id] = erased;
							if (bishi[eid] == erased)
								bishi[eid] = NullEntity;
							*erased = { id, key, fio::last_write_time(path) };
							sync<T, M>(erased, path);
						} else {
							cout << "this shouldn't happen" << endl;
						}
						return erased;
					}
				} else {
					if (bish.contains(data->filePath)) {
						bish.erase(data->filePath);
					} else {
						cout << "this shouldn't happen" << endl;
					}

					bish.emplace(key, data);
					data->filePath = key;
					checkModification<T, M>(data, path);
					return data;
				}
			} else {
				// entry point when someone erased the original and created a new file with the same name
				// treat as if it's the same file
				if (bish.contains(key)) {
					auto prev = asset<AssetData>(bish[key]);
					if (prev) { // double check, would always be true
						if (bishi[prev->inode] == prev)
							bishi[prev->inode] = NullEntity;
						bishi[id] = prev;
						*prev = { id, key, fio::last_write_time(path) };
						sync<T, M>(prev, path);
						return prev;
					} else {
						cout << "this shouldn't happen" << endl;
						return NullEntity;
					}
					return prev;
				} else { // completely new read
					auto dat = gProject.make<AssetData>(id, key, fio::file_time_type());
					dat.add<M>().add<T>();
					dat->lastWriteTime = fio::last_write_time(path);

					bishi.emplace(id, dat);
					bish.emplace(key, dat);
					return dat;
				}
			}
		}

		// Create a file and sync
		template<typename T, typename M>
		void create(const fio::path& path, AssetDatabase& base) {
			auto key = fio::relative(path, gProject.directory);
			key = key.parent_path() / findNonConflictingFilename(fio::directory_entry(path.parent_path()), path.filename());
			auto new_path = gProject.directory / key;

			if (!syncedHardPath<T, M>(new_path, base)) {
				auto meta = gProject.make<M>();
				auto txt = meta.add<T>();
				fio::create_directories(new_path.parent_path());
				txt->exportFile(new_path, *meta);
				auto id = el_file::identifier(new_path);
				auto data = meta.add<AssetData>(id, key, fio::last_write_time(new_path));
				base.registry.emplace(id, data);
				base.names.emplace(key, data);
			} else {
				cout << "An asset holding the erased file of the same name already exists. Consider resolving the conflict first." << endl;
			}
		}

		// Erase a file and sync in editor, unless already loaded in editor (has AssetLoaded), then don't sync (+ AssetConflict)
		template<typename T, typename M>
		void erase(const fio::path& path, AssetDatabase& base) {
			if (syncedHardPath<T, M>(path, base)) {
				auto key = fio::relative(path, gProject.directory);
				if (base.names.contains(key)) {
					asset<T> data = base.names[key];
					base.names.erase(key);
					auto id = asset<AssetData>(data)->inode;
					if (base.registry.contains(id))
						base.registry.erase(id);
					data->unload(data.get<M>());
					data.destroy();
					fio::remove(path);
				}
			}
		}

		// Attach AssetModified. This must be done manually. You can also do this directly via gProject
		template<typename T>
		void modify(asset<T> data) {
			data.addif<AssetModified>();
		}

		// If unloaded, load (+ AssetLoaded)
		// If original file no longer exists, proceed to erase internal data
		template<typename T, typename M>
		asset<T> open(const fio::path& key, AssetDatabase& base) {
			if (base.names.contains(key)) {
				asset<T> t = base.names[key];
				if (!t.has<AssetLoaded>()) {
					auto path = gProject.directory / key;
					if (syncedHardPath<T, M>(path, base)) {
						t->importFile(path, t.get<M>());
						t.add<AssetLoaded>();
						return t;
					}
				};
			}

			return asset<T>();
		}

		// Load All
		template<typename T, typename M>
		void openAll(AssetDatabase& base) {
			for (asset<AssetData> data : gProject.view<T>()) {
				if (data) {
					open<T, M>(data->filePath, base);
				}
			}
		}

		// If loaded, unload (-AssetLoaded, -AssetModified, -AssetConflict) 
		// This does not check whether file was saved. Make sure to double check)
		// If original file no longer exists, proceed to erase internal data
		template<typename T, typename M>
		void close(const fio::path& key, AssetDatabase& base) {
			if (base.names.contains(key)) {
				asset<T> t = base.names[key];
				if (t.has<AssetLoaded>()) {
					t->unload(t.get<M>());
					t.remove<AssetLoaded>();
				}
				if (t.has<AssetModified>())
					t.remove<AssetModified>();
				if (t.has<AssetConflict>())
					t.remove<AssetConflict>();
			}

			auto path = gProject.directory / key;
			syncedHardPath<T, M>(path, base);
		}

		// If modified, export current file (-AssetModified, -AssetConflict)
		template<typename T, typename M>
		void save(asset<T> data) {
			if (data && data.has<AssetModified>()) {
				auto path = gProject.directory / data.get<AssetData>().filePath;
				unsafeSave<T, M>(path, data);
				data.remove<AssetModified>();
			}
		}

		// If modified, import current file (-AssetModified, -AssetConflict)
		// If original file no longer exists, proceed to erase internal data
		template<typename T, typename M>
		void revert(asset<AssetData> data, AssetDatabase& base) {
			//if (data.has<AssetModified>())
				data.remove<AssetModified>();
			//if (data.has<AssetConflict>())
				data.remove<AssetConflict>();

			auto path = gProject.directory / data->filePath;
			if (syncedHardPath<T, M>(path, base)) {
				auto& t = data.get<T>();
				auto& m = data.get<M>();
				t.unload(m);
				t.importFile(path, m);
			}
		}

		// If AssetConflict is attached, simply remove and don't do anything
		// It's a semantics issue; nothing happens, the file would simply no longer be considered conflicting
		void ignoreConflict(asset<AssetData> data);

		// Rename file directly from here
		void rename(const fio::path& path, string new_name, AssetDatabase& base);

		// Find non-conflicting name of a directory (probably OS dependend, currently only Windows style)
		string findNonConflictingFilename(fio::directory_entry dir, fio::path filename);
	private:
		void eraseUnused(AssetDatabase& base);

		template<typename T, typename M>
		void unsafeSave(const fio::path& path, asset<T> data) {
			data->exportFile(path, data.get<M>());
			auto& d = data.get<AssetData>();
			d.lastWriteTime = fio::last_write_time(path);
			d.inode = el_file::identifier(path);
			if (data.has<AssetConflict>())
				data.remove<AssetConflict>();
		}

		template<typename T, typename M>
		void sync(asset<AssetData> data, const fio::path& path) {
			if (data.has<AssetLoaded>()) {
				if (data.has<AssetModified>())
					data.replace<AssetConflict>(false);
				else {
					auto& t = data.get<T>();
					auto& m = data.get<M>();
					t.unload(m);
					t.importFile(path, m);
				}
			}
		}

		template<typename T, typename M>
		void checkModification(asset<AssetData> data, const fio::path& path) {
			auto lwt = fio::last_write_time(path);
			if (lwt > data->lastWriteTime && data.has<AssetLoaded>()) {
				if (data.has<AssetModified>()) {
					gProject.emplace_or_replace<AssetConflict>(data, false);
				} else {
					auto& t = data.get<T>();
					auto& m = data.get<M>();
					t.unload(m);
					t.importFile(path, m);
					data->lastWriteTime = lwt;
				}
			}
		}

		template<typename T, typename M>
		bool syncedHardPath(const fio::path& path, AssetDatabase& base) {
			auto key = fio::relative(path, gProject.directory);
			if (base.names.contains(key)) {
				if (!fio::exists(path)) {
					auto data = asset<AssetData>(base.names[key]);
					base.names.erase(key);
					if (data) {
						base.registry.erase(data->inode);
						data.get<T>().unload(data.get<M>());
						data.destroy();
					} return false;
				} return true;
			} else if (fio::exists(path)) {
				syncAsset<T, M>(path, base);
				return true;
			}
			return false;
		}

	};
}
