#include "atelier.h"

#include "../elements/basic.h"
#include "../elements/sprite.h"
#include "../elements/textbox.h" 

namespace el
{
	void updateBasicSprites(Observer& obs) {
		for (auto e : obs) {
			obj<Sprite>(e)->update(e);
		} obs.clear();
		for (auto e : gStage->view<Canvas>()) {
			obj<Canvas>(e)->update();
		}
		for (auto e : gStage->view<Textfield>()) {
			obj<Textfield>(e)->update(e);
		}
		for (auto e : gStage->view<Textbox>()) {
			obj<Textbox>(e)->update(e);
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
			obj<Sprite>(e)->batch();
		}
		//for (auto e : gStage->view<Canvas>()) {
		//	obj<Canvas>(e)->batch();
		//}
		//for (auto e : gStage->view<Textfield>()) {
		//	obj<Textfield>(e)->batch();
		//}
		//for (auto e : gStage->view<Textbox>()) {
		//	obj<Textbox>(e)->batch(e);
		//}
	}

	void batchDebuggingComponents(ShapeDebug* debugger, const color8& color) {
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