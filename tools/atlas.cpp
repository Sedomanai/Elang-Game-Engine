#include "atlas.h"

#include "texture.h"

namespace el {
	template struct CellImpl<0>;
	template struct ClipImpl<0>;
	template struct AtlasImpl<0>;

	//TODO: change ordering and data structure
	template<int N>
	void AtlasImpl<N>::makeFromAtlsFile(const string& filePath) {
		int aw, ah;

		string out;
		loadFile(filePath.c_str(), out);

		AtlasAction act = AtlasAction::NONE;
		int index, x, y, w, h, ox, oy;
		asset<ClipImpl<N>> clip;
		iterate(out, '\n', [&](strview line, sizet) -> bool {
			iterate(line, ' ', [&](strview word, sizet iter) -> bool {
				switch (iter) {
				case 0: act = gAtlasActions[string(word)]; break;
				case 1:
					switch (act) {
					case AtlasAction::SET_WIDTH:
						aw = toInt(word);
						return false;
					case AtlasAction::SET_HEIGHT:
						ah = toInt(word);
						return false;
					case AtlasAction::SET_CLIP:
						clip = gProject->makeSub<ClipImpl<N>>();
						clips.emplace(string(word), clip);
						return false;
					}
				default:
					switch (act) {
					case AtlasAction::SET_USER:
						if (gProject->textures.contains(string(word))) {
							textures.push_back(gProject->textures[string(word)]);
						} else {
							cout << "Atlas " << filePath << " could not link to non-existing texture " << string(word) << endl;
						}
						break;
					case AtlasAction::SET_CELL:
						switch (iter) {
						case 1: index = toInt(word); break;
						case 2: x = toInt(word); break;
						case 3: y = toInt(word); break;
						case 4: w = toInt(word); break;
						case 5: h = toInt(word); break;
						case 6: ox = toInt(word); break;
						case 7: oy = toInt(word); break;
						case 9:
							auto cell = gProject->makeSub<CellImpl<N>>(x, y, w, h, ox, oy, aw, ah, index);
							cells.emplace(string(word), cell);
							break;
						}
						break;
					case AtlasAction::SET_CLIP: 
						// TODO: CHANGE THIS LATER, DURATION NO LONGER EXISTS FOR FRAME
						//		 CLIPS LOADED THIS WAY DO NOT WORK AT THIS MOMENT
						if (iter % 2 == 0) {
							//frame.cell = toInt(word);
						} else {
							clip->emplace_back();
						}
						break;
					}
					break;
				}
				return false;
			});
			return false;
		});
	}

	template<int N>
	void AtlasImpl<N>::destroy() {
		for (auto e : cells) {
			gProject->destroy(e.second);
		} for (auto e : clips) {
			gProject->destroy(e.second);
		} cells.clear();
		clips.clear();
	}

	template<int N>
	vector<asset<CellImpl<N>>> AtlasImpl<N>::packedAndCachedCells() {
		auto&& cv = linearCells();

		vector<CellImpl<N>> data; data.reserve(cv.size());
		for (sizet i = 0; i < cv.size(); i++) {
			data.emplace_back(*cv[i]);
		}

		//gProject->remove<CellImpl<N>>(ents.begin(), ents.end());
		for (sizet i = 0; i < data.size(); i++) {
			auto cell = cv[i];
			cell.remove<CellImpl<N>>();
			cell.add<CellImpl<N>>(data[i]);
		} 

		std::sort(cv.begin(), cv.end(), [&](auto lhs, auto rhs) { return lhs->index < rhs->index; });
		return std::move(cv);
	}

	template<int N>
	void AtlasImpl<N>::recreateCells(const vector<asset<CellImpl<N>>>& cv) {
		for (sizet i = 0; i < cv.size(); i++) {
			auto cell = cv[i];
			CellImpl<N> n = *cell;
			n.index = (uint32)i;
			cell.remove<CellImpl<N>>();
			cell.add<CellImpl<N>>(n);
		}
	}

	template<int N>
	vector<asset<CellImpl<N>>> AtlasImpl<N>::linearCells(eSortType sortType) {
		vector<asset<CellImpl<N>>> cv; cv.reserve(cells.count());

		for (auto it : cells) {
			cv.emplace_back(it.second);
		} 

		switch (sortType) {
		case eSortType::INDEX:
			std::sort(cv.begin(), cv.end(), [&](auto lhs, auto rhs) { return lhs->index < rhs->index; });
			break;
		case eSortType::ENTITY_HANDLE:
			std::sort(cv.begin(), cv.end(), [&](auto lhs, auto rhs) { return (uint32)lhs < (uint32)rhs; });
			break;
		}
		
		return std::move(cv);
	}

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

	template<int N>
	CellImpl<N>::CellImpl(int x, int y, int w, int h, int oX_, int oY_, int aw, int ah, int index) :
		oX((int16)oX_), oY((int16)oY_),
		left((float)(oX_)), down((float)(-oY_)), right((float)(oX_ + w)), up((float)(-oY_ + h)),
		uvLeft((float)x / (float)aw),
		uvDown((float)(y + h) / (float)ah),
		uvRight((float)(x + w) / (float)aw),
		uvUp((float)y / (float)ah),
		index((uint32)index)
	{

	}
}