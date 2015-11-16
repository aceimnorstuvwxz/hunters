//
//  HeroGraphicUnitAnimationDef.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#ifndef HeroGraphicUnitAnimationDef_hpp
#define HeroGraphicUnitAnimationDef_hpp

#include <stdio.h>
#include "cocos2d.h"

//英雄的各部件不同等级动作的定义
//英雄不需要攻击，举盾等动作，只需要一个idle状态时的微动即可。

enum class HeroGraphicType:int
{

    SHIELD = 0,
    HEAD = 1,
    BODY = 2,
    SWORD = 3,
    MAX = 4
    // 没有脚，脚全程特殊处理
    // 间接了也决定了绘图的先后关系
};

inline std::string heroGraphicType2string(HeroGraphicType t)
{
    std::string r;
    switch (t) {
        case HeroGraphicType::HEAD:
            r = "HEAD";
            break;
        case HeroGraphicType::BODY:
            r = "BODY";
            break;
        case HeroGraphicType::SWORD:
            r = "SWORD";
            break;
        case HeroGraphicType::SHIELD:
            r = "SHIELD";
            break;

        default:
            assert(false);
            break;
    }
    return r;
}

struct HeroGraphicAnimationFrame
{
    // 图形 是X-Y为主的平面，Z可以用来表达前后关系，本质上是3D的，但移动和旋转都应该在XY内进行。
    cocos2d::Vec3 position; //本帧的位置
    cocos2d::Vec3 rotation; //旋转
    float scaler; //缩放
    float stayTime; //在本帧的停留时间
    float moveTime; //从上一帧到本帧的移动时间
};

class HeroGraphicAnimationDef
{
public:
    static void loadConfig(); //在启动时加载
    static std::vector<HeroGraphicAnimationFrame> fetchAnimationConfig(HeroGraphicType whichPart, int level);

protected:
    static std::unordered_map<int, std::vector<HeroGraphicAnimationFrame>> _configMaps[static_cast<int>(HeroGraphicType::MAX)];
};


#endif /* HeroGraphicUnitAnimationDef_hpp */
