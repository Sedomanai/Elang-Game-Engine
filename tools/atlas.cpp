#include <elpch.h>
#include "atlas.h"
#include "cell.h"
#include "clip.h"
#include "project.h"
#include "asset_database.h"
#include "hashmap.h"
#include "../common/stream.h"

namespace el {
	void AtlasMeta::clear() {
		users.clear();
		cellorder.clear();
		cliporder.clear();
		cellnames.clear();
		clipnames.clear();
	}
	stream& operator<<(stream& save, const AtlasMeta& am) {
		save << am.width << am.height << am.users;
		return save;
	}
	stream& operator>>(stream& load, AtlasMeta& am) {
		load >> am.width >> am.height >> am.users;
		return load;
	}

	void Atlas::importFile(const fio::path& filePath, AtlasMeta& meta) {
		stream loader;
		loader.toLoad(filePath.generic_u8string().data());

		int32 cellCount;
		loader >> meta >> cellCount;
		for (sizet i = 0; i < (sizet)cellCount; i++) {
			auto cell = gProject.make<SubAssetData>(i, "", meta.self);
			loader >> cell->name;

			auto cell_meta = cell.add<CellMeta>();
			loader >> *cell_meta;

			addCell(cell, meta);
		}

		int32 clipCount;
		loader >> clipCount;

		for (sizet i = 0; i < (sizet)clipCount; i++) {
			auto clip = gProject.make<SubAssetData>(i, "", meta.self);
			clip.add<ClipMeta>();
			auto& frames = *clip.add<Clip>();
			int32 frameCount;

			loader >> clip->name >> frames.speed >> frames.repeat >> frameCount;

			for (sizet i = 0; i < (sizet)frameCount; i++) {
				int32 cellindex;
				loader >> cellindex;
				frames.cells.emplace_back(meta.cellorder[cellindex]);
			}

			meta.cliporder.emplace_back(clip);
			meta.clipnames.emplace(clip, clip->name);
			clips.emplace(clip->name, clip);
		}

		loader.close();
	}

	void Atlas::exportFile(const fio::path& filePath, AtlasMeta& data) {
		stream saver;
		saver.toSave(filePath.generic_u8string().data());

		auto datasize = data.cellorder.size();
		saver << data << (int32)datasize;

		for (sizet i = 0; i < datasize; i++) {
			asset<SubAssetData> cell = data.cellorder[i];
			auto& meta = cell.get<CellMeta>();
			saver << cell->name << meta;
		}

		datasize = data.cliporder.size();
		saver << (int32)datasize;

		for (sizet i = 0; i < datasize; i++) {
			asset<SubAssetData> clip = data.cliporder[i];
			auto& cc = clip.get<Clip>();

			sizet framecount = cc.cells.size();
			saver << clip->name << cc.speed << cc.repeat << (int32)framecount;
			for (sizet i = 0; i < framecount; i++) {
				saver << (int32)asset<SubAssetData>(cc.cells[i])->index;
			}
		}

		saver.close();
	}

	void Atlas::unload(AtlasMeta& data) {
		auto& cellorder = data.cellorder;
		for (auto i = 0; i < cellorder.size(); i++) {
			gProject.destroy(cellorder[i]);
		}

		auto& cliporder = data.cliporder;
		for (auto i = 0; i < cliporder.size(); i++) {
			gProject.destroy(cliporder[i]);
		}

		data.clear();
		cells.clear();
		clips.clear();
	}

	void Atlas::addCell(asset<SubAssetData> cell, AtlasMeta& meta) {
		if (cell && cell.has<CellMeta>()) {
			auto& cellmeta = cell.get<CellMeta>();
			cell.add<Cell>(cellmeta, meta.width, meta.height);
			meta.cellorder.emplace_back(cell);
			meta.cellnames.emplace(cell, cell->name);
			cells.emplace(cell->name, cell);
		}
	}
}