#include "Actor.h"
#include "GameAgent.h"
using namespace cc;

Actor::Actor(uint32_t modelID, MODELTYPE type, cc::CLASSIFYTYPE subType) {
    _data.modelID = modelID;
    _data.modelType = type;
    _data.classifyType = subType;
}
Actor::Actor(Actor&& actor)
{
    if (actor._res) {
        actor._res = false;
    }
    this->_data = actor._data;
    this->_res = true;
}
Actor::~Actor() {
    if(_res)
        GameAgent::getInstance()->removeModel(_data.modelID);
    _res = false;
}

bool Actor::init(cc::Vec3 position, cc::Vec3 eulerAngle) {
    _data.position = position;
    _data.eulerAngle = eulerAngle; 
    GameAgent::getInstance()->createModel(_data.modelID, _data.modelType, _data.classifyType, position, eulerAngle);
    _res = true;
    return true;
}

void Actor::update(Vec3 position, Vec3 eulerAngle, float speed) {
    _data.position = position;
    _data.eulerAngle = eulerAngle;
    _data.speed = speed;
    GameAgent::getInstance()->updateModel(_data.modelID, position, eulerAngle, speed);
}

void Actor::enable() {
    GameAgent::getInstance()->enable(_data.modelID);
}

void Actor::disable() {
    GameAgent::getInstance()->disable(_data.modelID);
}

void Actor::animationOn() {
    GameAgent::getInstance()->animationOn(_data.modelID);
}

void Actor::animationOff() {
    GameAgent::getInstance()->animationOff(_data.modelID);
}
