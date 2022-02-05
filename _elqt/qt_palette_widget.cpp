#include "qt_palette_widget.h"

namespace el {

	void PaletteButtonEvent::onHover(Entity e) {
		gStage = stage;
		if (gMouse.state(0) == eInput::ONCE) {
			auto celldata = ent<CellData>(e);
			if (celldata) {
				sig_Clicked.invoke(celldata->cell);
			}
		}
	}

	QElangPaletteWidget::QElangPaletteWidget(QWidget* parent)
		: QElangTextureWidget(parent)
	{
		//ui.setupUi(this);

		connect(ui.view, &QElangView::sig_InitializeGL, [&]() {
			mStage.storage<CellData>().reserve(300);
			mStage.storage<Hitbox>().reserve(300);
			mStage.storage<Button>().reserve(300);
			cellEvent.stage = &mStage;
		});
	}

	QElangPaletteWidget::~QElangPaletteWidget()
	{
	}

	void QElangPaletteWidget::onTextureUpdate(asset<Texture> tex) {
		setStage(mStage);
		for (auto e : gStage->view<Button>()) {
			gStage->destroy(e);
		}

		if (tex->atlas) {
			mAtlas = tex->atlas;
			auto& cells = mAtlas->cells;
			for (auto it : cells) {
				auto& cell = *asset<Cell>(it.second);
				Hitbox rect;
				rect.l = cell.uvLeft * tex->width();
				rect.r = cell.uvRight * tex->width();
				rect.b = -cell.uvDown * tex->height();
				rect.t = -cell.uvUp * tex->height();

				auto obj = gStage->make<Hitbox>(rect);
				obj.add<CellData>(it.second);
				obj.add<Button>(&cellEvent);
			}
		}
	}

} // namespace il
