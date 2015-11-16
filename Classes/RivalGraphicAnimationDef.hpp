//
//  RivalGraphicAnimationDef.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#ifndef RivalGraphicAnimationDef_hpp
#define RivalGraphicAnimationDef_hpp

#include <stdio.h>

enum class RivalGraphicType:int
{
    SHIELD = 0,
    HEAD = 1,
    BODY = 2,
    SWORD = 3,
    MAX = 4
};

inline std::string rivalGraphicType2string(RivalGraphicType t)
{
    std::string r;
    switch (t) {
        case RivalGraphicType::HEAD:
            r = "HEAD";
            break;
        case RivalGraphicType::BODY:
            r = "BODY";
            break;
        case RivalGraphicType::SWORD:
            r = "SWORD";
            break;
        case RivalGraphicType::SHIELD:
            r = "SHIELD";
            break;

        default:
            assert(false);
            break;
    }
    return r;
}

enum RivalGraphicMode:int
{
    IDLE = 0,
    DEFENSE = 1,
    ATTACK = 2,
    MAX = 3
};

inline std::string rivalGraphicMode2string(RivalGraphicMode m)
{
    std::string r;
    switch (m) {
        case RivalGraphicMode::IDLE:
            r = "IDLE";
            break;
        case RivalGraphicMode::DEFENSE:
            r = "DEFENSE";
            break;
        case RivalGraphicMode::ATTACK:
            r = "ATTACK";
            break;

        default:
            assert(false);
            break;
    }
    return r;
}

struct RivalGraphicAnimationFrame
{
    cocos2d::Vec3 position;
    cocos2d::Vec3 rotation;
    float scaler;
    float stayTime;
    float moveTime;
};

class RivalGraphicAnimationDef
{
public:
    static void loadConfig(); //在启动时加载
    static std::vector<RivalGraphicAnimationFrame> fetchAnimationConfig(RivalGraphicType whichPart, RivalGraphicMode whichMode, int whichRival);

protected:
    static std::unordered_map<int, std::vector<RivalGraphicAnimationFrame>> _configMaps[static_cast<int>(RivalGraphicType::MAX)][static_cast<int>(RivalGraphicMode::MAX)]; // *注意这是一个2维数组(哪个部位，哪个状态，哪一套)

};

#endif /* RivalGraphicAnimationDef_hpp */
