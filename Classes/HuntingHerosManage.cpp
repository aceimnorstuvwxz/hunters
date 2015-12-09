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
#include "QuestDef.hpp"

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
        if (head.op_fetchHero())
        head.op_fetchHero()->op_startAiming();
    }

}

void HuntingHerosManage::op_aimingCancel() //power bar 通知瞄准取消
{

    for (auto& head : _headIcons) {
        if (head.op_fetchHero())

        head.op_fetchHero()->op_stopAiming();
    }

}

void HuntingHerosManage::op_configAiming(float angle, float strenth) //设置瞄准的参数
{

    for (auto& head : _headIcons) {
        if (head.op_fetchHero())

        head.op_fetchHero()->op_configAiming(angle, strenth);
    }
}

void HuntingHerosManage::op_toastBow(float angle, float strenth) //以这个角度开始放箭
{
    const float time_step = QuestDef::BOW_TOAST_STEP;
    for (int i = 0; i < 4; i++) {
        _mainCamera->scheduleOnce([this,i,angle, strenth](float dt) {
            if (_positionMap[i]->op_fetchHero())
            _positionMap[i]->op_fetchHero()->op_toastShoot(angle,strenth);
        }, i*time_step, fmt::sprintf("dfgfdg %d", i));
    }
}
void HuntingHerosManage::op_tellGoldChange() //被通知金币改变
{
    for (auto& h : _headIcons) {
        h.op_tellGoldChange();
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
