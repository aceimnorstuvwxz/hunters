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
    _effectsMap[SN_THUNDER_HIT] = "thunder_hit.mp3";
    _effectsMap[SN_NEW_WAVE] = "new_wave.mp3";
    _effectsMap[SN_NEW_HERO] = "new_hero.mp3";
    _effectsMap[SN_HERO_UPGRADE] = "new_hero.mp3";
    _effectsMap[SN_TORNADO] = "tornado.wav";
//    _effectsMap[SN_WIND_CHANGE] = "wind_change.wav";
    _effectsMap[SN_KEY] = "key.mp3";
    _effectsMap[SN_CLICK] = "click.mp3";

    _musicMap[SN_M_0] = "m_0.mp3";
    _musicMap[SN_M_1] = "m_1.mp3";
    _musicMap[SN_M_2] = "m_2.mp3";
    _musicMap[SN_M_3] = "m_3.mp3";
    _musicMap[SN_M_WELCO] = "m_welco.mp3";
    _musicMap[SN_M_CREDITS] = "m_credits.mp3";


    for (auto file : _effectsMap) {

        CCLOG("preload effect %s", file.second.c_str());
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(fmt::sprintf("sounds/%s", file.second).c_str());
    }
    for (auto file : _musicMap) {
        CCLOG("preload music %s", file.second.c_str());
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fmt::sprintf("sounds/%s", file.second).c_str());

    }

}

void ACSoundManage::play(int name, bool loop)
{
    if (_effectsMap.count(name)) {
        _soundIdMap[name] = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(fmt::sprintf("sounds/%s", _effectsMap[name]).c_str(), loop);
    } else if (_musicMap.count(name)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(fmt::sprintf("sounds/%s", _musicMap[name]).c_str(), loop);
    } else {
        assert(false);
        // preload first!
    }
}

void ACSoundManage::stop(int SN_NAME)
{
    if (_soundIdMap.count(SN_NAME)) CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_soundIdMap[SN_NAME]);
}


bool ACSoundManage::getSoundState()
{
    bool v = UserDefault::getInstance()->getBoolForKey("cbf_sound", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(v ? 1.f : 0.f);
    return v;
}
void ACSoundManage::soundSwitch()
{
    bool old = UserDefault::getInstance()->getBoolForKey("cbf_sound", true);
    UserDefault::getInstance()->setBoolForKey("cbf_sound", !old);
    getSoundState();

}
bool ACSoundManage::getMusicState()
{
    bool v =  UserDefault::getInstance()->getBoolForKey("cbf_music", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v ? 1.f : 0.f);
    return v;
}
void ACSoundManage::musicSwitch()
{
    bool old = UserDefault::getInstance()->getBoolForKey("cbf_music", true);
    UserDefault::getInstance()->setBoolForKey("cbf_music", !old);
    getMusicState();
}