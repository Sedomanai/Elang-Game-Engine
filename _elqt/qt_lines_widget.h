#pragma once

#include <QWidget>
#include <QLayout.h>
#include <common/container.h>
#include <tools/pointer.h>

#include "ui_qt_int_line.h"

#include "_elang_qt_global.h"

//#include "ui_qt_lines_widget.h"

namespace el
{
	struct QIlangLine { 
		friend class QIlangLinesWidget;
		QIlangLine() : mWidget(0), mIndex(-1) {};

	private:
		uint mIndex;
		QWidget* mWidget; 
	};
	struct QIlangIntLine : QIlangLine { Ui::QIlangIntLineUI ui; };

	// TODO: CREATE MORE
	/*struct QIlangIntLine : QIlangLine { Ui::QIlangIntLine ui; };
	struct QIlangIntLine : QIlangLine { Ui::QIlangIntLine ui; };
	struct QIlangIntLine : QIlangLine { Ui::QIlangIntLine ui; };*/

	class _ELANGQT_EXPORT QIlangLinesWidget : public QWidget
	{
		Q_OBJECT

	public:
		QIlangLinesWidget(QWidget* parent = Q_NULLPTR);
		~QIlangLinesWidget();

		p<QIlangIntLine> createIntLine(const string& label);


		bool hasLine(const string& label);
		void removeLine(const string& label);
		void removeLine(p<QIlangIntLine>);

	private:
		pCache cache;
		QVBoxLayout* layout;
		hashmap<string, pptr<QIlangLine>> lines;
	};
}
