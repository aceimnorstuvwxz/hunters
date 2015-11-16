//
//  RivalGraphicAnimationDef.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#include "RivalGraphicAnimationDef.hpp"
std::unordered_map<int, std::vector<RivalGraphicAnimationFrame>> RivalGraphicAnimationDef::_configMaps[static_cast<int>(RivalGraphicType::MAX)][static_cast<int>(RivalGraphicMode::MAX)]; // *注意这是一个2维数组(哪个部位，哪个状态，哪一套)

std::vector<RivalGraphicAnimationFrame> RivalGraphicAnimationDef::fetchAnimationConfig(RivalGraphicType whichPart, RivalGraphicMode whichMode, int whichRival)
{
    if (_configMaps[static_cast<int>(whichPart)][static_cast<int>(whichMode)].count(whichRival)) {
        return _configMaps[static_cast<int>(whichPart)][static_cast<int>(whichMode)][whichRival];
    } else {
        // 如果不存在，按idle给出
        return _configMaps[static_cast<int>(whichRival)][static_cast<int>(RivalGraphicMode::IDLE)][whichRival];
    }

}

void RivalGraphicAnimationDef::loadConfig() //在启动时加载
{
    // init
    for (int type = 0; type < static_cast<int>(RivalGraphicType::MAX); type++) {
        for (int mode = 0; mode < static_cast<int>(RivalGraphicMode::MAX); mode++) {
            _configMaps[type][mode] = {};
        }
    }

    // * 注意Rival的配置项中多了Scaler *
    // 0级套装
    // idle 必须完全配置
    _configMaps[static_cast<int>(RivalGraphicType::HEAD)][RivalGraphicMode::IDLE][0] =
    {
        {{-2,13,1}, {0,0,0}, 1.2f, 0.5f, 0.5f},
        {{-2,13.5f,1}, {0,0,0}, 1.2f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::BODY)][RivalGraphicMode::IDLE][0] =
    {
        {{0,6,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        //        {{0,6,0}, {0,0,0}, 0.5f, 0.5f}//身体不动
    };

    _configMaps[static_cast<int>(RivalGraphicType::SWORD)][RivalGraphicMode::IDLE][0] =
    {
        {{-6.5f,18,0}, {0,0,35}, 1.f, 0.5f, 0.5f},
        {{-6.5f,19,0}, {0,0,35}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::SHIELD)][RivalGraphicMode::IDLE][0] =
    {
        {{5,7,2}, {0,0,0}, 1.2f, 0.5f, 0.5f},
        {{5,7.5f,2}, {0,0,0}, 1.2f, 0.5f, 0.5f}
    };

    // defense 仅需要的配置，未配置的会按idle给出
    _configMaps[static_cast<int>(RivalGraphicType::HEAD)][RivalGraphicMode::DEFENSE][0] =
    {
        {{1,13,1}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{1,13.5f,1}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::BODY)][RivalGraphicMode::DEFENSE][0] =
    {
        {{-1.f,6,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        //        {{0,6,0}, {0,0,0}, 0.5f, 0.5f}//身体不动
    };

    _configMaps[static_cast<int>(RivalGraphicType::SWORD)][RivalGraphicMode::DEFENSE][0] =
    {
        {{7.1f,20,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{7.5f,21,0}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::SHIELD)][RivalGraphicMode::DEFENSE][0] =
    {
        {{-5,9,2}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{-5,9.5f,2}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    // attack 仅需要的配置，未配置的会按idle给出
    _configMaps[static_cast<int>(RivalGraphicType::HEAD)][RivalGraphicMode::ATTACK][0] =
    {
        {{1,13,1}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{1,13.5f,1}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::BODY)][RivalGraphicMode::ATTACK][0] =
    {
        {{-1.f,6,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        //        {{0,6,0}, {0,0,0}, 0.5f, 0.5f}//身体不动
    };

    _configMaps[static_cast<int>(RivalGraphicType::SWORD)][RivalGraphicMode::ATTACK][0] =
    {
        {{7.1f,20,0}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{7.5f,21,0}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };

    _configMaps[static_cast<int>(RivalGraphicType::SHIELD)][RivalGraphicMode::ATTACK][0] =
    {
        {{-5,9,2}, {0,0,0}, 1.f, 0.5f, 0.5f},
        {{-5,9.5f,2}, {0,0,0}, 1.f, 0.5f, 0.5f}
    };
}