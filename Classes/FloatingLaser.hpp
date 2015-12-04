//
//  FloatingLaser.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#ifndef FloatingLaser_hpp
#define FloatingLaser_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"

class FloatingLaser:FloatingLaserProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    virtual void op_appear(cocos2d::Vec2 pos, float time); //出现的位置 从上方降下来 time 有效时间，时间一到 自动消失
    virtual void op_toastLaser(cocos2d::Vec2 target); //释放一个激光，转动角度，释放激光，有后坐力表现

    virtual bool op_isEnable() {
        return _enable;
    }
    void update(float dt);

protected:
    bool _enable = false;
    float _leftTime = 0;
    cocos2d::Vec2 _position;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    PixelNode* _pxMachine;
    static constexpr int N_NUM_LASER = 4;
    std::deque<PixelNode*> _pxLasers;
    PixelTextNode* _ptxTime;
    void initLaserThings();

};

#endif /* FloatingLaser_hpp */
