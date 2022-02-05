#pragma once

#include <QDialog>
#include "ui_qt_find_texture_dialog.h"

namespace el
{
	class QElangFindTextureDialog : public QDialog
	{
		Q_OBJECT

	public:
		QElangFindTextureDialog(QWidget* parent = Q_NULLPTR);
		~QElangFindTextureDialog();

	private:
		Ui::QElangFindTextureDialogUI ui;
	};
}
