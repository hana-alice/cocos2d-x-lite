#pragma once
#include "ccdef.h"
#include "math/Vec3.h"
class Actor {
public:
    bool init(uint32_t modelID, cc::MODELTYPE modelType, cc::CLASSIFYTYPE subType);

    void create(cc::Vec3 position, cc::Vec3 eulerAngle);

    void remove();
    void update(cc::Vec3 position, cc::Vec3 eulerAngle, float speed);

    void updateMaterialProperty(cc::MaterialProperty prop);
    // play animation at once, recall to play from begining again.
    void playAnimation();
    void repalceView();

    void enable();
    void disable();

    void animationOn();
    void animationOff();

    inline const cc::CMIData &getData() { return _data; }

private:
    cc::CMIData _data;
};
