#include "QCustomStacked.h"

QCustomStacked::QCustomStacked(QWidget* parent) : QStackedWidget(parent), target(target) {
	// UI�� �ʿ��� �������� �����մϴ�.
	font.setFamily(QString::fromUtf8("Noto Sans KR Light"));
	font.setPointSize(11);
	this->library = new FlowLayout;
	this->sid = new SIDCpp("the-intersection");
};

QCustomStacked::~QCustomStacked() {};

void QCustomStacked::login() { // ���� �α����� �����մϴ�.
	Json::StyledWriter writer;
	std::string clientid = sid->createClientID("the-intersection"); // ��� ���� ID�� ����ϴ�.
	Json::Value userdata = sid->login(clientid, id_form->text().toStdString(), pw_form->text().toStdString()); // �α����� �����մϴ�.
	if (userdata["error"])
	{
		return this->loginError(); // ������ �߻��ϸ� �˾��� ���ϴ�.
	}

	LoadJson* lj = new LoadJson;
	
	Json::Value userdataFile = lj->LoadUserData(); // ���� ������ �ε��մϴ�.

	// �ҷ��� ���Ͽ� �α����� ������ ������ �ֽ��ϴ�.
	userdataFile["sid"]["clientid"] = clientid;
	userdataFile["sid"]["sessid"] = userdata["sessid"];
	userdataFile["sid"]["pid"] = userdata["pid"];
	userdataFile["sid"]["nickname"] = userdata["nickname"];
	lj->Save("data/user.json", userdataFile); // ������ ������ �����մϴ�.
	Json::Value game = lj->LoadLibraryOn(userdata["pid"].asString()); // ������ ����� ���� ���̺귯���� �ҷ��ɴϴ�.
	lj->Save("data/game.json", game); // ���� ���� ���̺귯�� ���Ͽ� �����մϴ�.

	this->widget(4)->findChild<QLineEdit *>("steamidEnter")->setText(userdataFile["steamid64"].asCString()); // ������ �α׾ƿ��ߴٰ� �ٽ� �α������� ��츦 ����� ������ ���� ������ �����մϴ�.

	this->loadGameLibrary(); // ���� ���̺귯�� â�� �غ��մϴ�.
	this->setCurrentIndex(0); // ���� ���̺귯�� â�� ǥ���մϴ�.
}

void QCustomStacked::loginError() {
	QMessageBox::information(0, QString("Error"), QString("Login failed. Try again."), QMessageBox::Ok); // �α��� ���� �޽����� �߽��մϴ�.
}

void QCustomStacked::setGamePage(int gameid) {
	delete game;
	LoadJson* lj = new LoadJson;
	auto* gamePage = this->widget(1);
	Json::Value data = lj->LoadLibraryW(); // ���� ���̺귯���� �ε��մϴ�.
	char str[10];
	auto* gameText = gamePage->findChild<QLabel *>("gameInfoName");
	gameText->setText(data["games"][itoa(gameid, str, 10)]["name"].asCString()); // ���� �̸��� �ֽ��ϴ�.

	// ���� �÷����� ���� ��ü�� �����մϴ�.
	if (data["games"][itoa(gameid, str, 10)]["platform"].asString().compare("steam") == 0)
	{
		game = new SteamGame(gameid, data["games"][itoa(gameid, str, 10)]["additional"]["steamid"].asInt());
	}
	else
	{
		game = new Game(gameid);
	}
};

void QCustomStacked::setUsername(std::string name) { // ���� ���̺귯�� â�� ���� ��ó ��� â�� �ִ� ���� �г����� �����մϴ�.
	std::string out = "Logged in as ";
	out += name;
	this->widget(0)->findChild<QLabel *>("libraryName")->setText(out.c_str());
	this->widget(2)->findChild<QLabel *>("launcherName")->setText(out.c_str());
}

void QCustomStacked::startGame() { // ������ �����մϴ�.
	this->game->execute();
}

void QCustomStacked::setGameLibrary() { // loadGameLibrary()�� �����ϴ� �����Դϴ�.
	this->loadGameLibrary();
}

