#include <elpch.h>
#include "basic.h"
#include "sprite.h"
#include "../tools/cell.h"
#include "../tools/texture.h"
#include "../tools/atlas.h"
#include "../tools/material.h"
#include "../tools/painter.h"
#include "../tools/stage.h"

namespace el
{
	template struct SpriteType<SpriteVertex>;

	template<typename V>
	SpriteType<V>::SpriteType() : Quad<V>(NullEntity, NullEntity), mFlipped(false) {}

	template<typename V>
	SpriteType<V>::SpriteType(asset<Material> material_, asset<Painter> painter_, const string& cell)
		: Quad<V>(material_, painter_), mFlipped(false)
	{
		setCell(cell);
	}
	template<typename V>
	void SpriteType<V>::setCell(asset<Cell> cell) { mCell = cell; }
	template<typename V>
	bool SpriteType<V>::hasValidCell() { return mCell; }
	template<typename V>
	asset<Cell> SpriteType<V>::cell() { return mCell; }
	template<typename V>
	void SpriteType<V>::flip() { mFlipped = !mFlipped; }
	template<typename V>
	void SpriteType<V>::flipside(bool invert) { mFlipped = invert; }


	template<typename V>
	void SpriteType<V>::setCell(const string& value) {
		if (Visage::material && Visage::material->hasTexture()) {
			auto tex = Visage::material->textures[0];
			if (tex->atlas) {
				mCell = tex->atlas->cells[value];
			}
		}
	}

	template<typename V>
	void SpriteType<V>::recalc(Position& position) {
		if (mCell) {
			auto& cell = *mCell;
			if (mFlipped) {
				Quad<V>::mVertices[0].pos = vec2(-cell.right, cell.up) + position;
				Quad<V>::mVertices[0].uv = vec2(cell.uvRight, cell.uvUp);
				Quad<V>::mVertices[1].pos = vec2(-cell.left, cell.up) + position;
				Quad<V>::mVertices[1].uv = vec2(cell.uvLeft, cell.uvUp);
				Quad<V>::mVertices[2].pos = vec2(-cell.left, cell.down) + position;
				Quad<V>::mVertices[2].uv = vec2(cell.uvLeft, cell.uvDown);
				Quad<V>::mVertices[3].pos = vec2(-cell.right, cell.down) + position;
				Quad<V>::mVertices[3].uv = vec2(cell.uvRight, cell.uvDown);
			} else {
				Quad<V>::mVertices[0].pos = vec2(cell.left, cell.up) + position;
				Quad<V>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
				Quad<V>::mVertices[1].pos = vec2(cell.right, cell.up) + position;
				Quad<V>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
				Quad<V>::mVertices[2].pos = vec2(cell.right, cell.down) + position;
				Quad<V>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
				Quad<V>::mVertices[3].pos = vec2(cell.left, cell.down) + position;
				Quad<V>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
			}
		} else if (Visage::material && Visage::material->hasTexture()) {
			auto tex = Visage::material->textures[0];
			auto w = float(tex->width());
			auto h = -float(tex->height());
			Quad<V>::mVertices[0].pos = vec2(0.0f, h) + position;
			Quad<V>::mVertices[0].uv = vec2(0.0f, 1.0f);
			Quad<V>::mVertices[1].pos = vec2(w, h) + position;
			Quad<V>::mVertices[1].uv = vec2(1.0f, 1.0f);
			Quad<V>::mVertices[2].pos = vec2(w, 0.0f) + position;
			Quad<V>::mVertices[2].uv = vec2(1.0f, 0.0f);
			Quad<V>::mVertices[3].pos = position;
			Quad<V>::mVertices[3].uv = vec2(0.0f, 0.0f);
		}
	}

	template<typename V>
	void SpriteType<V>::recalc(Planar& plane) {
		if (mCell) {
			auto& cell = *mCell;
			Quad<V>::mVertices[0].pos = plane * vec2(cell.left, cell.up);
			Quad<V>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
			Quad<V>::mVertices[1].pos = plane * vec2(cell.right, cell.up);
			Quad<V>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
			Quad<V>::mVertices[2].pos = plane * vec2(cell.right, cell.down);
			Quad<V>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
			Quad<V>::mVertices[3].pos = plane * vec2(cell.left, cell.down);
			Quad<V>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
		}
		else {
			auto tex = Visage::material->textures[0];
			auto w = float(tex->width()) / 2.0f;
			auto h = float(tex->height()) / 2.0f;
			Quad<V>::mVertices[0].pos = plane * vec2(-w, -h);
			Quad<V>::mVertices[0].uv = vec2(0.0f, 1.0f);
			Quad<V>::mVertices[1].pos = plane * vec2(w, -h);
			Quad<V>::mVertices[1].uv = vec2(1.0f, 1.0f);
			Quad<V>::mVertices[2].pos = plane * vec2(w, h);
			Quad<V>::mVertices[2].uv = vec2(1.0f, 0.0f);
			Quad<V>::mVertices[3].pos = plane * vec2(-w, h);
			Quad<V>::mVertices[3].uv = vec2(0.0f, 0.0f);
		}
	}

	template<typename V>
	void SpriteType<V>::recalc(Entity  e) {
		if (Visage::material && Visage::material->hasTexture() && gStage->valid(e)) {
			if (gStage->any_of<Planar>(e)) {
				recalc(gStage->get<Planar>(e));
			} else if (gStage->any_of<Position>(e)) {
				recalc(gStage->get<Position>(e));
			}
		}
	}
}