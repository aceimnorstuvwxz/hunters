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
    _waiting = true;
    _angle = angle;
    _strenth = strenth;
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        if (_headIcons[i].op_isAlive()) {
            cnt++;
        }
    }
    _timeTarget = QuestDef::BOW_TOAST_STEP * cnt;
}

void HuntingHerosManage::op_tellArrowIdle()
{
    /*if (_waiting) {
        this->toastBowAst();
    }*/
}
void HuntingHerosManage::toastBowAst() //以这个角度开始放箭
{
    const float time_step = QuestDef::BOW_TOAST_STEP;
    _waiting = false;
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        if (_positionMap[i]->op_isAlive()) {
            _mainCamera->scheduleOnce([this,i](float dt) {
                if (_positionMap[i]->op_fetchHero())
                    _positionMap[i]->op_fetchHero()->op_toastShoot(_angle,_strenth);
            }, cnt*time_step, fmt::sprintf("arrow shoot per node %d", i));
            cnt++;
        }
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
    _timePast += dt;
    if (_waiting && _timePast > _timeTarget) {
        toastBowAst();
        _timePast = 0;
    }
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

bool HuntingHerosManage::op_fetchIsHeadsBusy() //获取是否升级忙，则不会放箭
{
    bool ret = false;
    for (int i = 0; i < 4; i++) {
        if (_headIcons[i].op_isHeadBusy()) {ret = true;break;}
    }
    return ret;
}

void HuntingHerosManage::op_configPaused(bool pause)
{
    for (auto& h : _headIcons) {
        h.op_fetchHero(true)->op_configPaused(pause);
    }
}