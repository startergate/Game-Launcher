#include <Game.h>

Game::Game(int gameid) {
    this->gameid = gameid; // 게임??고유 ID�??�수�?받습?�다.
}

Game::~Game() {}

void Game::execute() {
    std::string executeable = "";
    system(executeable.c_str());
}

Json::Value Game::GameListGenerate() {
    Json::Value games_docs;
    LoadJson* lj = new LoadJson;
    Json::Value games_doc = lj->LoadLibraryW(); // 게임 ?�이브러�??�일??로드?�니??
    Json::Value user = lj->LoadUserData(); // ?��? ?�보 ?�일??로드?�니??
    Json::Value steamgames = lj->LoadSteam(); // ?��? ?�이브러리�? 로드?�니??
    Json::Value new_game_doc;
    Json::Value new_games;

    Json::Value& games = games_doc["games"];
    Json::Value& s_games = steamgames["response"]["games"];
    std::list<std::string> toRemove;

    for (Json::ValueIterator it = games.begin(); it != games.end(); it++) if (!(*it)["platform"].asString().compare("steam")) toRemove.push_back((*it)["tiid"].asCString()); // ?�랫?�이 ?��???게임?�을 찾습?�다.
    for (auto const& it : toRemove) games_doc["games"].removeMember(it.c_str()); // ?�랫?�이 ?��???게임?�을 ??��?�니??

    for (Json::ValueIterator it = s_games.begin(); it != s_games.end(); it++)
    {
        char buffer[10];
        Json::Value gamedata;

        // ?��? API로�???받�? ?�이?�에???�이?��? 추출?�니??
        gamedata["tiid"] = itoa((*it)["appid"].asInt(), buffer, 10);
        gamedata["name"] = (*it)["name"].asCString();
        gamedata["isInstalled"] = "false";
        gamedata["addedTime"] = 1530489600;
        gamedata["playedTime"] = (*it)["playtime_forever"].asInt();
        gamedata["platform"] = "steam";
        gamedata["additional"]["steamid"] = (*it)["appid"].asInt();

        // 추출???�이?��? 게임 ?�이브러�?객체???�습?�다.
        games_doc["games"][itoa((*it)["appid"].asInt(), buffer, 10)] = gamedata;
    }

    std::ofstream data;
    data.open("data/game.json");

    data << games_doc; // 게임 ?�이브러�?객체�??�일�????�합?�다.

    lj->UploadLibrary(user["sid"]["pid"].asString(), games_doc); // 게임 ?�이브러리�? ?�버�??�로?�합?�다. 백업????��???�니??

    return games_doc; // 게임 ?�이브러리�? 리턴?�니??
}

SteamGame::SteamGame(int gameid, int steamid) : Game(gameid) {
    this->steamid = steamid; // ?��? 게임???�해??steamid ?�수�?추�?�?받습?�다.
}

SteamGame::~SteamGame() {}

void SteamGame::execute() {
    // ?��? ?�행 명령?��? 만듭?�다.
    std::string executeable = "explorer steam://run/";
    executeable += std::to_string(steamid);
    executeable.append("/");

    system(executeable.c_str()); // 명령?��? ?�행?�니??
}