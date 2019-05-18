#include "ClickableQLabel.h"
#include <Windows.h>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
	OutputDebugString(L" ���� \n");
	emit changeStackedWidgetIndex(this->target);
}