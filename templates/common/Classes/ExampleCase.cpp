#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "ExampleCase.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "ccdef.h"
#include "Model.h"

using namespace rapidjson;
using namespace cc;
void ExampleCase::init() {
    std::ifstream ifs("D:\\dev\\project\\auto-drive111\\auto-drive\\native\\engine\\common\\Classes\\transfer.json");
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
            uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint64_t timeStamp = atoi(singlePack["timestamp"].GetString());
            float posX = atof(singlePack["positionX"].GetString());
            float posY = atof(singlePack["positionY"].GetString());
            float posZ = atof(singlePack["positionZ"].GetString());
            float eulerX = atof(singlePack["eulerX"].GetString());
            float eulerY = atof(singlePack["eulerY"].GetString());
            float eulerZ = atof(singlePack["eulerZ"].GetString());
            _simulateDatas.push_back({CMIACTION::CREATE,
                                      (ModelType)modelType,
                                      0,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "updateModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint64_t timeStamp = atoi(singlePack["timestamp"].GetString());
            float posX = atof(singlePack["positionX"].GetString());
            float posY = atof(singlePack["positionY"].GetString());
            float posZ = atof(singlePack["positionZ"].GetString());
            float eulerX = atof(singlePack["eulerX"].GetString());
            float eulerY = atof(singlePack["eulerY"].GetString());
            float eulerZ = atof(singlePack["eulerZ"].GetString());
            float speed = atof(singlePack["speed"].GetString());
            _simulateDatas.push_back({CMIACTION::UPDATE,
                                      ModelType::CAR, // wahtever
                                      speed,
                                      modelID,
                                      timeStamp,
                                      Vec3(posX, posY, posZ),
                                      Vec3(eulerX, eulerY, eulerZ)});
        } else if (strcmp(singlePack["action"].GetString(), "removeModel") == 0) {
            //uint32_t modelType = atoi(singlePack["modelType"].GetString());
            uint32_t modelID = std::hash<std::string>{}(singlePack["modelID"].GetString());
            uint64_t timeStamp = atoi(singlePack["timestamp"].GetString());
            _simulateDatas.push_back({CMIACTION::REMOVE,
                                      ModelType::CAR,
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
    static std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();
    auto curTime = std::chrono::steady_clock::now();
    long long secs = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - prevTime).count();

    static std::unordered_map<uint32_t, Model> modelMap;

    GameAgent *agent = GameAgent::getInstance();
    for (int i = 0; i < _simulateDatas.size(); i++) {
        if (secs == _simulateDatas[i].timeStamp) {
            
            const CMIData &data = _simulateDatas[i];
            if (data.action == CMIACTION::CREATE) {
                Model model;
                model.init(data.modelID, data.type);
                model.create(data.position, data.eulerAngle);
                modelMap.insert({ data.modelID, model});
            } else {
                auto iter = std::find_if(modelMap.begin(), modelMap.end(), [data](std::unordered_map<uint32_t, Model>::value_type val) {
                    return val.second.getData().modelID == data.modelID;
                });
                if (iter != modelMap.end()) {
                    switch (data.action) {
                    case CMIACTION::UPDATE:
                        iter->second.update(data.position, data.eulerAngle, data.speed);
                        break;
                    case CMIACTION::REMOVE:
                    {
                        iter->second.remove();
                        modelMap.erase(iter);
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