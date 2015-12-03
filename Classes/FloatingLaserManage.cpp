//
//  FloatingLaserManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#include "FloatingLaserManage.hpp"
#include "format.h"
#include "intersection.h"
#include "HuntingHeroMetaSuitManage.hpp"
#include "QuestDef.hpp"
#include "SOCommon.h"

USING_NS_CC;
void FloatingLaserManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initLaserThings();
}

void FloatingLaserManage::initLaserThings()
{
    for (int i = 0; i < N_LASER; i++) {
        _laserMachines[i].init(_mainLayer, _mainCamera);
        _laserQueue.push_back(&(_laserMachines[i]));
    }
}

void FloatingLaserManage::op_launchOneLaser() //释放一个激光器
{
    auto laser = _laserQueue.front();
    _laserQueue.pop_front();
    _laserQueue.push_back(laser);
    laser->op_appear({random(-100.f, 100.f), random(65.f, 80.f)}, 20.f);

}

int FloatingLaserManage::op_fetchLaserNumber() //获取天上有几个激光器
{
    int r = 0;
    for (int i = 0; i < N_LASER; i++) {
        if (_laserMachines[i].op_isEnable()) {
            r++;
        }
    }
    return r;
}

void FloatingLaserManage::op_toastLaser(cocos2d::Vec2 target) //放激光
{
    for (int i = 0; i < N_LASER; i++) {
        if (_laserMachines[i].op_isEnable()) {
            _laserMachines[i].op_toastLaser(target);
        }
    }
}

void FloatingLaserManage::update(float dt)
{
    
}