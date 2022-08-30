#include "atlas.h"
#include "texture.h"

namespace el {/*
	/*void AtlasLoader::loadGrid(sizet column, sizet row, bool nearest) {
		stringstream ss;
		ss << "el_grid_" << column << 'x' << row;

		if (mGridAtlas.find(ss.str()) != mGridAtlas.end())
			return;

		auto& grid = mGridAtlas.emplace(ss.str(), GridAtlas{}).first->second;

		double offsetX = nearest ? 0 : 1.0 / (long(column) * 128);
		double offsetY = nearest ? 0 : 1.0 / (long(row) * 128);

		for (sizet i = 0; i < column * row; i++) {
			auto x = double(i / row);
			auto y = double(i % row);
			grid.push_back(GridCell{
				float(x / column + offsetX), float(y / row + offsetY), float((x + 1) / column - offsetX), float((y + 1) / row - offsetY)
			});
		}
	}*/

}