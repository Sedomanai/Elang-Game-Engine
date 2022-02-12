#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	template<typename V, int M, int T, int C>
	struct ELANG_DLL CanvasImpl : Quad<V, M, T, C>
	{
		CanvasImpl() : Quad<V, M, T, C>() {};
		CanvasImpl(asset<MaterialImpl<M, T>> material, asset<PainterImpl<M, T, C>> painter);

		void update();
		aabb bounds;
		vec2 uvpos, uvsize;

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage<M, T, C>::material, Visage<M, T, C>::painter, bounds, uvpos, uvsize);
		}
	};

	using Canvas = CanvasImpl<SpriteVertex, 0, 0, 0>;
}