#pragma once
#include "math/Vec3.h"
namespace cc {

    enum ModelType : UINT32 {
        MAINCAR,
        CAR,
        HUMAN,
        BYCYCLE,
        TRUCK,
        COUNT // COUNT
    };

    enum class CMIACTION : uint32_t {
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
	
    enum class CMIDATATYPE {
        ACTION = 0,
        MODEL_TYPE,
        SPEED,
        MODEL_ID,
        TIMESTAMP,
        POS_X,
        POS_Y,
        POS_Z,
        EULER_X,
        EULER_Y,
        EULER_Z,
        COUNT
    };
}
