#pragma once
#include "visage.h"
#include "basic.h"

namespace el
{
	struct ELANG_DLL Canvas : Quad
	{
		Canvas() : Quad() {};
		Canvas(asset<Material> material, asset<Painter> painter);

		void update();
		aabb bounds;
		vec2 uvpos, uvsize;

		template<typename T>
		void serialize(T& archive) {
			archive(material, painter, bounds, uvpos, uvsize);
		}
	};
}