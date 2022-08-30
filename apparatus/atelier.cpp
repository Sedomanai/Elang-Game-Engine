#include "atelier.h"


namespace el
{
	void updateBasicGraphics2d() {
		for (auto e : gStage->view<GraphicUpdate2d>()) {
			auto spr = obj<Sprite>(e);
			if (spr)
				spr->update(spr);
			if (spr.has<Canvas<SpriteVertex>>())
				spr.get<Canvas<SpriteVertex>>().update();
		} gStage->clear<GraphicUpdate2d>();
	}

	void paintStageOrder() {
		//auto& at = gStage->atelier;
		//for (sizet i = 0; i < at.size(); i++) {
		//	//auto p = asset<Painter>(at[i]);
		//	//if (p) p->paint();
		//}
	}

	void batchBasic2DComponents() {
		for (auto e : gStage->view<Sprite>()) {
			obj<Sprite>(e)->batch();
		}
		for (auto e : gStage->view<Canvas<SpriteVertex>>()) {
			obj<Canvas<SpriteVertex>>(e)->batch();
		}
		//for (auto e : gStage->view<Textfield>()) {
		//	obj<Textfield>(e)->batch();
		//}
		//for (auto e : gStage->view<Textbox>()) {
		//	obj<Textbox>(e)->batch(e);
		//}
	}

	void batchDebuggingComponents(ShapeDebug2d* debugger, const color8& color) {
		for (auto e : gStage->view<Box>()) {
			auto rect = obj<Box>(e);
			debugger->line.batchAABB(*rect, color);
		}
		for (auto e : gStage->view<Radius>()) {
			auto circ = obj<Radius>(e);
			debugger->line.batchCircle(*circ, color);
		}
	}
}