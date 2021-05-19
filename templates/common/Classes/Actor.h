#pragma once
#include "math/Vec3.h"
#include "ccdef.h"
class Actor
{
public:
    bool init(uint32_t modelID, cc::MODELTYPE type);

    void create(cc::Vec3 position, cc::Vec3 eulerAngle);
    void remove();
    void update(cc::Vec3 position, cc::Vec3 eulerAngle, float speed);

    void enable();
    void disable();

    void animationOn();
    void animationOff();

    inline const cc::CMIData& getData() { return _data; }

private:
    cc::CMIData _data;
};

