#include "bmpfont.h"

namespace el
{
	void FontFace::makeFromBfntFile(const string& filePath) {
		el::stream ss;
		ss.toLoad(filePath.c_str());
		ss.shift(4);

		uint16 aw = 0, ah = 0, lh = 0;
		while (!ss.invalid()) {
			uint8_t block_type; ss >> block_type;
			uint32 block_size; ss >> block_size;

			switch (block_type) {
			default:
				ss.shift(block_size);
				break;
			case 2:
				ss >> lineHeight;
				ss.shift(2);
				ss >> aw; ss >> ah;
				ss.shift(7);
				break;
			case 4:
				if (ss.invalid())
					return;
				for (auto i = 0; i < (sizet)(block_size / 20); i++) {
					uint32 id;
					uint16 x, y, w, h;
					int16 ox, oy, adv;
					ss >> id >> x >> y >> w >> h >> ox >> oy >> adv;
					ss.shift(2);

					auto glyph =
						gProject->makeSub<FontGlyph>(x, y, w, h, ox, oy, aw, ah, (float)adv);

					glyphs.emplace(id, glyph);
				}
				break;
			}
		}
	}

	void FontFace::destroy() {
		for (auto e : glyphs) {
			gProject->destroy(e.second);
		}
	}

	FontGlyph::FontGlyph(uint16 x, uint16 y, uint16 w, uint16 h, int16 oX, int16 oY, uint16 aw, uint16 ah, float adv_) :
		left((float)(oX)), down((float)(-oY - h)), right((float)(oX + w)), up((float)(-oY)),
		uvLeft((float)x / (float)aw),
		uvDown((float)(y + h) / (float)ah),
		uvRight((float)(x + w) / (float)aw),
		uvUp((float)y / (float)ah),
		adv(adv_)
	{
		/*if (w % 2 != 0) {
			right++;
		} if (h % 2 != 0) {
			up++;
		}*/
	}
}