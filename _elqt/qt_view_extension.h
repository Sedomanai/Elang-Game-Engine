#pragma once
#include "_elang_qt_global.h"
#include <glil/extension.h>
#include <qevent.h>
#include <qwidget.h>
#include <qopenglwidget.h>

#include <tools/registry.h>
#include <tools/mousesym.h>
#include <tools/keysym.h>
#include <tools/signal.h>

namespace el {
	class _ELANGQT_EXPORT QElangView : public QOpenGLWidget
	{
		Q_OBJECT

	signals:
		void sig_InitializeGL();
		void sig_PaintGL();
		void sig_ResizeGL(int w, int h);
		void sig_MousePress();
		void sig_MouseRelease();
		void sig_MouseMove();
		void sig_KeyPress(QKeyEvent*);
		void sig_KeyRelease(QKeyEvent*);
		void sig_ScrollWheel();
	public:
		QElangView(QWidget* parent);
		~QElangView();

		static signal<> sSig_GlobalGL;
	protected:
		void initializeGL() override;
		void paintGL() override;
		void resizeGL(int w, int h) override;

		void wheelEvent(QWheelEvent*) override;
		void mousePressEvent(QMouseEvent*) override; // Transform& camera);
		void mouseReleaseEvent(QMouseEvent*) override;
		void mouseMoveEvent(QMouseEvent*) override;
		void keyPressEvent(QKeyEvent*) override;
		void keyReleaseEvent(QKeyEvent*) override;


		static bool sInitialized;
		bool mInitialized;
		float mWidth, mHeight;
	};
}