// (C) 2015 Arisecbf

#ifndef SOCommon_hpp
#define SOCommon_hpp

#include <stdio.h>
#include "cocos2d.h"
// cocos2d-x 相关的加强辅助，项目间通用，常用。



inline cocos2d::Vec2 soGenPos(const cocos2d::Vec2& pos)
{
    auto size = cocos2d::Director::getInstance()->getVisibleSize();
    return {size.width * pos.x, size.height * pos.y};
}

// 注意坐标的层次结构
inline bool soCheckSpriteContain(cocos2d::Sprite* sp, cocos2d::Vec2 point)
{
    auto center = sp->getPosition();
    cocos2d::Size size = {sp->getContentSize().width * sp->getScaleX(), sp->getContentSize().height * sp->getScaleY()};
    cocos2d::Rect rect;
    rect = {center.x - size.width/2, center.y - size.height/2, size.width, size.height};
    return rect.containsPoint(point);
}

inline cocos2d::Color3B color4b2color3b(cocos2d::Color4B c){
    return {c.r, c.g, c.b};
}

inline float vector2angel(cocos2d::Vec2 v)
{
    if (v.length() == 0) {
        return 0;
    }

    float pir = std::acos(v.x/v.length());
    if (v.y < 0) {
        pir = 2*3.1415926 - pir;
    }
    return  360-pir*(180.f/3.1415926);
}


#endif /* SOCommon_hpp */
