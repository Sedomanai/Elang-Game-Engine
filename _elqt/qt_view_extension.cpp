#include "qt_view_extension.h"
#include <qwidget.h>

namespace el {
	bool QElangView::sInitialized = false;
	signal<> QElangView::sSig_GlobalGL;

	QElangView::QElangView(QWidget* parent) : QOpenGLWidget(parent), mInitialized(false) {}

	QElangView::~QElangView() {}

	void QElangView::initializeGL() {
		QOpenGLWidget::initializeGL();

		if (!sInitialized) {
			if (glewInit() == GLEW_OK) {
				sSig_GlobalGL.invoke();
				sInitialized = true;
			} else throw "GLEW could not initialize.";
		}

		if (sInitialized) { // if GLEW successfully initiated
			if (!mInitialized) {
				emit sig_InitializeGL();
				mInitialized = true;
			}
		}
	}                
	void QElangView::paintGL() { if (sInitialized) sig_PaintGL(); }
	void QElangView::resizeGL(int w, int h) { mWidth = w; mHeight = h; if (sInitialized) sig_ResizeGL(w, h); }

	void QElangView::wheelEvent(QWheelEvent* me) {
		float temp = me->angleDelta().y() / 120.0f;
		gMouse.updateWheel(temp);
		emit sig_ScrollWheel();
		update();
	}

	void QElangView::mousePressEvent(QMouseEvent* me) {
		auto mpos = vec2(me->localPos().x() - mWidth * 0.5f, -me->localPos().y() + mHeight * 0.5f);
		gMouse.updateKeys(mpos);
		switch (me->button()) {
		case Qt::MouseButton::LeftButton:
			gMouse.onPress(mpos, MouseSym::LEFT);
			break;
		case Qt::MouseButton::RightButton:
			gMouse.onPress(mpos, MouseSym::RIGHT);
			break;
		case Qt::MouseButton::MiddleButton:
			gMouse.onPress(mpos, MouseSym::MIDDLE);
			break;
		} 
		
		emit sig_MousePress();
		update();
	}

	void QElangView::mouseReleaseEvent(QMouseEvent* me) {
		auto mpos = vec2(me->localPos().x() - mWidth * 0.5f, -me->localPos().y() + mHeight * 0.5f);
		gMouse.updateKeys(mpos);
		switch (me->button()) {
		case Qt::MouseButton::LeftButton:
			gMouse.onRelease(mpos, MouseSym::LEFT);
			break;
		case Qt::MouseButton::RightButton:
			gMouse.onRelease(mpos, MouseSym::RIGHT);
			break;
		case Qt::MouseButton::MiddleButton:
			gMouse.onRelease(mpos, MouseSym::MIDDLE);
			break;
		} 

		emit sig_MouseRelease();
		update();
	}

	void QElangView::mouseMoveEvent(QMouseEvent* me) {
		auto mpos = vec2(me->localPos().x() - mWidth * 0.5f, -me->localPos().y() + mHeight * 0.5f);
		gMouse.updateKeys(mpos);

		emit sig_MouseMove(); 
		update();
	}
	void QElangView::keyPressEvent(QKeyEvent* e) {
		emit sig_KeyPress(e);
	}
	void QElangView::keyReleaseEvent(QKeyEvent* e) {
		emit sig_KeyRelease(e);
	}
}