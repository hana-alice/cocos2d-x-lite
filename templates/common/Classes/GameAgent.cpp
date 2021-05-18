#include "GameAgent.h"
#include "base/TypeDef.h"
#include "cocos/bindings/jswrapper/SeApi.h"
namespace cc {
GameAgent *GameAgent::_instance = nullptr;
namespace {
se::Value _CMIDataCmds;

se::Object *jsonConfigs = nullptr;

} // namespace

std::vector<CMIData> recordCmds;
;

GameAgent::GameAgent() {
    _instance = this;
}

GameAgent *GameAgent::getInstance() {
    return _instance;
}

bool GameAgent::init() {
    return true;
}

bool GameAgent::tick(long long deltaT) {

    se::AutoHandleScope scp;

    if (!jsonConfigs) {
        jsonConfigs = se::Object::createArrayObject(0);
    }

    if (se::ScriptEngine::getInstance()->isValid()) {
        for (size_t j = 0; j < recordCmds.size(); j++) {
            /*auto cfg = se::Object::createArrayObject(propSize);
            for (size_t i = 0; i < propSize; i++) {
                cfg->setArrayElement(i, eg[i]);
            }
            jsonConfigs->setArrayElement(j, se::Value(cfg));*/
        }
        if (!recordCmds.empty()) {
            // order matters
            for (size_t i = 0; i < recordCmds.size(); i++) {
                const uint propSize = (uint32_t)cc::CMIDATATYPE::COUNT;
                auto cfg = se::Object::createArrayObject(propSize);
                const CMIData& cmd = recordCmds[i];

                cfg->setArrayElement(0, se::Value((uint32_t)cmd.action));
                cfg->setArrayElement(1, se::Value((uint32_t)cmd.type));
                cfg->setArrayElement(2, se::Value((uint32_t)cmd.speed));
                cfg->setArrayElement(3, se::Value((uint32_t)cmd.modelID));
                cfg->setArrayElement(4, se::Value((uint32_t)cmd.timeStamp));
                cfg->setArrayElement(5, se::Value((uint32_t)cmd.position.x));
                cfg->setArrayElement(6, se::Value((uint32_t)cmd.position.y));
                cfg->setArrayElement(7, se::Value((uint32_t)cmd.position.z));
                cfg->setArrayElement(8, se::Value((uint32_t)cmd.eulerAngle.x));
                cfg->setArrayElement(9, se::Value((uint32_t)cmd.eulerAngle.y));
                cfg->setArrayElement(10, se::Value((uint32_t)cmd.eulerAngle.z));

                jsonConfigs->setArrayElement(i, se::Value(cfg));
            }
        }
        recordCmds.clear();
        _CMIDataCmds = se::Value(jsonConfigs);

        auto globalJSThis = se::ScriptEngine::getInstance()->getGlobalObject();
        se::Value nsVal;
        if (!globalJSThis->getProperty("_CMIDataCmds", &nsVal)) {
            globalJSThis->setProperty("_CMIDataCmds", _CMIDataCmds);
        }
    }

    return true;
}

void GameAgent::createModel(uint32_t modelID, ModelType type, Vec3 position, Vec3 eulerAngle) {
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::CREATE;
        it->type = type;
        it->position = position;
        it->eulerAngle = eulerAngle;
    } else {
        recordCmds.push_back({CMIACTION::CREATE,
                              type,
                              0,
                              modelID,
                              _timeStamp,
                              position,
                              eulerAngle});
    }
}

void GameAgent::removeModel(uint32_t modelID) {
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::REMOVE;
    } else {
        recordCmds.push_back({CMIACTION::REMOVE,
                              ModelType::COUNT,
                              0,
                              modelID,
                              _timeStamp,
                              Vec3(0, 0, 0),
                              Vec3(0, 0, 0)});
    }
}

void GameAgent::updateModel(uint32_t modelID, Vec3 position, Vec3 eulerAngle, float speed) {
    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::UPDATE;
    }
    recordCmds.push_back({CMIACTION::UPDATE,
                          ModelType::COUNT,
                          speed,
                          modelID,
                          _timeStamp,
                          position,
                          eulerAngle});
}

void GameAgent::enable(uint32_t modelID) {
}

void GameAgent::disable(uint32_t modelID) {
}

} // namespace cc