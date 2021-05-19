#include "Actor.h"
#include "GameAgent.h"
using namespace cc;
bool Actor::init(uint32_t modelID, MODELTYPE type)
{
	_data.modelID = modelID;
	_data.type = type;
	return true;
}

void Actor::create(Vec3 position, Vec3 eulerAngle)
{
	_data.position = position;
	_data.eulerAngle = eulerAngle;
	GameAgent::getInstance()->createModel(_data.modelID, _data.type, position, eulerAngle);
}

void Actor::remove()
{
	GameAgent::getInstance()->removeModel(_data.modelID, _data.type);
}

void Actor::update(Vec3 position, Vec3 eulerAngle, float speed)
{
	_data.position = position;
	_data.eulerAngle = eulerAngle;
	_data.speed = speed;
	GameAgent::getInstance()->updateModel(_data.modelID, _data.type, position, eulerAngle, speed);
}

void Actor::enable()
{
	GameAgent::getInstance()->enable(_data.modelID);
}

void Actor::disable()
{
	GameAgent::getInstance()->disable(_data.modelID);
}

void Actor::animationOn()
{
	GameAgent::getInstance()->animationOn(_data.modelID);
}

void Actor::animationOff()
{
	GameAgent::getInstance()->animationOff(_data.modelID);
}
