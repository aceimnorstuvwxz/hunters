//
//  HuntingMonsterManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/2.
//
//

#include "HuntingMonsterManage.hpp"
USING_NS_CC;


void HuntingMonsterManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
}

void HuntingMonsterManage::update(float dt)
{
    _timeLeft -= dt;
    if (_timeLeft <= 0) {
        addMonster(HuntingMonsterGeneralType::NORMAL, HuntingMonsterSpecialType::NONE, false, _currentWave);
        _currentWaveMonsterCnt--;
        if (_currentWaveMonsterCnt == 0) {
            _currentWave++;
            _currentWaveMonsterCnt = 10;
            _timeLeft = 10;
        } else {
            _timeLeft = 2;
        }
    }
}

void HuntingMonsterManage::addMonster(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level)
{
    auto sp = HuntingMonster::create();
    sp->init(_mainLayer, _mainCamera);
    sp->op_configType(generalType, specialType, true, level);
    _monsters.push_back(sp);
}