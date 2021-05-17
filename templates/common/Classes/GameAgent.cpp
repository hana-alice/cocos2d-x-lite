#include "GameAgent.h"

GameAgent* GameAgent::_instance = nullptr;
GameAgent::GameAgent() {
    _instance = this;
}

GameAgent* GameAgent::getInstance() {
    return _instance;
}

bool GameAgent::init() {
    return true;
}

void GameAgent::onPause() {

}

void GameAgent::onResume() {

}

void GameAgent::tick(long long deltaT) {
    
}



