#pragma once
#include "math/Vec3.h"
namespace cc {

enum class MODELTYPE : int32_t {
    MAINCAR = 0,
    CAR = 1,
    HUMAN,
    BYCYCLE,
    TRUCK,
};

enum class CMIACTION : int32_t {
    CREATE,
    REMOVE,
    UPDATE
};

enum class CLASSIFYTYPE : int32_t {
    OPTION_0,
    OPTION_1,
    OPTION_2
};

enum class CMIDATATYPE {
    ACTION = 0,
    MODEL_TYPE,
    CLASSIFY_TYPE,
    //TRACKTYPE,
    SPEED,
    MODEL_ID,
    TIMESTAMP,
    POS_X,
    POS_Y,
    POS_Z,
    EULER_X,
    EULER_Y,
    EULER_Z,
    //MATERIAL_PROPERTY,
    COUNT
};

enum class TRACKTYPE
{
    OPTION_0,
    OPTION_1,
    OPTION_2,
};

struct MaterialProperty {
    float roughness;
    float metallic;
    float occlusion;
    Vec3 albedo;
    Vec3 emissive;
};

struct CMIData {
    CMIACTION action;
    MODELTYPE modelType;
    CLASSIFYTYPE classifyType;
    //TRACKTYPE trackType;
    float speed;
    uint32_t modelID;
    float timeStamp;
    Vec3 position;
    Vec3 eulerAngle;
    //MaterialProperty matProp;
};

} // namespace cc
