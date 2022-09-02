#include <elpch.h>
#include "asset_sync.h"
#include "../common/string_algorithm.h"
#include "../tools/hashset.h"

namespace el
{
	void AssetSync::ignoreConflict(asset<AssetData> data) {
		data.addif<AssetModified>();
		data.remove<AssetConflict>();
	}
		
	void AssetSync::rename(const fio::path& path, string new_name, AssetDatabase& base) {
		if (path.stem() == new_name)
			return;

		auto key = fio::relative(path, gProject.directory);
		if (fio::exists(path)) {
			auto new_key = key.parent_path()
				/ findNonConflictingFilename(fio::directory_entry(path.parent_path()), new_name + key.extension().generic_u8string());
			cout << new_key << endl;

			if (base.names.contains(new_key)) {
				cout << "Rename failed: target name already exists loaded in program. Sync or resolve conflict first." << endl;
			} else {
				if (base.names.contains(key)) {
					cout << key << endl;
					fio::rename(gProject.directory / key, gProject.directory / new_key);
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

	string AssetSync::findNonConflictingFilename(fio::directory_entry dir, fio::path filename) {
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
						hash.emplace(el_string::toSizet(sv.substr(2, 1)));
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

	void AssetSync::eraseUnused(AssetDatabase& base) {
		auto& names = base.names;
		auto& registry = base.registry;

		vector<fio::path> paths;
		for (auto it = names.begin(); it != names.end(); it++) {
			if (!gProject.valid(it.value())) {
				paths.emplace_back(it.key());
			}
		}

		for (auto it = paths.begin(); it != paths.end(); it++) {
			names.erase(*it);
		}

		vector<long long> paths2;
		for (auto it = registry.begin(); it != registry.end(); it++) {
			if (!gProject.valid(it.value())) {
				paths2.emplace_back(it.key());
			}
		}

		for (auto it = paths2.begin(); it != paths2.end(); it++) {
			registry.erase(*it);
		}
	}
}
