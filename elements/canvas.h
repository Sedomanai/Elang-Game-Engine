#pragma once
#include "../tools/atlas.h"
#include "visage.h"
#include "basic.h"

namespace el
{
	template<typename VertexType>
	struct ELANG_DLL Canvas : Quad<VertexType>
	{
		Canvas();
		Canvas(asset<Material> material, asset<Painter> painter);

		void update();
		void update(asset<Cell>);
		aabb bounds;
		vec2 uvpos, uvsize;

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(Visage::material, Visage::painter, bounds, uvpos, uvsize);
		}
	};
}