//
//  ACSoundManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#include "ACSoundManage.hpp"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "format.h"
USING_NS_CC;

ACSoundManage ACSoundManage::_instance;


void ACSoundManage::load()
{

    _effectsMap[SN_ARROW_NORMAL_HIT] = "arrow_normal_hit.wav";
    _effectsMap[SN_ADD_HERO] = "add_hero.mp3";
    _effectsMap[SN_ARROW_SHOOT] = "arrow_shoot.wav";
    _effectsMap[SN_LASER_HIT] = "laser_hit.wav";
    _effectsMap[SN_GOLD_COST] = "gold_cost.mp3";
    _effectsMap[SN_CROW_CRY] = "crow_cry.wav";
    _effectsMap[SN_HIT_SHIELD] = "hit_shield.wav";


    for (auto file : _effectsMap) {

        CCLOG("preload effect %s", file.second.c_str());
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(fmt::sprintf("sounds/%s", file.second).c_str());
    }
    for (auto file : _musicMap) {
        CCLOG("preload music %s", file.second.c_str());
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fmt::sprintf("sounds/%s", file.second).c_str());

    }

}

void ACSoundManage::play(int name)
{
    if (_effectsMap.count(name)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(fmt::sprintf("sounds/%s", _effectsMap[name]).c_str());
    } else if (_musicMap.count(name)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(fmt::sprintf("sounds/%s", _musicMap[name]).c_str());
    } else {
        assert(false);
        // preload first!
    }
}