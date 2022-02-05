#pragma once

#include <QWidget>
#include "qt_view_extension.h"
#include "ui_qt_texture_widget.h"

#include "_elang_qt_global.h"

#include <elements/basic.h>
#include <elements/sprite.h>

namespace el
{
	struct GizmoSprite : Sprite { using Sprite::Sprite; };

	class _ELANGQT_EXPORT QElangTextureWidget : public QWidget
	{
		Q_OBJECT

	public:
		QElangTextureWidget(QWidget* parent = Q_NULLPTR);
		~QElangTextureWidget();

		void updateTick();
		void updateMaterial(asset<Material>);
		QElangView* view();
	protected:
		Ui::QElangTextureWidgetUI ui;
		virtual void onTextureUpdate(asset<Texture>) {};

		Qt::CursorShape mMoveCursor;

		Stage mStage;
		ent<Sprite> mTexSprite;
		int mWinWidth, mWinHeight;

		aabb mCamBounds;
		vec2 mMoveDelta;
		vec2 mMoveCenter;
		vec2 mScrollStep;
		float mResolution;

		asset<Camera> mMainCam;
		asset<Painter> mSpritePainter;
		asset<Material> mTexMat;
		ShapeDebug* mGadgetPainter;

		bool mSuppressScroll;
		bool mMovingScreen;

		void connectMouseInput();
		void syncCamera();
		void syncScrollBars();
		void showEvent(QShowEvent* e) override;
	};

}





