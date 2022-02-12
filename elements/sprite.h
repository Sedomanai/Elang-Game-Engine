#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	
	template<typename U, typename V>
	struct SpriteImpl : std::enable_if_t<std::is_base_of_v<Material, U>&& std::is_base_of_v<Painter, V>>, 
		Quad<SpriteVertex, U, V>
	{
		SpriteImpl() : Quad<SpriteVertex, U, V>() {};
		SpriteImpl(asset<U> material, asset<V> painter, const string& cell_key);

		void update(Entity e);
		void setCell(const string& value);
		void flip() { mFlipped = !mFlipped; }
		void flipside(bool invert) { mFlipped = invert; }

		template<typename T>
		void serialize(T& archive) {
			archive(Visage<U, V>::material, Visage<U, V>::painter, mCell, mFlipped);
		}
	protected:
		void updatePositionSprite(obj<Position> position);
		void updatePlanarSprite(obj<Planar> plane);

		asset<Cell> mCell;
		bool mFlipped;
	};


	template<typename U, typename V>
	inline SpriteImpl<U, V>::SpriteImpl(asset<U> material_, asset<V> painter_, const string& cell)
		: Quad<SpriteVertex, U, V>(material_, painter_), mFlipped(false)
	{
		setCell(cell);
	}
	template<typename U, typename V>
	inline void SpriteImpl<U, V>::setCell(const string& value) {
		if (Visage<U, V>::material && Visage<U, V>::material->hasTexture()) {
			auto tex = Visage<U, V>::material->textures[0];
			if (tex->atlas) {
				mCell = tex->atlas->cells.safe_data(value);
			}
		}
	}

	template<typename U, typename V>
	inline void SpriteImpl<U, V>::updatePositionSprite(obj<Position> position) {
		if (position) {
			auto& pos = *position;
			if (mCell) {
				auto& cell = *mCell;
				if (mFlipped) {
					Quad<SpriteVertex, U, V>::mVertices[0].pos = vec2(cell.right, cell.up) + pos;
					Quad<SpriteVertex, U, V>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					Quad<SpriteVertex, U, V>::mVertices[1].pos = vec2(cell.left, cell.up) + pos;
					Quad<SpriteVertex, U, V>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					Quad<SpriteVertex, U, V>::mVertices[2].pos = vec2(cell.left, cell.down) + pos;
					Quad<SpriteVertex, U, V>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					Quad<SpriteVertex, U, V>::mVertices[3].pos = vec2(cell.right, cell.down) + pos;
					Quad<SpriteVertex, U, V>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				}
				else {
					Quad<SpriteVertex, U, V>::mVertices[0].pos = vec2(cell.left, cell.up) + pos;
					Quad<SpriteVertex, U, V>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					Quad<SpriteVertex, U, V>::mVertices[1].pos = vec2(cell.right, cell.up) + pos;
					Quad<SpriteVertex, U, V>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					Quad<SpriteVertex, U, V>::mVertices[2].pos = vec2(cell.right, cell.down) + pos;
					Quad<SpriteVertex, U, V>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					Quad<SpriteVertex, U, V>::mVertices[3].pos = vec2(cell.left, cell.down) + pos;
					Quad<SpriteVertex, U, V>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				}
			}
			else {
				auto tex = Visage<U, V>::material->textures[0];
				auto w = float(tex->width());
				auto h = -float(tex->height());
				Quad<SpriteVertex, U, V>::mVertices[0].pos = vec2(0.0f, h) + pos;
				Quad<SpriteVertex, U, V>::mVertices[0].uv = vec2(0.0f, 1.0f);
				Quad<SpriteVertex, U, V>::mVertices[1].pos = vec2(w, h) + pos;
				Quad<SpriteVertex, U, V>::mVertices[1].uv = vec2(1.0f, 1.0f);
				Quad<SpriteVertex, U, V>::mVertices[2].pos = vec2(w, 0.0f) + pos;
				Quad<SpriteVertex, U, V>::mVertices[2].uv = vec2(1.0f, 0.0f);
				Quad<SpriteVertex, U, V>::mVertices[3].pos = pos;
				Quad<SpriteVertex, U, V>::mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	template<typename U, typename V>
	inline void SpriteImpl<U, V>::updatePlanarSprite(obj<Planar> plane) {
		if (plane) {
			auto& tr = *plane;
			if (mCell) {
				auto& cell = *mCell;
				Quad<SpriteVertex, U, V>::mVertices[0].pos = tr * vec2(cell.left, cell.up);
				Quad<SpriteVertex, U, V>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
				Quad<SpriteVertex, U, V>::mVertices[1].pos = tr * vec2(cell.right, cell.up);
				Quad<SpriteVertex, U, V>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
				Quad<SpriteVertex, U, V>::mVertices[2].pos = tr * vec2(cell.right, cell.down);
				Quad<SpriteVertex, U, V>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
				Quad<SpriteVertex, U, V>::mVertices[3].pos = tr * vec2(cell.left, cell.down);
				Quad<SpriteVertex, U, V>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
			}
			else {
				auto tex = Visage<U, V>::material->textures[0];
				auto w = float(tex->width()) / 2.0f;
				auto h = float(tex->height()) / 2.0f;
				Quad<SpriteVertex, U, V>::mVertices[0].pos = tr * vec2(-w, -h);
				Quad<SpriteVertex, U, V>::mVertices[0].uv = vec2(0.0f, 1.0f);
				Quad<SpriteVertex, U, V>::mVertices[1].pos = tr * vec2(w, -h);
				Quad<SpriteVertex, U, V>::mVertices[1].uv = vec2(1.0f, 1.0f);
				Quad<SpriteVertex, U, V>::mVertices[2].pos = tr * vec2(w, h);
				Quad<SpriteVertex, U, V>::mVertices[2].uv = vec2(1.0f, 0.0f);
				Quad<SpriteVertex, U, V>::mVertices[3].pos = tr * vec2(-w, h);
				Quad<SpriteVertex, U, V>::mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	template<typename U, typename V>
	inline void SpriteImpl<U, V>::update(Entity e) {
		if (Visage<U, V>::material && Visage<U, V>::material->hasTexture()) {
			updatePositionSprite(e);
			updatePlanarSprite(e);
		}
	}

	using Sprite = SpriteImpl<Material, Painter>;
}
