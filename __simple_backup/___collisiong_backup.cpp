#define CUTE_C2_IMPLEMENTATION

#include "shape2d.h"
#include <cute/cute_c2.h>
#include "painter.h"

namespace el
{
	void aabb::debug(ShapeDebug* debug, const color& c, bool fill) {
		debug->line.batchAABB(*this, c, fill ? 0 : E_DANGLING_VERTICES);
		if (fill) {
			debug->fill.batchAABB(*this, c, E_DANGLING_VERTICES);
		}
	}
	void circle::debug(ShapeDebug* debug, const color& c, bool fill) {
		debug->point.batchCircle(*this, c, (E_DANGLING_VERTICES | E_DANGLING_INDICES));
	}
	void poly2d::debug(ShapeDebug* debug, const color& c, bool fill) {
		debug->line.batchPoly(*this, c, (E_DANGLING_VERTICES | E_DANGLING_INDICES));
	}

	aabb poly2d::findRange() {
		aabb range;
		range.l = range.b = INFINITY;
		range.r = range.t = -INFINITY;
		for (int i = 0; i < count; i++) {
			auto& v = verts[i];
			range.l = min(v.x, range.l);
			range.b = min(v.y, range.b);
			range.r = max(v.x, range.r);
			range.t = max(v.y, range.t);
		} return range;
	}


	void norms2d(vec2* verts, vec2* norms, int count) {
		c2Norms(reinterpret_cast<c2v*>(verts), reinterpret_cast<c2v*>(norms), 4);
	}
	int hitPolyToPoly(const poly2d& a, const poly2d& b) {
		return c2PolytoPoly(reinterpret_cast<const c2Poly*>(&a), 0, reinterpret_cast<const c2Poly*>(&b), 0);
	};

}