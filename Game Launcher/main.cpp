#include "GameLauncher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) // Qt ���ø����̼��� VS�� ����� �⺻���� �����Ǵ� �����Դϴ�.
{
	QApplication a(argc, argv);
	GameLauncher w;
	w.show();
	return a.exec();
}