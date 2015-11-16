//
//  ProgressInfo.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#include "ProgressInfo.hpp"
#include "cocos2d.h"

USING_NS_CC;

ProgressInfo ProgressInfo::_instance;


void ProgressInfo::load() //从持久化中读出已有的进度数据
{
    //TODO real laod
    _hasProgress =  UserDefault::getInstance()->getBoolForKey("_hasProgress", false);
    if (_hasProgress) {
        _levelIndex = UserDefault::getInstance()->getIntegerForKey("_levelIndex");
        _levelInitBlood = UserDefault::getInstance()->getIntegerForKey("_levelInitBlood");
        _levelInitBloodMax = UserDefault::getInstance()->getIntegerForKey("_levelInitBloodMax");
        _levelInitAttackMin = UserDefault::getInstance()->getIntegerForKey("_levelInitAttackMin");
        _levelInitAttackMax = UserDefault::getInstance()->getIntegerForKey("_levelInitAttackMax");
        _levelInitPower = UserDefault::getInstance()->getIntegerForKey("_levelInitPower");
    } else {
        reset();
    }
}
void ProgressInfo::save() //保存当前的进度数据到持久化中
{
    UserDefault::getInstance()->setBoolForKey("_hasProgress", _hasProgress);

    UserDefault::getInstance()->setIntegerForKey("_levelIndex", _levelIndex);
    UserDefault::getInstance()->setIntegerForKey("_levelInitBlood", _levelInitBlood);
    UserDefault::getInstance()->setIntegerForKey("_levelInitBloodMax", _levelInitBloodMax);
    UserDefault::getInstance()->setIntegerForKey("_levelInitAttackMin", _levelInitAttackMin);
    UserDefault::getInstance()->setIntegerForKey("_levelInitAttackMax", _levelInitAttackMax);
    UserDefault::getInstance()->setIntegerForKey("_levelInitPower", _levelInitPower);
}
void ProgressInfo::reset() //新游戏时需要先重置进度及hero的数据
{
    _levelIndex = 0;

    // 在这里配置Hero的初始属性
    _levelInitBlood = 100;
    _levelInitBloodMax = 100;
    _levelInitAttackMin = 2;
    _levelInitAttackMax = 5;
    _levelInitPower = 3;
}

void ProgressInfo::saveLevelResult(int blood, int bloodMax, int attackMin, int attackMax, int power) //在关卡BOSS打败之后调用，来保持进度，levelIndex会自动加1，会立即持久化
{
    _hasProgress = true;

    _levelIndex++;

    _levelInitBlood = blood;
    _levelInitBloodMax = bloodMax;
    _levelInitAttackMin = attackMin;
    _levelInitAttackMax = attackMax;
    _levelInitPower = power;

    save();
}
