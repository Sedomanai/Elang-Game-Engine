#pragma once

#include "qt_texture_widget.h"
#include <elements/button.h>

namespace el {

	struct CellData
	{
		CellData(asset<Cell> cell_) : cell(cell_) {};
		asset<Cell> cell;
	};

	struct _ELANGQT_EXPORT PaletteButtonEvent : ButtonEvent
	{
		Stage* stage;
		void onHover(Entity e) override;
		single_signal<asset<Cell>> sig_Clicked;
	};

	class _ELANGQT_EXPORT QElangPaletteWidget : public QElangTextureWidget
	{
		Q_OBJECT

	public:
		QElangPaletteWidget(QWidget *parent = Q_NULLPTR);
		~QElangPaletteWidget();
		PaletteButtonEvent cellEvent;

		asset<Atlas> atlas() { return mAtlas; }

	private:
		asset<Atlas> mAtlas;
		void onTextureUpdate(asset<Texture>) override;
	};

} 
