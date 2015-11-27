//
//  HuntingHerosManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#include "HuntingHerosManage.hpp"
#include "format.h"
#include "intersection.h"
#include "SOCommon.h"

USING_NS_CC;

void HuntingHerosManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHeadsThings();
}

void HuntingHerosManage::op_aimingStart() //power bar 通知开始瞄准
{
    for (auto& head : _headIcons) {
        head.op_fetchHero()->op_startAiming();
    }

}

void HuntingHerosManage::op_aimingCancel() //power bar 通知瞄准取消
{

    for (auto& head : _headIcons) {
        head.op_fetchHero()->op_stopAiming();
    }

}

void HuntingHerosManage::op_configAiming(float angle, float strenth) //设置瞄准的参数
{

    for (auto& head : _headIcons) {
        head.op_fetchHero()->op_configAiming(angle, strenth);
    }
}

void HuntingHerosManage::op_toastBow(float angle, float strenth) //以这个角度开始放箭
{
    for (auto& head : _headIcons) {
        head.op_fetchHero()->op_toastShoot();
    }
}


void HuntingHerosManage::update(float dt)
{

}

void HuntingHerosManage::initHeadsThings()
{
    for (int i = 0; i < 4; i++) {
        _headIcons[i].init(_mainLayer, _mainCamera);
        _headIcons[i].op_configPosition(static_cast<HeroPositionType>(i), true);
        _headIcons[i].configProtocals(this);
        _positionMap[i] = &_headIcons[i];
    }

}
