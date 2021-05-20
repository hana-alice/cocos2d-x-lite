#pragma once
#include "ccdef.h"
#include "math/Vec3.h"
class Actor {
public:
    Actor(uint32_t modelID, cc::MODELTYPE modelType, cc::CLASSIFYTYPE subType);
    Actor(Actor&& actor);

    ~Actor();

    bool init(cc::Vec3 position, cc::Vec3 eulerAngle);
    void update(cc::Vec3 position, cc::Vec3 eulerAngle, float speed);

    void enable();
    void disable();

    void animationOn();
    void animationOff();

    inline const cc::CMIData &getData() { return _data; }

private:
    Actor(const Actor& actor) = delete;
    Actor& operator=(const Actor& actor) = delete;

    bool _res = false;
    cc::CMIData _data;
    
};
