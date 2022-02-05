#include "atelier.h"

#include "../elements/basic.h"
#include "../elements/sprite.h"
#include "../elements/textbox.h" 

namespace el
{
	void updateBasicSprites(Observer& obs) {
		for (auto e : obs) {
			ent<Sprite>(e)->update(e);
		} obs.clear();
		for (auto e : gStage->view<Canvas>()) {
			ent<Canvas>(e)->update();
		}
		for (auto e : gStage->view<Textfield>()) {
			ent<Textfield>(e)->update(e);
		}
		for (auto e : gStage->view<Textbox>()) {
			ent<Textbox>(e)->update(e);
		}
	}

	void paintStageOrder() {
		auto& at = gStage->atelier;
		for (sizet i = 0; i < at.size(); i++) {
			//auto p = asset<Painter>(at[i]);
			//if (p) p->paint();
		}
	}

	void batchBasic2DComponents() {
		for (auto e : gStage->view<Sprite>()) {
			ent<Sprite>(e)->batch();
		}
		//for (auto e : gStage->view<Canvas>()) {
		//	ent<Canvas>(e)->batch();
		//}
		//for (auto e : gStage->view<Textfield>()) {
		//	ent<Textfield>(e)->batch();
		//}
		//for (auto e : gStage->view<Textbox>()) {
		//	ent<Textbox>(e)->batch(e);
		//}
	}

	void batchDebuggingComponents(ShapeDebug* debugger, const color& color) {
		for (auto e : gStage->view<Box>()) {
			auto rect = ent<Box>(e);
			debugger->line.batchAABB(*rect, color);
		}
		for (auto e : gStage->view<Radius>()) {
			auto circ = ent<Radius>(e);
			debugger->line.batchCircle(*circ, color);
		}
	}
}