#include "Model.h"
#include "GameAgent.h"
using namespace cc;
bool Model::init(uint32_t modelID, ModelType type)
{
	_data.modelID = modelID;
	_data.type = type;
	return true;
}

void Model::create(Vec3 position, Vec3 eulerAngle)
{
	_data.position = position;
	_data.eulerAngle = eulerAngle;
	GameAgent::getInstance()->createModel(_data.modelID, _data.type, position, eulerAngle);
}

void Model::remove()
{
	GameAgent::getInstance()->removeModel(_data.modelID, _data.type);
}

void Model::update(Vec3 position, Vec3 eulerAngle, float speed)
{
	_data.position = position;
	_data.eulerAngle = eulerAngle;
	_data.speed = speed;
	GameAgent::getInstance()->updateModel(_data.modelID, _data.type, position, eulerAngle, speed);
}

void Model::enable()
{
	GameAgent::getInstance()->enable(_data.modelID);
}

void Model::disable()
{
	GameAgent::getInstance()->disable(_data.modelID);
}

void Model::animationOn()
{
	GameAgent::getInstance()->animationOn(_data.modelID);
}

void Model::animationOff()
{
	GameAgent::getInstance()->animationOff(_data.modelID);
}
