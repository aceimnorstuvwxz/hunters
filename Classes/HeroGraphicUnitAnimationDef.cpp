//
//  HeroGraphicUnitAnimationDef.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#include "HeroGraphicUnitAnimationDef.hpp"

std::unordered_map<int, std::vector<HeroGraphicAnimationFrame>> HeroGraphicAnimationDef::_configMaps[static_cast<int>(HeroGraphicType::MAX)];


std::vector<HeroGraphicAnimationFrame> HeroGraphicAnimationDef::fetchAnimationConfig(HeroGraphicType whichPart, int level)
{
    if (_configMaps[static_cast<int>(whichPart)].count(level)) {
        return _configMaps[static_cast<int>(whichPart)][level];
    } else {
        CCLOG("missing hero graphic animation config for Part=%d Level=%d, using default.", static_cast<int>(whichPart), level);
        return _configMaps[static_cast<int>(whichPart)][0]; // 0作为默认配置
    }
}

void HeroGraphicAnimationDef::loadConfig() //在启动时加载
{

    // 初始化各个身体位置的map
    for (int i = 0; i < static_cast<int>(HeroGraphicType::MAX); i++) {
        _configMaps[i] = {};
    }

    // 在这里对英雄（我）的装备图形进行动画配置
    // * 新加入缩放
    // 0级
    _configMaps[static_cast<int>(HeroGraphicType::HEAD)][0] =
    {
        {{1,13,1}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{1,13.5f,1}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(HeroGraphicType::BODY)][0] =
    {
        {{-1.f,6,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
//        {{0,6,0}, {0,0,0}, 1.f, 0.5f, 0.5f}//身体不动
    };

    _configMaps[static_cast<int>(HeroGraphicType::SWORD)][0] =
    {
        {{7.1f,20,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{7.5f,21,0}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(HeroGraphicType::SHIELD)][0] =
    {
        {{-5,9,2}, {0,0,0}, 1.1f, 0.5f, 0.5f},
        {{-5,9.5f,2}, {0,0,0}, 1.1f, 0.5f, 0.5f}
    };

}

