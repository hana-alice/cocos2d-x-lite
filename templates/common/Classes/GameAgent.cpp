#include "GameAgent.h"
#include "cocos/bindings/jswrapper/SeApi.h"
#include "base/TypeDef.h"
namespace cc{
GameAgent * GameAgent::_instance = nullptr;
namespace {
    se::Value _CMIDataCmds;
    const uint propSize = 11;

    enum class CC_DLL CMIACTION : uint32_t {
        CREATE,
        REMOVE,
        UPDATE
    };

    struct CMIData {
        CMIACTION action;
        ModelType type;
        float speed;
        uint32_t modelID;
        uint64_t timeStamp;
        cc::Vec3 position;
        cc::Vec3 eulerAngle;
    };
}

std::vector<CMIData> recordCmds;;

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
    //se::Value eg[propSize] = { se::Value(0),
    //se::Value(0),
    //se::Value(3.0),
    //se::Value(20211201),
    //se::Value(0),
    //se::Value(1.0), se::Value(1.0),se::Value(1.0),
    //se::Value(1.0),se::Value(1.0),se::Value(1.0) };

    if (se::ScriptEngine::getInstance()->isValid()) {
        const uint jsonConfigSize = 3;
        /*auto configs = se::Object::createArrayObject(jsonConfigSize);
        se::ValueArray arr;
        arr.reserve(jsonConfigSize);
        for (size_t j = 0; j < jsonConfigSize; j++)
        {
            auto cfg = se::Object::createArrayObject(propSize);
            for (size_t i = 0; i < propSize; i++)
            {
                cfg->setArrayElement(i, eg[i]);
            }
            configs->setArrayElement(j, se::Value(cfg));
        }*/
        if (!recordCmds.empty()) {
            // order matters
            for (size_t i = 0; i < recordCmds.size(); i++) {
                
                
            }
        }
        recordCmds.clear();
        _CMIDataCmds = se::Value(se::Value(1));
        se::ScriptEngine::getInstance()->getGlobalObject()->getProperty("_CMIDataCmds", &_CMIDataCmds);

        _CMIDataCmds.setInt32(2);
    }
}

void GameAgent::createModel(uint32_t modelID, ModelType type, Vec3 position, Vec3 eulerAngle)
{
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData& data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::CREATE;
        it->type = type;
        it->position = position;
        it->eulerAngle = eulerAngle;
    }
    else {
        recordCmds.push_back({
        CMIACTION::CREATE,
        type,
        0,
        modelID,
        _timeStamp,
        position,
        eulerAngle
            });
    }
    
}

void GameAgent::removeModel(uint32_t modelID)
{
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData& data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
        });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::REMOVE;
    }
    else {
        recordCmds.push_back({
        CMIACTION::REMOVE,
        ModelType::COUNT,
        0,
        modelID,
        _timeStamp,
        Vec3(0,0,0),
        Vec3(0,0,0)
        });
    }
}

void GameAgent::updateModel(uint32_t modelID, Vec3 position, Vec3 eulerAngle, float speed)
{
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData& data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
        });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::UPDATE;
    }
    recordCmds.push_back({
        CMIACTION::UPDATE,
        ModelType::COUNT,
        speed,
        modelID,
        _timeStamp,
        position,
        eulerAngle
        });
}

void GameAgent::enable(uint32_t modelID)
{
}

void GameAgent::disable(uint32_t modelID)
{
}

}//namspace cc