#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Actor.h"
#include "ExampleCase.h"
#include "ccdef.h"
#include "cocos/base/Log.h"
#include "json/document.h"
#include "json/rapidjson.h"

using namespace rapidjson;
using namespace cc;
void ExampleCase::init() {
    std::ifstream ifs("E:\\tmp\\autodrivedemo1\\autodrivedemo\\autodrivedemo\\native\\engine\\common\\Classes\\transfer.json");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    Document document;
    document.Parse(content.c_str());
    assert(document.IsObject());
    assert(document.HasMember("transferData"));

    const Value &data = document["transferData"];
    assert(data.IsArray());

    _simulateDatas.reserve(data.Size());
    for (SizeType i = 0; i < data.Size(); i++) {
        assert(data[i].HasMember("timestamp"));
        assert(data[i].HasMember("action"));
        assert(data[i].HasMember("modelID"));

        const auto &singlePack = data[i];
        const auto str = singlePack["action"].GetString();
        if (strcmp(singlePack["action"].GetString(), "createModel") == 0) {

            int32_t modelType = singlePack["modelType"].GetInt();
            uint32_t modelID = singlePack["modelID"].GetInt();
            float timeStamp = singlePack["timestamp"].GetFloat();
            int32_t classifyType = singlePack["classifyType"].GetInt();

            float posX = singlePack["pX"].GetFloat();
            float posY = singlePack["pY"].GetFloat();
            float posZ = singlePack["pZ"].GetFloat();
            float eulerX = singlePack["eX"].GetFloat();
            float eulerY = singlePack["eY"].GetFloat();
            float eulerZ = singlePack["eZ"].GetFloat();
            
            _simulateDatas.push_back({CMIACTION::CREATE,
                                      (MODELTYPE)modelType,
                                      (CLASSIFYTYPE)classifyType,
                                      0,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "updateModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = singlePack["modelID"].GetInt();
            float timeStamp = singlePack["timestamp"].GetFloat();
            float posX = singlePack["pX"].GetFloat();
            float posY = singlePack["pY"].GetFloat();
            float posZ = singlePack["pZ"].GetFloat();
            float eulerX = singlePack["eX"].GetFloat();
            float eulerY = singlePack["eY"].GetFloat();
            float eulerZ = singlePack["eZ"].GetFloat();
            float speed = singlePack["speed"].GetFloat();
            _simulateDatas.push_back({CMIACTION::UPDATE,
                                      MODELTYPE::CAR, // wahtever
                                      CLASSIFYTYPE::OPTION_0,
                                      speed,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "removeModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = singlePack["modelID"].GetInt();
            float timeStamp = singlePack["timestamp"].GetFloat();
            _simulateDatas.push_back({CMIACTION::REMOVE,
                                      MODELTYPE::CAR,
                                      CLASSIFYTYPE::OPTION_0,
                                      0.0f,
                                      modelID,
                                      timeStamp,
                                      Vec3(0.0f, 0.0f, 0.0f),
                                      Vec3(0.0f, 0.0f, 0.0f)});
        }
    }
    /*std::sort(_simulateDatas.begin(), _simulateDatas.end(), [](const CMIData &lhs, const CMIData &rhs) {
        return lhs.timeStamp < rhs.timeStamp;
    });*/
}

void ExampleCase::update() {
    static std::chrono::steady_clock::time_point firstFrame = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point prevTime = firstFrame;
    auto curTime = std::chrono::steady_clock::now();
    long long secs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - firstFrame).count();

    static uint32_t delta = 0;
    delta += std::chrono::duration_cast<std::chrono::milliseconds>(curTime - prevTime).count();
    prevTime = curTime;

    static std::unordered_map<uint32_t, Actor> actorMap;

    GameAgent *agent = GameAgent::getInstance();
    float jsTimeStamp = agent->getGlobalTimeStamp();
    for (int i = 0; i < _simulateDatas.size(); i++) {
        if (fabs(jsTimeStamp - _simulateDatas[i].timeStamp) < 0.016) {
            const CMIData &data = _simulateDatas[i];

            auto iter = actorMap.find(data.modelID);
            if (iter == actorMap.end()) {
                if (data.action == CMIACTION::CREATE) {
                    Actor actor;
                    actor.init(data.modelID, data.modelType, data.classifyType);
                    actor.create(data.position, data.eulerAngle);
                    actorMap.insert({ data.modelID, actor });
                }

            } else {
                //auto iter = actorMap.find(data.modelID);
                //if (iter != actorMap.end()) {
                    switch (data.action) {
                        case CMIACTION::UPDATE:
                            iter->second.update(data.position, data.eulerAngle, data.speed);
                            break;
                        case CMIACTION::REMOVE: {
                            iter->second.remove();
                            actorMap.erase(iter);
                            break;
                        }
                        default:
                            break;
                    }
                //}
            }
        }
    }
}