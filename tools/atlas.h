/*****************************************************************//**
 * @file   atlas.h
 * @brief  Tools for importing and exporting sprite atlas 
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "../common/container.h"
#include "registry.h"
#include "asset_data.h"

namespace el
{
	struct Atlas;
	struct Texture;

	struct Cell;
	struct CellMeta
	{
#pragma warning( push )
#pragma warning( disable : 26495 )
		CellMeta() {}
#pragma warning( pop )

		CellMeta(sizet x_, sizet y_, sizet w_, sizet h_, sizet oX_, sizet oY_) 
			: x((int32)x_), y((int32)y_), w((int32)w_), h((int32)h_), oX((int32)oX_), oY((int32)oY_) {}

		friend stream& operator<<(stream& save, const CellMeta& cc) {
			save << cc.x << cc.y << cc.w << cc.h << cc.oX << cc.oY;
			return save;
		}
		friend stream& operator>>(stream& load, CellMeta& cc) {
			load >> cc.x >> cc.y >> cc.w >> cc.h >> cc.oX >> cc.oY;
			return load;
		}

		int16 x, y, w, h, oX, oY;
	};

	struct Cell
	{
		Cell(const CellMeta& cc, sizet aw, sizet ah) :
			left((float)-cc.oX), down((float)cc.oY - cc.h), right((float)(-cc.oX + cc.w)), up((float)(cc.oY)),
			uvLeft((float)cc.x / (float)aw),
			uvDown((float)(cc.y + cc.h) / (float)ah),
			uvRight((float)(cc.x + cc.w) / (float)aw),
			uvUp((float)cc.y / (float)ah) {};

		void mold(const CellMeta& cc, sizet aw, sizet ah) {
			left = (float)-cc.oX;
			down = (float)cc.oY - cc.h;
			right = (float)(-cc.oX + cc.w);
			up = (float)(cc.oY);
			uvLeft = (float)cc.x / (float)aw;
			uvDown = (float)(cc.y + cc.h) / (float)ah;
			uvRight = (float)(cc.x + cc.w) / (float)aw;
			uvUp = (float)cc.y / (float)ah;
		}
		
		float left, down, right, up;
		float uvLeft, uvDown, uvRight, uvUp;

		template<typename T>
		void serialize(T& archive) {
			archive(left, down, right, up, uvLeft, uvDown, uvRight, uvUp);
		}
	};

	struct ClipMeta { sizet i; };
	struct Clip : vector<asset<Cell>>
	{
		float speed;
		bool repeat;

		Clip() : vector<asset<Cell>>(), speed(1.0f), repeat(false) {}

		template<typename T>
		void serialize(T& archive) {
			archive(cereal::base_class<vector<asset<Cell>>>(this), speed, repeat);
		}
	};

	struct AtlasMeta
	{
		friend struct Atlas;

		Entity self;
		sizet width, height;
		vector<string> users;
		vector<Entity> cellorder, cliporder;
		hashmap<Entity, string> cellnames, clipnames;

		friend stream& operator<<(stream& save, const AtlasMeta& am) {
			save << am.width << am.height << am.users;
			return save;
		}
		friend stream& operator>>(stream& load, AtlasMeta& am) {
			load >> am.width >> am.height >> am.users;
			return load;
		}
	private:
		void clear() {
			users.clear();
			cellorder.clear();
			cliporder.clear();
			cellnames.clear();
			clipnames.clear();
		}
	};

	struct Atlas
	{
		friend struct Texture;

		hashmap<string, Entity> cells, clips;
		Atlas() {}

		void importFile(const fio::path& filePath, AtlasMeta& meta) {
			stream loader;
			loader.toLoad(filePath.generic_u8string().data());

			int32 cellCount;
			loader >> meta >> cellCount;
			for (sizet i = 0; i < (sizet)cellCount; i++) {
				auto cell = gProject->make<SubAssetData>(i, "", meta.self);
				loader >> cell->name;

				auto cell_meta = cell.add<CellMeta>();
				loader >> *cell_meta;

				addCell(cell, meta);
			}

			int32 clipCount;
			loader >> clipCount;

			for (sizet i = 0; i < (sizet)clipCount; i++) {
				auto clip = gProject->make<SubAssetData>(i, "", meta.self);
				clip.add<ClipMeta>();
				auto& frames = *clip.add<Clip>();
				int32 frameCount;

				loader >> clip->name >> frames.speed >> frames.repeat >> frameCount;

				for (sizet i = 0; i < (sizet)frameCount; i++) {
					int32 cellindex;
					loader >> cellindex;
					frames.emplace_back(meta.cellorder[cellindex]);
				}

				meta.cliporder.emplace_back(clip);
				meta.clipnames.emplace(clip, clip->name);
				clips.emplace(clip->name, clip);
			}

			loader.close();
		}

		void exportFile(const fio::path& filePath, AtlasMeta& data) {
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

				sizet framecount = cc.size();
				saver << clip->name << cc.speed << cc.repeat << (int32)framecount;
				for (sizet i = 0; i < framecount; i++) {
					saver << (int32)asset<SubAssetData>(cc[i])->index;
				}
			}

			saver.close();
		}

		void unload(AtlasMeta& data) {
			auto& cellorder = data.cellorder;
			for (auto i = 0; i < cellorder.size(); i++) {
				gProject->destroy(cellorder[i]);
			}

			auto& cliporder = data.cliporder;
			for (auto i = 0; i < cliporder.size(); i++) {
				gProject->destroy(cliporder[i]);
			}

			data.clear();
			cells.clear();
			clips.clear();
		}

		template<typename T>
		void serialize(T& archive) {
			archive(cells, clips);
		}

		void addCell(asset<SubAssetData> cell, AtlasMeta& meta) {
			if (cell && cell.has<CellMeta>()) {
				auto& cellmeta = cell.get<CellMeta>();
				cell.add<Cell>(cellmeta, meta.width, meta.height);
				meta.cellorder.emplace_back(cell);
				meta.cellnames.emplace(cell, cell->name);
				cells.emplace(cell->name, cell);
			}
		}
	};
}