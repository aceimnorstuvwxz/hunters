//
//  HuntingArrowManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#ifndef HuntingArrowManage_hpp
#define HuntingArrowManage_hpp

#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"
#include "HeroHeadAndUpgrade.hpp"


struct ArrowUnit
{
    PixelNode* _pxNode; //图形本体
    cocos2d::Vec2 _speed; //速度

};

class HuntingArrowManage:public HuntingArrowManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(WindBarProtocal* windBarProtocal) {
        _windBarProtocal = windBarProtocal;
    }
    virtual void op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) ; //放箭

    void update(float dt);

protected:
    WindBarProtocal* _windBarProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    std::list<ArrowUnit> _arrowUnits;

    bool dealWithUnit(ArrowUnit& unit, float dt);
    
};
#endif /* HuntingArrowManage_hpp */
