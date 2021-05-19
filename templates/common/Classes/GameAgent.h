/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#pragma once
/**
 @brief    The cocos2d Application.
 
 The reason for implement as private inheritance is to hide some interface call by Director.
 */
#include "math/Vec3.h"
#include "ccdef.h"

class Game;
namespace cc {

typedef bool (*OnTickCallbackFunc)(long long deltaTime);
class GameAgent {

    friend class Game;

public:
    static GameAgent *getInstance();

    bool init();
    

    void createModel(uint32_t modelID, MODELTYPE type, Vec3 position, Vec3 eulerAngle);
    void removeModel(uint32_t modelID, MODELTYPE type);
    void updateModel(uint32_t modelID, MODELTYPE type, Vec3 position, Vec3 eulerAngle, float speed);

    void enable(uint32_t modelID);
    void disable(uint32_t modelID);

    void animationOn(uint32_t modelID);
    void animationOff(uint32_t modelID);

private:
    GameAgent(Game* gameHandler);

    void onPause();
    void onResume();
    void tick();
    

    static GameAgent *_instance;
    uint32_t _timeStamp = 0;
    Game* _gameHadler = nullptr;
};
} // namespace cc