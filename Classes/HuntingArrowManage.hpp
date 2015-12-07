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




class HuntingArrowManage:public HuntingArrowManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(WindBarProtocal* windBarProtocal, HuntingMonsterManageProtocal* huntingMonsterManageProtocal, FlyingCrowManageProtocal* flyingCrowManageProtocal) {
        _windBarProtocal = windBarProtocal;
        _huntingMonsterManageProtocal = huntingMonsterManageProtocal;
        _flyingCrowManageProtocal = flyingCrowManageProtocal;
    }
    virtual void op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) ; //放箭

    void update(float dt);

protected:
    WindBarProtocal* _windBarProtocal;
    HuntingMonsterManageProtocal* _huntingMonsterManageProtocal;
    FlyingCrowManageProtocal* _flyingCrowManageProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    std::list<ArrowUnit> _arrowUnits;

    void dealWithUnit(ArrowUnit& unit, float dt);
    
};
#endif /* HuntingArrowManage_hpp */
