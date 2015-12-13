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

    static bool _needTellWave = true;
    _timeLeft -= dt;
    if (_timeLeft <= 0) {

        if ( _needTellWave ) {
            _needTellWave = false;
            _topIconsProtocal->op_addWave();
        }

        // 主类别
        HuntingMonsterGeneralType gt;
        float ran = rand_0_1();
        if (ran < (0.05 + _currentWave/50.f*0.3f)) {
            gt = HuntingMonsterGeneralType::GIANT;
        } else if (ran < (0.05 + _currentWave/50.f*0.3f + 0.1f)) {
            gt = HuntingMonsterGeneralType::BIG;
        } else {
            gt = HuntingMonsterGeneralType::NORMAL;
        }

        if (_currentWave > 15) {
            float newR = rand_0_1();
            if (newR < 0.05f) {
                gt = HuntingMonsterGeneralType::SUPER;
            }
        }
        
        if (_currentWave > 25) {
            float newR = rand_0_1();
            if (newR < 0.09f) {
                gt = HuntingMonsterGeneralType::SUPER;
            } else if (newR < 0.04f) {
                gt = HuntingMonsterGeneralType::TITAN;
            }
        }

        //盾
        bool hasShiled = false;
        int shieldCount = 0;
        if (_currentWave > 10) {
            hasShiled = rand_0_1() < std::min(0.5f, 0.1f+0.5f*_currentWave/50.f);
            shieldCount = hasShiled ? (1 + _currentWave/15) : 0;
        }

        //特殊类别
        HuntingMonsterSpecialType st = HuntingMonsterSpecialType::NONE;
        if (gt == HuntingMonsterGeneralType::NORMAL ||
            gt == HuntingMonsterGeneralType::BIG)
        {

            float rcfg = 0.03f + 0.1f * _currentWave/30.f;
            if (rand_0_1() < rcfg) {
                st = HuntingMonsterSpecialType::ACCE;
            } else if (_currentWave > 13 && rand_0_1() < rcfg) {
                st = HuntingMonsterSpecialType::ATKFIRE;
            }
        }


        //等级
        int level = _currentWave;
        float ran2 = rand_0_1();
        if (ran2 < 0.1) {
            level += 3;
        } else if (ran2 < 0.2) {
            level += 2;
        } else if (ran2 < 0.3) {
            level += 1;
        }
        level += 7;//去掉开始的不好看的敌人
        level = std::min(40, level);
        addMonster(gt, st, hasShiled, level, shieldCount);
        _currentWaveMonsterCnt--;
        if (_currentWaveMonsterCnt == 0) {
            _currentWave++;
            _needTellWave = true;
            _currentWaveMonsterCnt = random(8, 15);
            _timeLeft = random(15, 30);

            //龙卷风
            bool t0 = ((_currentWave == 3 || _currentWave == 10 || _currentWave == 15 )|| (_currentWave > 3 && rand_0_1() < (0.2f + 0.3*(_currentWave/40.f))));
            bool t1 = (_currentWave > 15 && rand_0_1() < (0.1f + 0.25*(_currentWave/40.f)));

            _tornadoManageProtocal->op_configTornado(t0, random(-60, 50), t1, random(20, 100));

            //风
            if (_currentWave > 5) {
                _windBarProtocal->op_configWind(random(0, 5));
            } else {
                _windBarProtocal->op_configWind(random(0, 2));
            }
//            ACSoundManage::s()->play(ACSoundManage::SN_WIND_CHANGE);


        } else {
            _timeLeft =
            gt == HuntingMonsterGeneralType::TITAN ? random(15, 20) :
            gt == HuntingMonsterGeneralType::SUPER ? random(13, 18) :
            gt == HuntingMonsterGeneralType::GIANT ? random(10, 15) :
            gt == HuntingMonsterGeneralType::BIG? random(8, 10): random(5,8);
            _timeLeft = _timeLeft*0.6;
            if (rand_0_1() < 0.2) {
                _timeLeft = _timeLeft*3;//有一定几率拉开一定距离
            }
        }
    }

    for (auto sp : _monsters) {
        sp->update(dt);
    }


    for (auto iter = _monsters.begin(); iter != _monsters.end(); ) {
        if ((*iter)->isDead() ){
            iter = _monsters.erase(iter);
        } else {
            iter++;
        }
    }

}

void HuntingMonsterManage::addMonster(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level, int shieldCount)
{
    auto sp = HuntingMonster::create();
    sp->init(_mainLayer, _mainCamera);
    sp->op_configType(generalType, specialType, hasShield, level, shieldCount);
    sp->configProtocal(_energyBarProtocal, _floatingLaserManageProtocal, _effetcManageProtocal, _topIconsProtocal, _particleManageProtocal);
    _monsters.push_back(sp);
}

void HuntingMonsterManage::op_dealCollision(ArrowUnit& arrow) //bool 返回，true 表示撞到了，那么对于 arrowManage 可以去掉此 arrow 了。//arrow 的所有效果都在另一侧制造。
{
    //在这里处理 箭与怪物的碰撞
    //炸弹在 与地板碰撞处理，其它的箭也会与地板处理（要消失！）
    //这里的碰撞的难点在于对于穿透箭的处理。
    for (auto iter = _monsters.begin(); iter != _monsters.end(); ) {
        if ((*iter)->isDead() ){
            iter = _monsters.erase(iter);
        } else {
            iter++;
        }
    }

    for (auto mon : _monsters) {
        mon->op_dealWithArrow(arrow);
    }
    for (auto iter = _monsters.begin(); iter != _monsters.end(); ) {
        if ((*iter)->isDead() ){
            iter = _monsters.erase(iter);
        } else {
            iter++;
        }
    }
    if (arrow._leftHitTimes > 0) {
        groundCollision(arrow);//地板测试
    }
}

void HuntingMonsterManage::op_thunder(float pos)
{
    for (auto sp : _monsters) {
        sp->op_thunderTest(pos);
    }
}

void HuntingMonsterManage::op_bomb(float pos, int grade)
{
    for (auto sp : _monsters) {
        sp->op_bombTest(pos, grade);
    }
}

void HuntingMonsterManage::groundCollision(ArrowUnit& arrow)
{
    if (arrow._pxNode->getPositionZ() <= 0) {
        if (arrow._type == HuntingArrowType::BOMB_0 ||
            arrow._type == HuntingArrowType::BOMB_1 ||
            arrow._type == HuntingArrowType::BOMB_2 ) {
            _effetcManageProtocal->op_bomb(arrow._type == HuntingArrowType::BOMB_0 ? 0 :
                                           arrow._type == HuntingArrowType::BOMB_1 ? 1 : 2, {arrow._pxNode->getPositionY(), arrow._pxNode->getPositionZ()});
        }
        if (arrow._type == HuntingArrowType::SLOW_2) {
            _effetcManageProtocal->op_thunder({arrow._pxNode->getPositionY(), arrow._pxNode->getPositionZ()});
        }
        arrow._leftHitTimes = 0;
    }
}

void HuntingMonsterManage::op_configPaused(bool pause)
{
    for (auto& m : _monsters) {
        m->op_configPaused(pause);
    }
}
