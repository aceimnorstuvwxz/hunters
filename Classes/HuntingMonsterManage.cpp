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

        int level = _currentWave;
        float ran2 = rand_0_1();
        if (ran2 < 0.1) {
            level += 2;
        } else if (ran2 < 0.3) {
            level += 1;
        }
        level = std::min(40, level);
        addMonster(gt, HuntingMonsterSpecialType::NONE, false, level);
        _currentWaveMonsterCnt--;
        if (_currentWaveMonsterCnt == 0) {
            _currentWave++;
            _currentWaveMonsterCnt = random(10, 20);
            _timeLeft = random(15, 30);
        } else {
            _timeLeft = gt == HuntingMonsterGeneralType::GIANT ? random(10, 15) :
            gt == HuntingMonsterGeneralType::BIG? random(8, 10): random(5,8);
            _timeLeft = _timeLeft*0.6;
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
    sp->configProtocal(_energyBarProtocal, _floatingLaserManageProtocal);
    _monsters.push_back(sp);
}

void HuntingMonsterManage::op_dealCollision(ArrowUnit& arrow) //bool 返回，true 表示撞到了，那么对于 arrowManage 可以去掉此 arrow 了。//arrow 的所有效果都在另一侧制造。
{
    //在这里处理 箭与怪物的碰撞
    //炸弹在 与地板碰撞处理，其它的箭也会与地板处理（要消失！）
    //这里的碰撞的难点在于对于穿透箭的处理。

    for (auto iter = _monsters.begin(); iter != _monsters.end(); ) {
        if ((*iter)->op_dealWithArrow(arrow) ){
            iter = _monsters.erase(iter);
        } else {
            iter++;
        }
    }
    if (arrow._leftHitTimes > 0) {
        groundCollision(arrow);//地板测试
    }
}

void HuntingMonsterManage::groundCollision(ArrowUnit& arrow)
{
    if (arrow._pxNode->getPositionZ() <= 0) {
        arrow._leftHitTimes = 0;
    }
}
