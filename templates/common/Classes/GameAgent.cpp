#include "GameAgent.h"
#include "base/TypeDef.h"
#include "cocos/bindings/event/CustomEventTypes.h"
#include "cocos/bindings/event/EventDispatcher.h"
#include "cocos/bindings/jswrapper/SeApi.h"
#include "cocos/bindings/manual/jsb_classtype.h"
#include "cocos/bindings/manual/jsb_global.h"
#include "cocos/bindings/manual/jsb_module_register.h"

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
    se::ScriptEngine* se = se::ScriptEngine::getInstance();

    jsb_set_xxtea_key("");
    jsb_init_file_operation_delegate();

#if defined(CC_DEBUG) && (CC_DEBUG > 0)
    // Enable debugger here
    jsb_enable_debugger("0.0.0.0", 6086, false);
#endif

    se->setExceptionCallback([](const char* location, const char* message, const char* stack) {
        // Send exception information to server like Tencent Bugly.
        CC_LOG_ERROR("\nUncaught Exception:\n - location :  %s\n - msg : %s\n - detail : \n      %s\n", location, message, stack);
        });

    jsb_register_all_modules();

    se->start();

    se::AutoHandleScope hs;
    jsb_run_script("jsb-adapter/jsb-builtin.js");
    jsb_run_script("main.js");

    se->addAfterCleanupHook([]() {
        JSBClassType::destroy();
        });
    return true;
}

void GameAgent::onPause()
{
    cc::CustomEvent event;
    event.name = EVENT_COME_TO_BACKGROUND;
    cc::EventDispatcher::dispatchCustomEvent(event);
    cc::EventDispatcher::dispatchEnterBackgroundEvent();
}

void GameAgent::onResume()
{
    cc::CustomEvent event;
    event.name = EVENT_COME_TO_FOREGROUND;
    cc::EventDispatcher::dispatchCustomEvent(event);
    cc::EventDispatcher::dispatchEnterForegroundEvent();
}

void GameAgent::tick() {

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
                const CMIData &cmd = recordCmds[i];

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
}

void GameAgent::createModel(uint32_t modelID, ModelType type, Vec3 position, Vec3 eulerAngle) {
/*    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::CREATE;
        it->type = type;
        it->position = position;
        it->eulerAngle = eulerAngle;
    } else */{
        recordCmds.push_back({CMIACTION::CREATE,
                              type,
                              0,
                              modelID,
                              _timeStamp,
                              position,
                              eulerAngle});
    }
}

void GameAgent::removeModel(uint32_t modelID, ModelType type) {
/*    auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
        return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    });

    if (it != recordCmds.end()) {
        it->action = CMIACTION::REMOVE;
    } else*/ {
        recordCmds.push_back({CMIACTION::REMOVE,
                              ModelType::CAR,
                              0,
                              modelID,
                              _timeStamp,
                              Vec3(0, 0, 0),
                              Vec3(0, 0, 0)});
    }
}

void GameAgent::updateModel(uint32_t modelID, ModelType type, Vec3 position, Vec3 eulerAngle, float speed) {
    //auto it = std::find_if(recordCmds.begin(), recordCmds.end(), [modelID](const CMIData &data) {
    //    return modelID == data.modelID && data.action != CMIACTION::REMOVE;
    //});
    //
    //if (it == recordCmds.end())
    //    return;

    // for Example Presentation TS update every second, merge update when frame driven.
    // this is friednly to increase frame rate.
    recordCmds.push_back({CMIACTION::UPDATE,
                          ModelType::CAR,
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

void GameAgent::animationOn(uint32_t modelID)
{
}

void GameAgent::animationOff(uint32_t modelID)
{
}

} // namespace cc