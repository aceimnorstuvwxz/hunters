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

        HuntingMonsterGeneralType gt;
        float ran = rand_0_1();
        if (ran < (0.05 + _currentWave/50.f*0.3f)) {
            gt = HuntingMonsterGeneralType::GIANT;
        } else if (ran < (0.05 + _currentWave/50.f*0.3f + 0.1f)) {
            gt = HuntingMonsterGeneralType::BIG;
        } else {
            gt = HuntingMonsterGeneralType::NORMAL;
        }


        addMonster(gt, HuntingMonsterSpecialType::NONE, false, _currentWave);
        _currentWaveMonsterCnt--;
        if (_currentWaveMonsterCnt == 0) {
            _currentWave++;
            _currentWaveMonsterCnt = 10;
            _timeLeft = random(20, 50);
        } else {
            _timeLeft = gt == HuntingMonsterGeneralType::GIANT ? random(10, 15) :
            gt == HuntingMonsterGeneralType::BIG? random(8, 10): random(5,8);
        }
    }

    for (auto sp : _monsters) {
        sp->update(dt);
    }
}

void HuntingMonsterManage::addMonster(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level)
{
    auto sp = HuntingMonster::create();
    sp->init(_mainLayer, _mainCamera);
    sp->op_configType(generalType, specialType, true, level);
    _monsters.push_back(sp);
}