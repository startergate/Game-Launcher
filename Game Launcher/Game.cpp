#include <Game.h>

Game::Game(int gameid) {
	this->gameid = gameid; // ������ ���� ID�� �μ��� �޽��ϴ�.
}

Game::~Game() {}

void Game::execute() {
	std::string executeable = "";
	system(executeable.c_str());
}

Json::Value Game::GameListGenerate() {
	Json::Value games_docs;
	LoadJson* lj = new LoadJson;
	Json::Value games_doc = lj->LoadLibraryW(); // ���� ���̺귯�� ������ �ε��մϴ�.
	Json::Value user = lj->LoadUserData(); // ���� ���� ������ �ε��մϴ�.
	Json::Value steamgames = lj->LoadSteam(); // ���� ���̺귯���� �ε��մϴ�.
	Json::Value new_game_doc;
	Json::Value new_games;

	Json::Value& games = games_doc["games"];
	Json::Value& s_games = steamgames["response"]["games"];
	std::list<std::string> toRemove;

	for (Json::ValueIterator it = games.begin(); it != games.end(); it++) if (!(*it)["platform"].asString().compare("steam")) toRemove.push_back((*it)["tiid"].asCString()); // �÷����� ������ ���ӵ��� ã���ϴ�.
	for (auto const& it : toRemove) games_doc["games"].removeMember(it.c_str()); // �÷����� ������ ���ӵ��� �����մϴ�.

	for (Json::ValueIterator it = s_games.begin(); it != s_games.end(); it++)
	{
		char buffer[10];
		Json::Value gamedata;

		// ���� API�κ��� ���� �����Ϳ��� �����͸� �����մϴ�.
		gamedata["tiid"] = itoa((*it)["appid"].asInt(), buffer, 10);
		gamedata["name"] = (*it)["name"].asCString();
		gamedata["isInstalled"] = "false";
		gamedata["addedTime"] = 1530489600;
		gamedata["playedTime"] = (*it)["playtime_forever"].asInt();
		gamedata["platform"] = "steam";
		gamedata["additional"]["steamid"] = (*it)["appid"].asInt();

		// ������ �����͸� ���� ���̺귯�� ��ü�� �ֽ��ϴ�.
		games_doc["games"][itoa((*it)["appid"].asInt(), buffer, 10)] = gamedata;
	}

	std::ofstream data;
	data.open("data/game.json");

	data << games_doc; // ���� ���̺귯�� ��ü�� ���Ϸ� �����մϴ�.

	lj->UploadLibrary(user["sid"]["pid"].asString(), games_doc); // ���� ���̺귯���� ������ ���ε��մϴ�. ����� ������ �մϴ�.

	return games_doc; // ���� ���̺귯���� �����մϴ�.
}

SteamGame::SteamGame(int gameid, int steamid) : Game(gameid) {
	this->steamid = steamid; // ���� ���ӿ� ���ؼ� steamid �μ��� �߰��� �޽��ϴ�.
}

SteamGame::~SteamGame() {}

void SteamGame::execute() {
	// ���� ���� ��ɾ ����ϴ�.
	std::string executeable = "explorer steam://run/";
	executeable += std::to_string(steamid);
	executeable.append("/");

	system(executeable.c_str()); // ��ɾ �����մϴ�.
}