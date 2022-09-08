#include <elpch.h>
#include "cell.h"
#include "../common/stream.h"

namespace el {
	CellMeta::CellMeta() 
		: x(0), y(0), w(0), h(0), oX(0), oY(0) {}

	CellMeta::CellMeta(sizet x_, sizet y_, sizet w_, sizet h_, sizet oX_, sizet oY_)
		: x((int32)x_), y((int32)y_), w((int32)w_), h((int32)h_), oX((int32)oX_), oY((int32)oY_) {
	}

	Cell::Cell(const CellMeta& cc, sizet aw, sizet ah) :
		left((float)-cc.oX), down((float)cc.oY - cc.h), right((float)(-cc.oX + cc.w)), up((float)(cc.oY)),
		uvLeft((float)cc.x / (float)aw),
		uvDown((float)(cc.y + cc.h) / (float)ah),
		uvRight((float)(cc.x + cc.w) / (float)aw),
		uvUp((float)cc.y / (float)ah) {
	};

	void Cell::mold(const CellMeta& cc, sizet aw, sizet ah) {
		left = (float)-cc.oX;
		down = (float)cc.oY - cc.h;
		right = (float)(-cc.oX + cc.w);
		up = (float)(cc.oY);
		uvLeft = (float)cc.x / (float)aw;
		uvDown = (float)(cc.y + cc.h) / (float)ah;
		uvRight = (float)(cc.x + cc.w) / (float)aw;
		uvUp = (float)cc.y / (float)ah;
	}

	stream& operator<<(stream& save, const CellMeta& cc) {
		save << cc.x << cc.y << cc.w << cc.h << cc.oX << cc.oY;
		return save;
	}
	stream& operator>>(stream& load, CellMeta& cc) {
		load >> cc.x >> cc.y >> cc.w >> cc.h >> cc.oX >> cc.oY;
		return load;
	}
}