void QCustomStacked::loadGameLibrary() { // ���� ���̺귯���� �ε��ϰ� ǥ���մϴ�.
	LoadJson lj;
	QList<GameButton*> buttons = library->findChildren<GameButton*>();
	for (size_t i = 0; i < buttons.length(); i++) // ���� â�� �ִ� ��� ���� ��ư�� ����ϴ�.
	{
		library->removeWidget(buttons[i]);
	}

	game->GameListGenerate(); // ���� ����Ʈ�� �ٽ� ����ϴ�.

	Json::Value data = lj.LoadLibrary(); // ���� ���̺귯���� �ε��մϴ�.
	Json::Value& games = data["games"];
	for (Json::ValueIterator it = games.begin(); it != games.end(); it++) {
		// ��ư���� ����ϴ�.
		GameButton* button = new GameButton(target);
		std::string objectname = "game";
		objectname += (*it)["tiid"].asCString();
		button->setObjectName(QString::fromUtf8(objectname.c_str()));
		button->setGeometry(QRect(0, 0, 171, 101));
		button->setFont(font);
		std::string tempButtonStyleSheet = buttonStyleSheet;
		tempButtonStyleSheet += "background-image: url(\"GameThumbnail/";
		tempButtonStyleSheet.append((*it)["tiid"].asCString());
		tempButtonStyleSheet.append(".jpg\");\n");
		button->setStyleSheet(QString::fromUtf8(tempButtonStyleSheet.c_str()));
#ifndef QT_NO_ACCESSIBILITY
		button->setAccessibleName(QApplication::translate("GameLauncherClass", (*it)["name"].asCString(), nullptr));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
		button->setAccessibleDescription(QApplication::translate("GameLauncherClass", (*it)["tiid"].asCString(), nullptr));
#endif // QT_NO_ACCESSIBILITY
		button->setText(QApplication::translate("GameLauncherClass", (*it)["name"].asCString(), nullptr));
		button->setGameid((*it)["tiid"].asCString());
		QObject::connect(button, SIGNAL(changeStackedWidgetIndex(int)), this, SLOT(setCurrentIndex(int)));
		QObject::connect(button, SIGNAL(changeGameWidget(int)), this, SLOT(setGamePage(int)));
		buttons.append(button); // ��ư�� ��ư ����Ʈ�� �߰��մϴ�.
		library->addWidget(button);
	}
}

void QCustomStacked::init(QWidget* target) {
	// �ֿ� �������� �ʱ�ȭ�մϴ�.
	this->target = target;
	this->library->setParent(target);
	this->target->setLayout(library);
}

void QCustomStacked::loginInit(QLineEdit* id, QLineEdit* pw) {
	// �α��� ���� ���� �����ɴϴ�.
	this->id_form = id;
	this->pw_form = pw;
}

void QCustomStacked::changeToUserTab() {
	// ���� ���� ������ �ٲߴϴ�.
	this->setCurrentIndex(4);
}

void QCustomStacked::setSteamID64() { // ���� ���� ������ �ٲߴϴ�.
	LoadJson* lj = new LoadJson;
	Json::Value userdata = lj->LoadUserData(); // ���� �����͸� �ε��մϴ�.
	userdata["steamid64"] = this->widget(4)->findChild<QLineEdit *>("steamidEnter")->text().toStdString();
	lj->Save("data/user.json", userdata); // SteamID64�� �ٽ� �ε��� ���Ͽ� �ֽ��ϴ�.
	this->loadGameLibrary(); // ���� ���̺귯���� �ٽ� �ε��մϴ�.
}

void QCustomStacked::logout() {
	LoadJson* lj = new LoadJson;
	Json::Value userdata = lj->LoadUserData(); // ���� �����͸� �ε��մϴ�.
	this->sid->logout(userdata["sid"]["clientid"].asString(), userdata["sid"]["sessid"].asString()); // SID ������ ����� �α׾ƿ��� �����մϴ�.
	
	// ���Ͽ� �ִ� ���� �����͸� ����ϴ�.
	userdata["sid"]["sessid"] = "";
	userdata["sid"]["nickname"] = "";
	userdata["sid"]["pid"] = "";
	lj->Save("data/user.json", userdata); // ���� �����͸� ���� ������ �����մϴ�.
	this->setCurrentIndex(3);
}