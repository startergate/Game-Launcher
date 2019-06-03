#include "GameButton.h"

GameButton::GameButton(QWidget* parent, Qt::WindowFlags f)
	: QPushButton(parent) {

}

GameButton::~GameButton() {}

void GameButton::mousePressEvent(QMouseEvent* event) {
	OutputDebugString(L" ���ӹ�ư ���� \n");
	char id[3];
	//sprintf(id, "%d", this->gameid);
	//OutputDebugStringA(id);
	emit changeStackedWidgetIndex(1);
	emit changeGameWidget(this->gameid);
}

void GameButton::setGameid(int gameid) {
	this->gameid = gameid;
}

void GameButton::setGameid(std::string gameid) {
	this->gameid = atoi(gameid.c_str());
}