#include "sprite.h"

namespace el
{
	template struct SpriteImpl<SpriteVertex, 0, 0, 0>;
	template struct SpriteImpl<SpriteVertex, 1, 1, 1>;
	template struct SpriteImpl<SpriteVertex, 1, 0, 1>;
	template struct SpriteImpl<SpriteVertex, 1, 1, 0>;
	template struct SpriteImpl<SpriteVertex, 1, 0, 0>;

	template<typename V, int M, int T, int C>
	SpriteImpl<V, M, T, C>::SpriteImpl(asset<MaterialImpl<M, T>> material_, asset<PainterImpl<M, T, C>> painter_, const string& cell)
		: Quad<V, M, T, C>(material_, painter_), mFlipped(false)
	{
		setCell(cell);
	}

	template<typename V, int M, int T, int C>
	void SpriteImpl<V, M, T, C>::setCell(const string& value) {
		if (Visage<M, T, C>::material && Visage<M, T, C>::material->hasTexture()) {
			auto tex = Visage<M, T, C>::material->textures[0];
			if (tex->atlas) {
				mCell = tex->atlas->cells.safe_data(value);
			}
		}
	}

	template<typename V, int M, int T, int C>
	void SpriteImpl<V, M, T, C>::updatePositionSprite(obj<Position> position) {
		if (position) {
			auto& pos = *position;
			if (mCell) {
				auto& cell = *mCell;
				if (mFlipped) {
					Quad<V, M, T, C>::mVertices[0].pos = vec2(cell.right, cell.up) + pos;
					Quad<V, M, T, C>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					Quad<V, M, T, C>::mVertices[1].pos = vec2(cell.left, cell.up) + pos;
					Quad<V, M, T, C>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					Quad<V, M, T, C>::mVertices[2].pos = vec2(cell.left, cell.down) + pos;
					Quad<V, M, T, C>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					Quad<V, M, T, C>::mVertices[3].pos = vec2(cell.right, cell.down) + pos;
					Quad<V, M, T, C>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				} else {
					Quad<V, M, T, C>::mVertices[0].pos = vec2(cell.left, cell.up) + pos;
					Quad<V, M, T, C>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
					Quad<V, M, T, C>::mVertices[1].pos = vec2(cell.right, cell.up) + pos;
					Quad<V, M, T, C>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
					Quad<V, M, T, C>::mVertices[2].pos = vec2(cell.right, cell.down) + pos;
					Quad<V, M, T, C>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
					Quad<V, M, T, C>::mVertices[3].pos = vec2(cell.left, cell.down) + pos;
					Quad<V, M, T, C>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
				}
			}
			else {
				auto tex = Visage<M, T, C>::material->textures[0];
				auto w = float(tex->width());
				auto h = -float(tex->height());
				Quad<V, M, T, C>::mVertices[0].pos = vec2(0.0f, h) + pos;
				Quad<V, M, T, C>::mVertices[0].uv = vec2(0.0f, 1.0f);
				Quad<V, M, T, C>::mVertices[1].pos = vec2(w, h) + pos;
				Quad<V, M, T, C>::mVertices[1].uv = vec2(1.0f, 1.0f);
				Quad<V, M, T, C>::mVertices[2].pos = vec2(w, 0.0f) + pos;
				Quad<V, M, T, C>::mVertices[2].uv = vec2(1.0f, 0.0f);
				Quad<V, M, T, C>::mVertices[3].pos = pos;
				Quad<V, M, T, C>::mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	template<typename V, int M, int T, int C>
	void SpriteImpl<V, M, T, C>::updatePlanarSprite(obj<Planar> plane) {
		if (plane) {
			auto& tr = *plane;
			if (mCell) {
				auto& cell = *mCell;
				Quad<V, M, T, C>::mVertices[0].pos = tr * vec2(cell.left, cell.up);
				Quad<V, M, T, C>::mVertices[0].uv = vec2(cell.uvLeft, cell.uvUp);
				Quad<V, M, T, C>::mVertices[1].pos = tr * vec2(cell.right, cell.up);
				Quad<V, M, T, C>::mVertices[1].uv = vec2(cell.uvRight, cell.uvUp);
				Quad<V, M, T, C>::mVertices[2].pos = tr * vec2(cell.right, cell.down);
				Quad<V, M, T, C>::mVertices[2].uv = vec2(cell.uvRight, cell.uvDown);
				Quad<V, M, T, C>::mVertices[3].pos = tr * vec2(cell.left, cell.down);
				Quad<V, M, T, C>::mVertices[3].uv = vec2(cell.uvLeft, cell.uvDown);
			}
			else {
				auto tex = Visage<M, T, C>::material->textures[0];
				auto w = float(tex->width()) / 2.0f;
				auto h = float(tex->height()) / 2.0f;
				Quad<V, M, T, C>::mVertices[0].pos = tr * vec2(-w, -h);
				Quad<V, M, T, C>::mVertices[0].uv = vec2(0.0f, 1.0f);
				Quad<V, M, T, C>::mVertices[1].pos = tr * vec2(w, -h);
				Quad<V, M, T, C>::mVertices[1].uv = vec2(1.0f, 1.0f);
				Quad<V, M, T, C>::mVertices[2].pos = tr * vec2(w, h);
				Quad<V, M, T, C>::mVertices[2].uv = vec2(1.0f, 0.0f);
				Quad<V, M, T, C>::mVertices[3].pos = tr * vec2(-w, h);
				Quad<V, M, T, C>::mVertices[3].uv = vec2(0.0f, 0.0f);
			}
			return;
		}
	}

	template<typename V, int M, int T, int C>
	void SpriteImpl<V, M, T, C>::update(Entity e) {
		if (Visage<M, T, C>::material && Visage<M, T, C>::material->hasTexture()) {
			updatePositionSprite(e);
			updatePlanarSprite(e);
		}
	}
}