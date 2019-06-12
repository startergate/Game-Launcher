#include "GameLauncher.h"
#include "json/writer.h"
GameLauncher::GameLauncher(QWidget *parent)
	: QMainWindow(parent)
{
	LoadJson lj;
	ui.setupUi(this);

	// ���� ClickableQLabel�� ��ǥ�� �������ݴϴ�.
	ui.libraryLauncherTitle->target = 2;
	ui.launcherLibraryTitle->target = 0;
	ui.goBack->target = 0;
	ui.goBack_2->target = 0;
	Json::Value userdata = lj.LoadUserData(); // ���� ������ ������ �ε��մϴ�.
	SIDCpp sid("the-intersection"); // SIDCpp ��ü�� �����մϴ�.
	Json::Value loginData = sid.login(userdata["sid"]["clientid"].asCString(), userdata["sid"]["sessid"].asCString()); // �ڵ� �α����� �õ��մϴ�.
	if (loginData["error"])
	{
		ui.stackedWidget->setCurrentIndex(3); // �ڵ� �α����� �����ϸ�, �α��� â���� �����ϴ�.
	}
	else
	{		
		ui.stackedWidget->setCurrentIndex(0); // �ڵ� �α����� �����ϸ�, ���� ���̺귯�� â���� �����ϴ�.
		ui.stackedWidget->setUsername(loginData["nickname"].asString()); // QCustomStacked�� �ִ� ������ �󺧵鿡 �г����� �־��ݴϴ�.
		ui.steamidEnter->setText(userdata["steamid64"].asCString()); // ���� ���� �Է� â�� ���� �����͸� �־��ݴϴ�.
	}

	ui.stackedWidget->init(ui.scrollAreaWidgetContents); // QStackedWidget�� �����մϴ�.
	ui.stackedWidget->loginInit(ui.idEnter, ui.pwEnter); // �α��� ���� �غ��մϴ�.
	ui.stackedWidget->loadGameLibrary(); // ���� ���̺귯���� �ε��ϰ�, ���̺귯�� �������� ǥ���մϴ�.
}

void GameLauncher::buttonClicked(int gameid) {
	
}