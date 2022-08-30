/*****************************************************************//**
 * @file   assetsync.h
 * @brief  Used to Sync
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once
#include "../tools/registry.h"
#include "../tools/asset.h"

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
	struct AssetSync
	{
		friend struct AssetLoader;

		template<typename T, typename M>
		asset<T> syncAsset(const fio::path& path, AssetDatabase& reg) {
			auto id = fileIdentifier(path);
			auto key = fio::relative(path, gProject->directory);

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
							auto eid = erased->id;
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
						if (bishi[prev->id] == prev)
							bishi[prev->id] = NullEntity;
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
					auto dat = gProject->make<AssetData>(id, key, fio::file_time_type());
					dat.add<M>().add<T>();
					dat->lastWriteTime = fio::last_write_time(path);

					bishi.emplace(id, dat);
					bish.emplace(key, dat);
					return dat;
				}
			}
		}

		void clean() {
			auto view = gProject->view<AssetData>();
			for (asset<AssetData> data : view) {
				if (!data.has<AssetSynced>()) {
					if (data.has<AssetLoaded>()) {
						gProject->emplace_or_replace<AssetConflict>(data, true);
						data->id = -1;
					} else
						data.add<AssetErased>();
				}
			} gProject->clear<AssetSynced>();

			auto erased = gProject->view<AssetErased>();
			gProject->destroy(erased.begin(), erased.end());

			for (asset<SubAssetData> data : gProject->view<SubAssetData>()) {
				if (gProject->valid(data->parent)) {
					data.add<AssetErased>();
				}
			}
			auto erasedSub = gProject->view<AssetErased>();
			gProject->destroy(erasedSub.begin(), erasedSub.end());

			for (asset<AssetConflict> conf : gProject->view<AssetConflict>()) {
				if (!conf.has<AssetModified>())
					conf.add<AssetModified>();
			}
			eraseUnused(gTexts);
			eraseUnused(gAtlases);
		}

		template<typename T, typename M>
		void create(const fio::path& path, AssetDatabase& base) {
			auto key = fio::relative(path, gProject->directory);
			key = key.parent_path() / findNonConflictingFilename(fio::directory_entry(path.parent_path()), path.filename());
			auto new_path = gProject->directory / key;

			if (!syncedHardPath<T, M>(new_path, base)) {
				auto meta = gProject->make<M>();
				auto txt = meta.add<T>();
				fio::create_directories(new_path.parent_path());
				txt->exportFile(new_path, *meta);
				auto id = fileIdentifier(new_path);
				auto data = meta.add<AssetData>(id, key, fio::last_write_time(new_path));
				base.registry.emplace(id, data);
				base.names.emplace(key, data);
			} else {
				cout << "An asset holding the erased file of the same name already exists. Consider resolving the conflict first." << endl;
			}
		}

		template<typename T, typename M>
		void erase(const fio::path& path, AssetDatabase& base) {
			if (syncedHardPath<T, M>(path, base)) {
				auto key = fio::relative(path, gProject->directory);
				if (base.names.contains(key)) {
					asset<T> data = base.names[key];
					base.names.erase(key);
					auto id = asset<AssetData>(data)->id;
					if (base.registry.contains(id))
						base.registry.erase(id);
					data->unload(data.get<M>());
					data.destroy();
					fio::remove(path);
				}
			}
		}

		template<typename T>
		void modify(asset<T> data) {
			gProject->emplace_or_replace<AssetModified>(data);
		}

		template<typename T, typename M>
		asset<T> open(const fio::path& key, AssetDatabase& base) {
			if (base.names.contains(key)) {
				asset<T> t = base.names[key];
				if (!t.has<AssetLoaded>()) {
					auto path = gProject->directory / key;
					if (syncedHardPath<T, M>(path, base)) {
						t->importFile(path, t.get<M>());
						t.add<AssetLoaded>();
						return t;
					}
				};
			}

			return asset<T>();
		}

		template<typename T, typename M>
		void openAll(AssetDatabase& base) {
			for (asset<AssetData> data : gProject->view<T>()) {
				if (data) {
					open<T, M>(data->filePath, base);
				}
			}
		}

		template<typename T, typename M>
		void close(const fio::path& key, AssetDatabase& base) {
			if (base.names.contains(key)) {
				asset<T> t = base.names[key];
				if (t.has<AssetLoaded>()) {
					t->unload(t.get<M>());
					gProject->remove<AssetLoaded>(t);
				}
				if (t.has<AssetModified>())
					gProject->remove<AssetModified>(t);
				if (t.has<AssetConflict>())
					gProject->remove<AssetConflict>(t);
			}

			auto path = gProject->directory / key;
			syncedHardPath<T, M>(path, base);
		}

		template<typename T, typename M>
		void save(asset<T> data) {
			if (data && data.has<AssetModified>()) {
				auto path = gProject->directory / data.get<AssetData>().filePath;
				unsafeSave<T, M>(path, data);
				data.remove<AssetModified>();
			}
		}

		template<typename T, typename M>
		void revert(asset<AssetData> data, AssetDatabase& base) {
			if (data.has<AssetModified>())
				data.remove<AssetModified>();
			if (data.has<AssetConflict>())
				data.remove<AssetConflict>();

			auto path = gProject->directory / data->filePath;
			if (syncedHardPath<T, M>(path, base)) {
				auto& t = data.get<T>();
				auto& m = data.get<M>();
				t.unload(m);
				t.importFile(path, m);
			}
		}

		void ignoreConflict(asset<AssetData> data) {
			gProject->emplace_or_replace<AssetModified>(data);
			data.remove<AssetConflict>();
		}
		
		void rename(const fio::path& path, string new_name, AssetDatabase& base) {
			if (path.stem() == new_name)
				return;

			auto key = fio::relative(path, gProject->directory);
			if (fio::exists(path)) {
				auto new_key = key.parent_path()
					/ findNonConflictingFilename(fio::directory_entry(path.parent_path()), new_name + key.extension().generic_u8string());
				cout << new_key << endl;

				if (base.names.contains(new_key)) {
					cout << "Rename failed: target name already exists loaded in program. Sync or resolve conflict first." << endl;
				} else {
					if (base.names.contains(key)) {
						cout << key << endl;
						fio::rename(gProject->directory / key, gProject->directory / new_key);
						asset<AssetData> data = base.names[key];
						base.names[new_key] = data;
						data->filePath = new_key;
						base.names.erase(key);
					}
				}
			} else {
				cout << "Path does not exist anymore. Check if external file is renamed/erased, then sync/resolve. Abort." << endl;
			}
		}

		string findNonConflictingFilename(fio::directory_entry dir, fio::path filename) {
			string stem, new_name;
			new_name = filename.filename().generic_u8string();

			// attach " - Copy"
			for (auto e : fio::directory_iterator(dir)) {
				if (!e.is_directory() && filename == e.path().filename()) {
					stem = filename.stem().generic_u8string() + " - Copy";
					break;
				}
			}

			if (!stem.empty()) {
				new_name = stem + filename.extension().generic_u8string();

				vector<string> buffer;
				for (auto e : fio::directory_iterator(dir)) {
					auto fname = e.path().filename();
					if (!e.is_directory()) {
						auto st = fname.stem().generic_u8string();
						if (st.find(stem) == 0) {
							buffer.emplace_back(st);
						}
					}
				}

				if (buffer.size() != 0) {
					hashset<sizet> hash;
					for (auto str : buffer) {
						auto sv = ((strview)str).substr(stem.size());
						if (sv.size() > 2 && sv.substr(0, 2) == " (") {
							hash.emplace(toSizet(sv.substr(2, 1)));
						}
					}

					sizet i = 2;
					while (true) {
						if (!hash.contains(i)) {
							break;
						} i++;
					}

					// attach " (n)"
					stem += " (" + std::to_string(i) + ")";
					new_name = stem + filename.extension().generic_u8string();
				}
			}

			return new_name;
		}
	private:
		template<typename T, typename M>
		void unsafeSave(const fio::path& path, asset<T> data) {
			data->exportFile(path, data.get<M>());
			auto& d = data.get<AssetData>();
			d.lastWriteTime = fio::last_write_time(path);
			d.id = fileIdentifier(path);
			if (data.has<AssetConflict>())
				data.remove<AssetConflict>();
		}

		void eraseUnused(AssetDatabase& base) {
			auto& names = base.names;
			auto& registry = base.registry;

			vector<fio::path> paths;
			for (auto it = names.begin(); it != names.end(); it++) {
				if (!gProject->valid(it.value())) {
					paths.emplace_back(it.key());
				}
			}

			for (auto it = paths.begin(); it != paths.end(); it++) {
				names.erase(*it);
			}

			vector<long long> paths2;
			for (auto it = registry.begin(); it != registry.end(); it++) {
				if (!gProject->valid(it.value())) {
					paths2.emplace_back(it.key());
				}
			}

			for (auto it = paths2.begin(); it != paths2.end(); it++) {
				registry.erase(*it);
			}
		}

		template<typename T, typename M>
		void sync(asset<AssetData> data, const fio::path& path) {
			if (data.has<AssetLoaded>()) {
				if (data.has<AssetModified>())
					gProject->emplace_or_replace<AssetConflict>(data, false);
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
					gProject->emplace_or_replace<AssetConflict>(data, false);
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
			auto key = fio::relative(path, gProject->directory);
			if (base.names.contains(key)) {
				if (!fio::exists(path)) {
					auto data = asset<AssetData>(base.names[key]);
					base.names.erase(key);
					if (data) {
						base.registry.erase(data->id);
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
