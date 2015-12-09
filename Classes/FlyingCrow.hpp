//
//  FlyingCrow.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/6.
//
//

#ifndef FlyingCrow_hpp
#define FlyingCrow_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"

class FlyingCrow:public FlyingCrowProtocal
{
public:

    static std::shared_ptr<FlyingCrow> create() { return std::make_shared<FlyingCrow>(); }

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(TopIconsProtocal* topIconsProtocal, GlobalParticleManageProtocal* particleManage) {
        _topIconsProtocal = topIconsProtocal;
        _particleManage = particleManage;
    }


    virtual void op_config(bool acce, bool shield, cocos2d::Vec2 relativePosition);
    virtual bool op_dealWithArrow(ArrowUnit& arrow);
    virtual void op_configCry() { _need2cry = true;}


    void update(float dt);


    void toastDead(bool needDown = true);
    bool isDead() {return !_alive;}

protected:
    GlobalParticleManageProtocal* _particleManage;
    TopIconsProtocal* _topIconsProtocal;
    bool _need2cry = false;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();
    bool _alive = true;

    DynamicPixelNode* _dxCrow;
    PixelNode* _pxShield;
    float _speed;
    float _acce;

    PixelNode* _pxCrowShit;
    float _shitSpeed = 0;


    void initCrowThings();

};


#endif /* FlyingCrow_hpp */
