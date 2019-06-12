#include "GameButton.h"

GameButton::GameButton(QWidget* parent, Qt::WindowFlags f)
	: QPushButton(parent) {

}

GameButton::~GameButton() {}

void GameButton::mousePressEvent(QMouseEvent* event) { // ���콺 Ŭ���� �����մϴ�.
	char id[3];
	//sprintf(id, "%d", this->gameid);
	//OutputDebugStringA(id);
	emit changeStackedWidgetIndex(1); // changeStackedWidgetIndex(int) �ñ׳��� �߻���ŵ�ϴ�.
	emit changeGameWidget(this->gameid); // changeGameWidget(int) �ñ׳��� �߻����� ���� ���� â�� ���� ������ ǥ���մϴ�.
}

void GameButton::setGameid(int gameid) {
	this->gameid = gameid; // gameid�� ���ڷ� �����ɴϴ�.
}

void GameButton::setGameid(std::string gameid) {
	this->gameid = atoi(gameid.c_str()); // gameid�� string���� ������ int�� ��ȯ�մϴ�.
}