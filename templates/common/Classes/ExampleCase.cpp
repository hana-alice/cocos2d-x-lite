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
    std::ifstream ifs("E:\\tmp\\auto-drive\\auto-drive\\native\\engine\\common\\Classes\\transfer.json");
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
            int32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint32_t timeStamp = atoi(singlePack["timestamp"].GetString());
            float posX = atof(singlePack["positionX"].GetString());
            float posY = atof(singlePack["positionY"].GetString());
            float posZ = atof(singlePack["positionZ"].GetString());
            float eulerX = atof(singlePack["eulerX"].GetString());
            float eulerY = atof(singlePack["eulerY"].GetString());
            float eulerZ = atof(singlePack["eulerZ"].GetString());
            _simulateDatas.push_back({CMIACTION::CREATE,
                                      (MODELTYPE)modelType,
                                      0,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "updateModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint32_t timeStamp = atoi(singlePack["timestamp"].GetString());
            float posX = atof(singlePack["positionX"].GetString());
            float posY = atof(singlePack["positionY"].GetString());
            float posZ = atof(singlePack["positionZ"].GetString());
            float eulerX = atof(singlePack["eulerX"].GetString());
            float eulerY = atof(singlePack["eulerY"].GetString());
            float eulerZ = atof(singlePack["eulerZ"].GetString());
            float speed = atof(singlePack["speed"].GetString());
            _simulateDatas.push_back({CMIACTION::UPDATE,
                                      MODELTYPE::CAR, // wahtever
                                      speed,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "removeModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint32_t timeStamp = atoi(singlePack["timestamp"].GetString());
            _simulateDatas.push_back({CMIACTION::REMOVE,
                                      MODELTYPE::CAR,
                                      0.0f,
                                      modelID,
                                      timeStamp,
                                      Vec3(0.0f, 0.0f, 0.0f),
                                      Vec3(0.0f, 0.0f, 0.0f)});
        }
    }
    std::sort(_simulateDatas.begin(), _simulateDatas.end(), [](const CMIData &lhs, const CMIData &rhs) {
        return lhs.timeStamp < rhs.timeStamp;
    });
}

void ExampleCase::update() {
    static std::chrono::steady_clock::time_point firstFrame = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point prevTime = firstFrame;
    auto curTime = std::chrono::steady_clock::now();
    long long secs = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - firstFrame).count();

    static uint32_t delta = 0;
    delta += std::chrono::duration_cast<std::chrono::milliseconds>(curTime - prevTime).count();
    prevTime = curTime;

    if (delta < 200) {
        return;
    } else {
        delta -= 200;
    }

    static std::unordered_map<uint32_t, Actor> actorMap;

    GameAgent *agent = GameAgent::getInstance();
    for (int i = 0; i < _simulateDatas.size(); i++) {
        if (secs - 3 == _simulateDatas[i].timeStamp) {
            const CMIData &data = _simulateDatas[i];

            if (data.action == CMIACTION::CREATE) {
                Actor actor;
                actor.init(data.modelID, data.type);
                actor.create(data.position, data.eulerAngle);
                actorMap.insert({data.modelID, actor});
            } else {
                auto iter = actorMap.find(data.modelID);
                if (iter != actorMap.end()) {
                    switch (data.action) {
                        case CMIACTION::UPDATE:
                            iter->second.update(data.position, data.eulerAngle, data.speed);
                            break;
                        case CMIACTION::REMOVE: {
                            iter->second.remove();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }
    }
}