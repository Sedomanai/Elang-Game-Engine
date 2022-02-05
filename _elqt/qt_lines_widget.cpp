#include "qt_lines_widget.h"

namespace el
{
    QIlangLinesWidget::QIlangLinesWidget(QWidget* parent)
        : QWidget(parent) {
        layout = new QVBoxLayout(this);
        layout->setSpacing(2);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setObjectName(QString::fromUtf8("layout"));
        setLayout(layout);
    }

    QIlangLinesWidget::~QIlangLinesWidget() {}

    p<QIlangIntLine> QIlangLinesWidget::createIntLine(const string& label) {
        pptr<QIlangIntLine> line;
        line->mWidget = new QWidget(this);
        line->ui.setupUi(line->mWidget);
        line->ui.label->setObjectName(QString::fromStdString(label));
        line->ui.label->setText(QString::fromStdString(label));
        layout->addWidget(line->mWidget);
        lines.emplace(label, line);
        return lines[label];
    }

    bool QIlangLinesWidget::hasLine(const string& label) {
        return lines.contains(label);
    }
    void QIlangLinesWidget::removeLine(const string& label) {
        if (lines.contains(label)) {
            delete lines[label]->mWidget;
            lines.erase(label);
        }
    }
    void QIlangLinesWidget::removeLine(p<QIlangIntLine> line) {
        string label = line->ui.label->objectName().toStdString();
        if (lines.contains(label)) {
            delete line->mWidget;
            lines.erase(label);
        }
    }
}